//STACK SCRIPT


#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "THStack.h"
#include "TF1.h"
#include "TString.h"
#include <iostream>
#include <fstream>
#include <math.h>

using std::cout;
using std::endl;

// Declare all the other functions needed. See the function implementations for more
void decorate(TH1*h,const char* xtitle, const char* ytitle, const char* title,
	      int linecolor, int linewidth, int markercolor, int markerstyle, int tofill);
void decorate(TLegend *g, float textSize, TString legendheader);
float get_nevents(TH1F *hst, float bin_lo, float bin_hi);
float get_nevents_err(TH1F *hst, float bin_lo, float bin_hi);


/*
This function decorates the histogram, by setting the titles of the X and Y axes,
by setting the line and marker colors, the marker style, and by deciding whether
to fill in the histogram with a color or not.
 */
void decorate(TH1*h,const char* xtitle, const char* ytitle, const char* title,
	      int linecolor, int linewidth, int markercolor, int markerstyle, int tofill) {

  h->GetXaxis()->SetTitle(xtitle);
  h->GetYaxis()->SetTitle(ytitle);

  h->SetLineColor(linecolor); 
  h->SetLineWidth(linewidth);
  h->SetMarkerColor(markercolor);
  h->SetMarkerStyle(markerstyle);
  if(tofill==1) h->SetFillColor(markercolor);

  h->SetMarkerSize(1.0);
  h->SetTitle(title);
  
}
//This function decorates the legend, by setting the textsize, and filling in the legend with a pure white color.
void decorate(TLegend *g, float textSize, TString legendheader){
  // g->SetTextSize(textSize);
  g->SetTextSize(0.);
  g->SetFillStyle(1);
  g->SetFillColor(10);
  g->SetBorderSize(0);
  g->SetLineColor(10);
  //Usually legends should not have headers but if you want one, uncomment the next line.
  //g->SetHeader(legendheader);
}


// Here are a couple of other utility functions

// For a given histogram hst, return the number of entries between bin_lo and bin_hi
float get_nevents(TH1F *hst, float bin_lo, float bin_hi)
{
  int bin_width = hst->GetBinWidth(1);
  int ibin_begin = 1;
  float nevents = 0;
  while ( hst->GetBinCenter(ibin_begin) < bin_lo )
    ibin_begin++;
  int ibin_end = ibin_begin;
  while ( hst->GetBinCenter(ibin_end) < bin_hi )
    ibin_end++;
  for(int i=ibin_begin; i<ibin_end; i++)
    nevents += hst->GetBinContent(i);

  return nevents;
}
// Partner function for above, returning the error for the above nevents
float get_nevents_err(TH1F *hst, float bin_lo, float bin_hi)
{
  int bin_width = hst->GetBinWidth(1);
  int ibin_begin = 1;
  float nevents_err = 0;
  while ( hst->GetBinCenter(ibin_begin) < bin_lo )
    ibin_begin++;
  int ibin_end = ibin_begin;
  while ( hst->GetBinCenter(ibin_end) < bin_hi )
    ibin_end++;
  for(int i=ibin_begin; i<ibin_end; i++)
    nevents_err += pow(hst->GetBinError(i),2);
  nevents_err = sqrt(nevents_err);
  return nevents_err;
}

// This is the primary function.It opens two histogram files, extracts two histograms from them, and plots them on top of each other (i.e. overlays them)
TH1F *hfinal;
void stackoverlaymaker_SingleTop()
{

  //First declare the file names (just strings)
  //THESE ARE SIGNAL
  //-----------------------------------------------
  
  //signal-top
  TString filename_schannel               = "hst_singletop_schannel.root";
  TString filename_tchannel               = "hst_singletop_tchannel_top.root";
  TString filename_tWchannel            = "hst_singletop_tWchannel_top.root";
  //signal-antitop
  TString filename_tchannel_anti       = "hst_singletop_tchannel_antitop.root";
  TString filename_tWchannel_anti    = "hst_singletop_tWchannel_antitop.root";

  //THESE ARE BKG
  //----------------------------------------------------
  TString filename_ttbar                      = "hst_singletop_background_ttbar.root" ;
  //TString filename_ttbar2                      = "TTBar_TTTo2L2Nu_17thapril.root";
  
  TString filename_wjets                      = "hst_singletop_background_wjets.root" ;
  TString filename_ww                         = "hst_singletop_background_ww.root";

  TString filename_DY1                         = "DYJetsToLL_M10to50_4thmarch.root";
  TString filename_DY2                        = "DYJetsToLL_M50_4thmarch.root";
  
  TString filename_QCD1                     ="QCD_MuEnriched_20to30_4thmarch.root";
  TString filename_QCD2                     = "QCD_MuEnriched_30to50_4thmarch.root";
  TString filename_QCD3                     = "QCD_MuEnriched_50to80_4thmarch.root";
  TString filename_QCD4                     = "QCD_MuEnriched_80to120_4thmarch.root";
  TString filename_QCD5                     = "QCD_MuEnriched_120to170_4thmarch.root";
  TString filename_QCD6                     = "QCD_MuEnriched_170to300_4thmarch.root";
  TString filename_QCD7                     =  "QCD_MuEnriched_300to470_4thmarch.root";
  TString filename_QCD8                     = "QCD_MuEnriched_470to600_4thmarch.root";
  TString filename_QCD9                     = "QCD_MuEnriched_600to800_4thmarch.root";
  TString filename_QCD10                   = "QCD_MuEnriched_800to1000_4thmarch.root";
  
  //data
  
  TString filename_dataA                        = "data_A.root";
  TString filename_dataB                         = "data_B.root";
  TString filename_dataC                         = "data_C.root";
  TString filename_dataD                         = "data_D.root";

  
  //Now let us open the files
  TFile *file_s               = new TFile(filename_schannel);
  TFile *file_t               = new TFile(filename_tchannel);
  TFile *file_tW            = new TFile(filename_tWchannel);
  //
  TFile *file_t_anti       = new TFile(filename_tchannel_anti);
  TFile *file_tW_anti    = new TFile(filename_tWchannel_anti);
  //
  TFile *file_ttbar         = new TFile(filename_ttbar);
    
  TFile *file_wjets         = new TFile(filename_wjets);
  TFile *file_ww             = new TFile(filename_ww);
  TFile *file_dy1             = new TFile(filename_DY1);
  TFile *file_dy2             = new TFile(filename_DY2);
  TFile *file_qcd1          = new TFile(filename_QCD1);// 20 to 30
  TFile *file_qcd2             = new TFile(filename_QCD2);//30 to 50
  TFile *file_qcd3             = new TFile(filename_QCD3);//50 to 80
  TFile *file_qcd4             = new TFile(filename_QCD4);//80 to 120  
  TFile *file_qcd5             = new TFile(filename_QCD5);//120 to 170
  TFile *file_qcd6             = new TFile(filename_QCD6); //170 to 300
  TFile *file_qcd7             = new TFile(filename_QCD7);//300 to 470
  TFile *file_qcd8             = new TFile(filename_QCD8);//470 to 600
  TFile *file_qcd9             = new TFile(filename_QCD9); //600 to 800
  TFile *file_qcd10             = new TFile(filename_QCD10);//800 to 1000  

  //data
  
  TFile *file_dataA           = new TFile(filename_dataA);
  TFile *file_dataB           = new TFile(filename_dataB);
  TFile *file_dataC           = new TFile(filename_dataC);
  TFile *file_dataD           = new TFile(filename_dataD);
  //------------------------------------------------------------------------------------------------------------------------------
 
  TString plotnames[10]  = {"mu_sip3d_baseline" };

  TString xtitle[10]=   {"Leading Muon Sip 3D"};

  TString ytitle ="Events";
  // Can use latex-type commands in strings. For example "#mu^{1} p_{T}"
  


  TString plot;
  TH1F *h1,*h2,*h3,*h4,*h5,*h6,*h7,*h8, *h9, *h10,*h11, *h12, *h13, *h14, *h15, *h16, *h17, *h18, *h19, *h20, *h21, *h22, *h23, *h24, *h25,*h26;
  TH1F *h1new, *h2new, *h3new, *h4new, *h5new, *h6new, *h7new, *h8new ,*h9new,*h10new,*h11new,*h12new,*h13new,*h14new,*h15new,*h16new, *h17new,*h18new,*h19new, *h20new, *h21new, *h22new,*h23new ,*h24new ,*h25new ,*h26new  ; 
  
  for(int i=0; i<25; i++){
    // for(int i=0; i< sizeof(plotnames); i++){
    plot= plotnames[i]; 
    
    //Now open the respective histograms from the files
    h1 = (TH1F*)file_s->Get(plot);
    h2 = (TH1F*)file_t->Get(plot);
    h3 = (TH1F*)file_tW->Get(plot);
    //
    h4 = (TH1F*)file_ttbar->Get(plot);
    //h22 = (TH1F*)file_ttbar2->Get(plot);

    h5 = (TH1F*)file_wjets->Get(plot);
    h6 = (TH1F*)file_ww->Get(plot);
    h20 = (TH1F*)file_dy1->Get(plot);
    h21 = (TH1F*)file_dy2->Get(plot);
    h10 = (TH1F*)file_qcd1->Get(plot);
    h11 = (TH1F*)file_qcd2->Get(plot);
    h12 = (TH1F*)file_qcd3->Get(plot);
    h13 = (TH1F*)file_qcd4->Get(plot);
    h14 = (TH1F*)file_qcd5->Get(plot);
    h15 = (TH1F*)file_qcd6->Get(plot);
    h16 = (TH1F*)file_qcd7->Get(plot);
    h17 = (TH1F*)file_qcd8->Get(plot);
    h18 = (TH1F*)file_qcd9->Get(plot);
    h19 = (TH1F*)file_qcd10->Get(plot);
    //
    h7 = (TH1F*)file_t_anti->Get(plot);
    h8 = (TH1F*)file_tW_anti->Get(plot);
    //
    //h9 = (TH1F*)file_data->Get(plot);
    h23 = (TH1F*)file_dataA->Get(plot);
    h24 = (TH1F*)file_dataB->Get(plot);
    h25 = (TH1F*)file_dataC->Get(plot);
    h26 = (TH1F*)file_dataD->Get(plot);
    
  
    //Decorate the histograms using function decorate
    
    decorate(h1,xtitle[i],ytitle,"",kRed+1,2,kRed+1,20,0); //signal
    decorate(h2,xtitle[i],ytitle,"",kRed,2,kRed,22,0);
    decorate(h3,xtitle[i],ytitle,"",kBlue,2,kBlue,21,0);
    //
    decorate(h7,xtitle[i],ytitle,"",kPink+1,2,kPink+1,22,0);
    decorate(h8,xtitle[i],ytitle,"",kTeal-7,2,kTeal-7,21,0);
    //bkg
    decorate(h4,xtitle[i],ytitle,"",kOrange-2,2,kOrange-2,20,1);//ttbar
    decorate(h5,xtitle[i],ytitle,"",kBlue+1,2,kBlue+1,20,1);//wjets
    decorate(h6,xtitle[i],ytitle,"", kRed+3,2,kRed+3,20,1);//ww
    
    decorate(h20,xtitle[i],ytitle,"", kAzure+6,2,kAzure+6,20,1);//DY
    decorate(h21,xtitle[i],ytitle,"", kAzure+6,2,kAzure+6,20,1);//DY

    //--------------------
    decorate(h10,xtitle[i],ytitle,"",kGray+1,2,kGray+1,20,1);//qcd
    decorate(h11,xtitle[i],ytitle,"",kGray+1,2,kGray+1,20,1);//qcd
    decorate(h12,xtitle[i],ytitle,"",kGray+1,2,kGray+1,20,1);//qcd
    decorate(h13,xtitle[i],ytitle,"",kGray+1,2,kGray+1,20,1);//qcd
    decorate(h14,xtitle[i],ytitle,"",kGray+1,2,kGray+1,20,1);//qcd
    decorate(h15,xtitle[i],ytitle,"",kGray+1,2,kGray+1,20,1);//qcd
    decorate(h16,xtitle[i],ytitle,"",kGray+1,2,kGray+1,20,1);//qcd
    decorate(h17,xtitle[i],ytitle,"",kGray+1,2,kGray+1,20,1);//qcd
    decorate(h18,xtitle[i],ytitle,"",kGray+1,2,kGray+1,20,1);//qcd
    decorate(h19,xtitle[i],ytitle,"",kGray+1,2,kGray+1,20,1);//qcd

    //data
    decorate(h23,xtitle[i],ytitle,"",kBlack,2,kBlack,20,0);//data
    decorate(h24,xtitle[i],ytitle,"",kBlack,2,kBlack,20,0);//data
    decorate(h25,xtitle[i],ytitle,"",kBlack,2,kBlack,20,0);//data
    decorate(h26,xtitle[i],ytitle,"",kBlack,2,kBlack,20,0);//data

    // the 1 or 0 here means- 1=fill the color and 0=doesn't fill the color


    //SCALING------------------------------------------------------------------------------------
    //luminosity you want to scale to
    /*
    //xsec
    s= 3.549;
    t=113.4;
    tW= 32.45;
    ttbar=365.34;
    wjets=53450;
    ww=51.14;
    t_anti =67.93
    tW_anti =32.51
    qcd_inclusive =  239000 +- 1432
    qcd1 =2538000 // 20 to 30
    qcd2 = 1369000 //30 to 50
    qcd3 = 378000 //50 to 80
    qcd4 = 89430 //80 to 120
    qcd5 = 21090 //120 to 170
    qcd6 = 7001 //170 to 300
    qcd7 = 618.9 //300 to 470
    qcd8 = 58.54 //470 to 600
    qcd9 = 18.05 //600 to 800
    qcd10 = 3.281 //800 to 1000
    DY1=15940 //10 to 50
    DY2= 6448//above50
    ttbar2 = 88.29 //dileptonic


    //N
    s= 2010000;
    t=2709000;
    tW=25106 ;275000
    ttbar=14190000;
    wjets=9698198;
    ww=8820771;
    t_anti = 2373000
    tW_anti =176000
    qcd_inclusive = 17392470

    qcd1 = 60640520// 20 to 30
    qcd2 = 5873770 //30 to 50
    qcd3 =  40022460//50 to 80
    qcd4 =  4594360 //80 to 120
    qcd5 = 38022190 //120 to 170
    qcd6 = 71870980 //170 to 300
    qcd7 =  58949760 //300 to 470
    qcd8 = 38453300 //470 to 600
    qcd9 =  37197940//600 to 800
    qcd10 = 78942990//800 to 1000
    DY1   =94452820/10 to 50
    DY2=195510800//50
    ttbar2 = 145020000//dileptonic
    */
    
    //lumi= N/xsec
    float  lumi_s =  2010000/3.549;
    float  lumi_t=2709000/113.4;
    float  lumi_tW=275000/32.45;
    //
    float lumi_ttbar=14190000/365.34;
    //float lumi_ttbar_dilep=145020000/88.29;
    float lumi_wjets=9698198/53450;
    float lumi_ww = 8820771/51.14;
    float lumi_dy1 = 94452820/15940;
    float lumi_dy2 = 195510800/6448;
    // float lumi_qcd_inclusive =17392470 /239000 ;
    float lumi_qcd1 = 60640520/2538000;// 20 to 30
    float lumi_qcd2 = 5873770/1369000;//30 to 50
    float lumi_qcd3 = 40022460/378000;//50 to 80
    float lumi_qcd4 = 4594360/89430;//80 to 120
    float lumi_qcd5 = 38022190/21090;//120 to 170
    float lumi_qcd6 = 71870980/7001; //170 to 300
    float lumi_qcd7 = 58949760/618.9; //300 to 470
    float lumi_qcd8 = 38453300/58.54; //470 to 600
    float lumi_qcd9 = 37197940/18.05; //600 to 800
     float lumi_qcd10 =78942990/3.281;//800 to 1000
    //
    float  lumi_t_anti=2373000/67.93;
    float  lumi_tW_anti=176000/32.51;

    //
    float lumi_data = 59.8*1000 ;  //2018 lumi - picobarn
    //float lumi_data = 1.946*1000 ;//2018_SingleMuonA_Somefiles - picobarn

    //
    float SF = 0.0695362;//sip3d
    //  float SF = 0.0576178; //iso

   
    
    h1->Scale(lumi_data/lumi_s);
    h2->Scale(lumi_data/lumi_t);
    h3->Scale(lumi_data/lumi_tW);
    h4->Scale(lumi_data/lumi_ttbar);
    // h22->Scale(lumi_data/lumi_ttbar_dilep);
    h5->Scale(lumi_data/lumi_wjets);
    h6->Scale(lumi_data/lumi_ww);
    h20->Scale(lumi_data/lumi_dy1);
    h21->Scale(lumi_data/lumi_dy2);
    //
    h7->Scale(lumi_data/lumi_t_anti);
    h8->Scale(lumi_data/lumi_tW_anti);
    h10->Scale((lumi_data* SF)/lumi_qcd1);
    h11->Scale((lumi_data* SF)/lumi_qcd2);
    h12->Scale((lumi_data* SF)/lumi_qcd3);
    h13->Scale((lumi_data* SF)/lumi_qcd4);
    h14->Scale((lumi_data* SF)/lumi_qcd5);
    h15->Scale((lumi_data* SF)/lumi_qcd6);
    h16->Scale((lumi_data* SF)/lumi_qcd7);
    h17->Scale((lumi_data* SF)/lumi_qcd8);
    h18->Scale((lumi_data* SF)/lumi_qcd9);
    h19->Scale((lumi_data* SF)/lumi_qcd10);
    


    // Now rebin the histograms if needed- Group nrebins bins together
    int nrebins = 30;//pt, pz

    //------------------------------------------------------------------------------------------------
    h1new=(TH1F*)h1->Rebin(nrebins);
    h2new=(TH1F*)h2->Rebin(nrebins);
    h3new=(TH1F*)h3->Rebin(nrebins);
    h4new=(TH1F*)h4->Rebin(nrebins);
    h5new=(TH1F*)h5->Rebin(nrebins);
    h6new=(TH1F*)h6->Rebin(nrebins);	
    //
    h7new=(TH1F*)h7->Rebin(nrebins);
    h8new=(TH1F*)h8->Rebin(nrebins);
    //
    h20new=(TH1F*)h20->Rebin(nrebins);
    h21new=(TH1F*)h21->Rebin(nrebins);
    //
    // h9new=(TH1F*)h9->Rebin(nrebins);
    h23new=(TH1F*)h23->Rebin(nrebins);
    h24new=(TH1F*)h24->Rebin(nrebins);
    h25new=(TH1F*)h25->Rebin(nrebins);
    h26new=(TH1F*)h26->Rebin(nrebins);

    h10new=(TH1F*)h10->Rebin(nrebins);
    h11new=(TH1F*)h11->Rebin(nrebins);
    h12new=(TH1F*)h12->Rebin(nrebins);
    h13new=(TH1F*)h13->Rebin(nrebins);
    h14new=(TH1F*)h14->Rebin(nrebins);
    h15new=(TH1F*)h15->Rebin(nrebins);
    h16new=(TH1F*)h16->Rebin(nrebins);
    h17new=(TH1F*)h17->Rebin(nrebins);
    h18new=(TH1F*)h18->Rebin(nrebins);
    h19new=(TH1F*)h19->Rebin(nrebins);

    // h22new=(TH1F*)h22->Rebin(nrebins);
    
    //h9new->Rebin(nrebins);

    
    //Now we scale the histograms by normalizing the histograms such that they have the same integral.usefull when comparing shapes, we normalize to 1
    /*
      h1->Scale(1.0/h1->Integral());
      h2->Scale(1.0/h2->Integral());
      h4->Scale(1.0/h4->Integral());
    */

    
    //Now let us set the last bin as the overflow bin
    int nbins = h1->GetNbinsX();
    
    h1new->SetBinContent(nbins,h1new->GetBinContent(nbins+1)+h1new->GetBinContent(nbins));
    h2new->SetBinContent(nbins,h2new->GetBinContent(nbins+1)+h2new->GetBinContent(nbins));
    h3new->SetBinContent(nbins,h3new->GetBinContent(nbins+1)+h3new->GetBinContent(nbins));
    h4new->SetBinContent(nbins,h4new->GetBinContent(nbins+1)+h4new->GetBinContent(nbins));
    h5new->SetBinContent(nbins,h5new->GetBinContent(nbins+1)+h5new->GetBinContent(nbins));
    h6new->SetBinContent(nbins,h6new->GetBinContent(nbins+1)+h6new->GetBinContent(nbins));
    
    
    h7new->SetBinContent(nbins,h7new->GetBinContent(nbins+1)+h7new->GetBinContent(nbins));
    h8new->SetBinContent(nbins,h8new->GetBinContent(nbins+1)+h8new->GetBinContent(nbins));
    
    
    //h9new->SetBinContent(nbins,h9new->GetBinContent(nbins+1)+h9new->GetBinContent(nbins));
    
    h23new->SetBinContent(nbins,h23new->GetBinContent(nbins+1)+h23new->GetBinContent(nbins));
    h24new->SetBinContent(nbins,h24new->GetBinContent(nbins+1)+h24new->GetBinContent(nbins));
    h25new->SetBinContent(nbins,h25new->GetBinContent(nbins+1)+h25new->GetBinContent(nbins));
    h26new->SetBinContent(nbins,h26new->GetBinContent(nbins+1)+h26new->GetBinContent(nbins));

   
    h10new->SetBinContent(nbins,h10new->GetBinContent(nbins+1)+h10new->GetBinContent(nbins));
    h11new->SetBinContent(nbins,h11new->GetBinContent(nbins+1)+h11new->GetBinContent(nbins));
    h12new->SetBinContent(nbins,h12new->GetBinContent(nbins+1)+h12new->GetBinContent(nbins));
    h13new->SetBinContent(nbins,h13new->GetBinContent(nbins+1)+h13new->GetBinContent(nbins));
    h14new->SetBinContent(nbins,h14new->GetBinContent(nbins+1)+h14new->GetBinContent(nbins));
    h15new->SetBinContent(nbins,h15new->GetBinContent(nbins+1)+h15new->GetBinContent(nbins));
    h16new->SetBinContent(nbins,h16new->GetBinContent(nbins+1)+h16new->GetBinContent(nbins));
    h17new->SetBinContent(nbins,h17new->GetBinContent(nbins+1)+h17new->GetBinContent(nbins));
    h18new->SetBinContent(nbins,h18new->GetBinContent(nbins+1)+h18new->GetBinContent(nbins));
    h19new->SetBinContent(nbins,h19new->GetBinContent(nbins+1)+h19new->GetBinContent(nbins));
  
    h20new->SetBinContent(nbins,h20new->GetBinContent(nbins+1)+h20new->GetBinContent(nbins));
    h21new->SetBinContent(nbins,h21new->GetBinContent(nbins+1)+h21new->GetBinContent(nbins));
    //   h22new->SetBinContent(nbins,h22new->GetBinContent(nbins+1)+h22new->GetBinContent(nbins));
  
  
  
    //    float n_ttbar =get_nevents(h4new,0,5)+get_nevents(h22new,0,5);
    /*
    float n_ttbar =get_nevents(h4new,0,5);
    float n_ww = get_nevents(h6new,0,5);
    float n_wjets  = get_nevents(h5new,0,5);
    float n_signal = get_nevents(h1new,0,5) + get_nevents(h2new,0,5)+get_nevents(h3new,0,5) + get_nevents(h7new,0,5) + get_nevents(h8new,0,5);

    float n_DY  = get_nevents(h20new,0,5)+ get_nevents(h21new,0,5);
    float n_qcd  = get_nevents(h10new,0,5)+ get_nevents(h11new,0,5)+get_nevents(h12new,0,5) + get_nevents(h13new,0,5) + get_nevents(h14new,0,5) + get_nevents(h15new,0,5) + get_nevents(h16new,0,5) + get_nevents(h17new,0,5)+ get_nevents(h18new,0,5) + get_nevents(h19new,0,5);


    
    float n_data  = get_nevents(h9new,0,5);
   
    cout<<"Data events = "<< n_data <<endl;
    cout<<"QCD events = "<< n_qcd <<endl;
    cout<<"ttbar events = "<<  n_ttbar<<endl;
    cout<<"ww events = "<< n_ww<<endl;
    cout<<"w+jets events = "<< n_wjets <<endl;
    cout<<"DY events = "<< n_DY <<endl;
    cout<<"signal events = "<< n_signal <<endl;
    
    float n_num =n_data - (n_qcd + n_ww + n_wjets + n_DY);
    // float n_num =n_data - (n_ttbar + n_ww + n_wjets);
    // get_nevents(h9new,1.25, 2.25)-(get_nevents(h4new,1.25, 2.25)+get_nevents(h5new,1.25, 2.25)+get_nevents(h6new,1.25, 2.25));
    
    //    float scale_factor_qcd= n_num / n_qcd ;
    float scale_factor_ttbar= n_num / n_ttbar ;
    cout<<"Scale Factor for ttbar = " <<scale_factor_ttbar<<endl;
    */

    
    //STACKING-------------------------------------------------------------------------------------------------------
    THStack *stack = new THStack("Stacked","");
    /*
      h1= schannel
      h2= tchannel
      h3= tWchannel
      h4= ttbar
      h5= wjets
      h6= ww
      h7= tchannel_anti
      h8= tWchannel_anti
      h9= data
      h10= qcd
    */
    
    //adding the Signal
    h1new->Add(h2new,1); //this means- h1new = h1new+h2new
    h1new->Add(h3new,1); //this means-h1new = h1new+h2new+h3new
    h1new->Add(h7new,1); //this means-h1new = h1new+h2new+h3new+h7new
    h1new->Add(h8new,1); //this means-h1new = h1new+h2new+h3new+h7new+h8new

    //adding QCD
    h10new -> Add(h11new,1);
    h10new -> Add(h12new,1);
    h10new -> Add(h13new,1);
    h10new -> Add(h14new,1);
    h10new -> Add(h15new,1);
    h10new -> Add(h16new,1);
    h10new -> Add(h17new,1);
    h10new -> Add(h18new,1);
    

    //adding DY
    h20new -> Add(h21new,1);


    //adding dat
    h23new -> Add(h24new,1);
    h23new -> Add(h25new,1);
    h23new -> Add(h26new,1);

    
    stack->Add(h20new);//DY
    stack->Add(h5new);//wjets
    stack->Add(h6new);//ww
    stack->Add(h10new);//QCD
    stack->Add(h4new);//ttbar
    stack->Add(h1new);//signal (MC)
  
    //Now let us declare a canvas 
    TCanvas *canvas = new TCanvas(plot,plot,800,600);
    // TCanvas *c = new TCanvas();
    // c("plot","plot",1000,800);
    
    //stack->Draw("HIST");
    /*
      hfinal->Add(h1new,1); //this means- hfinal = h1new+hfinal
      hfinal->Add(h2new,1); //this means- hfinal = h1new+hfinal+h2new
      hfinal->Add(h3new,1); //this means- hfinal = h1new+hfinal+h2new+h3new

      hfinal->Draw("hist same");
    */
    //h9new->Draw("ep");//data

    h23new->Draw("ep");//data
    
    stack->Draw("hist same");
    //h9new->SetStats(0);
    h23new->SetStats(0);
    //stack->Draw("hist same ");
    //    h1new->Draw("hist same");
    // h9new->Draw("ep same");//data
    h23new->Draw("ep same");//data
    
    
    
    // h2new->Draw("hist same");
    // h3new->Draw("hist same");
    // h7new->Draw("hist same");
    // h8new->Draw("hist same");
   

    canvas->SetTickx(1);
    canvas->SetTicky(1);
    stack->GetXaxis()->SetTitle(xtitle[i]);
    stack->GetYaxis()->SetTitle(ytitle);
    stack->GetYaxis()->SetTitleOffset(1);
    stack->GetYaxis()->SetTitleSize(0.04);
    canvas->SetLogy(1);
    // stack->SetMaximum(1000);
    stack->SetMinimum(10);

    //We set the stat box for the first one to be invisible first
    // h1->SetStats(0);
    // Now we define a legend
    TLegend *lg1 = new TLegend(0.65,0.60,0.88,0.86,NULL,"NDC");
    decorate(lg1,0.05,""); // Decorated the legend using function below.
    TString name;
    //
    name = "Signal [" + std::to_string((int)h1new->Integral()) + "]";
    lg1->AddEntry(h1new,name,"f");
  
    name = "ttbar [" + std::to_string((int)h4new->Integral()) + "]";
    lg1->AddEntry(h4new,name,"f");

    name = "QCD [" + std::to_string((int)h10new->Integral()) + "]";
    lg1->AddEntry(h10new,name,"f");

    name = "wjets [" + std::to_string((int)h5new->Integral()) + "]";
    lg1->AddEntry(h5new,name,"f");

    name = "DY [" + std::to_string((int)h20new->Integral()) + "]";
    lg1->AddEntry(h20new,name,"f");

    name = "ww [" + std::to_string((int)h6new->Integral()) + "]";
    lg1->AddEntry(h6new,name,"f");
    
   
    //    name = "data [" + std::to_string((int)h9new->Integral()) + "]";
    //lg1->AddEntry(h9new,name,"ep");
    name = "data [" + std::to_string((int)h23new->Integral()) + "]";
    lg1->AddEntry(h23new,name,"ep");
  
    lg1->Draw(); 

    canvas->SaveAs("/home/uttasavi/Uttsavi/Work/SingleTopAnalysis/single_top_plots/"+plot+".png");
    //
    // name = "tchannel (antitop) [" + std::to_string((int)h7new->Integral()) + "]";
    //    lg1->AddEntry(h7new,name,"l");
    //    name = "tWchannel (antitop) [" + std::to_string((int)h8new->Integral()) + "]";
    //    lg1->AddEntry(h8new,name,"l");
    
    //--------------------------------------------------------------------------
    
  }//for loop


  cout<<"_______________________________________________________________\n"<<endl;
  cout<<"schannel =19456 "<<endl;
  cout<<"tchannel(top) =197058 "<<endl;
  cout<<"tchannel(antitop) =127524 "<<endl;
  cout<<"tWchannel(top) =160751 "<<endl;
  cout<<"tWchannel(antitop) =157726 "<<endl;
  cout<<"_____________________________  END _______________________________\n"<<endl;
  
}//overlay_SingleTop











//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 /*
    TString plotnames[4]   ={"mu0_eta_baseline","jet0_Eta_baseline","jet1_Eta_baseline","bjet0_Eta_baseline"};
    TString xtitle[4]   ={ "Leading Muon Eta- baseline", "Leading Jet Eta- baseline" ,"Subleading Jet Eta- baseline","Leading bJet Eta- baseline"};
    
  
    TString plotnames[14]   ={"mu0_phi_baseline", "jet0_Phi_baseline", "jet1_Phi_baseline","bjet0_Phi_baseline","puppi_met_phi_baseline"};

    TString xtitle[14]   ={ "Leading Muon Phi- baseline","Leading Jet Phi- baseline", "Subleading Jet Phi- baseline","Leading bJet Phi- baseline","MET phi- baseline" }; 
  
  
    TString plotnames[25]  ={"mu0_pT_baseline", "Lt_baseline","ST_baseline", "jet0_pT_baseline","jet1_pT_baseline","Ht_baseline","dijet_pt_baseline","bjet0_pT_baseline","puppi_met_pT_baseline","Ht_50_baseline","Ht_70_baseline" };
  
    TString xtitle[25]={"Leading Muon p_{T}- baseline","LT- baseline","ST- baseline","Leading Jet p_{T}- baseline","Subleading Jet p_{T}- baseline", "Scalar sum of p_{T} of J0 & J1-(HT')- baseline","Dijet p_{T}- baseline","Leading b_jet p_{T}- baseline","MET - baseline","HT50- baseline","HT70- baseline"};
    
    TString plotnames[14]=  {"n_ele_baseline","n_lep_baseline","nMuon_baseline","n_jets_baseline","n_bjets_baseline"};
    TString xtitle[14]   ={"No of Electrons- baseline","No of Leptons- baseline","No of Muons- baseline","No of Jets- baseline","No of bJets- baseline"};
  
    
    TString plotnames[5] ={"mT_mu0_baseline","mT_Jet0_baseline","mT_Jet1_baseline","mT_bJet_baseline","mT_J0J1_baseline"};
    TString xtitle[5]={"Transverse mass Leading Muon- baseline","Transverse mass Leading Jet- baseline","Transverse mass Subleading Jet- baseline","Transverse mass bJet- baseline","Transverse mass Jet System- baseline"};
  
  
    TString plotnames[13]= {"dR_jets_baseline","dR_mu0j0_baseline","dR_mu0j1_baseline","dR_mu0bj0_baseline"};
    TString xtitle[13]= { "DeltaR b/w Jets- baseline","DeltaR b/w Mu0 and Jet0- baseline","DeltaR b/w Mu0 and Jet1- baseline","dR b/w Mu0 and bJet0- baseline"};
  
  
    TString plotnames[10]=  {"dphi_jets_baseline","dphi_mu0j0_baseline","dphi_mu0j1_baseline","dphi_mu0bj0_baseline","dphi_mu0MET_baseline", "dphi_J0MET_baseline", "dphi_J1MET_baseline","dphi_bJ0MET_baseline","dphi_J0J1MET_baseline","dphi_J0J1_mu0_baseline"};

    TString xtitle[10]= {"Deltaphi b/w Jets- baseline","Deltaphi b/w Mu0 and Jet0- baseline","Deltaphi b/w Mu0 and Jet1- baseline","dphi b/w Mu0 and bJet0- baseline","Delta phi b/w MET and mu0- baseline","Delta phi b/w MET and J0- baseline","Delta phi b/w MET and J1- baseline","Delta phi b/w MET and bjet0- baseline","Delta phi b/w MET and 2 jet system- baseline","Delta phi b/w Leading Muon and 2 jet system- baseline"};
  
    
    TString plotnames[14]= { "mu_dxy_baseline" ,"mu_dz_baseline"};
    TString xtitle[14]   ={"dxy of Leading Muon- baseline", "dz of Leading Muon- baseline"};
  
  
    TString plotnames[14]=  {"mu_sip3d_baseline" };
    TString xtitle[14] ={"Muon 3D impact parameter significance- baseline" };


    TString plotnames[14]=  {"Iso_mu0_baseline"};
    TString xtitle[14]   ={"Isolation of leading Muon- baseline"};
  */
  //-----------------------------------------------------------------------------------


  // TString plotnames[14]=  {"n_ele_baseline","n_lep_baseline","nMuon_baseline","n_jets_baseline","n_bjets_baseline"};
  //  TString xtitle[14]   ={"No. of Electrons","No. of Leptons","No. of Muons","No. of Jets","No. of bJets"};
  


  /*  
  TString plotnames[14]=  {"nMuon_cut1","n_jets_cut1","n_bjets_cut1"}; // after additional cuts
  TString xtitle[14]   ={"No of Muons","No of Jets","No of b-Jets"};
  
  
  TString plotnames[6]  ={"mu0_pT_baseline", "jet0_pT_baseline","jet1_pT_baseline","bjet0_pT_baseline","Ht_70_baseline" ,"Ht_baseline"};
  
  TString xtitle[6]={"Leading Muon p_{T}","Leading Jet p_{T}","Subleading Jet p_{T}","Leading b-jet p_{T}","HT_{70}","Ht"};
  
    
  TString plotnames[4]   ={"mu0_eta_baseline","jet0_Eta_baseline","jet1_Eta_baseline","bjet0_Eta_baseline"};
  TString xtitle[4]   ={ "Leading Muon Eta", "Leading Jet Eta" ,"Subleading Jet Eta","Leading bJet Eta"};
  
  
  TString plotnames[14]   ={"mu0_phi_baseline", "jet0_Phi_baseline", "jet1_Phi_baseline","bjet0_Phi_baseline","puppi_met_phi_baseline"};
  TString xtitle[14]   ={ "Mu0 Phi","Jet0 Phi", "Leading Jet Phi","Subleading bJet Phi","MET phi" }; 
  
  
  TString plotnames[4]=  {"mu_sip3d_baseline", "Iso_mu0_baseline" , "mu_dxy_baseline" ,"mu_dz_baseline"};
  TString xtitle[4] ={"Leading Muon Sip 3D","Leading Muon Isolation", "Leading Muon dxy", "Leading Muon dz"};

 
  
 TString plotnames[4]   ={"mu0_eta_baseline","jet0_Eta_baseline","jet1_Eta_baseline","bjet0_Eta_baseline"};
    TString xtitle[4]   ={ "Leading Muon Eta- baseline", "Leading Jet Eta- baseline" ,"Subleading Jet Eta- baseline","Leading bJet Eta- baseline"};
  
  
    TString plotnames[14]   ={"mu0_phi_baseline", "jet0_Phi_baseline", "jet1_Phi_baseline","bjet0_Phi_baseline"};

  TString xtitle[14]   ={ "Leading Muon Phi- baseline","Leading Jet Phi- baseline", "Subleading Jet Phi- baseline","Leading bJet Phi- baseline" }; 
  
    TString plotnames[10]  = {"jet_score_sum_baseline" };
    TString xtitle[10]= {"Sum of leading and subleading Jet Score"};
  */










//-------------------------------------------CUTS---------------------------------------------------------------
  
  
  //  TString plotnames[10]  =  {" n_EvtRan"," n_EvtTotal"};
  //  TString xtitle[10]=  {"No of Events Ran", "No of Good Events"};
 
  // TString plotnames[10]  = {"evt_wt_cut"};
  // TString xtitle[10]= {"Event Weight- cut"};
  

  // TString plotnames[14]=  {"nMuon_cut1","n_jets_cut1","n_bjets_cut1"}; // after additional cuts
  //TString xtitle[14]   ={"No of Muons- cut","No of Jets- cut","No of bJets- cut"};

  //String plotnames[14]=  {"n_jets_cut1"}; // after additional cuts
  //tring xtitle[14]   ={"No of Jets- cut"};

  // TString plotnames[10]  = {"jet_score_sum_cut1" ,"jet_score_product_cut1"};
  // TString xtitle[10]= {"Sum of Jet Score- cut","Product of Jet Score- cut"};
  
  
  //"St_cut1"
  //"ST- cut",
  //  TString plotnames[25]  ={"mu0_pT_cut1", "Lt_cut1", "jet0_pT_cut1","jet1_pT_cut1","Ht_cut1","Ht_j0j1_cut1","dijet_pt_cut1","bjet0_pT_cut1","puppi_met_pT_cut1","Ht_50_cut1","Ht_70_cut1"  };
  
  //TString xtitle[25]={"Leading Muon p_{T}- cut","LT- cut","Leading Jet p_{T}- cut","Subleading Jet p_{T}- cut","HT- cut", "Scalar sum of p_{T} of J0 & J1-(HT')- cut","Dijet p_{T}- cut","Leading b_jet p_{T}- cut","MET - cut","HT50- cut","HT70- cut"};

  
  // TString plotnames[14]   ={"mu0_eta_cut","jet0_Eta_cut","jet1_Eta_cut","bjet0_Eta_cut"};
  // TString xtitle[14]   ={ "Leading Muon Eta- cut", "Leading Jet Eta- cut" "Subleading Jet Eta- cut","Leading bJet Eta- cut"}; 



  
  // TString plotnames[14]   ={"mu0_phi_cut1", "jet0_Phi_cut1", "jet1_Phi_cut1","bjet0_Phi_cut1","puppi_met_phi_cut1"};
  // TString xtitle[14]   ={ "Leading Muon Phi- cut","Leading Jet Phi- cut", "Subleading Jet Phi- cut","Leading bJet Phi- cut","MET phi- cut" }; 



  
  // TString plotnames[13]= {"dR_jets_cut1","dR_mu0j0_cut1","dR_mu0j1_cut1","dR_mu0bj0_cut1"}; 
  //TString xtitle[13]= { "DeltaR b/w Jets- cut","DeltaR b/w Mu0 and Jet0- cut","DeltaR b/w Mu0 and Jet1- cut","dR b/w Mu0 and bJet0- cut"};

  
  
  //TString plotnames[10]={"dphi_jets_cut1","dphi_mu0j0_cut1","dphi_mu0j1_cut1","dphi_mu0bj0_cut1","dphi_mu0MET_cut1", "dphi_J0MET_cut1", "dphi_J1MET_cut1","dphi_bJ0MET_cut1","dphi_J0J1MET_cut1","dphi_J0J1_mu0_cut1"};
  
  //TString xtitle[10]= {"Deltaphi b/w Jets- cut","Deltaphi b/w Mu0 and Jet0- cut","Deltaphi b/w Mu0 and Jet1- cut","dphi b/w Mu0 and bJet0- cut","Delta phi b/w MET and mu0- cut","Delta phi b/w MET and J0- cut","Delta phi b/w MET and J1- cut","Delta phi b/w MET and bjet0- cut","Delta phi b/w MET and 2 jet system- cut","Delta phi b/w Leading Muon and 2 jet system- cut"};



  //TString plotnames[5] ={"mT_mu0_cut1","mT_Jet0_cut1","mT_Jet1_cut1","mT_bJet_cut1","mT_J0J1_cut1"};
  // TString xtitle[5]={"Transverse mass Leading Muon- cut","Transverse mass Leading Jet- cut","Transverse mass Subleading Jet- cut","Transverse mass bJet- cut","Transverse mass Jet System- cut"};



  
  
  //TString plotnames[4]={"invar_mass_mu0j0_cut1","invar_mass_mu0j1_cut1","invar_jet_mass_cut1","invar_mu0bjet_mass_cut1"};
  // TString xtitle[4] ={"Invariant Mass - Leading Muon & Leading Jet- cut","Invariant Mass - Leading Muon & Subleading Jet- cut","Invariant Jet Mass- cut","Invariant Mass - Leading Muon & Leading bJet- cut"};




  
  //  TString plotnames[14]= { "mu_dxy_cut1" ,"mu_dz_cut1"};
  //  TString xtitle[14]   ={"dxy of Leading Muon- cut", "dz of Leading Muon- cut"};

  

  //TString plotnames[14]=  {"Iso_mu0_cut1"};
  //  TString xtitle[14]   ={"Isolation of leading Muon- cut"};



  
  //TString plotnames[14]=  {"objects_pz_cut1"};
  // TString xtitle[14]   ={ "P_{z} of all the Objects in the Event Selection- cut"};



  // TString plotnames[14]=  {"mu_sip3d_cut1" };
  // TString xtitle[14] ={"Muon 3D impact parameter significance- cut" };
  

// int nrebins = 2;//eta phi
//  int nrebins = 4;// dphi
//int nrebins = 20;//mt, invar_mass,
//int nrebins = 5;//dr
//   int nrebins = 2;//dphi
// int nrebins = 1;//no of
//  int nrebins = 20;//jet score
// int nrebins = 20;//iso,dxy,dz
//      int nrebins = 20;//evt wt
//int nrebins = 10;//iso
//int nrebins = 5;//sip3d

