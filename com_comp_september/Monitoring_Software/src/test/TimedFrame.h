#ifndef __TIMEDFRAME__HH
#define __TIMEDFRAME__HH

#include <TNamed.h>
#include <TString.h>

#include <TGFrame.h>
#include <TQObject.h>
#include <RQ_OBJECT.h>

class TTimer;
class TList;
class TGWindow;

class TimedFrame : public TGCompositeFrame
{
	RQ_OBJECT("TimedFrame")
public:
	TimedFrame (const TGWindow *p=0, UInt_t w=1, UInt_t h=1, UInt_t options=0, Pixel_t back=GetDefaultFrameBackground());
	virtual 		~TimedFrame();
	virtual void 	 ToggleTimer();
	virtual void 	 TimerStart();
	virtual void 	 TimerStop();
	virtual void	 OnTimer();
	virtual void     Init() = 0;

	virtual TString  MakeName(const char *cname);
	virtual Bool_t 	 GetTimerStatus() {return fTimerStatus;}
	virtual TObject* GetObject(const char *cname);

	virtual void 	 DumpList();
	virtual void 	 DumpDebug();
	virtual void	 SetTimerInterval(Int_t i) {fInterval = i;}
	virtual void     AddObject(TObject *o);

	static TString   Now();

private:
	TTimer * 	fTimer; //!
	Int_t       fInterval;
	Bool_t		fTimerStatus;
	TList *		fChildren; //!
	ClassDef(TimedFrame, 1)
};

#endif //__TIMEDFRAME__HH
