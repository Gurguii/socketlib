#!/bin/bash

# Receives 3 params - <command2run> <sucessmessage> <failmessage>
function runcommand(){
	$1 &>"$debug_file"
	if [[ $? -eq 0 ]]; then 
		printf "%s\n" "$2"
	else 
		printf "%s\n" "$3"
		exit 1
	fi
}

if [[ $EUID != 0 ]]; then
	printf "[!] - Need sudo privileges\n"
	exit 0
fi

# Script location(no matter where it's being executed from) e.g. /home/user/socketlib/scripts/install.sh
script_dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

# Debug file path(will have output in case debugging of installation failing is required/desired)
debug_file="$script_dir/.install.debug"

echo "[+] Starting gsocket installation - $(date '+%D@%R')" | tee -a "$debug_file" 

# Get a valid package manager for current system
declare -A packagemanagers
packagemanagers['apt-get']="apt-get install -y"
packagemanagers['apt']="apt install -y"
packagemanagers['yum']="yum install -y"
packagemanagers['apk']="apk add --no-cache"
packagemanagers['pacman']="pacman -S --noconfirm"
packagemanagers['zypper']="zypper install -y"
packagemanagers['dnf']="dnf install -y"

package_manager=""
for pckm in ${!packagemanagers[@]}; do
	if command -v "$pckm" &>/dev/null; then
		package_manager=${packagemanagers[$pckm]}
	fi
done

required_commands=(cmake make gcc g++)
for tool in ${required_commands[@]}; do
	if ! command -v "$tool" &>/dev/null; then 
		read -r -p "$tool is required but not installed, install? y/n " ans
		if [[ ${ans,,} == "y" || ${ans,} == yes ]]; then
			printf "Installing %s\n" "$tool" | tee -a "$debug_file"	
			# This is smth like 'apt install -y cmake'
			$package_manager $tool 1>/dev/null 2>> "$debug_file"
			if ! command -v "$tool" &>/dev/null; then 
				printf "Problem installing %s\nmore information can be found @ %s\nexiting...\n" "$tool" "$debug_file"
				exit 1
			fi
		else 
			printf "Can't continue the installation...\n"
			exit 0
		fi
	fi
done

file="$script_dir/../CMakeLists.txt"
path="$script_dir/../build"

if [[ ! -e "$path" ]]; then 
	mkdir -p "$path"
fi
cd "$path"

cmake .. &>>"$debug_file"

runcommand "cmake .." "Running 'cmake ..' - building library [OK]" "Running 'cmake ..' - building library [FAILED]" 
runcommand "make install" "Running 'make install' - installing library in the system [OK]" "Running 'make install' - installing library in the system [FAILED]" 
runcommand "ldconfig" "Running 'ldconfig' - refresh system includes [OK]" "Running 'ldconfig' - refresh system includes [FAILED]" 

printf "[+] - Done, gsocket successfully installed!\n" | tee -a "$debug_file"
