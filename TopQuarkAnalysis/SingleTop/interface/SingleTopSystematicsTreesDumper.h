#ifndef __SINGLETOP_SYST_TREES_DUMPER_H__
#define __SINGLETOP_SYST_TREES_DUMPER_H__

/* \Class SingleTopSystematicsDumper
 *
 * \Authors A. Orso M. Iorio
 * 
 * Produces systematics histograms out of a standard Single Top n-tuple 
 * \ version $Id: SingleTopSystematicsTreesDumper.h,v 1.3 2011/05/09 02:20:50 oiorio Exp $
 */


//----------------- system include files
#include <memory>
#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>

//----------------- cmssw includes

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include <FWCore/Framework/interface/Run.h>

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNamesService.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "JetMETCorrections/JetVertexAssociation/interface/JetVertexMain.h"
#include "DataFormats/HepMCCandidate/interface/PdfInfo.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"




//--------------------TQAF includes
/*
#include "AnalysisDataFormats/TopObjects/interface/TopObject.h"
#include "AnalysisDataFormats/TopObjects/interface/TopLepton.h"
#include "AnalysisDataFormats/TopObjects/interface/TopJet.h"
#include "AnalysisDataFormats/TopObjects/interface/TopMET.h"
#include "AnalysisDataFormats/TopObjects/interface/TopElectron.h"
#include "AnalysisDataFormats/TopObjects/interface/TopMuon.h"
*/

//--------------------PAT includes
#include "DataFormats/PatCandidates/interface/Particle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"


//--------------------ROOT includes
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TLorentzVector.h"
#include "TH1.h"
#include "TH2.h"

//lorentzVector
#include "DataFormats/Math/interface/LorentzVector.h"

//B Tag reading from DB
#include "RecoBTag/Records/interface/BTagPerformanceRecord.h"
#include "CondFormats/PhysicsToolsObjects/interface/BinningPointByMap.h"
#include "RecoBTag/PerformanceDB/interface/BtagPerformance.h"

using namespace std;
using namespace edm;
using namespace reco;



class SingleTopSystematicsTreesDumper : public edm::EDAnalyzer {
 public:
  explicit SingleTopSystematicsTreesDumper(const edm::ParameterSet&);
  //  ~SingleTopSystematicsTreesDumper();
  
  
 private:
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();
  //void  EventInfo();

  math::PtEtaPhiELorentzVector top4Momentum(float leptonPx, float leptonPy, float leptonPz,float leptonE, float jetPx, float jetPy, float jetPz,float jetE, float metPx, float metPy);

  math::PtEtaPhiELorentzVector top4Momentum(math::PtEtaPhiELorentzVector lepton, math::PtEtaPhiELorentzVector jet, float metPx, float metPy);
  math::XYZTLorentzVector NuMomentum(float leptonPx, float leptonPy, float leptonPz, float leptonPt, float leptonE, float metPx, float metPy );

  
  float cosThetaLJ(math::PtEtaPhiELorentzVector lepton, math::PtEtaPhiELorentzVector jet, math::PtEtaPhiELorentzVector top);

  double BScaleFactor(string algo,string syst_name); 
  double MisTagScaleFactor(string algo,string syst_name,double sf, double eff, double sferr);
  double jetUncertainty(double eta, double ptCorr, int flavour);
  
  string rootFileName;

  std::vector<std::string> systematics,rate_systematics;

  edm::ParameterSet channelInfo;
  std::string channel;
  double crossSection,originalEvents,finalLumi,MTWCut,RelIsoCut; 
  edm::Event*   iEvent;
  
  double loosePtCut ;
  //  std::vector<float> leptonsPt,leptonsPhi,leptonsPz,leptonsEta,jetsPt,jetsPx,jetsPy,jetsPz,jetsEta,jetEnergy,jetsBTagAlgo,jetsAntiBTagAlgo,METPt,METPhi;
  
  //InputTags
  edm::InputTag leptonsPt_,
    leptonsPhi_,
    leptonsEta_,
    leptonsEnergy_,
    leptonsCharge_,
    leptonsRelIso_,
    leptonsID_,
    looseElectronsRelIso_,
    looseMuonsRelIso_,
    jetsPt_,
    jetsPhi_,
    jetsEta_,
    jetsEnergy_,
    jetsBTagAlgo_,
    jetsCorrTotal_,
    jetsAntiBTagAlgo_,
    METPt_,
    METPhi_,
    jetsFlavour_,
    UnclMETPx_,
    UnclMETPy_;

  // Handles
  edm::Handle<std::vector<float> > leptonsPt,
   leptonsPhi,
   leptonsEta,
   leptonsEnergy,
   leptonsCharge,
   leptonsRelIso,
   looseElectronsRelIso,
   looseMuonsRelIso,
   leptonsID,
   jetsEta,
   jetsPt,
   jetsPhi,
   jetsEnergy,
   jetsBTagAlgo,
   jetsAntiBTagAlgo,
   jetsFlavour,
   jetsCorrTotal,
   METPhi,
   METPt;

  edm::Handle< double > UnclMETPx,UnclMETPy;
  std::string leptonsFlavour_;  


  //Part for BTagging payloads
  edm::ESHandle<BtagPerformance> perfHP;
  edm::ESHandle<BtagPerformance> perfHE;

  //Part for JEC and JES
  string JEC_PATH;
  edm::FileInPath fip;
  JetCorrectionUncertainty *jecUnc;
  double JES_SW, JES_b_cut, JES_b_overCut;

  //Vectors definition  
  
  std::vector<math::PtEtaPhiELorentzVector> leptons,
    leptonsQCD,
    jets,
    loosejets,
    bjets,
    antibjets;
  
  //Base histograms 
  map<string, TTree*> trees;
  map<string, TTree*> treesWSample;

  map<string, TTree*> treesQCD;
  map<string, TTree*> treesWSampleQCD;
  
  double bTagThreshold;
  size_t bScanSteps;
  bool doBScan_,doQCD_;
  map<string, TTree*> treesScan[10];
  /*  map<string, TTree*> ForwardJetEta;
  map<string, TTree*> CosThetaLJ;
  map<string, TTree*> MTW;
  map<string, TTree*> TopMass;
  
  map<string, TTree*> CosThetaLJWSample;
  map<string, TTree*> ForwardJetEtaWSample;
  map<string, TTree*> MTWWSample;
  map<string, TTree*> TopMassWSample;

  //Part for Asymmetry

  map<string, TTree*> CosThetaLJPlus;
  map<string, TTree*> ForwardJetEtaPlus;
  map<string, TTree*> MTWPlus;
  map<string, TTree*> TopMassPlus;
  
  map<string, TTree*> CosThetaLJWSamplePlus;
  map<string, TTree*> ForwardJetEtaWSamplePlus;
  map<string, TTree*> MTWWSamplePlus;
  map<string, TTree*> TopMassWSamplePlus;

  map<string, TTree*> CosThetaLJMinus;
  map<string, TTree*> ForwardJetEtaMinus;
  map<string, TTree*> MTWMinus;
  map<string, TTree*> TopMassMinus;

  map<string, TTree*> CosThetaLJWSampleMinus;
  map<string, TTree*> ForwardJetEtaWSampleMinus;
  map<string, TTree*> MTWWSampleMinus;
  map<string, TTree*> TopMassWSampleMinus;
  */

  double etaTree,etaTree2,cosTree,topMassTree,weightTree,mtwMassTree,lowBTagTree,highBTagTree;
  int runTree, eventTree,lumiTree,chargeTree;

  
  double lepPt,lepPz,lepPhi,lepRelIso,fJetPhi,fJetPt,fJetPz,fJetE,bJetPt,bJetPz,bJetPhi,bJetE,metPt,metPhi,topPt,topPhi,topPz,topE,totalEnergy,totalMomentum;

  
 
};

#endif
