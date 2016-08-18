#ifndef __PayloadADC__HH
#define __PayloadADC__HH

#ifndef __CINT__

#include <vector>

class PayloadADC
{
public:
	PayloadADC(const std::vector<unsigned int> &data, const unsigned int &index = 0);
	virtual ~PayloadADC() 					{;}

	unsigned 	int 			GetTimeStamp() 			{return fTimeStamp;}
	unsigned 	int 			GetADC(unsigned int i) 	{return fADC[i];}
	         double 			GetADCf(unsigned int i, unsigned int evType = 0);
	std::vector<unsigned int> 	GetADC() 				{return fADC;}
	std::vector<double> 		GetADCvf(unsigned int evType = 0); 
	bool 						IsValid() 				{return fIsValid;}
	bool 						IsADC1() 				{return fIsADC1;}

	virtual void 				Dump(unsigned int evType = 0);

	unsigned 	int				GetNWords()				{return fNWords;}

private:
	PayloadADC() {;}

	std::vector<unsigned int> fADC;

	unsigned int fTimeStamp;

	bool fIsValid;
	bool fIsADC1;

	unsigned int fNWords; // this is the total words read = payload + head + trail
};

#endif

#endif // __PayloadADC__HH