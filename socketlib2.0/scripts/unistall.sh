if [[ $EUID != 0 ]]; then
	echo "[!] - Need sudo privileges!"
	exit 0
fi
rm -rf /usr/local/include/gsocket /usr/local/lib/libgsock*.so ./build/* ../build/* 1>/dev/null
