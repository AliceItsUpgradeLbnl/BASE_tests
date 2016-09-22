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

def testI2Cdriver(output):
    print "\n\n\nINFO: Executing I2C driver test"
 
    # open FTDI device
    commRDO.openRDO()

    # set global header cd00000b
    header = [int("0b",16),int("00",16), int("00",16), int("cd",16)]
    with open(output,"ab") as f:
        f.write(bytearray(header))
    
    stepdata5 = 0x55
    stepdataA = 0xAA
    for i in range(10):
        # Write pattern 0x55 to 8 bit register of TCA9535
        procDUT.writeTCA9535_dutboard("TCA9535","6","CMD2","0x55","0","2") 
        # Wait
        sleep(0.1) 
        # Read 8 bit register of TCA9535      
        procDUT.writeTCA9535_dutboard("TCA9535","6","CMD6","0x00","0","5")   
        # Wait for data to be ready
        sleep(0.1)
        # write to file
        ftdIO.readFifoToFile(output,3)
  
        # Write pattern 0xFF to 8 bit register of TCA9535
        procDUT.writeTCA9535_dutboard("TCA9535","6","CMD2","0xAA","0","2")
        # Wait
        sleep(0.1) 
        # Read 8 bit register of TCA9535      
        procDUT.writeTCA9535_dutboard("TCA9535","6","CMD6","0x00","0","5")   
        # Wait for data to be ready
        sleep(0.1)
        # write to file
        ftdIO.readFifoToFile(output,3)

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
    testI2Cdriver(output)

