//==========================================================================
// Statistics class - CPP file
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 25/8/2016
// Last modified: 25/8/2016
//==========================================================================

#include "Statistics.h"

//===========================================================================
//                       Constructors and Destructor
//===========================================================================

Statistics::Statistics()
{
  Initialize();
}

Statistics::~Statistics()
{
  Destroy();
}

//===========================================================================
//                            Inner methods
//===========================================================================

// Public Methods

void Statistics::DetermineMeanAndSigma(unsigned int dataDepth)
{
  assert(dataDepth != 0);
  
  double value = 0.;
  double sigmaValue = 0.;
  double meanValue = 0.;
  
  for (int i = 0; i < dataDepth; i++) {
    value = dataBuffer->front();
    dataBuffer->pop();
    meanValue += value;
    sigmaValue += (value*value);
  }
  meanValue /= dataDepth;
  sigmaValue /= dataDepth;
  sigmaValue -= meanValue*meanValue;
  sigmaValue = sqrt(sigmaValue);
  sigma->push(sigmaValue);
  mean->push(meanValue);
}

double Statistics::GetMean()
{
  double value = mean->front();
  mean->pop();

  return value;
} 

double Statistics::GetSigma()
{
  double value = sigma->front();
  sigma->pop();

  return value;
} 

void Statistics::PushWordToDataBuffer(double wordToPush)
{
  dataBuffer->push(wordToPush);
}

double Statistics::PopWordFromMeanBuffer()
{
  double value = mean->front();
  mean->pop();

  return value;
}

double Statistics::PopWordFromSigmaBuffer()
{
  double value = sigma->front();
  sigma->pop();

  return value;
}

// Private Methods

void Statistics::Initialize()
{
  dataBuffer = new std::queue<double>;
  mean = new std::queue<double>; 
  sigma = new std::queue<double>;
}

void Statistics::Destroy()
{
}
