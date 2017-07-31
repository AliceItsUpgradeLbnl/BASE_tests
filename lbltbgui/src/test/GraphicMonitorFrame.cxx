#include "GraphicMonitorFrame.h"

#include <TStyle.h>
#include <TClass.h>
#include <TList.h>
#include <TTimer.h>
#include <TGTab.h>
#include <TRootEmbeddedCanvas.h>
#include <TCanvas.h>
#include <TEllipse.h>
#include <TGStatusBar.h>
#include <TDatime.h>
#include <TGFileDialog.h>
#include <TGButton.h>
#include <TPaveText.h>
#include <TLegend.h>

#include "HistoList.h"
#include "DList.h"

#include "FileMonitor.h"

#include <TH1I.h>
#include <TGraph.h>

ClassImp(GraphicMonitorFrame);

#include <iostream>
using namespace std;

GraphicMonitorFrame::GraphicMonitorFrame (const TGWindow *p, const char *name, UInt_t options, Pixel_t back)
	: BaseMonitorFrame(p, name, options, back)
{
	fMonitor->SetOption(":binary");
	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);
	gStyle->SetScreenFactor(1);
}

GraphicMonitorFrame::~GraphicMonitorFrame()
{
	;
}

void GraphicMonitorFrame::ViewerInit()
{
	// embedded canvas
	TRootEmbeddedCanvas *fEmbCanvas = new TRootEmbeddedCanvas("fEmbCanvas",this,100,100,kSunkenFrame| kLHintsExpandX | kLHintsExpandY);
	fEmbCanvas->SetName(MakeName("fEmbCanvas"));
	fEmbCanvas->SetAutoFit(kTRUE);
	Int_t wfEmbCanvas = fEmbCanvas->GetCanvasWindowId();
	TCanvas *fTCanvas = new TCanvas(MakeName("fTCanvas"), 100, 100, wfEmbCanvas);
	AddObject(fTCanvas);
	fEmbCanvas->AdoptCanvas(fTCanvas);
	this->AddFrame(fEmbCanvas, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
	fEmbCanvas->SetContainer(this);
	AddObject(fEmbCanvas);

	// container for histograms - drawing
	DList *hl = new DList(MakeName("hl"),MakeName("hl"));
	AddObject(hl);
}

void GraphicMonitorFrame::UpdateGraphs()
{
	// make sure we do not time the drawing...
	Bool_t timerStat = GetTimerStatus();
	TimerStop();
	DList *hl = (DList*)GetObject("hl");
	cout << "[d] dlist at " << hl << endl;
	hl->DeactivateAll();
	hl->ClearList();
	Int_t ncol = 0;
	for (Int_t i = 0; i < fMonitor->GetSize(); i++)
	{
		BinaryFileMonitor *fm = (BinaryFileMonitor*)fMonitor->Get(i);
		cout << "[d] monitor number " << i << " at " << fm << endl;
		if (!fm)
		{
			cerr << "[e] Not a BinaryFileMonitor monitor: " << fMonitor->Get(i)->GetPath() << endl;
			continue;
		}
		if (fm->HasOption(":active"))
		{
			cout << "[i] active..." << endl;
			if (fm->IsPendingUpdate())
			{
				fm->Update();
			}
			else
			{
				cout << "[d] " << this->GetName() << " : " << fm->GetPath() << endl;
				cout << "    update skipped as no pending updates (force changed including)" << endl;
			}
			TGraph *gr = fm->GetGraph();
			if (gr != 0x0)
			{
				TString newTitle = gr->GetTitle() + TString::Format(" FILE #%d", i);
				gr->SetTitle(newTitle.Data());
				ncol = i;
				hl->Add(gr, TString::Format("+k%d +l1 +m%d +s7 +a50 l p ", ncol + 1, 20)); // + i));
			}
		}
	}
	cout << "[d] drawing..." << endl;
	hl->Draw();
	if (hl->GetNactive() > 0)
	{
		TLegend *leg = hl->Legend(this->GetName(), 0.1,0.8,0.9,0.9);
		leg->Draw();
	}
	// get back timer to the same state as at the start of the method
	if (timerStat == kTRUE)
		TimerStart();
}

void GraphicMonitorFrame::MakeHistograms()
{
	DList *hl = (DList*)GetObject("hl");
	cout << "[d] dlist at " << hl << endl;
	hl->DeactivateAll();
	for (Int_t i = 0; i < fMonitor->GetSize(); i++)
	{
		BinaryFileMonitor *fm = (BinaryFileMonitor*)fMonitor->Get(i);
		cout << "[d] monitor number " << i << " at " << fm << endl;
		if (!fm)
		{
			cerr << "[e] Not a binary file monitor: " << fMonitor->Get(i)->GetPath() << endl;
			continue;
		}
		if (fm->HasOption(":active"))
		{
			cout << "[i] active..." << endl;
			// will always poll - ::Update returns true when file lost data!
			DObject *obj = hl->GetByName(fm->GetPath());
			TH1I *h = 0;
			if (!obj)
			{
				// show only last N entries
				h = new TH1I(fm->GetPath(), fm->GetPath(), fm->GetNTail(), 0, fm->GetNTail());
				hl->Add(h, TString::Format("+k%d +l1 +m20 +s10 +a50 l p ", hl->GetSize() + 1));
				delete h;
			}
			else
			{
				obj->SetActive(kTRUE);
				h = (TH1I*)obj->GetObject();
				if (h->GetNbinsX() != fm->GetNTail())
				{
					TString opts = obj->GetOptions();
					cout << "[d] N bins changed... removing from draw list: " << fm->GetPath() << endl;
					hl->RemoveByName(fm->GetPath());
					cout << "[d] creating a new histogram with N bins: " << fm->GetNTail() << endl;
					h = new TH1I(fm->GetPath(), fm->GetPath(), fm->GetNTail(), 0, fm->GetNTail());
					cout << "[d] adding with options " << opts.Data() << endl;
					hl->Add(h, opts.Data());
					delete h;
				}
			}
			obj = hl->GetByName(fm->GetPath());
			h = (TH1I*)obj->GetObject();
			// this has to change - now the tail is in the GetUpdate()!
			cout << "[d] histogram " << h->GetName() << " at " << h << endl;
			h->Reset();
			cout << "[d] update..." << endl;
			Bool_t resetFlag = fm->Update();
			//fm->DumpUpdate();
			//std::vector<char> v = fm->GetContent();
			std::vector<char> v = fm->GetUpdate();
			cout << "[i] size of the update [bytes]: " << v.size() << endl;
			for (Int_t ibin = 1; ibin < v.size() + 1; ibin++)
			{
				if (ibin <= h->GetNbinsX())
				{
					//cout << "[d] ibin = " << ibin << " value = [" << short(v[ibin-1]) << "]" << endl;
					h->SetBinContent(ibin, (unsigned char)v[ibin-1]);
				}
			}
			//for (unsigned int ibin = 1; ibin < h->GetNbinsX(); ibin++)
			//{
			//	unsigned int idx = v.size() - h->GetNbinsX() + ibin;
			//	if (idx > 0 && idx < v.size())
			//	{
			//		h->SetBinContent(ibin, v[idx]);
			//		//cout << "[d] bin " << ibin << " " << v[idx] << endl;
			//	}
			//}
		}
		else
		{
			DObject *obj = hl->GetByName(fm->GetPath());
			obj->SetActive(kFALSE);
			//cout << "[d] not active?" << endl;
		}
	}
	cout << "[d] drawing..." << endl;
	hl->Draw();
	if (hl->GetNactive() > 0)
	{
		TLegend *leg = hl->Legend(this->GetName(), 0.1,0.8,0.9,0.9);
		leg->Draw();
	}
}

void GraphicMonitorFrame::ViewerUpdate()
{
	TCanvas *tc = (TCanvas*)GetObject("fTCanvas");
	if (tc)
	{
		cout << "[d] fTCanvas at " << tc << endl;
		tc->Clear();
		tc->cd();
		//MakeHistograms();
		UpdateGraphs();

		TString scomment = Now() + TString(" ") + TString(GetName());
		TPaveText *ptxt = (TPaveText*)tc->GetListOfPrimitives()->FindObject(MakeName("OnTimer"));
		if (ptxt)
		{
			ptxt->Clear();
			ptxt->AddText(scomment.Data());
		}
		else
		{
			ptxt = HistoUtils::drawComment(scomment.Data(), 0.1, 0.9, 0.9, 0.99);
			ptxt->SetName(MakeName("OnTimer"));
		}
		tc->Modified();
		tc->Update();
	}
	else
	{
		cout << "[e] " << this->GetName() << " fTCanvas at " << tc << endl;		
	}
}
