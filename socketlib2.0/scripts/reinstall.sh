#!/bin/bash
if [[ $EUID != 0 ]]; then
	echo "[!] - Need sudo privileges!"
fi
i=/install.sh
u=/unistall.sh
if [[ -e ./$u ]]; then
	bash ./$u && bash ./$i
elif  [[ -e ./scripts/$u ]]; then
	bash ./scripts/$u && bash ./scripts/$i
else
	echo "[!] - Can't find unistall/install scripts, please to go project root"
	exit 0
fi
