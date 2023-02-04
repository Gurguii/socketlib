#!/bin/bash
if [[ $EUID != 0 ]]; then
	echo "[!] - Need sudo privileges!"
fi
bash unistall.sh && bash install.sh
