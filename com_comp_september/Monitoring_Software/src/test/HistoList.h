#ifndef __histoList__hh
#define __histoList__hh

#include <Rtypes.h>

class TList;
class TH1;
class TH1F;
class TGraph;
class TGraphErrors;
class TGraphBentErrors;
class TMultiGraph;
class TPaveText;
class TLine;

namespace HistoUtils
{
  static Double_t _textSize;

  void setTextSize(Double_t v);

  TList* addHistogram(const char *hname, 
		      const char *hfile,
		      const char *descr,
		      TList *hlist,
		      Double_t norm = 1.);
  
  void drawBar(Double_t x1, Double_t x2, Double_t y1, Double_t y2);

  TLine* drawLine(Double_t x1, Double_t x2, Double_t y1, Double_t y2,
		  Double_t width = 3.);

  void resetMin(TList *hlist, Double_t min);

  void resetMax(TList *hlist, Double_t max);

  void normalize(TList *hlist, Double_t norm = 0);

  void changeAxisTitles(TList *hlist, 
			const char *xtitle, 
			const char *ytitle = 0,
			const char *ztitle = 0);

  void Sumw2(TList *hlist);

  void Rebin(TList *hlist, Int_t irebin);

  void drawHistograms(TList *hlist, const char* opt ="", 
		      Double_t fixmin = -9999.9999,
		      Double_t fixmax = 9999.9999);
  void colorize(TList *hlist, Int_t mild = 0);

  void stylizeLine(TList *hlist, Int_t istyle);

  void markerize(TList *hlist, Int_t mset = 0, Double_t ms = 0.9);
		     
  void drawLegend(TList *hlist, const char *opt = "L",
		  Double_t x1 = 0.6, Double_t y1 = 0.6,
		  Double_t x2 = 0.95, Double_t y2 = 0.95,
		  Bool_t useGpad = kTRUE);

  void setLineWidth(TList *hlist, Double_t w);

  void zoomAxis(TH1* h, Int_t ax, Double_t low, Double_t hi);

  void zoomAxis(TList *hlist, Int_t ax, Double_t low, Double_t hi);

  void fixMinimums(TList *hlist, Double_t low);

  Double_t getMinContent(TH1 *obj, Bool_t ignoreZero);
  Double_t getMinContent(TList *hlist, Bool_t ignoreZero);
  Double_t getMaxContent(TH1 *obj, Bool_t ignoreZero);
  Double_t getMaxContent(TList *hlist, Bool_t ignoreZero);


  TPaveText* drawComment(const char* comment,
			 Double_t x1 = 0.35, Double_t y1 = 0.76, 
			 Double_t x2 = 0.85, Double_t y2 = 0.94);

  TH1F* rebinResol(TH1* h);

  void getValWithin(TH1* h, Double_t low, Double_t high, 
		    Double_t &xval, 
		    Double_t &yval,
		    Double_t &yerr);

  TGraphBentErrors *rebinResolGraph(TH1* h, Double_t fracCorr = 0., 
				    Double_t lowptcutoff = 0,
				    Bool_t ignoreErr = kFALSE);
  TGraphErrors *rebinResolGraphErr(TH1* h, Double_t fracCorr = 0., 
				   Double_t lowptcutoff = 0,
				   Double_t errFrac = 0.1);

  void drawList(TList *hlist, const char *opt, Double_t ymin, const char* comm = 0);

  TMultiGraph *makeMultiGraphFromList(TList *hlist, 
				      const char *name = "genericName",
				      const char *title = "genericTitle;X;Y",
				      Double_t fracCorr = 0., 
				      Double_t lowptcutoff = 0,
				      Bool_t ignoreErr = kFALSE);

  TMultiGraph *makeMultiGraphFromListErr(TList *hlist, 
					 const char *name = "genericName",
					 const char *title = "genericTitle;X;Y",
					 Double_t fracCorr = 0., 
					 Double_t lowptcutoff = 0,
					 Double_t errfac = 0.1);

  TGraph *pointGraph(Double_t px = 0);

  TMultiGraph* drawListGraph(TList *hlist, const char *opt, 
			     Double_t ymin, Double_t ymax,
			     Double_t xmin = 0, Double_t xmax = 0,
			     Double_t lowptcutoff = 0,
			     Double_t shiftFrac = 0,
			     Double_t systErrFrac = 0,
			     const char* comm = 0);

  void drawErrBandTest(TH1* h, Double_t val);

  TH1* drawErrBand(TH1* h, Double_t val, Option_t *opt = "same");

  TGraphErrors *divideGraphs(TGraphErrors *g1, 
			     TGraphErrors *g2, Double_t scale1 = 1.);

  TMultiGraph *divideMultiGraph(TMultiGraph *mgr1, TMultiGraph *mgr2, 
				Int_t imax = 4,
				const char *cmod = "ratio: ",
				Double_t scale1 = 1.);

  // TH1 *divideGraphs(TGraph *g1, TGraph *g2);
  TList *drawFile(const char *fname,
		  const char *pattern = 0,
		  const char *patternExlude = 0,
		  const char *opt = "LP", 
		  Double_t ymin = -9999.9999, 
		  const char* comm = 0,
		  Bool_t newCanvas = kTRUE);
  

  TList *getSlices(const char *fname, const char *hname,
		   Int_t nslices = 0, Double_t *range = 0,
		   Int_t axis = 0, Int_t startbin = 1);
};
#endif
