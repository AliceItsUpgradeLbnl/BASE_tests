#!/usr/bin/python

#----------------------------------------------------------------------------------------
# Title: ADC Test script
# Institution: Lawrence Berkeley National Laboratory
# Author: J. Szornel modifying scripts by G. Contin
# Version: 1.0
# Created: 08-18-16
# Last edited: 08-19-16
# Description: This script tests the RDO board, Mezzanine Card, and ADC board chain. It
#              executes user-selected tests, which are present separately in this directory.
#              Resutls are written to a ./results folder. 
#----------------------------------------------------------------------------------------

import os
import io
import sys
import time
import proc
import ftdIO
import commRDO
import datetime
import testADCs
import readADCs
import testGPIOs
import writeDACs
import datadecoder
import writeCMDreg
import quickADCtest
from threading import Timer

#------------------------------------------ Main ----------------------------------------

os.system("clear")

def ADCruntests(): 
  print "-----------------------------------------------\n"
  print "----------- Starting ADC board tests ----------"
  print "-----------------------------------------------"

  DACwrite = "no"
  DACwrite = int(raw_input('Preform a simple write test to all DAC channels? Enter 1 for yes, 0 for no: '))
  if DACwrite == 1:
    print "\n \n \n"
    writeDACs.print_time()
    commRDO.openRDO()
    print "Setting all DACs to 0V \n"
    writeDACs.WriteAllDACs(int(0.0))
    time.sleep(1)
    print "Setting all DACs to a different value \n"
    DACmVsetting = raw_input("DAC output (mV) : ")
    print "DAC mV setting:", DACmVsetting
    writeDACs.WriteAllDACs(int(DACmVsetting))
    time.sleep(1)
    commRDO.closeRDO()
  elif DACwrite < 1:
    print "Skipping this test..."
  elif DACwrite > 1:
    print "Please restart script."
    sys.exit()

  DACsteps = int(raw_input("Preform an incremented write test to all DAC channels? Enter 1 for yes, 0 for no: "))
  if DACsteps == 1:
    print "\n \n \n"
    commRDO.openRDO()
    writeDACs.print_time()
    print "Starting incremented DAC write test \n"
    scanstart = raw_input("first voltage of scan (mV): \n")
    scanstop = raw_input("last voltage of scan (mV): \n")
    scanstep = raw_input("voltage scan step: \n")
    scanstop = int(scanstop) + ((int(scanstep))/2)
    sleeptime = raw_input("sleep time between settings: \n")
    writeDACs.WriteAllDACsIncremented(scanstart, scanstop, scanstep, sleeptime)
    commRDO.closeRDO()
  elif DACsteps < 1:
    print "Skipping this test..."
  elif DACsteps > 1:
    print "Please restart script."
    sys.exit()

  ADCread = int(raw_input("Preform a simple read test of ADCs? Enter 1 for yes, 0 for no: "))
  if ADCread == 1:
    print "\n \n \n"
    DUTid = readADCs.ReadADCs()
    print "DUTid ", DUTid
    time.sleep(1)
    readADCs.ADCDataFormat(DUTid)
    time.sleep(1)
    readADCs.ADCDataDecoder(DUTid)
    time.sleep(1)
    print "Scroll to bottom of output file (./results/ADCtest(DUTid)_noise.txt) for average values and standard deviations"
  elif DACwrite < 1:
    print "Skipping this test..."
  elif DACwrite > 1:
    print "Please restart script."
    sys.exit()

  print "The ADC/DAC test requires that ADCs and DACs are joined with the custom connectors. Check that this has been done."
  print "The quick ADC/DAC test (<5 min) checks 2 values (1000 and 2500mV). It reads each ADC once for each value. "
  ADCquick = int(raw_input('Preform a quick ADC/DAC test? Enter 1 for yes, 0 for no: '))
  if ADCquick == 1:
    print "\n \n \n"
    DUTid = quickADCtest.ADCQuickTest()
    print "DUTid ", DUTid
    quickfilename = "quicktest" + str(DUTid)
    datadecoder.ADCTestDataFormat(quickfilename)
    datadecoder.ADCTestDataDecoder(quickfilename)
    datadecoder.DummyXVariable(quickfilename)
    ADCquickplot = int(raw_input('Show plot of results? Enter 1 for yes, 0 for no: '))
    if ADCquickplot == 1:
      os.system("python ADCplot.py quicktest"+str(DUTid))
    else:
      print "Okay. Not plotting results from quicktest..."
  elif ADCquick < 1:
    print "Skipping this test..."
  elif ADCquick> 1:
    print "Please restart script."
    sys.exit()

  print "The ADC/DAC test requires that ADCs and DACs are joined with the custom connectors. Check that this has been done."
  print "The full ADC/DAC test (~40 min) checks 10 values (250 - 2490mV in increments of 250mV). It reads each ADC 10 times for each value." 

  ADCfull = int(raw_input("Preform a full ADC/DAC test? Enter 1 for yes, 0 for no: "))
  if ADCfull == 1:
    print "\n \n \n"
    DUTid = testADCs.ADCTest()
    print "DUTid ", DUTid
    fulltestfilename = "fulltest" + str(DUTid)
    datadecoder.ADCTestDataFormat(fulltestfilename)
    datadecoder.ADCTestDataDecoder(fulltestfilename)
    datadecoder.DummyXVariable(fulltestfilename)
    ADCfullplot = int(raw_input('Show plot of results? Enter 1 for yes, 0 for no: '))
    if ADCfullplot == 1:
      print "DUTid: ", DUTid
      os.system("python ADCplot.py fulltest"+str(DUTid))
  elif ADCfull < 1:
    print "Skipping this test..."
  elif ADCfull > 1:
    print "Please restart script."
    sys.exit()

  DAClinplot = int(raw_input("Show plot of DAC values? Enter 1 for yes, 0 for no: "))
  if DAClinplot == 1:
    print "\n \n \n"
    os.system("python DACplot.py fulltest"+str(DUTid))
  elif DAClinplot < 1:
    print "Skipping..."
  elif DAClinplot > 1:
    print "Please restart script."
    sys.exit()

  print "-----GPIO tests-----"
  GPIOs = int(raw_input("Check GPIOs? Enter 1 for yes, 0 for no: "))
  if GPIOs == 1:
    print "\n \n \n"
    print "-------------- starting GPIO tests --------------"
    commRDO.openRDO()
    DevEn = int(raw_input("Check DEV_ENABLE? Enter 1 for yes, 0 for no: "))
    if DevEn ==1:
      testGPIOs.DevEnable()
    CompLe = int(raw_input("Check COMP_LE? Enter 1 for yes, 0 for no: "))
    if CompLe ==1:
      testGPIOs.CompLE()
    CompOutput = int(raw_input("Check ADCMP603 (comparator) outputs? Enter 1 for yes, 0 for no: "))
    if CompOutput ==1:
      testGPIOs.ReadComparatorOutputs()
    I2CDigPot = int(raw_input("Check I2C DigiPot? Enter 1 for yes, 0 for no: "))
    if I2CDigPot ==1:
      testGPIOs.I2Cdigipot()
    I2Ctest = int(raw_input("Check I2C? Enter 1 for yes, 0 for no: "))
    if I2Ctest ==1:
      testGPIOs.testI2C()
    I2Cauxtest = int(raw_input("Check I2C? Enter 1 for yes, 0 for no: "))
    if I2Cauxtest ==1:
      testGPIOs.testauxI2C()
    DiffIO = int(raw_input("Check Differential IO? Enter 1 for yes, 0 for no: "))
    if DiffIO ==1:
      testGPIOs.DiffIOChecker()
    InterruptTest = int(raw_input("Check Interrupt? Enter 1 for yes, 0 for no: "))
    if InterruptTest ==1:
      testGPIOs.InterruptChecker()
    ADCDigPot = int(raw_input("Check ADC board DigiPots? Enter 1 for yes, 0 for no: "))
    if ADCDigPot ==1:
      testGPIOs.ADCBoardDigiPots()
    print "-------------- completed GPIO tests --------------"
    commRDO.closeRDO()

  print "-----------------------------------------------\n"
  print "---------- Completed ADC board tests ----------"
  print "-----------------------------------------------"

ADCruntests()

