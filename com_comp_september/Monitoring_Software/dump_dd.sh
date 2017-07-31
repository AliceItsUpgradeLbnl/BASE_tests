#!/bin/bash

fname=$1
if [ -f "$fname" ]; then

	dd if=$fname bs=4 2>/dev/null | hexdump -v -e '"%08_ax: "' -e ' 4/4 "%08x " " |"' -e '16/1 "%_p" "|\n"'

fi