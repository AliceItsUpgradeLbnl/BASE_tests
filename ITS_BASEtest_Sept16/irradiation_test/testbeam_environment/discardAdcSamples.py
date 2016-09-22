#!/usr/bin/env python
__author__ = "A. Collu"
__version__ = "1.0"
__status__ = "Prototype"

from time import sleep
import commRDO
import proc
import writeCMDreg
import ftdIO
import sys
import procDUT

def discardAdcSamples():
    print "\n\n\nINFO: Discarding first ADC samples after power on"

    trashfile = "data/trashfile.log"
    
    # open FTDI device
    commRDO.openRDO()
    
    print "discarding samples from ADC board adcs"
    writeCMDreg.writeCMDreg("SPIadc","start_adc_ch_31-0")
    # write to file
    ftdIO.readFifoToFile(trashfile,14)

    procDUT.write_dutboard("ADS7830","4","CMD0", "0x8C","0","1")

    sleep(0.05) 

    procDUT.write_dutboard("ADS7830","4","CMD1","0x00","0","4")

    ftdIO.readFifoToFile(trashfile,3)

    # close FTDI device
    commRDO.closeRDO()

if __name__ == "__main__":
    if(len(sys.argv) != 2):
        print "ERROR: output file not set - Stopping"
        exit(-1)
    else:
        output = sys.argv[1]
    # execute script
    testAmplifiers(output)

    
