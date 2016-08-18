#include "GuiTestFrame.h"

ClassImp(GuiTestFrame);

#include <TTimer.h>

GuiTestFrame::GuiTestFrame (const TGWindow *p, UInt_t w, UInt_t h, UInt_t options, Pixel_t back)
: TGCompositeFrame(p, w, h, options, back)
, fTimer(0)
{
	fTimer = new TTimer();
	fTimer->Connect("Timeout()", "GuiTestFrame", this, "UpdateOnTimer()");
	fTimer->Start(2000, kFALSE);
}
