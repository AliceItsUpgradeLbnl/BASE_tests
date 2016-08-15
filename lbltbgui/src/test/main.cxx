#include <TGMdiFrame.h>
#include <TGTab.h>
#include <TApplication.h>
#include <TGButton.h>

#include "TimedFrame.h"
#include "GraphicMonitorFrame.h"
#include "TextMonitorFrame.h"
#include "DecoderMonitorFrame.h"
#include "LogFile.h"
#include "MainFrame.h"
#include "Options.h"

#include <cstdlib>

int main(int argc, char *argv[])
{
	Options opts(argc, argv);

	LogFile *log = 0;
	TString sLogFile = TString::Format("%s_%d.log", gSystem->BaseName(argv[0]), gSystem->GetPid());
	if (!opts.IsSet("--no-log"))
	{
		log = new LogFile(sLogFile.Data());
		log->Redirect();
	}

	int fake_argc = 1;
	char *fake_argv[] = { argv[0] };
	TApplication *theApp = new TApplication("theApp", &fake_argc, fake_argv);

	MainFrame *fMainFrame = new MainFrame(gClient->GetRoot(), 1000, 600, kMainFrame | kVerticalFrame);
	fMainFrame->SetName("fMainFrame");

	if (!opts.IsSet("--no-log"))
	{
		TextMonitorFrame *logFrame = fMainFrame->AddTextMonitorFrame("Log");
		logFrame->AddFile(sLogFile.Data());
	}
	fMainFrame->HandleOptions(&opts);

	fMainFrame->Resize(1000,600);

	theApp->Run();

	//fMainFrame->SaveSource("tmp.C");

	return 0;
}