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

def testIOexpander(output):
    print "\n\n\nINFO: Executing IO expander Test"

    # open FTDI device
    commRDO.openRDO()

    # set global header cd00000a
    header = [int("0a",16),int("00",16), int("00",16), int("cd",16)]
    with open(output,"ab") as f:
        f.write(bytearray(header))

    # Part 1 of test: check that the open drain functionalities of the IO expander are OK and that read op is performed correctly

    # a) Check with open drain IO expander pins set to GND  

    # Set all the IO of the open drain IO expander (on the DUT, PCF8574) to ground       
    procDUT.write_dutboard("PCF8574","4","CMD0","0x00","0","1")
    # Set 8-bit wide output_port_0 of push-pull IO expander to VCC
    procDUT.writeTCA9535_dutboard("TCA9535","4","CMD2","0xFF","0","2")
    # Set 8-bit wide config_port_0 of push-pull IO expander (we want this to be an output port)
    procDUT.writeTCA9535_dutboard("TCA9535","4","CMD4","0x00","0","2")
    # Set 8-bit wide config_port_1 of push-pull IO expander (we want this to be an input port)
    procDUT.writeTCA9535_dutboard("TCA9535","4","CMD5","0xFF","0","2")
    # Read all the IO of the open drain IO expander 
    procDUT.write_dutboard("PCF8574","4","CMD0","0x00","0","4")
    # Wait for data to be ready
    sleep(0.2)
    # Write to file
    ftdIO.readFifoToFile(output,3)
    # Read 8-bit wide input_port_1 of push-pull IO expander
    procDUT.writeTCA9535_dutboard("TCA9535","4","CMD1","0x00","0","5")
    # Wait for data to be ready
    sleep(0.2)
    # Write to file
    ftdIO.readFifoToFile(output,3)  

    # b) Check with open drain IO expander pins set to VCC  

    # Set the IO of the open drain IO expander to VCC
    procDUT.write_dutboard("PCF8574","4","CMD0","0xFF","0","1")
    # Set 8-bit wide output_port_0 of push-pull IO expander to ground
    procDUT.writeTCA9535_dutboard("TCA9535","4","CMD2","0x00","0","2")
    # Read all the IO of the open drain IO expander 
    procDUT.write_dutboard("PCF8574","4","CMD0","0x00","0","4")
    # Wait for data to be ready
    sleep(0.1)
    # Write to file
    ftdIO.readFifoToFile(output,3)
    # Read 8-bit wide input_port_1 of push-pull IO expander
    procDUT.writeTCA9535_dutboard("TCA9535","4","CMD1","0x00","0","5")
    # Wait for data to be ready
    sleep(0.1)
    # Write to file
    ftdIO.readFifoToFile(output,3)

    # c) Check with both IO expanders at VCC (want to check if it reads correctly)

    # Set the IO of the open drain IO expander to VCC
    procDUT.write_dutboard("PCF8574","4","CMD0","0xFF","0","1")
    # Set 8-bit wide output_port_0 of push-pull IO expander to ground
    procDUT.writeTCA9535_dutboard("TCA9535","4","CMD2","0xFF","0","2")
    # Read all the IO of the open drain IO expander 
    procDUT.write_dutboard("PCF8574","4","CMD0","0x00","0","4")
    # Wait for data to be ready
    sleep(0.1)
    # Write to file
    ftdIO.readFifoToFile(output,3)
    # Read 8-bit wide input_port_1 of push-pull IO expander
    procDUT.writeTCA9535_dutboard("TCA9535","4","CMD1","0x00","0","5")
    # Wait for data to be ready
    sleep(0.1)
    # Write to file
    ftdIO.readFifoToFile(output,3)

    sleep(1)

    # Part 2 of test: check that the high current strength pull up functionality of the open drain IO expander is OK (on each channel)

    # Set 8-bit wide output_port_0 of push-pull IO expander to ground
    procDUT.writeTCA9535_dutboard("TCA9535","4","CMD2","0x00","0","2")

    scanstart = 0
    scanstop = 8
    scanstep = 1
    for bitnum in range(scanstart, scanstop, scanstep):
        stepdata = 2**int(bitnum)
        print "new mV setting:", bitnum, stepdata, bin(stepdata), hex(stepdata)

        # Set the IO of the open drain IO expander to ground
        procDUT.write_dutboard("PCF8574","4","CMD0","0x00","0","1")

        # Read 8-bit wide input_port_1 of push-pull IO expander
        procDUT.writeTCA9535_dutboard("TCA9535","4","CMD1","0x00","0","5")

        # Wait for data to be ready
        sleep(0.1)

        # Write to file
        ftdIO.readFifoToFile(output,3)  
        
        # Start the Interrupt checker in the firmware (for the push-pull IO expander)
        targetcmd = "0x" + str(7) + str(1) + str(0)
        print "Main command reg:", str(targetcmd)
        ftdIO.writeReg(128,int(targetcmd,16))

        sleep(0.5)

        # Set IO[bitnum = 0] of the open drain IO expander (on the DUT, PCF8574) to VCC
        procDUT.write_dutboard("PCF8574","4","CMD0",hex(stepdata),"0","1")

        # Wait for data to be ready
        sleep(0.1)

        # 1. Read firmware input register
        wordout = ftdIO.readReg(64)
        wordtofile = [wordout & 255,(wordout >> 8) & 255,(wordout >> 16) & 255,(wordout >> 24) & 255]

        # write to file
        with open(output,"ab") as f:
          f.write(bytearray(wordtofile))

    # Part 3 of test: check that the Interrupt of the open drain IO expander works well

    scanstart = 0
    scanstop = 8
    scanstep = 1
    for bitnum in range(scanstart, scanstop, scanstep):
        stepdata = (0xFF - 2**int(bitnum)) & 0xFF
        print "new mV setting:", bitnum, stepdata, bin(stepdata), hex(stepdata)

        # Set 8-bit wide output_port_0 of push-pull IO expander to VCC
        procDUT.writeTCA9535_dutboard("TCA9535","4","CMD2","0xFF","0","2")

        # Set IO of the open drain IO expander (on the DUT, PCF8574) to VCC
        procDUT.write_dutboard("PCF8574","4","CMD0","0xFF","0","1")
        
        # Read all the IO of the open drain IO expander 
        procDUT.write_dutboard("PCF8574","4","CMD0","0x00","0","4")
        # Wait for data to be ready
        sleep(0.1)
        # Write to file
        ftdIO.readFifoToFile(output,3)
        
        # Start the Interrupt checker in the firmware (for the open drain IO expander)
        targetcmd = "0x" + str(7) + str(2) + str(0)
        print "Main command reg:", str(targetcmd)
        ftdIO.writeReg(128,int(targetcmd,16))

        # Set IO[bitnum] of 8-bit wide output_port_0 of push-pull IO expander to ground, all the others to VCC
        procDUT.writeTCA9535_dutboard("TCA9535","4","CMD2",hex(stepdata),"0","2")
        sleep(0.5)

        # Read firmware input register
        wordout = ftdIO.readReg(64)
        wordtofile = [wordout & 255,(wordout >> 8) & 255,(wordout >> 16) & 255,(wordout >> 24) & 255]
        # Wait for data to be ready
        sleep(0.1)
        # write to file
        with open(output,"ab") as f:
            f.write(bytearray(wordtofile))

   
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
    testIOexpander(output)
