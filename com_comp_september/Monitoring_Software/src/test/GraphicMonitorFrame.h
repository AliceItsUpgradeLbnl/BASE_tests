#ifndef __GraphicMonitorFrame_HH
#define __GraphicMonitorFrame_HH

#include "BaseMonitorFrame.h"

#include <TQObject.h>
#include <RQ_OBJECT.h>
#include <TGFrame.h>

class TGWindow;

class GraphicMonitorFrame : public BaseMonitorFrame
{
	RQ_OBJECT("GraphicMonitorFrame")
public:
	GraphicMonitorFrame (const TGWindow *p, const char *name, UInt_t options=0, Pixel_t back=GetDefaultFrameBackground());
	virtual 		~GraphicMonitorFrame();
	virtual void 	ViewerInit();
	virtual void    ViewerUpdate();

protected:

	void MakeHistograms();
	void UpdateGraphs();
	
	ClassDef(GraphicMonitorFrame, 0)
};

#endif // __GraphicMonitorFrame_HH
