#!/bin/bash


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

function printyellow()
{
  printf "\e[93m$@\e[0m"
}

function printmagenta()
{
  printf "\e[35m$@\e[0m"
}

# Function that attempts removing given paths
# Receives a list of paths as a param - <path1> <path2> <path3> ...
function delete(){
	for i in "$@"; do
		rm -rf $i
		if [[ $? -eq 0 ]]; then
			printgreen "[OK] "; printf "Removing %s\n" "$i" | tee -a "$log_file"
		else
			printred "[FAIL] "; printf "Removing %s\n" "$i" | tee -a "$log_file"
			exit 1
		fi
done
}

# Check for sudo privileges (required to avoid permission problems)
if (( $EUID != 0 )); then
	printf "[!] - Need sudo privileges!\n"
	exit 1
fi

# Holds scripts' absolute path
script_dir=$(cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

installmanifest="$script_dir/../build/install_manifest.txt"
builddir="$script_dir/../build"
log_file="$script_dir/logs/uninstall.log"

# Check that .installpaths file exists
if ! [[ -e "$installmanifest" ]]; then
	printf "Can't find '%s', exiting...\n" "$installmanifest"
	exit 1
fi

printcyan "[+] Starting gsocket removal - $(date '+%d-%m-%Y at %H:%M')\n" | tee -a "$log_file"

if [[ -z "$installmanifest" || -z "$builddir" ]]; then
	printf "missing build/manifest_install.txt build directory itself\n"
	exit 1
fi

# Delete file installed by CMakeLists.txt
while read path; do
	delete "$path"
done < "$installmanifest"

# Delete /usr/local/include/gsocket
if [ -e "$builddir/gsocket_root_include_dir.txt" ]; then
	source "$builddir/gsocket_root_include_dir.txt"
	delete "$gsocket_root_include_dir"
fi

# Delete build dir
delete "$builddir"
