//==========================================================================
// Tests class - Header file 
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 8/25/2016
// Last modified: 8/26/2016
//==========================================================================

#ifndef TESTS_H
#define TESTS_H

// C/C++ Libs
#include <iostream>
#include <string.h>
#include <queue>

// Other user defined Libs
#include "CompositeOperation.h"
#include "Display.h"
 
class Tests
{
  public:
     Tests(char* filename, std::string deviceType, std::string testType);
    ~Tests();

    void        StartTest();

  private:
    void        Initialize(char* filename, std::string deviceType, std::string testType);
    void        Destroy();
  
    void        StartTestDifferentialAmplifiers();
    void        StartTestDacs();
    void        StartTestDcdcConverters();
    void        StartTestAdcs();
    void        StartTestDifferentialToSingleEndedDriver();
    void        StartTestComparator();
    void        StartTestResistanceToDigital();
    void        StartTestI2cDriver();
    void        StartTestOnlineMonitoring();
    void        StartTestIoExpander();

    void        StartTestDifferentialAmplifiersVoltageScan();
    void        StartTestDacsVoltageScan();
    void        StartTestDcdcConvertersStatusCheck0();
    void        StartTestDcdcConvertersStatusCheck1();
    void        StartTestAdcsVoltageScan(unsigned int adcNumber);
    void        StartTestDifferentialToSingleEndedDriverBiterrorRate();
    void        StartTestComparatorVoltageScan();
    void        StartTestResistanceToDigitalResistanceScan();
    void        StartTestI2cDriverTransactionFailureRate();
    void        StartTestIoExpanderStatusCheck0();
    void        StartTestIoExpanderStatusCheck1();


    CompositeOperation   *compositeOperation;
    Display              *display;

    std::string           deviceType;
    std::string           testType;

    double              **dataX;
    double              **dataY;
};

#endif
