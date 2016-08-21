#!/usr/bin/python

#----------------------------------------------------------------------------------------
# Title: ADC Test script
# Institution: Lawrence Berkeley National Laboratory
# Author: J. Szornel modifying scripts by G. Contin
# Version: 1.0
# Created: 07-15-16
# Last edited: 08-03-16
# Description: This script tests the RDO board, Mezzanine Card, and ADC board chain for
#              the ITS power system component radiation tolerance test in Sept 2016. The
#              script reads all 32 ADC channels on the ADC board 10 times. These values 
#              are stored in an output text file. The first read does not return proper
#              values. Consequent reads are averaged. The average value and standard
#              deviation for each channel is printed at the end of the text file.
#----------------------------------------------------------------------------------------
from __future__ import division
from time import strftime, sleep
import os
import io
import sys
import proc
import time
import ftdIO
import numpy
import commRDO
import datetime
import writeCMDreg
from threading import Timer


def printline(line,log):
    print line
    cmdstr="echo "+str(line)+ " >> "  + str(log)
    os.system(cmdstr)

def print_time():
    print "INFO:", time.strftime("%Y-%m-%d_%H-%M-%S")

def ReadADCs():

    displayfile = "results/displayfile.log"
    DUTid = str(raw_input("DUTid:" ))
    folder = "./results/"
    filename = "ADCtest" + str(DUTid)
    datafilename = filename
    startime = time.strftime("%Y-%m-%d_%H-%M-%S")
    t1 = datetime.datetime.now()
    output = "results/ADCtest" + str(DUTid) + ".dat"
    log = "results/ADCtest" + str(DUTid) + ".log"

    if os.path.exists(output): # Delete data file with this name if found
        os.remove(output)
    if os.path.exists(log): # Delete data file with this name if found
        os.remove(log)

    print startime, "ADC read test data on file:", output
    time.sleep(2)

    # open FTDI device
    commRDO.openRDO() 
    header = [int("09",16),int("00",16), int("00",16), int("c3",16)]
    with open(output,"a+b") as f:
        f.write(bytearray(header))

    # -> READ ALL THE ADC-BOARD ADCS
    # first set the frequency and the number of samples: 0x0 = 28 clock cycles, 1 sample
    ftdIO.writeReg(384,0x0)
    #readI2C(devName,OpName)
    writeCMDreg.writeCMDreg("SPIadc","start_adc_ch_31-0")
    time.sleep(1)
    # write to file
    ftdIO.readFifoToFile(output,14)

    # Read all ADCs again (discard first read)    
    writeCMDreg.writeCMDreg("SPIadc","start_adc_ch_31-0")
    time.sleep(1)
    ftdIO.readFifoToFile(output,14)

    writeCMDreg.writeCMDreg("SPIadc","start_adc_ch_31-0")
    time.sleep(1)
    ftdIO.readFifoToFile(output,14)

    writeCMDreg.writeCMDreg("SPIadc","start_adc_ch_31-0")
    time.sleep(1)
    ftdIO.readFifoToFile(output,14)

    writeCMDreg.writeCMDreg("SPIadc","start_adc_ch_31-0")
    time.sleep(1)
    ftdIO.readFifoToFile(output,14)

    writeCMDreg.writeCMDreg("SPIadc","start_adc_ch_31-0")
    time.sleep(1)
    ftdIO.readFifoToFile(output,14)
    
    writeCMDreg.writeCMDreg("SPIadc","start_adc_ch_31-0")
    time.sleep(1)
    ftdIO.readFifoToFile(output,14) 
    
    writeCMDreg.writeCMDreg("SPIadc","start_adc_ch_31-0")
    time.sleep(1)
    ftdIO.readFifoToFile(output,14) 
    
    writeCMDreg.writeCMDreg("SPIadc","start_adc_ch_31-0")
    time.sleep(1)
    ftdIO.readFifoToFile(output,14) 
    
    writeCMDreg.writeCMDreg("SPIadc","start_adc_ch_31-0")
    time.sleep(1)
    ftdIO.readFifoToFile(output,14) 
    
    writeCMDreg.writeCMDreg("SPIadc","start_adc_ch_31-0")
    time.sleep(1)
    ftdIO.readFifoToFile(output,14) 

    time.sleep(1)

    # Check and close up
    stoptime = time.strftime("%Y-%m-%d_%H-%M-%S")
    print stoptime, "Read ADC test data on file:", output
    t2 = datetime.datetime.now()
    stringInfo =  "Duration: " + str(t2.replace(microsecond=0)-t1.replace(microsecond=0))
    print stringInfo
    
    # close FTDI device
    commRDO.closeRDO()

    return DUTid

def ADCDataFormat(DUTid): #Reformats .dat file output by ReadADCs function to a .txt file  

  folder = "./results/"
  filename = "ADCtest" + str(DUTid)
  datafilename = filename

  outfile = folder + filename + ".txt"
  infile = folder + filename + ".dat"
  outf = open(outfile, 'w')
  with open(infile, 'rb') as f:
    for byte in iter(lambda: f.read(32), b''):
      data = byte.encode('hex')
      data = data.replace('efbeedfe', 'efbeedfe' + '\n')
      outf.write(data)
  f.close()
  outf.close()

def ADCDataDecoder(DUTid): #Decodes data, calculated mean, stdev for each ADC channel
  
  folder = "./results/"
  filename = "ADCtest" + str(DUTid)
  datafilename = filename
  
  outfile_ALL = folder + filename + "_noise" + ".txt" 
  infile = folder + filename + ".txt"
  outf_ALL = open(outfile_ALL, 'w')
  inf= open(infile, 'r')
  
  startpoint = 0
  globalheader='090000c3'

  numberofsamples = 0

  ADC0sum = []
  ADC1sum = []
  ADC2sum = []
  ADC3sum = []
  ADC4sum = []
  ADC5sum = []
  ADC6sum = []
  ADC7sum = []
  ADC8sum = []
  ADC9sum = []
  ADC10sum = []
  ADC11sum = []
  ADC12sum = []
  ADC13sum = []
  ADC14sum = []
  ADC15sum = []
  ADC16sum = []
  ADC17sum = []
  ADC18sum = []
  ADC19sum = []
  ADC20sum = []
  ADC21sum = []
  ADC22sum = []
  ADC23sum = []
  ADC24sum = []
  ADC25sum = []
  ADC26sum = []
  ADC27sum = []
  ADC28sum = []
  ADC29sum = []
  ADC30sum = []
  ADC31sum = []

  with inf as file:
    for line in inf:

        if line[startpoint + 0: startpoint +8] == globalheader:
            startpoint = 9
        elif line[startpoint + 0 : startpoint + 8] == 'fecadefa':
            print "end of file."
            break
        else:
            startpoint = 1

        ADC3_low = line[startpoint + 13: startpoint + 14]
        ADC2_low = line[startpoint + 14: startpoint + 15]
        ADC1 = line[startpoint + 11: startpoint + 13] + line[startpoint + 9: startpoint + 10]
        ADC0 = line[startpoint + 10: startpoint + 11] + line[startpoint + 7: startpoint + 9]
        ADC5_low= line[startpoint + 21: startpoint + 22]
        ADC4_low= line[startpoint + 22: startpoint + 23]
        ADC4_high= line[startpoint + 19: startpoint + 21]
        ADC3_high= line[startpoint + 17: startpoint + 19]
        ADC2_high= line[startpoint + 15: startpoint + 17]
        ADC7_low= line[startpoint + 29: startpoint + 30]
        ADC6_low= line[startpoint + 30: startpoint + 31]
        ADC7_high= line[startpoint + 27: startpoint + 29]
        ADC6_high= line[startpoint + 25: startpoint + 27]
        ADC5_high= line[startpoint + 23: startpoint + 25]
        ADC11_low= line[startpoint + 37: startpoint + 38]
        ADC10_low= line[startpoint + 38: startpoint + 39]
        ADC9= line[startpoint + 35: startpoint + 37] + line[startpoint + 33: startpoint + 34]
        ADC8= line[startpoint + 34: startpoint + 35] + line[startpoint + 31: startpoint + 33]
        ADC13_low= line[startpoint + 45: startpoint + 46]
        ADC12_low= line[startpoint + 46: startpoint + 47]
        ADC12_high= line[startpoint + 43: startpoint + 45]
        ADC11_high=line[startpoint + 41: startpoint + 43]
        ADC10_high=line[startpoint + 39: startpoint + 41]
        ADC15_low=line[startpoint + 53: startpoint + 54]
        ADC14_low=line[startpoint + 54: startpoint + 55]
        ADC15_high=line[startpoint + 51: startpoint + 53]
        ADC14_high=line[startpoint + 49: startpoint + 51]
        ADC13_high=line[startpoint + 47: startpoint + 49]
        ADC19_low=line[startpoint + 61: startpoint + 62]
        ADC18_low=line[startpoint + 62: startpoint + 63]
        ADC17=line[startpoint + 59: startpoint + 61] + line[startpoint + 57: startpoint + 58]
        ADC16=line[startpoint + 58: startpoint + 59] + line[startpoint + 55: startpoint + 57]
        ADC21_low=line[startpoint + 69: startpoint + 70]
        ADC20_low=line[startpoint + 70: startpoint + 71]
        ADC20_high=line[startpoint + 67: startpoint + 69]
        ADC19_high=line[startpoint + 65: startpoint + 67]
        ADC18_high=line[startpoint + 63: startpoint + 65]
        ADC23_low=line[startpoint + 77: startpoint + 78]
        ADC22_low=line[startpoint + 78: startpoint + 79]
        ADC23_high=line[startpoint + 75: startpoint + 77]
        ADC22_high=line[startpoint + 73: startpoint + 75]
        ADC21_high=line[startpoint + 71: startpoint + 73]
        ADC27_low=line[startpoint + 85: startpoint + 86]
        ADC26_low=line[startpoint + 86: startpoint + 87]
        ADC25=line[startpoint + 83: startpoint + 85] + line[startpoint + 81: startpoint + 82]
        ADC24=line[startpoint + 82: startpoint + 83] + line[startpoint + 79: startpoint + 81]
        ADC29_low=line[startpoint + 93: startpoint + 94]
        ADC28_low=line[startpoint + 94: startpoint + 95]
        ADC28_high=line[startpoint + 91: startpoint + 93]
        ADC27_high=line[startpoint + 89: startpoint + 91]
        ADC26_high=line[startpoint + 87: startpoint + 89]
        ADC31_low=line[startpoint + 101: startpoint + 102]
        ADC30_low=line[startpoint + 102: startpoint + 103]
        ADC31_high=line[startpoint + 99: startpoint + 101]
        ADC30_high=line[startpoint + 97: startpoint + 99]
        ADC29_high=line[startpoint + 95: startpoint + 97]               
        trailer=line[startpoint + 103: startpoint + 111]

        if trailer == '':
            break
      
        ADC0 = ADC0
        ADC1 = ADC1
        ADC2 = ADC2_high + ADC2_low
        ADC3 = ADC3_high + ADC3_low
        ADC4 = ADC4_high + ADC4_low
        ADC5 = ADC5_high + ADC5_low
        ADC6 = ADC6_high + ADC6_low
        ADC7 = ADC7_high + ADC7_low
        ADC8 = ADC8
        ADC9 = ADC9
        ADC10 = ADC10_high + ADC10_low
        ADC11 = ADC11_high + ADC11_low
        ADC12 = ADC12_high + ADC12_low
        ADC13 = ADC13_high + ADC13_low
        ADC14 = ADC14_high + ADC14_low
        ADC15 = ADC15_high + ADC15_low
        ADC16 = ADC16
        ADC17 = ADC17
        ADC18 = ADC18_high + ADC18_low
        ADC19 = ADC19_high + ADC19_low
        ADC20 = ADC20_high + ADC20_low
        ADC21 = ADC21_high + ADC21_low
        ADC22 = ADC22_high + ADC22_low
        ADC23 = ADC23_high + ADC23_low
        ADC24 = ADC24
        ADC25 = ADC25
        ADC26 = ADC26_high + ADC26_low
        ADC27 = ADC27_high + ADC27_low
        ADC28 = ADC28_high + ADC28_low
        ADC29 = ADC29_high + ADC29_low
        ADC30 = ADC30_high + ADC30_low   
        ADC31 = ADC31_high + ADC31_low
        
        ADC0 = ((int(ADC0, 16) & 0x00000fff)*(2.5/4096))
        ADC1 = ((int(ADC1, 16) & 0x00000fff)*(2.5/4096))
        ADC2 = ((int(ADC2, 16) & 0x00000fff)*(2.5/4096))
        ADC3 = ((int(ADC3, 16) & 0x00000fff)*(2.5/4096))
        ADC4 = ((int(ADC4, 16) & 0x00000fff)*(2.5/4096))
        ADC5 = ((int(ADC5, 16) & 0x00000fff)*(2.5/4096))
        ADC6 = ((int(ADC6, 16) & 0x00000fff)*(2.5/4096))
        ADC7 = ((int(ADC7, 16) & 0x00000fff)*(2.5/4096))
        ADC8 = ((int(ADC8, 16) & 0x00000fff)*(2.5/4096))
        ADC9 = ((int(ADC9, 16) & 0x00000fff)*(2.5/4096))
        ADC10 = ((int(ADC10, 16) & 0x00000fff)*(2.5/4096))
        ADC11 = ((int(ADC11, 16) & 0x00000fff)*(2.5/4096))
        ADC12 = ((int(ADC12, 16) & 0x00000fff)*(2.5/4096))
        ADC13 = ((int(ADC13, 16) & 0x00000fff)*(2.5/4096))
        ADC14 = ((int(ADC14, 16) & 0x00000fff)*(2.5/4096))
        ADC15 = ((int(ADC15, 16) & 0x00000fff)*(2.5/4096))
        ADC16 = ((int(ADC16, 16) & 0x00000fff)*(5.0/4096))
        ADC17 = ((int(ADC17, 16) & 0x00000fff)*(5.0/4096))
        ADC18 = ((int(ADC18, 16) & 0x00000fff)*(5.0/4096))
        ADC19 = ((int(ADC19, 16) & 0x00000fff)*(5.0/4096))
        ADC20 = ((int(ADC20, 16) & 0x00000fff)*(5.0/4096))
        ADC21 = ((int(ADC21, 16) & 0x00000fff)*(5.0/4096))
        ADC22 = ((int(ADC22, 16) & 0x00000fff)*(5.0/4096))
        ADC23 = ((int(ADC23, 16) & 0x00000fff)*(5.0/4096))
        ADC24 = ((int(ADC24, 16) & 0X00000fff)*(5.0/4096))
        ADC25 = ((int(ADC25, 16) & 0X00000fff)*(5.0/4096))
        ADC26 = ((int(ADC26, 16) & 0x00000fff)*(5.0/4096))
        ADC27 = ((int(ADC27, 16) & 0X00000fff)*(5.0/4096))
        ADC28 = ((int(ADC28, 16) & 0X00000fff)*(5.0/4096))
        ADC29 = ((int(ADC29, 16) & 0X00000fff)*(5.0/4096))
        ADC30 = ((int(ADC30, 16) & 0X00000fff)*(5.0/4096))
        ADC31 = ((int(ADC31, 16) & 0X00000fff)*(5.0/4096))

        if numberofsamples > 0:
            ADC0sum.append(ADC0)
            ADC1sum.append(ADC1)
            ADC2sum.append(ADC2)
            ADC3sum.append(ADC3)
            ADC4sum.append(ADC4)
            ADC5sum.append(ADC5)
            ADC6sum.append(ADC6)
            ADC7sum.append(ADC7)
            ADC8sum.append(ADC8)
            ADC9sum.append(ADC9)
            ADC10sum.append(ADC10)
            ADC11sum.append(ADC11)
            ADC12sum.append(ADC12)
            ADC13sum.append(ADC13)
            ADC14sum.append(ADC14)
            ADC15sum.append(ADC15)
            ADC16sum.append(ADC16)
            ADC17sum.append(ADC17)
            ADC18sum.append(ADC18)
            ADC19sum.append(ADC19)
            ADC20sum.append(ADC20)
            ADC21sum.append(ADC21)
            ADC22sum.append(ADC22)
            ADC23sum.append(ADC23)
            ADC24sum.append(ADC24)
            ADC25sum.append(ADC25)
            ADC26sum.append(ADC26)
            ADC27sum.append(ADC27)
            ADC28sum.append(ADC28)
            ADC29sum.append(ADC29)
            ADC30sum.append(ADC30)
            ADC31sum.append(ADC31)


        outf_ALL.write("ADC0: " + str(ADC0) + '\n' + "ADC1: " + str(ADC1) + '\n' + "ADC2: " + str(ADC2) + '\n' + "ADC3: " + str(ADC3) + '\n' +
                       "ADC4: " + str(ADC4) + '\n' + "ADC5: " + str(ADC5) + '\n' + "ADC6: " + str(ADC6) + '\n' + "ADC7: " + str(ADC7) + '\n' +
                       "ADC8: " + str(ADC8) + '\n' + "ADC9: " + str(ADC9) + '\n' + "ADC10: " + str(ADC10) + '\n' + "ADC11: " + str(ADC11) + '\n' + 
                       "ADC12: " + str(ADC12) + '\n' + "ADC13: " + str(ADC13) + '\n' + "ADC14: " + str(ADC14) + '\n' + "ADC15: " + str(ADC15) + '\n' + 
                       "ADC16: " + str(ADC16) + '\n' + "ADC17: " + str(ADC17) + '\n' + "ADC18: " + str(ADC18) + '\n' + "ADC19: " + str(ADC19) + '\n' + 
                       "ADC20: " + str(ADC20) + '\n' + "ADC21: " + str(ADC21) + '\n' + "ADC22: " + str(ADC22) + '\n' + "ADC23: " + str(ADC23) + '\n' +
                       "ADC24: " + str(ADC24) + '\n' + "ADC25: " + str(ADC25) + '\n' + "ADC26: " + str(ADC26) + '\n' + "ADC27: " + str(ADC27) + '\n' + 
                       "ADC28: " + str(ADC28) + '\n' + "ADC29: " + str(ADC29) + '\n' + "ADC30: " + str(ADC30) + '\n' + "ADC31: " + str(ADC31) + '\n' )
 
        numberofsamples = numberofsamples + 1

        if trailer == 'efbeedfe' or trailer =='efbeedfe':
          continue
        elif trailer =='fecadefa':
          break
        elif trailer == '':
            break
        else:
          print "Error! Incorrect trailer found!"
          print "trailer:" + trailer + ":"
          break

  outf_ALL.write("\n" + "-----------------------------------------------" + "\n" + "\n" +
                        "-------- means and standard deviations --------"  + "\n" + "\n" +
                        "ADC          mean            standard deviation"  + "\n" + "\n" +
                        "-----------------------------------------------" + "\n" + "\n")

  
 
  outf_ALL.write("ADC0:   " + str(numpy.mean(ADC0sum)) + "        " + str(numpy.std(ADC0sum)) + '\n' +
                 "ADC1:   " + str(numpy.mean(ADC1sum)) + "        " + str(numpy.std(ADC1sum)) + '\n' + 
                 "ADC2:   " + str(numpy.mean(ADC2sum)) + "        " + str(numpy.std(ADC2sum)) + '\n' +
                 "ADC3:   " + str(numpy.mean(ADC3sum)) + "        " + str(numpy.std(ADC3sum)) + '\n' +
                 "ADC4:   " + str(numpy.mean(ADC4sum)) + "        " + str(numpy.std(ADC4sum)) + '\n' +
                 "ADC5:   " + str(numpy.mean(ADC5sum)) + "        " + str(numpy.std(ADC5sum)) + '\n' +
                 "ADC6:   " + str(numpy.mean(ADC6sum)) + "        " + str(numpy.std(ADC6sum)) + '\n' +
                 "ADC7:   " + str(numpy.mean(ADC7sum)) + "        " + str(numpy.std(ADC7sum)) + '\n' + 
                 "ADC8:   " + str(numpy.mean(ADC8sum)) + "        " + str(numpy.std(ADC8sum)) + '\n' + 
                 "ADC10:  " + str(numpy.mean(ADC10sum)) + "        " + str(numpy.std(ADC10sum)) + '\n' +
                 "ADC11:  " + str(numpy.mean(ADC11sum)) + "        " + str(numpy.std(ADC11sum)) + '\n' + 
                 "ADC12:  " + str(numpy.mean(ADC12sum)) + "        " + str(numpy.std(ADC12sum)) + '\n' +
                 "ADC13:  " + str(numpy.mean(ADC13sum)) + "        " + str(numpy.std(ADC13sum)) + '\n' +
                 "ADC14:  " + str(numpy.mean(ADC14sum)) + "        " + str(numpy.std(ADC14sum)) + '\n' +
                 "ADC15:  " + str(numpy.mean(ADC15sum)) + "        " + str(numpy.std(ADC15sum)) + '\n' +
                 "ADC16:  " + str(numpy.mean(ADC16sum)) + "        " + str(numpy.std(ADC16sum)) + '\n' +
                 "ADC17:  " + str(numpy.mean(ADC17sum)) + "        " + str(numpy.std(ADC17sum)) + '\n' + 
                 "ADC18:  " + str(numpy.mean(ADC18sum)) + "        " + str(numpy.std(ADC18sum)) + '\n' + 
                 "ADC19:  " + str(numpy.mean(ADC19sum)) + "        " + str(numpy.std(ADC19sum)) + '\n' +
                 "ADC20:  " + str(numpy.mean(ADC20sum)) + "        " + str(numpy.std(ADC20sum)) + '\n' +
                 "ADC21:  " + str(numpy.mean(ADC21sum)) + "        " + str(numpy.std(ADC21sum)) + '\n' + 
                 "ADC22:  " + str(numpy.mean(ADC22sum)) + "        " + str(numpy.std(ADC22sum)) + '\n' +
                 "ADC23:  " + str(numpy.mean(ADC23sum)) + "        " + str(numpy.std(ADC23sum)) + '\n' +
                 "ADC24:  " + str(numpy.mean(ADC24sum)) + "        " + str(numpy.std(ADC24sum)) + '\n' +
                 "ADC25:  " + str(numpy.mean(ADC25sum)) + "        " + str(numpy.std(ADC25sum)) + '\n' +
                 "ADC26:  " + str(numpy.mean(ADC26sum)) + "        " + str(numpy.std(ADC26sum)) + '\n' +
                 "ADC27:  " + str(numpy.mean(ADC27sum)) + "        " + str(numpy.std(ADC27sum)) + '\n' + 
                 "ADC28:  " + str(numpy.mean(ADC28sum)) + "        " + str(numpy.std(ADC28sum)) + '\n' + 
                 "ADC29:  " + str(numpy.mean(ADC29sum)) + "        " + str(numpy.std(ADC29sum)) + '\n' +
                 "ADC30:  " + str(numpy.mean(ADC30sum)) + "        " + str(numpy.std(ADC30sum)) + '\n' +
                 "ADC31:  " + str(numpy.mean(ADC31sum)) + "        " + str(numpy.std(ADC31sum)) + '\n' )
  
  ADC0sum = []
  ADC1sum = []
  ADC2sum = []
  ADC3sum = []
  ADC4sum = []
  ADC5sum = []
  ADC6sum = []
  ADC7sum = []
  ADC8sum = []
  ADC9sum = []
  ADC10sum = []
  ADC11sum = []
  ADC12sum = []
  ADC13sum = []
  ADC14sum = []
  ADC15sum = []
  ADC16sum = []
  ADC17sum = []
  ADC18sum = []
  ADC19sum = []
  ADC20sum = []
  ADC21sum = []
  ADC22sum = []
  ADC23sum = []
  ADC24sum = []
  ADC25sum = []
  ADC26sum = []
  ADC27sum = []
  ADC28sum = []
  ADC29sum = []
  ADC30sum = []
  ADC31sum = []

  inf.close()
  outf_ALL.close()
  os.remove(infile)
  print "Test complete"
  

if __name__ == "__main__":

    os.system("clear")

    print "-----------------------------------------------------------------------------\n"
    print "---------------------------- ADC board test ---------------------------------\n"
    print "--------------------------------Read ADCs------------------------------------\n"

    DUTid = ReadADCs()
    time.sleep(1)
    print "ADC test complete"
    ADCDataFormat(DUTid)
    ADCDataDecoder(DUTid)

    print "------------------------------------------------------------------------#-----\n"
    print "-------------------------- end of ADC board test -----------------------#-----\n"
    print "------------------------------------------------------------------------#-----\n"


