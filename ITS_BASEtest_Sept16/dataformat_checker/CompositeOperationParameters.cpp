//==========================================================================
// Tests structures class - CPP file 
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 8/25/2016
// Last modified: 8/25/2016
//==========================================================================

#include "CompositeOperationParameters.h"

//=======================================================================
//                       Constructors and destructor
//=======================================================================

CompositeOperationParameters::CompositeOperationParameters()
{
  Initialize();
}

CompositeOperationParameters::~CompositeOperationParameters()
{
  Destroy();
}

//=======================================================================
//                             Inner Methods
//=======================================================================

// Public Methods

unsigned int CompositeOperationParameters::GetHeader(std::string compositeOperationType)
{
  if      (compositeOperationType == "differential_amplifiers")
    return globalHeaders->differentialAmplifiers;
  else if (compositeOperationType == "comparator")
    return globalHeaders->comparator;
  else if (compositeOperationType == "dacs")
    return globalHeaders->dacs;
  else if (compositeOperationType == "resistance_to_digital")
    return globalHeaders->resistanceToDigital;
  else if (compositeOperationType == "io_expander")
    return globalHeaders->ioExpander;
  else if (compositeOperationType == "adcs")
    return globalHeaders->adcs;
  else if (compositeOperationType == "i2c_driver")
    return globalHeaders->i2cDriver;
  else if (compositeOperationType == "differential_to_single_ended_driver")
    return globalHeaders->differentialToSingleEndedDriver;
  else if (compositeOperationType == "dcdc_converters")
    return globalHeaders->dcdcConverters;
  else if (compositeOperationType == "online_monitoring")
    return globalHeaders->onlineMonitoring;
  else {
    std::cout << "CompositeOperationParameters: Provided non-valid composite operation name (\"" << compositeOperationType << "\").\n" << std::endl;
    exit(EXIT_FAILURE);
  }
  return 0;
}

unsigned int CompositeOperationParameters::GetTrailer()
{
  return globalTrailer;
}

unsigned int CompositeOperationParameters::GetNumberOfWordsOfOperation(std::string compositeOperationType)
{
  unsigned int numberOfWordsOfOperation = 0;

  if      (compositeOperationType == "differential_amplifiers")
    numberOfWordsOfOperation = 3082;
  else if (compositeOperationType == "comparator")
    numberOfWordsOfOperation = 24;
  else if (compositeOperationType == "dacs")
    numberOfWordsOfOperation = 3082;
  else if (compositeOperationType == "resistance_to_digital")
    numberOfWordsOfOperation = 506;
  else if (compositeOperationType == "io_expander")
    numberOfWordsOfOperation = 84;
  else if (compositeOperationType == "adcs")
    numberOfWordsOfOperation = 794;
  else if (compositeOperationType == "i2c_driver")
    numberOfWordsOfOperation = 62;
  else if (compositeOperationType == "differential_to_single_ended_driver")
    numberOfWordsOfOperation = 322;
  else if (compositeOperationType == "dcdc_converters")
    numberOfWordsOfOperation = 282;
  else if (compositeOperationType == "online_monitoring")
    numberOfWordsOfOperation = 0xFFFFFFFF; // Not the real number (unknown size, will read all file)
  else
    numberOfWordsOfOperation = 0;    

  if (!numberOfWordsOfOperation) {
    std::cout << "CompositeOperationParameters: Provided non-valid composite operation name (\"" << compositeOperationType << "\").\n" << std::endl;
    exit(EXIT_FAILURE);
  } 

  return numberOfWordsOfOperation;
}


// Private Methods

void CompositeOperationParameters::Initialize()
{
  globalHeaders = new GlobalHeaders;

  globalHeaders->differentialAmplifiers = 0xAF000002;
  globalHeaders->comparator = 0xC1000005;
  globalHeaders->dacs = 0x0D000004;
  globalHeaders->resistanceToDigital = 0x6D000007;
  globalHeaders->ioExpander = 0xCD00000A;
  globalHeaders->adcs = 0xAD000008;
  globalHeaders->i2cDriver = 0xCD00000B;
  globalHeaders->differentialToSingleEndedDriver = 0xCD000003;
  globalHeaders->dcdcConverters = 0xAF00000C;
  globalHeaders->onlineMonitoring = 0xC3000009;

  globalTrailer = 0xFADECAFE;
}

void CompositeOperationParameters::Destroy()
{
  delete globalHeaders;
}
