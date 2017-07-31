#include "TextMonitorFrame.h"

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
#include <TGTextView.h>

#include "HistoList.h"

#include "FileMonitor.h"

ClassImp(TextMonitorFrame);

#include <iostream>
using namespace std;

TextMonitorFrame::TextMonitorFrame (const TGWindow *p, const char *name, UInt_t options, Pixel_t back)
	: BaseMonitorFrame(p, name, options, back)
{
	fMonitor->SetOption(":text");
}

TextMonitorFrame::~TextMonitorFrame()
{
	;
}

void TextMonitorFrame::ViewerInit()
{
	TGTextView *fTextView = new TGTextView(this,100,100,kSunkenFrame| kLHintsExpandX | kLHintsExpandY);
	fTextView->SetName(MakeName("fTextView"));
	this->AddFrame(fTextView, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
	AddObject(fTextView);
}

void TextMonitorFrame::ViewerUpdate()
{
	TGTextView *tx = (TGTextView*)GetObject("fTextView");
	if (tx)
	{
		for (Int_t i = 0; i < fMonitor->GetSize(); i++)
		{
			//cout << "[i] TextMonitorFrame::ViewerUpdate Loading File:" << fMonitor->Get(i)->GetPath() << endl;
			//tx->LoadFile(fMonitor->Get(i)->GetPath());
			TextFileMonitor *fm = (TextFileMonitor*)fMonitor->Get(i);
			if (!fm)
			{
				cerr << "[e] Not a text file monitor: " << fMonitor->Get(i)->GetPath() << endl;
				continue;
			}
			if (fm->HasOption(":active"))
			{
				if (fm->Update() == kTRUE)
				{
					tx->GetText()->Clear();
				}
				vector<string> v = fm->GetUpdate();
				for (unsigned int il = 0; il < v.size(); il++)
				{
					tx->AddLineFast(v[il].c_str());
				}
			}
			tx->Update();
			tx->ShowBottom();
		}
	}
}
