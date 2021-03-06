#!/usr/bin/python

#----------------------------------------------------------------------------------------
# Title: ADC Test script
# Institution: Lawrence Berkeley National Laboratory
# Author: J. Szornel modifying scripts by G. Contin.
# Version: 1.0
# Created: 07-15-16
# Last edited: 08-03-16
# Description: This script tests the RDO board, Mezzanine Card, and ADC board chain for
#              the ITS power system component radiation tolerance test in Sept 2016. The
#              script writes to each of 32 DAC channels on the ADC board. It then reads all 32
#              ADC channels (connected to DAC outputs) on the ADC board. These values 
#              are stored in an output file. The first read does not return proper values and
#              should be ignored.
#----------------------------------------------------------------------------------------

import os
import io
import sys
import time
import proc
import ftdIO
import commRDO
import datetime
import writeDACs
import writeCMDreg
from threading import Timer

#------------------------------------------ Main ----------------------------------------

os.system("clear")

print "-----------------------------------------------------------------------------\n"
print "---------------------------- ADC board test ---------------------------------\n"
print "---------------------------Quick ADC/DAC test--------------------------------\n"


def printline(line,log):
    print line
    cmdstr="echo "+str(line)+ " >> "  + str(log)
    os.system(cmdstr)

def print_time():
    print "INFO:", time.strftime("%Y-%m-%d_%H-%M-%S")

def ADCQuickTest(output,log):
    displayfile = "results/displayfile.log"

    # 0) Set DACs to 0V

    print('Setting all DAC outputs to 0V \n')

    writeDACs.WriteAllDACs(0.0)
    
    # 1) Configure ADCs, read once (need to throw away first read)

    # -> READ ALL THE ADC-BOARD ADCS
    # first set the frequency and the number of samples: 0x0 = 28 clock cycles, 1 sample
    ftdIO.writeReg(384,0x0)
    #readI2C(devName,OpName)
    writeCMDreg.writeCMDreg("SPIadc","start_adc_ch_31-0")
    time.sleep(1)
    # write to file
    ftdIO.readFifoToFile(output,14)

    writeCMDreg.writeCMDreg("SPIadc","start_adc_ch_31-0")
    time.sleep(.5)
    ftdIO.readFifoToFile(output,14)
    time.sleep(.5)

    # 2) Increment DACs, read ADCs

    DAC_Chip = range(0,4,1)
    DAC_Channel = ("CMD0", "CMD1", "CMD2", "CMD3")

    scanstart = 1000
    scanstop = 2500
    scanstep = 1000
    
    TestVoltages = range(scanstart, scanstop, scanstep)
    print "Test Voltages: ", TestVoltages
    

    for chip in DAC_Chip:
        chip = str(chip)
        for channel in DAC_Channel:
            writeDACs.WriteAllDACs(0.0)
            time.sleep(1)              
            for mVolts in TestVoltages:
                    print "channel:", channel, ", chip: ", chip 
                    print "new mV setting:", mVolts

                    stepdata = int(mVolts*4096/2500)
                    # -> INCREMENT DAC VOLTAGES:
                    proc.writeAD5624_adcboard(channel,hex(stepdata),chip)
                    proc.writeAD5624_adcboard(channel,hex(stepdata),chip)
                    time.sleep(1)
                    
                    # -> READ ALL THE ADC-BOARD ADCS 1 TIME:
                    writeCMDreg.writeCMDreg("SPIadc","start_adc_ch_31-0")
                    ftdIO.readFifoToFile(output,14)
                    time.sleep(.5) 

    writeDACs.WriteAllDACs(0.0)
    writeCMDreg.writeCMDreg("SPIadc","start_adc_ch_31-0")
    ftdIO.readFifoToFile(output,14)
    time.sleep(.5)

    # set global trailer 'fadecafe'
    trailer = [int("fe",16),int("ca",16),int("de",16),int("fa",16)]
    with open(output,"ab") as f:
        f.write(bytearray(trailer))
    
    # Check and close up
    stoptime = time.strftime("%Y-%m-%d_%H-%M-%S")
    print stoptime, "ADC/DAC quick test data on file:", output
    t2 = datetime.datetime.now()
    stringInfo =  "Duration: " + str(t2.replace(microsecond=0)-t1.replace(microsecond=0))
    print stringInfo
    print "INFO: ADC/DAC Test completed."
    f.close()

if __name__ == "__main__":
    DUTid = 2
    startime = time.strftime("%Y-%m-%d_%H-%M-%S")
    t1 = datetime.datetime.now()
    #output = "ADCTest/fulltest" + str(DUTid) + "_" + str(startime)+".dat"
    #log = "ADCTest/fulltest" + str(DUTid) + "_" + str(startime)+".log"
    output = "results/quicktest" + str(DUTid) + ".dat"
    log = "results/quicktest" + str(DUTid) + ".log"
    print startime, "ADC Test data on file:", output
    time.sleep(1)
    commRDO.openRDO()
    ADCQuickTest(output, log)
    commRDO.closeRDO()

print "-----------------------------------------------------------------------------\n"
print "-------------------------- end of ADC board test ----------------------------\n"
print "-----------------------------------------------------------------------------\n"

