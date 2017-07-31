#!/bin/bash

#module load root

. ./setup.sh

# show the bytes in a file with a 4 byte words
# cat ./data/test3.log | ./bin/hex

./bin/testTBGui --gr ./data/test3.log --gr setup.sh &


