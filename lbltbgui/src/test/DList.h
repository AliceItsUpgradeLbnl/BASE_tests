#ifndef __HLIST__HH
#define __HLIST__HH

#include <TObject.h>
#include <TString.h>
#include <TNamed.h>

class TList;
class TLegend;

class DObject : public TObject
{
public:
	DObject(TObject *obj, Bool_t clone = kTRUE, Option_t *opt = "", const char *name = 0);
	void 	SetObject(TObject *obj, Bool_t clone = kTRUE, Option_t *opt = "", const char *name = 0);
	void 	SetOptions(Option_t *opt) {fOptions = opt;}
	virtual ~DObject();
	TObject *GetObject() {return fObject;}
	void 	GetMinMaxY(Double_t &miny, Double_t &maxy);	
	void 	SetMinMaxY(Double_t miny, Double_t maxy);
	void 	GetMinMaxX(Double_t &miny, Double_t &maxy);	
	void 	SetMinMaxX(Double_t miny, Double_t maxy);
	void 	SetTitle(const char *t);
	const char *GetTitle();
	TString GetDrawOpt() {return fDrawOpt;}
	TString GetOptions() {return fOptions;}
	void 	ToggleActive() {fActive = !fActive;}
	Bool_t 	IsActive() {return fActive;}
	void 	SetActive(Bool_t val) {fActive = val;}
	void 	Draw(Option_t *opt);
private:
	DObject() : TObject() {;}

	TObject *fObject;
	TString fOptions;
	TString fDrawOpt;

	Bool_t fActive;

	ClassDef(DObject, 0)
};

class DList : public TNamed
{

public:
	DList(const char *name, const char *title);
	virtual 	~DList();
	Bool_t 		Add(TObject *obj, Option_t *opt, const char *newTitle = "");
	void 		Draw(Option_t *opts = "");
	TLegend* 	Legend(const char *title, Double_t x1=0.2, Double_t y1=0.8, Double_t x2=0.8, Double_t y2=0.6);
	void 		GetMinMaxY(Double_t &miny, Double_t &maxy);
	void 		SetMinMaxY(Double_t miny, Double_t maxy);	
	void 		GetMinMaxX(Double_t &miny, Double_t &maxy);
	void 		SetMinMaxX(Double_t miny, Double_t maxy);	
	DObject *	GetByName(const char *name);
	Int_t 		GetNactive();
	Bool_t 		RemoveByName(const char *name);
	Int_t 		GetN();
	Int_t 		GetSize() {return GetN();}
	void 		DeactivateAll();
	void 		ClearList(Option_t* option = "");
private:
	DList() {;}
	TList *fList; //!
	TLegend *fLegend;

	Bool_t IsUniqueName(const char *name);

	ClassDef(DList, 0)
};

#endif