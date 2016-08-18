#include "TabFrame.h"

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

#include "HistoList.h"

#include "FileMonitor.h"

ClassImp(TabFrame);

#include <iostream>
using namespace std;

TabFrame::TabFrame (const TGWindow *p, UInt_t w, UInt_t h, UInt_t options, Pixel_t back)
	: TimedFrame(p, w, h, options, back)
	, fMonitor(new FilesMonitor("FileMonitor", "FileMonitor"))
{
	;
}

void TabFrame::Init()
{

	TGVerticalLayout *fLayout = new TGVerticalLayout(this);
	this->SetLayoutManager(fLayout);
	AddObject(fLayout);

    // embedded canvas
	TRootEmbeddedCanvas *fEmbCanvas = new TRootEmbeddedCanvas("fEmbCanvas",this,100,100,kSunkenFrame| kLHintsExpandX | kLHintsExpandY);
	fEmbCanvas->SetName(MakeName("fEmbCanvas"));
	fEmbCanvas->SetAutoFit(kTRUE);
	Int_t wfEmbCanvas = fEmbCanvas->GetCanvasWindowId();
	TCanvas *fTCanvas = new TCanvas(MakeName("TCanvas"), 100, 100, wfEmbCanvas);
	AddObject(fTCanvas);
	fEmbCanvas->AdoptCanvas(fTCanvas);
	this->AddFrame(fEmbCanvas, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
	fEmbCanvas->SetContainer(this);
	AddObject(fEmbCanvas);

	TGStatusBar *fStatusBar = new TGStatusBar(this,1,1, kSunkenFrame| kLHintsExpandX);
	fStatusBar->SetName(MakeName("fStatusBar"));
	fStatusBar->SetParts(3);
	this->AddFrame(fStatusBar, new TGLayoutHints(kLHintsLeft | kLHintsBottom | kLHintsExpandX));
	AddObject(fStatusBar);

	// controls
	TGGroupFrame *fGroupFrame = new TGGroupFrame(this,"Controls");
	fGroupFrame->SetLayoutManager(new TGHorizontalLayout(fGroupFrame));
	fGroupFrame->Resize(10,10);
	this->AddFrame(fGroupFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX));

	TGTextButton *selectFileToMonitor = new TGTextButton(fGroupFrame,"Select &File To Monitor");
	selectFileToMonitor->Connect("Clicked()","TabFrame",this,"SelectFileToMonitor()");   
	fGroupFrame->AddFrame(selectFileToMonitor, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 5,5,3,4));

	TGTextButton *dumpListButton = new TGTextButton(fGroupFrame,"&DumpList");
	dumpListButton->Connect("Clicked()","TabFrame",this,"DumpList()");   
	fGroupFrame->AddFrame(dumpListButton, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 5,5,3,4));

	TGCheckButton *toggleTimerButton = new TGCheckButton(fGroupFrame,"Li&ve");
	toggleTimerButton->SetName(MakeName("toggleTimerButton"));
	toggleTimerButton->SetOn(kTRUE);
	toggleTimerButton->Connect("Clicked()","TabFrame",this,"ToggleTimer()");
	fGroupFrame->AddFrame(toggleTimerButton, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 5,5,3,4));
	AddObject(toggleTimerButton);

	fLayout->Layout();

	ToggleTimer();

	OnTimer();	
}

TabFrame::~TabFrame()
{
	;
}

void TabFrame::UpdateStatusBar()
{
	// Update Status Bar
	TString snow = Now();
	//cout << "[d] ::OnTimer" << endl;
	TGStatusBar *stat = (TGStatusBar*)GetObject("fStatusBar");
	if (stat)
	{
		TString stmp = TString::Format("N files monitored: %d", fMonitor->GetList()->GetSize());
		stat->SetText(stmp, 0);
		stat->SetText(TString::Format("This Frame at: 0x%lx", (long int)this), 1);
		stat->SetText(TString("Now: ") + snow, 2);
	}
}

Bool_t TabFrame::HasFileChanged()
{
	Bool_t changed = fMonitor->HasChanged();
	TGStatusBar *stat = (TGStatusBar*)GetObject("fStatusBar");
	if (stat)
	{
		if (changed == kTRUE)
		{
			stat->SetText( "Changed: True", 1);
		}
		else
		{
			stat->SetText( "Changed: False", 1);
		}
	}
	return changed;
}

void TabFrame::OnTimer()
{
	UpdateStatusBar();
	HasFileChanged();

	//DumpList();
	TCanvas *tc = (TCanvas*)GetObject("TCanvas");
	if (tc)
	{
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
			ptxt = HistoUtils::drawComment(scomment.Data());
			ptxt->SetName(MakeName("OnTimer"));
		}
		tc->Modified();
		tc->Update();
	}
}

void TabFrame::SelectFileToMonitor()
{
    TGFileInfo file_info_;
    const char *filetypes[] = {"Python Files", "*.py", 0, 0};
    file_info_.fFileTypes = filetypes;
    file_info_.fIniDir = StrDup(".");	
	TGFileDialog *dial = new TGFileDialog(gClient->GetRoot(), this, kFDOpen, &file_info_);
	TString stmp = file_info_.fFilename;
	if (stmp.Length() > 0)
	{
		fMonitor->AddFile(file_info_.fFilename);
		TGStatusBar *stat = (TGStatusBar*)GetObject("fStatusBar");
		if (stat)
		{
			TString stmp = TString::Format("N files monitored: %d", fMonitor->GetList()->GetSize());
			stat->SetText(stmp, 0);
		}
		// change the TAB name?
		//TGWindow *p = GetParent();
		//if (p)
		//{
		//	TString sh = gSystem->BaseName(fFileToMonitor);
		//	p->SetName(sh);
		//}
	}
	OnTimer();
}
