//==========================================================================
// Single operation class - CPP file 
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 8/25/2016
// Last modified: 8/26/2016
//==========================================================================

#include "SingleOperationParameters.h"

//=======================================================================
//                       Constructors and destructor
//=======================================================================

SingleOperationParameters::SingleOperationParameters()
{
  Initialize();
}

SingleOperationParameters::~SingleOperationParameters()
{
  Destroy();
}

//=======================================================================
//                             Inner Methods
//=======================================================================

// Public

unsigned int SingleOperationParameters::GetHeader(std::string singleOperationType)
{
  if      (singleOperationType == "adcs")
    return localHeaders->adcs;
  else if (singleOperationType == "i2c")
    return localHeaders->i2c;
  else if (singleOperationType == "i2c_aux")
    return localHeaders->i2cAux;
  else if (singleOperationType == "io_checker")
    return localHeaders->ioChecker;
  else if (singleOperationType == "register_fields")
    return 0x00000000;
  else {
    std::cout << "SingleOperationParameters: Provided non-valid single operation name (\"" << singleOperationType << "\").\n" << std::endl;
    exit(EXIT_FAILURE);
  }
  return 0;
}

unsigned int SingleOperationParameters::GetTrailer()
{
  return localTrailer;
}

unsigned int SingleOperationParameters::GetHeaderMask(std::string singleOperationType)
{
  if      (singleOperationType == "adcs")
    return localHeaderMasks->adcs;
  else if (singleOperationType == "i2c")
    return localHeaderMasks->i2c;
  else if (singleOperationType == "i2c_aux")
    return localHeaderMasks->i2cAux;
  else if (singleOperationType == "io_checker")
    return localHeaderMasks->ioChecker;
  else if (singleOperationType == "register_fields")
    return 0x00000000;
  else {
    std::cout << "SingleOperationParameters: Provided non-valid single operation name (\"" << singleOperationType << "\").\n" << std::endl;
    exit(EXIT_FAILURE);
  }
  return 0;
}

unsigned int SingleOperationParameters::GetNumberOfPayloadWords(std::string singleOperationType)
{
  if      (singleOperationType == "adcs")
    return numberOfPayloadWords->adcs;
  else if (singleOperationType == "i2c")
    return numberOfPayloadWords->i2c;
  else if (singleOperationType == "i2c_aux")
    return numberOfPayloadWords->i2cAux;
  else if (singleOperationType == "io_checker")
    return numberOfPayloadWords->ioChecker;
  else if (singleOperationType == "register_fields")
    return numberOfPayloadWords->registerFields;
  else {
    std::cout << "SingleOperationParameters: Provided non-valid single operation name (\"" << singleOperationType << "\").\n" << std::endl;
    exit(EXIT_FAILURE);
  }
  return 0;
}

unsigned int SingleOperationParameters::GetWordHotBitPattern(std::string wordType)
{
  if      (wordType == "localHeader")
    return wordHotBitPatterns->localHeader;
  else if (wordType == "dataWord")
    return wordHotBitPatterns->dataWord;
  else if (wordType == "localTrailer")
    return wordHotBitPatterns->localTrailer;
  else if (wordType == "registerWord")
    return wordHotBitPatterns->registerWord;
  else {
    std::cout << "SingleOperationParameters: Provided non-valid identifier for word type (\"" << wordType << "\").\n" << std::endl;
    exit(EXIT_FAILURE);
  }
  return 0;
}

unsigned int SingleOperationParameters::GetWordHotBitPosition()
{
  return wordHotBitPosition;
}

// Private

void SingleOperationParameters::Initialize()
{
  localHeaders = new LocalHeaders;
  localHeaderMasks = new LocalHeaderMasks;
  wordHotBitPatterns = new WordHotBitPatterns;
  numberOfPayloadWords = new NumberOfPayloadWords;

  localHeaders->adcs      = 0xADC00000;
  localHeaders->i2c       = 0xABCD0000;
  localHeaders->i2cAux    = 0xABCF0000;
  localHeaders->ioChecker = 0x010D0000;

  localHeaderMasks->adcs      = 0xFFF00000;
  localHeaderMasks->i2c       = 0xFFFF0000;
  localHeaderMasks->i2cAux    = 0xFFFF0000;
  localHeaderMasks->ioChecker = 0xFFFF0000;

  wordHotBitPatterns->localHeader  = 0x01000000;
  wordHotBitPatterns->dataWord     = 0x11000000;
  wordHotBitPatterns->localTrailer = 0x10000000;
  wordHotBitPatterns->registerWord = 0x00000000;

  wordHotBitPosition = 0x11000000;

  numberOfPayloadWords->adcs           = 12; 
  numberOfPayloadWords->i2c            = 1;  
  numberOfPayloadWords->i2cAux         = 1;  
  numberOfPayloadWords->ioChecker      = 8;
  numberOfPayloadWords->registerFields = 1;  

  localTrailer = 0xFEEDBEEF;
}

void SingleOperationParameters::Destroy()
{
  delete localHeaders;
  delete localHeaderMasks;
  delete numberOfPayloadWords;
  delete wordHotBitPatterns;
}
