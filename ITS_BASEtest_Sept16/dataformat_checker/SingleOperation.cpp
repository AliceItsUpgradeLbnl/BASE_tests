//==========================================================================
// Single operation class - CPP file
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 25/8/2016
// Last modified: 25/8/2016
// Description: contains methods to check the data (in 32-bit words) acquired
//              during the tests
//==========================================================================

#include "SingleOperation.h"

//===========================================================================
//                       Constructors and Destructor
//===========================================================================

SingleOperation::SingleOperation(FileReader *fileReader)
{
  Initialize(fileReader);
}

SingleOperation::~SingleOperation()
{
  Destroy();
}

//===========================================================================
//                            Inner methods
//===========================================================================

// Public

bool SingleOperation::CheckSingleOperationData(std::string singleOperationType)
{
  if (!IsLocalHeader(singleOperationType))   return false;
  if (!IsPayload(singleOperationType))       return false;
  if (!IsLocalTrailer(singleOperationType))  return false;
  return true;
}

bool SingleOperation::GetSingleOperationData(std::string singleOperationType)
{
  if (!IsLocalHeader(singleOperationType))  return false;
  GetPayload(singleOperationType);
  if (!IsLocalTrailer(singleOperationType)) return false;
  return true;
}

bool SingleOperation::CheckOnlineMonitoringData()
{
  if (!CheckLocalHeaderForOnlineMonitoring()) return false;
  if (!IsPayload("adcs"))                     return false;
  if (!IsLocalTrailer("adcs"))                return false;
  return true;
}

bool SingleOperation::GetOnlineMonitoringData()
{
  if (!CheckLocalHeaderForOnlineMonitoring()) return false;
  GetPayload("adcs");
  if (!IsLocalTrailer("adcs"))                return false;
  return true;
}

bool SingleOperation::CheckLocalHeaderForOnlineMonitoring()
{
  unsigned int wordFromBuffer = fileReader->PopWordFromDataBuffer();
  if ( wordFromBuffer == 0xFADECAFE ) { // A pity to have to hard code this
    numberOfRuns++;
    numberOfSamplesPerRunOnlineMonitoring->push(numberOfSamplesOfThisRun);
    wordFromBuffer = fileReader->PopWordFromDataBuffer(); // Next global header or 0xFFFFFFFF
    if (wordFromBuffer == 0xFFFFFFFF)
      return false;
    else {
      numberOfSamplesOfThisRun = 0;
      wordFromBuffer = fileReader->PopWordFromDataBuffer(); // Local header
    }
  }

  numberOfSamplesOfThisRun++;

  unsigned int header = wordFromBuffer;
  unsigned int maskedHeader = (header & singleOperationParameters->GetHeaderMask("adcs"));
  if (maskedHeader != singleOperationParameters->GetHeader("adcs")) {
    std::cout << "SingleOperation: Found non-valid local header. Expected: " << DatatypeConversion::Itoh(singleOperationParameters->GetHeader("adcs"), true) << " Found: " << DatatypeConversion::Itoh(maskedHeader, true) << "\n" << std::endl;
    exit(EXIT_FAILURE);
  }
  return true;
}

bool SingleOperation::IsLocalHeader(std::string singleOperationType)
{
  if (singleOperationType == "register_fields") return true;

  unsigned int header = fileReader->PopWordFromDataBuffer();
  unsigned int maskedHeader = (header & singleOperationParameters->GetHeaderMask(singleOperationType));
  if (maskedHeader != singleOperationParameters->GetHeader(singleOperationType)) {
    std::cout << "SingleOperation: Found non-valid local header. Expected: " << DatatypeConversion::Itoh(singleOperationParameters->GetHeader(singleOperationType), true) << " Found: " << DatatypeConversion::Itoh(maskedHeader, true) << "\n" << std::endl;
    exit(EXIT_FAILURE);
  }

  return true;
}

bool SingleOperation::IsPayload(std::string singleOperationType)
{
  if (singleOperationType == "register_fields") return IsRegisterWord();

  unsigned int numberOfPayloadWords = singleOperationParameters->GetNumberOfPayloadWords(singleOperationType);
  for (int i = 0; i < numberOfPayloadWords; i++) {
    if(!IsDataword()) {
      std::cout << "SingleOperation: Found non-valid dataword\n" << std::endl;
      return false;
    }
  }
  return true;
}

void SingleOperation::GetPayload(std::string singleOperationType)
{
  unsigned int numberOfPayloadWords = singleOperationParameters->GetNumberOfPayloadWords(singleOperationType);
  PushDataToPayloadBuffer(numberOfPayloadWords);
}

unsigned int SingleOperation::GetNumberOfRuns()
{
  return numberOfRuns;
} 

bool SingleOperation::IsLocalTrailer(std::string singleOperationType)
{
  if (singleOperationType == "register_fields") return true;

  unsigned int trailer = fileReader->PopWordFromDataBuffer();
  if (trailer != singleOperationParameters->GetTrailer()) {
    std::cout << "SingleOperation: Found non-valid local trailer. Expected: " << DatatypeConversion::Itoh(singleOperationParameters->GetTrailer(), true) << " Found: " << DatatypeConversion::Itoh(trailer, true) << "\n" << std::endl;
    return false;
  }
  return true;
}

bool SingleOperation::IsDataword()
{
  unsigned int dataWord = fileReader->PopWordFromDataBuffer();
  unsigned int maskedDataword = (dataWord & singleOperationParameters->GetWordHotBitPosition());
  return (maskedDataword == singleOperationParameters->GetWordHotBitPattern("dataWord"));
}

bool SingleOperation::IsRegisterWord()
{
  unsigned int registerWord = fileReader->PopWordFromDataBuffer();
  unsigned int maskedRegisterWord = (registerWord & singleOperationParameters->GetWordHotBitPosition());
  return (maskedRegisterWord == singleOperationParameters->GetWordHotBitPattern("registerWord"));
}

void SingleOperation::PushDataToPayloadBuffer(unsigned int numberOfWordsToPush)
{
  for (int i = 0; i < numberOfWordsToPush; i++) {
    unsigned int wordToPush = fileReader->PopWordFromDataBuffer();
    payloadBuffer->push(wordToPush);
  }
}

unsigned int SingleOperation::PopWordFromPayloadBuffer()
{
  unsigned int valueFromBuffer = payloadBuffer->front();
  payloadBuffer->pop();

  return valueFromBuffer;
}

unsigned int SingleOperation::PopWordFromNumberOfSamplesPerRunBuffer()
{
  unsigned int valueFromBuffer = numberOfSamplesPerRunOnlineMonitoring->front();
  numberOfSamplesPerRunOnlineMonitoring->pop();

  return valueFromBuffer;
}

// Private

void SingleOperation::Initialize(FileReader *fileReader)
{
  singleOperationParameters = new SingleOperationParameters;
  payloadBuffer = new std::queue<unsigned int>;
  numberOfSamplesPerRunOnlineMonitoring = new std::queue<unsigned int>;

  numberOfSamplesOfThisRun = 0; 
  numberOfRuns = 0;

  this->fileReader = fileReader;
}

void SingleOperation::Destroy()
{
  delete singleOperationParameters;

  delete payloadBuffer;
  delete numberOfSamplesPerRunOnlineMonitoring;
}
