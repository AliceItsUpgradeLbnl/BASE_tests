#!/usr/bin/env python
__author__ = "G. Contin"
__version__ = "1.0"
__status__ = "Prototype"

import os
import sys
import ftdIO
import commRDO
from time import sleep
path = os.getcwd() + "/tables/"
#print path



def write_dutboard(device,ndevice,cmd,data,target,nbytes):
   
    fname = path+"DUT_I2C_SPI.reg"
    l = []
    header = []
    found = sladdr = DB0 = DB1 = DB2 = 0
    data_low = data_high = 0
    with open(fname,'r') as f:
        header = next(f).split()
        for line in f:
            l = line.split()
            if ((l[0]==device)&(l[1]==cmd)):
                print "INFO: Executing operation", l[1], "on device", l[0] , l[2], l[3], l[4]

                # build words
                found = 1
                data = int(data,16)
                if l[2] != "-":
                    sladdr = int(l[2],16)
                if (l[3] != "-" and l[3] != "*") :
                    DB0 = int(l[3],16)
                if (l[3] == "*"):
                    DB0 = data
                if l[4] == "*":
                    DB1 = ((data >> 4) & 0xFF)
                if l[5] == "*":
                    DB2 = ((data << 4) & 0xF0)
                print "debug............", hex(sladdr), hex(DB0), hex(DB1), hex(DB2)

                data_low = (sladdr & 0xFF) | ((DB0 << 8) & 0xFF00)
                data_high = (DB1 & 0xff) | ((DB2 << 8) & 0xFF00)
                    
                if found == 1:
                    print "INFO: sending words", bin(data_low), bin(data_high),hex(data_low), hex(data_high)  
                
        if found == 0:
            print "ERROR: no line found in DUT_I2C_SPI.reg"
            exit(-1)
        print "INFO: sending words", hex(data_low), hex(data_high) 
        targetcmd = "0x" +str(ndevice) + str(target) + str(nbytes)
        print "Main command reg:", str(targetcmd)

        ftdIO.writeReg(192, data_low)
        ftdIO.writeReg(256, data_high)    
        
        ftdIO.writeReg(128,int(targetcmd,16))



def writeTCA9535_dutboard(device,ndevice,cmd,data,target,nbytes):
   
    fname = path+"DUT_I2C_SPI.reg"
    l = []
    header = []
    found = sladdr = DB0 = DB1 = DB2 = 0
    data_low = data_high = 0
    with open(fname,'r') as f:
        header = next(f).split()
        for line in f:
            l = line.split()
            if ((l[0]==device)&(l[1]==cmd)):
                print "INFO: Executing operation", l[1], "on device", l[0] , l[2], l[3], l[4]

                # build words
                found = 1
                data = int(data,16)
                if l[2] != "-":
                    sladdr = int(l[2],16)
                if (l[3] != "-" and l[3] != "*") :
                    DB0 = int(l[3],16)
                if (l[3] == "*"):
                    DB0 = data
                if l[4] == "*":
                    DB1 = (data & 0xFF)
                if l[5] == "*":
                    DB2 = ((data >> 8) & 0xFF)

                print "debug............", hex(sladdr), hex(DB0), hex(DB1), hex(DB2)  
                data_low = (sladdr & 0xFF) | ((DB0 << 8) & 0xFF00)
                data_high = (DB1 & 0xFF) | ((DB2 << 8) & 0xFF00)
                
                    
                if found == 1:
                    print "INFO: sending words", bin(data_low), bin(data_high),hex(data_low), hex(data_high)  
                
        if found == 0:
            print "ERROR: no line found in DUT_I2C_SPI.reg"
            exit(-1)
        print "INFO: sending words", hex(data_low), hex(data_high) 
        targetcmd = "0x" +str(ndevice) + str(target) + str(nbytes)
        print "Main command reg:", str(targetcmd)

        ftdIO.writeReg(192, data_low)
        ftdIO.writeReg(256, data_high)
        
        ftdIO.writeReg(128,int(targetcmd,16))



def writeAD7997_dutboard(device,ndevice,cmd,data,target,nbytes):
   
    fname = path+"DUT_I2C_SPI.reg"
    l = []
    header = []
    found = sladdr = DB0 = DB1 = DB2 = 0
    data_low = data_high = 0
    with open(fname,'r') as f:
        header = next(f).split()
        for line in f:
            l = line.split()
          #  if len(l)!=15:
           #     print "ERROR: wrong numbers of arguments for",l[0],l[1], "in ADCDACBOARD_I2C_SPI.reg!"
            #    exit(-1)
            if ((l[0]==device)&(l[1]==cmd)):
                print "INFO: Executing operation", l[1], "on device", l[0] , l[2], l[3], l[4]

                # build words
                found = 1
                data = int(data,16)
                if l[2] != "-":
                    sladdr = int(l[2],16)
                if (l[3] != "-" and l[3] != "*") :
                    DB0 = int(l[3],16)
                if (l[3] == "*"):
                    DB0 = data
                if l[4] == "*":
                    DB1 = (data & 0xFF)
                if l[5] == "*":
                    DB2 = ((data >> 8) & 0xFF)
                print "debug............", hex(sladdr), hex(DB0), hex(DB1), hex(DB2)
                # for I2C
                # data_low = address | DB0 << 8
                # data_high = DB1 | DB2 << 8    
                data_low = (sladdr & 0xFF) | ((DB0 << 8) & 0xFF00)
                data_high = (DB1 & 0xff) | ((DB2 << 8) & 0xFF00)
                
                #print "bin", bin(data_low), bin(data_high)   
                #print "hex" hex(data_low), hex(data_high),
                    
                if found == 1:
                    print "INFO: sending words", bin(data_low), bin(data_high),hex(data_low), hex(data_high)  
                
        if found == 0:
            print "ERROR: no line found in DUT_I2C_SPI.reg"
            exit(-1)
        print "INFO: sending words", hex(data_low), hex(data_high) 
        targetcmd = "0x" +str(ndevice) + str(target) + str(nbytes)
        print "Main command reg:", str(targetcmd)

        ftdIO.writeReg(192, data_low)
        ftdIO.writeReg(256, data_high)
        
        ftdIO.writeReg(128,int(targetcmd,16))



def writeADC128D818_dutboard(device,ndevice,cmd,data,target,nbytes):
   
    fname = path+"DUT_I2C_SPI.reg"
    l = []
    header = []
    found = sladdr = DB0 = DB1 = DB2 = 0
    data_low = data_high = 0
    with open(fname,'r') as f:
        header = next(f).split()
        for line in f:
            l = line.split()
          #  if len(l)!=15:
           #     print "ERROR: wrong numbers of arguments for",l[0],l[1], "in ADCDACBOARD_I2C_SPI.reg!"
            #    exit(-1)
            if ((l[0]==device)&(l[1]==cmd)):
                print "INFO: Executing operation", l[1], "on device", l[0] , l[2], l[3], l[4]

                # build words
                found = 1
                data = int(data,16)
                if l[2] != "-":
                    sladdr = int(l[2],16)
                if (l[3] != "-" and l[3] != "*") :
                    DB0 = int(l[3],16)
                if (l[3] == "*"):
                    DB0 = data
                if l[4] == "*":
                    DB1 = (data & 0xFF)
                if l[5] == "*":
                    DB2 = ((data >> 8) & 0xFF)
                print "debug............", hex(sladdr), hex(DB0), hex(DB1), hex(DB2)
                # for I2C
                # data_low = address | DB0 << 8
                # data_high = DB1 | DB2 << 8    
                data_low = (sladdr & 0xFF) | ((DB0 << 8) & 0xFF00)
                data_high = (DB1 & 0xff) | ((DB2 << 8) & 0xFF00)
                
                #print "bin", bin(data_low), bin(data_high)   
                #print "hex" hex(data_low), hex(data_high),
                    
                if found == 1:
                    print "INFO: sending words", bin(data_low), bin(data_high),hex(data_low), hex(data_high)  
                
        if found == 0:
            print "ERROR: no line found in DUT_I2C_SPI.reg"
            exit(-1)
        print "INFO: sending words", hex(data_low), hex(data_high) 
        targetcmd = "0x" +str(ndevice) + str(target) + str(nbytes)
        print "Main command reg:", str(targetcmd)

        ftdIO.writeReg(192, data_low)
        ftdIO.writeReg(256, data_high)

        ftdIO.writeReg(128,int(targetcmd,16))



def writeAD5235_dutboard(device,ndevice,cmd,data,target,nbytes):

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
                #data = int(data,16) << 4
                DB0 = int(l[3],16)
                if l[4] == "*":
                    DB1 = (int(data,16) >> 8) & 0x03
                if l[5] == "*":
                    DB2 =int( data,16) & 0xff
                # for SPI
                # data_low = DB2 | DB1 << 8
                # data_high = DB0 & 0xff
                data_low = DB2 | (DB1 << 8)
                data_high = DB0 & 0xff
                
                #print bin(data_low), bin(data_high)   
                #print hex(data_low), hex(data_high)
                    
                if found == 1:
                    print "INFO: sending words", bin(data_low), bin(data_high), hex(data_low), hex(data_high)
                
        if found == 0:
            print "ERROR: no line found in ADCDACBOARD_I2C_SPI.reg"
            exit(-1)
        
        ftdIO.writeReg(192, data_low)
        ftdIO.writeReg(256, data_high)
        #  
        targetcmd = "0x5" + str(target) + "3"
        print "Main command reg:", str(targetcmd)
        
        ftdIO.writeReg(128,int(targetcmd,16))
        ftdIO.writeReg(128,int(targetcmd,16))



def writeLTC2635_dutboard(cmd,data,target):
    device = "LTC2635"
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
                print "ERROR: wrong numbers of arguments for",l[0],l[1], "in DUT_I2C_SPI.reg!"
                exit(-1)
            if ((l[0]==device)&(l[1]==cmd)):
                print "INFO: Executing operation", l[1], "on device", l[0] 
                # build words
                found = 1
                data = int(data,16)
                sladdr = int(l[2],16)
                DB0 = int(l[3],16)
                if l[4] == "*":
                    DB1 = data & 0xff
                 # for I2C
                # data_low = address | DB0 << 8
                # data_high = DB1 | DB2 << 8     
                data_low = sladdr | DB0 << 8
                data_high = DB1 & 0xff | DB2 << 8
                
                print bin(data_low), bin(data_high)   
                print hex(data_low), hex(data_high)
                    
                if found == 1:
                    print "INFO: sending words", bin(data_low), bin(data_high) 
                    
        if found == 0:
            print "ERROR: no line found in ADCDACBOARD_I2C_SPI.reg"
            exit(-1)
        
        ftdIO.writeReg(192, data_low)
        ftdIO.writeReg(256, data_high)

        #  
        targetcmd = "0x4" + str(target) + "3"
        print "Main command reg:", str(targetcmd)
        
        ftdIO.writeReg(128,int(targetcmd,16))


def writeADS7830_dutboard(cmd,data,target):
    device = "LTC2635"
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
                print "ERROR: wrong numbers of arguments for",l[0],l[1], "in DUT_I2C_SPI.reg!"
                exit(-1)
            if ((l[0]==device)&(l[1]==cmd)):
                print "INFO: Executing operation", l[1], "on device", l[0] 
                # build words
                found = 1
                data = int(data,16)
                sladdr = int(l[2],16)
                if l[3] == "*":
                    DB0 = data & 0xfc
                if l[4] == "*":
                    DB1 = data & 0xff
                 # for I2C
                # data_low = address | DB0 << 8
                # data_high = DB1 | DB2 << 8     
                data_low = sladdr | DB0 << 8
                data_high = DB1 & 0xff | DB2 << 8
                
                print bin(data_low), bin(data_high)   
                print hex(data_low), hex(data_high)
                    
                if found == 1:
                    print "INFO: sending words", bin(data_low), bin(data_high) 
                    
        if found == 0:
            print "ERROR: no line found in ADCDACBOARD_I2C_SPI.reg"
            exit(-1)
        
        ftdIO.writeReg(192, data_low)
        ftdIO.writeReg(256, data_high)

        #  
        targetcmd = "0x4" + str(target) + "3"
        print "Main command reg:", str(targetcmd)
        
        ftdIO.writeReg(128,int(targetcmd,16))

def writeMAX31865_dutboard(device,ndevice,cmd,data,target,nbytes):

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
                data = int(data,16) | 0xff
                DB0 = int(l[3],16)
                if l[4] == "*":
                    DB1 = data
                # for SPI
                # data_low = DB2 | DB1 << 8
                # data_high = DB0 & 0xff
                data_low = DB2 | (DB1 << 8)
                data_high = DB0 & 0xff
                
                #print bin(data_low), bin(data_high)   
                #print hex(data_low), hex(data_high)
                    
                if found == 1:
                    print "INFO: sending words", bin(data_low), bin(data_high), hex(data_low), hex(data_high)
                
        if found == 0:
            print "ERROR: no line found in ADCDACBOARD_I2C_SPI.reg"
            exit(-1)
        
        ftdIO.writeReg(192, data_low)
        ftdIO.writeReg(256, data_high)
        #  
        targetcmd = "0x5" + str(target) + str(nbytes)
        print "Main command reg:", str(targetcmd)
        
        ftdIO.writeReg(128,int(targetcmd,16))


if __name__ == "__main__":
    if(len(sys.argv) != 7):
        print "ERROR: wrong number of arguments for", sys.argv[0]
        exit(-1)
    else:
        #device = sys.argv[1]
        device = sys.argv[1]
        ndevice = sys.argv[2]
        cmd = sys.argv[3]
        data = sys.argv[4]
        target = sys.argv[5]
        nbytes = sys.argv[6]
    #commRDO.openRDO()
    write_dutboard(device,ndevice,cmd,data,target,nbytes)
    #commRDO.closeRDO()
