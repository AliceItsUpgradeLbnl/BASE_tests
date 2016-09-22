//==========================================================================
// Data format checker
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 25/8/2016
// Last modified: 25/8/2016
// Description: performs data consistency check for the offline operations
// and for the online monitoring data
//==========================================================================


#include "CompositeOperation.h"

void LeaveOneLineSpacing();
void CheckNumberOfParameters(unsigned int numberOfParameters, char* thisFile);
void CheckIfInputFileExistsAndNotEmpty(FILE* filename);
void PrintWrongNumberOfParametersErrorMessage();
void PrintCommandLineSyntax(char* thisFile);
void PrintWrongExpectedFormatErrorMessage();
void PrintListOfExpectedFormats();

int main(int argc, char* argv[])
{
  CheckNumberOfParameters(argc, argv[0]);

  char *dataFile = argv[1];

  CompositeOperationParameters *compositeOperationParameters = new CompositeOperationParameters;

  LeaveOneLineSpacing();

  unsigned int positionOfOperationHeader = 1;

  if (!(strcmp(argv[2], "differential_amplifiers"))) {
    // Check differential amplifiers data
    std::cout << "-> 1) Starting check on differential amplifiers data....." << std::endl;
    std::string compositeOperationType = "differential_amplifiers";
    CompositeOperation *differentialAmplifiers = new CompositeOperation(dataFile, compositeOperationType);
    differentialAmplifiers->CheckOperationDataFormat(positionOfOperationHeader);
    delete differentialAmplifiers;
    std::cout << "-> Check on the differential amplifiers data format performed correctly.\n" << std::endl;
  }
  else if (!(strcmp(argv[2], "comparator"))) {
    // Check comparator data
    std::cout << "-> 2) Starting check on comparator data....." << std::endl;
    std::string compositeOperationType = "comparator";
    CompositeOperation *comparator = new CompositeOperation(dataFile, compositeOperationType);
    comparator->CheckOperationDataFormat(positionOfOperationHeader);
    delete comparator;
    std::cout << "-> Check on the comparator data format performed correctly.\n" << std::endl;
  }
  else if (!(strcmp(argv[2], "dacs"))) {
    // Check DACs data
    std::cout << "-> 3) Starting check on DACs data....." << std::endl;
    std::string compositeOperationType = "dacs";
    CompositeOperation *dacs = new CompositeOperation(dataFile, compositeOperationType);
    dacs->CheckOperationDataFormat(positionOfOperationHeader);
    delete dacs;
    std::cout << "-> Check on the DACs data format performed correctly.\n" << std::endl;
  }
  else if (!(strcmp(argv[2], "resistance_to_digital"))) {
    // Check resistance to digital data
    std::cout << "-> 4) Starting check on resistance to digital data....." << std::endl;
    std::string compositeOperationType = "resistance_to_digital";
    CompositeOperation *resistanceToDigital = new CompositeOperation(dataFile, compositeOperationType);
    resistanceToDigital->CheckOperationDataFormat(positionOfOperationHeader);
    delete resistanceToDigital;
    std::cout << "-> Check on the resistance to digital data format performed correctly.\n" << std::endl;
  }
  else if (!(strcmp(argv[2], "io_expander"))) {
    // Check IO expander data
    std::cout << "-> 5) Starting check on IO expander data....." << std::endl;
    std::string compositeOperationType = "io_expander";
    CompositeOperation *ioExpander = new CompositeOperation(dataFile, compositeOperationType);
    ioExpander->CheckOperationDataFormat(positionOfOperationHeader);
    delete ioExpander;
    std::cout << "-> Check on the IO expander data format performed correctly.\n" << std::endl;
  }
  else if (!(strcmp(argv[2], "adcs"))) {
    // Check ADCs data
    std::cout << "-> 6) Starting check on ADCs data....." << std::endl;
    std::string compositeOperationType = "adcs";
    CompositeOperation *adcs = new CompositeOperation(dataFile, compositeOperationType);
    adcs->CheckOperationDataFormat(positionOfOperationHeader);
    delete adcs;
    std::cout << "-> Check on the ADCs data format performed correctly.\n" << std::endl;
  }
  else if (!(strcmp(argv[2], "i2c_driver"))) {
    // Check I2C driver data
    std::cout << "-> 7) Starting check on I2C driver data....." << std::endl;
    std::string compositeOperationType = "i2c_driver";
    CompositeOperation *i2cDriver = new CompositeOperation(dataFile, compositeOperationType);
    i2cDriver->CheckOperationDataFormat(positionOfOperationHeader);
    delete i2cDriver;
    std::cout << "-> Check on the I2C driver data format performed correctly.\n" << std::endl;
  }
  else if (!(strcmp(argv[2], "differential_to_single_ended_driver"))) {
    // Check differential to single ended data
    std::cout << "-> 8) Starting check on differential to single ended data....." << std::endl;
    std::string compositeOperationType = "differential_to_single_ended_driver";
    CompositeOperation *differentialToSingleEndedDriver = new CompositeOperation(dataFile, compositeOperationType);
    differentialToSingleEndedDriver->CheckOperationDataFormat(positionOfOperationHeader);
    delete differentialToSingleEndedDriver;
    std::cout << "-> Check on the differential to single ended data format performed correctly.\n" << std::endl;
  }
  else if (!(strcmp(argv[2], "dcdc_converters"))) {
    // Check DC-DC converters data
    std::cout << "-> 9) Starting check on DC-DC converters data....." << std::endl;
    std::string compositeOperationType = "dcdc_converters";
    CompositeOperation *dcdcConverters = new CompositeOperation(dataFile, compositeOperationType);
    dcdcConverters->CheckOperationDataFormat(positionOfOperationHeader);
    delete dcdcConverters;
    std::cout << "-> Check on the DC-DC converters data format performed correctly.\n" << std::endl;
  }
  else if (!(strcmp(argv[2], "online_monitoring"))) {
    // Check online monitoring data
    std::cout << "-> Starting check on online monitoring data....." << std::endl;
    std::string compositeOperationType = "online_monitoring";
    CompositeOperation *onlineMonitoring = new CompositeOperation(dataFile, compositeOperationType);
    onlineMonitoring->CheckOperationDataFormat(positionOfOperationHeader);
    delete onlineMonitoring;
    std::cout << "-> Check on the online monitoring data format performed correctly.\n" << std::endl;
  }
  else {
    PrintWrongExpectedFormatErrorMessage();
    PrintListOfExpectedFormats();
    exit(EXIT_FAILURE);
  }
  delete compositeOperationParameters;

  return 0;
}

void CheckNumberOfParameters(unsigned int numberOfParameters, char* thisFile)
{
  if (numberOfParameters != 3) {
    PrintWrongNumberOfParametersErrorMessage();
    PrintCommandLineSyntax(thisFile);
    PrintListOfExpectedFormats();
    exit(EXIT_FAILURE);
  }
}

void PrintWrongNumberOfParametersErrorMessage()
{
  LeaveOneLineSpacing(); 
  std::cout << "Passed wrong number of command line parameters. Three parameters must be passed." << std::endl;
}

void PrintCommandLineSyntax(char* thisFile)
{
  LeaveOneLineSpacing(); 
  std::cout << "Accepted syntax: \"" << thisFile << " file_to_process expected_format.\". Example: \"" << thisFile << " filename.dat differential_amplifiers\"" << std::endl;
}

void PrintWrongExpectedFormatErrorMessage()
{
  LeaveOneLineSpacing(); 
  std::cout << "Passed wrong name for expected data format." << std::endl;
}

void PrintListOfExpectedFormats()
{
  LeaveOneLineSpacing(); 
  std::cout << "Pick up one of the expected formats from the following list:" << std::endl;
  LeaveOneLineSpacing(); 
  std::cout << "1)  differential_amplifiers"             << std::endl;
  std::cout << "2)  comparator"                          << std::endl;
  std::cout << "3)  dacs"                                << std::endl;
  std::cout << "4)  resistance_to_digital"               << std::endl;
  std::cout << "5)  io_expander"                         << std::endl;
  std::cout << "6)  adcs"                                << std::endl;
  std::cout << "7)  i2c_driver"                          << std::endl;
  std::cout << "8)  differential_to_single_ended_driver" << std::endl;
  std::cout << "9)  dcdc_converters"                     << std::endl;
  std::cout << "10) online_monitoring"                   << std::endl;
  LeaveOneLineSpacing(); 
}

void LeaveOneLineSpacing()
{
  std::cout << std::endl;
}

