#!/bin/bash

script_dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

if [[ $EUID != 0 ]]; then
	echo "[!] - Need sudo privileges!"
	exit 0
fi

installscript="$script_dir"/install.sh
uninstallscript="$script_dir"/uninstall.sh
scripts=("$installscript" "$uninstallscript")

for file in "$scripts"; do
	if [[ -e ./$file ]]; then
		printf "[!] - Can't find "$file"\n"
		exit 1
	fi
done

bash "$uninstallscript"
if (( $? )); then
	# uninstall script failed
	exit 1
fi
bash "$installscript"
