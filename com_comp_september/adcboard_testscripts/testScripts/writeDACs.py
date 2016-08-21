#!/usr/bin/python

#----------------------------------------------------------------------------------------
# Title: ADC Test script
# Institution: Lawrence Berkeley National Laboratory
# Author: J. Szornel modifying scripts by G. Contin
# Version: 1.0
# Created: 07-15-16
# Last edited: 08-03-16
# Description: This script tests the RDO board, Mezzanine Card, and ADC board chain DACs
#              by writing a value (selectable inside the script) to each DAC on the ADC
#              board.
#----------------------------------------------------------------------------------------

import os
import io
import sys
import time
import proc
import ftdIO
import commRDO
import datetime
import writeCMDreg
from threading import Timer

#------------------------------------------ Main ----------------------------------------

#os.system("clear")

def print_time():
    print "INFO:", time.strftime("%Y-%m-%d_%H-%M-%S")

def WriteAllDACs(mVolts):
    
    stepdata = int(mVolts*4096/2500)

    # 1) Set all DACs to a given value
    print "Setting all DAC outputs to " + str(stepdata)

    proc.writeAD5624_adcboard("CMD4",hex(stepdata),"0")
    proc.writeAD5624_adcboard("CMD4",hex(stepdata),"0")
    proc.writeAD5624_adcboard("CMD4",hex(stepdata),"1")
    proc.writeAD5624_adcboard("CMD4",hex(stepdata),"1")
    proc.writeAD5624_adcboard("CMD4",hex(stepdata),"2")
    proc.writeAD5624_adcboard("CMD4",hex(stepdata),"2")
    proc.writeAD5624_adcboard("CMD4",hex(stepdata),"3")
    proc.writeAD5624_adcboard("CMD4",hex(stepdata),"3")
    proc.writeAD5624_adcboard("CMD4",hex(stepdata),"4")
    proc.writeAD5624_adcboard("CMD4",hex(stepdata),"4")
    proc.writeAD5624_adcboard("CMD4",hex(stepdata),"5")
    proc.writeAD5624_adcboard("CMD4",hex(stepdata),"5")
    proc.writeAD5624_adcboard("CMD4",hex(stepdata),"6")
    proc.writeAD5624_adcboard("CMD4",hex(stepdata),"6")
    proc.writeAD5624_adcboard("CMD4",hex(stepdata),"7")
    proc.writeAD5624_adcboard("CMD4",hex(stepdata),"7")
    time.sleep(1)

def WriteAllDACsIncremented(scanstart, scanstop, scanstep, sleeptime):

    scanstart = int(scanstart)
    scanstop = int(scanstop)
    scanstep = int(scanstep)
    sleeptime = int(sleeptime)

    for mVolts in range(scanstart, scanstop, scanstep):
        print "new mV setting:", mVolts
        stepdata = int(mVolts*4096/2500)

        # -> INCREMENT DAC VOLTAGES:
        proc.writeAD5624_adcboard("CMD4",hex(stepdata),"0")
        proc.writeAD5624_adcboard("CMD4",hex(stepdata),"0")
        proc.writeAD5624_adcboard("CMD4",hex(stepdata),"1")
        proc.writeAD5624_adcboard("CMD4",hex(stepdata),"1")
        proc.writeAD5624_adcboard("CMD4",hex(stepdata),"2")
        proc.writeAD5624_adcboard("CMD4",hex(stepdata),"2")
        proc.writeAD5624_adcboard("CMD4",hex(stepdata),"3")
        proc.writeAD5624_adcboard("CMD4",hex(stepdata),"3")
        proc.writeAD5624_adcboard("CMD4",hex(stepdata),"4")
        proc.writeAD5624_adcboard("CMD4",hex(stepdata),"4")
        proc.writeAD5624_adcboard("CMD4",hex(stepdata),"5")
        proc.writeAD5624_adcboard("CMD4",hex(stepdata),"5")
        proc.writeAD5624_adcboard("CMD4",hex(stepdata),"6")
        proc.writeAD5624_adcboard("CMD4",hex(stepdata),"6")
        proc.writeAD5624_adcboard("CMD4",hex(stepdata),"7")
        proc.writeAD5624_adcboard("CMD4",hex(stepdata),"7")
        time.sleep(sleeptime)

def WriteDACs(scanstart, scanstop, scanstep):
    
    DAC_Chip = range(0,8,1)
    DAC_Channel = ("CMD0", "CMD1", "CMD2", "CMD3")

    scanstart = 0
    scanstop = 2500
    scanstep = 250

    scanstart = int(scanstart)
    scanstop = int(scanstop)
    scanstep = int(scanstep)
    
    TestVoltages = range(scanstart, scanstop, scanstep)
    TestVoltages.append(2490)

    print "Test voltages: ", TestVoltages
    
    for chip in DAC_Chip:
        chip = str(chip)
        print "chip:", chip
        for channel in DAC_Channel:
            for mVolts in range(scanstart, scanstop, scanstep):
                print "new mV setting:", mVolts
                stepdata = int(mVolts*4096/2500)
                # -> INCREMENT DAC VOLTAGES:
                proc.writeAD5624_adcboard(channel,hex(stepdata),chip)
                proc.writeAD5624_adcboard(channel,hex(stepdata),chip)
                time.sleep(1)

    time.sleep(60)


if __name__ == "__main__":

    os.system("clear")
    print "-----------------------------------------------------------------------------\n"
    print "---------------------------- ADC board test ---------------------------------\n"
    print "-------------------------------Write DACs------------------------------------\n"


    print_time()
    commRDO.openRDO()
    WriteAllDACs(1250)
    WriteAllDACsIncremented(0, 2000, 1000, 1)
    commRDO.closeRDO()
   

    print "-----------------------------------------------------------------------------\n"
    print "-------------------------- end of ADC board test ----------------------------\n"
    print "-----------------------------------------------------------------------------\n"
