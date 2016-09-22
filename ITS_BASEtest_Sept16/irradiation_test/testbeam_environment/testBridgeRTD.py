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

def testBridgeRTD(output):
    print "\n\n\nINFO: Executing Resistance to Digital Scan Test"

    # open FTDI device
    commRDO.openRDO()

    # set global header 6d000007
    header = [int("07",16),int("00",16), int("00",16), int("6d",16)]
    with open(output,"ab") as f:
        f.write(bytearray(header))
   
    # Enable RTD
    # 28 01 80 C0
    ftdIO.writeReg(192,0x0128)
    ftdIO.writeReg(256,0xC080)
    ftdIO.writeReg(128,0x0403)

    # -> INCREASE THE RESISTANCE BY 5 OHM AND REPEAT THE MEASUREMENT TO COVER RESISTANCE VALUES 100, 105, ..., 150 OHM (11 STEPS)
    scanstart = 0
    scanstop = 21
    scanstep = 1
    reskohm = 0
    stepdata = 256
    for reskohm in range(scanstart, scanstop, scanstep):
        stepdata = stepdata - 1
        print "new res setting - kOhm, setting, hex(setting): " + str(reskohm), stepdata, hex(stepdata)
        
        print ".................................................................", reskohm
        # -> SET THE 1K DIGITAL POTENTIOMETERS ON THE ADC BOARD TO 100 OHM DP_1K (AD5254, CH 0)
        proc.writeAD5254_adcboard("CMD0",hex(stepdata),"0")
        
        sleep(0.1)

        # read 4 times
        for i in range(0,4,1):
            channeldata = ((2**(i) << 8) & 0xFF00) + 0x28

            # Read the least significant part of the resistance value

            # read from RTD to bridge
            ftdIO.writeReg(192,channeldata)
            ftdIO.writeReg(256,0x0002)
            ftdIO.writeReg(128,0x0403)
            # Wait for data to be ready
            # read from bridge to Firmware
            ftdIO.writeReg(192,0x0028)
            ftdIO.writeReg(128,0x0408)
            # Wait for data to be ready
            sleep(0.1)
            # write to file
            ftdIO.readFifoToFile(output,3)

            # Read the most significant part of the resistance value

            # read from RTD to bridge
            ftdIO.writeReg(192,channeldata)
            ftdIO.writeReg(256,0x0001)
            ftdIO.writeReg(128,0x0403)
            # Wait for data to be ready
            # read from bridge to Firmware
            ftdIO.writeReg(192,0x0028)
            ftdIO.writeReg(128,0x0408)
            # Wait for data to be ready
            sleep(0.1)
            # write to file
            ftdIO.readFifoToFile(output,3)
            print "............................................................", i
           

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
    testBridgeRTD(output)
