#!/usr/bin/env python
__author__ = "G. Contin"
__version__ = "2.0"
__status__ = "Prototype"

from time import sleep
import commRDO
import proc
import writeCMDreg
import ftdIO
import sys

def testComparator(output):
    print "\n\n\nINFO: Executing Comparator DC Test"

    scanstart = 0
    scanstop = 2501
    scanstep = 250
    stepdata = 0
    mVolts = 0
    out = 0
    barout = 0

    # open FTDI device
    commRDO.openRDO()

    # write global header 'c1000005' to file
    header = [int("05",16),int("00",16), int("00",16), int("c1",16)]
    with open(output,"ab") as f:
        f.write(bytearray(header))

    # a) Set the following DAC channels to the values:
    #  DAC0-0 TO 0V (ADCMP603BCPZ_P_IN)
    #  DAC0-1 TO 1.25V (ADCMP603BCPZ_N_IN)
    print "new mV setting: 1.25V for DAC0-1"
    proc.writeAD5624_adcboard("CMD1","0x800","0")
 
    # b) Set high comparator latch enable input using firmware output register
    #   i) Set BIT0 of firmware output register to 1
    ftdIO.writeReg(320,0x1)

    # -> INCREMENT DAC VOLTAGES ACCORDING TO THE FOLLOWING:
    #  DAC0-0 OF 250mV
    #  KEEP DAC0-1 TO 1.25V
    print "beginning scan"

    scanstart = 0
    scanstop = 2501
    scanstep = 250
    stepdata = 0
    mVolts = 0
    out = 0
    barout = 0
   
    for mVolts in range(scanstart, scanstop, scanstep):
        # -> INCREMENT DAC VOLTAGES
        stepdata = int(mVolts*1.6)
        print "new mV setting:", mVolts, "for DAC0-0"
        proc.writeAD5624_adcboard("CMD0",hex(stepdata),"0")

        sleep(0.1)

        # -> READ COMPARATOR OUTPUTS USING FIRMWARE GPIO INPUT REGISTER:
        #  ADCMP603_Q_OUT (firmware input register bit 0)
        #  ADCMP603_Q_BAR_OUT (firmware input register bit 1)    
        out = ftdIO.readReg(64) & 0x3
        print "Comparator output:", out

        # write to file
        wordout = [out,int("00",16), int("00",16), int("00",16)]
        with open(output,"ab") as f:
            f.write(bytearray(wordout))

    # b) Set high comparator latch enable input using firmware output register
    #   i) Set BIT0 of firmware output register to 0
    ftdIO.writeReg(320,0x0)

    # -> INCREMENT DAC VOLTAGES ACCORDING TO THE FOLLOWING:
    #  DAC0-0 OF 250mV
    #  KEEP DAC0-1 TO 1.25V
    print "beginning scan"
   
    for mVolts in range(scanstart, scanstop, scanstep):
        # -> INCREMENT DAC VOLTAGES
        stepdata = int(mVolts*1.6)
        print "new mV setting:", mVolts, "for DAC0-0"
        proc.writeAD5624_adcboard("CMD0",hex(stepdata),"0")

        sleep(0.1)

        # -> READ COMPARATOR OUTPUTS USING FIRMWARE GPIO INPUT REGISTER:
        #  ADCMP603_Q_OUT (firmware input register bit 0)
        #  ADCMP603_Q_BAR_OUT (firmware input register bit 1)    
        out = ftdIO.readReg(64) & 0x3
        print "Comparator output:", out

        # write to file
        wordout = [out,int("00",16), int("00",16), int("00",16)]
        with open(output,"ab") as f:
            f.write(bytearray(wordout))

    # write global trailer 'fadecafe' to file    
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
    testComparator(output)

    
