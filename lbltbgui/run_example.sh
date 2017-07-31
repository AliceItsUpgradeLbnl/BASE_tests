#!/bin/bash


[ ! -z $MODULESHOME ] && module load root 

. ./setup.sh

# show the bytes in a file with a 4 byte words
# cat ./data/test3.log | ./bin/hex

# ./bit/hex32bit ./data/test3.log

#./bin/testTBGui --gr ./data/test3.log --gr setup.sh &
#./bin/testTBGui --gr ./data/test3.log &

#./bin/testTBGui \
#	--tb ./data/online_board1_dose0_2016-04-11_15-47-07.log \
#	--tb ./data/online_board1_dose0_2016-04-11_17-25-27.log \
#	--tb ./data/online_board1_dose0_2016-04-11_17-28-38.log \
#	--head 0xc3000009 \
#	--ch 0 --ch 15 --ch 3 --ch 5 --ch 2 --ch 17

./bin/testTBGui \
	--tb ./data/online_board1_dose0_2016-04-11_15-47-07.log \
	--head 0xc3000009 \
	--ch 0 --ch 1 --ch 2 --ch 3 --ch 4 --ch 10 --ch 23 &

./bin/testTBGui \
	--tb ./data/offline_board1_dose0_2016-04-11_15-41-25.log \
	--head 0xAF000002 \
	--ch 0 --ch 1 --ch 16 --ch 17 --ch 19 &

./bin/testTBGui \
	--tb ./data/offline_board1_dose0_2016-04-11_15-41-25.log \
	--head 0xc1000005 \
	--ch 0 --ch 1 --ch 2 &

./bin/testTBGui \
	--tb ./data/2016-04-17/offline_board1_dose0_2016-04-17_17-19-11.log \
	--tail 0 \
	--offline \
	--head 0xcd000013 \
	--ch 7 --ch 8 &

./bin/testTBGui \
	--tb ./data/2016-04-17/offline_board1_dose0_2016-04-17_17-19-11.log \
	--tail 0 \
	--offline \
	--head 0xcd000003 \
	--ch 7 --ch 8 &

./bin/testTBGui \
	--tb ./data/2016-04-17/online_board1_dose0_2016-04-17_17-20-59.log \
	--head 0xc3000009 \
	--ch 0 --ch 1 --ch 2 --ch 3 --ch 4 --ch 10 --ch 23 &
