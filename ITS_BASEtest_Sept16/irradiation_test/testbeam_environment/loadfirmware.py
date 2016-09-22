#!/usr/bin/python

#---------------------------------------------------------------------------
# Addressability script file for the ALICE ITS Power Board 2-module version
# Lawrence Berkeley National Laboratory
# Author: Alberto Collu
# Created: 10-14-2015
# Last edited: 10-14-2015 (Alberto Collu) 
# Description: This scripts loads the firmware into the FPGA
#---------------------------------------------------------------------------

import sys
import os
import shutil
import subprocess

def LoadFirmware():
        subprocess.call(['/bin/bash', '-i', '-c', "./firmware/scripts/loadfirmware.sh"])
	return()
