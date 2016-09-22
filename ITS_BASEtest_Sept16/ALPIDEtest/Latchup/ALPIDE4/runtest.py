#!/usr/bin/python

import sys
import os
import time
from datetime import datetime
import daq
#import hameg
import usb
DOWN_SAMPLING_FACTOR=10
SHUTOFF_DELAY=80000 # 1 ms in 80MHz cycles

# in 51.2us
AUTOSHTDWN_TIME    = 20
CLOCK_ENABLE_TIME  = 12
SIGNAL_ENABLE_TIME = 12
DRST_TIME          = 13

DAQBOARD_VERSION   = 2

daq=daq.DAQ()
#hameg=hameg.HAMEG(False)
assert daq.read_firmware_id()==0x247E0611
daq.drain_sel_samples()
t=datetime.now()
fname="latchup-%s.log" % (str(t).replace(' ','_'))
if len(sys.argv[0])>1:
#    directory=sys.argv[1]
    directory=str(datetime.now()).replace(' ','_')
    if os.path.exists(directory):
        raise Exception("Error: directory exists already...")
    if os.path.exists("./runfolder_list.txt"):
        runfile = open("./runfolder_list.txt",'a+')
    else:
        runfile = open("./runfolder_list.txt",'w+')
    runfile.write(directory + "\n")
    runfile.close()
    os.makedirs(directory)
    fname=directory+'/'+fname
    
sys.stdout.write("Logging to \"%s\".\n" % fname)
logfile=open(fname,'w')
i=0
while True:
    # POWER DOWN
    daq.write_enable_ldos(False)
    daq.write_enable_sel(False)
    # ENABLE BACKBIAS
#    hameg.output()
    # CONFIGURE RESET SEQUENCER
    daq.set_daqboard_version(DAQBOARD_VERSION)
    daq.write_startup_times(AUTOSHTDWN_TIME,CLOCK_ENABLE_TIME,SIGNAL_ENABLE_TIME,DRST_TIME)
    # WAIT A SECOND
    time.sleep(1)
    daq.write_daq_reg(5,6,1)   #Disable PowerOnReset (necessary for Alpide4)
    # ARM LATCHUP CIRCUIT
    t=datetime.now()
    msg="# %s -> ARMING LATCHUP CIRCUIT\n" % str(t)
    sys.stdout.write(msg)
    logfile.write(msg)
    daq.write_current_thresholds(250, 900) ### was (250, 900)
    daq.write_current_downsampling_factor(DOWN_SAMPLING_FACTOR)
    daq.write_auto_shutoff_delay(SHUTOFF_DELAY)
    daq.write_enable_sel()
    # POWER ON
    ton=datetime.now()
    msg="# %s -> (RE)ENABLING POWER\n" % str(ton)
    sys.stdout.write(msg)
    logfile.write(msg)
    daq.write_enable_ldos()

    time.sleep(0.2)
    daq.write_current_thresholds(250,900)
    daq.write_chip_reg(0,6,2,147) # Set VRESETD DAC

    # MONITOR CURRENTS AND LDO STATE(s)
    msg="# TIME\t\t\t\tI(AVDD)\t\tI(DVDD)\t\tTemperature\tI(board)\tI(bias)\t\tF(board)\tF(bias)\n"
    sys.stdout.write(msg)
    logfile.write(msg)
    sel=False
    fuses=[]
    status=[]
    while not sel:
        adcs=daq.read_adcs()
#        currents_hameg=[hameg.get_current(1),hameg.get_current(3)]
#        fuses=[hameg.get_fuse_triggered(1),hameg.get_fuse_triggered(3)]
        status=daq.read_ldo_status()
        t=datetime.now()
#        msg="%s : \t%f\t%f\t%f\t%f\t%f\t%d\t\t%d" % (str(t),adcs[0],adcs[1],adcs[2],currents_hameg[0],currents_hameg[1],fuses[0],fuses[1])
        msg="%s : \t%f\t%f\t%f" % (str(t),adcs[0],adcs[1],adcs[2])
        sys.stdout.write('\r'+msg)
        sys.stdout.flush()
        logfile.write(msg+'\n')
        time.sleep(0.1)
        sel=not(status[0] and status[1] and status[2]) 
        #or fuses[0] or fuses[1]
    # REPORT LATCHUP
    i+=1
    sys.stdout.write('\n')
    tlu=datetime.now()
    msg="# %s -> LATCHUP no. %d after %s!\n" % (str(tlu),i,str(tlu-ton))
    sys.stdout.write(msg)
    logfile.write(msg)
    # WAIT A SECOND
    time.sleep(1)
    if not(status[0] and status[1] and status[2]):
        # READOUT POST-MORTEM
        sel_t=daq.read_sel_times()
        sel_pm=daq.read_sel_samples()
        #fname="latchup-%06d-%s.dat" % (i,str(tlu).replace(' ','_'))
        fname=""
	fname="./" + directory + "/" + "latchup-%d.dat" % (i,)
        if len(sys.argv)>1:
            fname=sys.argv[1]+'/'+fname
        msg="# writing post mortem data to \"%s\"...\n" % fname
        sys.stdout.write(msg)
        logfile.write(msg)
        f=open(fname,"w")
        f.write(msg)
        f.write("# T_poweron = %d, T_latchup = %d (dT = %d) (all in 80 MHz clock cycles)\n" % (sel_t[0],sel_t[1],sel_t[1]-sel_t[0]))
        f.write("# T\t\tdT\tLDO\tI(digital,core)\tI(digital,I/O)\tI(analogue)\n")
        for sample in sel_pm:
            f.write("%d\t%d\t%d\t%d\t\t%d\t\t%d\n" % (sample[0],sample[0]-sel_t[1],sample[1],sample[2][3],sample[2][4],sample[2][5]))
        f.close()
        daq.drain_sel_samples()
#    if fuses[0]:
#        print "ERROR: main power off"
#        break


