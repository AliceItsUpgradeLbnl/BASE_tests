#!/usr/bin/env python
__author__ = "G. Contin"
__version__ = "1.0"
__status__ = "Prototype"

import ftdIO
from time import sleep
import sys

def openRDO():
    # open FTDI device by serial number
    ftdimap = open('ftdimap')
    for line in ftdimap:
        ftdiID=line.split()[2]
    # print "serial number:", ftdiID
    ftdIO.open_ftdi(ftdiID)

def closeRDO():
    # close FTDI again
    ftdIO.close_ftdi()

def checkFWID():
    fwID = ftdIO.readReg(0)
    print"ftdiID =", ftdiID, ", FirmwareID =",fwID


if __name__ == "__main__":
    print "INFO: RDO Communication test"
    openRDO()
    fwID = ftdIO.readReg(0)
    print "FirmwareID =",fwID
    sleep(1)
    closeRDO()
    
