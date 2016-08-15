A monitoring GUI for Test beam at LBL 88" cyclotron for new ALICE ITS components.

SEE:
http://portal.nersc.gov/project/alice/ploskon/lbltb/index.html

EDIT AS OF 7/26

Made script run_one.sh - runs gui for an "online" data file. Also now prints log to screen, rather than a file.

PayloadADC.cxx:97 Changed how fADC was filled - filled according to sample structure (see DAC data Format file)

Plot still has all the channels go to -1 at several points. Printed "val" from PayloadADC.cxx:171 Intermiediate conclusion is that generating the voltage values doesn't result in -1 values, either plotting errors or some other buffer before plotting something happens.

next step: Maybe look at how GetADCvf function is used (contains vector of voltage values)