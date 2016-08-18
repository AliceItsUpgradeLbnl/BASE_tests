#ifndef __DecoderMonitorFrame_HH
#define __DecoderMonitorFrame_HH

#include "BaseMonitorFrame.h"

#include <TQObject.h>
#include <RQ_OBJECT.h>
#include <TGFrame.h>

class TGWindow;

class DecoderMonitorFrame : public BaseMonitorFrame
{
	RQ_OBJECT("DecoderMonitorFrame")
public:
	DecoderMonitorFrame (const TGWindow *p, const char *name, UInt_t options=0, Pixel_t back=GetDefaultFrameBackground());
	virtual 		~DecoderMonitorFrame();
	virtual void 	ViewerInit();
	virtual void    ViewerUpdate();
	virtual void    SetActiveHeader(Int_t h);
	virtual void 	SetActiveChannel(Int_t h);
	virtual void 	ToggleMissingTrailers();

protected:

	void 			UpdateGraphs();
	void 			SyncHeaders();

	std::vector<Bool_t> fChannelsSelected;

	Bool_t fMissingTrailers;

	ClassDef(DecoderMonitorFrame, 0)
};

#endif // __DecoderMonitorFrame_HH
