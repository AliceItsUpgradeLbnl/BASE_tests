#ifndef __BaseMonitorFrame_HH
#define __BaseMonitorFrame_HH

#include "TimedFrame.h"

#include <TQObject.h>
#include <RQ_OBJECT.h>
#include <TGFrame.h>
#include <TSystem.h>

class TGWindow;
class FilesMonitor;

class BaseMonitorFrame : public TimedFrame
{
	RQ_OBJECT("BaseMonitorFrame")
public:
	BaseMonitorFrame (const TGWindow *p, const char *name, UInt_t options=0, Pixel_t back=GetDefaultFrameBackground());
	virtual 		~BaseMonitorFrame();
	virtual void    Init();
	virtual void 	ViewerInit() = 0;
	virtual void    ViewerUpdate() = 0;
	virtual void 	ToggleTimer();
	virtual void 	TimerStart(Bool_t change_cbox = kFALSE);
	virtual void 	TimerStop(Bool_t change_cbox = kFALSE);
	virtual void 	SetOnline(Bool_t online);

	virtual void 	SetActive(Int_t i);
	virtual void    SetNTail(Int_t i);
	virtual void    UserNTail(Int_t i);
	virtual void 	Update();
	virtual void 	UpdateFileList();
	virtual void 	SelectFileToMonitor();
	virtual void 	DeSelectFileToMonitor();
	virtual Bool_t 	HasFileChanged();
	virtual void 	OnTimer();

	virtual void	DumpDebug();

	virtual void AddFile(const char *fname);
	virtual void RemoveFile(const char *fname);

	virtual void ToggleIgnoreFileStatus();

	virtual void SetInputDir(const char *sdir) {fInputDir = sdir;}
	
protected:

	Bool_t			fIgnoreFileStatus;
	TString			fInputDir;
	FilesMonitor 	*fMonitor;

	ClassDef(BaseMonitorFrame, 0)
};

#endif // __BaseMonitorFrame_HH
