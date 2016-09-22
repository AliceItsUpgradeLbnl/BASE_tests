//==========================================================================
// Data Analysis
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 25/8/2016
// Last modified: 25/8/2016
// Description: generates the plots for the selected test
//==========================================================================

#include "Tests.h"

void LeaveOneLineSpacing();
void CheckParameterNumber(unsigned int numberOfParameters, char* thisFile);
void PrintWrongNumberOfParametersErrorMessage();
void PrintCommandLineSyntax(char* thisFile);
void PrintWrongDeviceTypeMessage();
void PrintListOfTests();

int main(int argc, char* argv[])
{
  CheckParameterNumber(argc, argv[0]);

  char *dataFile = argv[1];

  Tests *tests;

  LeaveOneLineSpacing();

  if (!(strcmp(argv[2], "differential_amplifiers"))) {
    // Plot differential amplifiers data
    std::cout << "-> 1) Plotting differential amplifiers data (" << argv[3] << ") ....." << std::endl;
    std::string deviceType = "differential_amplifiers";
    std::string testName(argv[3]);
    tests = new Tests(dataFile, deviceType, testName);
    tests->StartTest();
    std::cout << "-> Test on the differential amplifiers data performed correctly.\n" << std::endl;
  }
  else if (!(strcmp(argv[2], "comparator"))) {
    // Plot comparator data
    std::cout << "-> 2) Plotting comparator data (" << argv[3] << ") ....." << std::endl;
    std::string deviceType = "comparator";
    std::string testName(argv[3]); 
    tests = new Tests(dataFile, deviceType, testName);
    tests->StartTest();
    std::cout << "-> Test on the comparator data performed correctly.\n" << std::endl;
  }
  else if (!(strcmp(argv[2], "dacs"))) {
    // Plot DACs data
    std::cout << "-> 3) Plotting DACs data (" << argv[3] << ") ....." << std::endl;
    std::string deviceType = "dacs";
    std::string testName(argv[3]); 
    tests = new Tests(dataFile, deviceType, testName);
    tests->StartTest();
    std::cout << "-> Test on the DACs data performed correctly.\n" << std::endl;
  }
  else if (!(strcmp(argv[2], "resistance_to_digital"))) {
    // Plot RTD data
    std::cout << "-> 4) Plotting RTD data (" << argv[3] << ") ....." << std::endl;
    std::string deviceType = "resistance_to_digital";
    std::string testName(argv[3]); 
    tests = new Tests(dataFile, deviceType, testName);
    tests->StartTest();
    std::cout << "-> Test on the RTD data performed correctly.\n" << std::endl;
  }
  else if (!(strcmp(argv[2], "io_expander"))) {
    // Plot Io expander data
    std::cout << "-> 5) Plotting IO expander data (" << argv[3] << ") ....." << std::endl;
    std::string deviceType = "io_expander";
    std::string testName(argv[3]); 
    tests = new Tests(dataFile, deviceType, testName);
    tests->StartTest();
    std::cout << "-> Test on the IO expander data performed correctly.\n" << std::endl;
  }
  else if (!(strcmp(argv[2], "adcs"))) {
    // Plot ADCs data
    std::cout << "-> 6) Plotting ADCs data (" << argv[3] << ") ....." << std::endl;
    std::string deviceType = "adcs";
    std::string testName(argv[3]); 
    tests = new Tests(dataFile, deviceType, testName);
    tests->StartTest();
    std::cout << "-> Test on the ADCs data performed correctly.\n" << std::endl;
  }
  else if (!(strcmp(argv[2], "i2c_driver"))) {
    // Plot I2C driver data
    std::cout << "-> 7) Plotting I2C driver data (" << argv[3] << ") ....." << std::endl;
    std::string deviceType = "i2c_driver";
    std::string testName(argv[3]); 
    tests = new Tests(dataFile, deviceType, testName);
    tests->StartTest();
    std::cout << "-> Test on the I2C driver data performed correctly.\n" << std::endl;
  }
  else if (!(strcmp(argv[2], "differential_to_single_ended_driver"))) {
    // Plot differential to single ended driver data
    std::cout << "-> 8) Plotting differential to single ended data (" << argv[3] << ") ....." << std::endl;
    std::string deviceType = "differential_to_single_ended_driver";
    std::string testName(argv[3]); 
    tests = new Tests(dataFile, deviceType, testName);
    tests->StartTest();
    std::cout << "-> Test on the differential to single ended data performed correctly.\n" << std::endl;
  }
  else if (!(strcmp(argv[2], "dcdc_converters"))) {
    // Plot DC-DC converters data
    std::cout << "-> 9) Plotting DC-DC converters data (" << argv[3] << ") ....." << std::endl;
    std::string deviceType = "dcdc_converters";
    std::string testName(argv[3]); 
    tests = new Tests(dataFile, deviceType, testName);
    tests->StartTest();
    std::cout << "-> Test on the DC-DC converters data performed correctly.\n" << std::endl;
  }
  else if (!(strcmp(argv[2], "online_monitoring"))) {
    // Plot online monitoring data
    std::cout << "-> 10) Plotting online monitoring data (" << argv[3] << ") ....." << std::endl;
    std::string deviceType = "online_monitoring";
    std::string testName(argv[3]); 
    tests = new Tests(dataFile, deviceType, testName);
    tests->StartTest();
    std::cout << "-> Test on the online monitoring data performed correctly.\n" << std::endl;
  }
  else {
    PrintWrongDeviceTypeMessage();
    PrintListOfTests();
    exit(EXIT_FAILURE);
  }
  delete tests;

  return 0;
}

void CheckParameterNumber(unsigned int numberOfParameters, char* thisFile)
{
  if (numberOfParameters != 4) {
    PrintWrongNumberOfParametersErrorMessage();
    PrintCommandLineSyntax(thisFile);
    PrintListOfTests();
    exit(EXIT_FAILURE);
  }
}

void PrintWrongNumberOfParametersErrorMessage()
{
  LeaveOneLineSpacing(); 
  std::cout << "Passed wrong number of command line parameters. Four parameters must be passed." << std::endl;
}

void PrintCommandLineSyntax(char* thisFile)
{
  LeaveOneLineSpacing(); 
  std::cout << "Accepted syntax: \"" << thisFile << " file_to_process device_type test_name\". Example: \"" << thisFile << " filename.dat differential_amplifiers voltage_scan\"." << std::endl;
}

void PrintWrongDeviceTypeMessage()
{
  LeaveOneLineSpacing(); 
  std::cout << "Passed wrong device type." << std::endl;
}

void PrintListOfTests()
{
  LeaveOneLineSpacing(); 
  std::cout << "Pick up one of the following device types and test names:" << std::endl;
  LeaveOneLineSpacing(); 
  std::cout << "1)  differential_amplifiers"             << std::endl;
  std::cout << "      a) voltage_scan"                   << std::endl;
  std::cout << "2)  comparator"                          << std::endl;
  std::cout << "      a) voltage_scan"                   << std::endl;
  std::cout << "3)  dacs"                                << std::endl;
  std::cout << "      a) voltage_scan"                   << std::endl;
  std::cout << "4)  resistance_to_digital"               << std::endl;
  std::cout << "      a) resistance_scan"                << std::endl;
  std::cout << "5)  io_expander"                         << std::endl;
  std::cout << "      a) status_check0"                  << std::endl;
  std::cout << "      b) status_check1"                  << std::endl;
  std::cout << "6)  adcs"                                << std::endl;
  std::cout << "      a) voltage_scan0"                  << std::endl;
  std::cout << "      b) voltage_scan1"                  << std::endl;
  std::cout << "      c) voltage_scan2"                  << std::endl;
  std::cout << "7)  i2c_driver"                          << std::endl;
  std::cout << "      a) transaction_failurerate"        << std::endl;
  std::cout << "8)  differential_to_single_ended_driver" << std::endl;
  std::cout << "      a) biterror_rate"                  << std::endl;
  std::cout << "9)  dcdc_converters"                     << std::endl;
  std::cout << "      a) status_check0"                  << std::endl;
  std::cout << "      b) status_check1"                  << std::endl;
  std::cout << "10) online_monitoring"                   << std::endl;
  std::cout << "      a) dose_test0"                     << std::endl;
  std::cout << "      a) dose_test1"                     << std::endl;
  std::cout << "      a) dose_test2"                     << std::endl;
  std::cout << "      a) dose_test3"                     << std::endl;
  LeaveOneLineSpacing(); 
}

void LeaveOneLineSpacing()
{
  std::cout << std::endl;
}
