#!/bin/bash

debug_file="$script_dir/.install.debug"
echo "Starting gsocket installation - "$(date '+%D@%R')" " | tee -a "$debug_file" 

if [[ $EUID != 0 ]]; then
	printf "[!] - Need sudo privileges\n"
	exit 0
fi

declare -A packagemanagers
packagemanagers['apt-get']="apt-get install -y"
packagemanagers['apt']="apt install -y"
packagemanagers['yum']="yum install -y"
packagemanagers['apk']="apk add --no-cache"
packagemanagers['pacman']="pacman -S --noconfirm"
packagemanagers['zypper']="zypper install -y"
packagemanagers['dnf']="dnf install -y"

for pckm in ${!packagemanagers[@]}; do
	if command -v "$pckm" &>/dev/null; then
		package_manager=${packagemanagers[$pckm]}
	fi
done

script_dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

required_commands=(cmake make gcc g++)
for tool in ${required_commands[@]}; do
	if ! command -v "$tool" &>/dev/null; then 
		read -r -p "$tool is required but not installed, install? y/n " ans
		if [[ ${ans,,} == "y" || ${ans,} == yes ]]; then
			printf "Installing %s\n" "$tool" | tee -a "$debug_file"	
			# This is smth like 'apt install -y cmake'
			$package_manager $tool 1> /dev/null 2>> "$debug_file"
			if ! command -v "$tool"; then 
				printf "Problem information installing %s\nmore information can be found @ %s\nexiting...\n" "$tool" "$debug_file"
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

printf "Running 'cmake ..' - building library []\r" 
cmake .. 1>/dev/null 2>>"$debug_file" 
if [[ $? -eq 0 ]]; then
	printf "Running 'cmake ..' - building library [OK]"
fi

printf "\nRunning 'make install' - installing library in the system []\r"
make install 1>/dev/null 2>>"$debug_file"
if [[ $? -eq 0 ]]; then 
	printf "Running 'make install' installing library in the system [OK]"
fi

printf "\nRunning 'ldconfig' - refresh system includes []\r"
ldconfig 1>/dev/null 2>>"$debug_file"
if [[ $? -eq 0 ]]; then 
	printf "Running 'ldconfig' - refresh system includes [OK]"
fi
printf "\n[+] - Done, gsocket successfully installed!\n"
