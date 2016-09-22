//==========================================================================
// Decoder parameters class - Header file 
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 8/25/2016
// Last modified: 8/26/2016
//==========================================================================

#ifndef DECODERPARAMETERS_H
#define DECODERPARAMETERS_H

// C/C++ Libs
#include <iostream>
#include <string.h>
#include <math.h>

// User defined libraries
#include "SingleOperation.h"
 
class DecoderParameters
{
  public:
     DecoderParameters();
    ~DecoderParameters();

    unsigned int       GetMask(std::string deviceType, std::string bitName);
    unsigned int       GetHotBitPosition(std::string deviceType, std::string bitName);

    double             GetDynamicRangeAdcs(unsigned int channelNumber);

  private:
    void               Initialize();
    void               Destroy();

    unsigned int       GetIoCheckerErrorFlagMask(unsigned int channelNumber);
    unsigned int       GetComparatorMask(std::string bitName);
    unsigned int       GetInterruptCheckerMask(std::string bitName);

    unsigned int       GetIoCheckerErrorFlagHotBitPosition(unsigned int channelNumber);
    unsigned int       GetInterruptCheckerHotBitPosition(std::string bitName);
    unsigned int       GetComparatorHotBitPosition(std::string bitName);

    struct RegisterBitFields
    {
      unsigned int ioCheckerErrorFlags[8];
      unsigned int comparatorOutput;
      unsigned int comparatorOutputBar;
      unsigned int interruptCheckerInitialState; 
      unsigned int interruptCheckerTransition; 
    } *dataMasks, *hotBitPositions;
    
    double             dynamicRangeAdcs[32];
};

#endif
