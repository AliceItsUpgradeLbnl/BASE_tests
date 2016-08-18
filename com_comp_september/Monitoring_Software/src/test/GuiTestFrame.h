#ifndef __BTLBLED_TEST_HH
#define __BTLBLED_TEST_HH

class TTimer;

#include <TGFrame.h>
#include <iostream>

class GuiTestFrame : public TGCompositeFrame
{
public:
	GuiTestFrame (const TGWindow *p=0, UInt_t w=1, UInt_t h=1, UInt_t options=0, Pixel_t back=GetDefaultFrameBackground());

	virtual ~GuiTestFrame() {;}

	void UpdateOnTimer()
	{
		std::cout << "this is the base class" << std::endl;
	}

private:

	TTimer *fTimer; //!

	ClassDef(GuiTestFrame, 0)
};

#endif // __BTLBLED_TEST_HH
