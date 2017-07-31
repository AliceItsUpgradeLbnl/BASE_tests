#include "TimedFrame.h"

#include <TTimer.h>
#include <TList.h>
#include <TDatime.h>
#include <TClass.h>

#include <iostream>
using namespace std;

ClassImp(TimedFrame);

TimedFrame::TimedFrame(const TGWindow *p, UInt_t w, UInt_t h, UInt_t options, Pixel_t back)
	: TGCompositeFrame(p, w, h, options, back)
	, fTimer(new TTimer)
	, fInterval(5000)
	, fTimerStatus(kFALSE)
	, fChildren(new TList)
{
	fChildren->SetOwner(kFALSE);
	fTimer->Connect("Timeout()", "TimedFrame", this, "OnTimer()");

	// alternative implementation (no signal-slot):
	// 1. reimplement
	// ::HandleTimer(TTimer *t)
	// 2. pass the pointer to the timer
	// fTimer->SetObject(this);

}

void TimedFrame::AddObject(TObject *o)
{
	if (o)
	{
		fChildren->Add(o);
	}
}

TimedFrame::~TimedFrame()
{
	delete fTimer;
	delete fChildren;
}

void TimedFrame::ToggleTimer()
{
	if (kTRUE == fTimerStatus)
	{
		TimerStop();
	}
	else
	{
		TimerStart();
	}
}

void TimedFrame::TimerStart()
{
	fTimer->Reset();
	fTimer->Start(fInterval, kFALSE);
	fTimerStatus = kTRUE;
}

void TimedFrame::TimerStop()
{
	fTimer->TurnOff();
	fTimer->Reset();
	fTimerStatus = kFALSE;
}

TString TimedFrame::MakeName(const char *cname)
{
	return TString::Format("%s_%s", GetName(), cname);
}

void TimedFrame::DumpList()
{
	cout << "[d] " << IsA()->GetName() << " ::DumpList [" << GetName() << "] dump fChildren" << endl;
	TIter next(fChildren);
	TObject *obj = 0;
	while ( ( obj = next() ) )
	{
		cout << "     "; 
		std::cout.width (30);
		cout << obj->GetName();
		cout << " ";
		std::cout.width (30);
		cout << obj->IsA()->GetName() << endl;
	}
}

void TimedFrame::DumpDebug()
{
	DumpList();
}

TString TimedFrame::Now()
{
	TDatime td;
	TString stime = TString::Format("%04d-%02d-%02d %02d:%02d:%02d", td.GetYear(), td.GetMonth(), td.GetDay(), td.GetHour(), td.GetMinute(), td.GetSecond());
	return stime;
}

void TimedFrame::OnTimer()
{
	cout << "[d] " << GetName() << "::OnTimer" << endl;
}

TObject *TimedFrame::GetObject(const char *cname)
{
	TString sname = this->MakeName(cname);
	return fChildren->FindObject(sname.Data());
}
