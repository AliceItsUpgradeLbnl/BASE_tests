//==========================================================================
// Composite Operation class - CPP file
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 25/8/2016
// Last modified: 25/8/2016
// Description: contains methods to check the data (in 32-bit words)
//              acquired during the online/offline tests
//==========================================================================

#include "CompositeOperation.h"

//===========================================================================
//                       Constructors and Destructor
//===========================================================================

CompositeOperation::CompositeOperation(char* filename, std::string compositeOperationType)
{
  Initialize(filename, compositeOperationType);
}

CompositeOperation::~CompositeOperation()
{
  Destroy();
}

//===========================================================================
//                            Inner methods
//===========================================================================

// Public Methods

void CompositeOperation::CheckOperationDataFormat(unsigned int positionOfOperationHeader)
{
  fileReader->ReadDataFromFile(operationHeader, positionOfOperationHeader, numberOfWordsOfOperation);
  RunChecksOnOperationData();
}

void CompositeOperation::GetOperationData(unsigned int positionOfOperationHeader)
{
  fileReader->ReadDataFromFile(operationHeader, positionOfOperationHeader, numberOfWordsOfOperation);
  DecodeAndReformatOperationData();
}

// Private Methods

void CompositeOperation::Initialize(char* filename, std::string compositeOperationType)
{
  compositeOperationParameters = new CompositeOperationParameters;
  fileReader = new FileReader(filename);
  singleOperation = new SingleOperation(fileReader);
  decoder = new Decoder(singleOperation);
  statistics = new Statistics;

  onlineData = new OnlineData;
  onlineData->numberOfPointsPerRun = new std::queue<unsigned int>;

  comparatorData = new ComparatorData;
  comparatorData->output = new std::queue<bool>;
  comparatorData->outputBar = new std::queue<bool>;

  adcData = new AdcData;
  adcData->sigma = new std::queue<double>*[32];
  adcData->mean  = new std::queue<double>*[32];
  for (int i = 0; i < 32; i++) adcData->mean[i] = new std::queue<double>;
  for (int i = 0; i < 32; i++) adcData->sigma[i] = new std::queue<double>;

  i2cData = new I2cData;

  i2cData->resistanceToDigital = new ResistanceToDigital;
  i2cData->resistanceToDigital->mean = new std::queue<double>;
  i2cData->resistanceToDigital->sigma = new std::queue<double>;

  i2cData->adcs = new AdcDataDut*[3];
  i2cData->i2cDriver = new std::queue<unsigned int>;
  for (int i = 0; i < 3; i++) {
    i2cData->adcs[i] = new AdcDataDut;
    i2cData->adcs[i]->mean = new std::queue<double>*[8];
    i2cData->adcs[i]->sigma = new std::queue<double>*[8];
    for (int j = 0; j < 8; j++) {
      i2cData->adcs[i]->mean[j] = new std::queue<double>;
      i2cData->adcs[i]->sigma[j] = new std::queue<double>;
    }
  }

  i2cData->ioExpander = new IoExpander;
  i2cData->ioExpander->initial_state = new std::queue<bool>*[8];
  i2cData->ioExpander->transition_occurred = new std::queue<bool>*[8];
  for (int i = 0; i < 8; i++) {
    i2cData->ioExpander->initial_state[i] = new std::queue<bool>;
    i2cData->ioExpander->transition_occurred[i] = new std::queue<bool>;
  }  

  i2cData->differentialToSingleEndedDriver = new std::queue<unsigned int>*[8];
  for (int i = 0; i < 8; i++) {
    i2cData->differentialToSingleEndedDriver[i] = new std::queue<unsigned int>;
  }

  this->compositeOperationType = compositeOperationType;
  this->operationHeader = compositeOperationParameters->GetHeader(compositeOperationType);
  this->operationTrailer = compositeOperationParameters->GetTrailer();
  this->numberOfWordsOfOperation = compositeOperationParameters->GetNumberOfWordsOfOperation(compositeOperationType);
}

void CompositeOperation::Destroy()
{ 
  delete compositeOperationParameters;
  delete fileReader;
  delete singleOperation;
  delete decoder;
  delete statistics;

  delete adcData;
  delete i2cData;
  delete comparatorData;
  delete onlineData;
}

void CompositeOperation::DecodeAndReformatOperationData()
{
  CheckOperationHeader();
  GetOperationPayload();
  CheckOperationTrailer();
}

void CompositeOperation::GetOperationPayload()
{
  if      (compositeOperationType == "differential_amplifiers")
    FillDataStructuresDifferentialAmplifiers();
  else if (compositeOperationType == "comparator")
    FillDataStructuresComparator();
  else if (compositeOperationType == "dacs")
    FillDataStructuresDacs();
  else if (compositeOperationType == "resistance_to_digital")
    FillDataStructuresResistanceToDigital();
  else if (compositeOperationType == "io_expander")
    FillDataStructuresIoExpander();
  else if (compositeOperationType == "adcs")
    FillDataStructuresAdcs();
  else if (compositeOperationType == "i2c_driver")
    FillDataStructuresI2cDriver();
  else if (compositeOperationType == "differential_to_single_ended_driver")
    FillDataStructuresDifferentialToSingleEndedDriver();
  else if (compositeOperationType == "dcdc_converters")
    FillDataStructuresDcdcConverters();
  else if (compositeOperationType == "online_monitoring")
    FillDataStructuresOnlineMonitoring();
  else { 
    std::cout << "CompositeOperation: Provided non-valid composite operation name.\n" << std::endl; 
    exit(EXIT_FAILURE);
  }
}

void CompositeOperation::FillDataStructuresDifferentialAmplifiers()
{ 
  unsigned int numberOfPoints = 22;
  unsigned int numberOfSamplesPerPoint = 10;
  unsigned int numberOfAdcChannels = 32;
  for (int i = 0; i < numberOfPoints; i++) {
    for (int j = 0; j < numberOfSamplesPerPoint; j++) {
      singleOperation->GetSingleOperationData("adcs");
      decoder->DecodeData("adcs", "none");
    }
    for (int j = 0; j < numberOfAdcChannels; j++) {
      for (int k = 0; k < numberOfSamplesPerPoint; k++) {
        double wordToPush = decoder->PopWordFromDataBuffer("adcs", DatatypeConversion::Itos(j));
        statistics->PushWordToDataBuffer(wordToPush);
      }
      statistics->DetermineMeanAndSigma(numberOfSamplesPerPoint);
      adcData->mean[j]->push(statistics->GetMean());
      adcData->sigma[j]->push(statistics->GetSigma());
    }
  }
}

void CompositeOperation::FillDataStructuresComparator()
{
  unsigned int numberOfPoints = 22;
  for (int i = 0; i < numberOfPoints; i++) {
    singleOperation->GetSingleOperationData("register_fields");
    decoder->DecodeData("register_fields", "none");
    comparatorData->output->push(decoder->GetStatusOfFirmwareRegisterBit("comparator_output"));
    comparatorData->outputBar->push(decoder->GetStatusOfFirmwareRegisterBit("comparator_output_bar"));
  }
}

void CompositeOperation::FillDataStructuresDacs()
{
  unsigned int numberOfPoints = 22;
  unsigned int numberOfSamplesPerPoint = 10;
  unsigned int numberOfAdcChannels = 32;
  for (int i = 0; i < numberOfPoints; i++) {
    for (int j = 0; j < numberOfSamplesPerPoint; j++) {
      singleOperation->GetSingleOperationData("adcs");
      decoder->DecodeData("adcs", "none");
    }
    for (int j = 0; j < numberOfAdcChannels; j++) {
      for (int k = 0; k < numberOfSamplesPerPoint; k++) statistics->PushWordToDataBuffer(decoder->PopWordFromDataBuffer("adcs", DatatypeConversion::Itos(j)));
      statistics->DetermineMeanAndSigma(numberOfSamplesPerPoint);
      adcData->mean[j]->push(statistics->GetMean());
      adcData->sigma[j]->push(statistics->GetSigma());
    }
  } 
}

void CompositeOperation::FillDataStructuresResistanceToDigital()
{
  unsigned int numberOfPoints = 21;
  unsigned int numberOfReadsPerPoint = 4;
  double resistance = 0.;
  for (int i = 0; i < numberOfPoints; i++) {
    for (int j = 0; j < numberOfReadsPerPoint; j++) {
      singleOperation->GetSingleOperationData("i2c");
      decoder->DecodeData("i2c", "MAX31865_LOW");
      resistance = decoder->PopWordFromDataBuffer("i2c", "no_channel");
      singleOperation->GetSingleOperationData("i2c");
      decoder->DecodeData("i2c", "MAX31865_HIGH");
      resistance += decoder->PopWordFromDataBuffer("i2c", "no_channel");
      statistics->PushWordToDataBuffer(resistance);
    }
    statistics->DetermineMeanAndSigma(numberOfReadsPerPoint);
    i2cData->resistanceToDigital->mean->push(statistics->GetMean());
    i2cData->resistanceToDigital->sigma->push(statistics->GetSigma());
  }
}

void CompositeOperation::FillDataStructuresIoExpander()
{
  unsigned int numberOfPreliminaryTests = 3;
  unsigned int numberOfChannels = 8;
  unsigned int numberOfReadsPerChannel = 8; // Not taking into account the first
  std::string deviceAndChannel;
  bool opendrain_state, pushpull_state;
  // First part of the test
  for (int testNumber = 0; testNumber < numberOfPreliminaryTests; testNumber++) {
    singleOperation->GetSingleOperationData("i2c");
    decoder->DecodeData("i2c", "PCF8574");
    singleOperation->GetSingleOperationData("i2c");
    decoder->DecodeData("i2c", "TCA9535");
    for (int i = 0; i < numberOfChannels; i++) {
      char channel = DatatypeConversion::Itoc(i + 48);
      deviceAndChannel = "PCF8574_CH_";
      deviceAndChannel.append(&channel);
      opendrain_state = decoder->PopWordFromDataBuffer("i2c", deviceAndChannel);
      deviceAndChannel = "TCA9535_CH_";
      deviceAndChannel.append(&channel);
      pushpull_state = decoder->PopWordFromDataBuffer("i2c", deviceAndChannel);
      if (testNumber == 0)
        i2cData->ioExpander->initial_state[0]->push(!opendrain_state && !pushpull_state);
      else if (testNumber == 1)
        i2cData->ioExpander->initial_state[0]->push(!opendrain_state && !pushpull_state);
      else if (testNumber == 2)
        i2cData->ioExpander->initial_state[0]->push(opendrain_state && pushpull_state);
      else {
        std::cout << "CompositeOperation: number of test is out of range." << std::endl;
      }
    }
  }
  // Second part of the test
  for (int i = 0; i < numberOfChannels; i++) {
    singleOperation->GetSingleOperationData("i2c");
    decoder->DecodeData("i2c", "TCA9535");
    for (int j = 0; j < numberOfChannels; j++) { 
      char channel = DatatypeConversion::Itoc(j + 48);
      deviceAndChannel = "TCA9535_CH_";
      deviceAndChannel.append(&channel);
      pushpull_state = (bool) decoder->PopWordFromDataBuffer("i2c", deviceAndChannel);
      if (pushpull_state) {
        std::cout << "CompositeOperation: state of the IO open drain expander is non-false." << std::endl;
      }
    }
    singleOperation->GetSingleOperationData("register_fields");
    decoder->DecodeData("register_fields", "none");
    i2cData->ioExpander->initial_state[0]->push(decoder->GetStatusOfFirmwareRegisterBit("interrupt_checker_initial_state"));  
    i2cData->ioExpander->transition_occurred[0]->push(decoder->GetStatusOfFirmwareRegisterBit("interrupt_checker_transition_occurred"));    
  }
  // Third part of the test
  for (int i = 0; i < numberOfChannels; i++) {
    singleOperation->GetSingleOperationData("i2c");
    decoder->DecodeData("i2c", "PCF8574");
    for (int j = 0; j < numberOfChannels; j++) { 
      char channel = DatatypeConversion::Itoc(j + 48);
      deviceAndChannel = "PCF8574_CH_";
      deviceAndChannel.append(&channel);
      opendrain_state = (bool) decoder->PopWordFromDataBuffer("i2c", deviceAndChannel);
      if (!opendrain_state) {
        std::cout << "CompositeOperation: state of the push-pull IO expander is non-true." << std::endl;
      }
    }
    singleOperation->GetSingleOperationData("register_fields");
    decoder->DecodeData("register_fields", "none");
    i2cData->ioExpander->initial_state[0]->push(decoder->GetStatusOfFirmwareRegisterBit("interrupt_checker_initial_state"));  
    i2cData->ioExpander->transition_occurred[0]->push(decoder->GetStatusOfFirmwareRegisterBit("interrupt_checker_transition_occurred"));    
  }
}

void CompositeOperation::FillDataStructuresAdcs()
{
  unsigned int numberOfAdcs = 3;
  unsigned int numberOfPoints = 11;
  unsigned int numberOfChannels = 8;
  unsigned int numberofSamplesPerChannel = 1;
  for (int i = 0; i < numberOfPoints; i++) {
    for (int adcNumber = 0; adcNumber < numberOfAdcs; adcNumber++) {
      for (int j = 0; j < numberOfChannels; j++) {
        singleOperation->GetSingleOperationData("i2c");
        if (adcNumber == 0)
          decoder->DecodeData("i2c", "ADS7830");
        else if (adcNumber == 1)
          decoder->DecodeData("i2c", "ADC128D818");
        else if (adcNumber == 2)
          decoder->DecodeData("i2c", "AD7997");
        double wordToPush = decoder->PopWordFromDataBuffer("i2c", "no_channel");
        i2cData->adcs[adcNumber]->mean[j]->push(wordToPush);
      }
    }
  }
}

void CompositeOperation::FillDataStructuresI2cDriver()
{
  unsigned int numberOfTests = 10;
  unsigned int numberofPatternsPerTest = 2;
  unsigned int numberOfBitsToCheck = 16;
  unsigned int errorPattern = 0;
  unsigned int numberOfErrors = 0;
  for (int i = 0; i < numberOfTests; i++) {
    for (int j = 0; j < numberofPatternsPerTest; j++) {
      singleOperation->GetSingleOperationData("i2c_aux");
      decoder->DecodeData("i2c_aux", "P82B96DP");
      errorPattern = decoder->GetI2cDriverWord();
      if (j == 0) errorPattern ^= 0x00000055;
      else        errorPattern ^= 0x000000AA;
      for (int k = 0; k < numberOfBitsToCheck; k++) numberOfErrors += ((errorPattern >> k) & 0x00000001);
    }
  }
  i2cData->i2cDriver->push(numberOfErrors);
}

void CompositeOperation::FillDataStructuresDifferentialToSingleEndedDriver()
{
  unsigned int numberOfPoints = 32;
  unsigned int numberOfChannels = 8;
  for (int i = 0; i < numberOfPoints; i++) {
    singleOperation->GetSingleOperationData("io_checker");
    decoder->DecodeData("io_checker", "none");
    for (int j = 0; j < numberOfChannels; j++) {
      unsigned int wordToPush = decoder->PopWordFromDataBuffer("io_checker", DatatypeConversion::Itos(i));
      i2cData->differentialToSingleEndedDriver[j]->push(wordToPush);
    }
  }
}

void CompositeOperation::FillDataStructuresDcdcConverters()
{
  unsigned int numberOfPoints = 2;
  unsigned int numberOfSamplesPerPoint = 10;
  unsigned int numberOfAdcChannels = 32;
  for (int i = 0; i < numberOfPoints; i++) {
    for (int j = 0; j < numberOfSamplesPerPoint; j++) {
      singleOperation->GetSingleOperationData("adcs");
      decoder->DecodeData("adcs", "none");
    }
    for (int j = 0; j < numberOfAdcChannels; j++) {
      for (int k = 0; k < numberOfSamplesPerPoint; k++) statistics->PushWordToDataBuffer(decoder->PopWordFromDataBuffer("adcs", DatatypeConversion::Itos(j)));
      statistics->DetermineMeanAndSigma(numberOfSamplesPerPoint);
      adcData->mean[j]->push(statistics->GetMean());
      adcData->sigma[j]->push(statistics->GetSigma());
    }
  }
}

void CompositeOperation::FillDataStructuresOnlineMonitoring()
{
  unsigned int numberOfAdcChannels = 32;
  unsigned int numberOfSamplesPerPoint = 10;
  while(true) {
    if (!singleOperation->GetOnlineMonitoringData()) {
      break;
    }
    decoder->DecodeData("adcs", "none");
  }
  onlineData->numberOfRuns = singleOperation->GetNumberOfRuns();
  unsigned int numberOfPointsPerRun[onlineData->numberOfRuns];
  for (int i = 0; i < onlineData->numberOfRuns; i++) {
    numberOfPointsPerRun[i] = singleOperation->PopWordFromNumberOfSamplesPerRunBuffer()/numberOfSamplesPerPoint;
    onlineData->numberOfPointsPerRun->push(numberOfPointsPerRun[i]);
  }  
  for (int k = 0; k < onlineData->numberOfRuns; k++) {
    for (int y = 0; y < numberOfPointsPerRun[k]; y++) {
      for (int i = 0; i < numberOfAdcChannels; i++) {
        for (int j = 0; j < numberOfSamplesPerPoint; j++) statistics->PushWordToDataBuffer(decoder->PopWordFromDataBuffer("adcs", DatatypeConversion::Itos(i)));
        statistics->DetermineMeanAndSigma(numberOfSamplesPerPoint);
        adcData->mean[i]->push(statistics->GetMean());
        adcData->sigma[i]->push(statistics->GetSigma());
      }
    }
  }
}

void CompositeOperation::RunChecksOnOperationData()
{
  CheckOperationHeader();
  CheckOperationPayload();
  CheckOperationTrailer();
}

void CompositeOperation::CheckOperationHeader()
{
  unsigned int header = fileReader->PopWordFromDataBuffer(); 
  if (header != operationHeader){
    std::cout << "CompositeOperation: Found non-valid composite operation header. Expected: " << DatatypeConversion::Itoh(operationHeader, true) << " Found: " << DatatypeConversion::Itoh(header, true) << "\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void CompositeOperation::CheckOperationPayload()
{
  if      (compositeOperationType == "differential_amplifiers")
    CheckOperationPayloadDifferentialAmplifiers();
  else if (compositeOperationType == "comparator")
    CheckOperationPayloadComparator();
  else if (compositeOperationType == "dacs")
    CheckOperationPayloadDacs();
  else if (compositeOperationType == "resistance_to_digital")
    CheckOperationPayloadResistanceToDigital();
  else if (compositeOperationType == "io_expander")
    CheckOperationPayloadIoExpander();
  else if (compositeOperationType == "adcs")
    CheckOperationPayloadAdcs();
  else if (compositeOperationType == "i2c_driver")
    CheckOperationPayloadI2cDriver();
  else if (compositeOperationType == "differential_to_single_ended_driver")
    CheckOperationPayloadDifferentialToSingleEndedDriver();
  else if (compositeOperationType == "dcdc_converters")
    CheckOperationPayloadDcdcConverters();
  else if (compositeOperationType == "online_monitoring")
    CheckOperationPayloadOnlineMonitoring();
  else { 
    std::cout << "CompositeOperation: Consistency checks for the selected composite operation are not implemented.\n" << std::endl; 
    exit(EXIT_FAILURE);
  }
}

void CompositeOperation::CheckOperationTrailer()
{
  if (compositeOperationType == "online_monitoring") {
  }
  else { 
    unsigned int trailer = fileReader->PopWordFromDataBuffer();
    if (trailer != operationTrailer) {
      std::cout << "CompositeOperation: Found non-valid composite operation trailer. Expected: " << DatatypeConversion::Itoh(operationTrailer, true) << " Found: " << DatatypeConversion::Itoh(trailer, true) << "\n" << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

void CompositeOperation::CheckOperationPayloadOnlineMonitoring()
{
  unsigned int i = 0;
  while(true) {
    if (!singleOperation->CheckOnlineMonitoringData()) {
      break;
    }
    i++;
  }
}

void CompositeOperation::CheckOperationPayloadDifferentialAmplifiers()
{
  unsigned int numberOfPoints = 22;
  unsigned int numberofSamplesPerPoint = 10;
  for (int i = 0; i < numberOfPoints; i++) {
    for (int j = 0; j < numberofSamplesPerPoint; j++) {
      if (!singleOperation->CheckSingleOperationData("adcs")) {
        std::cout << "CompositeOperation: Error found while checking data consistency for the differential amplifiers on the DUT board at point number " << i << " and sample number " << j << "\n" << std::endl;
        exit(EXIT_FAILURE);
      }
    }
  }
}

void CompositeOperation::CheckOperationPayloadComparator()
{
  unsigned int numberOfPoints = 22;
  for (int i = 0; i < numberOfPoints; i++) {
    if (!singleOperation->CheckSingleOperationData("register_fields")) {
      std::cout << "CompositeOperation: Error found while checking data consistency for the comparator on the DUT board at point number " << i << "\n" << std::endl;
      exit(EXIT_FAILURE);      
    }
  }
}

void CompositeOperation::CheckOperationPayloadDacs()
{
  unsigned int numberOfPoints = 22;
  unsigned int numberofSamplesPerPoint = 10;
  for (int i = 0; i < numberOfPoints; i++) {
    for (int j = 0; j < numberofSamplesPerPoint; j++) {
      if (!singleOperation->CheckSingleOperationData("adcs")) {
        std::cout << "CompositeOperation: Error found while checking data consistency for the DACs on the DUT board at point number " << i << " and sample number " << j << "\n" << std::endl;
        exit(EXIT_FAILURE);
      }
    }
  }
}

void CompositeOperation::CheckOperationPayloadAdcs()
{
  unsigned int numberOfAdcs = 3;
  unsigned int numberOfPoints = 11;
  unsigned int numberOfChannels = 8;
  unsigned int numberofSamplesPerChannel = 10;
  for (int adcNumber = 0; adcNumber < numberOfAdcs; adcNumber++) {
    for (int i = 0; i < numberOfPoints; i++) {
      for (int j = 0; j < numberOfChannels; j++) {
        if (!singleOperation->CheckSingleOperationData("i2c")) {
          std::cout << "CompositeOperation: Error found while checking data consistency for ADC " << adcNumber << " on the DUT board at point number " << i << " channel number " << j << "\n" << std::endl;
          exit(EXIT_FAILURE);
        }
      }
    }
  }
}

void CompositeOperation::CheckOperationPayloadResistanceToDigital()
{
  unsigned int numberOfPoints = 21;
  unsigned int numberOfReadsPerPoint = 4;
  unsigned int numberOfRegisters = 2;
  for (int i = 0; i < numberOfPoints; i++) {
    for (int j = 0; j < numberOfReadsPerPoint; j++) {
      for (int k = 0 ; k < numberOfRegisters; k++) {
        if (!singleOperation->CheckSingleOperationData("i2c")) {
          std::cout << "CompositeOperation: Error found while checking data consistency for the resistance to digital on the DUT board at point number " << i << ", read number " << j << " and register number " << k << "\n" << std::endl;
          exit(EXIT_FAILURE);
        }
      }
    }
  }
}

void CompositeOperation::CheckOperationPayloadDifferentialToSingleEndedDriver()
{
  unsigned int numberOfPoints = 32;
  for (int i = 0; i < numberOfPoints; i++) {
    if (!singleOperation->CheckSingleOperationData("io_checker")) {
      std::cout << "CompositeOperation: Error found while checking data consistency for the differential to single ended on the DUT board at point number " << i << "\n" << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

void CompositeOperation::CheckOperationPayloadIoExpander()
{
  unsigned int numberOfChannels = 8;
  unsigned int numberOfI2cReadsFirstPart = 6;
  for (int i = 0; i < numberOfI2cReadsFirstPart; i++) {
    if (!singleOperation->CheckSingleOperationData("i2c")) {
      std::cout << "CompositeOperation: Error found while checking data consistency for the IO expander on the DUT board (stuck at first part).\n" << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  for (int i = 0; i < numberOfChannels; i++) {   
    if (!singleOperation->CheckSingleOperationData("i2c")) {
      std::cout << "CompositeOperation: Error found while checking data consistency for the IO expander on the DUT board (stuck at second part - I2C).\n" << std::endl;
      exit(EXIT_FAILURE);
    }
    if (!singleOperation->CheckSingleOperationData("register_fields")) {
      std::cout << "CompositeOperation: Error found while checking data consistency for the IO expander on the DUT board (stuck at second part - register).\n" << std::endl;
      exit(EXIT_FAILURE);
    } 
  }
  for (int i = 0; i < numberOfChannels; i++) {
    if (!singleOperation->CheckSingleOperationData("i2c")) {
      std::cout << "CompositeOperation: Error found while checking data consistency for the IO expander on the DUT board (stuck at third part - I2C).\n" << std::endl;
      exit(EXIT_FAILURE);
    }  
    if (!singleOperation->CheckSingleOperationData("register_fields")) {
      std::cout << "CompositeOperation: Error found while checking data consistency for the IO expander on the DUT board (stuck at third part - register).\n" << std::endl;
      exit(EXIT_FAILURE);
    }    
  }
}

void CompositeOperation::CheckOperationPayloadDcdcConverters()
{
  unsigned int numberOfPoints = 2;
  unsigned int numberofSamplesPerPoint = 10;
  for (int i = 0; i < numberOfPoints; i++) {
    for (int j = 0; j < numberofSamplesPerPoint; j++) {
      if (!singleOperation->CheckSingleOperationData("adcs")) {
        std::cout << "CompositeOperation: Error found while checking data consistency for the DC-DC converters on the DUT board at point number " << i << " and sample number " << j << "\n" << std::endl;
        exit(EXIT_FAILURE);
      }
    }
  }
}

void CompositeOperation::CheckOperationPayloadI2cDriver()
{
  unsigned int numberOfTests = 10;
  unsigned int numberofPatternsPerTest = 2;
  for (int i = 0; i < numberOfTests; i++) {
    for (int j = 0; j < numberofPatternsPerTest; j++) {
      if (!singleOperation->CheckSingleOperationData("i2c_aux")) {
        std::cout << "CompositeOperation: Error found while checking data consistency for the I2C driver on the DUT board during test number " << i << " and pattern number " << j << "\n" << std::endl;
        exit(EXIT_FAILURE);
      }
    }
  }
}
