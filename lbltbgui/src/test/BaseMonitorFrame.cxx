#include "BaseMonitorFrame.h"

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
#include <TGComboBox.h>
#include <TGLabel.h>
#include <TPaveText.h>

#include "HistoList.h"

#include "FileMonitor.h"

ClassImp(BaseMonitorFrame);

#include <iostream>
using namespace std;

BaseMonitorFrame::BaseMonitorFrame (const TGWindow *p, const char *name, UInt_t options, Pixel_t back)
	: TimedFrame(p, 10, 10, options, back)
	, fIgnoreFileStatus(kFALSE)
	, fInputDir(".")
	, fMonitor(new FilesMonitor("FileMonitor", "FileMonitor"))
{
	SetName(name);
}

void BaseMonitorFrame::Init()
{
	TString thisClass = this->IsA()->GetName();

	TGVerticalLayout *fLayout = new TGVerticalLayout(this);
	this->SetLayoutManager(fLayout);
	AddObject(fLayout);

	TGStatusBar *fStatusBar = new TGStatusBar(this, 10, 10, kSunkenFrame| kLHintsExpandX);
	fStatusBar->SetName(MakeName("fStatusBar"));
	fStatusBar->SetParts(3);
	this->AddFrame(fStatusBar, new TGLayoutHints(kLHintsLeft | kLHintsBottom | kLHintsExpandX));
	AddObject(fStatusBar);

	// controls
	TGGroupFrame *fGroupFrame = new TGGroupFrame(this,"Controls");
	fGroupFrame->SetLayoutManager(new TGHorizontalLayout(fGroupFrame));
	fGroupFrame->Resize(10,10);
	this->AddFrame(fGroupFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX));

	TGTextButton *selectFileToMonitor = new TGTextButton(fGroupFrame,"&Add File");
	selectFileToMonitor->Connect("Clicked()",thisClass.Data(),this,"SelectFileToMonitor()");   
	fGroupFrame->AddFrame(selectFileToMonitor, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 5,5,3,4));

	TGTextButton *deselectFileToMonitor = new TGTextButton(fGroupFrame,"&Remove File");
	deselectFileToMonitor->Connect("Clicked()",thisClass.Data(),this,"DeSelectFileToMonitor()");   
	fGroupFrame->AddFrame(deselectFileToMonitor, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 5,5,3,4));

	TGTextButton *debugButton = new TGTextButton(fGroupFrame,"Debug Dump");
	debugButton->Connect("Clicked()",thisClass.Data(),this,"DumpDebug()");   
	fGroupFrame->AddFrame(debugButton, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 5,5,3,4));

	TGTextButton *refreshButton = new TGTextButton(fGroupFrame,"Refresh");
	refreshButton->Connect("Clicked()",thisClass.Data(),this,"UpdateFileList()");   
	fGroupFrame->AddFrame(refreshButton, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 5,5,3,4));

	// tail selection
	TGLabel *tailComboLabel = new TGLabel(fGroupFrame, "TailNEntries");
	tailComboLabel->SetText("Tail");
	fGroupFrame->AddFrame(tailComboLabel, new TGLayoutHints(kLHintsLeft | kLHintsExpandY, 5,5,5,0));
	TGComboBox *tileListCombo = new TGComboBox(fGroupFrame, "Files");
	tileListCombo->SetName(MakeName("ftileListCombo"));
	tileListCombo->Connect("Selected(Int_t)",thisClass.Data(),this,"SetNTail(Int_t)");
	tileListCombo->EnableTextInput(kFALSE);
	tileListCombo->RemoveAll();
	tileListCombo->AddEntry("file size", 0);
	tileListCombo->AddEntry("100B", 	 100);	
	tileListCombo->AddEntry("500B", 	 500);	
	tileListCombo->AddEntry(" 1kB", 	 1000);
	tileListCombo->AddEntry(" 5kB", 	 5000);
	tileListCombo->AddEntry("10kB", 	 10000);
	tileListCombo->AddEntry("100kB",	 100000);
	tileListCombo->AddEntry("1MB",		 1000000);
	tileListCombo->AddEntry("10MB",		 10000000);
	tileListCombo->Select(10000, kFALSE); // 10kB is the default setting
	tileListCombo->SetEnabled(kTRUE);
	fGroupFrame->AddFrame(tileListCombo, new TGLayoutHints(kLHintsLeft | kLHintsExpandX | kLHintsExpandY));
	AddObject(tileListCombo);

	TGCheckButton *toggleTimerButton = new TGCheckButton(fGroupFrame,"Li&ve");
	toggleTimerButton->SetName(MakeName("toggleTimerButton"));
	//toggleTimerButton->SetOn(kTRUE);
	toggleTimerButton->Connect("Clicked()",thisClass.Data(),this,"ToggleTimer()");
	fGroupFrame->AddFrame(toggleTimerButton, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 5,5,3,4));
	AddObject(toggleTimerButton);

	TGCheckButton *toggleIgnoreFileStatusButton = new TGCheckButton(fGroupFrame,"Ignore File Status");
	toggleIgnoreFileStatusButton->SetName(MakeName("toggleIgnoreFileStatusButton"));
	toggleIgnoreFileStatusButton->SetOn(fIgnoreFileStatus);
	toggleIgnoreFileStatusButton->Connect("Clicked()",thisClass.Data(),this,"ToggleIgnoreFileStatus()");
	fGroupFrame->AddFrame(toggleIgnoreFileStatusButton, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 5,5,3,4));
	AddObject(toggleIgnoreFileStatusButton);

	TGGroupFrame *fGroupFrameList = new TGGroupFrame(this,"List");
	fGroupFrameList->SetLayoutManager(new TGHorizontalLayout(fGroupFrameList));
	fGroupFrameList->Resize(40, 40);
	this->AddFrame(fGroupFrameList, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX));
	TGLabel *fileComboLabel = new TGLabel(fGroupFrameList, "ActivateDeactivateFile");
	fileComboLabel->SetText("De/Activate Display File ");
	fGroupFrameList->AddFrame(fileComboLabel, new TGLayoutHints(kLHintsLeft | kLHintsExpandY, 5,5,5,0));
	TGComboBox *fileListCombo = new TGComboBox(fGroupFrameList, "FileList");
	fileListCombo->SetName(MakeName("fFileListCombo"));
	fileListCombo->Connect("Selected(Int_t)",thisClass.Data(),this,"SetActive(Int_t)");
	fileListCombo->EnableTextInput(kFALSE);
	fileListCombo->RemoveAll();
	fileListCombo->AddEntry("No files to activate...", 0);
	fileListCombo->SetEnabled(kFALSE);
	fGroupFrameList->AddFrame(fileListCombo, new TGLayoutHints(kLHintsLeft | kLHintsExpandX | kLHintsExpandY));
	AddObject(fileListCombo);

	ViewerInit();

	fLayout->Layout();

	TimerStart();
	TGCheckButton *cb = (TGCheckButton *)GetObject("toggleTimerButton");
	if (cb)
		cb->SetOn(GetTimerStatus());
}

void BaseMonitorFrame::ToggleTimer()
{
	TimedFrame::ToggleTimer();
	TGCheckButton *cb = (TGCheckButton *)GetObject("toggleTimerButton");
	if (cb)
		cb->SetOn(GetTimerStatus());
}

void BaseMonitorFrame::TimerStart(Bool_t change_cbox)
{
	TimedFrame::TimerStart();
	if (change_cbox)
	{
		TGCheckButton *cb = (TGCheckButton *)GetObject("toggleTimerButton");
		if (cb)
			cb->SetOn(GetTimerStatus());
	}
}

void BaseMonitorFrame::TimerStop(Bool_t change_cbox)
{
	TimedFrame::TimerStop();
	if (change_cbox)
	{
		TGCheckButton *cb = (TGCheckButton *)GetObject("toggleTimerButton");
		if (cb)
			cb->SetOn(GetTimerStatus());
	}
}

void BaseMonitorFrame::SetOnline(Bool_t online)
{
	if (online)
	{
		TimerStart(kTRUE);
	}
	else
	{
		TimerStop(kTRUE);
	}
}

void BaseMonitorFrame::ToggleIgnoreFileStatus()
{
	fIgnoreFileStatus = !fIgnoreFileStatus;
	TGCheckButton *cb = (TGCheckButton *)GetObject("toggleIgnoreFileStatusButton");
	if (cb)
		cb->SetOn(fIgnoreFileStatus);	
}

void BaseMonitorFrame::SetActive(Int_t i)
{
	//fMonitor->ToggleOption(i, ":active");

	cout << "[i] toggle active for cbox entry " << i << endl;

	TGComboBox *cb = (TGComboBox*)GetObject("fFileListCombo");
	TString s = cb->GetListBox()->GetEntry(i)->GetTitle(); // properly set via AddEntry
	//TString s = cb->GetListBox()->GetSelectedEntry()->GetTitle();
	FileMonitor *fm = fMonitor->GetMonitor(s.Data());
	if (fm)
	{
		fm->ToggleOption(":active");
		cout << "[i] toggle active for:" << fm->GetPath() << endl;
	}
	else
	{
		cerr << "[e] ::SetActive not found: " << s << endl;
	}
	UpdateFileList();
	OnTimer();
}

void BaseMonitorFrame::SetNTail(Int_t i)
{
	TGComboBox *cb = (TGComboBox*)GetObject("ftileListCombo");
	if (cb)
		cb->Select(i, kFALSE);
	fMonitor->SetNTail(i);
	cout << "[d] set tail " << i << endl;
	if (i > 100000)
	{
		cout << "[w] turning off live feature - large data size" << endl;
		TimerStop();
		TGCheckButton *cb = (TGCheckButton *)GetObject("toggleTimerButton");
		if (cb)
			cb->SetOn(GetTimerStatus());

	}
	UpdateFileList();
	OnTimer();
}

void BaseMonitorFrame::UserNTail(Int_t i)
{
	TGComboBox *cb = (TGComboBox*)GetObject("ftileListCombo");
	if (cb)
	{
		if (i == 0)
		{
			SetNTail(0);
		}
		else
		{
			cb->InsertEntry(TString::Format("User %d", i), i, -1);
			SetNTail(i);
		}
	}	
}

BaseMonitorFrame::~BaseMonitorFrame()
{
	;
}

void BaseMonitorFrame::Update()
{
	// Update Status Bar
	TString snow = Now();
	//cout << "[d] ::OnTimer" << endl;
	TGStatusBar *stat = (TGStatusBar*)GetObject("fStatusBar");
	if (stat)
	{
		TString stmp = TString::Format("N files monitored: %d Active: %d", fMonitor->GetList()->GetSize(), fMonitor->CountWithOption(":active"));
		stat->SetText(stmp, 0);
		stat->SetText(TString::Format("This Frame at: 0x%lx", (long int)this), 1);
		stat->SetText(TString("Latest update: ") + snow, 2);
	}
}

Bool_t BaseMonitorFrame::HasFileChanged()
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

void BaseMonitorFrame::OnTimer()
{
	Bool_t timerStat = GetTimerStatus();
	TimerStop();
	Update();
	if (HasFileChanged() || fIgnoreFileStatus == kTRUE)
	{
		//cout << "[d] calling ::ViewerUpdate because HasFileChanged returned TRUE..." << endl;
		ViewerUpdate();
	}
	if (timerStat == kTRUE)
		TimerStart();
}

void BaseMonitorFrame::UpdateFileList()
{
	TGComboBox *tb = (TGComboBox*)GetObject("fFileListCombo");
	if (tb)
	{
		tb->RemoveAll();
		for (Int_t i = 0; i < fMonitor->GetList()->GetSize(); i++)
		{
			TString spath = fMonitor->GetPath(i);
			cout << "[d] ::UpdateFileList +:" << spath << endl;
			Int_t nEnrties = tb->GetNumberOfEntries();
			tb->AddEntry(spath.Data(), nEnrties);
		}
		if (tb->GetNumberOfEntries() > 0)
		{
			tb->SetEnabled(kTRUE);
			tb->Select(fMonitor->GetList()->GetSize() - 1, kFALSE);
		}
		else
		{
			tb->AddEntry("No files to activate...", 0);
			tb->SetEnabled(kFALSE);
		}
	}
	fMonitor->ForceChanged();
}

void BaseMonitorFrame::DumpDebug()
{
	DumpList();
	cout << "[d] " << this->GetName() << endl;
	for (Int_t i = 0; i < fMonitor->GetList()->GetSize(); i++)
	{
		FileMonitor *fm = fMonitor->Get(i);
		cout << "    " <<  fm->GetPath () << " " << fm->GetOption() << endl;
		cout << "    dumping tail " << fm->GetNTail() << endl;
		fm->DumpContent(fm->GetNTail());
	}
}

void BaseMonitorFrame::AddFile(const char *fname)
{
	fMonitor->AddFile(fname, ":active");
	UpdateFileList();
	OnTimer();
}

void BaseMonitorFrame::RemoveFile(const char *fname)
{
	fMonitor->RemoveFile(fname);
	UpdateFileList();
	OnTimer();
}

void BaseMonitorFrame::SelectFileToMonitor()
{
    TGFileInfo file_info_;
    const char *filetypes[] = {"Any File", "*.*", 0, 0};
    file_info_.fFileTypes = filetypes;
    file_info_.fIniDir = StrDup(fInputDir.Data());	
	TGFileDialog *dial = new TGFileDialog(gClient->GetRoot(), this, kFDOpen, &file_info_);
	TString stmp = file_info_.fFilename;
	if (stmp.Length() > 0)
	{
		fMonitor->AddFile(file_info_.fFilename);
	}
	UpdateFileList();
	OnTimer();
}

void BaseMonitorFrame::DeSelectFileToMonitor()
{
    TGFileInfo file_info_;
    const char *filetypes[] = {"Any File", "*.*", 0, 0};
    file_info_.fFileTypes = filetypes;
    file_info_.fIniDir = StrDup(fInputDir.Data());	
	TGFileDialog *dial = new TGFileDialog(gClient->GetRoot(), this, kFDOpen, &file_info_);
	TString stmp = file_info_.fFilename;
	if (stmp.Length() > 0)
	{
		fMonitor->RemoveFile(file_info_.fFilename);
	}
	UpdateFileList();
	OnTimer();
}
