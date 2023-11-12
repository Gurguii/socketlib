#!/bin/bash

# Receives 3 params - <command2run> <sucessmessage> <failmessage>
function runcommand(){
	$1 &>/dev/null
	if [[ $? -eq 0 ]]; then 
		printgreen "[OK] "; printf "%s\n" "$2" | tee -a "$log_file"
	else 
		printred "[FAIL] "; printf "%s\n" "$2" | tee -a "$log_file"
		exit 1
	fi
}

function printred()
{
  printf "\e[91m$@\e[0m"
}

function printgreen()
{
  printf "\e[92m$@\e[0m"
}

function printcyan()
{
  printf "\e[36m$@\e[0m"
}

if [[ $EUID != 0 ]]; then
	printf "[!] - Need sudo privileges\n"
	exit 0
fi

# Script location(no matter where it's being executed from) e.g. /home/user/socketlib/scripts/install.sh
script_dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

# Debug file path(will have output in case debugging of installation failing is required/desired)
log_file="$script_dir/logs/install.log"
printcyan "[+] Starting gsocket installation - $(date '+%d-%m-%Y at %H:%M')\n" | tee -a "$log_file" 

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
			printf "Installing %s\n" "$tool" | tee -a "$log_file"	
			# This is smth like 'apt install -y cmake'
			$package_manager $tool 1>/dev/null 2>> "$log_file"
			if ! command -v "$tool" &>/dev/null; then 
				printf "Problem installing %s\nmore information can be found @ %s\nexiting...\n" "$tool" "$log_file"
				exit 1
			fi
		else 
			printf "Can't continue the installation...\n"
			exit 1
		fi
	fi
done

file="$script_dir/../CMakeLists.txt"
path="$script_dir/../build"

if [[ ! -e "$path" ]]; then 
	mkdir -p "$path"
fi
cd "$path"

cmake .. &>/dev/null 

runcommand "cmake .." "Running 'cmake ..' - building library" 
runcommand "make install" "Running 'make install' - installing library in the system" 
runcommand "ldconfig" "Running 'ldconfig' - refresh system includes " 

if ! ldconfig -p | grep "libgsocket" &>/dev/null; then
	printred "WARNING"
	read -rp " libgsocket not detected in 'ldconfig -v' what means the library won't be detected during runtime, fix? y/n " ans
	if [[ ${ans,,} == "y" || ${ans,,} == "yes" ]]; then
		printf "/usr/local/lib" > "/etc/ld.so.conf.d/gsocket.conf"
		runcommand "ldconfig" "Running 'ldconfig' - refreshing system includes" 
	fi
fi

printcyan "[+] - Done, gsocket successfully installed!\n\n" | tee -a "$log_file"
