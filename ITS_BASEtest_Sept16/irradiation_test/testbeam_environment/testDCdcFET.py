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

def testDCdcFET(output):
    print "\n\n\nINFO: Executing DC-DC Conv and FET Switches"

    # open FTDI device
    commRDO.openRDO()

    # set global header af00000c
    header = [int("0c",16),int("00",16), int("00",16), int("af",16)]
    with open(output,"ab") as f:
        f.write(bytearray(header))

    # Disable the DC-DC converters and FET switches --> 0
    ftdIO.writeReg(320,0x0)

    sleep(2)
    
    for i in range(0,10):
        print "Read all the ADCs, iteration", i
        # -> READ ALL THE ADC-BOARD ADCS
        # readI2C(devName,OpName)
        writeCMDreg.writeCMDreg("SPIadc","start_adc_ch_31-0")
        # write to file
        ftdIO.readFifoToFile(output,14)

    # Enable the DC-DC converters and FET switches --> 1
    ftdIO.writeReg(320,0x2)

    sleep(2)

    for i in range(0,10):
        print "Read all the ADCs, iteration", i
        # -> READ ALL THE ADC-BOARD ADCS
        # readI2C(devName,OpName)
        writeCMDreg.writeCMDreg("SPIadc","start_adc_ch_31-0")
        # write to file
        ftdIO.readFifoToFile(output,14)
    
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
    testDCdcFET(output)
