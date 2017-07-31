#include "DList.h"

#include <TList.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TH1.h>
#include <TGraph.h>
#include <TClass.h>
#include <TMath.h>
#include <TLegend.h>
#include <TCanvas.h>

ClassImp(DList);

#include <iostream>
#include <limits>
using namespace std;

Int_t GetOption(const char *sep, TString &sopt, Bool_t isbool = kFALSE)
{
	Int_t retval = -1;
	TObjArray *ts = sopt.Tokenize(" ");
	TObjString *s = 0;
	TIter it(ts);
	while ((s = (TObjString*)it.Next())!=0)
	{
		TString stmp = s->GetString();
		if (stmp(0,TString(sep).Length()) == TString(sep))
		{
			retval = TString(stmp(2, stmp.Length())).Atoi();
			sopt.ReplaceAll(stmp, "");
		}
	}
	delete ts;

	if (isbool)
	{
		if (retval < 0)
		{
			retval = kFALSE;
		}
		else
		{
			retval = kTRUE;
		}
	}
	return retval;
}

TString ProcessOptions(TObject *p, TString options)
{
	//cout << "[i] options = " << options << endl;
	TString stmp = options;
	Int_t linestyle = GetOption("+l", stmp);
	Int_t linewidth = GetOption("+w", stmp);
	Int_t marksize  = GetOption("+s", stmp);
	Int_t markstyle = GetOption("+m", stmp);
	Int_t 	  kolor = GetOption("+k", stmp);
	Int_t 	  alpha = GetOption("+a", stmp);
	Int_t 	  noleg = GetOption("noleg", stmp, kTRUE);

	//cout << "    linestyle: " << linestyle << endl;
	//cout << "    linewidth: " << linewidth << endl;
	//cout << "    marksize:  " << marksize << endl;
	//cout << "    markstyle: " << markstyle << endl;
	//cout << "    noleg:     " << noleg << endl;
	//cout << "    kolor:     " << kolor << endl;
	//cout << "    alpha: 	" << alpha << endl;

	if (p->IsA()->InheritsFrom("TH1"))
	{
		TH1 *h = (TH1*)p;
		if (linestyle >= 0)
			h->SetLineStyle(linestyle);
		if (linewidth >= 0)
			h->SetLineWidth(linewidth);
		if (markstyle >= 0)
			h->SetMarkerStyle(markstyle);
		if (marksize >= 0)
			h->SetMarkerSize(marksize/10.);
		if (kolor >= 0)
		{
			if (alpha >= 0)
			{
				h->SetLineColorAlpha(kolor, alpha/100.);
				h->SetMarkerColorAlpha(kolor, alpha/100.);
			}
			else
			{
				h->SetLineColor(kolor);
				h->SetMarkerColor(kolor);
			}
		}
	}

	if (p->IsA()->InheritsFrom("TGraph"))
	{
		TGraph *h = (TGraph*)p;
		if (linestyle >= 0)
			h->SetLineStyle(linestyle);
		if (linewidth >= 0)
			h->SetLineWidth(linewidth);
		if (markstyle >= 0)
			h->SetMarkerStyle(markstyle);
		if (marksize >= 0)
			h->SetMarkerSize(marksize/10.);
		if (kolor >= 0)
		{
			if (alpha >= 0)
			{
				h->SetLineColorAlpha(kolor, alpha/100.);
				h->SetMarkerColorAlpha(kolor, alpha/100.);
			}
			else
			{
				h->SetLineColor(kolor);
				h->SetMarkerColor(kolor);
			}
		}

		h->SetFillStyle(1001);
		h->SetFillColor(0);
	}

	TString ret = stmp;
	return ret;
}

DObject::DObject(TObject *obj, Bool_t clone, Option_t *opt, const char *name)
	: fObject(0)
	, fOptions("")
	, fDrawOpt("")
	, fActive(kTRUE)
{
	SetObject(obj, clone, opt, name);
}

DObject::~DObject()
{
	delete fObject;
}

void DObject::SetObject(TObject *obj, Bool_t clone, Option_t *opt, const char *name)
{
	if (clone == kTRUE)
	{
		TString newName = TString::Format("%s_dobject", obj->GetName());
		if (name != 0)
		{
			newName = name;
		}
		fObject = obj->Clone(newName.Data());
		//cout << "[d] DObject::SetObject new name : " << newName << endl;
		if (fObject->IsA()->InheritsFrom("TH1"))
			((TH1*)fObject)->SetDirectory(0);
	}
	else
	{
		fObject = obj;
	}

	fOptions = opt;
	if (fObject->IsA()->InheritsFrom("TH1"))
		{
			TH1 *htmp = (TH1*)fObject;
			fDrawOpt = ProcessOptions(htmp, opt);
		}
	if (fObject->IsA()->InheritsFrom("TGraph"))
		{
			TGraph *htmp = (TGraph*)fObject;
			fDrawOpt = ProcessOptions(htmp, opt);
		}
}

void DObject::GetMinMaxY(Double_t &miny, Double_t &maxy)
{
	if (fObject->IsA()->InheritsFrom("TH1"))
		{
			TH1 *htmp = (TH1*)fObject;
			//miny = htmp->GetMinimum();
			//maxy = htmp->GetMaximum();
			for (Int_t ib = 1; ib < htmp->GetNbinsX() + 1; ib++)
			{
				miny = TMath::Min(miny, htmp->GetBinContent(ib));
				maxy = TMath::Max(maxy, htmp->GetBinContent(ib));
			}
		}	

	if (fObject->IsA()->InheritsFrom("TGraph"))
		{
			TGraph *htmp = (TGraph*)fObject;
			for (Int_t i = 0; i < htmp->GetN(); i++)
			{
				Double_t x = 0;
				Double_t y = 0;
				htmp->GetPoint(i, x, y);
				miny = TMath::Min(miny, y);
				maxy = TMath::Max(maxy, y);
			}
		}	
}

void DObject::SetMinMaxY(Double_t min, Double_t max)
{
	if (fObject->IsA()->InheritsFrom("TH1"))
		{
			TH1 *htmp = (TH1*)fObject;
			htmp->SetMinimum(min);
			htmp->SetMaximum(max);
		}	

	if (fObject->IsA()->InheritsFrom("TGraph"))
		{
			TGraph *htmp = (TGraph*)fObject;
			htmp->SetMinimum(min);
			htmp->SetMaximum(max);
			htmp->GetYaxis()->SetLimits(min, max);
		}	
}

void DObject::GetMinMaxX(Double_t &miny, Double_t &maxy)
{
	if (fObject->IsA()->InheritsFrom("TH1"))
		{
			TH1 *htmp = (TH1*)fObject;
			//miny = htmp->GetMinimum();
			//maxy = htmp->GetMaximum();
			for (Int_t ib = 1; ib < htmp->GetNbinsX() + 1; ib++)
			{
				miny = TMath::Min(miny, htmp->GetBinLowEdge(ib));
				maxy = TMath::Max(maxy, htmp->GetBinCenter(ib) + htmp->GetBinWidth(ib));
			}
		}	

	if (fObject->IsA()->InheritsFrom("TGraph"))
		{
			TGraph *htmp = (TGraph*)fObject;
			for (Int_t i = 0; i < htmp->GetN(); i++)
			{
				Double_t x = 0;
				Double_t y = 0;
				htmp->GetPoint(i, x, y);
				miny = TMath::Min(miny, x);
				maxy = TMath::Max(maxy, x);
			}
		}	
}

void DObject::SetMinMaxX(Double_t min, Double_t max)
{
	if (fObject->IsA()->InheritsFrom("TH1"))
		{
			TH1 *htmp = (TH1*)fObject;
			//htmp->SetMinimum(min);
			//htmp->SetMaximum(max);
			//htmp->GetXaxis()->SetLimits(min, max);
		}	

	if (fObject->IsA()->InheritsFrom("TGraph"))
		{
			TGraph *htmp = (TGraph*)fObject;
			//htmp->SetMinimum(min);
			//htmp->SetMaximum(max);
			htmp->GetXaxis()->SetLimits(min, max);
		}	
}

void DObject::Draw(Option_t *opt)
{
	if (fObject->IsA()->InheritsFrom("TH1"))
	{
		TH1 *htmp = (TH1*)fObject;
		htmp->Draw(opt);
	}

	if (fObject->IsA()->InheritsFrom("TGraph"))
	{
		TGraph *htmp = (TGraph*)fObject;
		htmp->Draw(opt);
	}
}

void DObject::SetTitle(const char *t)
{
	if (fObject->IsA()->InheritsFrom("TH1"))
		{
			TH1 *htmp = (TH1*)fObject;
			htmp->SetTitle(t);
		}		

	if (fObject->IsA()->InheritsFrom("TGraph"))
		{
			TGraph *htmp = (TGraph*)fObject;
			htmp->SetTitle(t);
		}
}

const char *DObject::GetTitle()
{
	if (fObject->IsA()->InheritsFrom("TH1"))
		{
			TH1 *htmp = (TH1*)fObject;
			return htmp->GetTitle();
		}		

	if (fObject->IsA()->InheritsFrom("TGraph"))
		{
			TGraph *htmp = (TGraph*)fObject;
			return htmp->GetTitle();
		}
	return "No title";
}

// LIST

DList::DList(const char *name, const char *title)
	: TNamed(name, title)
	, fList(new TList)
	, fLegend(0)
{
	fList->SetOwner(kTRUE);
}

DList::~DList()
{
	delete fList;
}

Bool_t DList::IsUniqueName(const char *name)
{
	TString scmp = name;
	TIter it(fList);
	DObject *obj = 0;
	while ((obj = (DObject*)it.Next())!=0)
	{
		TString stmp = obj->GetObject()->GetName();
		if (stmp == scmp)
			return kFALSE;
	}
	return kTRUE;
}

DObject *DList::GetByName(const char *name)
{
	TString scmp = TString::Format("%s_%s",name,this->GetName());
	TIter it(fList);
	DObject *obj = 0;
	while ((obj = (DObject*)it.Next())!=0)
	{
		TString stmp = obj->GetObject()->GetName();
		if (stmp == scmp)
			return obj;
	}
	return (DObject*)0x0;	
}

Bool_t DList::RemoveByName(const char *name)
{
	TString scmp = TString::Format("%s_%s",name,this->GetName());
	TIter it(fList);
	DObject *obj = 0;
	while ((obj = (DObject*)it.Next())!=0)
	{
		TString stmp = obj->GetObject()->GetName();
		if (stmp == scmp)
		{
			fList->Remove(obj);
			return kTRUE;
		}
	}
	return kFALSE;
}

void DList::ClearList(Option_t* option)
{
	fList->Clear(option);
}

Int_t DList::GetN()
{
	return fList->GetEntries();
}

Bool_t DList::Add(TObject *obj, Option_t *opt, const char *newTitle)
{
	TString newNameBase = TString::Format("%s_%s", obj->GetName(), this->GetName());
	TString newName = newNameBase;
	Int_t ic = 1;
	while (!IsUniqueName(newName.Data()))
	{
		newName = newNameBase + TString::Format("%d", ic);
		ic++;
	}

	if (!obj->IsA()->InheritsFrom("TH1") && 
		!obj->IsA()->InheritsFrom("TGraph") &&
		!obj->IsA()->InheritsFrom("TF1"))
	{
		return kFALSE;
	}

	DObject *d = new DObject(obj, kTRUE, opt, newName.Data());
	if (strlen(newTitle) > 0)
	{
		d->SetTitle(newTitle);
	}

	fList->Add(d);

	return kTRUE;
}

void DList::GetMinMaxY(Double_t &miny, Double_t &maxy)
{
	miny = std::numeric_limits<double>::max();
	maxy = -1. * std::numeric_limits<double>::max();
	TIter it(fList);
	DObject *obj = 0;
	while ((obj = (DObject*)it.Next())!=0)
	{
		Double_t tmpmin = std::numeric_limits<double>::max();
		Double_t tmpmax = -1. * tmpmin;
		obj->GetMinMaxY(tmpmin, tmpmax);
		miny = TMath::Min(miny, tmpmin);
		maxy = TMath::Max(maxy, tmpmax);
	}
}
void DList::SetMinMaxY(Double_t min, Double_t max)
{
	TIter it(fList);
	DObject *obj = 0;
	while ((obj = (DObject*)it.Next())!=0)
	{
		obj->SetMinMaxY(min, max);
	}	
}

void DList::GetMinMaxX(Double_t &miny, Double_t &maxy)
{
	miny = std::numeric_limits<double>::max();
	maxy = -1. * std::numeric_limits<double>::max();
	TIter it(fList);
	DObject *obj = 0;
	while ((obj = (DObject*)it.Next())!=0)
	{
		Double_t tmpmin = std::numeric_limits<double>::max();
		Double_t tmpmax = -1. * tmpmin;
		obj->GetMinMaxX(tmpmin, tmpmax);
		miny = TMath::Min(miny, tmpmin);
		maxy = TMath::Max(maxy, tmpmax);
	}
}
void DList::SetMinMaxX(Double_t min, Double_t max)
{
	TIter it(fList);
	DObject *obj = 0;
	while ((obj = (DObject*)it.Next())!=0)
	{
		obj->SetMinMaxX(min, max);
	}	
}

void DList::Draw(Option_t *opts)
{
	Double_t delta = 0;

	Double_t miny = std::numeric_limits<double>::max();
	Double_t maxy = -1. * std::numeric_limits<double>::max();
	GetMinMaxY(miny, maxy);
	delta = TMath::Abs(maxy - miny);
	miny = miny - delta * 0.1;
	maxy = maxy + delta * 0.2;
	if (TMath::Abs(miny) < 1e-50 && TMath::Abs(maxy) < 1e-50)
	{
		miny = -1;
		maxy =  1;
	}
	SetMinMaxY(miny, maxy);

	Double_t minx = std::numeric_limits<double>::max();
	Double_t maxx = -1. * std::numeric_limits<double>::max();
	GetMinMaxX(minx, maxx);
	delta = TMath::Abs(maxx - minx);
	minx = minx - delta * 0.1;
	maxx = maxx + delta * 0.2;
	if (TMath::Abs(minx) < 1e-50 && TMath::Abs(maxx) < 1e-50)
	{
		minx = -1;
		maxx =  1;
	}
	SetMinMaxX(minx, maxx);

	Int_t counter = 0;
	TIter it(fList);
	DObject *obj = 0;
	while ((obj = (DObject*)it.Next())!=0)
	{
		if (obj->IsActive() == kFALSE) continue;
		TString dopt = obj->GetDrawOpt() + " " + opts;
		if (counter == 0)
		{
			if (obj->GetObject()->IsA()->InheritsFrom("TGraph"))
				dopt += "A";
		}
		else
		{
			dopt += " same";
		}
		//cout << "[i] ::Draw " << obj->GetObject()->GetName() << " opt: [" << dopt << "]" 
		//	<< " counter: " << counter
		//	<< endl;
		obj->Draw(dopt);
		counter++;
	}	
}

Int_t DList::GetNactive()
{
	Int_t counter = 0;
	TIter it(fList);
	DObject *obj = 0;
	while ((obj = (DObject*)it.Next())!=0)
	{
		if (obj->IsActive() == kFALSE) continue;
		counter++;
	}	
	return counter;	
}

void DList::DeactivateAll()
{
	TIter it(fList);
	DObject *obj = 0;
	while ((obj = (DObject*)it.Next())!=0)
	{
		obj->SetActive(kFALSE);
	}	
}

//TLegend* DList::Legend(const char *title, Double_t x1, Double_t y1, Double_t x2, Double_t y2)
//{
//	if (fLegend)
//	{
//		delete fLegend;
//	}
//	fLegend = 0;
//	if (gPad)
//	{
//		fLegend = gPad->BuildLegend(x1, y1, x2, y2, title);
//		fLegend->SetFillColorAlpha(kWhite, 0.5);
//	}
//	return fLegend;
//}

TLegend* DList::Legend(const char *title, Double_t x1, Double_t y1, Double_t x2, Double_t y2)
{
	if (fLegend)
	{
		delete fLegend;
	}
	fLegend = new TLegend(x1, y1, x2, y2);
	fLegend->SetHeader(title);
	fLegend->SetFillColorAlpha(kWhite, 0.5);
	fLegend->SetNColumns(1);
	fLegend->SetBorderSize(0);
	TIter it(fList);
	DObject *obj = 0;
	while ((obj = (DObject*)it.Next())!=0)
	{
		if (obj->IsActive())
			fLegend->AddEntry(obj->GetObject(), obj->GetTitle(), obj->GetDrawOpt());
	}
	return fLegend;
}
