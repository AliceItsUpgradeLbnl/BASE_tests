#!/bin/bash


[ ! -z $MODULESHOME ] && module load root 

. ./setup.sh

./build.sh

./bin/testTBGui \
        --tb testdata.dat\
	--head 0xC3000009 \
	--ch 7 --ch 8 --ch 9 --ch 10 --ch 11 \