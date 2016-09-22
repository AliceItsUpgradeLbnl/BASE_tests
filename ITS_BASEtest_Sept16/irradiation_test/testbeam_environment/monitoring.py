#!/usr/bin/env python
__author__ = "G. Contin"
__version__ = "2.0"
__status__ = "Prototype"

from time import strftime, sleep
import datetime
import proc
import commRDO
import ftdIO
import procDUT
import proc
import os

def printline(line,log):
    print line
    cmdstr="echo "+str(line)+ " >> "  + str(log)
    os.system(cmdstr)

#def monitoring():
# periodic task
    # send command
    #exit(-1)
    # read data with timestamp

    # read back configuration

    # stop system

#def monitoring.runMon():
    
def configMon():
    print "\n\n\nINFO: Configuring device for monitoring"
    sleep(1)
    
    # Enable AMPLIFIERS, DC-DC converters and FET switches on the DUT board
    # Set BIT1 of firmware output register to 1
    ftdIO.writeReg(320,0x2)
    
    # Set outputs of the differential amplifiers to values different than 0V (480mV, 960mV and 2.4V
    # Can use CMD2 for DAC0 (AD5624 on ADC board) with 12-bit data field set to x312
    proc.writeAD5624_adcboard("CMD2","0x312","0")
    
    # Set the comparator output to high
    # Can use CMD0 for DAC0 (AD5624 device on ADC board) with data x800
    proc.writeAD5624_adcboard("CMD0","0x800","0")
    # Can use CMD1 for DAC0 (AD5624 device on ADC board) with data x000
    proc.writeAD5624_adcboard("CMD1","0x000","0")

    # Set DAC AD5671R outputs to midscale
    # Can use CMD8 with data x800
    # write_dutboard(device,ndevice,cmd,data,target,nbytes):
    procDUT.write_dutboard("AD5671R","4","CMD8","0x800","0","3")  

    # Set the 1K digital potentiometer on the ADC board to ~100 OHM
    # Can use CMD0 for DP_1K (AD5254) on the ADC board with value x19
    #proc.writeAD5254_adcboard("CMD0",0x19,"0")
    proc.writeAD5254_adcboard("CMD0","0xFC","0")
    # Enable RTD
    # 28 01 80 C0
    ftdIO.writeReg(192,0x0128)
    ftdIO.writeReg(256,0xC080)
    ftdIO.writeReg(128,0x0403)

if __name__ == "__main__":
     # open FTDI device
    commRDO.openRDO()

    configMon()
    # close FTDI device
    commRDO.closeRDO()
