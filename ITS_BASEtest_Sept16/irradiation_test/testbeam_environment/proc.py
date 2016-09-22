#!/usr/bin/env python
__author__ = "G. Contin"
__version__ = "1.0"
__status__ = "Prototype"

import os
import sys
import ftdIO
from time import sleep
path = os.getcwd() + "/tables/"
#print path

def writeAD5624_adcboard(cmd,data,target):
    device = "AD5624"
    fname = path+"ADCDACBOARD_I2C_SPI.reg"
    l = []
    header = []
    found = sladdr = DB0 = DB1 = DB2 = 0
    data_low = data_high = 0
    with open(fname,'r') as f:
        header = next(f).split()
        for line in f:
            l = line.split()
            if len(l)!=15:
                print "ERROR: wrong numbers of arguments for",l[0],l[1], "in ADCDACBOARD_I2C_SPI.reg!"
                exit(-1)
            if ((l[0]==device)&(l[1]==cmd)):
                # build words
                found = 1
                data = int(data,16) << 4
                DB0 = int(l[3],16)
                DB1 = (data >> 8) & 0xff
                DB2 = data & 0xf0
                data_low = DB2 | (DB1 << 8)
                data_high = DB0 & 0xff
                
        if found == 0:
            print "ERROR: no line found in ADCDACBOARD_I2C_SPI.reg"
            exit(-1)
        
        ftdIO.writeReg(192, data_low)
        ftdIO.writeReg(256, data_high)

        targetcmd = "0x1" + str(target) + "3"
        print "Main command reg:", str(targetcmd)
        
        ftdIO.writeReg(128,int(targetcmd,16))
        sleep(0.1)
        ftdIO.writeReg(128,int(targetcmd,16))
        sleep(0.1)
       
def writeAD5254_adcboard(cmd,data,target):
    device = "AD5254"
    fname = path+"ADCDACBOARD_I2C_SPI.reg"
    l = []
    header = []
    found = sladdr = DB0 = DB1 = DB2 = 0
    data_low = data_high = 0
    with open(fname,'r') as f:
        header = next(f).split()
        for line in f:
            l = line.split()
            if len(l)!=15:
                print "ERROR: wrong numbers of arguments for",l[0],l[1], "in ADCDACBOARD_I2C_SPI.reg!"
                exit(-1)
            if ((l[0]==device)&(l[1]==cmd)):
#                print "INFO: Executing operation", l[1], "on device", l[0] 
                # build words
                found = 1
                data = int(data,16)
                sladdr = int(l[2],16)
                if l[3] != "-":
                    DB0 = int(l[3],16)
                if l[4] == "*":
                    DB1 = data & 0xff
                # for I2C
                # data_low = address | DB0 << 8
                # data_high = DB1 | DB2 << 8    
                data_low = sladdr | DB0 << 8
                data_high = DB1 & 0xff
                
                #print bin(data_low), bin(data_high)   
                #print hex(data_low), hex(data_high)
                    
#                if found == 1:
#                    print "INFO: sending words", bin(data_low), bin(data_high), hex(data_low), hex(data_high) 
                
        if found == 0:
            print "ERROR: no line found in ADCDACBOARD_I2C_SPI.reg"
            exit(-1)
        
        ftdIO.writeReg(192, data_low)
        ftdIO.writeReg(256, data_high)

        #  
        targetcmd = "0x3" + str(target) + "2"
        print "Main command reg:", str(targetcmd)
        
        ftdIO.writeReg(128,int(targetcmd,16))
        ftdIO.writeReg(128,int(targetcmd,16))

def writeAD5254_dutboard(cmd,data,target):
    device = "AD5254"
    fname = path+"DUT_I2C_SPI.reg"
    l = []
    header = []
    found = sladdr = DB0 = DB1 = DB2 = 0
    data_low = data_high = 0
    with open(fname,'r') as f:
        header = next(f).split()
        for line in f:
            l = line.split()
            if len(l)!=15:
                print "ERROR: wrong numbers of arguments for",l[0],l[1], "in ADCDACBOARD_I2C_SPI.reg!"
                exit(-1)
            if ((l[0]==device)&(l[1]==cmd)):
                print "INFO: Executing operation", l[1], "on device", l[0] 
                # build words
                found = 1
                data = int(data,16)
                sladdr = int("2C",16)
                if l[3] != "-":
                    DB0 = int(l[3],16)
                if l[4] == "*":
                    DB1 = data & 0xff
                # for I2C
                # data_low = address | DB0 << 8
                # data_high = DB1 | DB2 << 8    
                data_low = sladdr | DB0 << 8
                data_high = DB1 & 0xff
                
                #print bin(data_low), bin(data_high)   
                #print hex(data_low), hex(data_high)
                    
                if found == 1:
                    print "INFO: sending words", bin(data_low), bin(data_high) 
                
        if found == 0:
            print "ERROR: no line found in DUT_I2C_SPI.reg"
            exit(-1)
        
        ftdIO.writeReg(192, data_low)
        ftdIO.writeReg(256, data_high)

        #  
        targetcmd = "0x4" + str(target) + "2"
        print "Main command reg:", str(targetcmd)
        
        ftdIO.writeReg(128,int(targetcmd,16))
        ftdIO.writeReg(128,int(targetcmd,16))



def writeAD9833_adcboard(cmd,data,target):
    device = "AD9833"
    fname = path+"ADCDACBOARD_I2C_SPI.reg"
    l = []
    header = []
    found = sladdr = DB0 = DB1 = DB2 = 0
    data_low = data_high = 0
    with open(fname,'r') as f:
        header = next(f).split()
        for line in f:
            l = line.split()
            if len(l)!=15:
                print "ERROR: wrong numbers of arguments for",l[0],l[1], "in ADCDACBOARD_I2C_SPI.reg!"
                exit(-1)
            if ((l[0]==device)&(l[1]==cmd)):
                print "INFO: Executing operation", l[1], "on device", l[0] 
                # build words
                found = 1
                if l[3] != "*":
                    DB0 = int(l[3],16)
                else:
                    DB0 = (int(data,16) >> 8) & 0xff
                if l[4] != "*":
                    DB1 = int(l[4],16)
                else:
                    DB1 = int(data,16) & 0xff
                # for SPI
                # data_low = DB2 | DB1 << 8
                # data_high = DB0 & 0xff

                data_low = DB2 | DB1 << 8
                data_high = DB0 & 0xff
                # print bin(data_low), bin(data_high)   
                # print hex(data_low), hex(data_high)
                    
                if found == 1:
                    print "INFO: sending word", bin(data_low), bin(data_high), hex(data_low), hex(data_high)
                    
        if found == 0:
            print "ERROR: no line found in ADCDACBOARD_I2C_SPI.reg"
            exit(-1)
        
        ftdIO.writeReg(192, data_low)
        print "REad data low:", ftdIO.readReg(192)
        ftdIO.writeReg(256, data_high)
        print "REad data high:", ftdIO.readReg(256)
        #  
        targetcmd = "0x2" + str(target) + "2"
        print "Main command reg:", str(targetcmd)
        
        ftdIO.writeReg(128,int(targetcmd,16))
        

if __name__ == "__main__":
    if(len(sys.argv) != 3):
        print "ERROR: wrong number of arguments for", sys.argv[0]
        exit(-1)
    else:
        #device = sys.argv[1]
        cmd = sys.argv[1]
        data = sys.argv[2]
    commRDO.openRDO()
    writeAD5624_adcboard(cmd,data)
#    writeAD5254_adcboard(cmd,data):  
 #   writeAD9833_adcboard(cmd,data)
    commRDO.closeRDO()
