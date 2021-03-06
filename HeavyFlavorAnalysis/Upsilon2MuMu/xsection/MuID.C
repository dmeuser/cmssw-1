#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"

void MuIDCheckVsfMuIDmmb(){

  TFile *f = new TFile("101201.fl10.mm.ups1s.xsReader_1SBin.default.root");
  TH2D *MuIDCheck_Deno_1S;
  MuIDCheck_Deno_1S = (TH2D*)gFile->Get("MuIDCheck_Deno_1S");
  TH2D *MuIDCheck_Numa_1S;
  MuIDCheck_Numa_1S = (TH2D*)gFile->Get("MuIDCheck_Numa_1S");
  TH2D *Ratio = (TH2D*)MuIDCheck_Deno_1S->Clone(); 
  Ratio->SetName("MuIDCheckRatio");
  Ratio->Reset();
  TH2D *Diff = (TH2D*)MuIDCheck_Deno_1S->Clone();   
  Diff->SetName("MuIDCheck - fMuIDmmb");
  Diff->Reset();  
  
  double bin_contentBefore(0); double bin_contentAfter(0); double bin_ratio(0.);
  for (int i = 1; i <= MuIDCheck_Deno_1S->GetNbinsX(); ++i) {
    for (int j = 1; j <= MuIDCheck_Deno_1S->GetNbinsY(); ++j){	
      
      bin_contentBefore = MuIDCheck_Deno_1S->GetCellContent(i,j);
      bin_contentAfter = MuIDCheck_Numa_1S->GetCellContent(i,j);
      
      cout << "bin_contentDeno = " << bin_ contentBefore << endl;
      cout << "bin_contentNuma = " << bin_ contentAfter << endl;
      
      bin_ratio = bin_contentAfter / bin_contentBefore;
      cout << "bin_ratio = " << bin_ratio << endl;
      Ratio->SetCellContent(i,j,bin_ratio);
      bin_contentBefore=0;
      bin_contentAfter=0;
      bin_ratio=0;
    }
  }
  
  TFile *f1 = new TFile("MuID.root");
  TH2D *fMuIDEff;
  fMuIDEff = (TH2D*)gFile->Get("fMuIDEff");
  
  TH1D *Diff_1D = new TH1D("Diff_1D","Ratio:#rho^{TNP}", 50,0.9,1.1); 
  
  double muidcheck(-99); double muideff(-99); double diff(-99);
  for (int i = 1; i <= Ratio->GetNbinsX(); ++i) {
    for (int j = 1; j <= Ratio->GetNbinsY(); ++j){	
      
      muidcheck = Ratio->GetCellContent(i,j);
      muideff = fMuIDEff->GetCellContent(i,j);
      
      diff = muidcheck - muideff;
      cout << " "  << i << " " << j << endl;
      cout << "ratio = " << muidcheck/muideff << endl;
      cout << "diff = " << diff << endl;
      Diff->SetCellContent(i,j,muidcheck/muideff);
      Diff_1D->Fill(muidcheck/muideff);
      muidcheck=0;
      muideff=0;
      diff=0;
    }
  }
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  c1->Divide(3,1); 
  c1->cd(1);
  Ratio->Draw("colz");
  c1->cd(2);
  fMuIDEff->Draw("colz");
  c1->cd(3);
  Diff->Draw("colz");
  
  gStyle->SetOptStat(00000000000);
  TCanvas *c2 = new TCanvas("c2","c2",900,600);
  fMuIDEff->SetTitle("Di-muon Efficiency with Tag and Probe Method");
  fMuIDEff->GetYaxis()->SetTitle("p_{T}^{#Upsilon} [GeV/c]");
  fMuIDEff->GetXaxis()->SetTitle("|y^{#Upsilon}|");
  fMuIDEff->Draw("colz");
  c2->SaveAs("MuidTNP.pdf");
  
  TCanvas *c3 = new TCanvas("c3","c3",900,600);
  Ratio->SetTitle("Di-muon Efficiency with MC Simulation");
  Ratio->GetYaxis()->SetTitle("p_{T}^{#Upsilon} [GeV/c]");
  Ratio->GetXaxis()->SetTitle("|y^{#Upsilon}|");
  Ratio->Draw("colz"); 
  c3->SaveAs("MuidMCSim.pdf");
  
  TCanvas *c4 = new TCanvas("c4","c4",900,600);
  Diff->SetTitle("Ratio: #rho^{TNP}");
  Diff->GetYaxis()->SetTitle("p_{T}^{#Upsilon} [GeV/c]");
  Diff->GetXaxis()->SetTitle("|y^{#Upsilon}|");
  Diff->Draw("colz"); 
  c4->SaveAs("Muidrho.pdf");
  
  TCanvas *c5 = new TCanvas("c5","c5",900,600);
  Diff_1D->SetTitle("Ratio:#rho^{TNP}");
  Diff_1D->GetXaxis()->SetTitle("Ratio: #rho^{TNP}");
  gStyle->SetOptStat(1111111111);
  Diff_1D->Draw(); 
  c5->SaveAs("Muidrho_1D.pdf");  
  
  
}



void MuIDCheckVsfMuIDmct(){

  TFile *f = new TFile("101201.fl10.mm.ups1s.tnpMCtruth.xsReader.default.root");
  TH2D *MuIDCheck_Deno_1S;
  MuIDCheck_Deno_1S = (TH2D*)gFile->Get("MuIDCheck_Deno_1S");
  TH2D *MuIDCheck_Numa_1S;
  MuIDCheck_Numa_1S = (TH2D*)gFile->Get("MuIDCheck_Numa_1S");
  TH2D *Ratio = (TH2D*)MuIDCheck_Deno_1S->Clone(); 
  Ratio->SetName("MuIDCheckRatio");
  Ratio->Reset();
  TH2D *Diff = (TH2D*)MuIDCheck_Deno_1S->Clone();   
  Diff->SetName("MuIDCheck - fMuIDmmb");
  Diff->Reset();  
  
  double bin_contentBefore(0); double bin_contentAfter(0); double bin_ratio(0.);
  for (int i = 1; i <= MuIDCheck_Deno_1S->GetNbinsX(); ++i) {
    for (int j = 1; j <= MuIDCheck_Deno_1S->GetNbinsY(); ++j){	
      
      bin_contentBefore = MuIDCheck_Deno_1S->GetCellContent(i,j);
      bin_contentAfter = MuIDCheck_Numa_1S->GetCellContent(i,j);
      
      cout << "bin_contentDeno = " << bin_ contentBefore << endl;
      cout << "bin_contentNuma = " << bin_ contentAfter << endl;
      
      bin_ratio = bin_contentAfter / bin_contentBefore;
      cout << "bin_ratio = " << bin_ratio << endl;
      Ratio->SetCellContent(i,j,bin_ratio);
      bin_contentBefore=0;
      bin_contentAfter=0;
      bin_ratio=0;
    }
  }
  
  TFile *f1 = new TFile("MuID_MCtruth.root");
  TH2D *fMuIDEff;
  fMuIDEff = (TH2D*)gFile->Get("fMuIDEff");
  
  double muidcheck(-99); double muideff(-99); double diff(-99);
  for (int i = 1; i <= Ratio->GetNbinsX(); ++i) {
    for (int j = 1; j <= Ratio->GetNbinsY(); ++j){	
      
      muidcheck = Ratio->GetCellContent(i,j);
      muideff = fMuIDEff->GetCellContent(i,j);
      
      diff = muidcheck - muideff;
      
      cout << "diff = " << diff << endl;
      Diff->SetCellContent(i,j,diff);
      muidcheck=0;
      muideff=0;
      diff=0;
    }
  }
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  c1->Divide(3,1); 
  c1->cd(1);
  Ratio->Draw("colz");
  c1->cd(2);
  fMuIDEff->Draw("colz");
  c1->cd(3);
  Diff->Draw("colz");
}



void fMuIDmmbVsfMuIDmct(){
  
  TFile *f = new TFile("MuID_MC.root");
  TH2D *fMuIDEff;
  fMuIDEff = (TH2D*)gFile->Get("fMuIDEff");
  
  
  TFile *f1 = new TFile("MuID_MCtruth.root");
  TH2D *fMuIDEfftruth;
  fMuIDEfftruth = (TH2D*)gFile->Get("fMuIDEff");
  
  TH2D *Diff = (TH2D*)fMuIDEff->Clone();   
  Diff->SetName("fMuIDmmb - fMuIDmct");
  Diff->Reset();  
  
  double muidcheck(-99); double muideff(-99); double diff(-99);
  for (int i = 1; i <= fMuIDEff->GetNbinsX(); ++i) {
    for (int j = 1; j <= fMuIDEff->GetNbinsY(); ++j){	
      
      muidcheck = fMuIDEff->GetCellContent(i,j);
      muideff = fMuIDEfftruth->GetCellContent(i,j);
      
      diff = muidcheck - muideff;
      
      cout << "diff = " << diff << endl;
      Diff->SetCellContent(i,j,diff);
      muidcheck=0;
      muideff=0;
      diff=0;
    }
  }
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  c1->Divide(3,1); 
  c1->cd(1);
  fMuIDEff->Draw("colz");
  c1->cd(2);
  fMuIDEfftruth->Draw("colz");
  c1->cd(3);
  Diff->Draw("colz");
}


void MuIDCheckVsfMuID_DATA(){

  TFile *f = new TFile("101201.fl10.mm.ups1s.tnpMCtruth.xsReader.default.root");
  TH2D *MuIDCheck_Deno_1S;
  MuIDCheck_Deno_1S = (TH2D*)gFile->Get("MuIDCheck_Deno_1S");
  TH2D *MuIDCheck_Numa_1S;
  MuIDCheck_Numa_1S = (TH2D*)gFile->Get("MuIDCheck_Numa_1S");
  TH2D *Ratio = (TH2D*)MuIDCheck_Deno_1S->Clone(); 
  Ratio->SetName("MuIDCheckRatio");
  Ratio->Reset();
  TH2D *Diff = (TH2D*)MuIDCheck_Deno_1S->Clone();   
  Diff->SetName("MuIDCheck - fMuIDmmb");
  Diff->Reset();  
  
  double bin_contentBefore(0); double bin_contentAfter(0); double bin_ratio(0.);
  for (int i = 1; i <= MuIDCheck_Deno_1S->GetNbinsX(); ++i) {
    for (int j = 1; j <= MuIDCheck_Deno_1S->GetNbinsY(); ++j){	
      
      bin_contentBefore = MuIDCheck_Deno_1S->GetCellContent(i,j);
      bin_contentAfter = MuIDCheck_Numa_1S->GetCellContent(i,j);
      
      cout << "bin_contentDeno = " << bin_ contentBefore << endl;
      cout << "bin_contentNuma = " << bin_ contentAfter << endl;
      
      bin_ratio = bin_contentAfter / bin_contentBefore;
      cout << "bin_ratio = " << bin_ratio << endl;
      Ratio->SetCellContent(i,j,bin_ratio);
      bin_contentBefore=0;
      bin_contentAfter=0;
      bin_ratio=0;
    }
  }
  
  TFile *f1 = new TFile("MuID_DATA.root");
  TH2D *fMuIDEff;
  fMuIDEff = (TH2D*)gFile->Get("fMuIDEff");
  
  double muidcheck(-99); double muideff(-99); double diff(-99);
  for (int i = 1; i <= Ratio->GetNbinsX(); ++i) {
    for (int j = 1; j <= Ratio->GetNbinsY(); ++j){	
      
      muidcheck = Ratio->GetCellContent(i,j);
      muideff = fMuIDEff->GetCellContent(i,j);
      
      diff = muidcheck - muideff;
      
      cout << "diff = " << diff << endl;
      Diff->SetCellContent(i,j,diff);
      muidcheck=0;
      muideff=0;
      diff=0;
    }
  }
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  c1->Divide(3,1); 
  c1->cd(1);
  Ratio->Draw("colz");
  c1->cd(2);
  fMuIDEff->Draw("colz");
  c1->cd(3);
  Diff->Draw("colz");
}


void UG_Acceptance(){

  TFile *f = new TFile("Acc.xsReader_1S.default.root");
  TH2D *UG_AllGenRes_1S;
  UG_AllGenRes_1S = (TH2D*)gFile->Get("UG_AllGenRes_1S");
  TH2D *UG_RecoGenRes_1S;
  UG_RecoGenRes_1S = (TH2D*)gFile->Get("UG_RecoGenRes_1S");
  TH2D *Ratio = (TH2D*)UG_RecoGenRes_1S->Clone(); 
  Ratio->SetName("Acceptance");
  Ratio->Reset();
    
  double bin_contentBefore(0); double bin_contentAfter(0); double bin_ratio(0.);
  for (int i = 1; i <= UG_RecoGenRes_1S->GetNbinsX(); ++i) {
    for (int j = 1; j <= UG_RecoGenRes_1S->GetNbinsY(); ++j){	
      
      bin_contentBefore = UG_AllGenRes_1S->GetCellContent(i,j);
      bin_contentAfter = UG_RecoGenRes_1S->GetCellContent(i,j);
      
      cout << "bin_contentAllGenRes = " << bin_ contentBefore << endl;
      cout << "bin_contentRecoGenRes = " << bin_ contentAfter << endl;
      
      bin_ratio = bin_contentAfter / bin_contentBefore;
      cout << "bin_ratio = " << bin_ratio << endl;
      Ratio->SetCellContent(i,j,bin_ratio);
      bin_contentBefore=0;
      bin_contentAfter=0;
      bin_ratio=0;
    }
  }
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  //c1->Divide(3,1); 
  c1->cd(1);
  //UG_AllGenRes_1S->Draw("colz");
  //c1->cd(2);
  //UG_RecoGenRes_1S->Draw("colz");
  //c1->cd(3);
  Ratio->Draw("colz");

}
