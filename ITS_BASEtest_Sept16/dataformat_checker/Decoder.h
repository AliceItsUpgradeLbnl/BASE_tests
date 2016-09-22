//==========================================================================
// Decoder class - Header file 
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 8/25/2016
// Last modified: 8/26/2016
//==========================================================================

#ifndef DECODER_H
#define DECODER_H

// C/C++ Libs
#include <iostream>
#include <string.h>

// Other user defined Libs
#include "DecoderParameters.h"  
#include "SingleOperation.h"
 
class Decoder
{
  public:
     Decoder(SingleOperation *singleOperation);
    ~Decoder();

    void               DecodeData(std::string singleOperationType, std::string device);

    bool               GetStatusOfFirmwareRegisterBit(std::string bitName);
    unsigned int       GetI2cDriverWord();
    double             PopWordFromDataBuffer(std::string bufferName, std::string channelNumber);

  private:
    void               Initialize(SingleOperation *singleOperation);
    void               Destroy();

    void               GetAdcBlockData();

    void               DecodeAdcData();
    void               DecodeAdcBlockData(unsigned int blockNumber);
    void               DecodeFirmwareRegister();
    void               DecodeI2cData(std::string device);
    void               DecodeI2cAuxData(std::string device);
    void               DecodeIoCheckerData();

    void               DecodeI2cDataFromADS7830();
    void               DecodeI2cDataFromADC128D818();
    void               DecodeI2cDataFromAD7997();
    void               DecodeI2cDataFromPCF8574();
    void               DecodeI2cDataFromTCA9535();
    void               DecodeI2cDataFromP82B96DP(); 
    void               DecodeI2cDataFromMAX31865Low();   
    void               DecodeI2cDataFromMAX31865High();

    double             PopWordFromAdcChannelBuffer(unsigned int channelNumber);
    double             PopWordFromIoCheckerErrorCounterBuffer();
    double             PopWordFromI2cBuffer(std::string deviceAndChannel);

    struct AdcData
    {
      unsigned int              blockWords[3];
      std::queue<double>      **channels;
    } *adcData;

    struct ComparatorData
    {
      bool output;
      bool outputBar;
    } *comparatorData;

    struct InterruptCheckerData
    {
      bool initialState;
      bool transitionOccurred;
    } *interruptCheckerData;
    
    struct IoCheckerData
    {
      std::queue<unsigned int>  *errorCounterBuffer;
      bool                       errorFlagChannel[8];
    } *ioCheckerData;

    struct I2cData
    {
      std::queue<double>        *data;
      std::queue<double>       **ioExpanderChannelState;
      unsigned int               i2cDriverWord;
    } *i2cData;

    DecoderParameters      *decoderParameters;
    SingleOperation        *singleOperation;
};

#endif
