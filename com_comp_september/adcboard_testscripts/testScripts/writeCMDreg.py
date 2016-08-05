#!/usr/bin/env python
__author__ = "G. Contin"
__version__ = "1.0"
__status__ = "Prototype"

import ftdIO
from time import strftime, sleep
import sys
import commRDO
import os

path = os.getcwd() + "/tables/"

def writeCMDreg(devName,opName):
    
    l = []
    header = []
    newline = "timestamp_"
    output = open(path+"output.log",'w')
    found = 0
    with open(path+"cmdReg.dat",'r') as f:
        header = next(f).split()
        
        for line in f:
            l = line.split()
            if ((l[0]==devName)&(l[1]==opName)):
                found = 1
                if len(l)!=7:
                    print "ERROR: CMD table line is incomplete!"
                    exit(-1)
                #print l[0], l[1], l[2], l[3], l[4], l[5], l[6]
                cmd = int((int(l[2])<<11)|(int(l[3])<<8)|(int(l[4])<<4)|(int(l[5])<<2)|(int(l[6])<<0))
                print "INFO: Executing operation ", l[1]," on device", l[0],":", hex(cmd)    
            
                # write CMD register (128) into FPGA
                ftdIO.writeReg(128,cmd)
                sleep(0.2)
                if found == 1:
                    print "INFO: register has been written"
        if found == 0:
            print "ERROR: no line found in cmdReg.dat"
            exit(-1)

        #outReg = ftdIO.readReg(128)
        #if (outReg == cmd):
        #     test = "passed"
        #else:
        #     test = "failed"
        #     print "WARNING: unexpected register content:", hex(outReg), "instead of", hex(cmd)

        
        
if __name__ == "__main__":
    if(len(sys.argv) != 3):
        print "ERROR: devName and opName need to be specified - Stopping"
        exit(-1)
    devName = sys.argv[1]
    opName = sys.argv[2]


    # open FTDI device by serial number
    commRDO.openRDO()
    writeCMDreg(devName, opName)
    commRDO.closeRDO()
