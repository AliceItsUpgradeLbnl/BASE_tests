//==========================================================================
// Single operation parameters class - Header file 
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 8/25/2016
// Last modified: 8/26/2016
//==========================================================================

#ifndef SINGLEOPERATIONPARAMETERS_H
#define SINGLEOPERATIONPARAMETERS_H

// C/C++ Libs
#include <iostream>
#include <string.h>
 
class SingleOperationParameters
{
  public:
     SingleOperationParameters();
    ~SingleOperationParameters();

    unsigned int       GetHeader(std::string singleOperationType);
    unsigned int       GetTrailer();
    unsigned int       GetHeaderMask(std::string singleOperationType);
    unsigned int       GetNumberOfPayloadWords(std::string singleOperationType);
    unsigned int       GetWordHotBitPattern(std::string wordType);
    unsigned int       GetWordHotBitPosition();

  private:
    void               Initialize();
    void               Destroy();

    struct LocalHeaders {
      unsigned int     ioChecker;
      unsigned int     adcs;
      unsigned int     i2c;
      unsigned int     i2cAux;
    } *localHeaders;

    struct LocalHeaderMasks {
      unsigned int     ioChecker;
      unsigned int     adcs;
      unsigned int     i2c;
      unsigned int     i2cAux;
    } *localHeaderMasks;

    unsigned int wordHotBitPosition;

    struct WordHotBitPatterns {
      unsigned int     localHeader;
      unsigned int     dataWord;
      unsigned int     localTrailer;
      unsigned int     registerWord;
    } *wordHotBitPatterns;

    struct NumberOfPayloadWords {
      unsigned int     ioChecker;
      unsigned int     adcs;
      unsigned int     i2c;
      unsigned int     i2cAux;
      unsigned int     registerFields;
    } *numberOfPayloadWords;

    unsigned int localTrailer;

};

#endif
