#!/bin/bash

[ ! -z $MODULESHOME ] && module load root

. ./setup.sh

echo $LBLTBGUI

sfiles="online_board1_dose0_2016-04-11_17-28-38.log online_board1_dose0_2016-04-11_17-25-27.log online_board1_dose0_2016-04-11_15-47-07.log offline_board1_dose0_2016-04-11_17-26-07.log offline_board1_dose0_2016-04-11_15-41-25.log"

for fn in $sfiles
do
	fns="$THISDIR/data/$fn"
	echo "[i] scan file: $fns"
	for h in 0xc3000009 0xaf000002 0xc1000005 0x0d000004 0xcd000003 0xcd000013
	do
		echo "[i] checking for $h"
		hex32bit $fns | grep "$h"
	done
	echo
done