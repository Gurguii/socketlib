#!/bin/bash
if [[ $EUID != 0 ]]; then
	echo "[!] - Need sudo privileges"
	exit 0
fi

path=""
file="CMakeLists.txt"

if [[ -e $file ]]; then
	path="./build"
	if [[ ! -e "build" ]]; then
		mkdir "./build"
	fi
elif [[ -e "../$file" ]]; then
	path="../build"
	if [[ ! -e "../build" ]]; then
		mkdir "../build"
	fi
else
	echo "[!] Can't find $file, please stay in the 'CMakeLists.txt' directory"
	exit 0
fi

cd "$path"
cmake .. && make install && ldconfig
echo "[+] - Done, gsocket successfully installed!\n"