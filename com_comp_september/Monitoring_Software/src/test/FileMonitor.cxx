#include "FileMonitor.h"

#include <TList.h>
#include <TGraph.h>
#include <TAxis.h>

#include <climits> /* PATH_MAX */
#include <cstdlib>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "Masks.h"
#include "PayloadADC.h"
#include "Decoder.h"

using namespace std;

ClassImp(FileMonitor);

FileMonitor::FileMonitor(const char * path)
: TObject()
, fPath(path)
, fOption("")
, fFStat()
, fNTail(10000)
, fForceChanged(kFALSE)
, fPendingUpdate(kFALSE)
{
	;
}

FileMonitor::FileMonitor(const char * path, const char *option)
: TObject()
, fPath(path)
, fOption(option)
, fFStat()
, fNTail(10000)
, fForceChanged(kFALSE)
, fPendingUpdate(kFALSE)
{
	;
}

Bool_t FileMonitor::HasOption(const char *opt)
{
	return fOption.Contains(opt);
}

void FileMonitor::ToggleOption(const char *opt)
{
	if (HasOption(opt))
	{
		fOption.ReplaceAll(opt, "");
	}
	else
	{
		fOption += TString(opt);
	}
}

Bool_t FileMonitor::IsValid()
{
	FileStat_t stattmp;
	int retval = gSystem->GetPathInfo(fPath.Data(), stattmp);
	if (retval == 0)
		return kTRUE;
	return kFALSE;
}

Bool_t FileMonitor::HasChanged()
{
	if (fForceChanged)
	{
		fForceChanged  = kFALSE;
		fPendingUpdate = kTRUE;
		return kTRUE;
	}

	if (!HasOption(":active"))
		return kFALSE;

	Bool_t changed = kFALSE;
	if (fPath.Length() <= 0 || IsValid() == kFALSE)
		return changed;

	FileStat_t stattmp;
	int retval = gSystem->GetPathInfo(fPath.Data(), stattmp);
	if (retval == 0)
	{
	        //cout << "[i] FileMonitor::HasChanged " << fPath << endl;
		if (stattmp.fMtime != fFStat.fMtime)
		{
			changed = kTRUE;
			//cout << stattmp.fMtime << "    Mtime changed..." << endl;
		}
		if (stattmp.fIno != fFStat.fIno)
		{
			changed = kTRUE;
			//cout << stattmp.fIno << "    Ino changed..." << endl;
		}
		if (stattmp.fSize != fFStat.fSize)
		{
			changed = kTRUE;
			//cout << stattmp.fSize << "    Size changed..." << endl;
		}
		fFStat.fDev    = stattmp.fDev;
		fFStat.fIno    = stattmp.fIno;
		fFStat.fMode   = stattmp.fMode;
		fFStat.fUid    = stattmp.fUid;
		fFStat.fGid    = stattmp.fGid;
		fFStat.fSize   = stattmp.fSize;
		fFStat.fMtime  = stattmp.fMtime;
		fFStat.fIsLink = stattmp.fIsLink;
		fFStat.fUrl    = stattmp.fUrl;
	}

	fPendingUpdate = changed;
	return changed;
}

// TEXT FILE MONITOR

using namespace std;

ClassImp(TextFileMonitor);

TextFileMonitor::TextFileMonitor(const char *path)
: FileMonitor(path)
, fStream()
, fContent()
, fUpdate()
{
	;
}

TextFileMonitor::TextFileMonitor(const char *path, const char *option)
: FileMonitor(path, option)
, fStream()
, fContent()
, fUpdate()
{
	;
}

TextFileMonitor::~TextFileMonitor()
{
	;
}

Bool_t TextFileMonitor::Update()
{
	Bool_t resetFlag = kFALSE;
	unsigned int lastcount = fContent.size();

	fContent.clear();
	fStream.open(fPath.Data());
	for( std::string line; getline( fStream, line ); )
	{
		fContent.push_back(line);
	}	
	fStream.close();

	fUpdate.clear();
	if (lastcount > fContent.size())
	{
		fUpdate = fContent;
		resetFlag = kTRUE;
	}
	else
	{
		fUpdate.reserve(fContent.size() - lastcount);
		for (unsigned int il = lastcount; il < fContent.size(); il++)
		{
			fUpdate.push_back(fContent[il]);
		}
	}
	fPendingUpdate = kFALSE;
	return resetFlag;
}

std::vector<string> TextFileMonitor::GetUpdate()
{
	return fUpdate;
}

std::vector<string> TextFileMonitor::GetContent()
{
	return fContent;
}

void TextFileMonitor::DumpContent(unsigned int nwords)
{
	unsigned int istart = 0;
	if (nwords < fContent.size())
		istart = fContent.size() - nwords;
	for (unsigned int i = istart; i < fContent.size(); i++)
		cout << fContent[i] << endl;
}

// binary filemonitor

ClassImp(BinaryFileMonitor);

BinaryFileMonitor::BinaryFileMonitor(const char *path)
: FileMonitor(path)
, fStream()
, fContent()
, fUpdate()
, fHexStringContent()
, fHexStringUpdate()
, fNBytesWord(4)
{
	;
}

BinaryFileMonitor::BinaryFileMonitor(const char *path, const char *option)
: FileMonitor(path, option)
, fStream()
, fContent()
, fUpdate()
, fHexStringContent()
, fHexStringUpdate()
, fNBytesWord(4)
{
	;
}

BinaryFileMonitor::~BinaryFileMonitor()
{
	;
}

Bool_t BinaryFileMonitor::Update()
{
	Bool_t resetFlag = kFALSE;
	unsigned int lastcount = fContent.size();

	fContent.clear();

	fStream.open(fPath.Data(), ios::in|ios::binary|ios::ate);
	if (fStream.is_open())
	{
		std::streampos fileSize = 0;
		if (!fStream.eof() && !fStream.fail())
		{
			// this would put the whole file into the memory
			if (fNTail == 0)
			{
			        fStream.seekg(0, std::ios_base::end); //sets absolute position
				fileSize = fStream.tellg(); //if this fails, returns -1!!!!
				fContent.resize(fileSize);
				fStream.seekg(0, std::ios_base::beg);
				fStream.read(&fContent[0], fileSize);
			}
			else
			{
				// this reads the tail
				fStream.seekg(0, std::ios_base::end);
				fileSize = fStream.tellg();
				unsigned int nread = fNTail;
				std::streampos tpos = fileSize - std::streampos(fNTail);
				//std::streampos tpos = std::streampos(fNTail);
				if (tpos < 0) 
					{
						tpos  = 0;
						nread = fileSize;
					}
				cout << "[d] will read: " << nread << endl;
				fStream.seekg(tpos, std::ios_base::beg);
				//fStream.seekg(tpos, std::ios_base::end);
				fContent.resize(nread);
				fStream.read(&fContent[0], nread);
			}
		}
		fStream.close();
		cout << "[d] monitor content is " << fileSize << " in memory fContent.size() is [bytes] :" << fContent.size() << endl;
      //cout << WordToHex(&fContent[0], fContent.size());
	}
	else
	{
		cout << "[e] file not opened? " << fPath.Data() << endl;
	}

	fUpdate = fContent;
	//fUpdate.clear();
	//if (lastcount > fContent.size())
	//{
	//	resetFlag = kTRUE;
	//}
	//fUpdate.reserve(fNTail);
	//for (unsigned int i = 0; i < fNTail; i++)
	//{
	//	char c = 0;
	//	int idx = fContent.size() - fNTail + i;
	//	if (idx < fContent.size() && idx >= 0)
	//		c = fContent[idx];
	//	//cout << "[d] push_back to update: " << c << " at index " << idx << endl;
	//	fUpdate.push_back(c);
	//}

	fPendingUpdate = kFALSE;
	return resetFlag;
}

void BinaryFileMonitor::DumpContent(unsigned int nwords)
{
	vector<string> sc = GetHexStringContent();
	unsigned int istart = 0;
	if (nwords < sc.size())
		istart = sc.size() - nwords;
	for (unsigned int i = istart; i < sc.size(); i++)
	{
		cout << hex << setfill('0') << setw(8) << i * fNBytesWord << " ";
		cout << sc[i] << endl;
	}
	cout << dec << endl;
}

void BinaryFileMonitor::DumpContentWords(unsigned int nwords)
{
	if (fWords.size() <= 0)
		Words(&fContent[0], fContent.size());
	unsigned int istart = 0;
	if (nwords < fWords.size())
		istart = fWords.size() - nwords;
	for (unsigned int i = istart; i < fWords.size(); i++)
	{
		//cout << hex << setfill('0') << setw(8) << i * fNBytesWord * 8 << " ";
		cout << dec << setfill(' ') << setw(8) << i << " ";
		cout << hex << "0x" << setfill('0') << setw(8) << i << " ";
		cout << hex << "0x" << setfill('0') << setw(8) << fWords[i] << " ";
		for (unsigned int ib = 0; ib < 4; ib++)
			cout << hex << setfill('0') << setw(2) << BYTE(fWords[i], ib) << " ";
		cout << dec << Masks::debug_type_word(fWords[i]) << endl;
		//if (IS_ADC0_HEAD(fWords[i]))
		//{
		//	PayloadADC l(fWords, i);
		//	if (l.IsValid())
		//		l.Dump();
		//}
	}
	cout << dec << endl;	
}

void BinaryFileMonitor::DumpContentWords(unsigned int nwords, unsigned int mark0, unsigned int mark1, bool showADC)
{
	if (fWords.size() <= 0)
		Words(&fContent[0], fContent.size());
	unsigned int istart = 0;
	if (nwords < fWords.size())
		istart = fWords.size() - nwords;
	bool enabled = false;
	for (unsigned int i = istart; i < fWords.size(); i++)
	{
		if (fWords[i] == mark0)
			enabled = true;
		if (enabled)
		{
			//cout << hex << setfill('0') << setw(8) << i * fNBytesWord * 8 << " ";
			cout << dec << setfill(' ') << setw(8) << i << " ";
			cout << hex << "0x" << setfill('0') << setw(8) << i << " ";
			cout << hex << "0x" << setfill('0') << setw(8) << fWords[i] << " ";
			for (unsigned int ib = 0; ib < 4; ib++)
				cout << hex << setfill('0') << setw(2) << BYTE(fWords[i], ib) << " ";
			cout << dec << Masks::debug_type_word(fWords[i]) << endl;
			if (showADC)
			{
				if (IS_ADC_HEAD(fWords[i]))
				{
					PayloadADC l(fWords, i);
					if (l.IsValid())
						l.Dump(mark0);
				}
			}
		}
		if (fWords[i] == mark1)
			enabled = false;
	}
	cout << dec << endl;	
}

void BinaryFileMonitor::DumpContentWords(unsigned int ibegin, unsigned int iend)
{
	if (fWords.size() <= 0)
		Words(&fContent[0], fContent.size());
	for (unsigned int i = ibegin; i <= iend; i++)
	{
		//cout << hex << setfill('0') << setw(8) << i * fNBytesWord * 8 << " ";
		cout << dec << setfill(' ') << setw(8) << i << " ";
		cout << hex << "0x" << setfill('0') << setw(8) << i << " ";
		cout << hex << "0x" << setfill('0') << setw(8) << fWords[i] << " ";
		for (unsigned int ib = 0; ib < 4; ib++)
			cout << hex << setfill('0') << setw(2) << BYTE(fWords[i], ib) << " ";
		cout << dec << Masks::debug_type_word(fWords[i]) << endl;
		//if (IS_ADC0_HEAD(fWords[i]))
		//{
		//	PayloadADC l(fWords, i);
		//	if (l.IsValid())
		//		l.Dump();
		//}
	}
	cout << dec << endl;	
}

void BinaryFileMonitor::DumpUpdate(unsigned int nwords)
{
	vector<string> sc = GetHexStringUpdate();
	if (nwords <= 0)
		nwords = sc.size();
	if (nwords > sc.size())
		nwords = sc.size();
	for (unsigned int i = 0; i < nwords; i++)
	{
		cout << hex << setfill('0') << setw(8) << i * fNBytesWord << " ";
		cout << sc[i] << endl;
	}
	cout << dec << endl;
}

std::vector<char> BinaryFileMonitor::GetUpdate()
{
	return fUpdate;
}

std::vector<char> BinaryFileMonitor::GetContent()
{
	return fContent;
}

std::vector<std::string> BinaryFileMonitor::GetHexStringUpdate()
{
	unsigned int nbytes = 4;
	fHexStringUpdate.clear();
	fHexStringUpdate.reserve(fUpdate.size() / nbytes + 1);
	for (unsigned int i = 0; i < fUpdate.size(); i+=nbytes)
	{
		if ( i + nbytes < fUpdate.size())
			nbytes = nbytes;
		else
			nbytes = fUpdate.size() - i;
		string s = WordToHex(&fUpdate[i], nbytes);
		fHexStringUpdate.push_back(s);
	}
	return fHexStringUpdate;
}

std::vector<std::string> BinaryFileMonitor::GetHexStringContent()
{
	unsigned int nbytes = fNBytesWord;
	fHexStringContent.clear();
	fHexStringContent.reserve(fContent.size() / nbytes + 1);
	for (unsigned int i = 0; i < fContent.size(); i+=nbytes)
	{
		if ( i + nbytes >= fContent.size() )
			nbytes = fContent.size() - i;
		string s = WordToHex(&fContent[i], nbytes);
		fHexStringContent.push_back(s);
	}
	return fHexStringContent;
}

std::vector<unsigned int> BinaryFileMonitor::Words(char *buf, int nbytes, int nbytesword)
{
	if (nbytesword <= 0)
		nbytesword = fNBytesWord;
	if (nbytesword <= 0)
		nbytesword = 4;

	cout << endl;
	fWords.clear();
	unsigned int nwords = nbytes / nbytesword;
	if (nwords * nbytesword < nbytes)
		nwords += 1;
	fWords.reserve(nwords);
	for( unsigned int i = 0; i < nbytes; i+=4)
	{
		unsigned int word = 0;
		for (unsigned int ib = 0; ib < nbytesword; ib++)
		{
			if (i + ib >= nbytes)
				break;
			unsigned char c = buf[i + ib];
			unsigned int ic = (unsigned)c;
			int shift = (ib * 8);
			unsigned int shifted = (unsigned)c << shift;
			word += shifted;
			//cout << hex << setw(2) << setfill('0') << ic << " shift<<" << dec << shift << endl;
			if (ib == 3)
			{
				//cout << hex << setw(8) << setfill('0') << word << endl;
				fWords.push_back(word);
			}
		}
	}
	cout << dec << "[d] nwords = " << fWords.size() << " (reserved: " << nwords << ")" << endl;
	return fWords;
}

std::string BinaryFileMonitor::WordToHex(char *buf, int nbytes, int nbytesword)
{
	if (nbytesword <= 0)
		nbytesword = fNBytesWord;
	if (nbytesword <= 0)
		nbytesword = 4;

	stringstream ss;

	//ss << hex << uppercase << setfill('0');
	ss << hex << setfill('0');
	//cout << setw(8) << address;

	// Show the hex codes
	for( int i = nbytes - 1; i >= 0; i-- )
	{
		//if( i % 8 == 0 ) ss << ' ';
		unsigned char c = buf[i];
		ss << setw(2) << setfill('0') << (unsigned)c;
	}

	//for (int i = 0; i < nbytesword-nbytes; i++)
	//{
	//	ss << ' ' << setw(2) << "";
	//}

	// Show printable characters
	ss << " |";
	for( int i = 0; i < nbytes; i++)
	{
		if( buf[i] < 32 ) ss << '.';
		else ss << buf[i];
	}
	for (int i = 0; i < nbytesword-nbytes; i++)
	{
		ss << ' ';
	}
	ss << "|";
	//address += 16;

	return ss.str();
}

TGraph *BinaryFileMonitor::GetGraph()
{
	unsigned int npoints = fUpdate.size();
	//unsigned int npoints = fNTail;
	std::vector<double> fX;
	std::vector<double> fY;

	if (npoints == 0)
	{
		fX.push_back(-1);		
		fY.push_back(-1);		
	}
	else
	{
		fX.reserve(npoints);
		fY.reserve(npoints);
	}
	for (unsigned int i = 0; i < npoints; i++)
	{
		double val = -1.0;
		if (i < fUpdate.size())
			val = 1. * (unsigned)fUpdate[i];
		fX.push_back(i * 1.);
		fY.push_back(val * 1.);
	
	}

	TGraph *gr = new TGraph(fX.size(), &fX[0], &fY[0]);
	TString newTitle = GetPath();
	if (npoints == 0)
	{
		newTitle += TString(" [NO DATA]");
	}
	gr->SetTitle(newTitle.Data());
	gr->GetXaxis()->SetTitle("tail bytes");
	gr->GetYaxis()->SetTitle("decimal byte value");
	return gr;
}

// many monitors in a list

ClassImp(FilesMonitor)

FilesMonitor::FilesMonitor(const char *name, const char *title)
: TNamed(name, title)
, fMonitors(new TList)
, fForceChanged(kTRUE)
{
	fMonitors->SetOwner(kTRUE);
}

FilesMonitor::~FilesMonitor()
{
	delete fMonitors;
}

Bool_t FilesMonitor::HasChanged()
{
	TIter next(fMonitors);
	TObject *obj = 0;
	while ( ( obj = next() ) )
	{
		FileMonitor *m = (FileMonitor*)obj;
		if (m->HasChanged())
			return kTRUE;
	}
	if (fForceChanged == kTRUE)
	{
		fForceChanged = kFALSE;
		return kTRUE;
	}
	return kFALSE;
}

Bool_t FilesMonitor::HasChanged(const char *path)
{
	FileMonitor *fm = GetMonitor(path);
	if (fm)
	{
		return fm->HasChanged();
	}
	return kFALSE;
}

TString FilesMonitor::AbsolutePath(const char *fname)
{
	TString ret = "";
	char buf[PATH_MAX];
	char *res = realpath(fname, buf);
	if (res) 
	{
		ret = buf;
	} 
	else 
	{
		ret = "";
	}
	return ret;
}

void FilesMonitor::AddFile(const char *fname, const char *opt)
{
	TString spath = AbsolutePath(fname);
	if (spath.Length() <= 0)
	{
		cerr << "[e] ::AddFile unable to resolve file:" << fname << endl;
	}
	if (GetMonitor(spath.Data()) != 0x0)
	{
		cerr << "[w] file: " << spath.Data() << " already monitored." << endl;
		return;
	}
	FileMonitor *fm = 0;
	if (fOption.Contains(":text"))
	{
		fm = new TextFileMonitor(spath.Data(), opt);
	}
	else
	{
		if (fOption.Contains(":binary"))
		{
			fm = new BinaryFileMonitor(spath.Data(), opt);
		}
		else
		{
			if (fOption.Contains(":decoder"))
			{
				fm = new Decoder(spath.Data(), opt);
			}
			else
			{
				fm = new FileMonitor(spath.Data(), opt);
			}
		}
	}

	if (fm->IsValid() == kFALSE)
	{
		cerr << "[w] adding an invalid path to monitor: " << fm->GetPath() << endl;
	}
	fMonitors->Add(fm);
	fForceChanged = kTRUE;
}

void FilesMonitor::RemoveFile(const char *fname)
{
	TString spath = AbsolutePath(fname);
	if (spath.Length() <= 0)
	{
		cerr << "[e] ::RemoveFile unable to resolve file:" << fname << endl;
	}
	FileMonitor *fm = GetMonitor(spath.Data());
	if (fm != 0)
	{
		fMonitors->Remove(fm);
		cout << "[d] removing: " << spath << " from monitoring..." << endl;
	}
	fForceChanged = kTRUE;
}

FileMonitor *FilesMonitor::GetMonitor(const char *fname)
{
	TString spath = AbsolutePath(fname);
	TIter next(fMonitors);
	TObject *obj = 0;
	while ( ( obj = next() ) )
	{
		FileMonitor *m = (FileMonitor*)obj;
		if (m->GetPath() == spath)
			return m;
	}
	return 0x0;	
}

void FilesMonitor::ToggleOption(Int_t i, const char *opt)
{
	FileMonitor *fm = (FileMonitor*)fMonitors->At(i);
	if (fm)
	{
		fm->ToggleOption(opt);
		fForceChanged = kTRUE;
	}
	else
	{
//cerr << "[e] Bad file selected : #" << i << endl;
		;
	}
}

FileMonitor* FilesMonitor::Get(Int_t i)
{
	return (FileMonitor*)fMonitors->At(i);
}

const char * FilesMonitor::GetPath(Int_t i)
{
	FileMonitor *fm = Get(i);
	if (fm)
	{
		return fm->GetPath();
	}
	return "";
}

Int_t FilesMonitor::CountWithOption(const char * opt)
{
	Int_t iactive = 0;
	TIter next(fMonitors);
	TObject *obj = 0;
	while ( ( obj = next() ) )
	{
		FileMonitor *m = (FileMonitor*)obj;
		if (m->HasOption(opt))
			iactive++;
	}
	return iactive;		
}

Int_t FilesMonitor::GetSize()
{
	return fMonitors->GetSize();
}

void FilesMonitor::SetNTail(Int_t n)
{
	TIter next(fMonitors);
	TObject *obj = 0;
	while ( ( obj = next() ) )
	{
		FileMonitor *m = (FileMonitor*)obj;
		m->SetNTail(n);
	}
}

void FilesMonitor::ForceChanged()
{
	TIter next(fMonitors);
	TObject *obj = 0;
	while ( ( obj = next() ) )
	{
		FileMonitor *m = (FileMonitor*)obj;
		m->ForceChanged();
	}
}
