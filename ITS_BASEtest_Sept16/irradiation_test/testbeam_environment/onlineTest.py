#!/usr/bin/env python
__author__ = "G. Contin"
__version__ = "2.0"
__status__ = "Prototype"

from time import strftime, sleep
import datetime
import os
import proc
import discardAdcSamples
import ftdIO
import monitoring
import commRDO
import writeCMDreg
from threading import Timer
import time
import shutil
import sys

def printline(line,log):
    print line
    cmdstr="echo "+str(line)+ " >> "  + str(log)
    os.system(cmdstr)

def print_time():
    print "INFO:", time.strftime("%Y-%m-%d_%H-%M-%S")

def onlineTest(output,log):
    displayfile = "data/displayfile.log"
    counter = 0
    wantToExit = False

    # Discarding samples after poweron
    discardAdcSamples.discardAdcSamples()

    # open FTDI device
    commRDO.openRDO()

    # Set global header c3000009
    header = [int("09",16),int("00",16), int("00",16), int("c3",16)]
    with open(output,"a+b") as f:
        f.write(bytearray(header))

    # Configure system for monitoring
    monitoring.configMon()
    line = time.time()
    print line

    # Configure ADCs
    # first set the frequency and the number of samples: 0x0 = 28 clock cycles, 1 sample
    ftdIO.writeReg(384,0x0)

    # Start monitoring
    while True:
        try:
            counter = counter + 1
            line = "Cycle #"+ str(counter)
            printline(line,log)
            print_time()

            for i in range(0,10,1):
                print "read loop %d" % (i)
                writeCMDreg.writeCMDreg("SPIadc","start_adc_ch_31-0")
                # Wait till data is ready
                sleep(0.1)
                # write to file
                ftdIO.readFifoToFile(output,14)
                
            
            f.close()

            # Period
            sleep(8)

            shutil.copy(output,displayfile)

            # set global trailer 'fadecafe' on displayfile    
            trailer = [int("fe",16),int("ca",16),int("de",16),int("fa",16)]
            with open(displayfile,"ab") as f:
                f.write(bytearray(trailer))

            if (wantToExit):
                raise Exception()
    
        except KeyboardInterrupt:
            wantToExit = True
            continue
        
        except:
            # set global trailer 'fadecafe'    
            trailer = [int("fe",16),int("ca",16),int("de",16),int("fa",16)]
            with open(output,"ab") as f:
                f.write(bytearray(trailer))
    
            # Check and close up
            stoptime = time.strftime("%Y-%m-%d_%H-%M-%S")
            print stoptime, "Online Test data on file:", output 
            t2 = datetime.datetime.now()
            stringInfo =  "Duration: " + str(t2.replace(microsecond=0)-t1.replace(microsecond=0))
            print stringInfo
            print "INFO: Online Test completed."
    
            # close FTDI device
            commRDO.closeRDO()

            sys.exit(0)

if __name__ == "__main__":
    DUTid = 7
    dose = 0
    startime = time.strftime("%Y-%m-%d_%H-%M-%S")
    t1 = datetime.datetime.now()
    output = "data/online_board" + str(DUTid) + "_dose" + str(dose) + "_" + str(startime)+".dat"
    log = "Logs/board" + str(DUTid) + "_dose" + str(dose) + "_" + str(startime)+".log"
    print startime, "Online Test data on file:", output 
    sleep(2)
    onlineTest(output, log)
