#!/usr/bin/env python
__author__ = "G. Contin"
__version__ = "1.0"
__status__ = "Prototype"

from time import strftime, sleep
import datetime
from threading import Timer
import time
import loadfirmware
import discardAdcSamples
import testAmplifiers
import testComparator
import testDAC
import testBridgeRTD
import testIOexpander
import testADCs
import testI2Cdriver
import testDiff2SE
import testDCdcFET

def printline(line,output):
    cmdstr="echo "+str(line)+ " >> "  + str(output)
    os.system(cmdstr)

def print_time():
    print "INFO: ", time.strftime("%Y-%m-%d_%H-%M-%S")

def offlineTest(output):

    # Discard first ADC samples after poweron
    discardAdcSamples.discardAdcSamples()

    # Set up files
    
    # TEST 1: DIFFERENTIAL AMPLIFIERS
    testAmplifiers.testAmplifiers(output)
    sleep(0.5)

    # TEST 2: COMPARATOR
    testComparator.testComparator(output)
    sleep(0.5)

    # TEST 3: DAC
    testDAC.testDAC(output)
    sleep(0.5)

    # TEST 4: BRIDGE AND RESISTANCE TO DIGITAL CONVERTER
    testBridgeRTD.testBridgeRTD(output)
    sleep(0.5)

    # TEST 5: IO EXPANDER
    testIOexpander.testIOexpander(output)
    sleep(0.5)

    # TEST 6: ADC channels
    testADCs.testADCs(output)
    sleep(0.5)

    # TEST 7: I2C DRIVER
    testI2Cdriver.testI2Cdriver(output)
    sleep(0.5)

    # TEST 8: DIFFERENTIAL TO SINGLE ENDED DRIVER
    testDiff2SE.testDiff2SE(output)
    sleep(0.5)

    # TEST 9: DC-DC CONVERTERS and FET
    testDCdcFET.testDCdcFET(output)
    sleep(0.5)
    
    # Check and close up
    stoptime = time.strftime("%Y-%m-%d_%H-%M-%S")
    print stoptime, "Offline Test data on file:", output 
    t2 = datetime.datetime.now()
    stringInfo =  "Duration: " + str(t2.replace(microsecond=0)-t1.replace(microsecond=0))
    print stringInfo
    print "INFO: Offline Test completed."

if __name__ == "__main__":
    DUTid = 7
    dose = 0
    startime = time.strftime("%Y-%m-%d_%H-%M-%S")
    t1 = datetime.datetime.now()
    output = "data/offline_board" + str(DUTid) + "_dose" + str(dose) + "_" + str(startime)+".dat"
    print startime, "Offline Test data on file:", output 
    sleep(2)
    offlineTest(output)
