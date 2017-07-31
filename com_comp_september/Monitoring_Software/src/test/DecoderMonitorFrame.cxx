#include "DecoderMonitorFrame.h"

#include <TLegend.h>
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
#include <TGLabel.h>
#include <TGComboBox.h>
#include <TGSplitter.h>

#include "HistoList.h"
#include "DList.h"

#include "FileMonitor.h"
#include "Decoder.h"

#include <TGraph.h>

ClassImp(DecoderMonitorFrame);

#include <iostream>
using namespace std;

DecoderMonitorFrame::DecoderMonitorFrame (const TGWindow *p, const char *name, UInt_t options, Pixel_t back)
	: BaseMonitorFrame(p, name, options, back)
	, fChannelsSelected()
	, fMissingTrailers(kFALSE)
{
	fMonitor->SetOption(":decoder");
	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);
	gStyle->SetScreenFactor(1);	

	for (unsigned int i = 0; i < 32; i++)
		fChannelsSelected.push_back(kFALSE);
}

DecoderMonitorFrame::~DecoderMonitorFrame()
{
	;
}

void DecoderMonitorFrame::ViewerInit()
{
	TString thisClass = this->IsA()->GetName();

	TGGroupFrame *fGroupFrameList = new TGGroupFrame(this,"Data");
	fGroupFrameList->SetLayoutManager(new TGHorizontalLayout(fGroupFrameList));
	fGroupFrameList->Resize(40, 40);
	this->AddFrame(fGroupFrameList, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX));

	TGLabel *headerComboLabel = new TGLabel(fGroupFrameList, "ActivateHeader");
	headerComboLabel->SetText("Select Header ");
	fGroupFrameList->AddFrame(headerComboLabel, new TGLayoutHints(kLHintsLeft | kLHintsExpandY, 5,5,5,0));
	TGComboBox *headerListCombo = new TGComboBox(fGroupFrameList, "HeaderList");
	headerListCombo->SetName(MakeName("fheaderListCombo"));
	headerListCombo->Connect("Selected(Int_t)",thisClass.Data(),this,"SetActiveHeader(Int_t)");
	headerListCombo->EnableTextInput(kFALSE);
	headerListCombo->RemoveAll();
	headerListCombo->AddEntry("0xC3000009", 0xC3000009);
	headerListCombo->AddEntry("0xCD000013", 0xCD000013);
	headerListCombo->AddEntry("0xCD000003", 0xCD000003);
	headerListCombo->AddEntry("0x0D000004", 0x0D000004);
	headerListCombo->AddEntry("0xC1000005", 0xC1000005);
	headerListCombo->AddEntry("0xAF000002", 0xAF000002);
	headerListCombo->SetEnabled(kTRUE);
	fGroupFrameList->AddFrame(headerListCombo, new TGLayoutHints(kLHintsLeft | kLHintsExpandX | kLHintsExpandY));
	AddObject(headerListCombo);

	TGCheckButton *toggleMissingTrailersButton = new TGCheckButton(fGroupFrameList,"Missing Trailers");
	toggleMissingTrailersButton->SetName(MakeName("toggleMissingTrailersButton"));
	toggleMissingTrailersButton->SetOn(fMissingTrailers);
	toggleMissingTrailersButton->Connect("Clicked()",thisClass.Data(),this,"ToggleMissingTrailers()");
	fGroupFrameList->AddFrame(toggleMissingTrailersButton, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 5,5,3,4));
	AddObject(toggleMissingTrailersButton);

	TGLabel *channelComboLabel = new TGLabel(fGroupFrameList, "Activatechannel");
	channelComboLabel->SetText("Select Channel ");
	fGroupFrameList->AddFrame(channelComboLabel, new TGLayoutHints(kLHintsLeft | kLHintsExpandY, 5,5,5,0));
	TGComboBox *channelListCombo = new TGComboBox(fGroupFrameList, "channelList");
	channelListCombo->SetName(MakeName("fchannelListCombo"));
	channelListCombo->Connect("Selected(Int_t)",thisClass.Data(),this,"SetActiveChannel(Int_t)");
	channelListCombo->EnableTextInput(kFALSE);
	channelListCombo->RemoveAll();
	channelListCombo->SetEnabled(kTRUE);
	fGroupFrameList->AddFrame(channelListCombo, new TGLayoutHints(kLHintsLeft | kLHintsExpandX | kLHintsExpandY));
	AddObject(channelListCombo);

	// embedded canvas

	TGGroupFrame *fGroupFrameListTC = new TGGroupFrame(this,"Canvas'", kHorizontalFrame);
	fGroupFrameListTC->SetLayoutManager(new TGHorizontalLayout(fGroupFrameListTC));
	fGroupFrameListTC->Resize(40, 40);
	this->AddFrame(fGroupFrameListTC, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX | kLHintsExpandY));

	// for graphs
	TRootEmbeddedCanvas *fEmbCanvas = new TRootEmbeddedCanvas("fEmbCanvas",fGroupFrameListTC,100,100, kSunkenFrame);
	fEmbCanvas->SetName(MakeName("fEmbCanvas"));
	fEmbCanvas->SetAutoFit(kTRUE);
	Int_t wfEmbCanvas = fEmbCanvas->GetCanvasWindowId();
	TCanvas *fTCanvas = new TCanvas(MakeName("fTCanvas"), 100, 100, wfEmbCanvas);
	AddObject(fTCanvas);
	fEmbCanvas->AdoptCanvas(fTCanvas);
	fGroupFrameListTC->AddFrame(fEmbCanvas, new TGLayoutHints(kLHintsLeft | kLHintsExpandY | kLHintsExpandX));
	fEmbCanvas->SetContainer(fGroupFrameListTC);
	AddObject(fEmbCanvas);

	// for legend only
	TRootEmbeddedCanvas *fEmbCanvasL = new TRootEmbeddedCanvas("fEmbCanvasL",fGroupFrameListTC,300,100, kSunkenFrame | kFixedWidth);
	fEmbCanvasL->SetName(MakeName("fEmbCanvasL"));
	fEmbCanvasL->SetAutoFit(kTRUE);
	Int_t wfEmbCanvasL = fEmbCanvasL->GetCanvasWindowId();
	TCanvas *fTCanvasL = new TCanvas(MakeName("fTCanvasL"), 100, 100, wfEmbCanvasL);
	AddObject(fTCanvasL);
	fEmbCanvasL->AdoptCanvas(fTCanvasL);
	fGroupFrameListTC->AddFrame(fEmbCanvasL, new TGLayoutHints(kLHintsRight | kLHintsExpandY));
	fEmbCanvasL->SetContainer(fGroupFrameListTC);
	AddObject(fEmbCanvasL);

	TGVSplitter *splitter = new TGVSplitter(fGroupFrameListTC,2,2);
   	splitter->SetFrame(fEmbCanvasL, kFALSE);
   	fGroupFrameListTC->AddFrame(splitter, new TGLayoutHints(kLHintsLeft | kLHintsExpandY));

	// container for histograms - drawing
	DList *hl = new DList(MakeName("hl"),MakeName("hl"));
	AddObject(hl);
}

void DecoderMonitorFrame::ToggleMissingTrailers()
{
	fMissingTrailers = !fMissingTrailers;
	TGCheckButton *cb = (TGCheckButton *)GetObject("toggleMissingTrailersButton");
	if (cb)
	{
		cb->SetOn(fMissingTrailers);
	}
	UpdateFileList();
	OnTimer();
}

void DecoderMonitorFrame::SetActiveHeader(Int_t h)
{
	if (! Masks::is_known_header(h))
		return;

	fChannelsSelected.clear();
	fChannelsSelected.reserve(32);
	for (unsigned int i = 0; i < 32; i++)
	{
		fChannelsSelected.push_back(kFALSE);
	}

	for (Int_t i = 0; i < fMonitor->GetSize(); i++)
	{
		Decoder *fm = (Decoder*)fMonitor->Get(i);
		if (fm)
		{
			fm->SetHeader(h);
			fm->ForceChanged();
		}
	}	

	TGComboBox *tb = (TGComboBox*)GetObject("fheaderListCombo");
	if (tb)
		tb->Select(h, kFALSE);

	if (h == 0xC3000009)
	{
		TGComboBox *tb = (TGComboBox*)GetObject("fchannelListCombo");
		tb->RemoveAll();
		for (unsigned int i = 0; i < 32; i++)
		{
			tb->AddEntry(TString::Format("%d", i), i);
		}
	}

	if (h == 0xAF000002)
	{
		TGComboBox *tb = (TGComboBox*)GetObject("fchannelListCombo");
		tb->RemoveAll();
		tb->AddEntry(TString::Format("%d", 0), 0);
		tb->AddEntry(TString::Format("%d", 1), 1);
		tb->AddEntry(TString::Format("%d", 16), 16);
		tb->AddEntry(TString::Format("%d", 17), 17);
		tb->AddEntry(TString::Format("%d", 19), 19);		
	}

	if (h == 0xC1000005)
	{
		TGComboBox *tb = (TGComboBox*)GetObject("fchannelListCombo");
		tb->RemoveAll();
		tb->AddEntry("ADCMP603_Q_OUT", 0);
		tb->AddEntry("ADCMP603_Q_BAR_OUT", 1);
		tb->AddEntry("ADCMP603_LE_HYS_OUT", 2);
	}

	if (h == 0x0D000004)
	{
		TGComboBox *tb = (TGComboBox*)GetObject("fchannelListCombo");
		tb->RemoveAll();
		tb->AddEntry(TString::Format("%d", 2), 2);
		tb->AddEntry(TString::Format("%d", 3), 3);
		tb->AddEntry(TString::Format("%d", 4), 4);
		tb->AddEntry(TString::Format("%d", 5), 5);
	}

	if (h == 0xCD000003)
	{
		TGComboBox *tb = (TGComboBox*)GetObject("fchannelListCombo");
		tb->RemoveAll();
		for (unsigned int i = 7; i <= 18; i++)
			tb->AddEntry(TString::Format("%d", i), i);
	}

	if (h == 0xCD000013)
	{
		TGComboBox *tb = (TGComboBox*)GetObject("fchannelListCombo");
		tb->RemoveAll();
		for (unsigned int i = 7; i <= 17; i++)
			tb->AddEntry(TString::Format("%d", i), i);
	}

	Update();
	ViewerUpdate();
}

void DecoderMonitorFrame::SetActiveChannel(Int_t h)
{
	fChannelsSelected[h] = ! fChannelsSelected[h];
	if (fChannelsSelected[h])
	{
		TGComboBox *tb = (TGComboBox*)GetObject("fchannelListCombo");
		if (tb)
		{
			tb->Select(h, kFALSE);
		}
	}
	Update();
	ViewerUpdate();	
}

void DecoderMonitorFrame::SyncHeaders()
{
	unsigned int head = 0;
	TGComboBox *tb = (TGComboBox*)GetObject("fheaderListCombo");
	if (tb)
	{
		head = tb->GetSelected();
		for (Int_t i = 0; i < fMonitor->GetSize(); i++)
		{
			Decoder *fm = (Decoder*)fMonitor->Get(i);
			if (fm)
			{
				fm->SetHeader(head);
			}
		}
	}
}

void DecoderMonitorFrame::UpdateGraphs()
{
	// make sure we do not time the drawing...
	Bool_t timerStat = GetTimerStatus();
	TimerStop();
	SyncHeaders();
	DList *hl = (DList*)GetObject("hl");
	cout << "[d] dlist at " << hl << endl;
	hl->DeactivateAll();
	hl->ClearList();
	for (Int_t i = 0; i < fMonitor->GetSize(); i++)
	{
		Decoder *fm = (Decoder*)fMonitor->Get(i);
		cout << "[d] monitor number " << i << " at " << fm << endl;
		if (!fm)
		{
			cerr << "[e] Not a decoder monitor: " << fMonitor->Get(i)->GetPath() << endl;
			continue;
		}
		fm->SetMissingTrailers(fMissingTrailers);
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
			Int_t ncol = 0;
			for (unsigned int ich = 0; ich < fChannelsSelected.size(); ich++)
			{
				if (fChannelsSelected[ich] == kTRUE)
				{
					TGraph *gr = fm->GetGraph(ich);
					if (gr != 0x0)
					{
						TString newTitle = gr->GetTitle() + TString::Format(" FILE #%d", i);
						gr->SetTitle(newTitle.Data());
						//hl->Add(gr, TString::Format("+k%d +l1 +m%d +s7 +a50 l p ", ich + 1, 20 + i));
						//hl->Add(gr, TString::Format("+k%d +l1 +m%d +s7 +a50 l p ", hl->GetN() + 1, 20 + i));
						hl->Add(gr, TString::Format("+k%d +l1 +m%d +s7 +a50 l p ", ncol + 1, 20 + i));
					}
					ncol++;
					if (ncol == 4) ncol++;
				}
			}
		}
	}
	cout << "[d] drawing..." << endl;
	TCanvas *tc = (TCanvas*)GetObject("fTCanvas");
	tc->Clear();
	tc->cd();
	hl->Draw();
	gPad->SetGridy();
	tc->Modified();
	tc->Update();

	tc = (TCanvas*)GetObject("fTCanvasL");
	tc->Clear();
	tc->cd();
	if (hl->GetNactive() > 0)
	{
		//hl->Legend(this->GetName(), 0.1,0.8,0.9,0.9);
		TLegend * leg = hl->Legend(TString::Format("%s Legend", this->GetName()), 0.01,0.01,0.99,0.99);
		leg->Draw();
	}
	tc->Modified();
	tc->Update();

	// get back timer to the same state as at the start of the method
	if (timerStat == kTRUE)
		TimerStart();
}

void DecoderMonitorFrame::ViewerUpdate()
{
	TCanvas *tc = (TCanvas*)GetObject("fTCanvas");
	if (tc)
	{
		cout << "[d] fTCanvas at " << tc << endl;
		UpdateGraphs();

		tc->cd();
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
