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

def testAmplifiers(output):
    print "\n\n\nINFO: Executing Differential Amplifier Test"
    
    # open FTDI device
    commRDO.openRDO()

    # set global header af000002
    header = [int("02",16),int("00",16), int("00",16), int("af",16)]
    with open(output,"ab") as f:
        f.write(bytearray(header))

    # a)  Disable the amplifiers with an enable pin (make sure they are disabled):
        # i) Set BIT1 of firmware output register to 0
    ftdIO.writeReg(320,0x0)

    # b)  Set input voltage to the amplifiers to ~20mV (voltage to set at DAC is 10 times higher):
        #i) Can use CMD2 for DAC0 (AD5624 device on the ADC board) with 12-bit data field = x140

    proc.writeAD5624_adcboard("CMD2","0x140","0")

    sleep(0.2)

    # c)  Read the amplifier outputs 10 times consecutively:
        # i) Can read all the ADC on the ADC board 10 times consecutively (10 operations). Can put a sleep between consecutive operations.
        # -> READ ALL THE ADC-BOARD ADCS
        # first set the frequency and the number of samples: 0x0 = 28 clock cycles, 1 sample
        # ftdIO.writeReg(384,0x0)
    
    for i in range(0,10):
        print "read loop %d" % (i)
        writeCMDreg.writeCMDreg("SPIadc","start_adc_ch_31-0")
        # write to file
        ftdIO.readFifoToFile(output,14)
    
    # d)  Enable the amplifiers with an enable pin:
        # i) Set BIT1 of firmware output register to 1
    ftdIO.writeReg(320,0x2)
    # g-i) scan: i) Can use CMD2 for DAC0 (AD5624 device on the ADC board) with 12-bit data field +=x050

    scanstart = 0         # 0 
    scanstop = 50         # 0x320
    scanstep = 5          # 0x50
    stepdata = 0      
    mVolts = 0        
    step = 0
    for mVolts in range(scanstart, scanstop, scanstep):
        stepdata = int(mVolts*16)
        print "step %, new mV setting:", step, mVolts
        print ""
        proc.writeAD5624_adcboard("CMD2",hex(stepdata),"0")
        sleep(0.2)
        for i in range(0,10):
            print "read loop %d" % (i)
            writeCMDreg.writeCMDreg("SPIadc","start_adc_ch_31-0")
            # write to file
            ftdIO.readFifoToFile(output,14)
        step = step + 1
    
    # j-l) scan: i) Can use CMD2 for DAC0 (AD5624 device on the ADC board) with 12-bit data field +=x0F0

    scanstart = 50         # 0x320
    scanstop = 125        # 0x7D0
    scanstep = 15          # 0xF0
    stepdata = 0           
    mVolts = 0            
    step = 0
    for mVolts in range(scanstart, scanstop, scanstep):
        stepdata = int(mVolts*16)
        print "step %, new mV setting:", step, mVolts
        proc.writeAD5624_adcboard("CMD2",hex(stepdata),"0")
        sleep(0.2)
        for i in range(0,10):
            print "read loop %d" % (i)
            writeCMDreg.writeCMDreg("SPIadc","start_adc_ch_31-0")
            # write to file
            ftdIO.readFifoToFile(output,14)
        step = step + 1

    # j-l) scan: i) Can use CMD2 for DAC0 (AD5624 device on the ADC board) with 12-bit data field +=x0F0

    scanstart = 125        # 0xF0 
    scanstop = 251         # 0xFA0
    scanstep = 25           # 0x190
    stepdata = 0     
    mVolts = 0      
    step = 0
    for mVolts in range(scanstart, scanstop, scanstep):
        stepdata = int(mVolts*16)
        print "step %, new mV setting:", step, mVolts
        proc.writeAD5624_adcboard("CMD2",hex(stepdata),"0")
        sleep(0.2)
        for i in range(0,10):
            print "read loop %d" % (i)
            writeCMDreg.writeCMDreg("SPIadc","start_adc_ch_31-0")
            # write to file
            ftdIO.readFifoToFile(output,14)
        step = step + 1

    
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
    testAmplifiers(output)

    
