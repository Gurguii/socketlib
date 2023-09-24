#!/bin/bash

# Function that attempts removing given paths
# Receives a list of paths as a param - <path1> <path2> <path3> ...
function delete(){
	for i in "$@"; do
		rm -rf $i
		if [[ $? -eq 0 ]]; then
			printf "Removing "$i" [OK]\n" | tee -a "$log_file"
		else
			printf "Removing "$i" [FAILED]\n" | tee -a "$log_file"
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
log_file="$script_dir/logs/uninstall.log"

# Check that .installpaths file exists
if ! [[ -e "$installpathsfile" ]]; then
	printf "Can't find '%s', exiting...\n" "$installpathsfile"
	exit 1
fi

printf "[+] Starting gsocket removal - %s\n" "$(date '+%D@%R')" | tee -a "$log_file"

# Include .installpaths (contains absolute paths of installed files)
source "$installpathsfile"

if [[ -z $libdir || -z $includedir || -z $builddir ]]; then
	printf "missing some path in $installpathsfile\n"
	exit 1
fi

# Delete call to remove everything installed
for path in ${libdir[@]}; do
	delete "$path"
done
delete "$includedir"
delete "$builddir"
delete "$installpathsfile"
