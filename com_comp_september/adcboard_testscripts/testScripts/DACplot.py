import matplotlib.pyplot as plt
from numpy import mean
import numpy as np
import sys

######### MAIN FUNCTION ##############

def DACplot():

  folder = "./results/"
  filename = sys.argv[1]
  datafile = folder + filename + "_ALL" +".txt"

  data = np.loadtxt(datafile)
  data = np.transpose(data)

  DAC_Values = [ 0.25, 0.50, 0.75, 1.00, 1.25, 1.50, 1.75, 2.00, 2.25, 2.49]

  ADC0 = []
  ADC1 = [] 
  ADC2 = []
  ADC3 = []
  ADC4 = []
  ADC5 = []
  ADC6 = []
  ADC7 = []
  ADC8 = []
  ADC9 = []
  ADC10 = []
  ADC11 = []
  ADC12 = []
  ADC13 = []
  ADC14 = []
  ADC15 = []
  ADC16 = []
  ADC17 = []
  ADC18 = []
  ADC19 = []
  ADC20 = []
  ADC21 = []
  ADC22 = []
  ADC23 = []
  ADC24 = []
  ADC25 = []
  ADC26 = []
  ADC27 = []
  ADC28 = []
  ADC29 = []
  ADC30 = []
  ADC31 = []

  ADC0all = data[0]
  ADC1all = data[1]
  ADC2all = data[2]
  ADC3all = data[3]
  ADC4all = data[4]
  ADC5all = data[5]
  ADC6all = data[6]
  ADC7all = data[7]
  ADC8all = data[8]
  ADC9all = data[9]
  ADC10all = data[10]
  ADC11all = data[11]
  ADC12all = data[12]
  ADC13all = data[13]
  ADC14all = data[14]
  ADC15all = data[15]
  ADC16all = data[16]
  ADC17all = data[17]
  ADC18all = data[18]
  ADC19all = data[19]
  ADC20all = data[20]
  ADC21all = data[21]
  ADC22all = data[22]
  ADC23all = data[23]
  ADC24all = data[24]
  ADC25all = data[25]
  ADC26all = data[26]
  ADC27all = data[27]
  ADC28all = data[28]
  ADC29all = data[29]
  ADC30all = data[30]
  ADC31all = data[31]

######################

  adc_averager = []
  adc_final_values = []

  linearx = DAC_Values
  lineary = DAC_Values
  perfectlinear = np.polyfit(linearx, lineary, 2)
  plt.plot(linearx, lineary, marker = "*", label = "Linear Fit")

    
  for i in range(2, len(ADC0all), 1):
    if ((ADC0all[i]) > 0.15) :
      adc_averager.append(ADC0all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
          #print "meanADCavg", mean(adc_averager)
          adc_final_values.append(mean(adc_averager))
          #print adc_averager
          del adc_averager
          adc_averager = []
          if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC0')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()

            chi_squared_ADC0 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC0 chi squared =", chi_squared_ADC0

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC1all), 1):
    if ((ADC1all[i]) > 0.15) :
      adc_averager.append(ADC1all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
          plt.plot(DAC_Values, adc_final_values, label = 'ADC1')
          plt.legend()
          plt.title("ADC Board Test Plot")
          plt.xlabel("DAC Voltage (V)")
          plt.ylabel("ADC Voltage (V)")
          plt.draw()

          chi_squared_ADC1 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
          print "ADC1 chi squared =", chi_squared_ADC1

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC2all), 1):
    if ((ADC2all[i]) > 0.15) :
      adc_averager.append(ADC2all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
          #print "meanADCavg", mean(adc_averager)
          adc_final_values.append(mean(adc_averager))
          del adc_averager
          adc_averager = []
          if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC2')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC2 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC2 chi squared =", chi_squared_ADC2

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC3all), 1):
    if ((ADC3all[i]) > 0.15) :
      adc_averager.append(ADC3all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
          plt.plot(DAC_Values, adc_final_values, label = 'ADC3')
          plt.legend()
          plt.title("ADC Board Test Plot")
          plt.xlabel("DAC Voltage (V)")
          plt.ylabel("ADC Voltage (V)")
          plt.draw()
          chi_squared_ADC3 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
          print "ADC3 chi squared =", chi_squared_ADC3

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC4all), 1):
    if ((ADC4all[i]) > 0.15) :
      adc_averager.append(ADC4all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC4')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC4 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC4 chi squared =", chi_squared_ADC4

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC5all), 1):
    if ((ADC5all[i]) > 0.15) :
      adc_averager.append(ADC5all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC5')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC5 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC5 chi squared =", chi_squared_ADC5

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC6all), 1):
    if ((ADC6all[i]) > 0.15) :
      adc_averager.append(ADC6all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC6')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC6 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC6 chi squared =", chi_squared_ADC6

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC7all), 1):
    if ((ADC7all[i]) > 0.15) :
      adc_averager.append(ADC7all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC7')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC7 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC7 chi squared =", chi_squared_ADC7

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC8all), 1):
    if ((ADC8all[i]) > 0.15) :
      adc_averager.append(ADC8all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC8')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC8 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC8 chi squared =", chi_squared_ADC8

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC9all), 1):
    if ((ADC9all[i]) > 0.15) :
      adc_averager.append(ADC9all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC9')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC9 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC9 chi squared =", chi_squared_ADC9

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC10all), 1):
    if ((ADC10all[i]) > 0.15) :
      adc_averager.append(ADC10all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC10')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC10 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC10 chi squared =", chi_squared_ADC10

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC11all), 1):
    if ((ADC11all[i]) > 0.15) :
      adc_averager.append(ADC11all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC11')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC11 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC11 chi squared =", chi_squared_ADC11

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC12all), 1):
    if ((ADC12all[i]) > 0.15) :
      adc_averager.append(ADC12all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC12')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC12 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC12 chi squared =", chi_squared_ADC12

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC13all), 1):
    if ((ADC13all[i]) > 0.15) :
      adc_averager.append(ADC13all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC13')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC13 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC13 chi squared =", chi_squared_ADC13

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC14all), 1):
    if ((ADC14all[i]) > 0.15) :
      adc_averager.append(ADC14all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC14')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC14 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC14 chi squared =", chi_squared_ADC14

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC15all), 1):
    if ((ADC15all[i]) > 0.15) :
      adc_averager.append(ADC15all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC15')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC15 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC15 chi squared =", chi_squared_ADC15

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC16all), 1):
    if ((ADC16all[i]) > 0.15) :
      adc_averager.append(ADC16all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC16')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC16 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC16 chi squared =", chi_squared_ADC16

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC17all), 1):
    if ((ADC17all[i]) > 0.15) :
      adc_averager.append(ADC17all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC17')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC17 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC17 chi squared =", chi_squared_ADC17

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC18all), 1):
    if ((ADC18all[i]) > 0.15) :
      adc_averager.append(ADC18all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC18')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC18 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC18 chi squared =", chi_squared_ADC18

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC19all), 1):
    if ((ADC19all[i]) > 0.15) :
      adc_averager.append(ADC19all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC19')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC19 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC19 chi squared =", chi_squared_ADC19

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC20all), 1):
    if ((ADC20all[i]) > 0.15) :
      adc_averager.append(ADC20all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC20')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC20 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC20 chi squared =", chi_squared_ADC20

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC21all), 1):
    if ((ADC21all[i]) > 0.15) :
      adc_averager.append(ADC21all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC21')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC21 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC21 chi squared =", chi_squared_ADC21

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC22all), 1):
    if ((ADC22all[i]) > 0.15) :
      adc_averager.append(ADC22all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC22')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC22 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC22 chi squared =", chi_squared_ADC22

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC23all), 1):
    if ((ADC23all[i]) > 0.15) :
      adc_averager.append(ADC23all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC23')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC23 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC23 chi squared =", chi_squared_ADC23

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC24all), 1):
    if ((ADC24all[i]) > 0.15) :
      adc_averager.append(ADC24all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC24')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC24 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC24 chi squared =", chi_squared_ADC24

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC25all), 1):
    if ((ADC25all[i]) > 0.15) :
      adc_averager.append(ADC25all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC25')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC21 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC21 chi squared =", chi_squared_ADC21

  del adc_final_values
  adc_averager = []
  adc_final_values = []
  
  for i in range(2, len(ADC26all), 1):
    if ((ADC26all[i]) > 0.15) :
      adc_averager.append(ADC26all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC26')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC22 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC22 chi squared =", chi_squared_ADC22

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC27all), 1):
    if ((ADC27all[i]) > 0.15) :
      adc_averager.append(ADC27all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC27')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC27 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC27 chi squared =", chi_squared_ADC27

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC28all), 1):
    if ((ADC28all[i]) > 0.15) :
      adc_averager.append(ADC28all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC28')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC28 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC28 chi squared =", chi_squared_ADC28

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC29all), 1):
    if ((ADC29all[i]) > 0.15) :
      adc_averager.append(ADC29all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC29')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC29 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC29 chi squared =", chi_squared_ADC29

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC30all), 1):
    if ((ADC30all[i]) > 0.15) :
      adc_averager.append(ADC30all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC30')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC30 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC30 chi squared =", chi_squared_ADC30

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  for i in range(2, len(ADC31all), 1):
    if ((ADC31all[i]) > 0.15) :
      adc_averager.append(ADC31all[i])
      if ((int(i-1) > 8) and (int(i-1)%10) ==  0):
        #print "meanADCavg", mean(adc_averager)
        adc_final_values.append(mean(adc_averager))
        del adc_averager
        adc_averager = []
        if len(adc_final_values)==10: #for fulltest4 and subsequent change this to be 10!
            plt.plot(DAC_Values, adc_final_values, label = 'ADC31')
            plt.legend()
            plt.title("ADC Board Test Plot")
            plt.xlabel("DAC Voltage (V)")
            plt.ylabel("ADC Voltage (V)")
            plt.draw()
            chi_squared_ADC31 = np.sum((np.polyval(perfectlinear, DAC_Values) - adc_final_values) ** 2)
            print "ADC31 chi squared =", chi_squared_ADC31

  del adc_final_values
  adc_averager = []
  adc_final_values = []

  plt.xlabel("DAC Voltage (V)")
  plt.ylabel("ADC Voltage (V)")
  plt.draw()

  plt.xlim([0,3])
  #fig =plt.figure()
  #ax = plt.gca()
  #ax.set_xticks(np.arange(0,3,0.25))
  #ax.set_yticks(np.arange(0,2.5,0.25))
  plt.grid()
  plt.show()
  #plt.savefig(folder + filename + "_ALL" + ".png")



DACplot()




