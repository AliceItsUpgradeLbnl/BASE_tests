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
  print ("on")
  devenable = int('0000000000000010', 2)
  ftdIO.writeReg(320,devenable)
  sleep(5)
  print ("off")
  devenable = int('0000000000000000', 2)
  ftdIO.writeReg(320,devenable)
  sleep(5)
  print ("on")
  devenable = int('0000000000000010', 2)
  ftdIO.writeReg(320,devenable)
  sleep(5)
  print ("off")
  devenable = int('0000000000000000', 2)
  ftdIO.writeReg(320,devenable)
  sleep(5)

# 2) Check ADCMP603_LE_HYS_IN

def CompLE():
 
  print "---ADCMP603_LE_HYS_IN Test---"
  print ("on")
  complatch = int('0000000000000001', 2)
  ftdIO.writeReg(320,complatch)
  sleep(5)
  print ("off")
  devenable = int('0000000000000000', 2)
  ftdIO.writeReg(320,devenable)
  sleep(5)
  print ("on")
  devenable = int('0000000000000001', 2)
  ftdIO.writeReg(320,devenable)
  sleep(5)
  print ("off")
  devenable = int('0000000000000000', 2)
  ftdIO.writeReg(320,devenable)
  sleep(5)

# 3) I2C set digipot
# sets different values to dut board digipot

def I2Cdigipot():
  print "---I2C Digi Pot Test---"
  proc.writeAD5254_dutboard("CMD0","AA","0")
  sleep(1)
  proc.writeAD5254_dutboard("CMD0","00","0")
  sleep(1)
  proc.writeAD5254_dutboard("CMD0","FF","0")
  sleep(1)
  proc.writeAD5254_dutboard("CMD0","00","0")

# 4) I2C RW
# sets a value on dut digipot, reads back value
def testI2C():
  print "---I2C Test---"
  proc.writeAD5254_dutboard("CMD0","FF","0")
  sleep(1)
  proc.readAD5254_dutboard("CMD8", "00", "0")
  ftdIO.readFifoToFile(output,3)

# 4) Aux. I2C
def testauxI2C():
  print "---Aux I2C Test---"
  proc.writeAD5254_auxI2C_dutboard("CMD0","AA","0")
  sleep(1)
  proc.readAD5254_auxI2C_dutboard("CMD8", "00", "0")
  ftdIO.readFifoToFile(output,3)

# 5) Read comparator outputs

def ReadComparatorOutputs():

  print "---Read ADCMP603 Test---"
  # -> READ COMPARATOR OUTPUTS USING FIRMWARE GPIO INPUT REGISTER:

  out = ftdIO.readReg(64) & 0x1
  print "ADCMP603_Q_OUT",out
  
  barout = ftdIO.readReg(64) & 0x2
  print "ADCMP603_Q_BAR_OUT:",barout

  sleep(1)

# Differential IO checker

def DiffIOChecker():

  print "---Diff IO Test---"

  proc.writeDiffIOChecker("F700")
  ftdIO.readFifoToFile(output,10)
  print hex(proc.readDiffIOChecker())

def InterruptChecker():

  print "---Interrrupt Test---"

  proc.writeInterruptChecker("0710")
  print "---10 second sleep---"
  sleep(10)
  print hex(proc.readInterruptChecker())
  
  print "starting second test"
  proc.writeInterruptChecker("0710")
  print "---10 second sleep---"
  sleep(10)
  print hex(proc.readInterruptChecker())

def ADCBoardDigiPots():
  proc.writeAD5254_adcboard("CMD0","00","0")
  print "check '00'"
  sleep(10)
  proc.writeAD5254_adcboard("CMD0","FF","0")
  print "check 'FF'"
  sleep(10)
  proc.writeAD5254_adcboard("CMD0","AA","0")
  print "check 'AA'"
  sleep(5)

##-----------------------------------------------------------##
##-----------------------------------------------------------##


output = "./results/I2CReadTest.dat"

commRDO.openRDO()
#sleep(5)
#DevEnable()
#CompLE()
#ReadComparatorOutputs()
#I2Cdigipot()
#testI2C()
#testauxI2C()
#DiffIOChecker()
#InterruptChecker()
#ADCBoardDigiPots()
commRDO.closeRDO()

sys.exit()
