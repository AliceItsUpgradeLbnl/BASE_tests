#!/usr/bin/python

#----------------------------------------------------------------------------------------
# Title: ADC Test script plot
# Institution: Lawrence Berkeley National Laboratory
# Author: J. Szornel
# Version: 1.0
# Created: 07-15-16
# Last edited: 08-03-16
# Description: This script tests the RDO board, Mezzanine Card, and ADC board chain for
#              the ITS power system component radiation tolerance test in Sept 2016. The
#              script plots values for 32 ADC channels on the ADC board. The input is a 
#              text file created with the ADC/DAC test. Voltages are plotted vs. sample.
#              syntax for execution is "python ADCplot.py resultsfiletobeplotted"
#----------------------------------------------------------------------------------------

import matplotlib.pyplot as plt
import numpy as np
import sys

def PlotAllADCs():

  data = np.loadtxt(datafile)
  data = np.transpose(data)
  ADC0 = data[0]
  ADC1 = data[1]
  ADC2 = data[2]
  ADC3 = data[3]
  ADC4 = data[4]
  ADC5 = data[5]
  ADC6 = data[6]
  ADC7 = data[7]
  ADC8 = data[8]
  ADC9 = data[9]
  ADC10 = data[10]
  ADC11 = data[11]
  ADC12 = data[12]
  ADC13 = data[13]
  ADC14 = data[14]
  ADC15 = data[15]
  ADC16 = data[16]
  ADC17 = data[17]
  ADC18 = data[18]
  ADC19 = data[19]
  ADC20 = data[20]
  ADC21 = data[21]
  ADC22 = data[22]
  ADC23 = data[23]
  ADC24 = data[24]
  ADC25 = data[25]
  ADC26 = data[26]
  ADC27 = data[27]
  ADC28 = data[28]
  ADC29 = data[29]
  ADC30 = data[30]
  ADC31 = data[31]

  x_steps = folder + filename + "_x_steps.txt"
  xdata = np.loadtxt(x_steps)
  xdata = np.transpose(xdata)
 
  plt.plot(xdata, ADC0, marker = 'o', label = 'ADC0') 
  plt.plot(xdata, ADC1, marker = '*', label = 'ADC1') 
  plt.plot(xdata, ADC2, label = 'ADC2') 
  plt.plot(xdata, ADC3, label = 'ADC3') 
  plt.plot(xdata, ADC4, label = 'ADC4')
  plt.plot(xdata, ADC5, label = 'ADC5')
  plt.plot(xdata, ADC6, label = 'ADC6') 
  plt.plot(xdata, ADC7, label = 'ADC7') 
  plt.plot(xdata, ADC8, label = 'ADC8')
  plt.plot(xdata, ADC9, label = 'ADC9')
  plt.plot(xdata, ADC10, label = 'ADC10')
  plt.plot(xdata, ADC11, label = 'ADC11')
  plt.plot(xdata, ADC12, label = 'ADC12')
  plt.plot(xdata, ADC13, label = 'ADC13')
  plt.plot(xdata, ADC14, label = 'ADC14')
  plt.plot(xdata, ADC15, label = 'ADC15')
  plt.plot(xdata, ADC16, label = 'ADC16') 
  plt.plot(xdata, ADC17, label = 'ADC17')
  plt.plot(xdata, ADC18, label = 'ADC18') 
  plt.plot(xdata, ADC19, label = 'ADC19') 
  plt.plot(xdata, ADC20, label = 'ADC20') 
  plt.plot(xdata, ADC21, label = 'ADC21') 
  plt.plot(xdata, ADC22, label = 'ADC22') 
  plt.plot(xdata, ADC23, label = 'ADC23') 
  plt.plot(xdata, ADC24, label = 'ADC24') 
  plt.plot(xdata, ADC25, label = 'ADC25') 
  plt.plot(xdata, ADC26, label = 'ADC26') 
  plt.plot(xdata, ADC27, label = 'ADC27') 
  plt.plot(xdata, ADC28, label = 'ADC28') 
  plt.plot(xdata, ADC29, label = 'ADC29') 
  plt.plot(xdata, ADC30, label = 'ADC30')
  plt.plot(xdata, ADC31, label = 'ADC31')

  plt.ylim([-0.5,5.0])
  plt.xlim([0,100])
  #plt.xlim([0,4000])

  plt.title("ADC Board Test Plot")
  plt.xlabel("Voltage (V)")
  plt.ylabel("Voltage (V)")
  plt.legend()
  plt.savefig(folder + filename + ".png")
  plt.show()
  plt.close()

  sys.exit()

#---------------------------------------------------------------#
#---------------------------------------------------------------#
#---------------------------------------------------------------#

folder = "./results/"
filename = sys.argv[1]
datafile = folder + filename + "_ALL" +".txt"
PlotAllADCs()
sys.exit()

