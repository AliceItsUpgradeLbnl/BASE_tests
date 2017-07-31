#ifndef __TABFRAME_HH
#define __TABFRAME_HH

#include "TimedFrame.h"

#include <TQObject.h>
#include <RQ_OBJECT.h>
#include <TGFrame.h>
#include <TSystem.h>

class TGWindow;
class FilesMonitor;

class TabFrame : public TimedFrame
{
	RQ_OBJECT("TabFrame")
public:
	TabFrame (const TGWindow *p=0, UInt_t w=1, UInt_t h=1, UInt_t options=0, Pixel_t back=GetDefaultFrameBackground());
	virtual 		~TabFrame();
	virtual void 	UpdateStatusBar();
	virtual void 	SelectFileToMonitor();
	virtual Bool_t 	HasFileChanged();
	virtual void 	OnTimer();
	virtual void 	Init();

private:

	Bool_t      	fLiveFlag;
	FilesMonitor 	*fMonitor;

	ClassDef(TabFrame, 0)
};

#endif // __TABFRAME_HH
