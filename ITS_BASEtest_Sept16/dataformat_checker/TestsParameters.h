//==========================================================================
// Tests Parameters class - Header file 
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 8/25/2016
// Last modified: 8/26/2016
//==========================================================================

#ifndef TESTSPARAMETERS_H
#define TESTSPARAMETERS_H

// C/C++ Libs
#include <iostream>
#include <string.h>
 
class TestsParameters
{
  public:
     TestsParameters(std::string deviceType, std::string testType);
    ~TestsParameters();

    double     **dataX;

  private:

    void        Initialize(std::string deviceType, std::string testType);
    void        Destroy();

    void        BuildStructures();
    void        FillStructures(std::string deviceType, std::string testType);

    struct Xarray
    {
      unsigned int  depth;
      double       *data;
    } **xArrays;

    double         *onlineMonitoringArray;

};

#endif
