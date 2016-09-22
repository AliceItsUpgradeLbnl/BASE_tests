//==========================================================================
// Single operation class - Header file 
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 8/25/2016
// Last modified: 8/25/2016
//==========================================================================

#ifndef SINGLEOPERATION_H
#define SINGLEOPERATION_H

// C/C++ Libs
#include <iostream>
#include <string.h>

// Other user defined Libs
#include "FileReader.h"
#include "SingleOperationParameters.h"
 
class SingleOperation
{
  public:
     SingleOperation(FileReader *fileReader);
    ~SingleOperation();

    bool               CheckSingleOperationData(std::string singleOperationType);
    bool               CheckOnlineMonitoringData();

    bool               GetSingleOperationData(std::string singleOperationType);
    bool               GetOnlineMonitoringData();
  
    unsigned int       GetNumberOfRuns();

    unsigned int       PopWordFromPayloadBuffer();
    unsigned int       PopWordFromNumberOfSamplesPerRunBuffer();

  private:
    void               Initialize(FileReader *fileReader);
    void               Destroy();

    bool               CheckLocalHeaderForOnlineMonitoring();
    bool               IsLocalHeader(std::string singleOperationType);
    bool               IsPayload(std::string singleOperationType);
    bool               IsLocalTrailer(std::string singleOperationType);
    bool               IsDataword();
    bool               IsRegisterWord();

    void               GetPayload(std::string singleOperationType);

    void               SkipWord();

    void               PushDataToPayloadBuffer(unsigned int numberOfWordsToPush);

    std::queue<unsigned int> *payloadBuffer;
    std::queue<unsigned int> *numberOfSamplesPerRunOnlineMonitoring;

    unsigned int       numberOfSamplesOfThisRun;
    unsigned int       numberOfRuns;

    SingleOperationParameters *singleOperationParameters;
    FileReader                *fileReader;
};

#endif
