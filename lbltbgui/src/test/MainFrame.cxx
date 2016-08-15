#include "MainFrame.h"
#include <TApplication.h>

#include <TGMdiFrame.h>
#include <TGTab.h>
#include <TApplication.h>
#include <TGButton.h>
#include <TClass.h>

#include "TimedFrame.h"
#include "GraphicMonitorFrame.h"
#include "TextMonitorFrame.h"
#include "DecoderMonitorFrame.h"
#include "TRootEmbeddedCanvas.h"
#include "Options.h"

#include <iostream>
using namespace std;

ClassImp(MainFrame);

MainFrame::MainFrame(const TGWindow *p, UInt_t w, UInt_t h, UInt_t options)
	: TGMainFrame(p, w, h, options)
	, fTab(0)
	, fInputDir(".")
	, fOnlineFlag(kTRUE)
{
	// extra control
	// controls
	TGGroupFrame *fGroupFrame = new TGGroupFrame(this,"Global Controls");
	fGroupFrame->SetLayoutManager(new TGHorizontalLayout(fGroupFrame));
	fGroupFrame->Resize(10,10);
	this->AddFrame(fGroupFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX));

	TGTextButton *addTestBeamTabButton = new TGTextButton(fGroupFrame,"Add Test Beam Tab");
	addTestBeamTabButton->Connect("Clicked()",this->IsA()->GetName(),this,"AddTestBeamMonitorFrame()");   
	fGroupFrame->AddFrame(addTestBeamTabButton, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 5,5,3,4));

	TGTextButton *addGraphicTabButton = new TGTextButton(fGroupFrame,"Add Graphic Tab");
	addGraphicTabButton->Connect("Clicked()",this->IsA()->GetName(),this,"AddGraphicMonitorFrame()");   
	fGroupFrame->AddFrame(addGraphicTabButton, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 5,5,3,4));

	TGTextButton *addTextTabButton = new TGTextButton(fGroupFrame,"Add Text Tab");
	addTextTabButton->Connect("Clicked()",this->IsA()->GetName(),this,"AddTextMonitorFrame()");   
	fGroupFrame->AddFrame(addTextTabButton, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 5,5,3,4));

	TGTextButton *quitButton = new TGTextButton(fGroupFrame,"Quit");
	quitButton->Connect("Clicked()","TApplication",gApplication,"Terminate(Int_t)");   
	fGroupFrame->AddFrame(quitButton, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 5,5,3,4));

	fTab = new TGTab(this);
	this->AddFrame(fTab, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));

	this->SetLayoutBroken(kFALSE);
	this->SetMWMHints(kMWMDecorAll, kMWMFuncAll,kMWMInputModeless);
	this->MapSubwindows();
	this->Resize(this->GetDefaultSize());
	this->MapWindow();

}

MainFrame::~MainFrame()
{
	;
}

void MainFrame::CloseWindow()
{
   // Got close message for this MainFrame. Terminate the application
   // or returns from the TApplication event loop (depending on the
   // argument specified in TApplication::Run()).

   gApplication->Terminate(0);
}

TextMonitorFrame *MainFrame::AddTextMonitorFrame(const char *tname)
{
	TString tabName = TString::Format("Text[%d]", fTab->GetNumberOfTabs());
	if (tname != 0) tabName = tname;
	TString frameName = tabName + TString("-Frame");
	TextMonitorFrame *tg = new TextMonitorFrame(fTab, frameName.Data());
	fTab->AddTab(tabName.Data(), tg);
	tg->Init();
	tg->SetInputDir(fInputDir.Data());
	tg->SetOnline(fOnlineFlag);
	this->MapSubwindows();
	this->MapWindow();
	this->Resize();
	fTab->SetTab(fTab->GetNumberOfTabs()-1, kFALSE);
	return tg;
}

DecoderMonitorFrame *MainFrame::AddTestBeamMonitorFrame(const char *tname)
{
	TString tabName = TString::Format("TestBeam[%d]", fTab->GetNumberOfTabs());
	if (tname != 0) tabName = tname;
	TString frameName = tabName + TString("-Frame");
	DecoderMonitorFrame *tg = new DecoderMonitorFrame(fTab, frameName.Data());
	fTab->AddTab(tabName.Data(), tg);
	tg->Init();
	tg->SetInputDir(fInputDir.Data());
	tg->SetOnline(fOnlineFlag);
	this->MapSubwindows();
	this->MapWindow();
	this->Resize();
	fTab->SetTab(fTab->GetNumberOfTabs()-1, kFALSE);
	return tg;	
}

GraphicMonitorFrame *MainFrame::AddGraphicMonitorFrame(const char *tname)
{
	// alternative add - first composite frame...
	//TString tabName = TString::Format("Graphic-%d", fTab->GetNumberOfTabs());
	//TGCompositeFrame *compositeFrame = fTab->AddTab(tabName.Data());
	//compositeFrame->SetLayoutManager(new TGVerticalLayout(compositeFrame));
	//TString frameName = tabName + TString("-Frame");
	//GraphicMonitorFrame *tg = new GraphicMonitorFrame(compositeFrame, frameName.Data());
	//compositeFrame->AddFrame(tg, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));

	TString tabName = TString::Format("Graphic[%d]", fTab->GetNumberOfTabs());
	if (tname != 0) tabName = tname;
	TString frameName = tabName + TString("-Frame");
	GraphicMonitorFrame *tg = new GraphicMonitorFrame(fTab, frameName.Data());
	fTab->AddTab(tabName.Data(), tg);
	tg->Init();
	tg->SetInputDir(fInputDir.Data());	
	tg->SetOnline(fOnlineFlag);
	this->MapSubwindows();
	this->MapWindow();
	this->Resize();
	fTab->SetTab(fTab->GetNumberOfTabs()-1, kFALSE);
	return tg;
}

void MainFrame::HandleOptions(Options *opts)
{
	int nTail = atoi(opts->ArgWith("--tail").c_str());
	if (nTail >= 0)
	{
		cout << "[i] command line ntail: " << nTail << endl;
	}

	fInputDir = opts->ArgWith("--dir").c_str();
	cout << "[i] cmnd line monitors input dir: " << fInputDir.Data() << endl;

	fOnlineFlag = kTRUE;
	if (opts->IsSet("--offline"))
		fOnlineFlag = kFALSE;
	if (opts->IsSet("--online"))
		fOnlineFlag = kTRUE;

	cout << "[i] cmnd line online flag: " << fOnlineFlag << endl;

	if (opts->IsSet("--tx"))
	{
		TextMonitorFrame *mf = AddTextMonitorFrame();
		std::vector<std::string> vs = opts->ArgsWith("--tx");
		for (unsigned int i = 0; i < vs.size(); i++)
			if (vs[i].size() > 0)
				mf->AddFile(vs[i].c_str());
		if (nTail >= 0 && opts->IsSet("--tail"))
			mf->UserNTail(nTail);
		mf->OnTimer();
	}

	if (opts->IsSet("--gr"))
	{
		GraphicMonitorFrame *mf = AddGraphicMonitorFrame();
		std::vector<std::string> vs = opts->ArgsWith("--gr");
		for (unsigned int i = 0; i < vs.size(); i++)
			if (vs[i].size() > 0)
				mf->AddFile(vs[i].c_str());
		if (nTail >= 0 && opts->IsSet("--tail"))
			mf->UserNTail(nTail);
		mf->OnTimer();
	}

	if (opts->IsSet("--tb"))
	{
		DecoderMonitorFrame *mf = AddTestBeamMonitorFrame();

		std::vector<std::string> vs = opts->ArgsWith("--tb");
		for (unsigned int i = 0; i < vs.size(); i++)
			if (vs[i].size() > 0)
				mf->AddFile(vs[i].c_str());

		unsigned int head = strtol(opts->ArgWith("--head").c_str(), NULL, 16);
		if (head > 0)
		{
			mf->SetActiveHeader(head);
		}

		std::vector<std::string> vadc = opts->ArgsWith("--ch");
		for (unsigned int i = 0; i < vadc.size(); i++)
		{
			if (vadc[i].size() < 1) continue;
			int iadc = atoi(vadc[i].c_str());
			if (iadc >= 0 && iadc < 32)
				mf->SetActiveChannel(iadc);
		}
		if (nTail >= 0 && opts->IsSet("--tail"))
			mf->UserNTail(nTail);
		mf->OnTimer();
	}
}