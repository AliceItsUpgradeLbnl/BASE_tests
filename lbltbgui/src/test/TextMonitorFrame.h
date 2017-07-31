#ifndef __TextMonitorFrame_HH
#define __TextMonitorFrame_HH

#include "BaseMonitorFrame.h"

#include <TQObject.h>
#include <RQ_OBJECT.h>
#include <TGFrame.h>

class TGWindow;

class TextMonitorFrame : public BaseMonitorFrame
{
	RQ_OBJECT("TextMonitorFrame")
public:
	TextMonitorFrame (const TGWindow *p, const char *name, UInt_t options=0, Pixel_t back=GetDefaultFrameBackground());
	virtual 		~TextMonitorFrame();
	virtual void 	ViewerInit();
	virtual void    ViewerUpdate();

protected:

	ClassDef(TextMonitorFrame, 0)
};

#endif // __TextMonitorFrame_HH
