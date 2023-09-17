#!/bin/bash

# Function that attempts removing given paths
# Receives a list of paths as a param - <path1> <path2> <path3> ...
function delete(){
	for i in "$@"; do
		rm -rf $i
		if [[ $? -eq 0 ]]; then
			printf "Removing "$i" [OK]\n" | tee -a "$debug_file"
		else
			printf "Removing "$i" [FAILED]\n" | tee -a "$debug_file"
			exit 1
		fi
	done
}
# Check for sudo privileges (required to avoid permission problems)
if [[ $EUID != 0 ]]; then
	printf "[!] - Need sudo privileges!\n"
	exit 1
fi

# Holds scripts' absolute path
script_dir=$(cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
installpathsfile="$script_dir/.installpaths"
debug_file="$script_dir/.uninstall.debug"

# Check that .installpaths file exists
if ! [[ -e "$installpathsfile" ]]; then
	printf "Can't find '%s', exiting...\n" "$installpathsfile"
	exit 1
fi

printf "[+] Starting gsocket removal - %s\n" "$(date '+%D@%R')" | tee -a "$debug_file"

# Include .installpaths (contains absolute paths of installed files)
source "$installpathsfile"

# Delete call to remove everything installed 
delete "$includedir" "$libdir" "$builddir" 
