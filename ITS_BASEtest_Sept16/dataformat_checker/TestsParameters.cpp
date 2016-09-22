//==========================================================================
// TestsParameters class - CPP file
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 25/8/2016
// Last modified: 25/8/2016
// Description: contains methods to collect data from the various test
//              procedures and plot them
//==========================================================================

#include "TestsParameters.h"

//===========================================================================
//                       Constructors and Destructor
//===========================================================================

Tests::TestsParameters(char* filename, std::string deviceType, std::string testType)
{
  Initialize(filename, deviceType, testType);
}

TestsParameters::~TestsParameters()
{
  Destroy();
}

//===========================================================================
//                            Inner methods
//===========================================================================

// Private Methods

void TestsParameters::Initialize(std::string deviceType, std::string testType)
{
  BuildStructures();
  FillStructures(deviceType, testType);
}

void TestsParameters::Destroy()
{ 
  for (int i = 0; i < 32; i++) {
    delete dataX[i];
  }
  delete dataX;
}

void TestsParameters::BuildStructures()
{
  dataX = new double*[32];
  for (int i = 0; i < 32; i++) {
    dataX[i] = new double[100];
  }
  
  xArrays = new Xarray*[20];
  for (int i = 0; i < 32; i++) {
    xArrays[i] = new xArray[30];
    xArrays[i]->data = new double[30];
  }

}

void TestsParameters::BuildAndFillStructuresForOnlineMonitoring(unsigned int dosePoints)
{
  onlineMonitoringArray = new double[dosePoints];
  
}

void TestsParameters::InitializeParameters()
{
  xArrays[0]->data[] = {}
  onlineMonitoringArray = {};
}

void TestsParameters::FillStructures(std::string deviceType, std::string testType)
{
  if (!IsValidTest(deviceType, testType){
    exit(EXIT_FAILURE); 
  }

  if (deviceType == "online_monitoring") return;

  std::string fullTestName = deviceType + "_" + testType;
  FillStructures(fullTestName);

  if (fullTestName == "differential_amplifiers_voltage_scan") 
    FillDataX(0);
  else if (fullTestName == "comparator_voltage_scan")
    FillDataX(1);
  else if (fullTestName == "dacs_voltage_scan")
    FillDataX(1);
  else if (fullTestName == "resistance_to_digital_resistance_scan")
    FillDataX(1);
  else if (fullTestName == "io_expander_status_check")
    FillDataX(1);
  else if (fullTestName == "adcs_voltage_scan")
    FillDataX(1);
  else if (fullTestName == "i2c_driver_transaction_failurerate")
    FillDataX(1);
  else if (fullTestName == "differential_to_single_ended_driver_biterror_rate")
    FillDataX(1);
  else if (fullTestName == "dcdc_converters_status0")
    FillDataX(1);
  else if (fullTestName == "dcdc_converters_status1")
    FillDataX(1);
  else if (fullTestName == "online_monitoring_dose_test")
    FillDataOnlineMonitoring();
  else {
    std::cout << "TestsParameters: Selected test type does not exist (\"" << testType << "\") for device " << deviceType << "\n" << std::endl; 
    exit(EXIT_FAILURE);
  }
}

bool TestParameters::IsValidTest(std::string deviceType, std::string testType)
{
  bool isValidDeviceType = false;
  bool isValidTestType = false;
  if (deviceType == "differential_amplifiers") {
    isValidDeviceType = true;
    if (testType == "voltage_scan")
      isValidTestType = true;
  }
  else if (deviceType == "comparator") {
    isValidDeviceType = true;
    if (testType == "voltage_scan")
      isValidTestType = true;
  }
  else if (deviceType == "dacs") {
    isValidDeviceType = true;
    if (testType == "voltage_scan")
      isValidTestType = true;
  }
  else if (deviceType == "resistance_to_digital") {
    isValidDeviceType = true;
    if (testType == "resistance_scan")
      isValidTestType = true;
  }
  else if (deviceType == "io_expander") {
    isValidDeviceType = true;
    if (testType == "status_check")
      isValidTestType = true;
  }
  else if (deviceType == "adcs") {
    isValidDeviceType = true;
    if (testType == "voltage_scan")
      isValidTestType = true;
  }
  else if (deviceType == "i2c_driver") {
    isValidDeviceType = true;
    if (testType == "transaction_failurerate")
      isValidTestType = true;
  }
  else if (deviceType == "differential_to_single_ended_driver") {
    isValidDeviceType = true;
    if (testType == "biterror_rate")
      isValidTestType = true;
  }
  else if (deviceType == "dcdc_converters") {
    isValidDeviceType = true;
    if (testType == "status0")
      isValidTestType = true;
    else if (testType == "status1")
      isValidTestType = true;
  }
  else if (deviceType == "online_monitoring"){
    isValidDeviceType = true;
    if (testType == "dose_test")
      isValidTestType = true;    
  }

  if (!isValidDeviceType) {
    std::cout << "TestsParameters: Provided non-valid device type (\"" << deviceType << "\").\n" << std::endl; 
    return false;
  }
  else if (!isValidTestType) {
    std::cout << "TestsParameters: Provided non-valid test type for device type " << deviceType << " (\"" << testType << "\").\n" << std::endl; 
    return false;
  }
  else {
    return true;
  }
}

void TestParameters::FillDataX(unsigned int arrayType)
{
  for (int i = 0; i < 32; i++) {
    for (int j = 0; j < xArrays[0]->depth; j++) {
      dataX[i][j] = xArrays[arrayType]->data[j];
    }
  }  
}
