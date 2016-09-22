#!/usr/bin/env python
import usb
import array
import time

DAQ_USB_VID=0x04b4
DAQ_USB_PID=0x00f1
DAQBOARD_VERSION=2

CHIPID=0x10

class DAQ:
    def __init__(self):
        self.dev=usb.core.find(idVendor=DAQ_USB_VID,idProduct=DAQ_USB_PID)
        if self.dev is None:
            raise Exception('No DAQ board found')
        self.dev.set_configuration(0)
        self.dev.set_configuration(1)
        self.cfg=self.dev.get_active_configuration();
        self.itf=self.cfg[(0,0)];
        self.epo=self.itf[0];
        self.epi=self.itf[1];
        self.epd=self.itf[2];

    def conv32to8be(self,word):
        return [word>>24&0xFF,word>>16&0xFF,word>>8&0xFF,word>>0&0xFF]
    def conv8to32le(self,bytes):
        return bytes[3]<<24|bytes[2]<<16|bytes[1]<<8|bytes[0]<<0

    def write_daq_reg(self,mod_addr,sub_addr,data):
        assert mod_addr|0x0F==0xF
        assert sub_addr|0xFF==0xFF
        cmd=[
          sub_addr     ,
          mod_addr     ,
          0x00         ,
          0x00         ,
          data>> 0&0xFF,
          data>> 8&0xFF,
          data>>16&0xFF,
          data>>24&0xFF
        ]
        #print 'write_daq_reg',cmd
        nwrite=self.epo.write(cmd)
        assert nwrite==len(cmd)
        ret=self.epi.read(8)
        assert len(ret)==8
        assert ret[0]==0x01
        assert ret[1]==0x00
        assert ret[2]==0x00
        assert ret[3]==0x00
        assert ret[4]==sub_addr
        assert ret[5]==mod_addr
        assert ret[6]==0x00
        assert ret[7]==0x00 

    def read_daq_reg(self,mod_addr,sub_addr):
        assert mod_addr|0x0F==0xF
        assert sub_addr|0xFF==0xFF
        cmd=[
          sub_addr     ,
          0x10|mod_addr,
          0x00         ,
          0x00
        ]
#        print 'read_daq_reg',bin(cmd[0]),bin(cmd[1])
        nwrite=self.epo.write(cmd)
        assert nwrite==len(cmd)
#        print 'nwrite   ',nwrite
        ret=self.epi.read(8)
#        print 'ret=',ret
        assert len(ret)==8
        assert ret[0]==0
        assert ret[1]==0
        assert ret[2]==0 
        assert ret[3]==0
        return self.conv8to32le(ret[4:8])

    def write_chip_reg(self,rgn_addr,base_addr,sub_addr,data):
        assert rgn_addr |0x1F==0x1F
        assert base_addr|0x7==0x7
        assert sub_addr |0xFF==0xFF
        assert data     |0xFFFF==0xFFFF
        addr=rgn_addr<<11|base_addr<<8|sub_addr
        cmd=addr<<16|CHIPID<<8|0x9C
        self.write_daq_reg(4,1,data)
        self.write_daq_reg(4,0,cmd)

    def read_chip_reg(self,rgn_addr,base_addr,sub_addr):
        assert rgn_addr |0x1F==0x1F
        assert base_addr|0x7==0x7
        assert sub_addr |0xFF==0xFF
        addr=rgn_addr<<11|base_addr<<8|sub_addr
        cmd=addr<<16|CHIPID<<8|0x4E
        self.write_daq_reg(4,0,cmd)
        ret=self.read_daq_reg(4,1)
        assert ret&0xFF==CHIPID
        assert ret>>8|0xFFFF==0xFFFF
        return ret>>8&0xFFFF

    def read_adcs(self):
        reg3=self.read_daq_reg(1,3)
        reg4=self.read_daq_reg(1,4)
        reg5=self.read_daq_reg(1,5)
        adc0=reg3    &0xFFF
        adc1=reg3>>12&0xFFF
        adc2=reg4    &0xFFF
        adc3=reg4>>12&0xFFF # DVDD
        adc4=reg5    &0xFFF
        adc5=reg5>>12&0xFFF # AVDD
        return adc5,adc3,adc0 # AVDD, DVDD, temp

    def read_ldo_status(self):
        reg3=self.read_daq_reg(1,3)
        reg4=self.read_daq_reg(1,4)
        reg5=self.read_daq_reg(1,5)
        ldo0=(reg3>>24&0x1)==0x1
        ldo1=(reg4>>24&0x1)==0x1
        ldo2=(reg5>>24&0x1)==0x1
        return [ldo2,ldo1,ldo0]

    def read_sel_times(self):
        reg6=self.read_daq_reg(1,6)
        reg7=self.read_daq_reg(1,7)
        reg8=self.read_daq_reg(1,8)
        ton =(reg8    &0xFFFF)<<32|reg6
        tsel=(reg8>>16&0xFFFF)<<32|reg7
        return ton,tsel

    def drain_sel_samples(self):
        n=0
        while True:
            try:
                self.epd.read(6*4,100)
                n+=1
            except usb.core.USBError:
                break
        return n

    def read_sel_samples(self):
        data=[]
        for i in xrange(338):
            ret=self.epd.read(6*4)
            if i==0 and ret!=6*4:
                continue
            assert(len(ret)==6*4)
            assert self.conv8to32le(ret[0:4])&0x3F==0x04
            ldo=self.conv8to32le(ret[0:4])>>6&0x1==0x1
            ts=(self.conv8to32le(ret[8:12])&0xFFFFFF)<<24|self.conv8to32le(ret[4:8])&0xFFFFFF
            adcs=[self.conv8to32le(ret[12:16])&0xFFF,self.conv8to32le(ret[12:16])>>12&0xFFF,
                  self.conv8to32le(ret[16:20])&0xFFF,self.conv8to32le(ret[16:20])>>12&0xFFF,
                  self.conv8to32le(ret[20:24])&0xFFF,self.conv8to32le(ret[20:24])>>12&0xFFF]
            data+=[[ts,ldo,adcs]]
        return data

    def write_current_thresholds(self,avdd,dvdd):
        reg0=self.read_daq_reg(1,0)
        reg1=self.read_daq_reg(1,1)
        reg0=reg0&~0xFFFFFF|0xFFF<<12|dvdd
        reg1=reg1&~0xFFF   |avdd&0xFFF
        self.write_daq_reg(1,1,reg1)
        self.write_daq_reg(1,0,reg0)

    def write_current_downsampling_factor(self,factor):
        reg2=self.read_daq_reg(1,2)
        reg2=reg2&~(0xFFF<<20)|(factor&0xFFF)<<20
        self.write_daq_reg(1,2,reg2)

    def write_auto_shutoff_delay(self,delay):
        reg2=self.read_daq_reg(1,2)
        reg2=reg2&~(0xFFFFF)|delay&0xFFFFF
        self.write_daq_reg(1,2,reg2)

    def write_enable_sel(self,enable=True):
        reg0=self.read_daq_reg(1,0)
        if enable:
            reg0=reg0|1<<27|1<<26|1<<24
        else:
            reg0=reg0&~(1<<27|1<<26|1<<24)
        self.write_daq_reg(1,0,reg0)

    def write_enable_ldos(self,enable=True):
        reg0=self.read_daq_reg(1,0)
        if enable:
            reg0=reg0|1<<25
        else:
            reg0=reg0&~(1<<25)
        self.write_daq_reg(1,0,reg0)

    def read_firmware_id(self):
        return self.read_daq_reg(6,2)

    def write_startup_times(self,autoshtdwn,clock_enable,signal_enable,drst):
        self.write_daq_reg(5,0,0)
        self.write_daq_reg(5,1,(drst&0xFF)<<24|(signal_enable&0xFF)<<16|(clock_enable&0xFF)<<8|(autoshtdwn&0xFF))

    def set_daqboard_version(self,daqboard_version):
        if daqboard_version == 3:
            conf=0x1<<12|0x3<<9|0x3<<7|0x1<<6|0x1<<5|0x1<<4|0xf    
            self.write_daq_reg(2,0,conf) 
        return self.read_daq_reg(2,0)


if __name__=='__main__':
    daq=DAQ()
    print "Firmware: %08X"%daq.read_firmware_id()
    assert daq.read_firmware_id()==0x247E0611
    daq.set_daqboard_version(DAQBOARD_VERSION)
    daq.write_daq_reg(5,6,1)
    daq.write_startup_times(20,12,12,13)
    daq.write_current_thresholds(250,900)
    daq.write_current_downsampling_factor(0)
    daq.write_auto_shutoff_delay(1000)
    daq.write_enable_sel()
    print "# Powering on"
    daq.write_enable_ldos()
    for _ in xrange(10):
        adcs=daq.read_adcs()
        status=daq.read_ldo_status()
#        print "\t".join(map(str,adcs+status))
#	print "\t",adcs, status
	time.sleep(0.1)
    print "# Set DAC..."
    daq.write_chip_reg(0,6,2,147)
    print "# Lowering thresholds..."
    daq.write_current_thresholds(250,1000)
    for _ in xrange(10):
        adcs=daq.read_adcs()
        status=daq.read_ldo_status()
#        print "\t".join(map(str,currents+status))
#	print adcs,status,"\t"
	time.sleep(0.1)

    if not(status[0] and status[1] and status[2]):
        sel_t=daq.read_sel_times()
        sel_pm=daq.read_sel_samples()
        print "draining...",daq.drain_sel_samples()
        #print sel_t,sel_pm
    print "Power off..."
    daq.write_enable_sel(False)
    daq.write_enable_ldos(False)

