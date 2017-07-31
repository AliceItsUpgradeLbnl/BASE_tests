#include "HistoList.h"

#include <TFile.h>
#include <TList.h>
#include <TH1.h>
#include <TH1F.h>
#include <TH2.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TGraphBentErrors.h>
#include <TMultiGraph.h>
#include <TAxis.h>
#include <TPaveText.h>
#include <TPad.h>
#include <TLegend.h>
#include <TMath.h>
#include <TKey.h>
#include <TClass.h>
#include <iostream>
#include <TCanvas.h>
#include <TLine.h>
#include <TROOT.h>

using namespace std;

namespace HistoUtils
{

  TFile *openFile(const char *fname)
  {
    TFile *fin = (TFile*)gROOT->GetListOfFiles()->FindObject(fname);
    if (fin == 0)
      fin = TFile::Open(fname);
    return fin;
  }

  void setTextSize(Double_t v) {_textSize = v;}

  TList* addHistogram(const char *hname, 
		      const char *hfile,
		      const char *descr,
		      TList *hlist,
		      Double_t norm)
  {
    if (hlist == 0)
      {
	hlist = new TList();
	hlist->SetOwner(kTRUE);
      }

    TFile *fin = openFile(hfile);
    TH1* h = (TH1*)fin->Get(hname);
  
    if (h == 0)
      {
	cerr << "Unable to get the histogram " << hname 
	     << " from " << hfile
	     << endl;
	return hlist;
      }
  
    h->SetTitle(descr);

    if (norm != 0)
      {
	h->Scale(1./norm);
      }
    else
      {
	norm = h->Integral();
	if (norm != 0)
	  h->Scale(1./norm);
      }

    hlist->Add(h);
    return hlist;
  }

  void drawBar(Double_t x1, Double_t x2, Double_t y1, Double_t y2)
  {
    TBox *box = new TBox(x1, x2, y1, y2);
    box->SetFillColor(kYellow);
    box->Draw();
  }

  TLine* drawLine(Double_t x1, Double_t x2, Double_t y1, Double_t y2,
		  Double_t width)
  {
    TLine *line = new TLine(x1, x2, y1, y2);
    line->SetLineWidth(Width_t(width));
    line->Draw();
    return line;
  }

  void resetMin(TList *hlist, Double_t min)
  {
    TH1 *h = 0;
    TIter it(hlist);  
    it.Reset();
    while((h = (TH1*)it.Next()) != 0)
      {
	h->SetMinimum(min);
      }
  }

  void resetMax(TList *hlist, Double_t max)
  {
    TH1 *h = 0;
    TIter it(hlist);  
    it.Reset();
    while((h = (TH1*)it.Next()) != 0)
      {
	h->SetMaximum(max);
      }
  }

  void normalize(TList *hlist, Double_t norm)
  {
    TH1 *h = 0;
    TIter it(hlist);  
    it.Reset();
    Double_t _norm = 0;
    while((h = (TH1*)it.Next()) != 0)
      {
	if (norm == 0)
	  _norm = h->Integral();
	else
	  _norm = norm;

	if (_norm == 0)
	  _norm = 1;

	h->Scale(1./_norm);
      }
  }

  void changeAxisTitles(TList *hlist, 
			const char *xtitle, 
			const char *ytitle,
			const char *ztitle)
  {
    for (Int_t i = 0; i < hlist->GetEntries(); i++)
      {
	TH1 *h = (TH1 *)hlist->At(i);
	if (xtitle)
	  h->GetXaxis()->SetTitle(xtitle);
	if (ytitle)
	  h->GetYaxis()->SetTitle(ytitle);
	if (ztitle)
	  h->GetZaxis()->SetTitle(ztitle);
      } 
  }

  void Sumw2(TList *hlist)
  {
    for (Int_t i = 0; i < hlist->GetEntries(); i++)
      {
	TH1 *h = (TH1 *)hlist->At(i);
	h->Sumw2();
      }
  }

  void Rebin(TList *hlist, Int_t irebin)
  {
    for (Int_t i = 0; i < hlist->GetEntries(); i++)
      {
	TH1 *h = (TH1 *)hlist->At(i);
	h->Rebin(irebin);
      }
  }

  void drawHistograms(TList *hlist, const char* opt, 
		      Double_t fixmin,
		      Double_t fixmax)
  {
    TH1 *h = 0;
    TIter it(hlist);  

    Double_t max = 1e10;
    Double_t min = -1e10;

    h = (TH1*)hlist->At(0);
    max = h->GetMaximum();
    min = h->GetMinimum();

    it.Reset();
    while((h = (TH1*)it.Next()) != 0)
      {
	if (fixmax != 9999.9999)
	  {
	    max = fixmax;
	  }
	else
	  {
	    if (max < h->GetMaximum())
	      {
		max = h->GetMaximum();	  
	      }
	  }

	if (fixmin == -9999.9999)
	  {
	    if (min > h->GetMinimum())
	      {
		min = h->GetMinimum();	  
	      }      
	  }
	else
	  {
	    min = fixmin;
	  }
      }
  
    it.Reset();
    while((h = (TH1*)it.Next()) != 0)
      {
	h->SetMinimum(min);
	h->SetMaximum(max);
      }
  
    Int_t in = 0;
    it.Reset();
    while((h = (TH1*)it.Next()) != 0)
      {
	TString option = opt;
	if (in > 0)
	  option += "same";

	h->DrawCopy(option.Data());
	in++;
      }
  }

  void colorize(TList *hlist, Int_t mild)
  {
    Int_t maxCol = 20;
    Int_t icol[] = {kBlack, kRed, kBlue, kMagenta, 49, 30, 38, 8, 9, kGreen,
		    kBlack, kRed, kBlue, kMagenta, 49, 30, 38, 8, 9, kGreen};
    Int_t icolMild[] = {kBlack, 46, 9, 8, kRed, 30, 38, 49, kBlue, kMagenta, kGreen,
			kBlack, 46, 9, 8, kRed, 30, 38, 49, kBlue, kMagenta, kGreen};
    TH1 *h = 0;
    TIter it(hlist);  
    Int_t in = 0;
    Int_t currentCol = 0;
    while((h = (TH1*)it.Next()) != 0)
      {
	if (currentCol > maxCol)
	  currentCol = 0;
	Int_t col = icolMild[currentCol];
	if (mild == 0)
	  col = icol[currentCol];
	h->SetLineColor(col);
	h->SetMarkerColor(col);
	in++;
	currentCol++;
      }
  }

  void stylizeLine(TList *hlist, Int_t istyle)
  {
    TH1 *h = 0;
    TIter it(hlist);  
    Int_t in = 0;
    while((h = (TH1*)it.Next()) != 0)
      {
	h->SetLineStyle(istyle);
	in++;
      }
  }

  void markerize(TList *hlist, Int_t mset, Double_t ms)
  {
    Int_t imarkSolid[] = {20, 21, 22, 23, 29, 20, 21, 22, 23, 29, 20,
			  21, 22, 23, 29, 20, 21, 22, 23, 29, 20, 21};
    Int_t imarkOpen[] =  {24, 25, 26, 27, 28, 30, 24, 25, 26, 27, 28, 
			  30, 24, 25, 26, 27, 28, 30, 24, 25, 26, 27};
    Int_t imarkMixed[] = {20, 24, 21, 25, 22, 26, 23, 27, 29, 28, 30,
			  20, 24, 21, 25, 22, 26, 23, 27, 29, 28, 30};
			
    TH1 *h = 0;
    TIter it(hlist);  
    Int_t in = 0;
    while((h = (TH1*)it.Next()) != 0)
      {
	h->SetMarkerSize(ms);
	h->SetMarkerStyle(imarkMixed[in]);
	if (mset < 0)
	  h->SetMarkerStyle(imarkOpen[in]);
	if (mset > 0)
	  h->SetMarkerStyle(imarkSolid[in]);
	in++;
      }
  }
		     
  void drawLegend(TList *hlist, const char *opt,
		  Double_t x1, Double_t y1,
		  Double_t x2, Double_t y2,
		  Bool_t useGpad)
  {
    if (gPad != 0 && useGpad)
      {
	TPad *gp = (TPad*)gPad;
	//TLegend *leg = gPad->BuildLegend(x1, y1, x2, y2);
	TLegend *leg = gp->BuildLegend(x1, y1, x2, y2);
	leg->SetBorderSize(0);
	leg->SetFillColor(kWhite);  
	//leg->SetTextSize(0.035);
	leg->SetTextSize(HistoUtils::_textSize);
	//cout << HistoUtils::_textSize << endl;
      }
    else
      {
	TLegend *leg = new TLegend(x1, y1, x2, y2);      
	leg->SetBorderSize(0);
	leg->SetFillColor(kWhite);  
	//leg->SetTextSize(0.03);
	leg->SetTextSize(HistoUtils::_textSize);
	//leg->SetTextSize(0.04);
	//leg->SetTextSize(0.050);
	//leg->SetTextAlign(22); //center-center
	leg->SetTextAlign(32); //center-center
	TNamed *obj = 0;
	TIter it(hlist);  
	while((obj = (TNamed*)it.Next()) != 0)
	  leg->AddEntry(obj, obj->GetTitle(), opt);      
	leg->Draw();
      }
  }

  void setLineWidth(TList *hlist, Double_t w)
  {
    TH1 *obj = 0;
    TIter it(hlist);  
    while((obj = (TH1*)it.Next()) != 0)
      obj->SetLineWidth(Width_t(w));
  }

  void zoomAxis(TH1* h, Int_t ax, Double_t low, Double_t hi)
  {
    TAxis* axis = h->GetXaxis();
    if (ax == 1)
      axis = h->GetYaxis();
    if (ax == 2)
      axis = h->GetZaxis();

    axis->SetRange(axis->FindBin(low), axis->FindBin(hi));
  }

  void zoomAxis(TList *hlist, Int_t ax, Double_t low, Double_t hi)
  {
    TH1 *obj = 0;
    TIter it(hlist);  
    while((obj = (TH1*)it.Next()) != 0)
      zoomAxis(obj, ax, low, hi);
  }

  void fixMinimums(TList *hlist, Double_t low)
  {
    TH1 *obj = 0;
    TIter it(hlist);  
    while((obj = (TH1*)it.Next()) != 0)
      {
	if (obj->GetMinimum() <= 0)
	  obj->SetMinimum(low);
      }
  }

  Double_t getMinContent(TH1 *obj, Bool_t ignoreZero)
  {
    Double_t minval = 0;
    Int_t iter = 0;
    for(Int_t i = 1; i <= obj->GetNbinsX(); i++)
      {
	Double_t bc = obj->GetBinContent(i);
	if (bc < minval || iter == 0)
	  {
	    if (ignoreZero ==kFALSE )
	      {
		minval = bc;
		iter++;
	      }
	    else
	      {
		if (bc != 0)
		  {
		    minval = bc;
		    iter++;
		  }
	      }
	  }
      }
    return minval;
  }
  
  Double_t getMinContent(TList *hlist, Bool_t ignoreZero)
  {
    TH1 *obj = 0;
    TIter it(hlist);  
    Double_t minval = 0;
    Int_t iter = 0;
    while((obj = (TH1*)it.Next()) != 0)
      {
	Double_t tmp = getMinContent(obj, ignoreZero);
	if (tmp < minval || iter == 0)
	  minval = tmp;
	iter++;
      }
    return minval;
  }

  Double_t getMaxContent(TH1 *obj, Bool_t ignoreZero)
  {
    Double_t maxval = 0;
    Int_t iter = 0;
    for(Int_t i = 1; i <= obj->GetNbinsX(); i++)
      {
	Double_t bc = obj->GetBinContent(i);
	if (bc > maxval || iter == 0)
	  {
	    if (ignoreZero == kFALSE )
	      {
		maxval = bc;
		iter++;
	      }
	    else
	      {
		if (bc != 0)
		  {
		    maxval = bc;
		    iter++;
		  }
	      }
	  }
      }
    return maxval;
  }
  
  Double_t getMaxContent(TList *hlist, Bool_t ignoreZero)
  {
    TH1 *obj = 0;
    TIter it(hlist);  
    Double_t maxval = 0;
    Int_t iter = 0;
    while((obj = (TH1*)it.Next()) != 0)
      {
	Double_t tmp = getMinContent(obj, ignoreZero);
	if (tmp > maxval || iter == 0)
	  maxval = tmp;
	iter++;
      }
    return maxval;
  }

  TPaveText* drawComment(const char* comment,
			 Double_t x1, Double_t y1, 
			 Double_t x2, Double_t y2)
  {
    TPaveText *tx = new TPaveText(x1, y1, x2, y2, "brNDC");
    tx->SetBorderSize(0);
    tx->AddText(comment);
    tx->SetFillColor(kWhite);
    tx->Draw();
    return tx;
  }

  TH1F* rebinResol(TH1* h)
  {
    TString sretname = h->GetName();
    sretname += "_rebinned";

    Double_t ptLowBins[] = {0, 1, 2, 3, 5, 7, 9, 11, 13, 16, 19, 23, 28, 34, 41, 49, 59, 72};
    Int_t ibins = 18;
    TH1F* hret = new TH1F(sretname.Data(), sretname.Data(), ibins-1, &ptLowBins[0]);
    hret->GetXaxis()->SetTitle(h->GetXaxis()->GetTitle());
    hret->GetYaxis()->SetTitle(h->GetYaxis()->GetTitle());

    for (Int_t i = 1; i < h->GetNbinsX(); i++)
      {
	hret->Fill(h->GetBinCenter(i), h->GetBinContent(i));
      }

    for (Int_t i = 1; i < hret->GetNbinsX(); i++)
      {      
	Double_t ic = hret->GetBinContent(i);
	if (ic > 0)
	  {
	    hret->SetBinError(i, 1./TMath::Sqrt(hret->GetBinContent(i)));
	    hret->SetBinContent(i, ic/hret->GetBinWidth(i));
	  }
      }
    hret->SetLineColor(h->GetLineColor());
    return hret;
  }

  void getValWithin(TH1* h, Double_t low, Double_t high, 
		    Double_t &xval, 
		    Double_t &yval,
		    Double_t &yerr)
  {
    if (high - low <= 0 )
      {
	cout << "Wrong limts" << endl;
      }
    xval = 0;
    yval = 0;
    yerr = 0;
    Double_t iw = 0;
    for (Int_t i = 1; i < h->GetNbinsX(); i++)
      {
	if (h->GetBinCenter(i) > low &&
	    h->GetBinCenter(i) < high)
	  {
	    if (h->GetBinContent(i) != 0)
	      {
		yval += h->GetBinContent(i);
		yerr += h->GetBinError(i) * h->GetBinContent(i);
		xval += h->GetBinCenter(i) * h->GetBinContent(i);
		iw += 1.;
	      }
	  }
      }

    if (yval != 0)
      {
	yerr /= yval;
	xval /= yval;
	//yval /= (high-low);
	if (iw != 0)
	  yval /= iw;
      }
  }

  TGraphBentErrors *rebinResolGraph(TH1* h, Double_t fracCorr, 
				    Double_t lowptcutoff,
				    Bool_t ignoreErr)
  {
    Double_t binx[20] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 12, 14, 17, 20, 24, 29, 35, 42, 50, 60};
    //Double_t binx[20] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 12, 14, 17, 20, 24, 29, 35, 50, 60, 80};

    Double_t x[20];
    Double_t y[20];

    Double_t exl[20];
    Double_t exh[20];
    Double_t eyl[20];
    Double_t eyh[20];

    Int_t igrbin = 0;
    for (Int_t i = 1; i < 20; i++)
      {
	//cout << "bin " << i << " " << binx[i-1] << " : " << binx[i] << endl;
	Double_t xval = 0;
	Double_t yval = 0;
	Double_t yerr = 0;
	getValWithin(h, binx[i-1], binx[i], xval, yval, yerr);
	if (yval != 0 && xval > lowptcutoff)
	  {
	    //cout << xval << " : " << yval << endl;
	    x[igrbin] = xval;
	    y[igrbin] = yval;
	    if (ignoreErr)
	      {
		exl[igrbin] = xval - binx[i-1];
		exh[igrbin] = binx[i] - xval;
		eyh[igrbin] = 1e-20;
		eyl[igrbin] = 1e-20;
	      }
	    else
	      {
		exl[igrbin] = xval - binx[i-1];
		exh[igrbin] = binx[i] - xval;
		eyh[igrbin] = yerr;
		eyl[igrbin] = yerr;
	      }
	    igrbin++;
	  }
      }
    //igrbin--;

    //cout << "gr bins " << igrbin << endl;
    TGraphBentErrors *gr = new TGraphBentErrors(igrbin, &x[0], &y[0],
						&exl[0], &exh[0], &eyl[0], &eyh[0]);

    gr->SetTitle(h->GetTitle());

    gr->SetFillColor(h->GetFillColor());
    gr->SetFillStyle(h->GetFillStyle());

    gr->SetMarkerColor(h->GetMarkerColor());
    gr->SetMarkerStyle(h->GetMarkerStyle());
    gr->SetMarkerSize(h->GetMarkerSize());

    gr->SetLineColor(h->GetLineColor());
    gr->SetLineStyle(h->GetLineStyle());
    gr->SetLineWidth(h->GetLineWidth());

    //if (gr->GetMinimum() > h->GetMinimum())
    gr->SetMinimum(0.9*h->GetMinimum());
    //if (gr->GetMaximum() < h->GetMaximum())
    gr->SetMaximum(1.1*h->GetMaximum());
    gr->GetXaxis()->SetTitle(h->GetXaxis()->GetTitle());
    gr->GetYaxis()->SetTitle(h->GetYaxis()->GetTitle());

    if (fracCorr > 1e-4)
      {
	//cout << "hello ?" << endl;
	Double_t *xvals = gr->GetX();
	for (Int_t i = 0; i < gr->GetN(); i++)
	  {
	    xvals[i] = xvals[i] + xvals[i] * fracCorr;
	  }
      }

    gr->SetLineWidth(2);
      
    return gr;
  }

  TGraphErrors *rebinResolGraphErr(TH1* h, Double_t fracCorr, 
				   Double_t lowptcutoff,
				   Double_t errFrac)
  {
    Double_t binx[20] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 12, 14, 17, 20, 24, 29, 35, 42, 50, 60}; //std
    //Double_t binx[20] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 12, 14, 17, 20, 24, 29, 35, 50, 60, 70}; //new

    //Double_t binx[20] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 12, 14, 17, 20, 24, 29, 35, 50, 60, 80};

    Bool_t ignoreErr = kFALSE;

    Double_t x[20];
    Double_t y[20];

    Double_t ex[20];
    Double_t ey[20];

    Int_t igrbin = 0;
    for (Int_t i = 1; i < 20; i++)
      {
	//cout << "bin " << i << " " << binx[i-1] << " : " << binx[i] << endl;
	Double_t xval = 0;
	Double_t yval = 0;
	Double_t yerr = 0;
	getValWithin(h, binx[i-1], binx[i], xval, yval, yerr);
	if (yval != 0 && xval > lowptcutoff)
	  {
	    //cout << xval << " : " << yval << endl;
	    x[igrbin] = xval;
	    y[igrbin] = yval;
	    ex[igrbin] = 0;//xval - binx[i-1];
	    ey[igrbin] = errFrac * yval;
	    igrbin++;
	  }
      }
    //igrbin--;

    //cout << "gr bins " << igrbin << endl;
    TGraphErrors *gr = new TGraphErrors(igrbin, &x[0], &y[0],
					&ex[0], &ey[0]);
    gr->SetMarkerColor(h->GetMarkerColor());
    gr->SetMarkerStyle(h->GetMarkerStyle());
    gr->SetMarkerSize(h->GetMarkerSize());

    gr->SetLineColor(h->GetLineColor());
    gr->SetLineStyle(h->GetLineStyle());
    gr->SetLineWidth(h->GetLineWidth());

    //if (gr->GetMinimum() > h->GetMinimum())
    gr->SetMinimum(0.9*h->GetMinimum());
    //if (gr->GetMaximum() < h->GetMaximum())
    gr->SetMaximum(1.1*h->GetMaximum());
    gr->GetXaxis()->SetTitle(h->GetXaxis()->GetTitle());
    gr->GetYaxis()->SetTitle(h->GetYaxis()->GetTitle());

    if (fracCorr > 1e-4)
      {
	//cout << "hello ?" << endl;
	Double_t *xvals = gr->GetX();
	for (Int_t i = 0; i < gr->GetN(); i++)
	  {
	    xvals[i] = xvals[i] + xvals[i] * fracCorr;
	  }
      }

    //gr->SetFillColor(42);
    //gr->SetFillStyle(1001);
    //gr->SetMarkerSize(0);
    //gr->SetLineWidth(0);

    gr->SetLineWidth(2);

    return gr;
  }

  void drawList(TList *hlist, const char *opt, Double_t ymin, const char* comm)
  {
    colorize(hlist);
    markerize(hlist);
    drawHistograms(hlist, opt, ymin);
    //drawLegend(hlist, opt);
    drawLegend(hlist, opt, 0.468593,0.6282383,0.718593,0.8782383);//unfold qm
    if (comm)
      drawComment(comm);
  }

  TMultiGraph *makeMultiGraphFromList(TList *hlist, 
				      const char *name,
				      const char *title,
				      Double_t fracCorr,
				      Double_t lowptcutoff,
				      Bool_t ignoreErr)
  {
    TMultiGraph* mgr = new TMultiGraph(name, title);
    TGraphBentErrors *gr = 0;
    for (Int_t i = 0; i < hlist->GetEntries(); i++)
      {
	TH1* h = (TH1*)hlist->At(i);
	gr = rebinResolGraph(h, fracCorr, lowptcutoff, ignoreErr);
	mgr->Add(gr);
      }
    return mgr;
  }

  TMultiGraph *makeMultiGraphFromListErr(TList *hlist, 
					 const char *name,
					 const char *title,
					 Double_t fracCorr,
					 Double_t lowptcutoff,
					 Double_t errfac)
  {
    TMultiGraph* mgr = new TMultiGraph(name, title);
    //TGraphBentErrors *gr = 0;
    TGraphErrors *gr = 0;
    for (Int_t i = 0; i < hlist->GetEntries(); i++)
      {
	TH1* h = (TH1*)hlist->At(i);
	gr = rebinResolGraphErr(h, fracCorr, lowptcutoff, errfac);
	mgr->Add(gr);
      }
    return mgr;
  }

  TGraph *pointGraph(Double_t px)
  {
    Double_t x[] = {0};
    Double_t y[] = {1e-10};

    x[0] = px;

    TGraph *gr = new TGraph(1, x, y);
    return gr;
  }

  TMultiGraph* drawListGraph(TList *hlist, const char *opt, 
			     Double_t ymin, Double_t ymax,
			     Double_t xmin, Double_t xmax,
			     Double_t lowptcutoff,
			     Double_t shiftFrac,
			     Double_t systErrFrac,
			     const char* comm)
  {
    TMultiGraph *mgr = makeMultiGraphFromList(hlist, opt, opt, shiftFrac, lowptcutoff, kFALSE);
    mgr->Add(pointGraph(xmin));
    mgr->Add(pointGraph(xmax));
    mgr->SetMaximum(ymax);
    mgr->SetMinimum(ymin);
    mgr->Draw("AP");
    if (systErrFrac > 0)
      {
	for (Int_t i = 0; i < hlist->GetEntries(); i++)
	  {
	    TH1 *h = (TH1*)hlist->At(i);
	    TGraphErrors *systErr = rebinResolGraphErr(h, shiftFrac, 
						       lowptcutoff,
						       systErrFrac);
	    //systErr->Draw("[]");
	    //systErr->Draw("4");
	    systErr->SetFillColor(h->GetLineColor());
	    systErr->SetFillStyle(0);
	    systErr->Draw("3");
	    //mgr->Add(systErr);
	  }
      }
    mgr->Draw("P");

    if (comm)
      drawComment(comm);

    return mgr;
  }

  void drawErrBandTest(TH1* h, Double_t val)
  {
    TString newname = h->GetName();
    newname += "_hup";
    TH1 *hup   = (TH1*)h->Clone(newname.Data());
    newname = h->GetName();
    newname += "_hdown";
    TH1 *hdown = (TH1*)h->Clone(newname.Data());
  
    for (Int_t ibin = 1; ibin < h->GetNbinsX(); ibin++)
      {
	hup->SetBinContent(ibin, h->GetBinContent(ibin) + val * h->GetBinContent(ibin));
	hdown->SetBinContent(ibin, h->GetBinContent(ibin) - val * h->GetBinContent(ibin));
      }
    hup->DrawCopy("LS");
    hdown->DrawCopy("LS");
  }

  TH1* drawErrBand(TH1* h, Double_t val, Option_t *opt)
  {
    TString newname = h->GetName();
    newname += "_herrSyst";
    TH1 *herrSyst   = (TH1*)h->Clone(newname.Data());
    for (Int_t ibin = 1; ibin < h->GetNbinsX(); ibin++)
      {
	herrSyst->SetBinError(ibin, h->GetBinContent(ibin) + val * h->GetBinContent(ibin));
      }
    herrSyst->SetFillColor(42);
    herrSyst->SetFillStyle(kSolid);
    herrSyst->SetMarkerSize(0);
    herrSyst->SetLineWidth(0);
    herrSyst->DrawCopy(opt);
    return herrSyst;
  }

  TGraphErrors *divideGraphs(TGraphErrors *g1, 
			     TGraphErrors *g2, Double_t scale1)
  {
    TString sresultName = g1->GetName();
    sresultName += "_div_";
    sresultName += g2->GetName();

    TGraphErrors *result = (TGraphErrors*)g1->Clone(sresultName.Data());
    Double_t *ix = result->GetX();
    Double_t *iy = result->GetY();
    for (Int_t i= 0; i < g1->GetN(); i++)
      {
	ix[i] = g1->GetX()[i];
	if ((g2->GetY()[i]) != 0)
	  {
	    iy[i] = g1->GetY()[i] * scale1 / g2->GetY()[i];
	    if (g1->GetErrorY(i) > 0 && g2->GetErrorY(i) > 0)
	      {
		result->SetPointError(i, 0, 0);
	      }
	    else
	      {
		result->SetPointError(i, 0, 0);
	      }
	  }
	else
	  iy[i] = 0;
      }
    return result;
  }

  TMultiGraph *divideMultiGraph(TMultiGraph *mgr1, TMultiGraph *mgr2, 
				Int_t imax,
				const char *cmod,
				Double_t scale1)
  {
    TString sresultName = mgr1->GetName();
    sresultName += "_div_";
    sresultName += mgr2->GetName();
  
    TMultiGraph *ret = new TMultiGraph(sresultName.Data(), sresultName.Data());
    if (imax > mgr1->GetListOfGraphs()->GetEntries())
      imax = mgr1->GetListOfGraphs()->GetEntries();
    for (Int_t i = 0; 
	 i < imax;
	 i++)
      { 
	TGraphErrors *gr0 = (TGraphErrors*)mgr1->GetListOfGraphs()->At(i);
	TGraphErrors *gr1 = (TGraphErrors*)mgr2->GetListOfGraphs()->At(i);
	if (gr0 && gr1)
	  {
	    TGraphErrors *gr = divideGraphs(gr0, gr1, scale1);
	    TString snewTitle = Form("%s: %s / %s", cmod, 
				     gr0->GetTitle(),
				     gr1->GetTitle());
	    gr->SetTitle(snewTitle.Data());
	    ret->Add(gr);
	  }
      }
    return ret;
  }

  // TH1 *divideGraphs(TGraph *g1, TGraph *g2)
  // {
  //   TString sresultName = g1->GetName();
  //   sresultName += "_div_";
  //   sresultName += g2->GetName();
  
  //   TH1 *result = g1->GetHistogram()->Clone(sresultName.Data());
  //   //result->Reset("ICE");
  //   result->Divide(g2->GetHistogram());
  //   return result;
  // }

  TList *drawFile(const char *fname,
		  const char *pattern,
		  const char *patternExlude,
		  const char *opt, Double_t ymin, const char* comm,
		  Bool_t newCanvas)
  {
    TFile *fin = TFile::Open(fname);
    TList *keys = fin->GetListOfKeys();
    TIter it(keys);
    TKey *key = 0;
    TObject *obj = 0;
    TList *histograms = new TList();
    
    while ((key = (TKey*)it.Next()) != 0)
      {
	obj = (TObject*)fin->Get(key->GetName());
	if (obj == 0)
	  {
	    cerr << Form("Strange key %s but no object!", key->GetName()) << endl;
	    continue;
	  }
	TString str = obj->IsA()->GetName();
	if (obj->IsA()->InheritsFrom(TH1::Class()))
	  {
	    if (pattern != 0)
	      {
		TString hn = obj->GetName();
		if (hn.Contains(pattern))
		  {
		    if (patternExlude != 0)
		      {
			if (hn.Contains(patternExlude) == kFALSE)
			  histograms->Add(obj);
		      }
		    else
		      {
			histograms->Add(obj);		      
		      }
		  }
	      }
	    else
	      {
		histograms->Add(obj);
	      }
	  }
      }

    TString tcname = fname;
    if (pattern != 0)
      tcname += Form("_%s", pattern);
    if (patternExlude != 0)
      tcname += Form("_NOT_%s", patternExlude);
    tcname += "_figure";

    TCanvas *tc = 0;
    if (newCanvas == kTRUE)
      {
	tc = new TCanvas(tcname.Data(), tcname.Data());
	tc->cd();
      }
    HistoUtils::drawList(histograms, opt, ymin, comm);
    return histograms;
  }

  TList *getSlices(const char *fname, const char *hname,
		   Int_t nslices, Double_t *range,
		   Int_t axis, Int_t startbin)
  {    
    Bool_t toDelete = kFALSE;
    TList *outputList = new TList();
    
    TFile *fin = new TFile(fname);
    TH2 *hin = (TH2F*)fin->Get(hname);
    if (hin == 0)
      cout << Form("Unable to get %s from %s", hname, fname) << endl;
    Int_t maxSlices = 0;
    if (nslices == 0)
      maxSlices = hin->GetNbinsY();
    if (nslices < 0)
      maxSlices = TMath::Abs(nslices);

    if (maxSlices > 0)
      {
	nslices = maxSlices;
	range = new Double_t[2 * nslices];
	toDelete = kTRUE;
	if (startbin <= 0) startbin = 1;	
	for (Int_t iby = 1; iby <= nslices; iby++)
	  {
	    if (axis == 0)
	      {
		if (iby + startbin > hin->GetYaxis()->GetNbins())
		  {
		    nslices = iby;
		    break;
		  }
		range[(iby-1)*2    ] = hin->GetYaxis()->GetBinLowEdge(iby + startbin);
		range[(iby-1)*2 + 1] = hin->GetYaxis()->GetBinCenter(iby + startbin) + hin->GetXaxis()->GetBinWidth(iby + startbin);	    
	      }
	    if (axis != 0)
	      {
		if (iby + startbin > hin->GetXaxis()->GetNbins())
		  {
		    nslices = iby;
		    break;
		  }
		range[(iby-1)*2    ] = hin->GetXaxis()->GetBinLowEdge(iby + startbin);
		range[(iby-1)*2 + 1] = hin->GetXaxis()->GetBinCenter(iby + startbin) + hin->GetXaxis()->GetBinWidth(iby + startbin);	    
	      }
	  }
      }

    for (Int_t i = 0; i < nslices; i++)
      {
	Double_t fmin = range[i*2];
	Double_t fmax = range[i*2 + 1];
	
	TAxis *axisPerp = hin->GetYaxis();
	if (axis != 0)
	  axisPerp = hin->GetXaxis();
	
	Int_t bin0 = axisPerp->FindBin(fmin);
	Int_t bin1 = axisPerp->FindBin(fmax);      
	
	TString newName = Form("%s_%s_%1.2f_%1.2f_axis_%s",
			       fname,
			       hin->GetName(), fmin, fmax, 
			       axisPerp->GetTitle());
	
	TString newTitle = Form("%s [ %1.2f %1.2f ] axis:%s",
				hin->GetTitle(), fmin, fmax, 
				axisPerp->GetTitle());
	
	TH1D *proj = 0;
	if (axis == 0)
	  {
	    proj = hin->ProjectionX(newName.Data(), bin0, bin1);	  
	  }
	else
	  {
	    proj = hin->ProjectionY(newName.Data(), bin0, bin1);
	  }
	
	proj->SetTitle(newTitle.Data());
	outputList->Add(proj);
      }
    
    if (toDelete)
      {
	delete [] range;
	range = 0;
      }
    
    return outputList;
  }

}; //end namespace HistoUtils
