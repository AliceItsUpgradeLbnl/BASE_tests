setMode -bs
setMode -bs
setMode -bs
setMode -bs
setCable -port usb21
Identify -inferir
identifyMPM
assignFile -p 1 -file "./firmware/bitfiles/PXL_RDO_top.bit"
Program -p 1
exit
