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

def testADCs(output):
    print "\n\n\nINFO: Executing ADC Test"

    scanstart = 0
    scanstop = 2501
    scanstep = 250
    stepdata = 0
    mVolts = 0

    # open FTDI device
    commRDO.openRDO()     

    # set global header ad000008
    header = [int("08",16),int("00",16), int("00",16), int("ad",16)]
    with open(output,"ab") as f:
        f.write(bytearray(header))

    for mVolts in range(scanstart,scanstop, scanstep):
        stepdata = int(mVolts*1.6)
        print "new mV setting:", mVolts, hex(stepdata)

        # Set input voltages to 0V
        proc.writeAD5624_adcboard("CMD3",hex(stepdata),"0")
        proc.writeAD5624_adcboard("CMD0",hex(stepdata),"1")
        proc.writeAD5624_adcboard("CMD1",hex(stepdata),"1")

        sleep(0.5)

        # Setting up ADC128D818 (doing it now to separate it in time from the read ops):
        procDUT.writeADC128D818_dutboard("ADC128D818","4","CMD1","0x02","0","2")
        procDUT.writeADC128D818_dutboard("ADC128D818","4","CMD0","0x01","0","2")

        sleep(0.5)

        # Read voltages from the 8 channels of ADS7830 (one by one, no broadcast command):
        # for comparison write_dutboard(device,ndevice,cmd,data,target,nbytes):
        for channel in range(0,8,1):
            stepdata = 0x8C + (0x10 * channel)
            print "Step data here", hex(stepdata)
            procDUT.write_dutboard("ADS7830","4","CMD0", hex(stepdata),"0","1")

            sleep(0.05) 

            print "Reading ADS7830, channel:", channel, hex(stepdata)
            procDUT.write_dutboard("ADS7830","4","CMD1","0x00","0","4")

            sleep(0.05) 

            # write to file
            ftdIO.readFifoToFile(output,3)

        sleep(0.5)

        # Read voltages from 8 channels of ADC128D818 (one by one, no broadcast command,
        for channel in range(0,8,1):
            stepcommand = "CMD" + str(channel+2)
            print "Reading ADC128D818, channel:", channel, str(stepcommand)
            procDUT.writeADC128D818_dutboard("ADC128D818","4", str(stepcommand),"0x00","0","1")

            sleep(0.05)            

            stepcommand = "CMDA"
            procDUT.writeADC128D818_dutboard("ADC128D818","4", str(stepcommand),"0x00","0","8")

            sleep(0.05)

            # write to file
            ftdIO.readFifoToFile(output,3)
    
        # Read voltages from 8 channels of AD7997 (one by one, no broadcast command):
        for channel in range(0,8,1):

            stepdata = (((0x8 + channel) << 4) & 0xFF)
            print "Reading AD7997, channel:", channel, hex(stepdata)
            procDUT.writeAD7997_dutboard("AD7997","4","CMD5",hex(stepdata),"0","9")

            sleep(0.05) 

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
    testADCs(output)
