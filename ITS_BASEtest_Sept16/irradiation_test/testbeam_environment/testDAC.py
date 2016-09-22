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

def testDAC(output):
    print "\n\n\nINFO: Executing DAC DC Scan"

    # open FTDI device
    commRDO.openRDO() 

    # write global header '0d000004' to file
    header = [int("04",16),int("00",16), int("00",16), int("0d",16)]
    with open(output,"ab") as f:
        f.write(bytearray(header))

    # a) Read the DAC outputs on the DUT board 10 times consecutively:
    #    i) Can read all the ADC on the ADC board 10 times consecutively (10 operations).

    for i in range(0,10):
        print "read loop %d" % (i)
        writeCMDreg.writeCMDreg("SPIadc","start_adc_ch_31-0")
        # write to file
        ftdIO.readFifoToFile(output,14)  

    scanstart = 0
    scanstop = 2501
    scanstep = 125
    stepdata = 0
    mVolts = 0 

    for mVolts in range(scanstart, scanstop, scanstep):
        stepdata = int(mVolts*1.6)
        print "new mV setting:", mVolts, stepdata
        for channel in range(0,8,1):
            # write_dutboard(device,ndevice,cmd,data,target,nbytes):
            stepcommand = "CMD" + str(channel)
            # write_dutboard(device,ndevice,cmd,data,target,nbytes):
            procDUT.write_dutboard("AD5671R","4",str(stepcommand),hex(stepdata),"0","3")  

        sleep(0.1)

        for i in range(0,10):
            print "read loop %d" % (i)
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
    testDAC(output)
