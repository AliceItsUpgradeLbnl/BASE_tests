#!/usr/bin/env python
#author: J. Szornel
#Version: "1.0"
#Date Created: April 22, 2016
#Description: to create one data file with relevant information for plotting "online" data files. First the data is reformatted into a text file, then decoded, then the dose is calculated from a user input

from __future__ import division
from time import strftime, sleep
import datetime
import numpy
import os
import sys

folder = "./results/"
filename = sys.argv[1]

totalsamplenumber = 0

def ADCTestDataFormat(filename):
  
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

def ADCTestDataDecoder(filename):

  folder = "./results/"
  outfile_ALL = folder + filename + "_ALL" + ".txt"
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

        outf_ALL.write(str(ADC0) + " " + str(ADC1) + " " + str(ADC2) + " " + str(ADC3) + " "+ str(ADC4) + " "+ str(ADC5) + " "+ str(ADC6) + " "+ str(ADC7) + " "+ str(ADC8) + " "+ str(ADC9) + " "+ str(ADC10) + " "+ str(ADC11) + " "+ str(ADC12) + " "+ str(ADC13) + " "+ str(ADC14) + " "+ str(ADC15) + " "+ str(ADC16) + " "+ str(ADC17) + " "+ str(ADC18) + " "+ str(ADC19) + " "+ str(ADC20) + " "+ str(ADC21) + " "+ str(ADC22) + " "+ str(ADC23) + " " + str(ADC24) + " " + str(ADC25) + " " + str(ADC26) + " " + str(ADC27) + " " + str(ADC28) + " " + str(ADC29) + " " + str(ADC30) + " " + str(ADC31) + '\n')
 
        numberofsamples = numberofsamples + 1

        if trailer == 'efbeedfe' or trailer =='efbeedfe':
          continue
        elif trailer =='fecadefa':
          break
        else:
          print "trailer:" + trailer + ":"
          print "Error! Incorrect trailer found!"
          break

  inf.close()
  outf_ALL.close()

def LineCounter(filename):
  infile = folder + filename + "_ALL" + ".txt"
  global totalsamplenumber
  totalsamplenumber = 0
 
  inf = open(infile, 'r')
  totalsamplenumber = sum(1 for line in inf)
  inf.close()
  print str(totalsamplenumber)
  return totalsamplenumber

def DummyXVariable (filename):
  folder = "./results/"
  outfile_x_steps = folder + filename + "_x_steps" + ".txt"
  infile = folder + filename + "_ALL" + ".txt"
  
  outf_x_steps = open(outfile_x_steps, 'w')
  inf = open(infile, 'r')
 
  i = 0

  for line in inf:
    outf_x_steps.write(str(i) + '\n')
    i = i + 1

def main():
    
  print filename
  ADCTestDataFormat(filename)
  print "reformatting complete!"
  ADCTestDataDecoder(filename)
  print "decoding complete!"
  #LineCounter(filename)
  #print "line count complete!"
  DummyXVariable(filename)
  print "x variable complete!"

#                           execution
#---------------------------------------------------------------#
#---------------------------------------------------------------#
#---------------------------------------------------------------#

main()
