#ifndef __Decoder__HH
#define __Decoder__HH

#ifndef __CINT__

#include "Masks.h"
#include "PayloadADC.h"
#include "FileMonitor.h"

#include <vector>
#include <string>

class TGraph;
class Decoder : public BinaryFileMonitor
{
public:
	Decoder(const char *path);
	Decoder(const char *path, const char *option);

	virtual ~Decoder();

	TGraph *GetGraph(unsigned int iadc);

	virtual Bool_t 	Update();
	void           	SetHeader(unsigned int h);
	virtual void   	DumpContent(unsigned int nwords = -1);
	void			ResetData();
	virtual void	SetNTail(unsigned int n);

	virtual void 	SetMissingTrailers(Bool_t val) { fMissingTrailers = val;

	 
}

private:

	std::string DbgS(const char *s = "d");

	//0xC3000009
	Bool_t OnlineMonitoringADCRO();
	//0xAF000002
	Bool_t AmplifiersDCscan();
	//0xC1000005
	Bool_t ComparatorDCscan();
	//0x0D000004
	Bool_t DACtest();
	//0xCD000003
	Bool_t DigitalPotentiometerDCtest();
	//0xCD000013
	Bool_t DigitalPotentiometerACtest();

	Bool_t NextEvent(unsigned int &ibegin, unsigned int &iend);
	Bool_t LoadADCdata(unsigned int iend);

	bool SeekHeader();
	bool SeekTrailer();
	bool SeekADCHeader(unsigned int iend);

	void InsertMarker();

	unsigned int 	fPos;
	unsigned int 	fHeader;
	bool 			fError;

	std::vector<double> fX; //by default this is a time stamp or n-read register * DAC (0.25mV)
	std::vector<double> fXDAC; //this can be a DAC value - 0-1 (1/8 steps) or 0-2.5V (25mV steps)
	std::vector<double> fY[32]; // in case ADC

	TGraph 			*fGraph[32];//!
	TGraph			*fDummyGraph;//

	Bool_t 			fMissingTrailers;
};

#endif

#endif // __Decoder__HH
