#!/usr/bin/env python

#----------------------------------------------------------------------------------------
# Title: ADC board test script (GPIOs)
# Institution: Lawrence Berkeley National Laboratory
# Author: J. Szornel modifying scripts by G. Contin.
# Version: 1.0
# Created: 07-15-16
# Last edited: 08-03-16
# Description: This script tests the RDO board, Mezzanine Card, and ADC board chain for
#              the ITS power system component radiation tolerance test in Sept 2016. This
#              script contains tests for all of the GPIO connections for DUT board 2.
#----------------------------------------------------------------------------------------

import os
import sys
import proc
import ftdIO
import commRDO
from time import strftime, sleep

# 1) Check DEV_ENABLE

def DevEnable():

  print "---DEV_ENABLE Test---"
  print ("DEV_ENABLE switching on")
  devenable = int('0000000000000010', 2)
  ftdIO.writeReg(320,devenable)
  sleep(5)
  print ("DEV_ENABLE switching off")
  devenable = int('0000000000000000', 2)
  ftdIO.writeReg(320,devenable)
  sleep(5)
  print ("DEV_ENABLE switching on")
  devenable = int('0000000000000010', 2)
  ftdIO.writeReg(320,devenable)
  sleep(5)
  print ("DEV_ENABLE switching off")
  devenable = int('0000000000000000', 2)
  ftdIO.writeReg(320,devenable)
  sleep(5)

# 2) Check ADCMP603_LE_HYS_IN

def CompLE():
 
  print "---ADCMP603_LE_HYS_IN Test---"
  print ("ADCMP603_LE_HYS_IN switching on")
  complatch = int('0000000000000001', 2)
  ftdIO.writeReg(320,complatch)
  sleep(5)
  print ("ADCMP603_LE_HYS_IN switching off")
  devenable = int('0000000000000000', 2)
  ftdIO.writeReg(320,devenable)
  sleep(5)
  print ("ADCMP603_LE_HYS_IN switching on")
  devenable = int('0000000000000001', 2)
  ftdIO.writeReg(320,devenable)
  sleep(5)
  print ("ADCMP603_LE_HYS_IN switching off")
  devenable = int('0000000000000000', 2)
  ftdIO.writeReg(320,devenable)
  sleep(5)

# 3) I2C set digipot
# sets different values to dut board digipot

def I2Cdigipot():
  print "---I2C Digi Pot Test---"
  print "Writing AA to AD5254 DigiPot"
  proc.writeAD5254_dutboard("CMD0","AA","0")
  sleep(1)
  "Writing 00 to AD5254 DigiPot"
  proc.writeAD5254_dutboard("CMD0","00","0")
  sleep(1)
  "Writing FF to AD5254 DigiPot"
  proc.writeAD5254_dutboard("CMD0","FF","0")
  sleep(1)
  "Writing 00 to AD5254 DigiPot"
  proc.writeAD5254_dutboard("CMD0","00","0")

# 4) I2C RW
# sets a value on dut digipot, reads back value
def testI2C():
  print "---I2C Test---"
  output = "./results/I2CReadTest.dat"
  "Writing FF to AD5254 DigiPot"
  proc.writeAD5254_dutboard("CMD0","FF","0")
  sleep(1)
  "Reading AD5254 DigiPot"
  proc.readAD5254_dutboard("CMD8", "00", "0")
  ftdIO.readFifoToFile(output,3)

# 4) Aux. I2C
def testauxI2C():
  print "---Aux I2C Test---"
  output = "./results/I2CAUXReadTest.dat"
  "Writing AA to AD5254 DigiPot with AUX I2C"
  proc.writeAD5254_auxI2C_dutboard("CMD0","AA","0")
  sleep(1)
  "Reading AD5254 DigiPot with AUX I2C"
  proc.readAD5254_auxI2C_dutboard("CMD8", "00", "0")
  ftdIO.readFifoToFile(output,3)

# 5) Read comparator outputs

def ReadComparatorOutputs():

  print "---Read ADCMP603 Test---"
  print "Reading ADCMP603 \n"
  out = ftdIO.readReg(64) & 0x1
  print "ADCMP603_Q_OUT:",out
  barout = ftdIO.readReg(64) & 0x2
  print "ADCMP603_Q_BAR_OUT:",barout
  sleep(1)

# Differential IO checker

def DiffIOChecker():

  print "---Diff IO Test---"
  output = "./results/DiffIOTest.dat"
  print "Reading DiffIOChecker"
  proc.writeDiffIOChecker("F700")
  ftdIO.readFifoToFile(output,10)
  print hex(proc.readDiffIOChecker())

def InterruptChecker():

  print "---Interrrupt Test---"
  print "Initiating Interrupt Checker"
  proc.writeInterruptChecker("0710")
  print "---10 second sleep---"
  sleep(10)
  print "Reading Interrupt Checker"
  print hex(proc.readInterruptChecker())
  print "starting second test"
  proc.writeInterruptChecker("0710")
  print "---10 second sleep---"
  sleep(10)
  print "Reading Interrupt Checker"
  print hex(proc.readInterruptChecker())

def ADCBoardDigiPots():
  print "Writing ADC board Digipots with 00"
  proc.writeAD5254_adcboard("CMD0","00","0")
  print "check '00'"
  print "---10 second sleep---"
  sleep(10)
  print "Writing ADC board Digipots with FF"
  proc.writeAD5254_adcboard("CMD0","FF","0")
  print "check 'FF'"
  print "---10 second sleep---"
  sleep(10)
  print "Writing ADC board Digipots with AA"
  proc.writeAD5254_adcboard("CMD0","AA","0")
  print "check 'AA'"
  print "---5 second sleep---"
  sleep(5)

##-----------------------------------------------------------##
##-----------------------------------------------------------##

#def main():
if __name__ == "__main__":

  print "---------------starting GPIO tests---------------"
  commRDO.openRDO()
  sleep(1)
  DevEnable()
  CompLE()
  ReadComparatorOutputs()
  I2Cdigipot()
  testI2C()
  testauxI2C()
  DiffIOChecker()
  InterruptChecker()
  ADCBoardDigiPots()
  commRDO.closeRDO()
  print "---------------GPIO tests complete---------------"
  sys.exit()
