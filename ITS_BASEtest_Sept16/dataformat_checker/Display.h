//==========================================================================
// Display class - Header file 
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 5/16/2016
// Last modified: 5/16/2016
//==========================================================================

#ifndef DISPLAY_H
#define DISPLAY_H

// C/C++ Libs
#include <assert.h>

/*
#include "TMath.h"
#include "TFrame.h"
#include "TString.h"
#include "TColor.h"
#include "TAttAxis.h"
#include "TLatex.h"
#include "math.h"
#include "unistd.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "TH1.h"
*/

// CERN ROOT Libs
#include "TAxis.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TH1.h"
#include "TMath.h"
#include "TVirtualFFT.h"

// User defined Libs
#include "DatatypeConversion.h"
#include "DisplayStructures.h"

class Display
{
  public:
    // Constructors and destructor
     Display(std::string deviceType, std::string testType);
    ~Display();  

     void SetOnlineMonitoringNumberOfPoints(unsigned int numberOfPoints);

    // Fit Methods
//    void               FFT(unsigned int numBins, double *xBins, double *yBins);
    void               FFT(unsigned int numSamples, double *samples, double *amplitude, double *phase);
    double             GetSinAmplitude(unsigned int numBins, double *xBins, double *yBins);
    bool               Fit1D(unsigned int dataSize, double *dataX, double *dataY, unsigned int funcType, double *chisquare, double *params);
    double             GetChisquare(unsigned int dataSize, double *dataX, double *dataY);
    double             GetIntercept(unsigned int dataSize, double *dataX, double *dataY);
    double             GetSlope(unsigned int dataSize, double *dataX, double *dataY);
    double             GetAmplitudeAverage(unsigned int numSamples, double *samples);
    double             GetMaxAverage(unsigned int numSamples, double *samples);
    double             GetMinAverage(unsigned int numSamples, double *samples);
    double             GetMax(unsigned int numSamples, double *samples);
    double             GetMin(unsigned int numSamples, double *samples);
    double            *GetDbValue(double scFactor, unsigned int arrayDepth, double *array);
    double            *GetDbValue(unsigned int arrayDepth, double *array);
    double             GetDbValue(double value);
    double            *Scale(double scFactor, unsigned int arrayDepth, double *array);
    double             GetClosestLower(double yValue, unsigned int arrayDepth, double *yArray);
    double             GetClosestHigher(double yValue, unsigned int arrayDepth, double *yArray);
    double             GetxValue(double yValue, unsigned int arrayDepth, double *xArray, double *yArray);

    // Plot methods
    void               PlotFftAmplitude(unsigned int numSamples, double *samples, double xRange, DisplayStructures::GraphSettings *graphSettings);
    void               PlotSingleGraph(unsigned int graphIndex, double *dataX, double *dataY);
    void               PlotMultiGraph(double **dataX, double **dataY);


    // Internal objects
    DisplayStructures *displayStructures;
};

#endif
