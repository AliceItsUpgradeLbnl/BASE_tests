#!/bin/bash

[ ! -z $MODULESHOME ] && module load root 

. ./setup.sh

testfile="test.log"
rm -f $testfile
touch $testfile

testfile1="test1.log"
rm -f $testfile1
touch $testfile1

./bin/testTBGui \
	--tb $testfile \
	--head 0xAF000002 \
	--ch 7 --ch 8 --ch 9 --ch 10 --ch 11 \
	&

./bin/testTBGui \
	--tx $testfile1 \
	--gr $testfile1 \
	--tail 100 \
	&

for ((;;))
do
	echo "[i] cat to $testfile"
	#cat ./data/online_board1_dose0_2016-04-11_15-47-07.log >> $testfile
	cat ./data/offline_board1_dose0_2016-04-11_15-41-25.log >> $testfile
	#cat ./usethis.dat >> $testfile
	date >> $testfile1
	wc -l $testfile1 >> $testfile1
	sleep 5	
done
