////////////////////////////////////////////////////////////////////////////////
//
// jet_inspect_histos_x
// --------------------
//
//            07/19/2009 Philipp Schieferdecker <philipp.schieferdecker@cern.ch>
////////////////////////////////////////////////////////////////////////////////


#include "JetMETAnalysis/JetUtilities/interface/CommandLine.h"
#include "JetMETAnalysis/JetUtilities/interface/ObjectLoader.h"
#include "JetMETAnalysis/JetUtilities/interface/RootStyle.h"

#include <TApplication.h>
#include <TStyle.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TH1F.h>
#include <TF1.h>
#include <TText.h>
#include <TLine.h>
#include <TLegend.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>


using namespace std;


////////////////////////////////////////////////////////////////////////////////
// declare local functions
////////////////////////////////////////////////////////////////////////////////
void set_xaxis_range(TH1* h1,TH1* h2=0);
void get_xaxis_range(TH1* h,int& binmin,int& binmax);
void set_yaxis_range(TH1* h1,float ymin=-1.,float ymax=-1.);
void set_draw_attributes(TH1* h,
			 unsigned index,
			 bool fill,
			 const vector<int>& colors,
			 const vector<int>& fillstyles);
void draw_stats(TH1* h,double xoffset,Color_t color,Color_t fitColor);
void draw_range(const ObjectLoader<TH1F>& hl,
		const vector<unsigned int>& indices,
		bool  addFixedVars=true);
void draw_line_mean(TH1* h);
void draw_line_median(TH1* h);
void draw_line_peak(TH1* h);
void draw_line_legend(bool mean,bool median,bool peak);


////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
int main(int argc,char** argv)
{
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;

  vector<string> inputs     = cl.getVector<string>("inputs");
  vector<string> algs       = cl.getVector<string>("algs",          "kt4calo");
  vector<string> variables  = cl.getVector<string>("variables","RelRsp:RefPt");
  int            npercanvas = cl.getValue<int>    ("npercanvas",            0);
  float          ymin       = cl.getValue<float>  ("ymin",                 -1);
  float          ymax       = cl.getValue<float>  ("ymax",                 -1);
  bool           norm       = cl.getValue<bool>   ("norm",              false);
  bool           mean       = cl.getValue<bool>   ("mean",              false);
  bool           median     = cl.getValue<bool>   ("median",            false);
  bool           peak       = cl.getValue<bool>   ("peak",              false);
  bool           logx       = cl.getValue<bool>   ("logx",              false);
  bool           logy       = cl.getValue<bool>   ("logy",              false);
  bool           fill       = cl.getValue<bool>   ("fill",               true);
  bool           fullfit    = cl.getValue<int>    ("fullfit",            true);
  vector<int>    colors     = cl.getVector<int>   ("colors",               "");
  vector<int>    fillstyles = cl.getVector<int>   ("fillstyles",           "");
  string         prefix     = cl.getValue<string> ("prefix",               "");
  string         suffix     = cl.getValue<string> ("suffix",               "");
  string         opath      = cl.getValue<string> ("opath",                "");
  vector<string> formats    = cl.getVector<string>("formats",              "");
  bool           batch      = cl.getValue<bool>   ("batch",             false);


  if (!cl.check()) return 0;
  cl.print();

  if (prefix.empty()) prefix=algs[0];
  
  if (batch&&formats.size()==0) formats.push_back("pdf");

  if (!fillstyles.empty() && (colors.size()!=fillstyles.size())) {
    cout<<"Error: #Fillstyles has no corresponding colors!"<<endl;return 0;
  }
  
  argc = (batch) ? 2 : 1; if (batch) argv[1] = (char*)"-b";
  TApplication* app=new TApplication("jet_inspect_histos",&argc,argv);
  
  set_root_style();
    
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetOptTitle(0);//hh 11.04.2010
  
  vector<TCanvas*> c; int nx(1),ny(1);
  
  /// LOOP OVER FILES
  for (unsigned int ifile=0;ifile<inputs.size();ifile++) {

    string input(inputs[ifile]);
    TFile* file=new TFile(input.c_str(),"READ");
    if (!file->IsOpen()) {cout<<"Can't open "<<file->GetName()<<endl;return 0;}
    
    /// LOOP OVER ALGORITHMS
    for (unsigned int ialg=0;ialg<algs.size();ialg++) {

      string alg = algs[ialg];
      TDirectory* dir =(TDirectory*)file->Get(alg.c_str());
      if (0==dir) { cout<<"No dir "<<algs[ialg]<<" found"<<endl; return 0; }

      /// LOOP OVER VARIABLES
      for (unsigned int ivar=0;ivar<variables.size();ivar++) {
	
	string variable=variables[ivar];

	ObjectLoader<TH1F> hl;
	hl.load_objects(dir,variable);
	
	bool put_range = (npercanvas!=0||
			  (hl.nvariables()>0&&hl.nobjects(hl.nvariables()-1)==1));
	
	if (ifile==0&&ialg==0&&ivar==0) {
	  if (npercanvas==0)
	    npercanvas= (hl.nvariables()==0)? 1 : hl.nobjects(hl.nvariables()-1);
	  nx=(int)std::sqrt((float)npercanvas);
	  ny=nx;
	  if (nx*ny<npercanvas) nx++;
	  if (nx*ny<npercanvas) ny++;
	}
	
	hl.begin_loop();
	
	vector<unsigned int> indices; TH1F* h(0); unsigned int ihisto(0);
	unsigned int icolor(0);
	while ((h=hl.next_object(indices))) {

	  if (norm) {
	    TF1* f = h->GetFunction("fit");
	    if (0!=f) f->SetParameter(0,f->GetParameter(0)/h->Integral());
	    h->Sumw2();
	    h->Scale(1./h->Integral());
	  }
	  
	  if (ifile==0&&ialg==0&&ivar==0&&
	      (c.size()==0||ihisto%npercanvas==0)) {
	    stringstream sscname;sscname<<prefix<<"_"<<hl.quantity();
	    for (unsigned int i=0;i<hl.nvariables();i++) {
	      if (variables.size()>1&&hl.nobjects(i)==1) continue;
	      sscname<<"_"<<hl.variable(i);
	      if (i<hl.nvariables()-1||put_range)
		sscname<<hl.minimum(i,indices[i])<<"to"
		       <<hl.maximum(i,indices[i]);
	    }
	    if (!suffix.empty()) sscname<<"_"<<suffix;
	    c.push_back(new TCanvas(sscname.str().c_str(),
				    sscname.str().c_str(),
				    1000,1000));
	    c.back()->Divide(nx,ny,1e-04,1e-04);
	  }
	  
	  int icnv = ihisto/npercanvas;
	  int ipad = ihisto%npercanvas+1;
	  c[icnv]->cd(ipad);
	  
	  if (ifile==0&&ialg==0&&ivar==0) {
	    icolor=0;
	    if (logx) gPad->SetLogx();
	    if (logy) gPad->SetLogy();
	    gPad->SetLeftMargin(0.1);
	    gPad->SetRightMargin(0.05);
	    gPad->SetTopMargin(0.12);
	    gPad->SetBottomMargin(0.15);
	    set_xaxis_range(h);
	    if (ymin!=-1 || ymax!=-1) set_yaxis_range(h,ymin,ymax);
	    if (colors.empty()) h->SetLineColor(kBlack);
	    else h->SetLineColor(colors[icolor]);
	    set_draw_attributes(h,icolor,fill,colors,fillstyles);
	    h->Draw("EH");
	    h->SetMaximum(1.5*h->GetMaximum());
	    if (logy) h->SetMaximum(10.*h->GetMaximum());
	    if (colors.empty()) draw_stats(h,0.65,kBlack,kBlack);
	    else draw_stats(h,0.65,colors[icolor],colors[icolor]);
	    draw_range(hl,indices,(variables.size()==1));
	    draw_line_legend(mean,median,peak);
	  }
	  else {
	    icolor=ifile+ialg+ivar;
	    if (colors.empty()) h->SetLineColor(kBlue);
	    else if (icolor>colors.size()-1) {
	      cout<<"WARNING: #Histo Vs specified colors mismatch!"<<endl;
	      h->SetLineColor(kBlue);
	    }
	    else h->SetLineColor(colors[icolor]);
	    set_draw_attributes(h,icolor,fill,colors,fillstyles);
	    h->Draw("EHSAME");
	    if (colors.empty() || (icolor>colors.size()-1)) 
	      draw_stats(h,0.15,kBlue,kBlue);
	    else draw_stats(h,0.15,colors[icolor],colors[icolor]);
	    TH1F* hdraw = (TH1F*)gPad->GetListOfPrimitives()->First();
	    set_xaxis_range(hdraw,h);
	    if (ymin!=-1 || ymax!=-1) set_yaxis_range(hdraw,ymin,ymax);
	    if (h->GetMaximum()>hdraw->GetMaximum())
	      hdraw->SetMaximum(1.2*h->GetMaximum());
	  }
	  if (mean)   draw_line_mean(h);
	  if (median) draw_line_median(h);
	  if (peak)   draw_line_peak(h);

	  TF1* f = h->GetFunction("fit");
	  if (fullfit&&(0!=f)){
	    TF1* ff = (TF1*)f->Clone("ff");
	    ff->SetRange(h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
	    ff->SetLineColor(f->GetLineColor());
	    ff->SetLineStyle(kDashed);
	    ff->SetLineWidth(2);
	    ff->Draw("SAME");
	  }
	  
	  ihisto++;
	  
	} // histos
      } // variables
    } // algorithms
  } // inputs
  
  for (unsigned int ic=0;ic<c.size();ic++) {
    string output = c[ic]->GetName();
    if (!opath.empty()) output = opath + "/" + output;
    for (unsigned int ifmt=0;ifmt<formats.size();ifmt++) {
      c[ic]->Print((output+"."+formats[ifmt]).c_str());
    }
  }
  
  if (!batch) app->Run();
  
  return 0;
}


////////////////////////////////////////////////////////////////////////////////
// implement local functions
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
void set_xaxis_range(TH1* h1,TH1* h2)
{
  if (h1->GetEntries()==0) return;
  int binmin,binmax;
  get_xaxis_range(h1,binmin,binmax);
  if (0!=h2) {
    int binmin2,binmax2;
    get_xaxis_range(h2,binmin2,binmax2);
    binmin = std::min(binmin,binmin2);
    binmax = std::max(binmax,binmax2);
  }
  h1->GetXaxis()->SetRange(binmin,binmax);
}

//______________________________________________________________________________
void set_yaxis_range(TH1* h1,float ymin,float ymax)
{
  if (h1->GetEntries()==0) return;
  if (ymin!=-1) h1->SetMinimum(ymin);
  if (ymax!=-1) h1->SetMaximum(ymax);
}

//______________________________________________________________________________
void get_xaxis_range(TH1* h,int& binmin,int &binmax)
{
  binmin=-1; binmax=-1;
  for (int i=1;i<h->GetNbinsX();i++) {
    double bc = h->GetBinContent(i);
    if (bc>0) {
      if (binmin==-1) binmin=i;
      binmax=i;
    }
  }
}


//______________________________________________________________________________
void set_draw_attributes(TH1* h,unsigned index,bool fill,
			 const vector<int>& colors,
			 const vector<int>& fillstyles)
{
  if (fill) {
    Style_t fillstyle = (fillstyles.empty() || (index>fillstyles.size()-1)) ? 
      (3001+index) : fillstyles[index];
    h->SetFillColor(h->GetLineColor());
    h->SetFillStyle(fillstyle);
  }
  TF1* fitfnc = h->GetFunction("fit");
  if (0!=fitfnc) {
    fitfnc->SetLineWidth(2);
    fitfnc->SetLineColor(h->GetLineColor());
  }
}


//______________________________________________________________________________
void draw_stats(TH1* h,double xoffset,Color_t color,Color_t fitColor)
{
  TF1* fitfnc = h->GetFunction("fit");
  stringstream ssentries;
  ssentries<<setw(6) <<setiosflags(ios::left)<<"N:"
	   <<setw(10)<<resetiosflags(ios::left)<<setprecision(4)<<h->GetEntries();
  
  stringstream ssmean;
  ssmean<<setw(6)<<setiosflags(ios::left)<<"Mean:"
	<<setw(9)<<resetiosflags(ios::left)<<setprecision(4)<<h->GetMean();

  stringstream ssrms;
  ssrms<<setw(6)<<setiosflags(ios::left)<<"RMS:"
       <<setw(9)<<resetiosflags(ios::left)<<setprecision(4)<<h->GetRMS();

  stringstream sspeak;
  if (0!=fitfnc)
    sspeak<<setw(6)<<setiosflags(ios::left)<<"Peak: "
	  <<setw(10)<<resetiosflags(ios::left)<<setprecision(4)
	  <<fitfnc->GetParameter(1);

  stringstream sssgma;
  if (0!=fitfnc)
    sssgma<<setw(6)<<setiosflags(ios::left)<<"Sigma:"
	  <<setw(9)<<resetiosflags(ios::left)<<setprecision(4)
	  <<fitfnc->GetParameter(2);
  
  TLatex stats;
  stats.SetNDC(true);
  stats.SetTextAlign(12);
  stats.SetTextSize(0.045);
  stats.SetTextColor(color);
  stats.SetTextFont(42);
  
  stats.DrawLatex(xoffset,0.84,ssentries.str().c_str());
  stats.DrawLatex(xoffset,0.805,ssmean.str().c_str());
  stats.DrawLatex(xoffset,0.77,ssrms.str().c_str());
  if (0!=fitfnc) {
    stats.SetTextColor(fitColor);
    stats.DrawLatex(xoffset,0.735,sspeak.str().c_str());
    stats.DrawLatex(xoffset,0.70,sssgma.str().c_str());
  }
}


//______________________________________________________________________________
void draw_range(const ObjectLoader<TH1F>& hl,
		const vector<unsigned int>& indices,
		bool  addFixedVars)
{
  TLatex range;
  range.SetNDC(true);
  range.SetTextAlign(13);
  range.SetTextSize(0.055);
  range.SetTextFont(42);

  if (hl.nvariables()>2) range.SetTextSize(0.055-(hl.nvariables()*.0055));

  string varnameEta = "#eta";
  for (unsigned int i=0;i<hl.nvariables();i++)
    if (hl.variable(i)=="JetEta"&&hl.minimum(i,0)>=0) varnameEta="|#eta|";
  
  stringstream ssrange;

  for (unsigned int i=0;i<hl.nvariables();i++) {
    
    if (hl.nobjects(i)==1&&!addFixedVars) continue;
    
    string varname = hl.variable(i);
    string unit    = "";
    double varmin  = hl.minimum(i,indices[i]);
    double varmax  = hl.maximum(i,indices[i]);
    bool   threshold(false);
    
    if (varname=="RefPt")    { varname = "p_{T}^{REF}"; unit = " GeV"; }
    if (varname=="JetPt")    { varname = "p_{T}";       unit = " GeV"; }
    if (varname=="JetEta")   { varname = varnameEta;    unit =     ""; }
    if (varname=="JetPhi")   { varname = "#varphi";     unit =     ""; }
    if (varname=="PtRel")    { varname = "p_{T}^{rel}", unit = " GeV"; }
    if (varname=="RelLepPt") { varname = "p_{T}^{l}/p_{T}^{jet}",unit = ""; }
    if (varname=="ThreshPt") { varname = "p_{T}^{3rd}", unit = " GeV"; 
                               threshold = true; }

    if (threshold) ssrange<<varname<<" < "<<varmax<<unit<<"    ";
    else ssrange<<varmin<<" < "<<varname<<" < "<<varmax<<unit<<"    ";
  }
  
  range.DrawLatex(0.1,0.96,ssrange.str().c_str());
}


//______________________________________________________________________________
void draw_line_mean(TH1* h)
{
  double mean = h->GetMean();
  int binMean;
  for (binMean=1;binMean<h->GetNbinsX();binMean++) {
    float binLowEdge = h->GetBinLowEdge(binMean);
    float binHighEdge = binLowEdge + h->GetBinWidth(binMean);
    if (mean>=binLowEdge&&mean<binHighEdge) break;
  }
  double xmin = mean;
  double xmax = mean;
  double ymin = h->GetMinimum();
  double ymax = h->GetBinContent(binMean);
  TLine* lineMean = new TLine(xmin,ymin,xmax,ymax);
  lineMean->SetLineColor(h->GetLineColor());
  lineMean->SetLineStyle(2);
  lineMean->SetLineWidth(3);
  lineMean->Draw("SAME");
}


//______________________________________________________________________________
void draw_line_median(TH1* h)
{
  double median;
  double prb(0.5);
  h->GetQuantiles(1,&median,&prb);
  int binMedian;
  for (binMedian=1;binMedian<h->GetNbinsX();binMedian++) {
    double binLowEdge = h->GetBinLowEdge(binMedian);
    double binHighEdge = binLowEdge + h->GetBinWidth(binMedian);
    if (median>=binLowEdge&&median<binHighEdge) break;
  }
  double xmin = median;
  double xmax = median;
  double ymin = h->GetMinimum();
  double ymax = h->GetBinContent(binMedian);
  TLine* lineMedian = new TLine(xmin,ymin,xmax,ymax);
  lineMedian->SetLineColor(h->GetLineColor());
  lineMedian->SetLineStyle(3);
  lineMedian->SetLineWidth(3);
  lineMedian->Draw("SAME");
}


//______________________________________________________________________________
void draw_line_peak(TH1* h)
{
  TF1* fitFnc = h->GetFunction("fit"); if (0==fitFnc) return;
  double peak = fitFnc->GetParameter(1);
  double xmin = peak;
  double xmax = peak;
  double ymin = h->GetMinimum();
  double ymax = fitFnc->Eval(peak);
  TLine* linePeak = new TLine(xmin,ymin,xmax,ymax);
  linePeak->SetLineColor(h->GetLineColor());
  linePeak->SetLineStyle(4);
  linePeak->SetLineWidth(3);
  linePeak->Draw("SAME");
}


//______________________________________________________________________________
void draw_line_legend(bool mean,bool median,bool peak)
{
  if (!mean&&!median&&!peak) return;
  TLegend* leg = new TLegend(0.75,0.5,0.95,0.5-(mean+median+peak)*0.055);
  leg->SetLineColor(10);
  leg->SetFillColor(10);
  leg->SetBorderSize(0);
  if (mean) {
    TLine* lMean = new TLine();
    lMean->SetLineWidth(3);
    lMean->SetLineStyle(2);
    leg->AddEntry(lMean,"mean","l");
  }
  if (median) {
    TLine* lMedian = new TLine();
    lMedian->SetLineWidth(3);
    lMedian->SetLineStyle(3);
    leg->AddEntry(lMedian,"median","l");
  }
  if (peak) {
    TLine* lPeak = new TLine();
    lPeak->SetLineWidth(3);
    lPeak->SetLineStyle(4);
    leg->AddEntry(lPeak,"peak","l");
  }
  leg->Draw();
}

