//==========================================================================
// Composite operation parameters class - Header file 
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 8/25/2016
// Last modified: 8/25/2016
//==========================================================================

#ifndef COMPOSITEOPERATIONPARAMETERS_H
#define COMPOSITEOPERATIONPARAMETERS_H

// C/C++ Libs
#include <iostream>
#include <string.h>
 
class CompositeOperationParameters
{
  public:
     CompositeOperationParameters();
    ~CompositeOperationParameters();

    unsigned int       GetHeader(std::string compositeOperationType);
    unsigned int       GetTrailer();
    unsigned int       GetNumberOfWordsOfOperation(std::string compositeOperationType);

    struct GlobalHeaders {
      unsigned int     differentialAmplifiers;
      unsigned int     comparator;
      unsigned int     dacs;
      unsigned int     resistanceToDigital;
      unsigned int     ioExpander;
      unsigned int     adcs;
      unsigned int     i2cDriver;
      unsigned int     differentialToSingleEndedDriver;
      unsigned int     dcdcConverters;  
      unsigned int     onlineMonitoring;  
    } *globalHeaders;

    unsigned int globalTrailer;

  private:
    void               Initialize();
    void               Destroy();

};

#endif
