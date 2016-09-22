#!/usr/bin/env python
__author__ = "G. Contin"
__version__ = "1.0"
__status__ = "Prototype"

from time import sleep
import commRDO
import proc
import procDUT
import writeCMDreg
import ftdIO
import sys

def testDiff2SE(output):
    print "\n\n\nINFO: Executing Differential to Single ended Test"

    # open FTDI device
    commRDO.openRDO()

    # set global header cd000003
    header = [int("03",16),int("00",16), int("00",16), int("cd",16)]
    with open(output,"ab") as f:
        f.write(bytearray(header))
    

    for freqscale in range(0,32,1):
        stepfreq = (freqscale << 11) & 0xF800 
        print "stepfreq", stepfreq
        
        # Start the differential IO checker in the firmware
        startIO = 0x0700
        targetcmd = stepfreq + startIO
        print "Main command reg:", targetcmd, str(targetcmd), hex(targetcmd)
        ftdIO.writeReg(128,targetcmd)
        sleep(0.5)
        # Read from Fifo - 10 words
        # write to file
        ftdIO.readFifoToFile(output,10)
    


    # set global trailer 'fadecafe'    
    trailer = [int("fe",16),int("ca",16),int("de",16),int("fa",16)]
    with open(output,"ab") as f:
            f.write(bytearray(trailer))

   # close FTDI device
    commRDO.closeRDO()

if __name__ == "__main__":
    if(len(sys.argv) != 2):
        print "ERROR: output file not set - Stopping"
        exit(-1)
    else:
        output = sys.argv[1]
    # execute script
    testDiff2SE(output)
