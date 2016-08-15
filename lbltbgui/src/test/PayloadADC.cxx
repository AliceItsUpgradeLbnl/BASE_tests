#include "PayloadADC.h"

#include <iomanip>
#include <iostream>
using namespace std;

#include "Masks.h"

PayloadADC::PayloadADC(const std::vector<unsigned int> &data, const unsigned int &index)
	: fADC()
	, fTimeStamp(0)
	, fIsValid(false)
	, fIsADC1(false)
	, fNWords(0)
{
	// decode here
	unsigned int iw     = index;
	unsigned int nwords = 0; // if ADC0
	if ( ! IS_ADC_HEAD(data[iw]) )
	{
		cout << "[error] ::PayloadADC : word: 0x" << hex << setfill('0') << setw(8) << data[iw] << " not a valid header" << endl;		
		return;
	}
	if (IS_ADC0_HEAD(data[iw]))
		nwords = 12;
	if (IS_ADC1_HEAD(data[iw]))
	{
		nwords = 1000 * 12;
		fIsADC1 = true;
	}
	if (nwords < 12)
	{
		return;
	}
	for (unsigned int i = 0; i < nwords; i++)
	{
		unsigned int d = data[iw + 1 + i];
		if (! IS_PAYLW(d) )
		{
			fIsValid = false;
			cout << "[error] ::PayloadADC : word: 0x" << hex << setfill('0') << setw(8) << d << " not a payload" << endl;
			return;
		}
	}

	unsigned int trail = data[iw + nwords + 1];
	if (! IS_ADC_TRAIL(trail))
	{
		fIsValid = false;
		cout << "[error] ::PayloadADC : word: 0x" << hex << setfill('0') << setw(8) << trail << " should be an ADC trailer" << endl;
		return;
	}
	else
	{
		cout << "[d] ::PayloadADC : correct ADC trailer found - for expected N words = " << nwords << endl;
	}

	fTimeStamp = TSTAMP(data[iw]);

	fIsValid = true;

	fADC.reserve(nwords / 12 * 32); //ensuring fADC vector is large enough 12 words and 32 bit/word?
	for (unsigned int i = 0; i < nwords / 12 * 32; i++)	fADC.push_back(0); //initilaizing vector to be all 0's
	//cout << "[debug] ::PayloadADC : fADC reserved: " << fADC.size() << endl;
	for (unsigned int i = 0; i < nwords; i++) 
	  /*   {
	        unsigned int d = data[iw + 1 + i]; 
		unsigned int isample = i / 12 * 32;
		unsigned int ioff = 0;

		if ((i) / 3 == 0) ioff = 0; 
		if ((i) / 3 == 1) ioff = 8;
		if ((i) / 3 == 2) ioff = 16;
		if ((i) / 3 == 3) ioff = 24;

		//cout << "[debug] i=" << i << " isample=" << isample << " ioff=" << ioff << endl;

		fADC[0+ioff+isample] = BIT_INT(d,  0, 11);
		fADC[1+ioff+isample] = BIT_INT(d, 12, 23);
		fADC[2+ioff+isample] = BIT_INT(d, 24, 27); //d0 4b,4b,12b,12b (right t o left)
		fADC[3+ioff+isample] = BIT_INT(d, 28, 31);

		fADC[2+ioff+isample] += BIT_INT(d,  0,  7) << 4; //must change payload words
		fADC[3+ioff+isample] += BIT_INT(d,  8, 15) << 4; //chamge to d0,d1,d2
		fADC[4+ioff+isample] += BIT_INT(d, 16, 23) << 4;
		fADC[4+ioff+isample] += BIT_INT(d, 24, 27);
		fADC[5+ioff+isample] += BIT_INT(d, 28, 31);

		fADC[5+ioff+isample] += BIT_INT(d,  0,  7) << 4; 
		fADC[6+ioff+isample] += BIT_INT(d,  8, 15) << 4;
		fADC[7+ioff+isample] += BIT_INT(d, 16, 23) << 4;
		fADC[6+ioff+isample] += BIT_INT(d, 24, 27);
		fADC[7+ioff+isample] += BIT_INT(d, 28, 31);
	    }*/ 


	  {     //split according to payload word format (see DAC data format)
	        unsigned int d0 = data[iw + 1 + i]; //4,4,12,12 (bits)

	
		unsigned int isample = i / 12 * 32;
		unsigned int ioff = 0;

		if ((i) / 3 == 0) ioff = 0; 
		if ((i) / 3 == 1) ioff = 8;
		if ((i) / 3 == 2) ioff = 16;
		if ((i) / 3 == 3) ioff = 24;

		//cout << "[debug] i=" << i << " isample=" << isample << " ioff=" << ioff << endl;
		if ((i) % 3 == 0) {
		fADC[0+ioff+isample] = BIT_INT(d0,  0, 11);
		fADC[1+ioff+isample] = BIT_INT(d0, 12, 23);
		fADC[2+ioff+isample] = BIT_INT(d0, 24, 27); //d0 4b,4b,12b,12b (right t o left)
		fADC[3+ioff+isample] = BIT_INT(d0, 28, 31);
		}
		
		if ((i) % 3 == 1) {
		fADC[2+ioff+isample] += BIT_INT(d0,  0,  7) << 4; //must change payload words
		fADC[3+ioff+isample] += BIT_INT(d0,  8, 15) << 4; //change to d0,d1,d2
		fADC[4+ioff+isample] += BIT_INT(d0, 16, 23) << 4;
		fADC[4+ioff+isample] += BIT_INT(d0, 24, 27);
		fADC[5+ioff+isample] += BIT_INT(d0, 28, 31);
		}

		if ((i) % 3 == 2) {
		fADC[5+ioff+isample] += BIT_INT(d0,  0,  7) << 4; 
		fADC[6+ioff+isample] += BIT_INT(d0,  8, 15) << 4;
		fADC[7+ioff+isample] += BIT_INT(d0, 16, 23) << 4;
		fADC[6+ioff+isample] += BIT_INT(d0, 24, 27);
		fADC[7+ioff+isample] += BIT_INT(d0, 28, 31);
		}
	    }	

	fNWords = nwords + 2; // total words read
}

double PayloadADC::GetADCf(unsigned int i, unsigned int evType)
{	
	unsigned int flag = 0;
	if (evType > 0)
		flag = 1; // voltage
	if (IS_COP_DCM_HEAD(evType) || IS_COP_ACM_HEAD(evType))
		flag = 2; // resistance
	double retval = fADC[i] * 1.;
	if (flag == 1)
	{
	 	retval = 2.5 * fADC[i] / 4096.;
	}	
	if (flag == 2)
	{
		double volts = 2.5 * fADC[i] / 4096.;
		if (volts != 0)
		{
			//retval = 1e6 * (1. / ( 3.3/volts - 1.));
			retval = (1. / ( (3.3/volts) - 1.));
		}
		else
		{
			retval = 0.0;
		}
	}
	return retval;
}

std::vector<double> PayloadADC::GetADCvf(unsigned int evType)
{
	std::vector<double> v;
	v.reserve(fADC.size());
	for (unsigned int i = 0; i < fADC.size(); i++)
	{
		double val = GetADCf(i, evType);
		v.push_back(val);
		//cout << "v[i] = "<< val << endl;  
	}
	return v;
}

void PayloadADC::Dump(unsigned int evType)
{
	cout << "[debug] ::PayloadADC::Dump() ..." << endl;
	std::vector<double> v = GetADCvf(evType);
	for (unsigned int i = 0; i < fADC.size(); i++)
	{
		cout << "        ADC# " << setw(4) << setfill(' ') << i%32 << " " << setw(7) << fADC[i] << "\t <=> " << setw(14) << v[i] << endl;
	}
}

