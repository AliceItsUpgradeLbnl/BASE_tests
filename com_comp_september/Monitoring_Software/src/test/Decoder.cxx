#include "Decoder.h"

#include <TGraph.h>
#include <TAxis.h>

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;
short Xmarker;

Decoder::Decoder(const char *fname)
	: BinaryFileMonitor(fname)
	, fPos(0)
	, fHeader(0x0)
	, fError(false)
	, fX()
	, fXDAC()
	, fY()
	, fGraph()
	, fDummyGraph(0)
	, fMissingTrailers(kFALSE)
{
	for (unsigned int i = 0; i < 32; i++) fGraph[i] = 0;

	double x[] = {-1, 1};
	double y[] = { 0, 0};
	fDummyGraph = new TGraph(2, x, y);
	fDummyGraph->SetName("empty");
	fDummyGraph->SetTitle("no data");
	fDummyGraph->GetXaxis()->SetTitle("no label");
	fDummyGraph->GetYaxis()->SetTitle("no label");	
}

Decoder::Decoder(const char *fname, const char *option)
	: BinaryFileMonitor(fname, option)
	, fPos(0)
	, fHeader(0x0)
	, fError(false)
	, fX()
	, fXDAC()
	, fY()
	, fGraph()
	, fDummyGraph(0)
	, fMissingTrailers(kFALSE)
{
	for (unsigned int i = 0; i < 32; i++) fGraph[i] = 0;

	double x[] = {-1, 1};
	double y[] = { 0, 0};
	fDummyGraph = new TGraph(2, x, y);
	fDummyGraph->SetName("empty");
	fDummyGraph->SetTitle("no data");
	fDummyGraph->GetXaxis()->SetTitle("no label");
	fDummyGraph->GetYaxis()->SetTitle("no label");	
}

Decoder::~Decoder()
{
	delete fDummyGraph;
}

string Decoder::DbgS(const char *s)
{
	stringstream ss;
	ss << "[" << s << "] [0x" << hex << fHeader << "] " << dec;
	return ss.str();
}

void Decoder::DumpContent(unsigned int nwords)
{
	DumpContentWords(nwords);
}

Bool_t Decoder::Update()
{
	Bool_t b0 = BinaryFileMonitor::Update();
	Bool_t b1 = kFALSE;
	// re-read data
	std::vector<unsigned int> w = Words(&fContent[0], fContent.size());

	if (IS_COP_OMADCRO_HEAD(fHeader))
		b1 = OnlineMonitoringADCRO();

	if (IS_COP_ADCS_HEAD(fHeader))
		b1 = AmplifiersDCscan();

	if (IS_COP_CDCS_HEAD(fHeader))
		b1 = ComparatorDCscan();

	if (IS_COP_DACDCS_HEAD(fHeader))
		b1 = DACtest();

	if (IS_COP_DCM_HEAD(fHeader))
		b1 = DigitalPotentiometerDCtest();

	if (IS_COP_ACM_HEAD(fHeader))
		b1 = DigitalPotentiometerACtest();

	return b0 && b1;
}

TGraph *Decoder::GetGraph(unsigned int i)
{
	//0xc3000009
	if (IS_COP_OMADCRO_HEAD(fHeader))
	{
		unsigned int iadc = i;
		if (iadc>=32) return 0x0;
		if (fGraph[iadc])
		{
			delete fGraph[iadc];
			fGraph[iadc] = 0x0;
		}
		if (fX.size() > 0 && fY[iadc].size() > 0)
		{
			cout << "[d] " << GetPath() << endl;
			cout << "	 ::GetGraph n points: " << fX.size() << endl;
			fGraph[i] = new TGraph(fX.size(), &fX[0], &fY[iadc][0]);
			fGraph[i]->SetName(TString::Format("ADC_%d_0x%x", iadc, fHeader));
			fGraph[i]->SetTitle(TString::Format("ADC #%d [0x%x] ", iadc, fHeader));
			fGraph[i]->GetXaxis()->SetTitle("Sample Number");
			fGraph[i]->GetYaxis()->SetTitle("ADC (V)");
		}
		else
		{
			fDummyGraph->SetTitle(TString::Format("ADC #%d [0x%x] NO DATA", iadc, fHeader));
			return fDummyGraph;
		}
	}

	//0xaf000002
	if (IS_COP_ADCS_HEAD(fHeader))
	{
		unsigned int iadc = i;
		if (iadc>=32) return 0x0;
		if (fGraph[iadc])
		{
			delete fGraph[iadc];
			fGraph[iadc] = 0x0;
		}
		if (fX.size() > 0 && fY[iadc].size() > 0)
		{
			cout << "[d] " << GetPath() << endl;
			cout << "	 ::GetGraph n points: " << fXDAC.size() << endl;
			fGraph[i] = new TGraph(fXDAC.size(), &fXDAC[0], &fY[iadc][0]);
			fGraph[i]->SetName(TString::Format("ADC_%d_0x%x", iadc, fHeader));
			fGraph[i]->SetTitle(TString::Format("ADC #%d [0x%x] ", iadc, fHeader));
			fGraph[i]->GetXaxis()->SetTitle("DAC (V) [plotted % 5(V)]");
			fGraph[i]->GetYaxis()->SetTitle("ADC (V)");
		}
		else
		{
			fDummyGraph->SetTitle(TString::Format("ADC #%d [0x%x] NO DATA", iadc, fHeader));
			return fDummyGraph;
		}
	}

	//0xc1000005
	if (IS_COP_CDCS_HEAD(fHeader))
	{
		unsigned int iadc = i;
		if (iadc>=32) return 0x0;
		if (fGraph[iadc])
		{
			delete fGraph[iadc];
			fGraph[iadc] = 0x0;
		}
		if (fX.size() > 0 && fY[iadc].size() > 0)
		{
			cout << "[d] " << GetPath() << endl;
			cout << "	 ::GetGraph n points: " << fXDAC.size() << endl;
			fGraph[i] = new TGraph(fXDAC.size(), &fXDAC[0], &fY[iadc][0]);
			fGraph[i]->SetName(TString::Format("REG_%d_0x%x", iadc, fHeader));
			fGraph[i]->SetTitle(TString::Format("REG #%d [0x%x] ", iadc, fHeader));
			//fGraph[i]->GetXaxis()->SetTitle("n read / DAC");
			fGraph[i]->GetXaxis()->SetTitle("DAC (V) [plotted % 5(V)]");
			fGraph[i]->GetYaxis()->SetTitle("Register bit");
		}
		else
		{
			fDummyGraph->SetTitle(TString::Format("ADC #%d [0x%x] NO DATA", iadc, fHeader));
			return fDummyGraph;
		}
	}

	//0x0d000004
	if (IS_COP_DACDCS_HEAD(fHeader))
	{
		unsigned int iadc = i;
		if (iadc>=32) return 0x0;
		if (fGraph[iadc])
		{
			delete fGraph[iadc];
			fGraph[iadc] = 0x0;
		}
		if (fX.size() > 0 && fY[iadc].size() > 0)
		{
			cout << "[d] " << GetPath() << endl;
			cout << "	 ::GetGraph n points: " << fXDAC.size() << endl;
			fGraph[i] = new TGraph(fXDAC.size(), &fXDAC[0], &fY[iadc][0]);
			fGraph[i]->SetName(TString::Format("ADC_%d_0x%x", iadc, fHeader));
			fGraph[i]->SetTitle(TString::Format("ADC #%d [0x%x] ", iadc, fHeader));
			fGraph[i]->GetXaxis()->SetTitle("DAC (V) [plotted % 5(V)]");
			fGraph[i]->GetYaxis()->SetTitle("ADC (V)");
		}
		else
		{
			fDummyGraph->SetTitle(TString::Format("ADC #%d [0x%x] NO DATA", iadc, fHeader));
			return fDummyGraph;
		}
	}

	//0xcd000003
	if (IS_COP_DCM_HEAD(fHeader))
	{
		unsigned int iadc = i;
		if (iadc>=32) return 0x0;
		if (fGraph[iadc])
		{
			delete fGraph[iadc];
			fGraph[iadc] = 0x0;
		}
		if (fX.size() > 0 && fY[iadc].size() > 0)
		{
			cout << "[d] " << GetPath() << endl;
			cout << "	 ::GetGraph n points: " << fXDAC.size() << endl;
			fGraph[i] = new TGraph(fXDAC.size(), &fXDAC[0], &fY[iadc][0]);
			fGraph[i]->SetName(TString::Format("ADC_%d_0x%x", iadc, fHeader));
			fGraph[i]->SetTitle(TString::Format("ADC #%d [0x%x] ", iadc, fHeader));
			fGraph[i]->GetXaxis()->SetTitle("DAC [samples plotted with gap of 1]");
			fGraph[i]->GetYaxis()->SetTitle("R");
		}
		else
		{
			fDummyGraph->SetTitle(TString::Format("ADC #%d [0x%x] NO DATA", iadc, fHeader));
			return fDummyGraph;
		}
	}

	//0xcd000013
	if (IS_COP_ACM_HEAD(fHeader))
	{
		unsigned int iadc = i;
		if (iadc>=32) return 0x0;
		if (fGraph[iadc])
		{
			delete fGraph[iadc];
			fGraph[iadc] = 0x0;
		}
		if (fX.size() > 0 && fY[iadc].size() > 0)
		{
			cout << "[d] " << GetPath() << endl;
			cout << "	 ::GetGraph n points: " << fXDAC.size() << endl;
			fGraph[i] = new TGraph(fXDAC.size(), &fXDAC[0], &fY[iadc][0]);
			fGraph[i]->SetName(TString::Format("ADC_%d_0x%x", iadc, fHeader));
			fGraph[i]->SetTitle(TString::Format("ADC #%d [0x%x] ", iadc, fHeader));
			fGraph[i]->GetXaxis()->SetTitle("DAC [samples plotted with gap of 1]");
			fGraph[i]->GetYaxis()->SetTitle("R");
		}
		else
		{
			fDummyGraph->SetTitle(TString::Format("ADC #%d [0x%x] NO DATA", iadc, fHeader));
			return fDummyGraph;
		}
	}

	return fGraph[i];
}

//bool Decoder::SeekHeader()
//{
//	if (fWords.size() <= 0) return false;
//	while ((fPos < fWords.size()) && (! IS_COP_OMADCRO_HEAD(fWords[fPos])) )
//		fPos++;
//	return IS_COP_OMADCRO_HEAD(fWords[fPos]);
//}

bool Decoder::SeekHeader()
{
	if (fWords.size() <= 0) return false;
	while ((fPos < fWords.size()))
	{ 
		//if (IS_HEADW(fWords[fPos]) && fWords[fPos] == fHeader)
		if (fWords[fPos] == fHeader)
			break;
		fPos++;
	}
	return (fWords[fPos] == fHeader);	
}

bool Decoder::SeekTrailer()
{
	if (fWords.size() <= 0) return false;
	while ((fPos < fWords.size()) && (! IS_CO_TRAIL(fWords[fPos])) )
		fPos++;
	return IS_CO_TRAIL(fWords[fPos]);
}

bool Decoder::SeekADCHeader(unsigned int iend)
{
	if (iend <= 0) return false;
	if (iend > fWords.size()) return false;
	while ( (fPos < iend) && (! IS_ADC_HEAD(fWords[fPos])) )
	{
		fPos++;
	}
	return IS_ADC_HEAD(fWords[fPos]);
}

void Decoder::SetNTail(unsigned int n) 
{
	fNTail = n;
	ResetData();
}

void Decoder::SetHeader(unsigned int h) 
{
	if (fHeader != h)
	{
		ResetData();
		fHeader = h;
	}
}

void Decoder::ResetData()
{
	fPos = 0; //start from begining of the buffer
	for (unsigned int iadc = 0; iadc < 32; iadc++) fY[iadc].clear();
	fX.clear();	
	fXDAC.clear();	
	for (unsigned int i = 0; i < 32; i++) 
		{
			delete fGraph[i];
			fGraph[i] = 0;
		}
}

Bool_t Decoder::NextEvent(unsigned int &ibegin, unsigned int &iend)
{
	Bool_t retval = kFALSE;
	if (SeekHeader())
	{
		cout << DbgS() << "header 0x" << hex << fWords[fPos] << " found at " << dec << fPos << endl;
		ibegin 	= fPos;
		iend  	= fPos;
		if (SeekTrailer())
		{
			cout << DbgS() << "trailer 0x" << hex << fWords[fPos] << " found at " << dec << fPos << endl;
			iend = fPos;
			fPos = ibegin + 1;
			retval = kTRUE;
		}
		else
		{
			if (fMissingTrailers == kFALSE)
			{
				cout << "[e] missing trailer." << endl;
				retval = kFALSE;
			}
			else
			{
				cout << "[w] ignoring missing trailer..." << endl;
				iend = fPos;
				fPos = ibegin + 1;
				retval = kTRUE;
			}
		}
	}
	return retval;
}

Bool_t Decoder::LoadADCdata(unsigned int iend)
{
	unsigned int nValidADCdata = 0;
	unsigned int nsample = 0;
	double xOffset = 5.;
	if (IS_COP_ADCS_HEAD(fHeader) || IS_COP_CDCS_HEAD(fHeader) || IS_COP_DACDCS_HEAD(fHeader))
		xOffset = 5.;
	if (IS_COP_DCM_HEAD(fHeader))
		xOffset = 2.;
	if (IS_COP_ACM_HEAD(fHeader))
		xOffset = 2.;

	double DACoffset = 0;
	if (fXDAC.size() > 0)
		DACoffset = int(fXDAC[fXDAC.size() - 1] / xOffset + 1.) * xOffset; // should make it xOffset
	while (SeekADCHeader(iend))
	{
		PayloadADC l(fWords, fPos);
		if (l.IsValid())
		{
			//cout << DbgS() << "valid ADC data read Nwords: " << dec << l.GetNWords() << endl;
			fPos += l.GetNWords();
			nValidADCdata++;
			double DACval = l.GetTimeStamp() * 1.;
			cout << DbgS() << " decoded TStamp: " << l.GetTimeStamp() << endl;
			if (IS_COP_ADCS_HEAD(fHeader) || IS_COP_CDCS_HEAD(fHeader) || IS_COP_DACDCS_HEAD(fHeader))
			{
				if (nsample > 10)
				{
					nsample = 0;
				}
				if (nsample == 0)
				{
					if (fXDAC.size() > 0)
					{
						DACoffset = int(fXDAC[fXDAC.size() - 1] / xOffset + 1.) * xOffset; // should make it xOffset
					}
					else
					{
						DACoffset = 0;
					}
				}
				//cout << "DACoffset = " << DACoffset << endl;
				DACval = DACoffset + nsample * 0.25; 
			}
			if (IS_COP_DCM_HEAD(fHeader))
			{
				if (nsample > 8)
				{
					nsample = 0;
				}
				if (nsample == 0)
				{
					if (fXDAC.size() > 0)
					{
						DACoffset = int(fXDAC[fXDAC.size() - 1]) + xOffset; // should make it xOffset
					}
					else
					{
						DACoffset = 0;
					}
				}
				DACval = DACoffset + nsample * 1./8.; 
			}
			if (IS_COP_ACM_HEAD(fHeader))
			{
				// here sample is an actual bunch
				if (nsample > 25)
				{
					nsample = 0;
				}
				if (fXDAC.size() > 0)
				{
					if (nsample == 0)
					{
						DACoffset = int(fXDAC[fXDAC.size() - 1]) + xOffset; // should make it xOffset
					}
					else
					{
						DACoffset = int(fXDAC[fXDAC.size() - 1]) + 1.0; // should make it xOffset
					}
				}
				else
				{
					DACoffset = 0;
				}
				// decode bunches and samples
				// bunch of 10000 ADC samples...
				std::vector<double> v = l.GetADCvf(fHeader);
				cout << DbgS() << " bunch " << nsample << " of 1000 " << endl;
				cout << DbgS() << " number of samples per ADC: " << v.size() << " / 32 = " << v.size() / 32 << endl;
				unsigned int nsub_samples = v.size() / 32;
				for (unsigned int issample = 0; issample < nsub_samples; issample++)
				{
				        fX.push_back(l.GetTimeStamp() * 1.0 + 0.001 * issample);
			     		DACval = DACoffset + issample * 1./8.; 
					fXDAC.push_back(DACval);
					//cout << DbgS() << " nsample (of 26) = " << nsample << " DACoffset = " << DACoffset << " DACval =" << DACval 
					//	<< " sub-sample = " << issample << " (of 1000) " << endl;
					for (int iadc = 0; iadc < 32; iadc++)
					{
						double val = v[issample * 32 + iadc];
						fY[iadc].push_back(val);
						cout << "adc=" << iadc << "val =" << val << endl ;
						//cout << DbgS() << " adc=" << iadc << " val=" << val << " idx=" << issample * 32 + iadc 
						//	<< " (" << v.size() << ")" << endl;
					}
				}
			}
			else
			{
				// standard - n-samples on all channels
				// add the XY data
				//fX.push_back(l.GetTimeStamp() * 1.0);
				fX.push_back(Xmarker);
				fXDAC.push_back(DACval);
				std::vector<double> v = l.GetADCvf(fHeader);
				cout << DbgS() << " nsample: " << nsample << " DACoffset= " << DACoffset << " DACval=" << DACval << endl;
				cout << DbgS() << " number of samples per ADC: " << v.size() << " / 32 = " << v.size() / 32 << endl;
				if (v.size() > 32)
				{
					cout << DbgS() << "[error] trouble ahead... size of decoded values exceeds 32!" << endl;
				}
				for (unsigned int iadc = 0; iadc < v.size(); iadc++)
				{
					fY[iadc%32].push_back(v[iadc]);
					//cout << DbgS() << " adc=" << iadc << " val=" << v[iadc] << " idx=" << iadc << endl;
				}				
			}
		}
		else
		{
			cout << DbgS() << "invalid ADC data read... context:" << endl;
			DumpContentWords(fPos, fPos + 2);
			fPos += 1; // advance anyways
		}
		nsample++;
		Xmarker++;
	} // while
    cout << DbgS() << "N valid ADC data: " << nValidADCdata << endl;
    if (nValidADCdata > 0)
    	return kTRUE;
    return kFALSE;
}

void Decoder::InsertMarker()
{
	double lastx = 0;
	if (fX.size() > 0)
		lastx = fX[fX.size() - 1];
	fX.push_back(lastx + .1); //Just making the last element .1 greater?

	double lastxDAC = 0;
	if (fXDAC.size() > 0)
		lastxDAC = fXDAC[fXDAC.size() - 1];
	fXDAC.push_back(lastxDAC + .1);

	for (unsigned int iadc = 0; iadc < 32; iadc++) 
	fY[iadc].push_back(-1); //why the final GUI has things go to -1. -grepmarker-
}

//0xC3000009
Bool_t Decoder::OnlineMonitoringADCRO()
{
	unsigned int ibegin = 0;
	unsigned int iend   = 0;
	Bool_t retval = kFALSE;
	while (NextEvent(ibegin, iend))
	{
		cout << DbgS() << "[d] begin at " << ibegin << " end at " << iend << endl;
		// load the ADC data
		if (LoadADCdata(iend))
			retval = kTRUE;
		//	InsertMarker(); //wanted to stop plot from going to -1. edit 8/4/16
	}
	return retval;
}

//0xAF000002
Bool_t Decoder::AmplifiersDCscan()
{
	unsigned int ibegin = 0;
	unsigned int iend   = 0;
	Bool_t retval = kFALSE;
	while (NextEvent(ibegin, iend))
	{
		cout << DbgS() << "[d] begin at " << ibegin << " end at " << iend << endl;
		// load the ADC data
		if (LoadADCdata(iend))
			retval = kTRUE;
		//		InsertMarker();
	}
	return retval;
}

//0xC1000005
Bool_t Decoder::ComparatorDCscan()
{
	double xOffset = 5.;
	unsigned int ibegin = 0;
	unsigned int iend   = 0;
	Bool_t retval = kFALSE;
	while (NextEvent(ibegin, iend))
	{
		cout << DbgS() << "begin at " << ibegin << " end at " << iend << endl;
		cout << DbgS() << "expecing 11 samples - got words: " << iend - ibegin - 2 << endl;

		unsigned int nsample = 0;
		double DACoffset = 0;
		if (fXDAC.size() > 0)
			DACoffset = int(fXDAC[fXDAC.size() - 1] / xOffset + 1.) * xOffset; // should make it xOffset

		while (fPos < iend)
		{
			if (nsample > 10)
			{
				nsample = 0;
				DACoffset = int(fXDAC[fXDAC.size() - 1] / xOffset + 1.) * xOffset; // should make it xOffset
			}
			cout << DbgS() << "DACoffset = " << DACoffset << endl;
			double DACval = DACoffset + nsample * 0.25; 

			unsigned int w = fWords[fPos];
			if (IS_FWREG(w))
			{
				fX.push_back(fX.size());
				fY[0].push_back(ADCMP603_Q_OUT(w));
				fY[1].push_back(ADCMP603_Q_BAR_OUT(w));
				fY[2].push_back(ADCMP603_LE_HYS_OUT(w));
				fXDAC.push_back(DACval);
			}
			else
			{
				cout << DbgS() << "[e] expected a firmware register but got 0x" 
					<< hex << w << " at " << dec << fPos << endl;
			}
			fPos++;
			nsample++;
		}// while
		InsertMarker();
	}
	return retval;

}

//0x0D000004
Bool_t Decoder::DACtest()
{
	unsigned int ibegin = 0;
	unsigned int iend   = 0;
	Bool_t retval = kFALSE;
	while (NextEvent(ibegin, iend))
	{
		cout << DbgS() << "[d] begin at " << ibegin << " end at " << iend << endl;
		// load the ADC data
		if (LoadADCdata(iend))
			retval = kTRUE;
		InsertMarker();
	}
	return retval;
}

//0xCD000003
Bool_t Decoder::DigitalPotentiometerDCtest()
{
	unsigned int ibegin = 0;
	unsigned int iend   = 0;
	Bool_t retval = kFALSE;
	while (NextEvent(ibegin, iend))
	{
		cout << DbgS() << "[d] begin at " << ibegin << " end at " << iend << endl;
		// load the ADC data
		if (LoadADCdata(iend))
			retval = kTRUE;
		InsertMarker();		
	}
	return retval;
}

//0xCD000013
Bool_t Decoder::DigitalPotentiometerACtest()
{
	unsigned int ibegin = 0;
	unsigned int iend   = 0;
	Bool_t retval = kFALSE;
	while (NextEvent(ibegin, iend))
	{
		cout << DbgS() << "[d] begin at " << ibegin << " end at " << iend << endl;
		// load the ADC data
		if (LoadADCdata(iend))
			retval = kTRUE;
		InsertMarker();
	}
	return retval;
}
