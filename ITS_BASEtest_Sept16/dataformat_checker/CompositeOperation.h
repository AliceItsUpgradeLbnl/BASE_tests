//==========================================================================
// Composite Operation class - Header file 
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 8/25/2016
// Last modified: 8/26/2016
//==========================================================================

#ifndef COMPOSITEOPERATION_H
#define COMPOSITEOPERATION_H

// C/C++ Libs
#include <iostream>
#include <string.h>
#include <queue>
#include <stdlib.h>

// Other user defined Libs
#include "DatatypeConversion.h"
#include "CompositeOperationParameters.h"
#include "FileReader.h"
#include "SingleOperation.h"
#include "Decoder.h"
#include "Statistics.h"
 
class CompositeOperation
{
  public:
     CompositeOperation(char* filename, std::string compositeOperationType);
    ~CompositeOperation();

    void               CheckOperationDataFormat(unsigned int positionOfOperationHeader);
    void               GetOperationData(unsigned int positionOfOperationHeader);

    struct AdcData
    {
      std::queue<double>    **mean;
      std::queue<double>    **sigma;      
    } *adcData;

    struct ComparatorData
    {
      std::queue<bool>      *output;
      std::queue<bool>      *outputBar;
    } *comparatorData;

    struct AdcDataDut
    {
      std::queue<double>    **mean;
      std::queue<double>    **sigma;      
    };

    struct ResistanceToDigital
    {
      std::queue<double>     *mean;
      std::queue<double>     *sigma;      
    };

    struct IoExpander
    {
      std::queue<bool>      **initial_state;
      std::queue<bool>      **transition_occurred;      
    };

    struct I2cData
    {
      AdcDataDut                        **adcs;
      std::queue<unsigned int>          **differentialToSingleEndedDriver;
      ResistanceToDigital                *resistanceToDigital;
      IoExpander                         *ioExpander;
      std::queue<unsigned int>           *i2cDriver;
    } *i2cData;

    struct OnlineData
    {
      unsigned int                       numberOfRuns;
      std::queue<unsigned int>          *numberOfPointsPerRun;
    } *onlineData;

  private:
    void               Initialize(char* filename, std::string compositeOperationType);
    void               Destroy();

    void               PushAdcChannelDataToStatisticsBuffer(unsigned int channelNumber, unsigned int numberofWordsToPush);
    void               PushI2cDataToStatisticsBuffer(unsigned int numberofWordsToPush);
 
    void               RunChecksOnOperationData();
    void               DecodeAndReformatOperationData();
    void               CheckOperationHeader();
    void               CheckOperationPayload();
    void               CheckOperationTrailer();
    void               GetOperationPayload();

    void               FillDataStructuresDifferentialAmplifiers();
    void               FillDataStructuresComparator();
    void               FillDataStructuresDacs();
    void               FillDataStructuresResistanceToDigital();
    void               FillDataStructuresIoExpander();
    void               FillDataStructuresAdcs();
    void               FillDataStructuresI2cDriver();
    void               FillDataStructuresDifferentialToSingleEndedDriver();
    void               FillDataStructuresDcdcConverters();
    void               FillDataStructuresOnlineMonitoring();

    void               CheckOperationPayloadDifferentialAmplifiers();
    void               CheckOperationPayloadComparator();
    void               CheckOperationPayloadDacs();
    void               CheckOperationPayloadResistanceToDigital();
    void               CheckOperationPayloadIoExpander();
    void               CheckOperationPayloadAdcs();
    void               CheckOperationPayloadI2cDriver();
    void               CheckOperationPayloadDifferentialToSingleEndedDriver();
    void               CheckOperationPayloadDcdcConverters();
    void               CheckOperationPayloadOnlineMonitoring();

    std::string        compositeOperationType;

    unsigned int       operationHeader;
    unsigned int       operationTrailer;

    unsigned int       numberOfWordsOfOperation;

    CompositeOperationParameters *compositeOperationParameters;
    FileReader                   *fileReader;
    SingleOperation              *singleOperation;
    Decoder                      *decoder;
    Statistics                   *statistics;
};

#endif
