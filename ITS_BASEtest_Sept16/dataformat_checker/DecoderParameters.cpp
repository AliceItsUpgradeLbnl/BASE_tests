//==========================================================================
// Decoder parameters class - CPP file
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 25/8/2016
// Last modified: 25/8/2016
//==========================================================================

#include "DecoderParameters.h"

//===========================================================================
//                       Constructors and Destructor
//===========================================================================

DecoderParameters::DecoderParameters()
{
  Initialize();
}

DecoderParameters::~DecoderParameters()
{
  Destroy();
}

//===========================================================================
//                            Inner methods
//===========================================================================

// Public Methods

unsigned int DecoderParameters::GetMask(std::string deviceType, std::string bitName)
{
  if      (deviceType == "io_checker")
    return GetIoCheckerErrorFlagMask(DatatypeConversion::Stoi(bitName));
  else if (deviceType == "interrupt_checker")
    return GetInterruptCheckerMask(bitName);
  else if (deviceType == "comparator")
    return GetComparatorMask(bitName);
  else {
    std::cout << "DecoderParameters: Unknown device type provided while trying to get a bit mask. User provided argument \"" << deviceType << "\" which is invalid.\n" << std::endl; 
    exit(EXIT_FAILURE);
  }
  return 0;
}

unsigned int DecoderParameters::GetIoCheckerErrorFlagMask(unsigned int channelNumber)
{
  return dataMasks->ioCheckerErrorFlags[channelNumber];
}

unsigned int DecoderParameters::GetComparatorMask(std::string bitName)
{
  if      (bitName == "output")
    return dataMasks->comparatorOutput;
  else if (bitName == "output_bar")
    return dataMasks->comparatorOutputBar;
  else {
    std::cout << "DecoderParameters: Unknown bit name provided while trying to get the mask for comparator device.\n" << std::endl;
    exit(EXIT_FAILURE);
  }
  return 0;
}

unsigned int DecoderParameters::GetInterruptCheckerMask(std::string bitName)
{
  if      (bitName == "initial_state")
    return dataMasks->interruptCheckerInitialState;
  else if (bitName == "transition")
    return dataMasks->interruptCheckerTransition;
  else {
    std::cout << "DecoderParameters: Unknown bit name provided while trying to get the mask for interrupt checker device.\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

unsigned int DecoderParameters::GetHotBitPosition(std::string deviceType, std::string bitName)
{
  if      (deviceType == "io_checker")
    return GetIoCheckerErrorFlagHotBitPosition(DatatypeConversion::Stoi(bitName));
  else if (deviceType == "interrupt_checker")
    return GetInterruptCheckerHotBitPosition(bitName);
  else if (deviceType == "comparator")
    return GetComparatorHotBitPosition(bitName);
  else {
    std::cout << "DecoderParameters: Unknown device type provided while trying to get the hot bit positions for a device. User provided argument \"" << deviceType << "\" which is invalid.\n" << std::endl; 
    exit(EXIT_FAILURE);
  }
  return 0;
}

unsigned int DecoderParameters::GetIoCheckerErrorFlagHotBitPosition(unsigned int channelNumber)
{
  return hotBitPositions->ioCheckerErrorFlags[channelNumber];
}

unsigned int DecoderParameters::GetComparatorHotBitPosition(std::string bitName)
{
  if      (bitName == "output")
    return hotBitPositions->comparatorOutput;
  else if (bitName == "output_bar")
    return hotBitPositions->comparatorOutputBar;
  else {
    std::cout << "DecoderParameters: Unknown bit name provided while trying to get the mask for comparator device.\n" << std::endl;
    exit(EXIT_FAILURE);
  }
  return 0;
}

unsigned int DecoderParameters::GetInterruptCheckerHotBitPosition(std::string bitName)
{
  if      (bitName == "initial_state")
    return hotBitPositions->interruptCheckerInitialState;
  else if (bitName == "transition")
    return hotBitPositions->interruptCheckerTransition;
  else {
    std::cout << "DecoderParameters: Unknown bit name provided while trying to get the hot bit position for interrupt checker device.\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

double DecoderParameters::GetDynamicRangeAdcs(unsigned int channelNumber)
{
  return dynamicRangeAdcs[channelNumber];
} 

// Private Methods

void DecoderParameters::Initialize()
{
  dataMasks        = new RegisterBitFields;
  hotBitPositions = new RegisterBitFields;

  unsigned int numberOfIoCheckerChannels = 8;
  for (int i = 0; i < numberOfIoCheckerChannels; i++) {
    dataMasks->ioCheckerErrorFlags[i] = pow(2,i + 24);
    hotBitPositions->ioCheckerErrorFlags[i] = pow(2,i + 24);
  }

  dataMasks->comparatorOutput    = 0x00000001;
  dataMasks->comparatorOutputBar = 0x00000002;

  dataMasks->interruptCheckerInitialState = 0x00000040;
  dataMasks->interruptCheckerTransition = 0x00000080;

  hotBitPositions->comparatorOutput    = 0x00000001;
  hotBitPositions->comparatorOutputBar = 0x00000002;

  hotBitPositions->interruptCheckerInitialState = 0x00000040;
  hotBitPositions->interruptCheckerTransition = 0x00000080;

  dynamicRangeAdcs[0] = 2.5;
  dynamicRangeAdcs[1] = 2.5;
  dynamicRangeAdcs[2] = 2.5;
  dynamicRangeAdcs[3] = 2.5;
  dynamicRangeAdcs[4] = 2.5;
  dynamicRangeAdcs[5] = 2.5;
  dynamicRangeAdcs[6] = 2.5;
  dynamicRangeAdcs[7] = 2.5;
  dynamicRangeAdcs[8] = 2.5;
  dynamicRangeAdcs[9] = 2.5;
  dynamicRangeAdcs[10] = 2.5;
  dynamicRangeAdcs[11] = 2.5;
  dynamicRangeAdcs[12] = 2.5;
  dynamicRangeAdcs[13] = 2.5;
  dynamicRangeAdcs[14] = 2.5;
  dynamicRangeAdcs[15] = 2.5;
  dynamicRangeAdcs[16] = 5.0;
  dynamicRangeAdcs[17] = 5.0;
  dynamicRangeAdcs[18] = 5.0;
  dynamicRangeAdcs[19] = 5.0;
  dynamicRangeAdcs[20] = 5.0;
  dynamicRangeAdcs[21] = 5.0;
  dynamicRangeAdcs[22] = 5.0;
  dynamicRangeAdcs[23] = 5.0;
  dynamicRangeAdcs[24] = 5.0;
  dynamicRangeAdcs[25] = 5.0;
  dynamicRangeAdcs[26] = 5.0;
  dynamicRangeAdcs[27] = 5.0;
  dynamicRangeAdcs[28] = 5.0;
  dynamicRangeAdcs[29] = 5.0;
  dynamicRangeAdcs[30] = 5.0;
  dynamicRangeAdcs[31] = 5.0;
}

void DecoderParameters::Destroy()
{
  delete dataMasks;
}
