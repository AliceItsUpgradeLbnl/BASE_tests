#!/usr/bin/python

import sys,getopt


filename = None
blocksize = 1024

def read_binaryFile(filename,blocksize):
	offset = 0
	with open(filename,"rb") as f:
		block = f.read(int(blocksize))
		i = 0
		str = ""
		for ch in block:
			if (i < 3):
				str += hex(ord(ch))+" "
				i = i + 1
			else:
				str += hex(ord(ch))+"\n"
				i = 0
		print str


if __name__ == "__main__":
    if(len(sys.argv) != 3):
        print "ERROR: file and size not set - Stopping"
        exit(-1)
    else:
        filename = sys.argv[1]
	blocksize = sys.argv[2]
    # execute script
    print "INFO: Reading the first",blocksize,"bytes of file", filename
    read_binaryFile(filename,blocksize)
