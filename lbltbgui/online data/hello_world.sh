#!/bin/bash

rm ../testdata.dat
touch ../testdata.dat

#while true
#do
#    > ../testdata
#    cat online_board10_complete.dat >> ../testdata.dat
#    echo "Hello World"
#   cat ../testdata.dat | wc -l
#    sleep 2
#done

for i in {1..1000}
    do
    echo $i
    if [[ $((i % 2)) == 0 ]];
	then cat  online_board10_dose50_2016-04-18_23-57-42.dat >> ../testdata.dat;
	else cat  online_board1_dose20_2016-04-19_01-52-57.dat  >> ../testdata.dat;
    fi
    sleep 4
    done