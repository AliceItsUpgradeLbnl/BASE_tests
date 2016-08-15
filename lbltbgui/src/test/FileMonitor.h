#ifndef __FILE_MONITOR__
#define __FILE_MONITOR__

#include <TSystem.h>
#include <TObject.h>
#include <TNamed.h>
#include <TString.h>

class FileMonitor : public TObject
{
public:
	FileMonitor(const char *path);
	FileMonitor(const char *path, const char *option);
	virtual 				~FileMonitor() {;}
	virtual Bool_t 			HasChanged();
	virtual void			SetPath(const char *path, const char *option ="") 
	{
		fPath 	= path;
		fOption = option;
	}
	virtual void		 	SetOption(const char *option) {fOption = option;}
	virtual void 		 	ToggleOption(const char *opt);

	virtual const char*  	GetPath() 	{return fPath.Data();}
	virtual const char*  	GetOption() {return fOption.Data();}
	virtual Bool_t       	IsValid();
	virtual Bool_t		 	HasOption(const char *opt);
	virtual void		 	SetNTail(unsigned int n) {fNTail = n;}
	virtual unsigned int 	GetNTail() {return fNTail;}

	virtual void			DumpContent(unsigned int nwords = -1) {;}
	virtual void        	ForceChanged() {fForceChanged = kTRUE;}
	virtual Bool_t      	IsPendingUpdate() {return fPendingUpdate || fForceChanged;}

protected:
	TString			fPath;
	TString 		fOption;
	FileStat_t 		fFStat;	
	unsigned int 	fNTail;
	Bool_t          fForceChanged;
	Bool_t 			fPendingUpdate;

	ClassDef(FileMonitor, 0)
};

#include <fstream>
#include <string>

class TextFileMonitor : public FileMonitor
{
public:
	TextFileMonitor(const char *path);
	TextFileMonitor(const char *path, const char *option);
	virtual ~TextFileMonitor();

	Bool_t Update();
	std::vector<std::string> GetUpdate();
	std::vector<std::string> GetContent();

	virtual void		DumpContent(unsigned int nwords = -1);

private:
	std::ifstream 				fStream;
	std::vector<std::string> 	fContent;
	std::vector<std::string> 	fUpdate;

	ClassDef(TextFileMonitor, 0)
};

class TGraph;
class BinaryFileMonitor : public FileMonitor
{
public:
	BinaryFileMonitor(const char *path);
	BinaryFileMonitor(const char *path, const char *option);
	virtual ~BinaryFileMonitor();

	Bool_t Update();
	std::vector<char> GetUpdate();
	std::vector<char> GetContent();
	std::vector<std::string> GetHexStringUpdate();
	std::vector<std::string> GetHexStringContent();

	std::string WordToHex(char *buf, int nbytes, int nbytesword = -1);
	std::vector<unsigned int> Words(char *buf, int nbytes, int nbytesword = -1);

	virtual void DumpContent(unsigned int nwords = -1);
	virtual void DumpUpdate(unsigned int nwords = -1);
	virtual void DumpContentWords(unsigned int nwords = -1);
	virtual void DumpContentWords(unsigned int ibegin, unsigned int iend);
	virtual void DumpContentWords(unsigned int nwords, unsigned int mark0, unsigned int mark1, bool showADC = false);

	TGraph *GetGraph();

protected:
	std::ifstream 				fStream;
	std::vector<char> 			fContent;
	std::vector<char> 			fUpdate;
	std::vector<std::string> 	fHexStringContent;
	std::vector<std::string> 	fHexStringUpdate;
	std::vector<unsigned int>   fWords;

	Int_t						fNBytesWord;

	ClassDef(BinaryFileMonitor, 0)
};

class FilesMonitor : public TNamed
{
public:
	FilesMonitor(const char *name, const char *title);
	virtual					~FilesMonitor();
	virtual Bool_t			HasChanged();
	virtual Bool_t			HasChanged(const char *path);
	virtual void			AddFile(const char *fname, const char *opt = ":active");
	virtual void			RemoveFile(const char *fname);
	virtual FileMonitor * 	GetMonitor(const char *fname);
	virtual TList *			GetList() {return fMonitors;}
	virtual void			ToggleOption(Int_t i, const char *opt);
	virtual FileMonitor*	Get(Int_t i);
	virtual const char *    GetPath(Int_t i);
	virtual Int_t 			CountWithOption(const char * opt);
	virtual Int_t			GetSize();
	virtual void            SetOption(const char *opt) {fOption = opt;}
	virtual TString 		AbsolutePath(const char *fname);
	virtual void			SetNTail(Int_t n);
	virtual void 			ForceChanged();
private:
	TList 			*fMonitors;
	TString			fOption;
	Bool_t			fForceChanged;
	ClassDef(FilesMonitor, 0);
};

#endif // __FILE_MONITOR__
