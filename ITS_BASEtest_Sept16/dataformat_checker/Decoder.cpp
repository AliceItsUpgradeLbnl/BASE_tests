//==========================================================================
// Decoder - CPP file
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 25/8/2016
// Last modified: 25/8/2016
//==========================================================================

#include "Decoder.h"

//===========================================================================
//                       Constructors and Destructor
//===========================================================================

Decoder::Decoder(SingleOperation *singleOperation)
{
  Initialize(singleOperation);
}

Decoder::~Decoder()
{
  Destroy();
}

//===========================================================================
//                            Inner methods
//===========================================================================

// Public Methods

void Decoder::DecodeData(std::string singleOperationType, std::string device)
{
  if (singleOperationType == "adcs")
    DecodeAdcData();
  else if (singleOperationType == "i2c")
    DecodeI2cData(device);
  else if (singleOperationType == "i2c_aux")
    DecodeI2cAuxData(device);
  else if (singleOperationType == "io_checker")
    DecodeIoCheckerData();
  else if (singleOperationType == "register_fields")
    DecodeFirmwareRegister();
  else {
    std::cout << "Decoder: Provided non-valid single operation type (\"" << singleOperationType << "\")" << std::endl;
    exit(EXIT_FAILURE);
  }
}

// Private Methods

void Decoder::Initialize(SingleOperation *singleOperation)
{
  decoderParameters = new DecoderParameters();
  this->singleOperation = singleOperation;

  adcData = new AdcData;
  adcData->channels = new std::queue<double>*[32];
  for (int i = 0; i < 32; i++) adcData->channels[i] = new std::queue<double>;

  i2cData = new I2cData;
  i2cData->data = new std::queue<double>;
  i2cData->ioExpanderChannelState = new std::queue<double>*[8];
  for (int i = 0; i < 8; i++) {
    i2cData->ioExpanderChannelState[i] = new std::queue<double>;
  }

  comparatorData = new ComparatorData;
  interruptCheckerData = new InterruptCheckerData;
  ioCheckerData = new IoCheckerData;
  ioCheckerData->errorCounterBuffer = new std::queue<unsigned int>;
}

void Decoder::Destroy()
{
  delete decoderParameters;

  delete adcData;
  delete comparatorData;
  delete interruptCheckerData;
  delete ioCheckerData;
}

void Decoder::DecodeIoCheckerData()
{
  unsigned int numberOfIoCheckerPayloadWords = 8;

  unsigned int datawordMask = 0x0000FFFF; // Most significant bits may be used in the firmware (however 24th and 28th are always high)
  unsigned int maskedDataword;
  for (int i = 0; i < numberOfIoCheckerPayloadWords; i++) {
    maskedDataword = (singleOperation->PopWordFromPayloadBuffer() & datawordMask) ;
    ioCheckerData->errorCounterBuffer->push(maskedDataword);
  }
}

void Decoder::DecodeFirmwareRegister()
{
  unsigned int registerWord = singleOperation->PopWordFromPayloadBuffer();

  comparatorData->output    = ((registerWord & decoderParameters->GetMask("comparator", "output"))    == decoderParameters->GetHotBitPosition("comparator", "output"));
  comparatorData->outputBar = ((registerWord & decoderParameters->GetMask("comparator", "output_bar")) == decoderParameters->GetHotBitPosition("comparator", "output_bar"));

  interruptCheckerData->initialState       = ((registerWord & decoderParameters->GetMask("interrupt_checker", "initial_state")) == decoderParameters->GetHotBitPosition("interrupt_checker", "initial_state"));
  interruptCheckerData->transitionOccurred = ((registerWord & decoderParameters->GetMask("interrupt_checker", "transition"))    == decoderParameters->GetHotBitPosition("interrupt_checker", "transition"));

  unsigned int numberOfDifferentialToSingleEndedChannels = 8;
  unsigned int ioCheckerErrorFlagMask = 0;
  unsigned int ioCheckerErrorFlagBitPosition = 0;
  for (int i = 0; i < numberOfDifferentialToSingleEndedChannels; i++) {
    ioCheckerErrorFlagMask = decoderParameters->GetMask("io_checker", DatatypeConversion::Itos(i));
    ioCheckerErrorFlagBitPosition = decoderParameters->GetHotBitPosition("io_checker", DatatypeConversion::Itos(i));
    ioCheckerData->errorFlagChannel[i] = ((registerWord & ioCheckerErrorFlagMask) == ioCheckerErrorFlagBitPosition);
  }
}

void Decoder::DecodeI2cAuxData(std::string device)
{
  if (device == "P82B96DP") // I2C driver (reading from AD7997)
    DecodeI2cDataFromP82B96DP();
  else {
    std::cout << "Decoder: Unknown I2C AUX device type provided (\"" << device << "\").\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Decoder::DecodeI2cData(std::string device)
{
  if (device == "ADS7830") // ADC
    DecodeI2cDataFromADS7830();
  else if (device == "ADC128D818") // ADC
    DecodeI2cDataFromADC128D818();
  else if (device == "AD7997") // ADC
    DecodeI2cDataFromAD7997();
  else if (device == "PCF8574") // IO Expander (open drain)
    DecodeI2cDataFromPCF8574();
  else if (device == "TCA9535") // IO Expander (push pull)
    DecodeI2cDataFromTCA9535();
  else if (device == "MAX31865_LOW") // Resistance to digital least significant part
    DecodeI2cDataFromMAX31865Low();
  else if (device == "MAX31865_HIGH") // Resistance to digital least significant part
    DecodeI2cDataFromMAX31865High();
  else {
    std::cout << "Decoder: Unknown I2C device type provided (\"" << device << "\").\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Decoder::DecodeI2cDataFromADS7830()
{
  unsigned int i2cMask = 0x000000FF;
  unsigned int i2cDataWord = singleOperation->PopWordFromPayloadBuffer();
  unsigned maskedI2cDataword = (i2cDataWord & i2cMask);
  double adcRealValue = 2.5*((double) maskedI2cDataword)/i2cMask;
  i2cData->data->push(adcRealValue);
}

void Decoder::DecodeI2cDataFromADC128D818()
{
  unsigned int i2cMask = 0x0000FFF0;
  unsigned int i2cDataWord = singleOperation->PopWordFromPayloadBuffer();
  unsigned maskedI2cDataword = (i2cDataWord & i2cMask);
  double adcRealValue = 2.56*((double) maskedI2cDataword)/i2cMask;
  i2cData->data->push(adcRealValue);
}

void Decoder::DecodeI2cDataFromAD7997()
{
  unsigned int i2cMask = 0x00000FFF;
  unsigned int i2cDataWord = singleOperation->PopWordFromPayloadBuffer();
  unsigned maskedI2cDataword = (i2cDataWord & i2cMask);
  double adcRealValue = 2.5*((double) maskedI2cDataword)/i2cMask;
  i2cData->data->push(adcRealValue);
}

void Decoder::DecodeI2cDataFromPCF8574()
{
  unsigned int i2cMask = 0x000000FF;
  unsigned int i2cDataWord = singleOperation->PopWordFromPayloadBuffer();
  unsigned maskedI2cDataword = (i2cDataWord & i2cMask);
  unsigned int numberOfIoExpanderChannels = 8;
  for (int i = 0; i < numberOfIoExpanderChannels; i++) {
    i2cData->ioExpanderChannelState[i]->push((double) ((maskedI2cDataword >> i) & 0x00000001));
  }
}

void Decoder::DecodeI2cDataFromTCA9535()
{
  unsigned int i2cMask = 0x000000FF;
  unsigned int i2cDataWord = singleOperation->PopWordFromPayloadBuffer();
  unsigned maskedI2cDataword = (i2cDataWord & i2cMask);
  unsigned int numberOfIoExpanderChannels = 8;
  for (int i = 0; i < numberOfIoExpanderChannels; i++) {
    i2cData->ioExpanderChannelState[i]->push((double) ((maskedI2cDataword >> i) & 0x00000001));
  }
}

void Decoder::DecodeI2cDataFromP82B96DP()
{
  unsigned int i2cMask = 0x000000FF;
  unsigned int i2cDataWord = singleOperation->PopWordFromPayloadBuffer();
  unsigned maskedI2cDataword = (i2cDataWord & i2cMask);
  i2cData->i2cDriverWord = maskedI2cDataword;
}

void Decoder::DecodeI2cDataFromMAX31865Low()
{
  unsigned int i2cMask = 0x0000007F;
  unsigned int i2cDataWord = singleOperation->PopWordFromPayloadBuffer();
  unsigned maskedI2cDataword = ((i2cDataWord >> 1) & i2cMask);
  double rtdLowRealValue = (105.53/8644)*((double) maskedI2cDataword);
  i2cData->data->push(rtdLowRealValue);
}

void Decoder::DecodeI2cDataFromMAX31865High()
{
  unsigned int i2cMask = 0x00007F80;
  unsigned int i2cDataWord = singleOperation->PopWordFromPayloadBuffer();
  unsigned maskedI2cDataword = ((i2cDataWord << 7) & i2cMask);
  double rtdHighRealValue = ((105.53/8644)*((double) (maskedI2cDataword - 5762))) + 70.33;
  i2cData->data->push(rtdHighRealValue);
}

void Decoder::DecodeAdcData()
{
  unsigned int numberOfAdcBlocks = 4; // Each block is 8 channels
  
  for (int i = 0; i < numberOfAdcBlocks; i++) {
    GetAdcBlockData();
    DecodeAdcBlockData(i);
  }
}

void Decoder::GetAdcBlockData()
{
  unsigned int numberOfWordsInAdcBlock = 3;
  
  for (int i = 0; i < numberOfWordsInAdcBlock; i++) {
    adcData->blockWords[i] = singleOperation->PopWordFromPayloadBuffer();
  }
}

void Decoder::DecodeAdcBlockData(unsigned int blockNumber)
{
  adcData->channels[blockNumber*8 + 0]->push((decoderParameters->GetDynamicRangeAdcs(blockNumber*8 + 0)/4096)*(  adcData->blockWords[0] & 0x00000fff));
  adcData->channels[blockNumber*8 + 1]->push((decoderParameters->GetDynamicRangeAdcs(blockNumber*8 + 1)/4096)*(( adcData->blockWords[0] >> 12)  & 0x00000fff));
  adcData->channels[blockNumber*8 + 2]->push((decoderParameters->GetDynamicRangeAdcs(blockNumber*8 + 2)/4096)*(((adcData->blockWords[1] << 4)   & 0x00000ff0) + ((adcData->blockWords[0] >> 24)  & 0x0000000f)));
  adcData->channels[blockNumber*8 + 3]->push((decoderParameters->GetDynamicRangeAdcs(blockNumber*8 + 3)/4096)*(((adcData->blockWords[1] >> 4)   & 0x00000ff0) + ((adcData->blockWords[0] >> 28)  & 0x0000000f)));
  adcData->channels[blockNumber*8 + 4]->push((decoderParameters->GetDynamicRangeAdcs(blockNumber*8 + 4)/4096)*(((adcData->blockWords[1] >> 12)  & 0x00000ff0) + ((adcData->blockWords[1] >> 24)  & 0x0000000f)));
  adcData->channels[blockNumber*8 + 5]->push((decoderParameters->GetDynamicRangeAdcs(blockNumber*8 + 5)/4096)*(((adcData->blockWords[2] << 4)   & 0x00000ff0) + ((adcData->blockWords[1] >> 28)  & 0x0000000f)));  
  adcData->channels[blockNumber*8 + 6]->push((decoderParameters->GetDynamicRangeAdcs(blockNumber*8 + 6)/4096)*(((adcData->blockWords[2] >> 4)   & 0x00000ff0) + ((adcData->blockWords[2] >> 24)  & 0x0000000f)));
  adcData->channels[blockNumber*8 + 7]->push((decoderParameters->GetDynamicRangeAdcs(blockNumber*8 + 7)/4096)*(((adcData->blockWords[2] >> 12)  & 0x00000ff0) + ((adcData->blockWords[2] >> 28)  & 0x0000000f)));
}

bool Decoder::GetStatusOfFirmwareRegisterBit(std::string bitName)
{
  if      (bitName == "comparator_output")
    return comparatorData->output;
  else if (bitName == "comparator_output_bar")
    return comparatorData->outputBar;
  else if (bitName == "interrupt_checker_initial_state")
    return interruptCheckerData->initialState;
  else if (bitName == "interrupt_checker_transition_occurred")
    return interruptCheckerData->transitionOccurred;
  else if (bitName == "io_checker_error_flag_ch0")
    return ioCheckerData->errorFlagChannel[0];
  else if (bitName == "io_checker_error_flag_ch1")
    return ioCheckerData->errorFlagChannel[1];
  else if (bitName == "io_checker_error_flag_ch2")
    return ioCheckerData->errorFlagChannel[2];
  else if (bitName == "io_checker_error_flag_ch3")
    return ioCheckerData->errorFlagChannel[3];
  else if (bitName == "io_checker_error_flag_ch4")
    return ioCheckerData->errorFlagChannel[4];
  else if (bitName == "io_checker_error_flag_ch5")
    return ioCheckerData->errorFlagChannel[5];
  else if (bitName == "io_checker_error_flag_ch6")
    return ioCheckerData->errorFlagChannel[6];
  else if (bitName == "io_checker_error_flag_ch7")
    return ioCheckerData->errorFlagChannel[7];
  else {
    std::cout << "Decoder: Unknown bit name for firmware input register (\"" << bitName << "\").\n" << std::endl;
    exit(EXIT_FAILURE);
  }
  return true;
}

unsigned int Decoder::GetI2cDriverWord()
{
  return i2cData->i2cDriverWord;
}

double Decoder::PopWordFromDataBuffer(std::string bufferName, std::string channelNumber)
{
  if      (bufferName == "adcs")
    return PopWordFromAdcChannelBuffer(DatatypeConversion::Stoi(channelNumber));
  else if (bufferName == "io_checker")
    return PopWordFromIoCheckerErrorCounterBuffer();
  else if (bufferName == "i2c")
    return PopWordFromI2cBuffer(channelNumber);
  else {
    std::cout << "Decoder: Buffer name not found (\"" << bufferName << "\").\n" << std::endl;
    exit(EXIT_FAILURE);
  }
  return 0.;
}

double Decoder::PopWordFromIoCheckerErrorCounterBuffer()
{
  double valueFromBuffer = ioCheckerData->errorCounterBuffer->front();
  ioCheckerData->errorCounterBuffer->pop();

  return valueFromBuffer;
}

double Decoder::PopWordFromAdcChannelBuffer(unsigned int channelNumber)
{
  double valueFromBuffer = adcData->channels[channelNumber]->front();
  adcData->channels[channelNumber]->pop();

  return valueFromBuffer;
}

double Decoder::PopWordFromI2cBuffer(std::string deviceAndChannel)
{
  std::string deviceName = deviceAndChannel.substr(0,7);
  double valueFromBuffer;
  if (deviceAndChannel == "none") {
    valueFromBuffer = i2cData->data->front();
    i2cData->data->pop();
  }
  else if (deviceName == "PCF8574" || deviceName == "TCA9535") {
    unsigned int channelNumber = DatatypeConversion::Ctoi(deviceAndChannel[11]) - 48;
    if (channelNumber > 7) {
      std::cout << "Decoder: channel number for I2C device is higher than 7 (" << channelNumber << ")\n" << std::endl;
      exit(EXIT_FAILURE);
    }
    valueFromBuffer = i2cData->ioExpanderChannelState[channelNumber]->front();
    i2cData->ioExpanderChannelState[channelNumber]->pop();
  }
  else {
    valueFromBuffer = i2cData->data->front();
    i2cData->data->pop();
  }
   
  return valueFromBuffer;
}
