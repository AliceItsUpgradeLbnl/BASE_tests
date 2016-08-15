#include "Options.h"
#include "FileMonitor.h"
#include "Decoder.h"

#include <TGraph.h>
#include <TFile.h>

#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[])
{
	Options opts(argc, argv);

	string fname = opts.ArgWith("--in");
	if (fname.size() < 1)
	{
		if (argc>1)
		{
			fname = argv[1];
		}
		else
		{
			fname = "";
		}
	}

	unsigned int head = strtol(opts.ArgWith("--head").c_str(), NULL, 16);

	unsigned int nwords = atol(opts.ArgWith("-n").c_str());
	if (nwords <= 0)
		nwords = -1;
	if (opts.IsSet("--decoder"))
	{
		if (head <= 0)
		{
			cout << "[e] use --head <hex header arg>" << endl;
			return 0;
		}
		Decoder d(fname.c_str());
		d.SetNTail(0); // whole file
		d.SetHeader(head);
		d.Update();
		if (opts.IsSet("--graph"))
		{
			TFile *fout = new TFile(TString::Format("out_0x%x.root", head), "RECREATE");
			fout->cd();
			for (unsigned int i = 0; i < 32; i++)
			{
				TGraph *gr = d.GetGraph(i);
				gr->Write();
			}
			fout->Close();
			delete fout;
		}
	}
	else
	{
		BinaryFileMonitor m(fname.c_str());
		m.SetNTail(0); // whole file
		m.Update();
		if (opts.IsSet("--ascii"))
			m.DumpContent(nwords);
		if (head > 0)
		{
			m.DumpContentWords(-1, head, 0xfadecafe, opts.IsSet("--adc"));
		}
		else
		{				
			m.DumpContentWords(nwords);
		}
	}
	return 0;
}