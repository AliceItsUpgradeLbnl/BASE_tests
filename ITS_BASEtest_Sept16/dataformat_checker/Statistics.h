//==========================================================================
// Statistics class - Header file 
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 8/25/2016
// Last modified: 8/26/2016
//==========================================================================

#ifndef STATISTICS_H
#define STATISTICS_H

// C/C++ Libs
#include <iostream>
#include <string.h>
#include <queue>
#include <assert.h> 
#include <math.h>

// Other user defined Libs
 
class Statistics
{
  public:
     Statistics();
    ~Statistics();

   void                   DetermineMeanAndSigma(unsigned int dataDepth);

   double                 GetMean();
   double                 GetSigma();

    void                  PushWordToDataBuffer(double wordToPush);

    double                PopWordFromMeanBuffer();
    double                PopWordFromSigmaBuffer();

  private:
    void                  Initialize();
    void                  Destroy();

    std::queue<double>         *dataBuffer;

    std::queue<double>         *mean;
    std::queue<double>         *sigma;
};

#endif
