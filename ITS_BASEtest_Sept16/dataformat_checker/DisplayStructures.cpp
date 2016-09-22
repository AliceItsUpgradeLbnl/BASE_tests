//==========================================================================
// Tests structures class - CPP file 
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 5/16/2016
// Last modified: 5/16/2016
//==========================================================================

#include "DisplayStructures.h"

//=======================================================================
//                       Constructors and destructor
//=======================================================================

DisplayStructures::DisplayStructures(std::string deviceType, std::string testType)
{
  Initialize(deviceType, testType);
}

DisplayStructures::~DisplayStructures()
{
  Destroy();
}

//=======================================================================
//                                Methods
//=======================================================================

// private 

void DisplayStructures::Initialize(std::string deviceType, std::string testType)
{
  if (deviceType == "differential_amplifiers") {
    if (testType == "voltage_scan") {
      numberOfGraphsToPlot = 3;
      numberOfPointsPerGraph = 22;
      BuildDeviceStructures(numberOfGraphsToPlot);
      InitializeDifferentialAmplifiers(testType);
    }
    else {
      std::cout << "DisplayStructures: Unknown test type provided while initializing Object (\"" << testType << "\").\n" << std::endl;
      exit(EXIT_FAILURE);
    }       
  }
  else if (deviceType == "dacs") {
    if (testType == "voltage_scan") {
      numberOfGraphsToPlot = 8;
      numberOfPointsPerGraph = 22;
      BuildDeviceStructures(numberOfGraphsToPlot);
      InitializeDacs(testType);
    }
    else {
      std::cout << "DisplayStructures: Unknown test type provided while initializing Object (\"" << testType << "\").\n" << std::endl;
      exit(EXIT_FAILURE);
    }       
  }
  else if (deviceType == "dcdc_converters") {
    if (testType == "status_check0") {
      numberOfGraphsToPlot = 5;
      numberOfPointsPerGraph = 2;
      BuildDeviceStructures(numberOfGraphsToPlot);
      InitializeDcdcConverters(testType);
    }
    else if (testType == "status_check1") {
      numberOfGraphsToPlot = 6;
      numberOfPointsPerGraph = 2;
      BuildDeviceStructures(numberOfGraphsToPlot);
      InitializeDcdcConverters(testType);
    }
    else {
      std::cout << "DisplayStructures: Unknown test type provided while initializing Object (\"" << testType << "\").\n" << std::endl;
      exit(EXIT_FAILURE);
    }  
  }
  else if (deviceType == "adcs") {
    if (testType == "voltage_scan0") {
      numberOfGraphsToPlot = 8;
      numberOfPointsPerGraph = 11;
      BuildDeviceStructures(numberOfGraphsToPlot);
      InitializeAdcs(testType);
    }
    else if (testType == "voltage_scan1") {
      numberOfGraphsToPlot = 8;
      numberOfPointsPerGraph = 11;
      BuildDeviceStructures(numberOfGraphsToPlot);
      InitializeAdcs(testType);
    }
    else if (testType == "voltage_scan2") {
      numberOfGraphsToPlot = 8;
      numberOfPointsPerGraph = 11;
      BuildDeviceStructures(numberOfGraphsToPlot);
      InitializeAdcs(testType);
    }
    else {
      std::cout << "DisplayStructures: Unknown test type provided while initializing Object (\"" << testType << "\").\n" << std::endl;
      exit(EXIT_FAILURE);
    }       
  }
  else if (deviceType == "differential_to_single_ended_driver") {
    if (testType == "biterror_rate") {
      numberOfGraphsToPlot = 8;
      numberOfPointsPerGraph = 32;
      BuildDeviceStructures(numberOfGraphsToPlot);
      InitializeDifferentialToSingleEndedDriver(testType);
    }
    else {
      std::cout << "DisplayStructures: Unknown test type provided while initializing Object (\"" << testType << "\").\n" << std::endl;
      exit(EXIT_FAILURE);
    }       
  }
  else if (deviceType == "comparator") {
    if (testType == "voltage_scan") {
      numberOfGraphsToPlot = 2;
      numberOfPointsPerGraph = 22;
      BuildDeviceStructures(numberOfGraphsToPlot);
      InitializeComparator(testType);
    }
    else {
      std::cout << "DisplayStructures: Unknown test type provided while initializing Object (\"" << testType << "\").\n" << std::endl;
      exit(EXIT_FAILURE);
    }       
  }
  else if (deviceType == "resistance_to_digital") {
    if (testType == "resistance_scan") {
      numberOfGraphsToPlot = 1;
      numberOfPointsPerGraph = 21;
      BuildDeviceStructures(numberOfGraphsToPlot);
      InitializeResistanceToDigital(testType);
    }
    else {
      std::cout << "DisplayStructures: Unknown test type provided while initializing Object (\"" << testType << "\").\n" << std::endl;
      exit(EXIT_FAILURE);
    }       
  }
  else if (deviceType == "i2c_driver") {
    if (testType == "transaction_failurerate") {
      numberOfGraphsToPlot = 1;
      numberOfPointsPerGraph = 1;
      BuildDeviceStructures(numberOfGraphsToPlot);
      InitializeI2cDriver(testType);
    }
    else {
      std::cout << "DisplayStructures: Unknown test type provided while initializing Object (\"" << testType << "\").\n" << std::endl;
      exit(EXIT_FAILURE);
    }       
  }
  else if (deviceType == "io_expander") {
    if (testType == "status_check0") {
      numberOfGraphsToPlot = 1;
      numberOfPointsPerGraph = 40; 
      BuildDeviceStructures(numberOfGraphsToPlot);
      InitializeIoExpander(testType);
    }
    else if (testType == "status_check1") {
      numberOfGraphsToPlot = 1;
      numberOfPointsPerGraph = 16; 
      BuildDeviceStructures(numberOfGraphsToPlot);
      InitializeIoExpander(testType);
    }
    else {
      std::cout << "DisplayStructures: Unknown test type provided while initializing Object (\"" << testType << "\").\n" << std::endl;
      exit(EXIT_FAILURE);
    } 
  }   
  else if (deviceType == "online_monitoring") {
    if (testType == "dose_test0") {
      numberOfGraphsToPlot = 3;
      numberOfPointsPerGraph = 0; // Initialized to zero but it will be set afterwards in this case
      BuildDeviceStructures(numberOfGraphsToPlot);
      InitializeOnlineMonitoring(testType);
    }
    else if (testType == "dose_test1") {
      numberOfGraphsToPlot = 8;
      numberOfPointsPerGraph = 0;
      BuildDeviceStructures(numberOfGraphsToPlot);
      InitializeOnlineMonitoring(testType);
    }
    else if (testType == "dose_test2") {
      numberOfGraphsToPlot = 5;
      numberOfPointsPerGraph = 0;
      BuildDeviceStructures(numberOfGraphsToPlot);
      InitializeOnlineMonitoring(testType);
    }
    else if (testType == "dose_test3") {
      numberOfGraphsToPlot = 6;
      numberOfPointsPerGraph = 0;
      BuildDeviceStructures(numberOfGraphsToPlot);
      InitializeOnlineMonitoring(testType);
    }
    else {
      std::cout << "DisplayStructures: Unknown test type provided while initializing Object (\"" << testType << "\").\n" << std::endl;
      exit(EXIT_FAILURE);
    }       
  }
  else {
    std::cout << "DisplayStructures: Unknown device type provided while initializing Object (\"" << deviceType << "\").\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void DisplayStructures::Destroy()
{
  for (int i = 0; i < numberOfGraphsToPlot; i++) {
    delete graphSettings[i]->legend;
    delete graphSettings[i]->xAxis;
    delete graphSettings[i]->yAxis; 
  }
  delete graphSettings;
}

void DisplayStructures::SetOnlineMonitoringNumberOfPoints(unsigned int numberOfPoints)
{
  numberOfPointsPerGraph = numberOfPoints;
}

void DisplayStructures::BuildDeviceStructures(unsigned int width)
{
  graphSettings = new GraphSettings*[width];

  for (int i = 0; i < width; i++) {
    graphSettings[i] = new GraphSettings;
    graphSettings[i]->legend = new Legend;
    graphSettings[i]->xAxis = new Axis;
    graphSettings[i]->yAxis = new Axis;
  }
}

void DisplayStructures::InitializeDifferentialAmplifiers(std::string testType)
{
  if (testType == "voltage_scan") {
    // Vout vs Vin plots
    graphSettings[0]->graphName = strdup("MAX4199ESA_VoutvsVin.eps");
    graphSettings[1]->graphName = strdup("INA225_VoutvsVin.eps");
    graphSettings[2]->graphName = strdup("AD8418A_VoutvsVin.eps");
    graphSettings[0]->xAxis->label = strdup("Input voltage [V]");
    graphSettings[1]->xAxis->label = strdup("Input voltage [V]");
    graphSettings[2]->xAxis->label = strdup("Input voltage [V]");
    graphSettings[0]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[1]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[2]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[0]->legend->entryName = strdup("MAX4199ESA");
    graphSettings[1]->legend->entryName = strdup("INA225");
    graphSettings[2]->legend->entryName = strdup("AD8418A");
    graphSettings[0]->yAxis->maximum = 3.4;
    graphSettings[1]->yAxis->maximum = 3.4;
    graphSettings[2]->yAxis->maximum = 3.4;
    graphSettings[0]->legend->coordinates[0] = 0.35;
    graphSettings[0]->legend->coordinates[1] = 0.9;
    graphSettings[0]->legend->coordinates[2] = 0.6;
    graphSettings[0]->legend->coordinates[3] = 0.70;
    graphSettings[1]->legend->coordinates[0] = 0.35;
    graphSettings[1]->legend->coordinates[1] = 0.9;
    graphSettings[1]->legend->coordinates[2] = 0.6;
    graphSettings[1]->legend->coordinates[3] = 0.70;
    graphSettings[2]->legend->coordinates[0] = 0.35;
    graphSettings[2]->legend->coordinates[1] = 0.9;
    graphSettings[2]->legend->coordinates[2] = 0.6;
    graphSettings[2]->legend->coordinates[3] = 0.70;
  }
  else {
    std::cout << "DisplayStructures: Unknown test type provided while filling data structures for differential amplifiers (\"" << testType << "\").\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void DisplayStructures::InitializeDacs(std::string testType)
{
  if (testType == "voltage_scan") {
    // Vout vs Vin plots
    graphSettings[0]->graphName = strdup("ADC5671R_CH7_VoutvsDacin.eps");
    graphSettings[1]->graphName = strdup("ADC5671R_CH6_VoutvsDacin.eps");
    graphSettings[2]->graphName = strdup("ADC5671R_CH5_VoutvsDacin.eps");
    graphSettings[3]->graphName = strdup("ADC5671R_CH4_VoutvsDacin.eps");
    graphSettings[4]->graphName = strdup("ADC5671R_CH3_VoutvsDacin.eps");
    graphSettings[5]->graphName = strdup("ADC5671R_CH2_VoutvsDacin.eps");
    graphSettings[6]->graphName = strdup("ADC5671R_CH1_VoutvsDacin.eps");
    graphSettings[7]->graphName = strdup("ADC5671R_CH0_VoutvsDacin.eps");
    graphSettings[0]->xAxis->label = strdup("Dac value");
    graphSettings[1]->xAxis->label = strdup("Dac value");
    graphSettings[2]->xAxis->label = strdup("Dac value");
    graphSettings[3]->xAxis->label = strdup("Dac value");
    graphSettings[4]->xAxis->label = strdup("Dac value");
    graphSettings[5]->xAxis->label = strdup("Dac value");
    graphSettings[6]->xAxis->label = strdup("Dac value");
    graphSettings[7]->xAxis->label = strdup("Dac value");
    graphSettings[0]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[1]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[2]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[3]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[4]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[5]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[6]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[7]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[0]->legend->entryName = strdup("ADC5671R - Channel 7");
    graphSettings[1]->legend->entryName = strdup("ADC5671R - Channel 6");
    graphSettings[2]->legend->entryName = strdup("ADC5671R - Channel 5");
    graphSettings[3]->legend->entryName = strdup("ADC5671R - Channel 4");
    graphSettings[4]->legend->entryName = strdup("ADC5671R - Channel 3");
    graphSettings[5]->legend->entryName = strdup("ADC5671R - Channel 2");
    graphSettings[6]->legend->entryName = strdup("ADC5671R - Channel 1");
    graphSettings[7]->legend->entryName = strdup("ADC5671R - Channel 0");
    graphSettings[0]->yAxis->maximum = 3.4;
    graphSettings[1]->yAxis->maximum = 3.4;
    graphSettings[2]->yAxis->maximum = 3.4;
    graphSettings[3]->yAxis->maximum = 3.4;
    graphSettings[4]->yAxis->maximum = 3.4;
    graphSettings[5]->yAxis->maximum = 3.4;
    graphSettings[6]->yAxis->maximum = 3.4;
    graphSettings[7]->yAxis->maximum = 3.4;
    graphSettings[0]->legend->coordinates[0] = 0.35;
    graphSettings[0]->legend->coordinates[1] = 0.9;
    graphSettings[0]->legend->coordinates[2] = 0.6;
    graphSettings[0]->legend->coordinates[3] = 0.70;
    graphSettings[1]->legend->coordinates[0] = 0.35;
    graphSettings[1]->legend->coordinates[1] = 0.9;
    graphSettings[1]->legend->coordinates[2] = 0.6;
    graphSettings[1]->legend->coordinates[3] = 0.70;
    graphSettings[2]->legend->coordinates[0] = 0.35;
    graphSettings[2]->legend->coordinates[1] = 0.9;
    graphSettings[2]->legend->coordinates[2] = 0.6;
    graphSettings[2]->legend->coordinates[3] = 0.70;
    graphSettings[3]->legend->coordinates[0] = 0.35;
    graphSettings[3]->legend->coordinates[1] = 0.9;
    graphSettings[3]->legend->coordinates[2] = 0.6;
    graphSettings[3]->legend->coordinates[3] = 0.70;
    graphSettings[4]->legend->coordinates[0] = 0.35;
    graphSettings[4]->legend->coordinates[1] = 0.9;
    graphSettings[4]->legend->coordinates[2] = 0.6;
    graphSettings[4]->legend->coordinates[3] = 0.70;
    graphSettings[5]->legend->coordinates[0] = 0.35;
    graphSettings[5]->legend->coordinates[1] = 0.9;
    graphSettings[5]->legend->coordinates[2] = 0.6;
    graphSettings[5]->legend->coordinates[3] = 0.70;
    graphSettings[6]->legend->coordinates[0] = 0.35;
    graphSettings[6]->legend->coordinates[1] = 0.9;
    graphSettings[6]->legend->coordinates[2] = 0.6;
    graphSettings[6]->legend->coordinates[3] = 0.70;
    graphSettings[7]->legend->coordinates[0] = 0.35;
    graphSettings[7]->legend->coordinates[1] = 0.9;
    graphSettings[7]->legend->coordinates[2] = 0.6;
    graphSettings[7]->legend->coordinates[3] = 0.70;
  }
  else {
    std::cout << "DisplayStructures: Unknown test type provided while filling data structures for DACs (\"" << testType << "\").\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void DisplayStructures::InitializeDcdcConverters(std::string testType)
{
  if (testType == "status_check0") {
    // Status plots for first 5 DC-DC converters
    graphSettings[0]->graphName = strdup("FPF1007_Vout.eps");
    graphSettings[1]->graphName = strdup("MIC29371-3.3WU_Vout.eps");
    graphSettings[2]->graphName = strdup("MAX15053EWL_Vout.eps");
    graphSettings[3]->graphName = strdup("NTGD1100LT1G_DEV0_Vout.eps");
    graphSettings[4]->graphName = strdup("NTGD1100LT1G_DEV1_Vout.eps");
    graphSettings[0]->xAxis->label = strdup("Enable Status (Dis - En)");
    graphSettings[1]->xAxis->label = strdup("Enable Status (Dis - En)");
    graphSettings[2]->xAxis->label = strdup("Enable Status (Dis - En)");
    graphSettings[3]->xAxis->label = strdup("Enable Status (Dis - En)");
    graphSettings[4]->xAxis->label = strdup("Enable Status (Dis - En)");
    graphSettings[0]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[1]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[2]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[3]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[4]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[0]->legend->entryName = strdup("FPF1007");
    graphSettings[1]->legend->entryName = strdup("MIC29371-3.3WU");
    graphSettings[2]->legend->entryName = strdup("MAX15053EWL");
    graphSettings[3]->legend->entryName = strdup("NTGD1100LT1G - Device 0");
    graphSettings[4]->legend->entryName = strdup("NTGD1100LT1G - Device 1");
    graphSettings[0]->yAxis->maximum = 5;
    graphSettings[1]->yAxis->maximum = 5;
    graphSettings[2]->yAxis->maximum = 5;
    graphSettings[3]->yAxis->maximum = 5;
    graphSettings[4]->yAxis->maximum = 5;
    graphSettings[0]->legend->coordinates[0] = 0.35;
    graphSettings[0]->legend->coordinates[1] = 0.9;
    graphSettings[0]->legend->coordinates[2] = 0.6;
    graphSettings[0]->legend->coordinates[3] = 0.70;
    graphSettings[1]->legend->coordinates[0] = 0.35;
    graphSettings[1]->legend->coordinates[1] = 0.9;
    graphSettings[1]->legend->coordinates[2] = 0.6;
    graphSettings[1]->legend->coordinates[3] = 0.70;
    graphSettings[2]->legend->coordinates[0] = 0.35;
    graphSettings[2]->legend->coordinates[1] = 0.9;
    graphSettings[2]->legend->coordinates[2] = 0.6;
    graphSettings[2]->legend->coordinates[3] = 0.70;
    graphSettings[3]->legend->coordinates[0] = 0.35;
    graphSettings[3]->legend->coordinates[1] = 0.9;
    graphSettings[3]->legend->coordinates[2] = 0.6;
    graphSettings[3]->legend->coordinates[3] = 0.70;
    graphSettings[4]->legend->coordinates[0] = 0.35;
    graphSettings[4]->legend->coordinates[1] = 0.9;
    graphSettings[4]->legend->coordinates[2] = 0.6;
    graphSettings[4]->legend->coordinates[3] = 0.70;
  }
  else if (testType == "status_check1") {
    // Status plots for second 6 DC-DC converters
    graphSettings[0]->graphName = strdup("SI3865DDV_DEV1_Vout.eps");
    graphSettings[1]->graphName = strdup("SI3865DDV_DEV0_Vout.eps");
    graphSettings[2]->graphName = strdup("ADP7159ARDZ_DEV1_Vout.eps");
    graphSettings[3]->graphName = strdup("MIC39152WD_DEV1_Vout.eps");
    graphSettings[4]->graphName = strdup("MIC39152WD_DEV0_Vout.eps");
    graphSettings[5]->graphName = strdup("ADP7159ARDZ_DEV0_Vout.eps");
    graphSettings[0]->xAxis->label = strdup("Enable Status (Dis - En)");
    graphSettings[1]->xAxis->label = strdup("Enable Status (Dis - En)");
    graphSettings[2]->xAxis->label = strdup("Enable Status (Dis - En)");
    graphSettings[3]->xAxis->label = strdup("Enable Status (Dis - En)");
    graphSettings[4]->xAxis->label = strdup("Enable Status (Dis - En)");
    graphSettings[5]->xAxis->label = strdup("Enable Status (Dis - En)");
    graphSettings[0]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[1]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[2]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[3]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[4]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[5]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[0]->legend->entryName = strdup("SI3865DDV - Device 1");
    graphSettings[1]->legend->entryName = strdup("SI3865DDV - Device 0");
    graphSettings[2]->legend->entryName = strdup("ADP7159ARDZ - Device 1");
    graphSettings[3]->legend->entryName = strdup("MIC39152WD - Device 1");
    graphSettings[4]->legend->entryName = strdup("MIC39152WD - Device 0");
    graphSettings[5]->legend->entryName = strdup("ADP7159ARDZ - Device 0");
    graphSettings[0]->yAxis->maximum = 5;
    graphSettings[1]->yAxis->maximum = 5;
    graphSettings[2]->yAxis->maximum = 5;
    graphSettings[3]->yAxis->maximum = 5;
    graphSettings[4]->yAxis->maximum = 5;
    graphSettings[5]->yAxis->maximum = 5;
    graphSettings[0]->legend->coordinates[0] = 0.35;
    graphSettings[0]->legend->coordinates[1] = 0.9;
    graphSettings[0]->legend->coordinates[2] = 0.6;
    graphSettings[0]->legend->coordinates[3] = 0.70;
    graphSettings[1]->legend->coordinates[0] = 0.35;
    graphSettings[1]->legend->coordinates[1] = 0.9;
    graphSettings[1]->legend->coordinates[2] = 0.6;
    graphSettings[1]->legend->coordinates[3] = 0.70;
    graphSettings[2]->legend->coordinates[0] = 0.35;
    graphSettings[2]->legend->coordinates[1] = 0.9;
    graphSettings[2]->legend->coordinates[2] = 0.6;
    graphSettings[2]->legend->coordinates[3] = 0.70;
    graphSettings[3]->legend->coordinates[0] = 0.35;
    graphSettings[3]->legend->coordinates[1] = 0.9;
    graphSettings[3]->legend->coordinates[2] = 0.6;
    graphSettings[3]->legend->coordinates[3] = 0.70;
    graphSettings[4]->legend->coordinates[0] = 0.35;
    graphSettings[4]->legend->coordinates[1] = 0.9;
    graphSettings[4]->legend->coordinates[2] = 0.6;
    graphSettings[4]->legend->coordinates[3] = 0.70;
    graphSettings[5]->legend->coordinates[0] = 0.35;
    graphSettings[5]->legend->coordinates[1] = 0.9;
    graphSettings[5]->legend->coordinates[2] = 0.6;
    graphSettings[5]->legend->coordinates[3] = 0.70;
  }
  else {
    std::cout << "DisplayStructures: Unknown test type provided while filling data structures for device (\"" << testType << "\").\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void DisplayStructures::InitializeAdcs(std::string testType)
{
  if (testType == "voltage_scan0") {
    // Vout vs Vin plots for ADS7830
    graphSettings[0]->graphName = strdup("ADS7830_CH0_VoutvsVin.eps");
    graphSettings[1]->graphName = strdup("ADS7830_CH1_VoutvsVin.eps");
    graphSettings[2]->graphName = strdup("ADS7830_CH2_VoutvsVin.eps");
    graphSettings[3]->graphName = strdup("ADS7830_CH3_VoutvsVin.eps");
    graphSettings[4]->graphName = strdup("ADS7830_CH4_VoutvsVin.eps");
    graphSettings[5]->graphName = strdup("ADS7830_CH5_VoutvsVin.eps");
    graphSettings[6]->graphName = strdup("ADS7830_CH6_VoutvsVin.eps");
    graphSettings[7]->graphName = strdup("ADS7830_CH7_VoutvsVin.eps");
    graphSettings[0]->xAxis->label = strdup("Input Voltage [V]");
    graphSettings[1]->xAxis->label = strdup("Input Voltage [V]");
    graphSettings[2]->xAxis->label = strdup("Input Voltage [V]");
    graphSettings[3]->xAxis->label = strdup("Input Voltage [V]");
    graphSettings[4]->xAxis->label = strdup("Input Voltage [V]");
    graphSettings[5]->xAxis->label = strdup("Input Voltage [V]");
    graphSettings[6]->xAxis->label = strdup("Input Voltage [V]");
    graphSettings[7]->xAxis->label = strdup("Input Voltage [V]");
    graphSettings[0]->yAxis->label = strdup("Measured voltage [V]");
    graphSettings[1]->yAxis->label = strdup("Measured voltage [V]");
    graphSettings[2]->yAxis->label = strdup("Measured voltage [V]");
    graphSettings[3]->yAxis->label = strdup("Measured voltage [V]");
    graphSettings[4]->yAxis->label = strdup("Measured voltage [V]");
    graphSettings[5]->yAxis->label = strdup("Measured voltage [V]");
    graphSettings[6]->yAxis->label = strdup("Measured voltage [V]");
    graphSettings[7]->yAxis->label = strdup("Measured voltage [V]");
    graphSettings[0]->legend->entryName = strdup("ADS7830 - Channel 0");
    graphSettings[1]->legend->entryName = strdup("ADS7830 - Channel 1");
    graphSettings[2]->legend->entryName = strdup("ADS7830 - Channel 2");
    graphSettings[3]->legend->entryName = strdup("ADS7830 - Channel 3");
    graphSettings[4]->legend->entryName = strdup("ADS7830 - Channel 4");
    graphSettings[5]->legend->entryName = strdup("ADS7830 - Channel 5");
    graphSettings[6]->legend->entryName = strdup("ADS7830 - Channel 6");
    graphSettings[7]->legend->entryName = strdup("ADS7830 - Channel 7");
    graphSettings[0]->yAxis->maximum = 3.4;
    graphSettings[1]->yAxis->maximum = 3.4;
    graphSettings[2]->yAxis->maximum = 3.4;
    graphSettings[3]->yAxis->maximum = 3.4;
    graphSettings[4]->yAxis->maximum = 3.4;
    graphSettings[5]->yAxis->maximum = 3.4;
    graphSettings[6]->yAxis->maximum = 3.4;
    graphSettings[7]->yAxis->maximum = 3.4;
    graphSettings[0]->legend->coordinates[0] = 0.35;
    graphSettings[0]->legend->coordinates[1] = 0.9;
    graphSettings[0]->legend->coordinates[2] = 0.6;
    graphSettings[0]->legend->coordinates[3] = 0.70;
    graphSettings[1]->legend->coordinates[0] = 0.35;
    graphSettings[1]->legend->coordinates[1] = 0.9;
    graphSettings[1]->legend->coordinates[2] = 0.6;
    graphSettings[1]->legend->coordinates[3] = 0.70;
    graphSettings[2]->legend->coordinates[0] = 0.35;
    graphSettings[2]->legend->coordinates[1] = 0.9;
    graphSettings[2]->legend->coordinates[2] = 0.6;
    graphSettings[2]->legend->coordinates[3] = 0.70;
    graphSettings[3]->legend->coordinates[0] = 0.35;
    graphSettings[3]->legend->coordinates[1] = 0.9;
    graphSettings[3]->legend->coordinates[2] = 0.6;
    graphSettings[3]->legend->coordinates[3] = 0.70;
    graphSettings[4]->legend->coordinates[0] = 0.35;
    graphSettings[4]->legend->coordinates[1] = 0.9;
    graphSettings[4]->legend->coordinates[2] = 0.6;
    graphSettings[4]->legend->coordinates[3] = 0.70;
    graphSettings[5]->legend->coordinates[0] = 0.35;
    graphSettings[5]->legend->coordinates[1] = 0.9;
    graphSettings[5]->legend->coordinates[2] = 0.6;
    graphSettings[5]->legend->coordinates[3] = 0.70;
    graphSettings[6]->legend->coordinates[0] = 0.35;
    graphSettings[6]->legend->coordinates[1] = 0.9;
    graphSettings[6]->legend->coordinates[2] = 0.6;
    graphSettings[6]->legend->coordinates[3] = 0.70;
    graphSettings[7]->legend->coordinates[0] = 0.35;
    graphSettings[7]->legend->coordinates[1] = 0.9;
    graphSettings[7]->legend->coordinates[2] = 0.6;
    graphSettings[7]->legend->coordinates[3] = 0.70;
  }
  else if (testType == "voltage_scan1") {
    // Vout vs Vin plots for ADC128D818
    graphSettings[0]->graphName = strdup("ADC128D818_CH0_VoutvsVin.eps");
    graphSettings[1]->graphName = strdup("ADC128D818_CH1_VoutvsVin.eps");
    graphSettings[2]->graphName = strdup("ADC128D818_CH2_VoutvsVin.eps");
    graphSettings[3]->graphName = strdup("ADC128D818_CH3_VoutvsVin.eps");
    graphSettings[4]->graphName = strdup("ADC128D818_CH4_VoutvsVin.eps");
    graphSettings[5]->graphName = strdup("ADC128D818_CH5_VoutvsVin.eps");
    graphSettings[6]->graphName = strdup("ADC128D818_CH6_VoutvsVin.eps");
    graphSettings[7]->graphName = strdup("ADC128D818_CH7_VoutvsVin.eps");
    graphSettings[0]->xAxis->label = strdup("Input Voltage [V]");
    graphSettings[1]->xAxis->label = strdup("Input Voltage [V]");
    graphSettings[2]->xAxis->label = strdup("Input Voltage [V]");
    graphSettings[3]->xAxis->label = strdup("Input Voltage [V]");
    graphSettings[4]->xAxis->label = strdup("Input Voltage [V]");
    graphSettings[5]->xAxis->label = strdup("Input Voltage [V]");
    graphSettings[6]->xAxis->label = strdup("Input Voltage [V]");
    graphSettings[7]->xAxis->label = strdup("Input Voltage [V]");
    graphSettings[0]->yAxis->label = strdup("Measured voltage [V]");
    graphSettings[1]->yAxis->label = strdup("Measured voltage [V]");
    graphSettings[2]->yAxis->label = strdup("Measured voltage [V]");
    graphSettings[3]->yAxis->label = strdup("Measured voltage [V]");
    graphSettings[4]->yAxis->label = strdup("Measured voltage [V]");
    graphSettings[5]->yAxis->label = strdup("Measured voltage [V]");
    graphSettings[6]->yAxis->label = strdup("Measured voltage [V]");
    graphSettings[7]->yAxis->label = strdup("Measured voltage [V]");
    graphSettings[0]->legend->entryName = strdup("ADC128D818 - Channel 0");
    graphSettings[1]->legend->entryName = strdup("ADC128D818 - Channel 1");
    graphSettings[2]->legend->entryName = strdup("ADC128D818 - Channel 2");
    graphSettings[3]->legend->entryName = strdup("ADC128D818 - Channel 3");
    graphSettings[4]->legend->entryName = strdup("ADC128D818 - Channel 4");
    graphSettings[5]->legend->entryName = strdup("ADC128D818 - Channel 5");
    graphSettings[6]->legend->entryName = strdup("ADC128D818 - Channel 6");
    graphSettings[7]->legend->entryName = strdup("ADC128D818 - Channel 7");
    graphSettings[0]->yAxis->maximum = 3.4;
    graphSettings[1]->yAxis->maximum = 3.4;
    graphSettings[2]->yAxis->maximum = 3.4;
    graphSettings[3]->yAxis->maximum = 3.4;
    graphSettings[4]->yAxis->maximum = 3.4;
    graphSettings[5]->yAxis->maximum = 3.4;
    graphSettings[6]->yAxis->maximum = 3.4;
    graphSettings[7]->yAxis->maximum = 3.4;
    graphSettings[0]->legend->coordinates[0] = 0.35;
    graphSettings[0]->legend->coordinates[1] = 0.9;
    graphSettings[0]->legend->coordinates[2] = 0.6;
    graphSettings[0]->legend->coordinates[3] = 0.70;
    graphSettings[1]->legend->coordinates[0] = 0.35;
    graphSettings[1]->legend->coordinates[1] = 0.9;
    graphSettings[1]->legend->coordinates[2] = 0.6;
    graphSettings[1]->legend->coordinates[3] = 0.70;
    graphSettings[2]->legend->coordinates[0] = 0.35;
    graphSettings[2]->legend->coordinates[1] = 0.9;
    graphSettings[2]->legend->coordinates[2] = 0.6;
    graphSettings[2]->legend->coordinates[3] = 0.70;
    graphSettings[3]->legend->coordinates[0] = 0.35;
    graphSettings[3]->legend->coordinates[1] = 0.9;
    graphSettings[3]->legend->coordinates[2] = 0.6;
    graphSettings[3]->legend->coordinates[3] = 0.70;
    graphSettings[4]->legend->coordinates[0] = 0.35;
    graphSettings[4]->legend->coordinates[1] = 0.9;
    graphSettings[4]->legend->coordinates[2] = 0.6;
    graphSettings[4]->legend->coordinates[3] = 0.70;
    graphSettings[5]->legend->coordinates[0] = 0.35;
    graphSettings[5]->legend->coordinates[1] = 0.9;
    graphSettings[5]->legend->coordinates[2] = 0.6;
    graphSettings[5]->legend->coordinates[3] = 0.70;
    graphSettings[6]->legend->coordinates[0] = 0.35;
    graphSettings[6]->legend->coordinates[1] = 0.9;
    graphSettings[6]->legend->coordinates[2] = 0.6;
    graphSettings[6]->legend->coordinates[3] = 0.70;
    graphSettings[7]->legend->coordinates[0] = 0.35;
    graphSettings[7]->legend->coordinates[1] = 0.9;
    graphSettings[7]->legend->coordinates[2] = 0.6;
    graphSettings[7]->legend->coordinates[3] = 0.70;
  }
  else if (testType == "voltage_scan2") {
    // Vout vs Vin plots for AD7997
    graphSettings[0]->graphName = strdup("AD7997_CH0_VoutvsVin.eps");
    graphSettings[1]->graphName = strdup("AD7997_CH1_VoutvsVin.eps");
    graphSettings[2]->graphName = strdup("AD7997_CH2_VoutvsVin.eps");
    graphSettings[3]->graphName = strdup("AD7997_CH3_VoutvsVin.eps");
    graphSettings[4]->graphName = strdup("AD7997_CH4_VoutvsVin.eps");
    graphSettings[5]->graphName = strdup("AD7997_CH5_VoutvsVin.eps");
    graphSettings[6]->graphName = strdup("AD7997_CH6_VoutvsVin.eps");
    graphSettings[7]->graphName = strdup("AD7997_CH7_VoutvsVin.eps");
    graphSettings[0]->xAxis->label = strdup("Input Voltage [V]");
    graphSettings[1]->xAxis->label = strdup("Input Voltage [V]");
    graphSettings[2]->xAxis->label = strdup("Input Voltage [V]");
    graphSettings[3]->xAxis->label = strdup("Input Voltage [V]");
    graphSettings[4]->xAxis->label = strdup("Input Voltage [V]");
    graphSettings[5]->xAxis->label = strdup("Input Voltage [V]");
    graphSettings[6]->xAxis->label = strdup("Input Voltage [V]");
    graphSettings[7]->xAxis->label = strdup("Input Voltage [V]");
    graphSettings[0]->yAxis->label = strdup("Measured voltage [V]");
    graphSettings[1]->yAxis->label = strdup("Measured voltage [V]");
    graphSettings[2]->yAxis->label = strdup("Measured voltage [V]");
    graphSettings[3]->yAxis->label = strdup("Measured voltage [V]");
    graphSettings[4]->yAxis->label = strdup("Measured voltage [V]");
    graphSettings[5]->yAxis->label = strdup("Measured voltage [V]");
    graphSettings[6]->yAxis->label = strdup("Measured voltage [V]");
    graphSettings[7]->yAxis->label = strdup("Measured voltage [V]");
    graphSettings[0]->legend->entryName = strdup("AD7997 - Channel 0");
    graphSettings[1]->legend->entryName = strdup("AD7997 - Channel 1");
    graphSettings[2]->legend->entryName = strdup("AD7997 - Channel 2");
    graphSettings[3]->legend->entryName = strdup("AD7997 - Channel 3");
    graphSettings[4]->legend->entryName = strdup("AD7997 - Channel 4");
    graphSettings[5]->legend->entryName = strdup("AD7997 - Channel 5");
    graphSettings[6]->legend->entryName = strdup("AD7997 - Channel 6");
    graphSettings[7]->legend->entryName = strdup("AD7997 - Channel 7");
    graphSettings[0]->yAxis->maximum = 3.4;
    graphSettings[1]->yAxis->maximum = 3.4;
    graphSettings[2]->yAxis->maximum = 3.4;
    graphSettings[3]->yAxis->maximum = 3.4;
    graphSettings[4]->yAxis->maximum = 3.4;
    graphSettings[5]->yAxis->maximum = 3.4;
    graphSettings[6]->yAxis->maximum = 3.4;
    graphSettings[7]->yAxis->maximum = 3.4;
    graphSettings[0]->legend->coordinates[0] = 0.35;
    graphSettings[0]->legend->coordinates[1] = 0.9;
    graphSettings[0]->legend->coordinates[2] = 0.6;
    graphSettings[0]->legend->coordinates[3] = 0.70;
    graphSettings[1]->legend->coordinates[0] = 0.35;
    graphSettings[1]->legend->coordinates[1] = 0.9;
    graphSettings[1]->legend->coordinates[2] = 0.6;
    graphSettings[1]->legend->coordinates[3] = 0.70;
    graphSettings[2]->legend->coordinates[0] = 0.35;
    graphSettings[2]->legend->coordinates[1] = 0.9;
    graphSettings[2]->legend->coordinates[2] = 0.6;
    graphSettings[2]->legend->coordinates[3] = 0.70;
    graphSettings[3]->legend->coordinates[0] = 0.35;
    graphSettings[3]->legend->coordinates[1] = 0.9;
    graphSettings[3]->legend->coordinates[2] = 0.6;
    graphSettings[3]->legend->coordinates[3] = 0.70;
    graphSettings[4]->legend->coordinates[0] = 0.35;
    graphSettings[4]->legend->coordinates[1] = 0.9;
    graphSettings[4]->legend->coordinates[2] = 0.6;
    graphSettings[4]->legend->coordinates[3] = 0.70;
    graphSettings[5]->legend->coordinates[0] = 0.35;
    graphSettings[5]->legend->coordinates[1] = 0.9;
    graphSettings[5]->legend->coordinates[2] = 0.6;
    graphSettings[5]->legend->coordinates[3] = 0.70;
    graphSettings[6]->legend->coordinates[0] = 0.35;
    graphSettings[6]->legend->coordinates[1] = 0.9;
    graphSettings[6]->legend->coordinates[2] = 0.6;
    graphSettings[6]->legend->coordinates[3] = 0.70;
    graphSettings[7]->legend->coordinates[0] = 0.35;
    graphSettings[7]->legend->coordinates[1] = 0.9;
    graphSettings[7]->legend->coordinates[2] = 0.6;
    graphSettings[7]->legend->coordinates[3] = 0.70;
  }
  else {
    std::cout << "DisplayStructures: Unknown test type provided while filling data structures for ADCs (\"" << testType << "\").\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void DisplayStructures::InitializeDifferentialToSingleEndedDriver(std::string testType)
{
  if (testType == "biterror_rate") {
    // Error rate vs frequency
    graphSettings[0]->graphName = strdup("SN65MLVD080DGG_CH0_ErrRateVsFrequency.eps");
    graphSettings[1]->graphName = strdup("SN65MLVD080DGG_CH1_ErrRateVsFrequency.eps");
    graphSettings[2]->graphName = strdup("SN65MLVD080DGG_CH2_ErrRateVsFrequency.eps");
    graphSettings[3]->graphName = strdup("SN65MLVD080DGG_CH3_ErrRateVsFrequency.eps");
    graphSettings[4]->graphName = strdup("SN65MLVD080DGG_CH4_ErrRateVsFrequency.eps");
    graphSettings[5]->graphName = strdup("SN65MLVD080DGG_CH5_ErrRateVsFrequency.eps");
    graphSettings[6]->graphName = strdup("SN65MLVD080DGG_CH6_ErrRateVsFrequency.eps");
    graphSettings[7]->graphName = strdup("SN65MLVD080DGG_CH7_ErrRateVsFrequency.eps");
    graphSettings[0]->xAxis->label = strdup("Frequency [10 kHz]");
    graphSettings[1]->xAxis->label = strdup("Frequency [10 kHz]");
    graphSettings[2]->xAxis->label = strdup("Frequency [10 kHz]");
    graphSettings[3]->xAxis->label = strdup("Frequency [10 kHz]");
    graphSettings[4]->xAxis->label = strdup("Frequency [10 kHz]");
    graphSettings[5]->xAxis->label = strdup("Frequency [10 kHz]");
    graphSettings[6]->xAxis->label = strdup("Frequency [10 kHz]");
    graphSettings[7]->xAxis->label = strdup("Frequency [10 kHz]");
    graphSettings[0]->yAxis->label = strdup("Error rate [Number Of Errors]");
    graphSettings[1]->yAxis->label = strdup("Error rate [Number Of Errors]");
    graphSettings[2]->yAxis->label = strdup("Error rate [Number Of Errors]");
    graphSettings[3]->yAxis->label = strdup("Error rate [Number Of Errors]");
    graphSettings[4]->yAxis->label = strdup("Error rate [Number Of Errors]");
    graphSettings[5]->yAxis->label = strdup("Error rate [Number Of Errors]");
    graphSettings[6]->yAxis->label = strdup("Error rate [Number Of Errors]");
    graphSettings[7]->yAxis->label = strdup("Error rate [Number Of Errors]");
    graphSettings[0]->legend->entryName = strdup("SN65MLVD080DGG - Channel 0");
    graphSettings[1]->legend->entryName = strdup("SN65MLVD080DGG - Channel 1");
    graphSettings[2]->legend->entryName = strdup("SN65MLVD080DGG - Channel 2");
    graphSettings[3]->legend->entryName = strdup("SN65MLVD080DGG - Channel 3");
    graphSettings[4]->legend->entryName = strdup("SN65MLVD080DGG - Channel 4");
    graphSettings[5]->legend->entryName = strdup("SN65MLVD080DGG - Channel 5");
    graphSettings[6]->legend->entryName = strdup("SN65MLVD080DGG - Channel 6");
    graphSettings[7]->legend->entryName = strdup("SN65MLVD080DGG - Channel 7");
    graphSettings[0]->yAxis->maximum = 2000;
    graphSettings[1]->yAxis->maximum = 2000;
    graphSettings[2]->yAxis->maximum = 2000;
    graphSettings[3]->yAxis->maximum = 2000;
    graphSettings[4]->yAxis->maximum = 2000;
    graphSettings[5]->yAxis->maximum = 2000;
    graphSettings[6]->yAxis->maximum = 2000;
    graphSettings[7]->yAxis->maximum = 2000;
    graphSettings[0]->legend->coordinates[0] = 0.35;
    graphSettings[0]->legend->coordinates[1] = 0.9;
    graphSettings[0]->legend->coordinates[2] = 0.6;
    graphSettings[0]->legend->coordinates[3] = 0.70;
    graphSettings[1]->legend->coordinates[0] = 0.35;
    graphSettings[1]->legend->coordinates[1] = 0.9;
    graphSettings[1]->legend->coordinates[2] = 0.6;
    graphSettings[1]->legend->coordinates[3] = 0.70;
    graphSettings[2]->legend->coordinates[0] = 0.35;
    graphSettings[2]->legend->coordinates[1] = 0.9;
    graphSettings[2]->legend->coordinates[2] = 0.6;
    graphSettings[2]->legend->coordinates[3] = 0.70;
    graphSettings[3]->legend->coordinates[0] = 0.35;
    graphSettings[3]->legend->coordinates[1] = 0.9;
    graphSettings[3]->legend->coordinates[2] = 0.6;
    graphSettings[3]->legend->coordinates[3] = 0.70;
    graphSettings[4]->legend->coordinates[0] = 0.35;
    graphSettings[4]->legend->coordinates[1] = 0.9;
    graphSettings[4]->legend->coordinates[2] = 0.6;
    graphSettings[4]->legend->coordinates[3] = 0.70;
    graphSettings[5]->legend->coordinates[0] = 0.35;
    graphSettings[5]->legend->coordinates[1] = 0.9;
    graphSettings[5]->legend->coordinates[2] = 0.6;
    graphSettings[5]->legend->coordinates[3] = 0.70;
    graphSettings[6]->legend->coordinates[0] = 0.35;
    graphSettings[6]->legend->coordinates[1] = 0.9;
    graphSettings[6]->legend->coordinates[2] = 0.6;
    graphSettings[6]->legend->coordinates[3] = 0.70;
    graphSettings[7]->legend->coordinates[0] = 0.35;
    graphSettings[7]->legend->coordinates[1] = 0.9;
    graphSettings[7]->legend->coordinates[2] = 0.6;
    graphSettings[7]->legend->coordinates[3] = 0.70;
  }
  else {
    std::cout << "DisplayStructures: Unknown test type provided while filling data structures for differential to single ended driver (\"" << testType << "\").\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void DisplayStructures::InitializeComparator(std::string testType)
{
  if (testType == "voltage_scan") {
    // Vout vs Vin plots
    graphSettings[0]->graphName = strdup("Comparator_output.eps");
    graphSettings[1]->graphName = strdup("Comparator_output_bar.eps");
    graphSettings[0]->xAxis->label = strdup("Input voltage [V]");
    graphSettings[1]->xAxis->label = strdup("Input voltage [V]");
    graphSettings[0]->yAxis->label = strdup("State");
    graphSettings[1]->yAxis->label = strdup("State");
    graphSettings[0]->legend->entryName = strdup("Output signal state");
    graphSettings[1]->legend->entryName = strdup("Output bar signal state");
    graphSettings[0]->yAxis->maximum = 3.4;
    graphSettings[1]->yAxis->maximum = 3.4;
    graphSettings[0]->legend->coordinates[0] = 0.35;
    graphSettings[0]->legend->coordinates[1] = 0.9;
    graphSettings[0]->legend->coordinates[2] = 0.6;
    graphSettings[0]->legend->coordinates[3] = 0.70;
    graphSettings[1]->legend->coordinates[0] = 0.35;
    graphSettings[1]->legend->coordinates[1] = 0.9;
    graphSettings[1]->legend->coordinates[2] = 0.6;
    graphSettings[1]->legend->coordinates[3] = 0.70;
  }
  else {
    std::cout << "DisplayStructures: Unknown test type provided while filling data structures for comparator (\"" << testType << "\").\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void DisplayStructures::InitializeResistanceToDigital(std::string testType)
{
  if (testType == "resistance_scan") {
    // Vout vs Vin plots
    graphSettings[0]->graphName = strdup("MAX31865_MeasRoutVsRin.eps");
    graphSettings[0]->xAxis->label = strdup("Input resistance [Ohm]");
    graphSettings[0]->yAxis->label = strdup("Measured resistance [Ohm]");
    graphSettings[0]->legend->entryName = strdup("MAX31865 - Resistance");
    graphSettings[0]->yAxis->maximum = 400;
    graphSettings[0]->legend->coordinates[0] = 0.35;
    graphSettings[0]->legend->coordinates[1] = 0.9;
    graphSettings[0]->legend->coordinates[2] = 0.6;
    graphSettings[0]->legend->coordinates[3] = 0.70;
  }
  else {
    std::cout << "DisplayStructures: Unknown test type provided while filling data structures for resistance to digital (\"" << testType << "\").\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void DisplayStructures::InitializeI2cDriver(std::string testType)
{
  if (testType == "transaction_failurerate") {
    // Vout vs Vin plots
    graphSettings[0]->graphName = strdup("P82B96DP_FailureRate.eps");
    graphSettings[0]->xAxis->label = strdup("Test number");
    graphSettings[0]->yAxis->label = strdup("Failure Rate [Number of Bits]");
    graphSettings[0]->legend->entryName = strdup("P82B96DP");
    graphSettings[0]->yAxis->maximum = 400;
    graphSettings[0]->legend->coordinates[0] = 0.35;
    graphSettings[0]->legend->coordinates[1] = 0.9;
    graphSettings[0]->legend->coordinates[2] = 0.6;
    graphSettings[0]->legend->coordinates[3] = 0.70;
  }
  else {
    std::cout << "DisplayStructures: Unknown test type provided while filling data structures for I2C driver (\"" << testType << "\").\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void DisplayStructures::InitializeOnlineMonitoring(std::string testType)
{
  if (testType == "dose_test0") {
    // Vout vs Dose plots for differential amplifiers
    graphSettings[0]->graphName = strdup("MAX4199ESA_VoutVsDose.eps");
    graphSettings[1]->graphName = strdup("INA225_VoutVsDose.eps");
    graphSettings[2]->graphName = strdup("AD8418A_VoutVsDose.eps");
    graphSettings[0]->xAxis->label = strdup("Dose [kRad]");
    graphSettings[1]->xAxis->label = strdup("Dose [kRad]");
    graphSettings[2]->xAxis->label = strdup("Dose [kRad]");
    graphSettings[0]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[1]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[2]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[0]->legend->entryName = strdup("MAX4199ESA");
    graphSettings[1]->legend->entryName = strdup("INA225");
    graphSettings[2]->legend->entryName = strdup("AD8418A");
    graphSettings[0]->yAxis->maximum = 3.4;
    graphSettings[1]->yAxis->maximum = 3.4;
    graphSettings[2]->yAxis->maximum = 3.4;
    graphSettings[0]->legend->coordinates[0] = 0.35;
    graphSettings[0]->legend->coordinates[1] = 0.9;
    graphSettings[0]->legend->coordinates[2] = 0.6;
    graphSettings[0]->legend->coordinates[3] = 0.70;
    graphSettings[1]->legend->coordinates[0] = 0.35;
    graphSettings[1]->legend->coordinates[1] = 0.9;
    graphSettings[1]->legend->coordinates[2] = 0.6;
    graphSettings[1]->legend->coordinates[3] = 0.70;
    graphSettings[2]->legend->coordinates[0] = 0.35;
    graphSettings[2]->legend->coordinates[1] = 0.9;
    graphSettings[2]->legend->coordinates[2] = 0.6;
    graphSettings[2]->legend->coordinates[3] = 0.70;
  }
  else if (testType == "dose_test1") {
    // Vout vs Dose plots for DACs
    graphSettings[0]->graphName = strdup("ADC5671R_CH7_VoutVsDose.eps");
    graphSettings[1]->graphName = strdup("ADC5671R_CH6_VoutVsDose.eps");
    graphSettings[2]->graphName = strdup("ADC5671R_CH5_VoutVsDose.eps");
    graphSettings[3]->graphName = strdup("ADC5671R_CH4_VoutVsDose.eps");
    graphSettings[4]->graphName = strdup("ADC5671R_CH3_VoutVsDose.eps");
    graphSettings[5]->graphName = strdup("ADC5671R_CH2_VoutVsDose.eps");
    graphSettings[6]->graphName = strdup("ADC5671R_CH1_VoutVsDose.eps");
    graphSettings[7]->graphName = strdup("ADC5671R_CH0_VoutVsDose.eps");
    graphSettings[0]->xAxis->label = strdup("Dose [kRad]");
    graphSettings[1]->xAxis->label = strdup("Dose [kRad]");
    graphSettings[2]->xAxis->label = strdup("Dose [kRad]");
    graphSettings[3]->xAxis->label = strdup("Dose [kRad]");
    graphSettings[4]->xAxis->label = strdup("Dose [kRad]");
    graphSettings[5]->xAxis->label = strdup("Dose [kRad]");
    graphSettings[6]->xAxis->label = strdup("Dose [kRad]");
    graphSettings[7]->xAxis->label = strdup("Dose [kRad]");
    graphSettings[0]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[1]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[2]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[3]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[4]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[5]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[6]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[7]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[0]->legend->entryName = strdup("ADC5671R - Channel 7");
    graphSettings[1]->legend->entryName = strdup("ADC5671R - Channel 6");
    graphSettings[2]->legend->entryName = strdup("ADC5671R - Channel 5");
    graphSettings[3]->legend->entryName = strdup("ADC5671R - Channel 4");
    graphSettings[4]->legend->entryName = strdup("ADC5671R - Channel 3");
    graphSettings[5]->legend->entryName = strdup("ADC5671R - Channel 2");
    graphSettings[6]->legend->entryName = strdup("ADC5671R - Channel 1");
    graphSettings[7]->legend->entryName = strdup("ADC5671R - Channel 0");
    graphSettings[0]->yAxis->maximum = 3.4;
    graphSettings[1]->yAxis->maximum = 3.4;
    graphSettings[2]->yAxis->maximum = 3.4;
    graphSettings[3]->yAxis->maximum = 3.4;
    graphSettings[4]->yAxis->maximum = 3.4;
    graphSettings[5]->yAxis->maximum = 3.4;
    graphSettings[6]->yAxis->maximum = 3.4;
    graphSettings[7]->yAxis->maximum = 3.4;
    graphSettings[0]->legend->coordinates[0] = 0.35;
    graphSettings[0]->legend->coordinates[1] = 0.9;
    graphSettings[0]->legend->coordinates[2] = 0.6;
    graphSettings[0]->legend->coordinates[3] = 0.70;
    graphSettings[1]->legend->coordinates[0] = 0.35;
    graphSettings[1]->legend->coordinates[1] = 0.9;
    graphSettings[1]->legend->coordinates[2] = 0.6;
    graphSettings[1]->legend->coordinates[3] = 0.70;
    graphSettings[2]->legend->coordinates[0] = 0.35;
    graphSettings[2]->legend->coordinates[1] = 0.9;
    graphSettings[2]->legend->coordinates[2] = 0.6;
    graphSettings[2]->legend->coordinates[3] = 0.70;
    graphSettings[3]->legend->coordinates[0] = 0.35;
    graphSettings[3]->legend->coordinates[1] = 0.9;
    graphSettings[3]->legend->coordinates[2] = 0.6;
    graphSettings[3]->legend->coordinates[3] = 0.70;
    graphSettings[4]->legend->coordinates[0] = 0.35;
    graphSettings[4]->legend->coordinates[1] = 0.9;
    graphSettings[4]->legend->coordinates[2] = 0.6;
    graphSettings[4]->legend->coordinates[3] = 0.70;
    graphSettings[5]->legend->coordinates[0] = 0.35;
    graphSettings[5]->legend->coordinates[1] = 0.9;
    graphSettings[5]->legend->coordinates[2] = 0.6;
    graphSettings[5]->legend->coordinates[3] = 0.70;
    graphSettings[6]->legend->coordinates[0] = 0.35;
    graphSettings[6]->legend->coordinates[1] = 0.9;
    graphSettings[6]->legend->coordinates[2] = 0.6;
    graphSettings[6]->legend->coordinates[3] = 0.70;
    graphSettings[7]->legend->coordinates[0] = 0.35;
    graphSettings[7]->legend->coordinates[1] = 0.9;
    graphSettings[7]->legend->coordinates[2] = 0.6;
    graphSettings[7]->legend->coordinates[3] = 0.70;
  }
  else if (testType == "dose_test2") {
    // Vout vs Dose plots for first 5 DC-DC converters
    graphSettings[0]->graphName = strdup("FPF1007_VoutVsDose.eps");
    graphSettings[1]->graphName = strdup("MIC29371-3.3WU_VoutVsDose.eps");
    graphSettings[2]->graphName = strdup("MAX15053EWL_VoutVsDose.eps");
    graphSettings[3]->graphName = strdup("NTGD1100LT1G_DEV0_VoutVsDose.eps");
    graphSettings[4]->graphName = strdup("NTGD1100LT1G_DEV1_VoutVsDose.eps");
    graphSettings[0]->xAxis->label = strdup("Dose [kRad]");
    graphSettings[1]->xAxis->label = strdup("Dose [kRad]");
    graphSettings[2]->xAxis->label = strdup("Dose [kRad]");
    graphSettings[3]->xAxis->label = strdup("Dose [kRad]");
    graphSettings[4]->xAxis->label = strdup("Dose [kRad]");
    graphSettings[0]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[1]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[2]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[3]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[4]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[0]->legend->entryName = strdup("FPF1007");
    graphSettings[1]->legend->entryName = strdup("MIC29371-3.3WU");
    graphSettings[2]->legend->entryName = strdup("MAX15053EWL");
    graphSettings[3]->legend->entryName = strdup("NTGD1100LT1G - Device 0");
    graphSettings[4]->legend->entryName = strdup("NTGD1100LT1G - Device 1");
    graphSettings[0]->yAxis->maximum = 5;
    graphSettings[1]->yAxis->maximum = 5;
    graphSettings[2]->yAxis->maximum = 5;
    graphSettings[3]->yAxis->maximum = 5;
    graphSettings[4]->yAxis->maximum = 5;
    graphSettings[0]->legend->coordinates[0] = 0.35;
    graphSettings[0]->legend->coordinates[1] = 0.9;
    graphSettings[0]->legend->coordinates[2] = 0.6;
    graphSettings[0]->legend->coordinates[3] = 0.70;
    graphSettings[1]->legend->coordinates[0] = 0.35;
    graphSettings[1]->legend->coordinates[1] = 0.9;
    graphSettings[1]->legend->coordinates[2] = 0.6;
    graphSettings[1]->legend->coordinates[3] = 0.70;
    graphSettings[2]->legend->coordinates[0] = 0.35;
    graphSettings[2]->legend->coordinates[1] = 0.9;
    graphSettings[2]->legend->coordinates[2] = 0.6;
    graphSettings[2]->legend->coordinates[3] = 0.70;
    graphSettings[3]->legend->coordinates[0] = 0.35;
    graphSettings[3]->legend->coordinates[1] = 0.9;
    graphSettings[3]->legend->coordinates[2] = 0.6;
    graphSettings[3]->legend->coordinates[3] = 0.70;
    graphSettings[4]->legend->coordinates[0] = 0.35;
    graphSettings[4]->legend->coordinates[1] = 0.9;
    graphSettings[4]->legend->coordinates[2] = 0.6;
    graphSettings[4]->legend->coordinates[3] = 0.70;
  }
  else if (testType == "dose_test3") {
    // Vout vs Dose plots for second 6 DC-DC converters
    graphSettings[0]->graphName = strdup("SI3865DDV_DEV1_VoutVsDose.eps");
    graphSettings[1]->graphName = strdup("SI3865DDV_DEV0_VoutVsDose.eps");
    graphSettings[2]->graphName = strdup("ADP7159ARDZ_DEV1_VoutVsDose.eps");
    graphSettings[3]->graphName = strdup("MIC39152WD_DEV1_VoutVsDose.eps");
    graphSettings[4]->graphName = strdup("MIC39152WD_DEV0_VoutVsDose.eps");
    graphSettings[5]->graphName = strdup("ADP7159ARDZ_DEV0_VoutVsDose.eps");
    graphSettings[0]->xAxis->label = strdup("Dose [kRad]");
    graphSettings[1]->xAxis->label = strdup("Dose [kRad]");
    graphSettings[2]->xAxis->label = strdup("Dose [kRad]");
    graphSettings[3]->xAxis->label = strdup("Dose [kRad]");
    graphSettings[4]->xAxis->label = strdup("Dose [kRad]");
    graphSettings[5]->xAxis->label = strdup("Dose [kRad]");
    graphSettings[0]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[1]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[2]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[3]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[4]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[5]->yAxis->label = strdup("Output voltage [V]");
    graphSettings[0]->legend->entryName = strdup("SI3865DDV - Device 1");
    graphSettings[1]->legend->entryName = strdup("SI3865DDV - Device 0");
    graphSettings[2]->legend->entryName = strdup("ADP7159ARDZ - Device 1");
    graphSettings[3]->legend->entryName = strdup("MIC39152WD - Device 1");
    graphSettings[4]->legend->entryName = strdup("MIC39152WD - Device 0");
    graphSettings[5]->legend->entryName = strdup("ADP7159ARDZ - Device 0");
    graphSettings[0]->yAxis->maximum = 5;
    graphSettings[1]->yAxis->maximum = 5;
    graphSettings[2]->yAxis->maximum = 5;
    graphSettings[3]->yAxis->maximum = 5;
    graphSettings[4]->yAxis->maximum = 5;
    graphSettings[5]->yAxis->maximum = 5;
    graphSettings[0]->legend->coordinates[0] = 0.35;
    graphSettings[0]->legend->coordinates[1] = 0.9;
    graphSettings[0]->legend->coordinates[2] = 0.6;
    graphSettings[0]->legend->coordinates[3] = 0.70;
    graphSettings[1]->legend->coordinates[0] = 0.35;
    graphSettings[1]->legend->coordinates[1] = 0.9;
    graphSettings[1]->legend->coordinates[2] = 0.6;
    graphSettings[1]->legend->coordinates[3] = 0.70;
    graphSettings[2]->legend->coordinates[0] = 0.35;
    graphSettings[2]->legend->coordinates[1] = 0.9;
    graphSettings[2]->legend->coordinates[2] = 0.6;
    graphSettings[2]->legend->coordinates[3] = 0.70;
    graphSettings[3]->legend->coordinates[0] = 0.35;
    graphSettings[3]->legend->coordinates[1] = 0.9;
    graphSettings[3]->legend->coordinates[2] = 0.6;
    graphSettings[3]->legend->coordinates[3] = 0.70;
    graphSettings[4]->legend->coordinates[0] = 0.35;
    graphSettings[4]->legend->coordinates[1] = 0.9;
    graphSettings[4]->legend->coordinates[2] = 0.6;
    graphSettings[4]->legend->coordinates[3] = 0.70;
    graphSettings[5]->legend->coordinates[0] = 0.35;
    graphSettings[5]->legend->coordinates[1] = 0.9;
    graphSettings[5]->legend->coordinates[2] = 0.6;
    graphSettings[5]->legend->coordinates[3] = 0.70;
  }
  else {
    std::cout << "DisplayStructures: Unknown test type provided while filling data structures for device (\"" << testType << "\").\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void DisplayStructures::InitializeIoExpander(std::string testType)
{
  if (testType == "status_check0") {
    //
    graphSettings[0]->graphName = strdup("PCF8574_test0.eps");
    graphSettings[0]->xAxis->label = strdup("Trial number");
    graphSettings[0]->yAxis->label = strdup("Output");
    graphSettings[0]->legend->entryName = strdup("PCF8574");
    graphSettings[0]->yAxis->maximum = 2;
    graphSettings[0]->legend->coordinates[0] = 0.35;
    graphSettings[0]->legend->coordinates[1] = 0.9;
    graphSettings[0]->legend->coordinates[2] = 0.6;
    graphSettings[0]->legend->coordinates[3] = 0.70;
  }
  else if (testType == "status_check1") {
    //
    graphSettings[0]->graphName = strdup("PCF8574_test1.eps");
    graphSettings[0]->xAxis->label = strdup("Stimulated channel");
    graphSettings[0]->yAxis->label = strdup("Output");
    graphSettings[0]->legend->entryName = strdup("PCF8574");
    graphSettings[0]->yAxis->maximum = 2;
    graphSettings[0]->legend->coordinates[0] = 0.35;
    graphSettings[0]->legend->coordinates[1] = 0.9;
    graphSettings[0]->legend->coordinates[2] = 0.6;
    graphSettings[0]->legend->coordinates[3] = 0.70;
  }
  else {
    std::cout << "DisplayStructures: Unknown test type provided while filling data structures for IO expander (\"" << testType << "\").\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}


/*
void DisplayStructures::InitializeIoExpander(std::string testType)
{
  if (testType == "status_check0") {
    // channel output vs channel transitioned
    graphSettings[0]->graphName = strdup("PCF8574_CH0_InitialState.eps");
    graphSettings[1]->graphName = strdup("PCF8574_CH1_InitialState.eps");
    graphSettings[2]->graphName = strdup("PCF8574_CH2_InitialState.eps");
    graphSettings[3]->graphName = strdup("PCF8574_CH3_InitialState.eps");
    graphSettings[4]->graphName = strdup("PCF8574_CH4_InitialState.eps");
    graphSettings[5]->graphName = strdup("PCF8574_CH5_InitialState.eps");
    graphSettings[6]->graphName = strdup("PCF8574_CH6_InitialState.eps");
    graphSettings[7]->graphName = strdup("PCF8574_CH7_InitialState.eps");
    graphSettings[0]->xAxis->label = strdup("Channel transitioned");
    graphSettings[1]->xAxis->label = strdup("Channel transitioned");
    graphSettings[2]->xAxis->label = strdup("Channel transitioned");
    graphSettings[3]->xAxis->label = strdup("Channel transitioned");
    graphSettings[4]->xAxis->label = strdup("Channel transitioned");
    graphSettings[5]->xAxis->label = strdup("Channel transitioned");
    graphSettings[6]->xAxis->label = strdup("Channel transitioned");
    graphSettings[7]->xAxis->label = strdup("Channel transitioned");
    graphSettings[0]->yAxis->label = strdup("Channel output");
    graphSettings[1]->yAxis->label = strdup("Channel output");
    graphSettings[2]->yAxis->label = strdup("Channel output");
    graphSettings[3]->yAxis->label = strdup("Channel output");
    graphSettings[4]->yAxis->label = strdup("Channel output");
    graphSettings[5]->yAxis->label = strdup("Channel output");
    graphSettings[6]->yAxis->label = strdup("Channel output");
    graphSettings[7]->yAxis->label = strdup("Channel output");
    graphSettings[0]->legend->entryName = strdup("PCF8574 - Channel 0");
    graphSettings[1]->legend->entryName = strdup("PCF8574 - Channel 1");
    graphSettings[2]->legend->entryName = strdup("PCF8574 - Channel 2");
    graphSettings[3]->legend->entryName = strdup("PCF8574 - Channel 3");
    graphSettings[4]->legend->entryName = strdup("PCF8574 - Channel 4");
    graphSettings[5]->legend->entryName = strdup("PCF8574 - Channel 5");
    graphSettings[6]->legend->entryName = strdup("PCF8574 - Channel 6");
    graphSettings[7]->legend->entryName = strdup("PCF8574 - Channel 7");
    graphSettings[0]->yAxis->maximum = 2;
    graphSettings[1]->yAxis->maximum = 2;
    graphSettings[2]->yAxis->maximum = 2;
    graphSettings[3]->yAxis->maximum = 2;
    graphSettings[4]->yAxis->maximum = 2;
    graphSettings[5]->yAxis->maximum = 2;
    graphSettings[6]->yAxis->maximum = 2;
    graphSettings[7]->yAxis->maximum = 2;
    graphSettings[0]->legend->coordinates[0] = 0.35;
    graphSettings[0]->legend->coordinates[1] = 0.9;
    graphSettings[0]->legend->coordinates[2] = 0.6;
    graphSettings[0]->legend->coordinates[3] = 0.70;
    graphSettings[1]->legend->coordinates[0] = 0.35;
    graphSettings[1]->legend->coordinates[1] = 0.9;
    graphSettings[1]->legend->coordinates[2] = 0.6;
    graphSettings[1]->legend->coordinates[3] = 0.70;
    graphSettings[2]->legend->coordinates[0] = 0.35;
    graphSettings[2]->legend->coordinates[1] = 0.9;
    graphSettings[2]->legend->coordinates[2] = 0.6;
    graphSettings[2]->legend->coordinates[3] = 0.70;
    graphSettings[3]->legend->coordinates[0] = 0.35;
    graphSettings[3]->legend->coordinates[1] = 0.9;
    graphSettings[3]->legend->coordinates[2] = 0.6;
    graphSettings[3]->legend->coordinates[3] = 0.70;
    graphSettings[4]->legend->coordinates[0] = 0.35;
    graphSettings[4]->legend->coordinates[1] = 0.9;
    graphSettings[4]->legend->coordinates[2] = 0.6;
    graphSettings[4]->legend->coordinates[3] = 0.70;
    graphSettings[5]->legend->coordinates[0] = 0.35;
    graphSettings[5]->legend->coordinates[1] = 0.9;
    graphSettings[5]->legend->coordinates[2] = 0.6;
    graphSettings[5]->legend->coordinates[3] = 0.70;
    graphSettings[6]->legend->coordinates[0] = 0.35;
    graphSettings[6]->legend->coordinates[1] = 0.9;
    graphSettings[6]->legend->coordinates[2] = 0.6;
    graphSettings[6]->legend->coordinates[3] = 0.70;
    graphSettings[7]->legend->coordinates[0] = 0.35;
    graphSettings[7]->legend->coordinates[1] = 0.9;
    graphSettings[7]->legend->coordinates[2] = 0.6;
    graphSettings[7]->legend->coordinates[3] = 0.70;
  }
  else if (testType == "status_check1") {
    // channel output vs channel transitioned
    graphSettings[0]->graphName = strdup("PCF8574_CH0_TransitionOccurred.eps");
    graphSettings[1]->graphName = strdup("PCF8574_CH1_TransitionOccurred.eps");
    graphSettings[2]->graphName = strdup("PCF8574_CH2_TransitionOccurred.eps");
    graphSettings[3]->graphName = strdup("PCF8574_CH3_TransitionOccurred.eps");
    graphSettings[4]->graphName = strdup("PCF8574_CH4_TransitionOccurred.eps");
    graphSettings[5]->graphName = strdup("PCF8574_CH5_TransitionOccurred.eps");
    graphSettings[6]->graphName = strdup("PCF8574_CH6_TransitionOccurred.eps");
    graphSettings[7]->graphName = strdup("PCF8574_CH7_TransitionOccurred.eps");
    graphSettings[0]->xAxis->label = strdup("Channel transitioned");
    graphSettings[1]->xAxis->label = strdup("Channel transitioned");
    graphSettings[2]->xAxis->label = strdup("Channel transitioned");
    graphSettings[3]->xAxis->label = strdup("Channel transitioned");
    graphSettings[4]->xAxis->label = strdup("Channel transitioned");
    graphSettings[5]->xAxis->label = strdup("Channel transitioned");
    graphSettings[6]->xAxis->label = strdup("Channel transitioned");
    graphSettings[7]->xAxis->label = strdup("Channel transitioned");
    graphSettings[0]->yAxis->label = strdup("Channel output");
    graphSettings[1]->yAxis->label = strdup("Channel output");
    graphSettings[2]->yAxis->label = strdup("Channel output");
    graphSettings[3]->yAxis->label = strdup("Channel output");
    graphSettings[4]->yAxis->label = strdup("Channel output");
    graphSettings[5]->yAxis->label = strdup("Channel output");
    graphSettings[6]->yAxis->label = strdup("Channel output");
    graphSettings[7]->yAxis->label = strdup("Channel output");
    graphSettings[0]->legend->entryName = strdup("PCF8574 - Channel 0");
    graphSettings[1]->legend->entryName = strdup("PCF8574 - Channel 1");
    graphSettings[2]->legend->entryName = strdup("PCF8574 - Channel 2");
    graphSettings[3]->legend->entryName = strdup("PCF8574 - Channel 3");
    graphSettings[4]->legend->entryName = strdup("PCF8574 - Channel 4");
    graphSettings[5]->legend->entryName = strdup("PCF8574 - Channel 5");
    graphSettings[6]->legend->entryName = strdup("PCF8574 - Channel 6");
    graphSettings[7]->legend->entryName = strdup("PCF8574 - Channel 7");
    graphSettings[0]->yAxis->maximum = 2;
    graphSettings[1]->yAxis->maximum = 2;
    graphSettings[2]->yAxis->maximum = 2;
    graphSettings[3]->yAxis->maximum = 2;
    graphSettings[4]->yAxis->maximum = 2;
    graphSettings[5]->yAxis->maximum = 2;
    graphSettings[6]->yAxis->maximum = 2;
    graphSettings[7]->yAxis->maximum = 2;
    graphSettings[0]->legend->coordinates[0] = 0.35;
    graphSettings[0]->legend->coordinates[1] = 0.9;
    graphSettings[0]->legend->coordinates[2] = 0.6;
    graphSettings[0]->legend->coordinates[3] = 0.70;
    graphSettings[1]->legend->coordinates[0] = 0.35;
    graphSettings[1]->legend->coordinates[1] = 0.9;
    graphSettings[1]->legend->coordinates[2] = 0.6;
    graphSettings[1]->legend->coordinates[3] = 0.70;
    graphSettings[2]->legend->coordinates[0] = 0.35;
    graphSettings[2]->legend->coordinates[1] = 0.9;
    graphSettings[2]->legend->coordinates[2] = 0.6;
    graphSettings[2]->legend->coordinates[3] = 0.70;
    graphSettings[3]->legend->coordinates[0] = 0.35;
    graphSettings[3]->legend->coordinates[1] = 0.9;
    graphSettings[3]->legend->coordinates[2] = 0.6;
    graphSettings[3]->legend->coordinates[3] = 0.70;
    graphSettings[4]->legend->coordinates[0] = 0.35;
    graphSettings[4]->legend->coordinates[1] = 0.9;
    graphSettings[4]->legend->coordinates[2] = 0.6;
    graphSettings[4]->legend->coordinates[3] = 0.70;
    graphSettings[5]->legend->coordinates[0] = 0.35;
    graphSettings[5]->legend->coordinates[1] = 0.9;
    graphSettings[5]->legend->coordinates[2] = 0.6;
    graphSettings[5]->legend->coordinates[3] = 0.70;
    graphSettings[6]->legend->coordinates[0] = 0.35;
    graphSettings[6]->legend->coordinates[1] = 0.9;
    graphSettings[6]->legend->coordinates[2] = 0.6;
    graphSettings[6]->legend->coordinates[3] = 0.70;
    graphSettings[7]->legend->coordinates[0] = 0.35;
    graphSettings[7]->legend->coordinates[1] = 0.9;
    graphSettings[7]->legend->coordinates[2] = 0.6;
    graphSettings[7]->legend->coordinates[3] = 0.70;
  }
  else {
    std::cout << "DisplayStructures: Unknown test type provided while filling data structures for IO expander (\"" << testType << "\").\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}
*/
