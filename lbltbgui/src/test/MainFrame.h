#ifndef __MAIN_FRAME_HH
#define __MAIN_FRAME_HH

#include <TGFrame.h>
#include <RQ_OBJECT.h>

class TGTab;
class TextMonitorFrame;
class GraphicMonitorFrame;
class DecoderMonitorFrame;
class Options;

class MainFrame : public TGMainFrame
{
	RQ_OBJECT("MainFrame")
public:
	MainFrame(const TGWindow *p=0, UInt_t w=1, UInt_t h=1, UInt_t options=kVerticalFrame);
	virtual 	~MainFrame();
	virtual void CloseWindow();

	TextMonitorFrame *		AddTextMonitorFrame(const char *tname = 0);
	GraphicMonitorFrame * 	AddGraphicMonitorFrame(const char *tname = 0);
	DecoderMonitorFrame * 	AddTestBeamMonitorFrame(const char *tname = 0);

	TGTab *GetTab() {return fTab;}

	void HandleOptions(Options *opts);

private:

	// tab widget
	TGTab 	*fTab;//!
	TString fInputDir;
	Bool_t	fOnlineFlag;
	ClassDef(MainFrame, 0)
};

#endif // __MAIN_FRAME_HH