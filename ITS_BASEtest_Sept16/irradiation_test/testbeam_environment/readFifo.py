#!/usr/bin/env python
__author__ = "G. Contin"
__version__ = "1.0"
__status__ = "Prototype"

# read C object, like ftdIO

import ftdIO
import commRDO
import sys


if __name__ == "__main__":
    if(len(sys.argv) != 3):
        print "ERROR: output file and size not set - Stopping"
        exit(-1)
    else:
        output = sys.argv[1]
        numofwords = sys.argv[2]
    commRDO.openRDO()
    print output, int(numofwords)
    ftdIO.readFifoToFile(output,int(numofwords))
    commRDO.closeRDO()

