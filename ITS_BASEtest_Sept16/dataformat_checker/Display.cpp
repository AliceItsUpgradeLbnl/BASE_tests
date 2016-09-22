//==========================================================================
// Display class - CPP file 
// Author: Alberto Collu - Lawrence Berkeley National Laboratory (LBNL)
// Date of Creation: 5/16/2016
// Last modified: 5/16/2016
//==========================================================================

#include "Display.h"

//=======================================================================
//                      Constructors and destructor
//=======================================================================

Display::Display(std::string deviceType, std::string testType)
{
  displayStructures = new DisplayStructures(deviceType, testType);
}

Display::~Display()
{
  delete displayStructures;
}

//=======================================================================
//                                Methods
//=======================================================================

void Display::SetOnlineMonitoringNumberOfPoints(unsigned int numberOfPoints)
{
  displayStructures->SetOnlineMonitoringNumberOfPoints(numberOfPoints);
}

void Display::PlotSingleGraph(unsigned int graphIndex, double *dataX, double *dataY)
{
  assert(graphIndex < displayStructures->numberOfGraphsToPlot);       

  TCanvas *c = new TCanvas("c", "c", 1000, 1000);
  c->SetLeftMargin(0.15);  // This avoids the Y title to be cut

  TGraph *graph = new TGraph(displayStructures->numberOfPointsPerGraph, dataX, dataY);
  TLegend *leg = new TLegend(0.65, 0.9, 0.9, 0.75);

  graph->SetMarkerStyle(20);
  graph->SetLineColor(4);
  graph->SetLineWidth(2);

  leg->AddEntry(graph, displayStructures->graphSettings[graphIndex]->legend->entryName, "l");

  graph->Draw("AL");

  graph->GetXaxis()->SetTitle(displayStructures->graphSettings[graphIndex]->xAxis->label);
  graph->GetXaxis()->CenterTitle();
  graph->GetXaxis()->SetTitleOffset(1.2);
  graph->GetXaxis()->SetLabelSize(0.03);
  graph->GetYaxis()->SetTitle(displayStructures->graphSettings[graphIndex]->yAxis->label);
  graph->GetYaxis()->CenterTitle();
  graph->GetYaxis()->SetTitleOffset(1.2);
  graph->GetYaxis()->SetLabelSize(0.03);

  leg->Draw();

  c->Update();
  c->SaveAs(displayStructures->graphSettings[graphIndex]->graphName);

  delete leg;
  delete graph;
  delete c;
}

// Fits data with user defined function
bool Display::Fit1D(unsigned int dataSize, double *dataX, double *dataY, unsigned int funcType, double *chisquare, double *params)
{
  assert((dataX != NULL) && (dataY != NULL) && (chisquare != NULL) && (params != NULL)); // Pointers to input arrays cannot be NULL
  assert(dataSize != 0); // dataSize cannot be zero

  TCanvas *c = new TCanvas("c", "Fast Fourier Transform", 800, 600);  // Prepare canvas for drawing

  TF1 *f;
  if (funcType == 0) { // fit function is a straight line
    f = new TF1("f", "[1] * x + [0]");
  }
  else if (funcType == 1) { // fit function is a parabola 
    f = new TF1("f", "[2] * x * x + [1] * x + [0]");  
  }
  else if (funcType == 2) { // fit function is a sin
    f = new TF1("f", "[3] * sin([2] * x + [1]) + [0]");
//    f->FixParameter(0,1.08346);
  }
  else {
    return false;
  }

  TGraph *g = new TGraph(dataSize, dataX, dataY);
  g->Fit(f);
//  g->Draw("AL");
//  f->Draw("same");
//  c->SaveAs("trial.eps");
  *chisquare = f->GetChisquare();
  for (int i = 0; i < f->GetNumberFreeParameters(); i++) {
    params[i] = f->GetParameter(i); 
  }
  delete c;
  return true;
}

// Fits the input data with a straight line and returns its slope
double Display::GetIntercept(unsigned int dataSize, double *dataX, double *dataY)
{
  unsigned int funcType = 0;
  double chisquare;
  double params[2];

  Fit1D(dataSize, dataX, dataY, funcType, &chisquare, params);

  return params[0];
}

double Display::GetSlope(unsigned int dataSize, double *dataX, double *dataY)
{
  unsigned int funcType = 0;
  double chisquare;
  double params[2];

  Fit1D(dataSize, dataX, dataY, funcType, &chisquare, params);

  return params[1];
}

double Display::GetChisquare(unsigned int dataSize, double *dataX, double *dataY)
{
  unsigned int funcType = 0;
  double chisquare;
  double params[2];

  Fit1D(dataSize, dataX, dataY, funcType, &chisquare, params);

  return chisquare;
}
/*
void Display::FFT(unsigned int numBins, double *xBins, double *yBins)
{
  // Check inputs
  assert(xBins != NULL && yBins != NULL);                             // Pointers to input arrays are NULL
  
  // Define environment 
  TCanvas *c = new TCanvas("c", "Fast Fourier Transform", 800, 600);  // Prepare canvas for drawing
  TH1D *hsin = new TH1D("hsin", "hsin", numBins - 1, xBins);          // Declare hystogram
  TH1 *ft = 0;                                                        // Pointer to TH1 containing FFT must be NULL at firs


  for (int i = 0; i < numBins; i++){
    xBins[i] = xBins[i]/(xBins[numBins - 2] - xBins[0]);
    yBins[i] = yBins[i]/sqrt(numBins);
  }

  // Fill hystogram
//  for (int i = 0; i < numBins; i++){
//    hsin->Fill(xBins[i], yBins[i]);
//  }

//  hsin->Scale(1./sqrt(numBins - 1));

  // Compute the transform and look at the magnitude of the output
  TVirtualFFT::SetTransform(0);
  ft = hsin->FFT(ft, "MAG");
//  ft->Scale(1./sqrt(numBins - 1));
//  ft->SetAxisRange(0.,4*TMath::Pi());
  ft->SetTitle("FFT");
  ft->SetMarkerStyle(20);
  ft->SetLineColor(4);
  ft->SetLineWidth(2);
  ft->Draw();
  ft->GetXaxis()->SetTitle("Frequency");
  ft->GetXaxis()->CenterTitle();
  ft->GetXaxis()->SetTitleOffset(1.2);
  ft->GetYaxis()->SetTitle("FFT amplitude");
  ft->GetYaxis()->CenterTitle();
  ft->GetYaxis()->SetTitleOffset(2);
  // NOTE: for "real" frequencies you have to divide the x-axes range with the range of your function 
  // (in this case 4*Pi); y-axes has to be rescaled by a factor of 1/SQRT(n) to be right: this is not done automatically!
   
  ft->SetStats(kFALSE);
//  ft->GetXaxis()->SetLabelSize(0.05);
//  ft->GetYaxis()->SetLabelSize(0.05);
  c->Modified();
//  c->Update();
  c->SaveAs("trial.eps");

  delete ft;
  delete hsin;
  delete c;
}

void Display::FFT(unsigned int numSamples, double *samples, double *amplitude, double *phase)
{
  double ftreal[numSamples];
  double ftimg[numSamples];
  for (int i = 0; i < numSamples; i++) {
    ftreal[i] = 0.;
    ftimg[i] = 0.;
    for (int j = 0; j < numSamples; j++) {
      ftreal[i] += samples[j]*cos((double(i*j)/numSamples)*2.*(M_PI));
      ftimg[i] -= samples[j]*sin((double(i*j)/numSamples)*2.*(M_PI));
    }
  }
  for (int i = 0; i < numSamples; i++) {
    amplitude[i] = sqrt(pow(ftreal[i],2) + pow(ftimg[i],2));
    phase[i] = atan(ftimg[i]/ftreal[i]);
  }
};

void Display::PlotFftAmplitude(unsigned int numSamples, double *samples, double xRange, DisplayStructures::GraphSettings *graphSettings)
{
  double amplitude[numSamples];
  double phase[numSamples];
  double xSamples[numSamples];
  FFT(numSamples, samples, amplitude, phase);
  for (int i = 0; i < numSamples; i++) {
    xSamples[i] = double(i*xRange)/(numSamples);
    amplitude[i] /= numSamples/2.;
  }
  PlotGraph(numSamples - 2, &xSamples[1], &amplitude[1], graphSettings);
};

double Display::GetSinAmplitude(unsigned int numBins, double *xBins, double *yBins)
{
  double params[4];
  double chisquare;
  Fit1D(numBins/100, xBins, yBins, 2, &chisquare, params);
  return params[3];
}

double Display::GetAmplitudeAverage(unsigned int numSamples, double *samples)
{
  return (GetMaxAverage(numSamples, samples) - GetMinAverage(numSamples, samples));
}

double Display::GetMaxAverage(unsigned int numSamples, double *samples)
{
  assert(numSamples > 4); // At least 5 points are needed to compute maxAverage

  unsigned int nMax = 0;
  double maxAverage = 0.;
  for (int i = 2; i < numSamples - 2; i++) {
    if ((samples[i - 2] < samples[i - 1]) && (samples[i - 1] < samples[i - 1]) && (samples[i] > samples[i + 1]) && (samples[i + 1] > samples[i + 2])) {
      maxAverage += samples[i];
      nMax++;
    }
  }
  if (nMax != 0) maxAverage /= nMax;
  else maxAverage = GetMax(numSamples, samples);
  return maxAverage;
}

double Display::GetMax(unsigned int numSamples, double *samples)
{
  double max = samples[1];
  for (int i = 2; i < numSamples; i++) {
    if (samples[i] > max) {
      max = samples[i];
    }
  }
  return max;
}

double Display::GetMinAverage(unsigned int numSamples, double *samples)
{
  assert(numSamples > 4); // At least 5 points are needed to compute minAverage

  unsigned int nMin = 0;
  double minAverage = 0.;
  for (int i = 2; i < numSamples - 2; i++) {
    if ((samples[i - 2] > samples[i - 1]) && (samples[i - 1] > samples[i - 1]) && (samples[i] < samples[i + 1]) && (samples[i + 1] < samples[i + 2])) {
      minAverage = samples[i];
      nMin++;
    }
  }
  if (nMin != 0) minAverage /= nMin;
  else minAverage = GetMin(numSamples, samples);
  return minAverage;
}

double Display::GetMin(unsigned int numSamples, double *samples)
{
  double min = samples[1];
  for (int i = 2; i < numSamples; i++) {
    if (samples[i] < min) {
      min = samples[i];
    }
  }
  return min;
}

double Display::GetDbValue(double value)
{
  assert(value > 0.); // logarithm argument connot be negative
  return 20*log10(value);
}

double *Display::GetDbValue(double scFactor, unsigned int arrayDepth, double *array)
{
  assert(array != NULL);
  assert(arrayDepth != 0);
  assert(scFactor != 0.);

  for (int i = 0; i < arrayDepth; i++) {
    assert(array[i] >= 0.);
    if (array[i] == 0.) array[i] += 1.;
    array[i] = 20. * log10(array[i]/scFactor);
  }

  return array;
}

double *Display::GetDbValue(unsigned int arrayDepth, double *array)
{
  assert(array != NULL);
  assert(arrayDepth != 0);

  double scFactor = (array[0] != 0.) ? array[0] : 1.;

  for (int i = 0; i < arrayDepth; i++) {
    assert(array[i] >= 0.);
    if (array[i] == 0.) array[i] += 1.;
    array[i] = 20. * log10(array[i]/scFactor);
  }

  return array;
}

double *Display::Scale(double scFactor, unsigned int arrayDepth, double *array)
{
  assert(array != NULL);
  assert(arrayDepth != 0);
  assert(scFactor != 0.);

  for (int i = 0; i < arrayDepth; i++) {
    array[i] /= scFactor;
  }

  return array;
}


double Display::GetxValue(double yValue, unsigned int arrayDepth, double *xArray, double *yArray)
{
  assert((xArray != NULL) && (yArray != NULL)); // Pointers to array must not be NULL
  assert(arrayDepth != 0);
  
  double xValue = 0.;

  unsigned int iHigher = GetClosestHigher(yValue, arrayDepth, yArray);
  unsigned int iLower = GetClosestLower(yValue, arrayDepth, yArray);
  
  if (iHigher == iLower) {
    xValue = xArray[iHigher];
  }
  else {
//    assert(iHigher == iLower + 1);
    double slope = (xArray[iHigher] - xArray[iLower])/(yArray[iHigher] - yArray[iLower]);
    xValue = (slope > 0) ? (slope * (yValue - yArray[iLower]) + xArray[iLower]) : (slope * (yValue - yArray[iHigher]) + xArray[iHigher]);
  }

  return xValue;
}

// Array must cross yValue only once to avoid ambiguities
double Display::GetClosestHigher(double yValue, unsigned int arrayDepth, double *yArray)
{
  assert(yArray != NULL);
  assert(arrayDepth != 0);

  unsigned int iHigher = 0;
//  bool slope = (yArray[arrayDepth - 1] - yArray[0]) > 0.;

  if ((yValue >= yArray[arrayDepth - 1]) && (yValue >= yArray[0])) return 0;
  if ((yValue <= yArray[arrayDepth - 1]) && (yValue <= yArray[0])) return (arrayDepth - 1);

//  assert(slope ? (yArray[0] <= yValue && yArray[arrayDepth - 1] >= yValue) : (yArray[0] >= yValue && yArray[arrayDepth - 1] <= yValue));
  for (int i = 0; i < arrayDepth; i++) {
    if (yArray[i] <= yValue) {
      iHigher = i;
      break;
    }
  }
  return iHigher;
}

// Array must cross yValue only once to avoid ambiguities
double Display::GetClosestLower(double yValue, unsigned int arrayDepth, double *yArray)
{
  assert(yArray != NULL);
  assert(arrayDepth != 0);

  unsigned int iLower = 0;
//  bool slope = (yArray[arrayDepth - 1] - yArray[0]) > 0.;

  if ((yValue >= yArray[arrayDepth - 1]) && (yValue >= yArray[0])) return 0;
  if ((yValue <= yArray[arrayDepth - 1]) && (yValue <= yArray[0])) return (arrayDepth - 1);

//  assert(slope ? (yArray[0] <= yValue && yArray[arrayDepth - 1] >= yValue) : (yArray[0] >= yValue && yArray[arrayDepth - 1] <= yValue));
  for (int i = 0; i < arrayDepth; i++) {
    if (yArray[arrayDepth - i - 1] >= yValue) {
      iLower = arrayDepth - i - 1;
      break;
    }
  }
  return iLower;
}

unsigned int PlotGraph(unsigned int numOfPoints, double* dataX, double* dataY)
{
  TCanvas *c = new TCanvas("c","c",1000,1000);
//  TMultiGraph* mg = new TMultiGraph();
  TGraph* graph = new TGraph(numOfPoints,dataX,dataY);
  TLegend* leg = new TLegend(0.65, 0.9, 0.9, 0.75);
  graph->SetMarkerStyle(20);
  graph->SetLineColor(4);
  graph->SetLineWidth(2);
//  mg->Add(graph);
  leg->AddEntry(graph, "graph", "l");
  graph->Draw("APL");
  graph->GetXaxis()->SetTitle("Output voltage [V]");
  graph->GetXaxis()->CenterTitle();
  graph->GetXaxis()->SetTitleOffset(1.2);
//  mg->GetXaxis()->SetLabelOffset(0.005);
  graph->GetYaxis()->SetTitle("Input voltage [V]");
  graph->GetYaxis()->CenterTitle();
  graph->GetYaxis()->SetTitleOffset(1.2);
//  mg->GetYaxis()->SetLabelOffset(0.005);
//  mg->SetMaximum(2.2);
  leg->Draw();

  c->Update();
  c->SaveAs("Plot.eps");
  delete c;

  return 0;
}

*/

void Display::PlotMultiGraph(double **dataX, double **dataY)
{
  TCanvas *c = new TCanvas("c","c",1000, 1000);
  c->SetLeftMargin(0.15);  // This avoids the Y title to be cut

  TMultiGraph* mg = new TMultiGraph(); 
  TGraph* graph[displayStructures->numberOfGraphsToPlot];
  unsigned int colors[] = {1, 2, 3, 4, 6, 7, 8, 9, 16, 28, 30, 38, 40, 41, 42, 46, 49};

  TLegend* leg = new TLegend(displayStructures->graphSettings[0]->legend->coordinates[0], displayStructures->graphSettings[0]->legend->coordinates[1],
                             displayStructures->graphSettings[0]->legend->coordinates[2], displayStructures->graphSettings[0]->legend->coordinates[3]);

  for (unsigned int i = 0; i < displayStructures->numberOfGraphsToPlot; i++){
    graph[i] = new TGraph(displayStructures->numberOfPointsPerGraph, dataX[i], dataY[i]);
    graph[i]->SetMarkerStyle(20);
    graph[i]->SetLineColor(colors[i]);
    graph[i]->SetLineWidth(1);
    mg->Add(graph[i]);
    leg->AddEntry(graph[i], displayStructures->graphSettings[i]->legend->entryName, "l");
  }
  
  mg->Draw("AL*");
  mg->GetXaxis()->SetTitle(displayStructures->graphSettings[0]->xAxis->label);
  mg->GetXaxis()->CenterTitle();
  mg->GetXaxis()->SetTitleOffset(1.2);
  mg->GetXaxis()->SetLabelSize(0.03);
//  mg->GetXaxis()->SetLabelOffset(0.005);
  mg->GetYaxis()->SetTitle(displayStructures->graphSettings[0]->yAxis->label);
  mg->GetYaxis()->CenterTitle();
  mg->GetYaxis()->SetTitleOffset(1.2);
  mg->GetYaxis()->SetLabelOffset(0.005);
  mg->GetYaxis()->SetLabelSize(0.03);
  mg->SetMaximum(displayStructures->graphSettings[0]->yAxis->maximum);
//  mg->SetMaximum(3.5);
  mg->SetMinimum(0);
  leg->Draw();
  c->Update();
  c->SaveAs(displayStructures->graphSettings[0]->graphName);
  delete c;
}

