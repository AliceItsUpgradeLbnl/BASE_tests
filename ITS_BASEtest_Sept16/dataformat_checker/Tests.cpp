//==========================================================================
// Tests class - CPP file
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 25/8/2016
// Last modified: 25/8/2016
// Description: contains methods to collect data from the various test
//              procedures and plot them
//==========================================================================

#include "Tests.h"

//===========================================================================
//                       Constructors and Destructor
//===========================================================================

Tests::Tests(char* filename, std::string deviceType, std::string testType)
{
  Initialize(filename, deviceType, testType);
}

Tests::~Tests()
{
  Destroy();
}

//===========================================================================
//                            Inner methods
//===========================================================================

// Public Methods

void Tests::StartTest()
{
  if (deviceType == "differential_amplifiers")
    StartTestDifferentialAmplifiers();
  else if (deviceType == "dacs")
    StartTestDacs();
  else if (deviceType == "dcdc_converters")
    StartTestDcdcConverters();
  else if (deviceType == "adcs")
    StartTestAdcs();
  else if (deviceType == "differential_to_single_ended_driver")
    StartTestDifferentialToSingleEndedDriver();
  else if (deviceType == "comparator")
    StartTestComparator();
  else if (deviceType == "resistance_to_digital")
    StartTestResistanceToDigital();
  else if (deviceType == "i2c_driver")
    StartTestI2cDriver();
  else if (deviceType == "io_expander")
    StartTestIoExpander();
  else if (deviceType == "online_monitoring")
    StartTestOnlineMonitoring();
  else { 
    std::cout << "Tests: Selected device type does not exist (\"" << deviceType << "\").\n" << std::endl; 
    exit(EXIT_FAILURE);
  }  
}

// Private Methods

void Tests::Initialize(char* filename, std::string deviceType, std::string testType)
{
  compositeOperation = new CompositeOperation(filename, deviceType);
  display = new Display(deviceType, testType);

  dataX = new double*[32];
  dataY = new double*[32];
  for (int i = 0; i < 32; i++) {
    dataX[i] = new double[500];
    dataY[i] = new double[500];
  }

  this->deviceType = deviceType;
  this->testType = testType;

}

void Tests::Destroy()
{
  for (int i = 0; i < 32; i++) {
    delete dataX[i];
    delete dataY[i];
  }
  delete dataX;
  delete dataY;
  
  delete compositeOperation;
  delete display;
}

void Tests::StartTestDifferentialAmplifiers()
{
  if (testType == "voltage_scan") 
    StartTestDifferentialAmplifiersVoltageScan();
  else {
    std::cout << "Tests: Selected test type does not exist (\"" << testType << "\") for device " << deviceType << "\n" << std::endl; 
    exit(EXIT_FAILURE);
  }
}

void Tests::StartTestDacs()
{
  if (testType == "voltage_scan") 
    StartTestDacsVoltageScan();
  else {
    std::cout << "Tests: Selected test type does not exist (\"" << testType << "\") for device " << deviceType << "\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Tests::StartTestDcdcConverters()
{
  if (testType == "status_check0") 
    StartTestDcdcConvertersStatusCheck0();
  else if (testType == "status_check1") 
    StartTestDcdcConvertersStatusCheck1();
  else {
    std::cout << "Tests: Selected test type does not exist (\"" << testType << "\") for device " << deviceType << "\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Tests::StartTestAdcs()
{
  if (testType == "voltage_scan0") 
    StartTestAdcsVoltageScan(0);
  else if (testType == "voltage_scan1") 
    StartTestAdcsVoltageScan(1);
  else if (testType == "voltage_scan2") 
    StartTestAdcsVoltageScan(2);
  else {
    std::cout << "Tests: Selected test type does not exist (\"" << testType << "\") for device " << deviceType << "\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Tests::StartTestComparator()
{
  if (testType == "voltage_scan") 
    StartTestComparatorVoltageScan();
  else {
    std::cout << "Tests: Selected test type does not exist (\"" << testType << "\") for device " << deviceType << "\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Tests::StartTestDifferentialToSingleEndedDriver()
{
  if (testType == "biterror_rate") 
    StartTestDifferentialToSingleEndedDriverBiterrorRate();
  else {
    std::cout << "Tests: Selected test type does not exist (\"" << testType << "\") for device " << deviceType << "\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Tests::StartTestResistanceToDigital()
{
  if (testType == "resistance_scan") 
    StartTestResistanceToDigitalResistanceScan();
  else {
    std::cout << "Tests: Selected test type does not exist (\"" << testType << "\") for device " << deviceType << "\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Tests::StartTestI2cDriver()
{
  if (testType == "transaction_failurerate") 
    StartTestI2cDriverTransactionFailureRate();
  else {
    std::cout << "Tests: Selected test type does not exist (\"" << testType << "\") for device " << deviceType << "\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Tests::StartTestIoExpander()
{
  if (testType == "status_check0") 
    StartTestIoExpanderStatusCheck0();
  else if (testType == "status_check1") 
    StartTestIoExpanderStatusCheck1();
  else {
    std::cout << "Tests: Selected test type does not exist (\"" << testType << "\") for device " << deviceType << "\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Tests::StartTestDifferentialAmplifiersVoltageScan()
{
  unsigned int numberOfChannels = 32;
  unsigned int numberOfPoints = 22;
  unsigned int channelsToRead = 0xE0;
  unsigned int headerPosition = 1;
  compositeOperation->GetOperationData(headerPosition);
  for (int i = 0; i < numberOfChannels; i++) {
    if ((channelsToRead >> i) & 0x00000001) {
      for (int j = 0; j < numberOfPoints; j++) {
        dataY[i - 5][j] = compositeOperation->adcData->mean[i]->front();
        compositeOperation->adcData->mean[i]->pop();
        dataX[i - 5][j] = (double) j;
//        std::cout << i - 5 << " " << j << " " << dataY[i - 5][j] << std::endl;
      }
    }
  }
  display->PlotMultiGraph(dataX, dataY);
}

void Tests::StartTestDacsVoltageScan()
{
  unsigned int numberOfChannels = 32;
  unsigned int numberOfPoints = 22;
  unsigned int channelsToRead = 0x0000FF00;
  unsigned int headerPosition = 1;
  compositeOperation->GetOperationData(headerPosition);
  for (int i = 0; i < numberOfChannels; i++) {
    if ((channelsToRead >> i) & 0x00000001) {
      for (int j = 0; j < numberOfPoints; j++) {
        dataY[i - 8][j] = compositeOperation->adcData->mean[i]->front();
        compositeOperation->adcData->mean[i]->pop();
        dataX[i - 8][j] = (double) j;
      }
    }
  }
  display->PlotMultiGraph(dataX, dataY);
}

void Tests::StartTestDcdcConvertersStatusCheck0()
{
  unsigned int numberOfChannels = 32;
  unsigned int numberOfPoints = 2;
  unsigned int channelsToRead = 0x07C00000;
  unsigned int headerPosition = 1;
  compositeOperation->GetOperationData(headerPosition);
  for (int i = 0; i < numberOfChannels; i++) {
    if ((channelsToRead >> i) & 0x00000001) {
      for (int j = 0; j < numberOfPoints; j++) {
        dataY[i - 22][j] = compositeOperation->adcData->mean[i]->front();
        compositeOperation->adcData->mean[i]->pop();
        dataX[i - 22][j] = (double) j;
      }
    }
  }
  display->PlotMultiGraph(dataX, dataY);
}

void Tests::StartTestDcdcConvertersStatusCheck1()
{
  unsigned int numberOfChannels = 32;
  unsigned int numberOfPoints = 2;
  unsigned int channelsToRead = 0xF8200000;
  unsigned int headerPosition = 1;
  unsigned int indexDcdcConverters = 0; 
  compositeOperation->GetOperationData(headerPosition);
  for (int i = 0; i < numberOfChannels; i++) {
    if ((channelsToRead >> i) & 0x00000001) {
      for (int j = 0; j < numberOfPoints; j++) {
        dataY[indexDcdcConverters][j] = compositeOperation->adcData->mean[i]->front();
        compositeOperation->adcData->mean[i]->pop();
        dataX[indexDcdcConverters][j] = (double) j;
      }
      indexDcdcConverters++;
    }
  }
  display->PlotMultiGraph(dataX, dataY);
}

void Tests::StartTestAdcsVoltageScan(unsigned int adcNumber)
{
  unsigned int numberOfChannels = 8;
  unsigned int numberOfPoints = 11;
  unsigned int headerPosition = 1;
  compositeOperation->GetOperationData(headerPosition);
  for (int i = 0; i < numberOfChannels; i++) {
    for (int j = 0; j < numberOfPoints; j++) {
      dataY[i][j] = compositeOperation->i2cData->adcs[adcNumber]->mean[i]->front();
      compositeOperation->i2cData->adcs[adcNumber]->mean[i]->pop();
      dataX[i][j] = (double) j;
    }
  }
  display->PlotMultiGraph(dataX, dataY);
}

void Tests::StartTestDifferentialToSingleEndedDriverBiterrorRate()
{
  unsigned int numberOfChannels = 8;
  unsigned int numberOfPoints = 32;
  unsigned int headerPosition = 1;
  compositeOperation->GetOperationData(headerPosition);
  for (int i = 0; i < numberOfChannels; i++) {
    for (int j = 0; j < numberOfPoints; j++) {
      dataY[i][j] = compositeOperation->i2cData->differentialToSingleEndedDriver[i]->front();
      compositeOperation->i2cData->differentialToSingleEndedDriver[i]->pop();
      dataX[i][j] = (double) j;
    }
  }
  display->PlotMultiGraph(dataX, dataY);
}

void Tests::StartTestComparatorVoltageScan()
{
  unsigned int numberOfPoints = 22;
  unsigned int headerPosition = 1;
  compositeOperation->GetOperationData(headerPosition);
  for (int j = 0; j < numberOfPoints; j++) {
    dataY[0][j] = (double) compositeOperation->comparatorData->output->front();
    compositeOperation->comparatorData->output->pop();
    dataY[1][j] = (double) compositeOperation->comparatorData->outputBar->front();
    compositeOperation->comparatorData->outputBar->pop();
    dataX[0][j] = (double) j;
    dataX[1][j] = (double) j;
  }
  display->PlotMultiGraph(dataX, dataY);
}

void Tests::StartTestResistanceToDigitalResistanceScan()
{
  unsigned int numberOfPoints = 21;
  unsigned int headerPosition = 1;
  compositeOperation->GetOperationData(headerPosition);
  for (int j = 0; j < numberOfPoints; j++) {
    dataY[0][j] = (double) compositeOperation->i2cData->resistanceToDigital->mean->front();
    compositeOperation->i2cData->resistanceToDigital->mean->pop();
    dataX[0][j] = (double) j;
  }
  display->PlotMultiGraph(dataX, dataY);
}

void Tests::StartTestI2cDriverTransactionFailureRate()
{
  unsigned int numberOfPoints = 1;
  unsigned int headerPosition = 1;
  compositeOperation->GetOperationData(headerPosition);
  for (int j = 0; j < numberOfPoints; j++) {
    dataY[0][j] = (double) compositeOperation->i2cData->i2cDriver->front();
    compositeOperation->i2cData->i2cDriver->pop();
    dataX[0][j] = (double) j;
  }
  display->PlotMultiGraph(dataX, dataY);
}

void Tests::StartTestOnlineMonitoring()
{
  unsigned int channelsToPlot;
  if (testType == "dose_test0")
    channelsToPlot = 0x000000E0;
  else if (testType == "dose_test1")
    channelsToPlot = 0x0000FF00;
  else if (testType == "dose_test2")
    channelsToPlot = 0x07C00000;
  else if (testType == "dose_test3")
    channelsToPlot = 0xF8200000;
  else {
    std::cout << "Tests: Selected test type does not exist (\"" << testType << "\") for device " << deviceType << "\n" << std::endl;
    exit(EXIT_FAILURE);
  }

  unsigned int indexPoints = 0;
  unsigned int indexChannels = 0; 
  unsigned int numberOfAdcChannels = 32;
  unsigned int headerPosition = 1;
  compositeOperation->GetOperationData(headerPosition);
  unsigned int numberOfRuns = compositeOperation->onlineData->numberOfRuns;
  unsigned int numberOfPointsOfRun = 0;
  for (int i = 0; i < numberOfRuns; i++) {
    numberOfPointsOfRun = compositeOperation->onlineData->numberOfPointsPerRun->front();
    compositeOperation->onlineData->numberOfPointsPerRun->pop();
    for (int j = 0; j < numberOfPointsOfRun; j++) {
      indexChannels = 0;
      for (int k = 0; k < numberOfAdcChannels; k++) {
        if ((channelsToPlot >> k) & 0x00000001) {
          dataY[indexChannels][indexPoints] = compositeOperation->adcData->mean[k]->front();
          compositeOperation->adcData->mean[k]->pop();
          dataX[indexChannels][indexPoints] = (double) indexPoints;
          indexChannels++;
        }
      }
      indexPoints++;
    }
  }
  display->SetOnlineMonitoringNumberOfPoints(indexPoints);
  display->PlotMultiGraph(dataX, dataY);
}

void Tests::StartTestIoExpanderStatusCheck0()
{
  unsigned int numberOfPoints = 40;
  unsigned int headerPosition = 1;
  compositeOperation->GetOperationData(headerPosition);
  for (int j = 0; j < numberOfPoints; j++) {
    dataY[0][j] = (double) compositeOperation->i2cData->ioExpander->initial_state[0]->front();
    compositeOperation->i2cData->ioExpander->initial_state[0]->pop();
    dataX[0][j] = (double) j;
  }
  display->PlotMultiGraph(dataX, dataY);
}

void Tests::StartTestIoExpanderStatusCheck1()
{
  unsigned int numberOfChannels = 16;
  unsigned int headerPosition = 1;
  compositeOperation->GetOperationData(headerPosition);
  for (int i = 0; i < numberOfChannels; i++) {
    dataY[0][i] = (double) compositeOperation->i2cData->ioExpander->transition_occurred[0]->front();
    compositeOperation->i2cData->ioExpander->transition_occurred[0]->pop();
    dataX[0][i] = (double) i;
  }
  display->PlotMultiGraph(dataX, dataY);
}

