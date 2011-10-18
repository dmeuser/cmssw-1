
#include "L1Trigger/CSCTrackFinder/test/src/TrackHistogramList.h"
#include "L1Trigger/CSCTrackFinder/test/src/EffHistogramList.h"
namespace csctf_analysis
{
 TrackHistogramList::TrackHistogramList(const std::string dirname, const edm::ParameterSet* parameters)
  {
	double maxpt=parameters->getUntrackedParameter<double>("MaxPtHist");
	double minpt=parameters->getUntrackedParameter<double>("MinPtHist");
	int ptbins=parameters->getUntrackedParameter<double>("BinsPtHist");
	ptStep=(maxpt-minpt)/(1.0*ptbins);
	
	TFileDirectory dir = fs->mkdir(dirname);
	TFileDirectory allSubdir = dir.mkdir("All_Tracks");
	TFileDirectory allSubdirGhost = allSubdir.mkdir("Ghost_Tracks");
	
	TFileDirectory matchSubdir = dir.mkdir("Match_Tracks");
	TFileDirectory matchSubdirPt = matchSubdir.mkdir("Pt");
	TFileDirectory matchSubdirPhi = matchSubdir.mkdir("Phi");
	
	TFileDirectory matchSubdirOverall = matchSubdirPt.mkdir("Overall");
	TFileDirectory matchSubdirCSCOnly = matchSubdirPt.mkdir("CSCOnly");
	TFileDirectory matchSubdirCSCRestricted = matchSubdirPt.mkdir("CSCRestricted");
	TFileDirectory matchSubdirDTOnly = matchSubdirPt.mkdir("DTOnly");
	TFileDirectory matchSubdirOverlap = matchSubdirPt.mkdir("Overlap");
	
	//
	SimPt_vs_TFPt_FWD = allSubdir.make<TH2F>("SimPt_vs_TFPt_FWD","Forward Region: Sim Pt vs TF Pt",100,0,100,100,0,100);	
	matchedRefPt_FWD = matchSubdirPt.make<TH1F>("matchedRefPt_FWD","Forward Region: Pt of Matched Sim Tracks",500,0,100);
	SimPt_vs_TFPt_DT = allSubdir.make<TH2F>("SimPt_vs_TFPt_DT","DT Region: Sim Pt vs TF Pt",100,0,100,100,0,100);	
	matchedRefPt_DT = matchSubdirPt.make<TH1F>("matchedRefPt_DT","DT Region: Pt of Matched Sim Tracks",500,0,100);
	FR = allSubdir.make<TH1F>("FR","Track Finder FR Bit",2,-0.5,1.5);
	modeOcc = allSubdir.make<TH1F>("modeOcc","Mode Occupancy",16,-0.5,15.5);
	modeOccDT = allSubdir.make<TH1F>("modeOccDT","Mode Occupancy 0 <= #eta <= 0.9",16,-0.5,15.5);
	modeOccCSCOnly = allSubdir.make<TH1F>("modeOccCSCOnly","Mode Occupancy 1.2<= #eta <=2.4",16,-0.5,15.5);
	modeOccOverlap = allSubdir.make<TH1F>("modeOccOverlap","Mode Occupancy 0.9<= #eta <=1.2",16,-0.5,15.5);
	Eta = allSubdir.make<TH1F>("Eta"," Track #eta",50,0, 2.5); 
	signedEta = allSubdir.make<TH1F>("signedEta"," Track #eta",100,-2.5, 2.5); 
	Phi = allSubdir.make<TH1F>("Phi"," Track #phi",144,0,6.283);
	Phi_mod_10_endcap1 = allSubdir.make<TH1F>("Phi_mod_10_endcap1","Track #phi_mod_10_endcap1",140,-2,12);
	Phi_mod_10_endcap2 = allSubdir.make<TH1F>("Phi_mod_10_endcap2","Track #phi_mod_10_endcap2",140,-2,12);
	Pt = allSubdir.make<TH1F>("Pt"," Track Transverse Momentum",ptbins,minpt,maxpt);
	Pz = allSubdir.make<TH1F>("Pz", " Track Z Momentum", ptbins,minpt,maxpt);
	P =  allSubdir.make<TH1F>("P", " Track Total Momentum",ptbins,minpt,maxpt);
	Radius = allSubdir.make<TH1F>("Radius","Angle between tf and sim track", 30, 0, 0.6);
	Quality = allSubdir.make<TH1F>("Quality", " Track Quality",9,-0.5,8.5);
	matchMode= matchSubdir.make<TH1F>("matchMode","Matched Track Mode",16,-0.5,15.5);
	matchPhi = matchSubdirPhi.make<TH1F>("matchPhi","Matched Track #phi", 144, 0, 6.283);
	matchPhi_mod_10_Q2_endcap1 = matchSubdirPhi.make<TH1F>("matchPhi_mod_10_Q2_endcap1","Matched Track #phi_mod_10_endcap1 Qual>=2",140,-2,12);
	matchPhi_mod_10_Q3_endcap1 = matchSubdirPhi.make<TH1F>("matchPhi_mod_10_Q3_endcap1","Matched Track #phi_mod_10_endcap1 Qual>=3",140,-2,12);
	matchPhi_mod_10_Q2_endcap2 = matchSubdirPhi.make<TH1F>("matchPhi_mod_10_Q2_endcap2","Matched Track #phi_mod_10_endcap2 Qual>=2",140,-2,12);
	matchPhi_mod_10_Q3_endcap2 = matchSubdirPhi.make<TH1F>("matchPhi_mod_10_Q3_endcap2","Matched Track #phi_mod_10_endcap2 Qual>=3",140,-2,12);
	matchEta = matchSubdir.make<TH1F>("matchEta","Matched Track #eta",50,0, 2.5);
	signedMatchEta = matchSubdir.make<TH1F>("signedMatchEta","Matched Track #eta",100,-2.5, 2.5);
	//matchPt = dir.make<TH1F>("matchPt","Matched Track Pt",ptbins,minpt,maxpt);
	matchPtOverall = matchSubdirOverall.make<TH1F>("matchPtOverall","Matched Track Overall Pt",ptbins,minpt,maxpt);
	matchPtCSCOnly = matchSubdirCSCOnly.make<TH1F>("matchPtCSCOnly","Matched Track Pt; 1.2<= #eta <=2.4",ptbins,minpt,maxpt);
	matchPtCSCRestricted = matchSubdirCSCRestricted.make<TH1F>("matchPtCSCRestricted","Matched Track Pt; 1.2<= #eta <=2.1",ptbins,minpt,maxpt);
	matchPtDTOnly = matchSubdirDTOnly.make<TH1F>("matchPtDTOnly","Matched Track Pt; 0.0<= #eta <=0.9",ptbins,minpt,maxpt);
	matchPtOverlap = matchSubdirOverlap.make<TH1F>("matchPtOverlap","Matched Track Pt; 0.9<= #eta <=1.2",ptbins,minpt,maxpt);
	matchRadius = matchSubdir.make<TH1F>("matchRadius","Matched Angle between tf and sim track", 30, 0, 0.6);
	matchTFPt10Overall = matchSubdirOverall.make<TH1F>("matchTFPt10Overall","Matched Track Pt Tf > 10",ptbins,minpt,maxpt); 
	matchTFPt12Overall = matchSubdirOverall.make<TH1F>("matchTFPt12Overall","Matched Track Pt Tf > 12",ptbins,minpt,maxpt); 
	matchTFPt16Overall = matchSubdirOverall.make<TH1F>("matchTFPt16Overall","Matched Track Pt Tf > 16",ptbins,minpt,maxpt); 
	matchTFPt20Overall = matchSubdirOverall.make<TH1F>("matchTFPt20Overall","Matched Track Pt Tf > 20",ptbins,minpt,maxpt);
	matchTFPt40Overall = matchSubdirOverall.make<TH1F>("matchTFPt40Overall","Matched Track Pt Tf > 40",ptbins,minpt,maxpt);
	matchTFPt60Overall = matchSubdirOverall.make<TH1F>("matchTFPt60Overall","Matched Track Pt Tf > 60",ptbins,minpt,maxpt);
	matchTFPt10CSCOnly = matchSubdirCSCOnly.make<TH1F>("matchPt10CSCOnly","Matched Track Pt Tf > 10; 1.2<= #eta <=2.4",ptbins,minpt,maxpt); 
	matchTFPt12CSCOnly = matchSubdirCSCOnly.make<TH1F>("matchPt12CSCOnly","Matched Track Pt Tf > 12; 1.2<= #eta <=2.4",ptbins,minpt,maxpt);
	matchTFPt16CSCOnly = matchSubdirCSCOnly.make<TH1F>("matchPt16CSCOnly","Matched Track Pt Tf > 16; 1.2<= #eta <=2.4",ptbins,minpt,maxpt);
	matchTFPt20CSCOnly = matchSubdirCSCOnly.make<TH1F>("matchPt20CSCOnly","Matched Track Pt Tf > 20; 1.2<= #eta <=2.4",ptbins,minpt,maxpt);
	matchTFPt40CSCOnly = matchSubdirCSCOnly.make<TH1F>("matchPt40CSCOnly","Matched Track Pt Tf > 40; 1.2<= #eta <=2.4",ptbins,minpt,maxpt);
	matchTFPt60CSCOnly = matchSubdirCSCOnly.make<TH1F>("matchPt60CSCOnly","Matched Track Pt Tf > 60; 1.2<= #eta <=2.4",ptbins,minpt,maxpt);
	matchTFPt10CSCRestricted = matchSubdirCSCRestricted.make<TH1F>("matchPt10CSCRestricted","Matched Track Pt Tf > 10; 1.2<= #eta <=2.1",ptbins,minpt,maxpt);
	matchTFPt12CSCRestricted = matchSubdirCSCRestricted.make<TH1F>("matchPt12CSCRestricted","Matched Track Pt Tf > 12; 1.2<= #eta <=2.1",ptbins,minpt,maxpt);
	matchTFPt16CSCRestricted = matchSubdirCSCRestricted.make<TH1F>("matchPt16CSCRestricted","Matched Track Pt Tf > 16; 1.2<= #eta <=2.1",ptbins,minpt,maxpt);
	matchTFPt20CSCRestricted = matchSubdirCSCRestricted.make<TH1F>("matchPt20CSCRestricted","Matched Track Pt Tf > 20; 1.2<= #eta <=2.1",ptbins,minpt,maxpt);
	matchTFPt40CSCRestricted = matchSubdirCSCRestricted.make<TH1F>("matchPt40CSCRestricted","Matched Track Pt Tf > 40; 1.2<= #eta <=2.1",ptbins,minpt,maxpt);
	matchTFPt60CSCRestricted = matchSubdirCSCRestricted.make<TH1F>("matchPt60CSCRestricted","Matched Track Pt Tf > 60; 1.2<= #eta <=2.1",ptbins,minpt,maxpt);
	matchTFPt10Overlap = matchSubdirOverlap.make<TH1F>("matchPt10Overlap","Matched Track Pt Tf > 10; 0.9<= #eta <=1.2",ptbins,minpt,maxpt); 
	matchTFPt12Overlap = matchSubdirOverlap.make<TH1F>("matchPt12Overlap","Matched Track Pt Tf > 12; 0.9<= #eta <=1.2",ptbins,minpt,maxpt);
	matchTFPt16Overlap = matchSubdirOverlap.make<TH1F>("matchPt16Overlap","Matched Track Pt Tf > 16; 0.9<= #eta <=1.2",ptbins,minpt,maxpt);
	matchTFPt20Overlap = matchSubdirOverlap.make<TH1F>("matchPt20Overlap","Matched Track Pt Tf > 20; 0.9<= #eta <=1.2",ptbins,minpt,maxpt);
	matchTFPt40Overlap = matchSubdirOverlap.make<TH1F>("matchPt40Overlap","Matched Track Pt Tf > 40; 0.9<= #eta <=1.2",ptbins,minpt,maxpt);
	matchTFPt60Overlap = matchSubdirOverlap.make<TH1F>("matchPt60Overlap","Matched Track Pt Tf > 60; 0.9<= #eta <=1.2",ptbins,minpt,maxpt);
	matchTFPt10DTOnly = matchSubdirDTOnly.make<TH1F>("matchPt10DTOnly","Matched Track Pt Tf > 10; 0.0<= #eta <=0.9",ptbins,minpt,maxpt);
	matchTFPt12DTOnly = matchSubdirDTOnly.make<TH1F>("matchPt12DTOnly","Matched Track Pt Tf > 12; 0.0<= #eta <=0.9",ptbins,minpt,maxpt);
	matchTFPt16DTOnly = matchSubdirDTOnly.make<TH1F>("matchPt16DTOnly","Matched Track Pt Tf > 16; 0.0<= #eta <=0.9",ptbins,minpt,maxpt); 
	matchTFPt20DTOnly = matchSubdirDTOnly.make<TH1F>("matchPt20DTOnly","Matched Track Pt Tf > 20; 0.0<= #eta <=0.9",ptbins,minpt,maxpt);
	matchTFPt40DTOnly = matchSubdirDTOnly.make<TH1F>("matchPt40DTOnly","Matched Track Pt Tf > 40; 0.0<= #eta <=0.9",ptbins,minpt,maxpt);
	matchTFPt60DTOnly = matchSubdirDTOnly.make<TH1F>("matchPt60DTOnly","Matched Track Pt Tf > 60; 0.0<= #eta <=0.9",ptbins,minpt,maxpt);
	//fidPtDen = dir.make<TH1F>("fidPtDen", "Fiducial Sim Track Pt", ptbins,minpt,maxpt);
	ptDenOverall = allSubdir.make<TH1F>("ptDenOverall", "Overall Sim Track Pt", ptbins,minpt,maxpt);	
	ptDenCSCOnly = allSubdir.make<TH1F>("ptDenCSCOnly", "High Eta Sim Track Pt", ptbins,minpt,maxpt);
	ptDenCSCRestricted = allSubdir.make<TH1F>("ptDenCSCRestricted", "1.2<= #eta <=2.1 Sim Track Pt", ptbins,minpt,maxpt);	
	ptDenOverlap = allSubdir.make<TH1F>("ptDenOverlap", "Overlap Sim Track Pt", ptbins,minpt,maxpt);	
	ptDenDTOnly = allSubdir.make<TH1F>("ptDenDTOnly", "DT Sim Track Pt", ptbins,minpt,maxpt);
	EtaQ3 = allSubdir.make<TH1F>("EtaQ3"," Track #eta, Quality >= 3 Tracks",50,0,2.5);
	EtaQ2 = allSubdir.make<TH1F>("EtaQ2"," Track #eta, Quality >= 2 Tracks",50,0,2.5);
	EtaQ1 = allSubdir.make<TH1F>("EtaQ1"," Track #eta, Quality >= 1 Tracks",50,0,2.5);
	signedEtaQ3 = allSubdir.make<TH1F>("signedEtaQ3"," Track #eta, Quality >= 3 Tracks",100,-2.5,2.5);
	signedEtaQ2 = allSubdir.make<TH1F>("signedEtaQ2"," Track #eta, Quality >= 2 Tracks",100,-2.5,2.5);
	signedEtaQ1 = allSubdir.make<TH1F>("signedEtaQ1"," Track #eta, Quality >= 1 Tracks",100,-2.5,2.5);
	PhiQ3 = allSubdir.make<TH1F>("PhiQ3"," Track #phi, Quality >= 3 Tracks",144,0,6.283);
	PhiQ2 = allSubdir.make<TH1F>("PhiQ2"," Track #phi, Quality >= 2 Tracks",144,0,6.283);
	PhiQ1 = allSubdir.make<TH1F>("PhiQ1"," Track #phi, Quality >= 1 Tracks",144,0,6.283);
	PtQ3 = allSubdir.make<TH1F>("PtQ3"," Track Pt, Quality >= 3 Tracks",ptbins,minpt,maxpt);
	PtQ2 = allSubdir.make<TH1F>("PtQ2"," Track Pt, Quality >= 2 Tracks",ptbins,minpt,maxpt);
	PtQ1 = allSubdir.make<TH1F>("PtQ1"," Track Pt, Quality >= 1 Tracks",ptbins,minpt,maxpt);
	ghostEtaQ3 = allSubdirGhost.make<TH1F>("GhostEtaQ3"," Track #eta, Quality >= 3 Tracks",50,0,2.5);
	ghostEtaQ2 = allSubdirGhost.make<TH1F>("GhostEtaQ2"," Track #eta, Quality >= 2 Tracks",50,0,2.5);
	ghostEtaQ1 = allSubdirGhost.make<TH1F>("GhostEtaQ1"," Track #eta, Quality >= 1 Tracks",50,0,2.5);
	ghostSignedEtaQ3 = allSubdirGhost.make<TH1F>("signedGhostEtaQ3"," Track #eta, Quality >= 3 Tracks",100,-2.5,2.5);
	ghostSignedEtaQ2 = allSubdirGhost.make<TH1F>("signedGhostEtaQ2"," Track #eta, Quality >= 2 Tracks",100,-2.5,2.5);
	ghostSignedEtaQ1 = allSubdirGhost.make<TH1F>("signedGhostEtaQ1"," Track #eta, Quality >= 1 Tracks",100,-2.5,2.5);
	ghostPhiQ3 = allSubdirGhost.make<TH1F>("GhostPhiQ3"," Track #phi, Quality >= 3 Tracks",144,0,6.283);
	ghostPhiQ2 = allSubdirGhost.make<TH1F>("GhostPhiQ2"," Track #phi, Quality >= 2 Tracks",144,0,6.283);
	ghostPhiQ1 = allSubdirGhost.make<TH1F>("GhostPhiQ1"," Track #phi, Quality >= 1 Tracks",144,0,6.283);
	ghostPtQ3 = allSubdirGhost.make<TH1F>("GhostPhiQ3"," Track Pt, Quality >= 3 Tracks",ptbins,minpt,maxpt);
	ghostPtQ2 = allSubdirGhost.make<TH1F>("GhostPhiQ2"," Track Pt, Quality >= 2 Tracks",ptbins,minpt,maxpt);
	ghostPtQ1 = allSubdirGhost.make<TH1F>("GhostPhiQ1"," Track Pt, Quality >= 1 Tracks",ptbins,minpt,maxpt);
	ghostEta = allSubdirGhost.make<TH1F>("GhostEta","Sim #eta Value for Fake Tracks",50, 0, 2.5);
	ghostSignedEta = allSubdirGhost.make<TH1F>("GhostSignedEta","Sim #eta Value for Fake Tracks",100, -2.5, 2.5);
	ghostPhi = allSubdirGhost.make<TH1F>("GhostPhi","Sim #phi Value for Fake Tracks",144, 0, 6.283);
	ghostPt = allSubdirGhost.make<TH1F>("GhostPt", "Sim Pt Value for Fake Tracks",ptbins,minpt,maxpt);
	ghostRadius = allSubdirGhost.make<TH1F>("GhostRadius", "Radius Value for Fake Tracks", 30, 0, 0.6);
	rateHist = allSubdir.make<TH1F>("rateHist", "Number Passing vs TF Pt Threshold",ptbins, minpt, maxpt);
	
	
	// Setting Axis Titles!
	
	SimPt_vs_TFPt_FWD->GetXaxis()->SetTitle("Sim Pt");
	SimPt_vs_TFPt_FWD->GetYaxis()->SetTitle("TF Pt");
	SimPt_vs_TFPt_DT->GetXaxis()->SetTitle("Sim Pt");
	SimPt_vs_TFPt_DT->GetYaxis()->SetTitle("TF Pt");
	Eta->GetXaxis()->SetTitle("#eta");
	Eta->GetYaxis()->SetTitle("Counts");
	Phi->GetXaxis()->SetTitle("#phi");
	Phi->GetYaxis()->SetTitle("Counts");
	Pt->GetXaxis()->SetTitle("#pt");
	Pt->GetYaxis()->SetTitle("Counts");
	Quality->GetXaxis()->SetTitle("Track Quality");
	Quality->GetYaxis()->SetTitle("Counts");
	FR->GetXaxis()->SetTitle("FR Bit Value");
	FR->GetYaxis()->SetTitle("Counts");
	modeOcc->GetXaxis()->SetTitle("Mode");
	modeOcc->GetYaxis()->SetTitle("Counts");
	matchMode->GetXaxis()->SetTitle("Mode");
	matchMode->GetYaxis()->SetTitle("Counts");
	modeOccDT->GetXaxis()->SetTitle("Mode");
	modeOccDT->GetYaxis()->SetTitle("Counts");
	modeOccCSCOnly->GetXaxis()->SetTitle("Mode");
	modeOccCSCOnly->GetYaxis()->SetTitle("Counts");
	modeOccOverlap->GetXaxis()->SetTitle("Mode");
	modeOccOverlap->GetYaxis()->SetTitle("Counts");
	EtaQ3->GetXaxis()->SetTitle("#eta");
	EtaQ3->GetYaxis()->SetTitle("Counts");
	EtaQ2->GetXaxis()->SetTitle("#eta");
	EtaQ2->GetYaxis()->SetTitle("Counts");
	EtaQ1->GetXaxis()->SetTitle("#eta");
	EtaQ1->GetYaxis()->SetTitle("Counts");
	PhiQ3->GetXaxis()->SetTitle("#phi");
	PhiQ3->GetYaxis()->SetTitle("Counts");
	PhiQ2->GetXaxis()->SetTitle("#phi");
	PhiQ2->GetYaxis()->SetTitle("Counts");
	PhiQ1->GetXaxis()->SetTitle("#phi");
	PhiQ1->GetYaxis()->SetTitle("Counts");
	PtQ3->GetXaxis()->SetTitle("#pt");
	PtQ3->GetYaxis()->SetTitle("Counts");
	PtQ2->GetXaxis()->SetTitle("#pt");
	PtQ2->GetYaxis()->SetTitle("Counts");
	PtQ1->GetXaxis()->SetTitle("#pt");
	PtQ1->GetYaxis()->SetTitle("Counts");
	ghostEta->GetXaxis()->SetTitle("#eta");
	ghostEta->GetYaxis()->SetTitle("Counts");
	ghostPhi->GetXaxis()->SetTitle("#phi");
	ghostPhi->GetYaxis()->SetTitle("Counts");
	ghostPt->GetXaxis()->SetTitle("#pt");
	ghostPt->GetYaxis()->SetTitle("Counts");
	//ghostQuality->GetXaxis()->SetTitle("Track Quality");
	//ghostQuality->GetYaxis()->SetTitle("Counts");
	ghostEtaQ3->GetXaxis()->SetTitle("#eta");
	ghostEtaQ3->GetYaxis()->SetTitle("Counts");
	ghostEtaQ2->GetXaxis()->SetTitle("#eta");
	ghostEtaQ2->GetYaxis()->SetTitle("Counts");
	ghostEtaQ1->GetXaxis()->SetTitle("#eta");
	ghostEtaQ1->GetYaxis()->SetTitle("Counts");
	ghostPhiQ3->GetXaxis()->SetTitle("#phi");
	ghostPhiQ3->GetYaxis()->SetTitle("Counts");
	ghostPhiQ2->GetXaxis()->SetTitle("#phi");
	ghostPhiQ2->GetYaxis()->SetTitle("Counts");
	ghostPhiQ1->GetXaxis()->SetTitle("#phi");
	ghostPhiQ1->GetYaxis()->SetTitle("Counts");
	ghostPtQ3->GetXaxis()->SetTitle("#pt");
	ghostPtQ3->GetYaxis()->SetTitle("Counts");
	ghostPtQ2->GetXaxis()->SetTitle("#pt");
	ghostPtQ2->GetYaxis()->SetTitle("Counts");
	ghostPtQ1->GetXaxis()->SetTitle("#pt");
	ghostPtQ1->GetYaxis()->SetTitle("Counts");
	rateHist->GetXaxis()->SetTitle("Tf Pt Threshold (GeV)");
	rateHist->GetYaxis()->SetTitle("Number Passed");

	
  }
}
