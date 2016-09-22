#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <TStopwatch.h>
#include <TCanvas.h>
#include <time.h>
#include <TLegend.h>
#include <TLatex.h>
#include <vector>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TGaxis.h>
#include <TAxis.h>
#include <TROOT.h>
#include <fstream>

using namespace std;

void Latchup(int,int,int,string);

void Monitor(int thresh_ang=250, int thresh_core=900)
{
  TCanvas * c1 = new TCanvas("c1","Monster",1000,0,800,600);
  gROOT->SetBatch(kTRUE);
  TCanvas * bull= new TCanvas();
  gROOT->SetBatch(kFALSE);
  struct timespec pause;
  struct timespec rem;
  pause.tv_sec=0;
  rem.tv_sec=0;
  rem.tv_nsec = 0;
  TStopwatch * Timer = new TStopwatch();
  Timer->Reset();
  double ntime;
  c1->Divide(2,2);
  c1->cd(1)->SetPad(.005, .255, .35, .995);
  c1->cd(2)->SetPad(.35, .255, .995, .995);
  c1->cd(3)->SetPad(.005, .005, .405, .255);
  c1->cd(4)->SetPad(.405, .005, .995, .255);
  c1->cd();
  TLatex * count = new TLatex();
  TLatex * timer = new TLatex();
  TLatex * total = new TLatex();
  char timestr[50];
  char countstr[50];
  char latchname[50];
  int now;
  FILE* in;
  int dats=0;
  int mins=-1;
  ifstream runfolders("runfolder_list.txt");
  string run_folders = "";
  string directory = "";
  while(getline(runfolders,run_folders))
  {
    if(run_folders.empty())
    {
      continue;
    }
    directory = run_folders;
  }
  runfolders.close();
  while(true)
  {
    sprintf(countstr,"#scale[12]{%i}",dats);
    count = new TLatex(.3,.45,countstr);
    c1->cd(1)->Clear();
    count->Draw();
    c1->cd(1)->Update();
    now=0;
  	mins++;
        string strcommand= "echo -en; ls -X " + directory + " | awk -F . '{print $NF}' | uniq -c | grep dat | awk '{print $1}'";
        char *command=new char[strcommand.size()+1];
        command[strcommand.size()]=0;
        memcpy(command,strcommand.c_str(),strcommand.size());
	in=popen(command,"r");
  	fgets(countstr,5,in);
	  //pclose(in);
  	dats=atoi(countstr);
  	sprintf(latchname, "#scale[6]{%i}",dats);
  	total = new TLatex(.5, .5, latchname);
  	c1->cd(3)->Clear();
  	total->Draw();
  	c1->cd(3)->Update();
    while(now<60)
    {
  	  Timer->Start();
      sprintf(timestr,"#scale[6]{%i:%d}",mins,now);
      timer = new TLatex(.5,.05,timestr);
      c1->cd(4)->Clear();
      timer->Draw();
  	  c1->cd(4)->Update();
    	in=popen(command,"r");
    	fgets(countstr,5,in);
	  //pclose(in);
	if(atoi(countstr)!=dats)
    	{
	      dats++;
              cout<< dats << endl;
//    	      sprintf(latchname, "Graphs/Latchup_%i.png",dats);
              string graphpath = "/Graphs/Latchup_" + directory + "_";
              char *graphpathch=new char[graphpath.size()+1];
              graphpathch[graphpath.size()]=0;
              memcpy(graphpathch,graphpath.c_str(),graphpath.size());
              char datnum[50] = "";
              sprintf(datnum,"%i.png",dats);
              char temp[200]="";
              getcwd(temp, 200);
              cout << temp << endl;
              char latchpicname[300] = "";
              strcat(latchpicname,temp);
              strcat(latchpicname,graphpathch);
              strcat(latchpicname,datnum);
              cout << latchpicname << endl;
	      c1->cd(2)->Clear();
	      Latchup(dats,thresh_ang,thresh_core,directory);
	      c1->cd(2)->Update();
	      gROOT->SetBatch(kTRUE);
	      bull->Clear();
	      Latchup(dats,thresh_ang,thresh_core,directory);
	      bull->Update();
	      bull->SaveAs(latchpicname);
	      gROOT->SetBatch(kFALSE);
	      sprintf(countstr,"#scale[12]{%i}",dats);
	      count = new TLatex(.3,.45,countstr);
	      c1->cd(1)->Clear();
	      count->Draw();
	      c1->cd(1)->Update();
    	}
    	sprintf(latchname, "#scale[4]{%.4f per min}",(float)dats/(mins+(float)now/60));
    	total = new TLatex(.2, .2, latchname);
    	c1->cd(3)->Clear();
    	total->Draw();
    	c1->cd(3)->Update();
        now++;
    	ntime = Timer->RealTime();
    	if(ntime<1)
    	{
      	rem.tv_nsec=0;
      	pause.tv_nsec=(long)(1000000000*(1-ntime));
      	nanosleep(&pause,&rem);
      	nanosleep(&rem,NULL);
      	pclose(in);
      }
    	Timer->Reset();
    }
  }
}

void Latchup(int latchs,int thresh_ang,int thresh_core,string run_folders)
{
  char buffer[100] = "";
  string line;
  double max=0;
  int n=0;
  vector<double> Time,Core,IO,ANG;
//  sprintf(buffer,"/latchup-%i.dat",latchs);
  char *directory=new char[run_folders.size()+1];
  directory[run_folders.size()]=0;
  memcpy(directory,run_folders.c_str(),run_folders.size());
  strcat(buffer,directory);
  strcat(buffer,"/latchup-");
  char a[50] = "";
  sprintf(a,"%i.dat",latchs);
  strcat(buffer,a);
  std::cout << buffer << std::endl;
//  sprintf(buffer,"latchup-000001-2015-12-14_13:52:15.409616.dat",latchs);
  ifstream myfile(buffer);
  if(!myfile.good()){cout<<endl<<"File could not be loaded.";}
  else
  {
    double T,dT,LDO,I_core,I_io,I_ang;
    while(n<342)
    {
      n=n+1;
      if(n<=4){getline(myfile,line);}
      if(n>4)
      {
        myfile>>T>>dT>>LDO>>I_core>>I_io>>I_ang;
        Time.push_back(dT/80000);
        Core.push_back(I_core);
        if(I_core>max){max=I_core;}
        if(I_ang>max){max=I_ang;}
        IO.push_back(I_io);
        ANG.push_back(I_ang);
      }
    }
    myfile.close();
    vector<double> thresh_c(Time.size(),thresh_core);
    vector<double> thresh_a(Time.size(),thresh_ang);

    if(max<thresh_c[0]){max=thresh_c[0];}
    max=max*1.05;
 
    //Digital Core Current data
    TGraph * Digital_Core = new TGraph(Time.size(),&Time[0],&Core[0]);
    Digital_Core->SetName("Digital_Core");
    Digital_Core->SetLineColor(kBlue);
    Digital_Core->SetLineWidth(3);
    Digital_Core->GetYaxis()->SetRangeUser(0,max);
    // Digital_Core->SetTitle(Form("Latchup-%i",latchs));
    // Digital_Core->Draw("L");

    //Analog Current data
    TGraph * Analog = new TGraph(Time.size(),&Time[0],&ANG[0]);
    Analog->SetName("Analog");
    Analog->SetLineColor(kCyan);
    Analog->SetLineWidth(3);
    Analog->GetYaxis()->SetRangeUser(0,max);

    //Digital Core Threshold Line
    TGraph * CoreThresh = new TGraph(Time.size(),&Time[0],&thresh_c[0]);
    CoreThresh->SetName("CoreThresh");
    CoreThresh->SetLineColor(kRed);

    //Analog Threshold line
    TGraph * AngThresh = new TGraph(Time.size(),&Time[0],&thresh_a[0]);
    AngThresh->SetName("AngThresh");
    AngThresh->SetLineColor(kRed);

    //Compilation of all the previous Graphs, and Axis labels  
    TMultiGraph * all = new TMultiGraph(); 
    // sprintf(buffer,"Latchup-%i",latchs);
    all->SetTitle(buffer); 
    all->Add(Digital_Core);
    all->Add(Analog);
    all->Add(CoreThresh);
    all->Add(AngThresh);
    all->Draw("al");
    all->GetXaxis()->SetTitle("Time[ms]");
    all->GetYaxis()->SetTitle("Current [ADC Counts]");
    all->GetYaxis()->SetTitleOffset(1.4);
    all->GetYaxis()->CenterTitle();
    all->GetXaxis()->CenterTitle();

    //Right Hand Y-Axis
    TGaxis *axis = new TGaxis(1.715,0,1.715, max,0,max*330/4096,510,"+L");
    axis->Draw();
    axis->SetLineWidth(1);
    axis->SetTextFont(42);
    axis->SetTitleOffset(1.3);
    axis->SetTitle("Current [mA]");
    axis->CenterTitle();
    

    //Legend
    TLegend * leg = new TLegend(0.13,0.425,0.43,0.575);
    leg->AddEntry("Digital_Core","Digital Core Current","l");
    leg->AddEntry("CoreThresh",Form("Digital Core Threshold %i",thresh_core),"l");
    leg->AddEntry("Analog","Analog Current","l");
    leg->AddEntry("AngThresh",Form("Analog Threshold %i",thresh_ang),"l");
    leg->SetFillColor(kWhite);
    leg->Draw();

    //Clear
    delete leg, axis, Digital_Core, Analog, AngThresh, CoreThresh; /////////
  }
}
