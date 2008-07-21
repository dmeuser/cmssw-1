// -*- C++ -*-
//
// Package:    ZTauTauAnalysis
// Class:      ZTauTauAnalysis
// 
/**\class ZTauTauAnalysis ZTauTauAnalysis.cc UserCode/ZTauTauAnalysis/src/ZTauTauAnalysis.cc
   
Description: <one line class summary>


Implementation:
<Notes on implementation>
*/
//
// Original Author:  Letizia Lusito, Nicola De Filippis
//         Created:  Mon Apr  2 18:52:55 CEST 2007
// $Id$
//
//
////////

// system include files
#include <memory>
#include <iostream>
#include <typeinfo>
#include <iomanip>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"


#include "TLorentzVector.h"
#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Common/interface/RefVector.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Common/interface/EDProduct.h"

#include "Math/GenVector/BitReproducible.h"


#include "DetectorDescription/Core/interface/DDCompactView.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"

#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertex.h"
#include "SimDataFormats/EncodedEventId/interface/EncodedEventId.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"

#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

#include "MagneticField/Engine/interface/MagneticField.h" 
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h" 

#include "TrackingTools/GeomPropagators/interface/HelixExtrapolatorToLine2Order.h"

//MUON
#include "DataFormats/MuonReco/interface/Muon.h"
#include "RecoMuon/GlobalMuonProducer/src/GlobalMuonProducer.h"
#include "RecoMuon/MuonIsolation/interface/MuIsoBaseAlgorithm.h"
#include "RecoMuon/MuonIsolation/interface/MuIsoByTrackPt.h"
#include "RecoMuon/MuonIsolation/interface/MuIsoByTrackPt.h"
#include "RecoMuon/MuonIsolation/interface/MuIsoExtractor.h"
#include "RecoMuon/MuonIsolation/interface/MuIsoExtractorFactory.h"
#include "RecoMuon/MuonIsolation/interface/IsolatorByDeposit.h"
#include "DataFormats/MuonReco/interface/MuonIsolation.h" 
#include "RecoMuon/MuonIsolation/interface/MuIsoByTrackPt.h"
#include "DataFormats/MuonReco/interface/MuIsoDeposit.h"

//MET
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METCollection.h"

//TAU

#include "DataFormats/TauReco/interface/Tau.h"
#include "DataFormats/BTauReco/interface/PFIsolatedTauTagInfo.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "RecoTauTag/PFConeIsolation/interface/PFConeIsolationAlgorithm.h"
#include "RecoTauTag/PFConeIsolation/interface/PFConeIsolation.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminatorByIsolation.h"
#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"
#include "DataFormats/TauReco/interface/PFTau.h"


#include "DataFormats/JetReco/interface/CaloJet.h" 
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"


#include "FWCore/MessageLogger/interface/MessageLogger.h"


#include "DataFormats/TrackReco/interface/Track.h"
#include "RecoCaloTools/Selectors/interface/CaloConeSelector.h"

#include "UserAnalysisCode/ZTauTauAnalysis/plugins/ZTauTauAnalysis.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PhysicsTools/UtilAlgos/interface/SelectionAdderTrait.h"
#include "PhysicsTools/UtilAlgos/interface/StoreContainerTrait.h"
#include "PhysicsTools/UtilAlgos/interface/ParameterAdapter.h"
#include <algorithm>
#include <utility>



//////////////////
//#include "FastSimulation/Muons/plugins/FastL3MuonProducer.h"
//#include "FastSimulation/Muons/plugins/FastL1MuonProducer.h"


///btagging

#include "DataFormats/BTauReco/interface/JetTag.h"

//MC Match
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "PhysicsTools/CandUtils/interface/CandMatcherNew.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Common/interface/Association.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


// #include "PhysicsTools/HepMCCandAlgos/interface/MCCandMatcher.h" 

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/MuonReco/interface/MuIsoDeposit.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"



#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"

#include <TROOT.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <cmath>
#include "TStyle.h"
#include <string>
#include <vector>
#include <set>
#include <Math/VectorUtil.h>
#include "TLorentzVector.h"
#include <algorithm>
//#include "LorentzVector.h"

#include "HepMC/GenEvent.h"
#include "Math/GenVector/VectorUtil.h"
#include "Math/GenVector/PxPyPzE4D.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/TrajectoryCleaning/interface/TrajectoryCleanerBySharedHits.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/PatternTools/interface/TransverseImpactPointExtrapolator.h"
#include "TrackingTools/PatternTools/interface/TSCPBuilderNoMaterial.h"
#include "TrackingTools/PatternTools/interface/TrajectoryFitter.h"
#include "RecoTracker/CkfPattern/interface/TransientInitialStateEstimator.h"
#include "RecoTracker/Record/interface/TrackerRecoGeometryRecord.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "UserAnalysisCode/ZTauTauAnalysis/plugins/MuTkIsolation.h"
#include "UserAnalysisCode/ZTauTauAnalysis/plugins/BtaggingModule.h"

#include "RecoCaloTools/Selectors/interface/CaloDualConeSelector.h"
#include "PhysicsTools/Utilities/interface/PtComparator.h"
#include "Alignment/CommonAlignmentProducer/interface/AlignmentTrackSelector.h"

#include <PhysicsTools/RecoAlgos/src/TrackToCandidate.h>

//#include "UserAnalysisCode/ZTauTauAnalysis/interface/EventTkIsolation.h"
// #include <DataFormats/Common/interface/RefVectorIterator.h>
 

// #include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "DataFormats/Candidate/interface/Candidate.h"
 #include "DataFormats/Candidate/interface/CandidateFwd.h"
//  #include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Candidate/interface/CandMatchMap.h"
 #include "DataFormats/Candidate/interface/CompositeCandidate.h"

//  #include "PhysicsTools/HepMCCandAlgos/interface/MCCandMatcher.h"
//  #include "PhysicsTools/HepMCCandAlgos/interface/MCTruthCompositeMatcher.h"


///////////////  TRIGGER MUONS
#include "RecoMuon/L2MuonSeedGenerator/src/L2MuonSeedGenerator.h"



#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "CalibCalorimetry/CaloMiscalibTools/interface/HcalRecHitRecalib.h" 
#include "RecoCaloTools/MetaCollections/interface/CaloRecHitMetaCollections.h"
#include "DataFormats/Math/interface/LorentzVector.h"

//#include "UserAnalysisCode/ZTauTauAnalysis/plugins/MuonCaloIso.h"


//#include "PhysicsTools/IsolationUtil/interface/TauConeIsolationAlgo"
using namespace edm;
using namespace std;
using namespace reco;
using namespace HepMC; 
using namespace ROOT::Math::VectorUtil ;
//using namespace trigger;

struct ComparePt {
     bool operator()( const reco::Track &t1, const reco::Track &t2 ) const {
       return t1.pt()> t2.pt();
     }
   };

struct ComparePtMu {
     bool operator()( const Candidate &t1, const Candidate &t2 ) const {
       return t1.pt()> t2.pt();
     }
   };

//
// class declaration
//


// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//

ZTauTauAnalysis::ZTauTauAnalysis(const edm::ParameterSet& iConfig){
  
  //now do what ever initialization is needed
  
  cout<<"constructor"<<endl; 
  MuonCand=iConfig.getParameter<edm::InputTag>("muonscand");
TrackCand=iConfig.getParameter<edm::InputTag>("trackscand");

  GenMatch=iConfig.getParameter<edm::InputTag>("genmatch");
GenMatchL=iConfig.getParameter<edm::InputTag>("genmatchl");
  
  //trackTags_=iConfig.getUntrackedParameter<edm::InputTag>("tracks");
  TracksTag=iConfig.getUntrackedParameter<edm::InputTag>("tracks");
  MuonTag=iConfig.getUntrackedParameter<edm::InputTag>("muons");
  //GlobalMuonTag=iConfig.getUntrackedParameter<edm::InputTag>("globalmuons");
  
  PFTau_ = iConfig.getUntrackedParameter<string>("PFTaus","tauPFProducer");
  PFTauDiscriminatorByIsolation_ = iConfig.getUntrackedParameter<string>("PFTauDiscriminatorByIsolationProducer", "discriminationByIsolation");
  

  matchingCone_ = iConfig.getUntrackedParameter<double>("MatchingCone", 0.1);
  signalCone_ = iConfig.getUntrackedParameter<double>("SignalCone", 0.07);
  isolationCone_ = iConfig.getUntrackedParameter<double>("IsolationCone", 0.4);
  leadingTrackPt_ = iConfig.getUntrackedParameter<double>("LeadingTrackPt", 6.0);
  minimumTrackPt_ = iConfig.getUntrackedParameter<double>("MinimumTrackPt", 1.0);
  clusterTrackMatchingCone_ = iConfig.getUntrackedParameter<double>("ClusterTrackMatchingCone", 0.08);
  
  CaloMet_=iConfig.getParameter<string>("CaloMet"); 
  
 
  radiusConeExt_=iConfig.getParameter<double>("radiusConeExt");
  radiusConeInt_=iConfig.getParameter<double>("radiusConeInt");
  pTMin_=iConfig.getParameter<double>("pTMin");
  lip_=iConfig.getParameter<double>("lip");
  cut_=iConfig.getParameter<double>("cut");
  
  
  ///// isolation (mu)
  
  theHCALIsoDepositLabel=iConfig.getParameter<InputTag>("HCALIsoDepositLabel");
  theECALIsoDepositLabel=iConfig.getParameter<InputTag>("ECALIsoDepositLabel");
  theHOCALIsoDepositLabel = iConfig.getParameter<InputTag>( "HOCALIsoDepositLabel");
  theTrackerIsoDepositLabel=iConfig.getParameter<InputTag>("TrackerIsoDepositLabel");

  eventCount_=0;

 
  
  rootfile_   = iConfig.getUntrackedParameter<string>("rootfile","myroot.root");
  file_ = new TFile(rootfile_.c_str(),"RECREATE");
  
  // initialize the tree
  
  tree_ = new TTree("ZTauTauAnalysis","ZTauTauAnalysis Tree");
  
  tree_->Branch("Run",&irun,"irun/i");
  tree_->Branch("Event",&ievt,"ievt/i");
  
 tree_->Branch("neumuener",&neumuener,"neumuener/F");
 tree_->Branch("neutauener",&neutauener,"neutauener/F");
 tree_->Branch("neumuenertr",&neumuenertr,"neumuenertr/F");
 tree_->Branch("neutauenertr",&neutauenertr,"neutauenertr/F");
 tree_->Branch("jetMomener",&jetMomener,"jetMomener/F");
 tree_->Branch("jetMomenertr",&jetMomenertr,"jetMomenertr/F");
 tree_->Branch("jetMompt",&jetMompt,"jetMompt/F");
 tree_->Branch("jetMometa",&jetMometa,"jetMometa/F");
 tree_->Branch("jetMommass",&jetMommass,"jetMommass/F");
 tree_->Branch("jetMommasstr",&jetMommasstr,"jetMommasstr/F"); 
 tree_->Branch("muener",&muener,"muener/F");
 tree_->Branch("muenertr",&muenertr,"muenertr/F");
 tree_->Branch("mupt",&mupt,"mupt/F");
 tree_->Branch("mueta",&mueta,"mueta/F");
 tree_->Branch("mumass",&mumass,"mumass/F");
 tree_->Branch("mumasstr",&mumasstr,"mumasstr/F");
 tree_->Branch("Z_eta",&Z_eta,"Z_eta/F");
 tree_->Branch("Z_rap",&Z_rap,"Z_rap/F");
 tree_->Branch("Z_rapidity",&Z_rapidity,"Z_rapidity/F"); 
 tree_->Branch("Zeta_fin_rapidity",&Zeta_fin_rapidity,"Zeta_fin_rapidity/F");
 tree_->Branch("trvismass1",&trvismass1,"trvismass1/F");
 tree_->Branch("Zvisibletransversemass_relat2",&Zvisibletransversemass_relat2,"Zvisibletransversemass_relat2/F");
 tree_->Branch("tauhener_ap",&tauhener_ap,"tauhener_ap/F");
 tree_->Branch("tauhenertr_ap",&tauhenertr_ap,"tauhenertr_ap/F");
 tree_->Branch("tauhpt_ap",&tauhpt_ap,"tauhpt_ap/F");
 tree_->Branch("tauheta_ap",&tauheta_ap,"tauheta_ap/F");
 tree_->Branch("tauhmass_ap",&tauhmass_ap,"tauhmass_ap/F");
 tree_->Branch("tauhmasstr_ap",&tauhmasstr_ap,"tauhmasstr_ap/F");
 tree_->Branch("taulener_ap",&taulener_ap,"taulener_ap/F");
 tree_->Branch("taulenertr_ap",&taulenertr_ap,"taulenertr_ap/F");
 tree_->Branch("taulpt_ap",&taulpt_ap,"taulpt_ap/F");
 tree_->Branch("tauleta_ap",&tauleta_ap,"tauleta_ap/F");
 tree_->Branch("taulmass_ap",&taulmass_ap,"taulmass_ap/F");
 tree_->Branch("taulmasstr_ap",&taulmasstr_ap,"taulmasstr_ap/F");
 tree_->Branch("tauneutener",&tauneutener,"tauneutener/F");
 tree_->Branch("tauneutenertr",&tauneutenertr,"tauneutenertr/F");
 tree_->Branch("ptleadingtrack",&ptleadingtrack,"ptleadingtrack/F");
 tree_->Branch("charged",&charged,"charged/F");
 tree_->Branch("GenEnergyZ",&GenEnergyZ,"GenEnergyZ/F");
 tree_->Branch("GenTrEnergyZ",&GenTrEnergyZ,"GenTrEnergyZ/F");
 tree_->Branch("GenP_XZ",&GenP_XZ,"GenP_XZ/F");
 tree_->Branch("GenP_YZ",&GenP_YZ,"GenP_YZ/F");
 tree_->Branch("GenP_ZZ",&GenP_ZZ,"GenP_ZZ/F");
 tree_->Branch("GenP_TZ",&GenP_TZ,"GenP_TZ/F");
 tree_->Branch("GenInvMassZ",&GenInvMassZ,"GenInvMassZ/F");
 tree_->Branch("GenTrMassZ_rel",&GenTrMassZ_rel,"GenTrMassZ_rel/F");
 tree_->Branch("GenTrMassZ_A",&GenTrMassZ_A,"GenTrMassZ_A/F");
 tree_->Branch("GenVisMassZ",&GenVisMassZ,"GenVisMassZ/F");
 tree_->Branch("GenVisTrMassZ_A",&GenVisTrMassZ_A,"GenVisTrMassZ_A/F");
 tree_->Branch("GenVisTrMassZ_rel",&GenVisTrMassZ_rel,"GenVisTrMassZ_rel/F");

  //////////////da qui commento per prova
  tree_->Branch("OldIsoPTMu",&OldIsoPTMu,"OldIsoPTMu/F"); 
  tree_->Branch("OldIsoEnMu",&OldIsoEnMu,"OldIsoEnMu/F");
  tree_->Branch("OldIsoTrEnMu",&OldIsoTrEnMu,"OldIsoTrEnMu/F");
  tree_->Branch("OldIsoInvMassMu",&OldIsoInvMassMu,"OldIsoInvMassMu/F");
  tree_->Branch("OldIsoEtaMu",&OldIsoEtaMu,"OldIsoEtaMu/F");
  tree_->Branch("OldIsoTrMassMu",&OldIsoTrMassMu,"OldIsoTrMassMu/F");
  tree_->Branch("OldIsoChMu",&OldIsoChMu,"OldIsoChMu/F");
  tree_->Branch("OldIsoImpParMu",&OldIsoImpParMu,"OldIsoImpParMu/F");
  tree_->Branch("OldIsoPhiMu",&OldIsoPhiMu,"OldIsoPhiMu/F");   
  tree_->Branch("NumOldIsolMu",&NumOldIsolMu,"NumOldIsolMu/I");
  

  tree_->Branch("NewIsoPTMu",&NewIsoPTMu,"NewIsoPTMu/F"); 
  tree_->Branch("NewIsoEnMu",&NewIsoEnMu,"NewIsoEnMu/F");
  tree_->Branch("NewIsoTrEnMu",&NewIsoTrEnMu,"NewIsoTrEnMu/F");
  tree_->Branch("NewIsoInvMassMu",&NewIsoInvMassMu,"NewIsoInvMassMu/F");
  tree_->Branch("NewIsoEtaMu",&NewIsoEtaMu,"NewIsoEtaMu/F");
  tree_->Branch("NewIsoTrMassMu",&NewIsoTrMassMu,"NewIsoTrMassMu/F");
  tree_->Branch("NewIsoChMu",&NewIsoChMu,"NewIsoChMu/F");
  tree_->Branch("NewIsoImpParMu",&NewIsoImpParMu,"NewIsoImpParMu/F");
  tree_->Branch("NewIsoPhiMu",&NewIsoPhiMu,"NewIsoPhiMu/F"); 
  tree_->Branch("NumNewIsolMu",&NumNewIsolMu,"NumNewIsolMu/I");



  tree_->Branch("PTSum",&PTSum,"PTSum/F");
  tree_->Branch("isoValue",&isoValue,"isoValue/F");
  tree_->Branch("NumGloMuons",&NumGloMuons,"NumGloMuons/I");
  tree_->Branch("NumIsolTracks",&NumIsolTracks,"NumIsolTracks/F");
  tree_->Branch("NumMuons",&NumMuons,"NumMuons/I");

  tree_->Branch("CaloMETMissTrEner",&CaloMETMissTrEner,"CaloMETMissTrEner/F");
  tree_->Branch("CaloMETSumMissTrEner",&CaloMETSumMissTrEner,"CaloMETSumMissTrEner/F");
  tree_->Branch("CaloMETSignificance",&CaloMETSignificance,"CaloMETSignificance/F");
  tree_->Branch("CaloMET_E_long",&CaloMET_E_long,"CaloMET_E_long/F"); 
  tree_->Branch("CaloMET_time",&CaloMET_time,"CaloMET_time/F");
  tree_->Branch("phiMET",&phiMET,"phiMET/F");  
  tree_->Branch("FlagRecoMET",&FlagRecoMET,"FlagRecoMET/b");

  tree_->Branch("PFRecoEnergyZ",&PFRecoEnergyZ,"PFRecoEnergyZ/F");
  tree_->Branch("PFRecoTrEnergyZ",&PFRecoTrEnergyZ,"PFRecoTrEnergyZ/F");
  tree_->Branch("PFRecoTrVisEnergyZ",&PFRecoTrVisEnergyZ,"PFRecoTrVisEnergyZ/F");
  tree_->Branch("PFRecoP_XZ",&PFRecoP_XZ,"PFRecoP_XZ/F");
  tree_->Branch("PFRecoP_YZ",&PFRecoP_YZ,"PFRecoP_YZ/F");
  tree_->Branch("PFRecoP_ZZ",&PFRecoP_ZZ,"PFRecoP_ZZ/F");
  tree_->Branch("PFRecoP_TZ",&PFRecoP_TZ,"PFRecoP_TZ/F");
  tree_->Branch("PFRecoVisMassZ",&PFRecoVisMassZ,"PFRecoVisMassZ/F");
  tree_->Branch("PFRecoInvMassZ_A",&PFRecoInvMassZ_A,"PFRecoInvMassZ_A/F");
  tree_->Branch("PFRecoInvMassZ_B",&PFRecoInvMassZ_B,"PFRecoInvMassZ_B/F");
  tree_->Branch("PFRecoInvMassZ_C",&PFRecoInvMassZ_C,"PFRecoInvMassZ_C/F");
  tree_->Branch("PFRecoTrMassZ_A",&PFRecoTrMassZ_A,"PFRecoTrMassZ_A/F");
  tree_->Branch("PFRecoTrMassZ_B",&PFRecoTrMassZ_B,"PFRecoTrMassZ_B/F");
  tree_->Branch("PFRecoTrMassZ_C",&PFRecoTrMassZ_C,"PFRecoTrMassZ_C/F");
  tree_->Branch("PFRecoTrVisMassZ",&PFRecoTrVisMassZ,"PFRecoTrVisMassZ/F");
  tree_->Branch("PFRecoTrVisMassZ_rel",&PFRecoTrVisMassZ_rel,"PFRecoTrVisMassZ_rel/F");
  
  tree_->Branch("dr",&dr,"dr/F"); 
  
  tree_->Branch("deltaphiTauMET",&deltaphiTauMET,"deltaphiTauMET/F");
  tree_->Branch("cosDeltaPhiTauMET",&cosDeltaPhiTauMET,"cosDeltaPhiTauMET/F");
  tree_->Branch("deltaphiTM",&deltaphiTM,"deltaphiTM/F");
  tree_->Branch("cosDeltaPhiTM",&cosDeltaPhiTM,"cosDeltaPhiTM/F");
  tree_->Branch("deltaphiMuMET",&deltaphiMuMET,"deltaphiMuMET/F");
  tree_->Branch("cosDeltaPhiMuMET",&cosDeltaPhiMuMET,"cosDeltaPhiMuMET/F");

  tree_->Branch("tau_disc",&tau_disc,"tau_disc/F");
  tree_->Branch("tau_eta",&tau_eta,"tau_eta/F");
  tree_->Branch("tau_leading_trk_pt",&tau_leading_trk_pt,"tau_leading_trk_pt/F");
  tree_->Branch("tau_had_energy",&tau_had_energy,"tau_had_energy/F");
  tree_->Branch("tau_inv_mass",&tau_inv_mass,"tau_inv_mass/F");
  tree_->Branch("tau_inv_mass2",&tau_inv_mass2,"tau_inv_mass2/F");
  tree_->Branch("tau_sumpt_iso_ch_hadrons",&tau_sumpt_iso_ch_hadrons,"tau_sumpt_iso_ch_hadrons/F");
  tree_->Branch("tau_sumpt_iso_photons",&tau_sumpt_iso_photons,"tau_sumpt_iso_photons/F");
  tree_->Branch("leading_trk_sipt",&leading_trk_sipt,"leading_trk_sipt/F");
  tree_->Branch("chHadEnergyJet",&chHadEnergyJet,"chHadEnergyJet/F");
  tree_->Branch("tau_pt",&tau_pt,"tau_pt/F");
  tree_->Branch("tau_px",&tau_px,"tau_px/F");
  tree_->Branch("tau_py",&tau_py,"tau_py/F");
  tree_->Branch("tau_pz",&tau_pz,"tau_pz/F");
  tree_->Branch("tau_energy",&tau_energy,"tau_energy/F");
  tree_->Branch("tau_et",&tau_et,"tau_et/F");
  tree_->Branch("tau_ch_hadrons",&tau_ch_hadrons,"tau_ch_hadrons/I");
  tree_->Branch("tau_neu_hadrons",&tau_neu_hadrons,"tau_neu_hadrons/I");
  tree_->Branch("tau_photons",&tau_photons,"tau_photons/I");
  tree_->Branch("tau_sig_ch_hadrons",&tau_sig_ch_hadrons,"tau_sig_ch_hadrons/I");
  tree_->Branch("tau_iso_ch_hadrons",&tau_iso_ch_hadrons,"tau_iso_ch_hadrons/I");
  tree_->Branch("tau_iso_photons",&tau_iso_photons,"tau_iso_photons/I");
  tree_->Branch("tau_iso_neu_hadrons",&tau_iso_neu_hadrons,"tau_iso_neu_hadrons/I");
  tree_->Branch("tau_sig_photons",&tau_sig_photons,"tau_sig_photons/I");
  tree_->Branch("tau_sig_neu_hadrons",&tau_sig_neu_hadrons,"tau_sig_neu_hadrons/I");
  tree_->Branch("n_tau_candidate",&n_tau_candidate,"n_tau_candidate/I");
  tree_->Branch("tau_zvertex",&tau_zvertex,"tau_zvertex/D");
  tree_->Branch("tau_charge",&tau_charge,"tau_charge/D");
  tree_->Branch("QualityEvent",&QualityEvent,"QualityEvent/b");
  tree_->Branch("TauCharge",&TauCharge,"TauCharge/F");


  //////  tracce 
  tree_->Branch("NumTracks_8",&NumTracks_8,"NumTracks_8/I");
  tree_->Branch("NumTracks_10",&NumTracks_10,"NumTracks_10/I");  
  tree_->Branch("NumRecHitsMin",&NumRecHitsMin,"NumRecHitsMin/F");
  tree_->Branch("NumRecHitsMax",&NumRecHitsMax,"NumRecHitsMax/F");
  tree_->Branch("NumValRecHitsMin",&NumValRecHitsMin,"NumValRecHitsMin/F");
  tree_->Branch("NumValRecHitsMax",&NumValRecHitsMax,"NumValRecHitsMax/F");
  tree_->Branch("SumTrackPt8",&SumTrackPt8,"SumTrackPt8/F");
  tree_->Branch("SumTrackPt10",&SumTrackPt10,"SumTrackPt10/F");
  
  
  tree_->Branch("PFJetET",&PFJetET,"PFJetET/F"); 
  tree_->Branch("nPFjet",&nPFjet,"nPFjet/I");


tree_->Branch("b_discSM",&b_discSM,"b_discSM/F");
tree_->Branch("b_discSMNoIP",&b_discSMNoIP,"b_discSMNoIP/F");
tree_->Branch("b_discJP",&b_discJP,"b_discJP/F");

tree_->Branch("BDiscSumSM",&BDiscSumSM,"BDiscSumSM/F");
tree_->Branch("BDiscSumSMNoIP",&BDiscSumSMNoIP,"BDiscSumSMNoIP/F");
tree_->Branch("BDiscSumJP",&BDiscSumJP,"BDiscSumJP/F");

tree_->Branch("b_discTCHE",&b_discTCHE,"b_discTCHE/F");
tree_->Branch("b_discTCHP",&b_discTCHP,"b_discTCHP/F");

//tree_->Branch("b_discSL",&b_discSL,"b_discSL/F");

//
//tree_->Branch("BDiscMeanSL",&BDiscMeanSL,"BDiscMeanSL/F");
tree_->Branch("BDiscSumTCHE",&BDiscSumTCHE,"BDiscSumTCHE/F");
tree_->Branch("BDiscSumTCHP",&BDiscSumTCHP,"BDiscSumTCHP/F");
//

tree_->Branch("dr_a",&dr_a,"dr_a/F"); 
 
tree_->Branch("TauImpPar",&TauImpPar,"TauImpPar/F");

//tree_->Branch("tau_disc",&tau_disc,"tau_disc/F");

//tree_->Branch("",&,"/F");
tree_->Branch("tau_em_energy",&tau_em_energy,"tau_em_energy/F");
tree_->Branch("neuHadEnergyJet",&neuHadEnergyJet,"neuHadEnergyJet/F");
tree_->Branch("chEMEnergyJet",&chEMEnergyJet,"chEMEnergyJet/F");
tree_->Branch("chMuEnergyJet",&chMuEnergyJet,"chMuEnergyJet/F");
tree_->Branch("neuEMEnergyJet",&neuEMEnergyJet,"neuEMEnergyJet/F");
tree_->Branch("chMultJet",&chMultJet,"chMultJet/F");
tree_->Branch("neuMultJet",&neuMultJet,"neuMultJet/F");
tree_->Branch("muMultJet",&muMultJet,"muMultJet/F");


tree_->Branch("pdgId_moth",&pdgId_moth,"pdgId_moth/F");
tree_->Branch("pdgId_cand",&pdgId_cand,"pdgId_cand/F");
tree_->Branch("size_moth",&size_moth,"size_moth/F");







tree_->Branch("drTB",&drTB,"drTB/F");
tree_->Branch("tau_chargeTrue",&tau_chargeTrue,"tau_chargeTrue/F");

// tree_->Branch("DRMINTBSM",&DRMINTBSM,"DRMINTSMB/F");
// tree_->Branch("DRMAXMBSM",&DRMAXMBSM,"DRMAXMBSM/F");
// //tree_->Branch("nJetBSM",&nJetBSM,"nJetBSM/I");
// tree_->Branch("enerTrBjetSM1",&enerTrBjetSM1,"enerTrBjetSM1/F");
// tree_->Branch("enerTrBjetSM2",&enerTrBjetSM2,"enerTrBjetSM2/F");
// tree_->Branch("enerTrBjet3SM",&enerTrBjetSM3,"enerTrBjetSM3/F");
// tree_->Branch("enerTrBjetSM4",&enerTrBjetSM4,"enerTrBjetSM4/F");

// tree_->Branch("DRMINTBSMNoIP",&DRMINTBSMNoIP,"DRMINTBSMNoIP/F");
// tree_->Branch("DRMINMBSMNoIP",&DRMINMBSMNoIP,"DRMINMBSMNoIP/F");
// //tree_->Branch("nJetBSMNoIP",&nJetBSMNoIP,"nJetBSMNoIP/I");
// tree_->Branch("enerTrBjetSMNoIP1",&enerTrBjetSMNoIP1,"enerTrBjetSMNoIP1/F");
// tree_->Branch("enerTrBjetSMNoIP2",&enerTrBjetSMNoIP2,"enerTrBjetSMNoIP2/F");
// tree_->Branch("enerTrBjetSMNoIP3",&enerTrBjetSMNoIP3,"enerTrBjetSMNoIP3/F");
// tree_->Branch("enerTrBjetSMNoIP4",&enerTrBjetSMNoIP4,"enerTrBjetSMNoIP4/F");

// tree_->Branch("DRMINTBJP",&DRMINTBJP,"DRMINTBJP/F");
// tree_->Branch("DRMINMBJP",&DRMINMBJP,"DRMINMBJP/F");
// //tree_->Branch("nJetBJP",&nJetBJP,"nJetBJP/I");
// tree_->Branch("enerTrBjetJP1",&enerTrBjetJP1,"enerTrBjetJP1/F");
// tree_->Branch("enerTrBjetJP2",&enerTrBjetJP2,"enerTrBjetJP2/F");
// tree_->Branch("enerTrBjetJP3",&enerTrBjetJP3,"enerTrBjetJP3/F");
// tree_->Branch("enerTrBjetJP4",&enerTrBjetJP4,"enerTrBjetJP4/F");

// tree_->Branch("DRMINTBTCHE",&DRMINTBTCHE,"DRMINTBTCHE/F");
// tree_->Branch("DRMINMBTCHE",&DRMINMBTCHE,"DRMINMBTCHE/F");
// //tree_->Branch("nJetBTCHE",&nJetBTCHE,"nJetBTCHE/I");
// tree_->Branch("enerTrBjetTCHE1",&enerTrBjetTCHE1,"enerTrBjetTCHE1/F");
// tree_->Branch("enerTrBjetTCHE2",&enerTrBjetTCHE2,"enerTrBjetTCHE2/F");
// tree_->Branch("enerTrBjetTCHE3",&enerTrBjetTCHE3,"enerTrBjetTCHE3/F");
// tree_->Branch("enerTrBjetTCHE4",&enerTrBjetTCHE4,"enerTrBjetTCHE4/F");

// tree_->Branch("DRMINTBTCHP",&DRMINTBTCHP,"DRMINTBTCHP/F");
// tree_->Branch("DRMINMBTCHP",&DRMINMBTCHP,"DRMINMBTCHP/F");
// //tree_->Branch("nJetBTCHP",&nJetBTCHP,"nJetBTCHP/I");
// tree_->Branch("enerTrBjetTCHP1",&enerTrBjetTCHP1,"enerTrBjetTCHP1/F");
// tree_->Branch("enerTrBjetTCHP2",&enerTrBjetTCHP2,"enerTrBjetTCHP2/F");
// tree_->Branch("enerTrBjetTCHP3",&enerTrBjetTCHP3,"enerTrBjetTCHP3/F");
// tree_->Branch("enerTrBjetTCHP4",&enerTrBjetTCHP4,"enerTrBjetTCHP4/F");


// // tree_->Branch("b_discSM1",&b_discSM1,"b_discSM1/F");
// // tree_->Branch("b_discSM2",&b_discSM2,"b_discSM2/F");
// // tree_->Branch("b_discSM3",&b_discSM3,"b_discSM3/F");
// // tree_->Branch("b_discSM4",&b_discSM4,"b_discSM4/F");

// // tree_->Branch("b_discSMNoIP1",&b_discSMNoIP1,"b_discSMNoIP1/F");
// // tree_->Branch("b_discSMNoIP2",&b_discSMNoIP2,"b_discSMNoIP2/F");
// // tree_->Branch("b_discSMNoIP3",&b_discSMNoIP3,"b_discSMNoIP3/F");
// // tree_->Branch("b_discSMNoIP4",&b_discSMNoIP4,"b_discSMNoIP4/F");

// // tree_->Branch("b_discJP1",&b_discJP1,"b_discJP1/F");
// // tree_->Branch("b_discJP2",&b_discJP2,"b_discJP2/F");
// // tree_->Branch("b_discJP3",&b_discJP3,"b_discJP3/F");
// // tree_->Branch("b_discJP4",&b_discJP4,"b_discJP4/F");

// // tree_->Branch("b_discTCHE1",&b_discTCHE1,"b_discTCHE1/F");
// // tree_->Branch("b_discTCHE2",&b_discTCHE2,"b_discTCHE2/F");
// // tree_->Branch("b_discTCHE3",&b_discTCHE3,"b_discTCHE3/F");
// // tree_->Branch("b_discTCHE4",&b_discTCHE4,"b_disTCHEc4/F");

// tree_->Branch("b_discTCHP1",&b_discTCHP1,"b_discTCHP1/F");
// tree_->Branch("b_discTCHP2",&b_discTCHP2,"b_discTCHP2/F");
// tree_->Branch("b_discTCHP3",&b_discTCHP3,"b_discTCHP3/F");
// tree_->Branch("b_discTCHP4",&b_discTCHP4,"b_discTCHP4/F");


// tree_->Branch("drTBSM1",&drTBSM1,"drTBSM1/F");
// tree_->Branch("drTBSM2",&drTBSM2,"drTBSM2/F");
// tree_->Branch("drTBSM3",&drTBSM3,"drTBSM3/F");
// tree_->Branch("drTBSM4",&drTBSM4,"drTBSM4/F");
// tree_->Branch("b_discTSM",&b_discTSM,"b_discTSM/F");
// tree_->Branch("b_discPosFirstSM",&b_discPosFirstSM,"b_discPosFirstSM/F");
// tree_->Branch("b_discPosSecondSM",&b_discPosSecondSM,"b_discPosSecondSM/F");
// tree_->Branch("b_discPosThirdSM",&b_discPosThirdSM,"b_discPosThirdSM/F");
// tree_->Branch("b_discPosFourSM",&b_discPosFourSM,"b_discPosFourSM/F");

// tree_->Branch("drTBSMNoIP1",&drTBSMNoIP1,"drTBSMNoIP1/F");
// tree_->Branch("drTBSMNoIP2",&drTBSMNoIP2,"drTBSMNoIP2/F");
// tree_->Branch("drTBSMNoIP3",&drTBSMNoIP3,"drTBSMNoIP3/F");
// tree_->Branch("drTBSMNoIP4",&drTBSMNoIP4,"drTBSMNoIP4/F");
// tree_->Branch("b_discTSMNoIP",&b_discTSMNoIP,"b_discTSMNoIP/F");
// tree_->Branch("b_discPosFirstSMNoIP",&b_discPosFirstSMNoIP,"b_discPosFirstSMNoIP/F");
// tree_->Branch("b_discPosSecondSMNoIP",&b_discPosSecondSMNoIP,"b_discPosSecondSMNoIP/F");
// tree_->Branch("b_discPosThirdSMNoIP",&b_discPosThirdSMNoIP,"b_discPosThirdSMNoIP/F");
// tree_->Branch("b_discPosFourSMNoIP",&b_discPosFourSMNoIP,"b_discPosFourSMNoIP/F");

// tree_->Branch("drTBJP1",&drTBJP1,"drTBJP1/F");
// tree_->Branch("drTBJP2",&drTBJP2,"drTBJP2/F");
// tree_->Branch("drTBJP3",&drTBJP3,"drTBJP3/F");
// tree_->Branch("drTBJP4",&drTBJP4,"drTBJP4/F");
// tree_->Branch("b_discTJP",&b_discTJP,"b_discTJP/F");
// tree_->Branch("b_discPosFirstJP",&b_discPosFirstJP,"b_discPosFirstJP/F");
// tree_->Branch("b_discPosSecondJP",&b_discPosSecondJP,"b_discPosSecondJP/F");
// tree_->Branch("b_discPosThirdJP",&b_discPosThirdJP,"b_discPosThirdJP/F");
// tree_->Branch("b_discPosFourJP",&b_discPosFourJP,"b_discPosFourJP/F");

// tree_->Branch("drTBTCHE1",&drTBTCHE1,"drTBTCHE1/F");
// tree_->Branch("drTBTCHE2",&drTBTCHE2,"drTBTCHE2/F");
// tree_->Branch("drTBTCHE3",&drTBTCHE3,"drTBTCHE3/F");
// tree_->Branch("drTBTCHE4",&drTBTCHE4,"drTBTCHE4/F");
// tree_->Branch("b_discTTCHE",&b_discTTCHE,"b_discTTCHE/F");
// tree_->Branch("b_discPosFirstTCHE",&b_discPosFirstTCHE,"b_discPosFirstTCHE/F");
// tree_->Branch("b_discPosSecondTCHE",&b_discPosSecondTCHE,"b_discPosSecondTCHE/F");
// tree_->Branch("b_discPosThirdTCHE",&b_discPosThirdTCHE,"b_discPosThirdTCHE/F");
// tree_->Branch("b_discPosFourTCHE",&b_discPosFourTCHE,"b_discPosFourTCHE/F");

// tree_->Branch("drTBTCHP1",&drTBTCHP1,"drTBTCHP1/F");
// tree_->Branch("drTBTCHP2",&drTBTCHP2,"drTBTCHP2/F");
// tree_->Branch("drTBTCHP3",&drTBTCHP3,"drTBTCHP3/F");
// tree_->Branch("drTBTCHP4",&drTBTCHP4,"drTBTCHP4/F");
// tree_->Branch("b_discTTCHP",&b_discTTCHP,"b_discTTCHP/F");
// tree_->Branch("b_discPosFirstTCHP",&b_discPosFirstTCHP,"b_discPosFirstTCHP/F");
// tree_->Branch("b_discPosSecondTCHP",&b_discPosSecondTCHP,"b_discPosSecondTCHP/F");
// tree_->Branch("b_discPosThirdTCHP",&b_discPosThirdTCHP,"b_discPosThirdTCHP/F");
// tree_->Branch("b_discPosFourTCHP",&b_discPosFourTCHP,"b_discPosFourTCHP/F");


// tree_->Branch("drMB1SM",&drMB1SM,"drMB1SM/F");
// tree_->Branch("drMB2SM",&drMB2SM,"drMB2SM/F");
// tree_->Branch("drMB3SM",&drMB3SM,"drMB3SM/F");
// tree_->Branch("drMB3SM",&drMB1SM,"drMB3SM/F");

// tree_->Branch("bdiscM1SM",&bdiscM1SM,"bdiscM1SM/F");
// tree_->Branch("bdiscM2SM",&bdiscM2SM,"bdiscM2SM/F");
// tree_->Branch("bdiscM3SM",&bdiscM3SM,"bdiscM3SM/F");
// tree_->Branch("bdiscM4SM",&bdiscM4SM,"bdiscM4SM/F");

// tree_->Branch("nBjetMSM",&nBjetMSM,"nBjetMSM/I");
// tree_->Branch("Njet10ITC5",&Njet10ITC5,"Njet10ITC5/I");
// tree_->Branch("Njet15ITC5",&Njet15ITC5,"Njet15ITC5/I");
// tree_->Branch("Njet20ITC5",&Njet20ITC5,"Njet20ITC5/I");
// tree_->Branch("nJetITC5",&nJetITC5,"nJetITC5/I");

// tree_->Branch("Njet10SM",&Njet10SM,"Njet10SM/I");
// tree_->Branch("Njet15SM",&Njet15SM,"Njet15SM/I");
// tree_->Branch("Njet20SM",&Njet20SM,"Njet20SM/I");

// tree_->Branch("Njet10SMNoIP",&Njet10SMNoIP,"Njet10SMNoIP/I");
// tree_->Branch("Njet15SMNoIP",&Njet15SMNoIP,"Njet15SMNoIP/I");
// tree_->Branch("Njet20SMNoIP",&Njet20SMNoIP,"Njet20SMNoIP/I");

// tree_->Branch("Njet10JP",&Njet10JP,"Njet10JP/I");
// tree_->Branch("Njet15JP",&Njet15JP,"Njet15JP/I");
// tree_->Branch("Njet20JP",&Njet20JP,"Njet20JP/I");

// tree_->Branch("Njet10TCHE",&Njet10TCHE,"Njet10TCHE/I");
// tree_->Branch("Njet15TCHE",&Njet15TCHE,"Njet15TCHE/I");
// tree_->Branch("Njet20TCHE",&Njet20TCHE,"Njet20TCHE/I");

// tree_->Branch("Njet10TCHP",&Njet10TCHP,"Njet10TCHP/I");
// tree_->Branch("Njet15TCHP",&Njet15TCHP,"Njet15TCHP/I");
// tree_->Branch("Njet20TCHP",&Njet20TCHP,"Njet20TCHP/I");
 
// tree_->Branch("njetITC5",&njetITC5,"njetITC5/I");
// tree_->Branch("njet15ITC5",&njet15ITC5,"njet15ITC5/I");
// tree_->Branch("PT1",&PT1,"PT1/F");
// tree_->Branch("PT2",&PT2,"PT2/F");
// tree_->Branch("PT3",&PT3,"PT3/F");
// tree_->Branch("PT4",&PT4,"PT4/F");


/////////// Mu Isolation

tree_->Branch("delpx",&delpx,"delpx/F");
tree_->Branch("delpy",&delpy,"delpy/F");
tree_->Branch("delpz",&delpz,"delpz/F");
tree_->Branch("delsumet",&delsumet,"delsumet/F");

tree_->Branch("NMuCand",&NMuCand,"NMuCand/I");
tree_->Branch("EcalM_Tot",&EcalM_Tot,"EcalM_Tot[NMuCand]/F");
 tree_->Branch("HcalM_Tot",&HcalM_Tot,"HcalM_Tot[NMuCand]/F");
 tree_->Branch("HoM_Tot",&HoM_Tot,"HoM_Tot[NMuCand]/F");
 tree_->Branch("TrkM_Tot",&TrkM_Tot,"TrkM_Tot[NMuCand]/F");

 tree_->Branch("EcalM_Frac",EcalM_Frac,"EcalM_Frac[15][5]/F");
 tree_->Branch("HcalM_Frac",HcalM_Frac,"HcalM_Frac[15][5]/F");
 tree_->Branch("HoM_Frac",HoM_Frac,"HoM_Frac[15][5]/F");
 tree_->Branch("TrkM_Frac",TrkM_Frac,"TrkM_Frac[15][5]/F");
 tree_->Branch("TrkIsoM_N",TrkIsoM_N,"TrkIsoM_N[15][5]/F");

// tree_->Branch("mhcalfrac25",&mhcalfrac25,"mhcalfrac25/F");
// tree_->Branch("mtrackfrac25",&mtrackfrac25,"mtrackfrac25/F");
// tree_->Branch("mhofrac25",&mhofrac25,"mhofrac25/F");
// tree_->Branch("mecalfrac25",&mecalfrac25,"mecalfrac25/F");
// tree_->Branch("calfrac25",&calfrac25,"calfrac25/F");
// tree_->Branch("EcalM_Frac40",&EcalM_Frac40,"EcalM_Frac40/F");
// tree_->Branch("HcalM_Frac40",&HcalM_Frac40,"HcalM_Frac40/F");
// tree_->Branch("HoM_Frac40",&HoM_Frac40,"HoM_Frac40/F");
// tree_->Branch("TrkM_Frac40",&TrkM_Frac40,"TrkM_Frac40/F");
// tree_->Branch("TrkIsoM_N40",&TrkIsoM_N40,"TrkIsoM_N40/F");
// tree_->Branch("mhcalfrac40",&mhcalfrac40,"mhcalfrac40/F");
// tree_->Branch("mtrackfrac40",&mtrackfrac40,"mtrackfrac40/F");
// tree_->Branch("mhofrac40",&mhofrac40,"mhofrac40/F");
// tree_->Branch("mecalfrac40",&mecalfrac40,"mecalfrac40/F");
// tree_->Branch("calfrac40",&calfrac40,"calfrac40/F");




tree_->Branch("NPFJetCand",&NPFJetCand,"NPFJetCand/I");
tree_->Branch("NCaloJetCand",&NCaloJetCand,"NCaloJetCand/I");
tree_->Branch("NTrackCand",&NTrackCand,"NTrackCand/I");
tree_->Branch("NCaloJetCandSM",&NCaloJetCandSM,"NCaloJetCandSM/I");
tree_->Branch("NCaloJetCandSMNoIP",&NCaloJetCandSMNoIP,"NCaloJetCandSMNoIP/I");
tree_->Branch("NCaloJetCandJP",&NCaloJetCandJP,"NCaloJetCandJP/I");
tree_->Branch("NCaloJetCandTCHE",&NCaloJetCandTCHE,"NCaloJetCandTCHE/I");
tree_->Branch("NCaloJetCandTCHP",&NCaloJetCandTCHP,"NCaloJetCandTCHP/I");
tree_->Branch("NTauCand",&NTauCand,"NTauCand/I");

tree_->Branch("MuSize",&MuSize,"MuSize/I");
tree_->Branch("Muon_pt",Muon_pt,"Muon_pt[NMuCand]/F");
tree_->Branch("Muon_eta",Muon_eta,"Muon_eta[NMuCand]/F");
tree_->Branch("Muon_p",Muon_p,"Muon_p[NMuCand]/F");
 tree_->Branch("Muon_px",Muon_px,"Muon_px[NMuCand]/F");
tree_->Branch("Muon_py",Muon_py,"Muon_py[NMuCand]/F");
tree_->Branch("Muon_pz",Muon_pz,"Muon_pz[NMuCand]/F");
tree_->Branch("Muon_en",Muon_en,"Muon_en[NMuCand]/F");
tree_->Branch("Muon_tren",Muon_tren,"Muon_tren[NMuCand]/F");
tree_->Branch("Muon_phi",Muon_phi,"Muon_phi[NMuCand]/F");
tree_->Branch("Muon_theta",Muon_theta,"Muon_theta[NMuCand]/F");
tree_->Branch("Muon_charge",Muon_charge,"Muon_charge[NMuCand]/F");
tree_->Branch("MuonDz",&MuonDz,"MuonDz[NMuCand]/F");
tree_->Branch("MuonIP",&MuonIP,"MuonIP[NMuCand]/F");
tree_->Branch("Muonvty",&Muonvty,"Muonvty[NMuCand]/F");
tree_->Branch("Muonvtx",&Muonvtx,"Muonvtx[NMuCand]/F");
tree_->Branch("Muonvtz",&Muonvtz,"Muonvtz[NMuCand]/F");

tree_->Branch("TrackSize",&TrackSize,"TrackSize/I");
tree_->Branch("Tk_ValRecHits",&Tk_ValRecHits,"Tk_ValRecHits[NTrackCand]/F");
tree_->Branch("Tk_NumRecHits",&Tk_NumRecHits,"Tk_NumRecHits[NTrackCand]/F");
tree_->Branch("Tk_ChiSqr",&Tk_ChiSqr,"Tk_ChiSqr[NTrackCand]/F");
tree_->Branch("Tk_Pt",&Tk_Pt,"Tk_Pt[NTrackCand]/F");
tree_->Branch("Tk_d0",&Tk_d0,"Tk_d0[NTrackCand]/F");
tree_->Branch("Tk_dxy",&Tk_dxy,"Tk_dxy[NTrackCand]/F");
tree_->Branch("Tk_dz",&Tk_dz,"Tk_dz[NTrackCand]/F");
tree_->Branch("Tk_LostHits",&Tk_LostHits,"Tk_LostHits[NTrackCand]/F");

tree_->Branch("PFJetSize",&PFJetSize,"PFJetSize/I");
tree_->Branch("PFJet_Pt",&PFJet_Pt,"PFJet_Pt[NPFJetCand]/F");
tree_->Branch("PFJet_P",&PFJet_P,"PFJet_P[NPFJetCand]/F");
tree_->Branch("PFJet_E",&PFJet_E,"PFJet_E[NPFJetCand]/F");
tree_->Branch("PFJet_Et",&PFJet_Et,"PFJet_Et[NPFJetCand]/F");

tree_->Branch("CaloJetSize",&CaloJetSize,"CaloJetSize/I");
tree_->Branch("CaloJet_Pt",&CaloJet_Pt,"CaloJet_Pt[NCaloJetCand]/F");
tree_->Branch("CaloJet_P",&CaloJet_P,"CaloJet_P[NCaloJetCand]/F");
tree_->Branch("CaloJet_E",&CaloJet_E,"CaloJet_E[NCaloJetCand]/F");
 tree_->Branch("CaloJet_Et",&CaloJet_Et,"CaloJet_Et[NCaloJetCand]/F");

tree_->Branch("eventCount_",&eventCount_,"eventCount_/I");

tree_->Branch("NMuCandSM",&NMuCandSM,"NMuCandSM/I");
 tree_->Branch("SMCaloJet_Bdisc",&SMCaloJet_Bdisc,"SMCaloJet_Bdisc[NCaloJetCandSM]/D");
 tree_->Branch("SMCaloJet_DrT",&SMCaloJet_DrT,"SMCaloJet_DrT[NCaloJetCandSM]/D");
 tree_->Branch("SMCaloJet_DPhiT",&SMCaloJet_DPhiT,"SMCaloJet_DPhiT[NCaloJetCandSM]/D");
 tree_->Branch("SMCaloJet_DrM",SMCaloJet_DrM,"SMCaloJet_DrM[15][15]/D");
 tree_->Branch("SMCaloJet_DPhiM",SMCaloJet_DPhiM,"SMCaloJet_DPhiM[15][15]/D");

tree_->Branch("SMNoIPCaloJet_Bdisc",&SMNoIPCaloJet_Bdisc,"SMNoIPCaloJet_Bdisc[NCaloJetCandSMNoIP]/D");
 tree_->Branch("SMNoIPCaloJet_DrT",&SMNoIPCaloJet_DrT,"SMNoIPCaloJet_DrT[NCaloJetCandSMNoIP]/D");
 tree_->Branch("SMNoIPCaloJet_DPhiT",&SMNoIPCaloJet_DPhiT,"SMNoIPCaloJet_DPhiT[NCaloJetCandSMNoIP]/D");
 tree_->Branch("SMNoIPCaloJet_DrM",SMNoIPCaloJet_DrM,"SMNoIPCaloJet_DrM[15][15]/D");
 tree_->Branch("SMNoIPCaloJet_DPhiM",SMNoIPCaloJet_DPhiM,"SMNoIPCaloJet_DPhiM[15][15]/D");
tree_->Branch("NMuCandSMNoIP",&NMuCandSMNoIP,"NMuCandSMNoIP/I");

tree_->Branch("NMuCandJP",&NMuCandJP,"NMuCandJP/I");
 tree_->Branch("JPCaloJet_Bdisc",&JPCaloJet_Bdisc,"JPCaloJet_Bdisc[NCaloJetCandJP]/D");
 tree_->Branch("JPCaloJet_DrT",&JPCaloJet_DrT,"JPCaloJet_DrT[NCaloJetCandJP]/D");
 tree_->Branch("JPCaloJet_DPhiT",&JPCaloJet_DPhiT,"JPCaloJet_DPhiT[NCaloJetCandJP]/D");
 tree_->Branch("JPCaloJet_DrM",JPCaloJet_DrM,"JPCaloJet_DrM[15][15]/D");
 tree_->Branch("JPCaloJet_DPhiM",JPCaloJet_DPhiM,"JPCaloJet_DPhiM[15][15]/D");

tree_->Branch("NMuCandTCHE",&NMuCandTCHE,"NMuCandTCHE/I");
 tree_->Branch("TCHECaloJet_Bdisc",&TCHECaloJet_Bdisc,"TCHECaloJet_Bdisc[NCaloJetCandTCHE]/D");
 tree_->Branch("TCHECaloJet_DrT",&TCHECaloJet_DrT,"TCHECaloJet_DrT[NCaloJetCandTCHE]/D");
 tree_->Branch("TCHECaloJet_DPhiT",&TCHECaloJet_DPhiT,"TCHECaloJet_DPhiT[NCaloJetCandTCHE]/D");
 tree_->Branch("TCHECaloJet_DrM",TCHECaloJet_DrM,"TCHECaloJet_DrM[15][15]/D");
 tree_->Branch("TCHECaloJet_DPhiM",TCHECaloJet_DPhiM,"TCHECaloJet_DPhiM[15][15]/D");

tree_->Branch("NMuCandTCHP",&NMuCandTCHP,"NMuCandTCHP/I");
 tree_->Branch("TCHPCaloJet_Bdisc",&TCHPCaloJet_Bdisc,"TCHPCaloJet_Bdisc[NCaloJetCandTCHP]/D");
 tree_->Branch("TCHPCaloJet_DrT",&TCHPCaloJet_DrT,"TCHPCaloJet_DrT[NCaloJetCandTCHP]/D");
 tree_->Branch("TCHPCaloJet_DPhiT",&TCHPCaloJet_DPhiT,"TCHPCaloJet_DPhiT[NCaloJetCandTCHP]/D");
 tree_->Branch("TCHPCaloJet_DrM",TCHPCaloJet_DrM,"TCHPCaloJet_DrM[15][15]/D");
 tree_->Branch("TCHPCaloJet_DPhiM",TCHPCaloJet_DPhiM,"TCHPCaloJet_DPhiM[15][15]/D");


tree_->Branch("Tau_disc",&Tau_disc,"Tau_disc[NTauCand]/F");
tree_->Branch("Tau_eta",&Tau_eta,"Tau_eta[NTauCand]/F");
tree_->Branch("Tau_leading_trk_pt",&Tau_leading_trk_pt,"Tau_leading_trk_pt[NTauCand]/F");
tree_->Branch("Tau_had_energy",&Tau_had_energy,"Tau_had_energy[NTauCand]/F");
tree_->Branch("Tau_inv_mass",&Tau_inv_mass,"Tau_inv_mass[NTauCand]/F");
tree_->Branch("Tau_sumpt_iso_ch_hadrons",&Tau_sumpt_iso_ch_hadrons,"Tau_sumpt_iso_ch_hadrons[NTauCand]/F");
tree_->Branch("Tau_sumpt_iso_photons",&Tau_sumpt_iso_photons,"Tau_sumpt_iso_photons[NTauCand]/F");
tree_->Branch("Leading_trk_sipt",&Leading_trk_sipt,"Leading_trk_sipt[NTauCand]/F");
tree_->Branch("Tau_inv_mass2",&Tau_inv_mass2,"Tau_inv_mass2[NTauCand]/F");
tree_->Branch("ChHadEnergyJet",&ChHadEnergyJet,"ChHadEnergyJet[NTauCand]/F");
tree_->Branch("Tau_ch_hadrons",&Tau_ch_hadrons,"Tau_ch_hadrons[NTauCand]/I");
tree_->Branch("Tau_neu_hadrons",&Tau_neu_hadrons,"Tau_neu_hadrons[NTauCand]/I");
tree_->Branch("Tau_photons",&Tau_photons,"Tau_photons[NTauCand]/I");
tree_->Branch("Tau_sig_ch_hadrons",&Tau_sig_ch_hadrons,"Tau_sig_ch_hadrons[NTauCand]/I");
tree_->Branch("Tau_iso_ch_hadrons",&Tau_iso_ch_hadrons,"Tau_iso_ch_hadrons[NTauCand]/I");
tree_->Branch("Tau_iso_photons",&Tau_iso_photons,"Tau_iso_photons[NTauCand]/I");
tree_->Branch("Tau_iso_neu_hadrons",&Tau_iso_neu_hadrons,"Tau_iso_neu_hadrons[NTauCand]/I");
tree_->Branch("Tau_sig_photons",&Tau_sig_photons,"Tau_sig_photons[NTauCand]/I");
tree_->Branch("Tau_sig_neu_hadrons",&Tau_sig_neu_hadrons,"Tau_sig_neu_hadrons[NTauCand]/I");
tree_->Branch("Tau_pt",&Tau_pt,"Tau_pt[NTauCand]/F");
tree_->Branch("Tau_px",&Tau_px,"Tau_px[NTauCand]/F");
tree_->Branch("Tau_py",&Tau_py,"Tau_py[NTauCand]/F");
tree_->Branch("Tau_pz",&Tau_pz,"Tau_pz[NTauCand]/F");
tree_->Branch("Tau_energy",&Tau_energy,"Tau_energy[NTauCand]/F");
tree_->Branch("Tau_et",&Tau_et,"Tau_et[NTauCand]/F");
tree_->Branch("Tau_Charge",&Tau_Charge,"Tau_Charge[NTauCand]/F");
tree_->Branch("Tau_zvertex",&Tau_zvertex,"Tau_zvertex[NTauCand]/F");
tree_->Branch("Tau_charge",&Tau_charge,"Tau_charge[NTauCand]/F");
tree_->Branch("Tau_em_energy",&Tau_em_energy,"Tau_em_energy[NTauCand]/F");
tree_->Branch("NeuHadEnergyJet",&NeuHadEnergyJet,"NeuHadEnergyJet[NTauCand]/F");
tree_->Branch("ChEMEnergyJet",&ChEMEnergyJet,"ChEMEnergyJet[NTauCand]/F");
tree_->Branch("ChMuEnergyJet",&ChMuEnergyJet,"ChMuEnergyJet[NTauCand]/F");
tree_->Branch("NeuEMEnergyJet",&NeuEMEnergyJet,"NeuEMEnergyJet[NTauCand]/F");
tree_->Branch("ChMultJet",&ChMultJet,"ChMultJet[NTauCand]/F");
tree_->Branch("NeuMultJet",&NeuMultJet,"NeuMultJet[NTauCand]/F");
tree_->Branch("MuMultJet",&MuMultJet,"MuMultJet[NTauCand]/F");
tree_->Branch("MaximumHCALPFClusterEt",&MaximumHCALPFClusterEt,"MaximumHCALPFClusterEt[NTauCand]/F");
tree_->Branch("Tau_chargeTrue",&Tau_chargeTrue,"Tau_chargeTrue[NTauCand]/F");
tree_->Branch("PhiT",&PhiT,"PhiT[NTauCand]/F");
tree_->Branch("TauSize",&TauSize,"TauSize/I");
tree_->Branch("Tau_ImpPar",&Tau_ImpPar,"Tau_ImpPar[NTauCand]/F");
tree_->Branch("TauNTr",&TauNTr,"TauNTr[NTauCand]/F");
 
tree_->Branch("pdgId_mothLT",pdgId_mothLT,"pdgId_mothLT[20]/F");
tree_->Branch("pdgId_candLT",pdgId_candLT,"pdgId_candLT[20]/F");
tree_->Branch("size_mothLT",size_mothLT,"size_mothLT[20]/F");

tree_->Branch("pdgId_mothM",pdgId_mothM,"pdgId_mothM[20]/F");
tree_->Branch("pdgId_candM",pdgId_candM,"pdgId_candM[20]/F");
tree_->Branch("size_mothM",size_mothM,"size_mothM[20]/F");


tree_->Branch("VisMassZ",VisMassZ,"VisMassZ[20][20]/F");
tree_->Branch("InvMassZ_A",InvMassZ_A,"InvMassZ_A[20][20]/F");
tree_->Branch("InvMassZ_B",InvMassZ_B,"InvMassZ_B[20][20]/F");
tree_->Branch("InvMassZ_C",InvMassZ_C,"InvMassZ_C[20][20]/F");
tree_->Branch("PTZ",PTZ,"PTZ[20][20]/F");
tree_->Branch("EnZ",EnZ,"EnZ[20][20]/F");
tree_->Branch("TrEnZ",TrEnZ,"TrEnZ[20][20]/F");
tree_->Branch("TrVisEnZ",TrVisEnZ,"TrVisEnZ[20][20]/F");
tree_->Branch("PXZ",PXZ,"PXZ[20][20]/F");
tree_->Branch("PYZ",PYZ,"PYZ[20][20]/F");
tree_->Branch("PZZ",PZZ,"PZZ[20][20]/F");
tree_->Branch("PZ",PZ,"PZ[20][20]/F");
tree_->Branch("TrMassZ_A",TrMassZ_A,"TrMassZ_A[20][20]/F");
tree_->Branch("TrMassZ_B",TrMassZ_B,"TrMassZ_B[20][20]/F");
tree_->Branch("TrMassZ_C",TrMassZ_C,"TrMassZ_C[20][20]/F");
tree_->Branch("TrVisMassZ",TrVisMassZ,"TrVisMassZ[20][20]/F");
tree_->Branch("TrVisMassZ_rel",TrVisMassZ_rel,"TrVisMassZ_rel[20][20]/F");
tree_->Branch("VisPXZ",VisPXZ,"VisPXZ[20][20]/F");
tree_->Branch("VisPYZ",VisPYZ,"VisPYZ[20][20]/F");
tree_->Branch("VisPZZ",VisPZZ,"VisPZZ[20][20]/F");
tree_->Branch("VisPZ",VisPZ,"VisPZ[20][20]/F");
 

 
tree_->Branch("DeltaphiTM",DeltaphiTM,"DeltaphiTM[20][20]/F");
tree_->Branch("CosDeltaPhiTM",CosDeltaPhiTM,"CosDeltaPhiTM[20][20]/F");
tree_->Branch("DeltaphiTauMET",&DeltaphiTauMET,"DeltaphiTauMET[NTauCand]/F");
tree_->Branch("CosDeltaPhiTauMET",&CosDeltaPhiTauMET,"CosDeltaPhiTauMET[NTauCand]/F");
tree_->Branch("DeltaphiMuMET",&DeltaphiMuMET,"DeltaphiMuMET[NMuCand]/F");
tree_->Branch("CosDeltaPhiMuMET",&CosDeltaPhiMuMET,"CosDeltaPhiMuMET[NMuCand]/F");



tree_->Branch("pthat",&pthat,"pthat/F");

tree_->Branch("sizeRM",&sizeRM,"sizeRM/I");
tree_->Branch("sizeT",&sizeT,"sizeT/I");

tree_->Branch("id_process",&id_process,"id_process/F");
tree_->Branch("weight",&weight,"/D");
tree_->Branch("AlpgenProcessID",&AlpgenProcessID,"AlpgenProcessID/F");
//tree_->Branch("",&,"/F");

//tree_->Branch("",&,"/F");

}


ZTauTauAnalysis::~ZTauTauAnalysis()
{
  std::cout<<"ZTauTauAnalysis:: endJob Processed  "<< eventCount_<<" events"<<std::endl;
  //delete theIsolation;

  //store the tree in the output file
  file_->Write();
  
  //closing the file deletes the tree
  file_->Close();

   tree_=0;  
  file_=0;

  delete tree_;
  delete file_; 
}

//
// member functions
//

// ------------ method called to for each event  ------------
void
ZTauTauAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
  
{  

  eventCount_++; 
  irun=iEvent.id().run();
  ievt=iEvent.id().event();
  cout << "==============================================" << endl
       << " Event: " << eventCount_ << endl
       << " Run # " << irun << " Event # " << ievt << endl
       << "==============================================" << endl;
   
  
  NPFJETMAX=20;
  NCALOJETMAX=20;
  NMUMAX=20;
  NTRACKMAX=30;
  NTAUMAX=20;
  MuSize=0;
  skip=false; 
  
  bool foundmufromtau=false;
  bool foundneumufromtau=false;
  bool foundneutaufromtau=false;
  
  bool foundZ = false;
  
  bool foundjetfromtaup=false;
  
  bool foundlepttaup=false;
  
  bool foundtaup=false;
  bool foundtaun=false;
  
  bool foundjetfromtaun=false;
  
  bool foundlepttaun=false;
  
  bool foundmufromtaup=false;
  bool foundneumufromtaup=false;
  bool foundneutaufromtaup=false;
  bool  notfoundelectronp=true;
  
  bool foundmufromtaun=false;
  bool foundneumufromtaun=false;
  bool foundneutaufromtaun=false;
  bool notfoundelectronn=true;
 
  //bool FlagPT15=false; 

  bool FlagNewMuIsol=false;
  //bool FlagRecoTau=false;
  // bool FlagRecoMuon=false;
  FlagRecoMET=false;
  QualityEvent=false;
 
  ///////////INIZ MUON

   selected_.clear();  
   isoValue=-1000;   
   NumMuons=0;

   OldIsoPTMu=-1000;
   OldIsoPXMu=-1000;
   OldIsoPYMu=-1000;
  OldIsoPZMu=-1000;
   OldIsoEnMu=-1000;
   OldIsoTrEnMu=-1000;
   OldIsoInvMassMu=-1000;
   OldIsoEtaMu=-1000;
   OldIsoTrMassMu=-1000;
   OldIsoImpParMu=-1000;
   OldIsoPhiMu=-1000;
   OldIsoChMu=-1000; 
   NumOldIsolMu=0; 

   NewIsoPTMu=-1000;
   NewIsoPXMu=-1000;
   NewIsoPYMu=-1000;
   NewIsoPZMu=-1000;
   NewIsoEnMu=-1000;
   NewIsoTrEnMu=-1000;
   NewIsoInvMassMu=-1000;
   NewIsoEtaMu=-1000;
   NewIsoTrMassMu=-1000;
   NewIsoImpParMu=-1000;
   NewIsoPhiMu=-1000;
   NewIsoChMu=-1000; 
   NumNewIsolMu=0; 


   NumIsolTracks=-10;
   PTSum=-1000.;
   NumGloMuons=0;
  
   pthat=-100;   

 

   
   CaloMETMissTrEner=-1000.;
   CaloMETSumMissTrEner=-1000.;
   CaloMET_x=-1000.;
   CaloMET_y=-1000.;
   CaloMET_time=-1000.;    
   CaloMETSignificance=-1000.;
   CaloMET_E_long=-1000.;
   phiMET=-1000.;
   
   PFRecoEnergyZ=-1000.;
   PFRecoTrEnergyZ=-1000.;
   PFRecoTrVisEnergyZ=-1000.;
   PFRecoP_TZ=-1000.;
   PFRecoP_XZ=-1000.;
   PFRecoP_YZ=-1000.;
   PFRecoP_ZZ=-1000.; 
   PFRecoVisP_XZ=-1000.;
   PFRecoVisP_YZ=-1000.;
   PFRecoVisP_ZZ=-1000.; 
   PFRecoVisMassZ=-1000.;
   PFRecoInvMassZ_A=-1000.;
   PFRecoInvMassZ_B=-1000.;
   PFRecoInvMassZ_C=-1000.;
   PFRecoTrMassZ_A=-1000.;
   PFRecoTrMassZ_B=-1000.;
   PFRecoTrMassZ_C=-1000.;
   PFRecoTrVisMassZ=-1000.;
   PFRecoTrVisMassZ_rel=-1000.; 

   dr=1000;
   dr_a=-1000;
   deltaphiTauMET=-1000.;
   cosDeltaPhiTauMET=-1000.;
   deltaphiMuMET=-1000.;
   cosDeltaPhiMuMET=-1000.;
   deltaphiTM=-1000;
   cosDeltaPhiTM=-1000;
   

   tau_disc=-1000;
   phiT=-1000.;  
   tau_et=-1000;
   tau_eta=-1000.;
   tau_leading_trk_pt=-1000.;
   tau_had_energy=-1000.;
   tau_inv_mass=-1000.;          
   tau_inv_mass2=-1000.;           
   tau_sumpt_iso_ch_hadrons=-1000.;
   tau_sumpt_iso_photons=-1000.;
   leading_trk_sipt=-1000.;
   chHadEnergyJet=-1000.;
   tau_pt=-1000.;
   tau_px=-1000.;
   tau_py=-1000.;
   tau_pz=-1000.;
   tau_energy=-1000.;
  
   tau_ch_hadrons=-1000;              
   tau_neu_hadrons=-1000;           
   tau_photons=-1000;                 
   tau_sig_ch_hadrons=-1000;          
   tau_sig_photons=-1000;             
   tau_sig_neu_hadrons=-1000;        
   tau_iso_ch_hadrons=-1000;         
   tau_iso_photons =-1000;           
   tau_iso_neu_hadrons=-1000;
   n_tau_candidate=-1000;
   TauImpPar=-1000;

   tau_em_energy=-1000; 
   neuHadEnergyJet=-1000;
   chEMEnergyJet=-1000;
   chMuEnergyJet=-1000; 
   neuEMEnergyJet=-1000;
   chMultJet=-1000;
   neuMultJet=-1000;
   muMultJet=-1000;
   
   b_discSM=0;
   b_discSMNoIP=-1000;
   b_discTCHE=-1000;
 b_discTCHP=-1000;
   b_discJP=-1000;
   b_discSL=-1000;
   BDiscSumSM=-1000;
BDiscSumSMNoIP=-1000;
BDiscMeanSL=-1000;
BDiscSumTCHE=-1000;
BDiscSumTCHP=-1000;
BDiscSumJP=-1000;

 nJetBSM=0;
 enerTrBjetSM1=-1000;
 enerTrBjetSM2=-1000;
 enerTrBjetSM3=-1000;
 enerTrBjetSM4=-1000;
 b_discSM1=-1000;
 b_discSM2=-1000;
 b_discSM3=-1000;
 b_discSM4=-1000;

   
 nJetBSMNoIP=0;
 enerTrBjetSMNoIP1=-1000;
 enerTrBjetSMNoIP2=-1000;
 enerTrBjetSMNoIP3=-1000;
 enerTrBjetSMNoIP4=-1000;
 b_discSMNoIP1=-1000;
 b_discSMNoIP2=-1000;
 b_discSMNoIP3=-1000;
 b_discSMNoIP4=-1000;

 nJetBJP=0;
 enerTrBjetJP1=-1000;
 enerTrBjetJP2=-1000;
 enerTrBjetJP3=-1000;
 enerTrBjetJP4=-1000;
 b_discJP1=-1000;
 b_discJP2=-1000;
 b_discJP3=-1000;
 b_discJP4=-1000;

nJetBTCHE=0;
 enerTrBjetTCHE1=-1000;
 enerTrBjetTCHE2=-1000;
 enerTrBjetTCHE3=-1000;
 enerTrBjetTCHE4=-1000;
 b_discTCHE1=-1000;
 b_discTCHE2=-1000;
 b_discTCHE3=-1000;
 b_discTCHE4=-1000;

nJetBTCHP=0;
 enerTrBjetTCHP1=-1000;
 enerTrBjetTCHP2=-1000;
 enerTrBjetTCHP3=-1000;
 enerTrBjetTCHP4=-1000;
 b_discTCHP1=-1000;
 b_discTCHP2=-1000;
 b_discTCHP3=-1000;
 b_discTCHP4=-1000;



 tau_zvertex=-1000.;
 tau_charge=-1000.; 
 TauCharge=-1000;
 tau_chargeTrue=-1000;

   
   NumRecHitsMin=1000;
   NumRecHitsMax=-1000;
   NumValRecHitsMin=1000;
   NumValRecHitsMax=-1000;
   NumTracks_8=0;
   NumTracks_10=0;
   SumTrackPt8=0;
   SumTrackPt10=0;

 
   PFJetET=-1000.;
   nPFjet = 0;


   pdgId_cand=-1000;
   pdgId_moth=-1000;
   size_moth=-10;
   //MuTrEnH=-1000;
   //MuTrEnE=-1000;
   //MuPtT=-1000;



 drTB=-1000;
 drMB=-1000;

 DRMINTBSM=1000;
 DRMAXMBSM=-1000;

 drTBSM1=-1000;
 drTBSM2=-1000;
 drTBSM3=-1000;
 drTBSM4=-1000;

 b_discTSM=-1000;
 b_discPosFirstSM=b_discPosSecondSM=b_discPosThirdSM=b_discPosFourSM=-1000;


 DRMINTBSMNoIP=1000;
 DRMINMBSMNoIP=-1000;

 drTBSMNoIP1=-1000;
 drTBSMNoIP2=-1000;
 drTBSMNoIP3=-1000;
 drTBSMNoIP4=-1000;

 b_discTSMNoIP=-1000;
 b_discPosFirstSMNoIP=b_discPosSecondSMNoIP=b_discPosThirdSMNoIP=b_discPosFourSMNoIP=-1000;


 DRMINTBJP=1000;
 DRMINMBJP=-1000;

 drTBJP1=-1000;
 drTBJP2=-1000;
 drTBJP3=-1000;
 drTBJP4=-1000;

 b_discTJP=-1000;
 b_discPosFirstJP=b_discPosSecondJP=b_discPosThirdJP=b_discPosFourJP=-1000;

 DRMINTBTCHE=1000;
 DRMINMBTCHE=-1000;

 drTBTCHE1=-1000;
 drTBTCHE2=-1000;
 drTBTCHE3=-1000;
 drTBTCHE4=-1000;

 b_discTTCHE=-1000;
 b_discPosFirstTCHE=b_discPosSecondTCHE=b_discPosThirdTCHE=b_discPosFourTCHE=-1000;

 DRMINTBTCHP=1000;
 DRMINMBTCHP=-1000;

 drTBTCHP1=-1000;
 drTBTCHP2=-1000;
 drTBTCHP3=-1000;
 drTBTCHP4=-1000;

 b_discTTCHP=-1000;
 b_discPosFirstTCHP=b_discPosSecondTCHP=b_discPosThirdTCHP=b_discPosFourTCHP=-1000;

 float drTBMintmpSM=-1000; 
 drMB1tmpSM=drMB1SM=drMB2tmpSM=drMB2SM=drMB3tmpSM=drMB3SM=drMB4tmpSM=drMB4SM=1111;
 bdiscM1SM=bdiscM2SM=bdiscM3SM=bdiscM4SM=-999; 
 
 Njet10ITC5=Njet15ITC5=Njet20ITC5=nJetITC5=0;
 Njet10SM= Njet15SM=Njet20SM=Njet10SMNoIP=Njet15SMNoIP=Njet20SMNoIP=Njet10JP=Njet15JP=Njet20JP=Njet10TCHE=Njet15TCHE=Njet20TCHE=Njet10TCHP=Njet15TCHP=Njet20TCHP=0;
 
 njetITC5=njet15ITC5=0;
 PT1= PT2=PT3=PT4=-1000;
 nBjetMSM=0;
 PFTau Tau;
 sizeRM=sizeT=-100;
 
 id_process=-1;
 AlpgenProcessID=-666;
 weight=-0.9999;
 //delpx=delpy=delpz=delsumet=EcalM_Tot=HcalM_Tot=HoM_Tot=TrkM_Tot=-1000;
 //HcalM_Frac25=HoM_Frac25=TrkM_Frac25=TrkIsoM_N25=EcalM_Frac25=mhcalfrac25=mtrackfrac25=mhofrac25=mecalfrac25=calfrac25=-1000;
 HcalM_Frac40=HoM_Frac40=TrkM_Frac40=TrkIsoM_N40=EcalM_Frac40=mhcalfrac40=mtrackfrac40=mhofrac40=mecalfrac40=calfrac40=-1000;


 for(int l=0; l<20;l++){
   for(int m=0;m<20;m++){
     SMCaloJet_DrM[l][m]=-999;
     SMCaloJet_DPhiM[l][m]=-999;
     
     SMNoIPCaloJet_DrM[l][m]=-999;
     SMNoIPCaloJet_DPhiM[l][m]=-999;
     
     JPCaloJet_DrM[l][m]=-999;
     JPCaloJet_DPhiM[l][m]=-999;
     
     TCHECaloJet_DrM[l][m]=-999;
     TCHECaloJet_DPhiM[l][m]=-999;
     
     TCHPCaloJet_DrM[l][m]=-999;
     TCHPCaloJet_DPhiM[l][m]=-999;
     
     VisMassZ[l][m]=-999;
     InvMassZ_A[l][m]=-999;
     InvMassZ_B[l][m]=-999;
     InvMassZ_C[l][m]=-999;
     PTZ[l][m]=-999;
     EnZ[l][m]=-999;
     TrEnZ[l][m]=-999;
     TrVisEnZ[l][m]=-999;
     VisEnZ[l][m]=-999;
     PXZ[l][m]=-999;
     PYZ[l][m]=-999;
     PZZ[l][m]=-999;
     PZ[l][m]=-999;
     TrMassZ_A[l][m]=-999;
     TrMassZ_B[l][m]=-999;
     TrMassZ_C[l][m]=-999;
     TrVisMassZ[l][m]=-999; TrVisMassZ_rel[l][m]=-999; VisPXZ[l][m]=-999; VisPYZ[l][m]=-999; VisPZZ[l][m]=-999; VisPZ[l][m]=-999; 

     DeltaphiTM[l][m]=-999; CosDeltaPhiTM[l][m]=-999; 
   }
   for(int n=0;n<5;n++){
     
     EcalM_Frac[l][n]=-999;
     HcalM_Frac[l][n]=-999;
      HoM_Frac[l][n]=-999;
      TrkM_Frac[l][n]=-999;
      TrkIsoM_N[l][n]=-999;
   }
   
   
 
    Muon_px[l]=-999;
      Muon_py[l]=-999;
      Muon_pz[l]=-999;
      Muon_pt[l]=-999;
       Muon_p[l]=-999;
       Muon_en[l]=-999;
       Muon_tren[l]=-999;
       Muon_eta[l]=-999;
       Muon_phi[l]=-999;
       Muon_theta[l]=-999;
       Muon_charge[l]=-999;
      
       MuonDz[l]=-999;
       MuonIP[l]=-999;
       Muonvtx[l]=-999;
       Muonvty[l]=-999;
       Muonvtz[l]=-999;


EcalM_Tot[l]=-999;
HcalM_Tot[l]=-999;
HoM_Tot[l]=-999;
TrkM_Tot[l]=-999;




 Tau_disc[l]=-999; 
Tau_eta[l] =-999;
Tau_leading_trk_pt[l]=-999; 
Tau_had_energy[l]=-999; 
Tau_inv_mass[l]=-999; 
Tau_sumpt_iso_ch_hadrons[l] =-999;
Tau_sumpt_iso_photons[l]=-999; 
Leading_trk_sipt[l]=-999;  
Tau_inv_mass2[l]=-999; 
ChHadEnergyJet[l]=-999;
 Tau_ch_hadrons[l]=-999; 
Tau_neu_hadrons[l] =-999;
Tau_photons[l] =-999;
Tau_sig_ch_hadrons[l] =-999;
Tau_iso_ch_hadrons[l] =-999;
Tau_iso_photons[l] =-999;
Tau_iso_neu_hadrons[l]=-999; 
Tau_sig_photons[l] =-999;
Tau_sig_neu_hadrons[l]=-999;       
Tau_pt[l]=-999; 
Tau_px[l]=-999; 
Tau_py[l]=-999; 
Tau_pz[l] =-999;
Tau_energy[l] =-999;
Tau_et[l]=-999;
Tau_Charge[l]=-999;
Tau_zvertex[l] =-999;
Tau_charge[l]=-999; 
Tau_em_energy[l] =-999;
NeuHadEnergyJet[l]=-999;  
ChEMEnergyJet[l]=-999; 
ChMuEnergyJet[l] =-999;
NeuEMEnergyJet[l] =-999;
ChMultJet[l] =-999;
NeuMultJet[l]=-999;
MuMultJet[l]=-999;
MaximumHCALPFClusterEt[l]=-999; 
Tau_chargeTrue[l] =-999;
PhiT[l]=-999;
 TauNTr[l]=-999;

DeltaphiTauMET[l]=-999; CosDeltaPhiTauMET[l]=-999; DeltaphiMuMET[l]=-999;CosDeltaPhiMuMET[l]=-999;

pdgId_mothLT[l]=-999;
pdgId_candLT[l]=-999;
size_mothLT[l]=-999;

pdgId_mothM[l]=-999;
pdgId_candM[l]=-999;
size_mothM[l]=-999;
 }
 
 

 //Muon_pt[15]=-1000;
// Muon_pt[NMuCand]=Muon_eta[NMuCand]=Muon_p[NMuCand]=Muon_px[NMuCand]=Muon_py[NMuCand]=Muon_pz[NMuCand]=Muon_phi[NMuCand]=Muon_theta[NMuCand]=Muon_charge[NMuCand]=MuonDz[NMuCand]=MuonIP[NMuCand]=Muonvty[NMuCand]=Muonvtx[NMuCand]=Muonvtz[NMuCand]=-1000;

 // SMCaloJet_DrM[NCaloJetCandSM]=-666;
 //SMCaloJet_DPhiM[NCaloJetCandSM]=-666;





 //////////////    zuppe

Handle< HepMCProduct > mc;
  iEvent.getByLabel( "source", mc );
  const GenEvent * genEvt = mc->GetEvent();
    
  double processID = genEvt->signal_process_id();
  double pthat = genEvt->event_scale(); 

  cout<<"processID "<<processID<<" pthat "<< pthat<<endl;


// Handle< GenInfoProduct > gi;
//   iEvent.getRun().getByLabel( src_, gi);
//   double auto_cross_section = gi->cross_section(); // is automatically calculated at the end of each RUN --  units in nb
//   double external_cross_section = gi->external_cross_section(); // is the precalculated one written in the cfg file -- units is pb
//   double filter_eff = gi->filter_efficiency();

//   cout<<"auto_cross_section "<<auto_cross_section<<"external_cross_section "<<external_cross_section<<"filter_eff "<<filter_eff<<endl;




Handle< double> weightHandle;
iEvent.getByLabel ("csaweightproducer","weight", weightHandle);
 weight = * weightHandle;

Handle< int> AlpgenProcessIDHandle;
iEvent.getByLabel ("csaweightproducer","AlpgenProcessID", AlpgenProcessIDHandle);
AlpgenProcessID = * AlpgenProcessIDHandle;

 cout<<"weight "<<weight<<" AlpgenProcessID "<<AlpgenProcessID <<endl;

 Handle< int> genProcessID;
 iEvent.getByLabel ("genEventProcID", genProcessID);
 processID=*genProcessID;

 cout<<"processID "<<processID<<endl;

 //Handle <CandidateCollection> genParticles;
 //iEvent.getByLabel("genParticleCandidates",genParticles);


//  for (size_t i=0;i<genParticles->size();++i){
//    const Candidate &p=(*genParticles)[i];
//    int id=p.pdgId();
//    int st=p.status();

//  if(st==3 && (id==24||id==-24)){
//      id_process=0;
//      cout<<" KKLL found W+jets "<<endl;
//      //i=genParticles->size()-1;
// }



//  if(st==3 && (id==23||id==-23)){
//      id_process=1;
//      cout<<" KKLL found Z+jets "<<endl;
// //i=genParticles->size()-1;
// }

//  if(st==3 && (id==6||id==-6)){
//      id_process=2;
//      cout<<" KKLL found TTBAR "<<endl;
// //i=genParticles->size()-1;

// }

// }

  //////////////////////////////


//  edm::Handle<HepMCProduct> evt;

//   iEvent.getByLabel("source", evt);
  
//   HepMC::GenEvent *generated_event = new HepMC::GenEvent(*(evt->GetEvent()));
//   HepMC::GenEvent::particle_iterator p;
// pthat=(*generated_event).event_scale(); 
//   for (p = generated_event->particles_begin(); p!=generated_event->particles_end(); p++)
//     {
      
//       if ( ((*p) ->pdg_id()== 23) && ((*p)->status() == 3) )
//   	{
//   	  foundZ = true;
	  
//   	  if ((*p)->end_vertex()->particles_out_size()>2) 
	    
// 	    {
	      
// 	      muener=-100.,muenertr=-100.,mupt=-100.,mueta=-100.,mumass=-100.,mumasstr=-100.,mupx=-100.,mupy=-100.,mupz=-100., mutheta=-100.;
	      
// 	      neumuener=-100.,neumuenertr=-100.,neumupx=-100.,neumupy=-100.,neumupz=-100.,neumupt=-100.,neutaupt=-100.;
// 	      neutauener=-100.,neutauenertr=-100.,neutaupx=-100.,neutaupy=-100.,neutaupz=-100.;
	      
// 	      taulener_ap=-100.,taulenertr_ap=-100.,taulpt_ap=-100.,tauleta_ap=-100.,taulmass_ap=-100.,taulmasstr_ap=-100.;
	      
// 	      tauhener_ap=-100.,tauhenertr_ap=-100.,tauhpt_ap=-100.,tauheta_ap=-100.,tauhmass_ap=-100.,tauhmasstr_ap=-100.;
	      
// 	      jetMomener=-100.,jetMomenertr=-100.,jetMompt=-100.,jetMometa=-100.,jetMommass=-100.,jetMommasstr=-100., jetMomtheta=-100.;
// 	      jetMompx=-100.,jetMompy=-100.,jetMompz=-100.;
	      

// 	      Z_eta=-100.,Z_rap=-100.,Z_rapidity=-100.;
	   
	      
// 	      tauneutener=-100.,tauneutenertr=-100.,tauneutpx=-100.,tauneutpy=-100.,tauneutpz=-100.,tauneutpt=-100.;
	   
	      
// 	      etaul_bp=-100.,pxtaul_bp=-100.,pytaul_bp=-100.,pztaul_bp=-100.;
// 	      etauh_bp=-100.,pxtauh_bp=-100.,pytauh_bp=-100.,pztauh_bp=-100.;
// 	      etaul_ap=-100.,pxtaul_ap=-100.,pytaul_ap=-100.,pztaul_ap=-100.;
// 	      etauh_ap=-100.,pxtauh_ap=-100.,pytauh_ap=-100.,pztauh_ap=-100.;
	      
// 		Zeta_fin_eta=-100.,  Zeta_fin_rapidity=-100.;
	      
// 	      Zvisibletransversemass_relat2=-100.;
	      
// 	      trvismass1=-100.;
// 	      //trvismass2=-100.,trvismass3=-100.,trvismass4=-100. ;
// 	      tauh_aptheta=-100., taul_aptheta=-100;
// 	      //leptpx=-100.,leptpy=-100., leptpz=-100., leptener=-100., leptenertr=-100.;
// 	      //adrpx=-100., adrpy=-100., adrpz=-100.,adrener=-100., adrenertr=-100.;
// 	      //energiaZ=-100., energiatrZ=-100.,pxZ=-100., pyZ=-100., pzZ=-100., ptZ=-100.;
// 	      GenEnergyZ=-100., GenTrEnergyZ=-100., GenVisTrEnergyZ=-100.,GenP_XZ=-100., GenP_YZ=-100., GenP_ZZ=-500., GenP_TZ=-100., GenInvMassZ=-100., GenTrMassZ_rel=-100., GenTrMassZ_A=-100.; 
// 		//GenTrMassZ_B=-100., 
// 		GenVisMassZ=-100., GenVisTrMassZ_A=-100.;
// 	      //GenVisTrMassZ_B=-100., 
// 	      GenVisTrMassZ_rel=-100., GenVisEnergyZ=-100.;
// 	      // energiatrasvvisZ=-100.;
	      
// for( HepMC::GenVertex::particle_iterator aDaughter=(*p)->end_vertex()->particles_begin(HepMC::descendants); aDaughter !=(*p)->end_vertex()->particles_end(HepMC::descendants);aDaughter++)
		
// 		{ 
		  
// 		  ///////////////////////////////////////////////////////////////////
		  
// 		  if( (*aDaughter)->pdg_id()==15 ) 
// 		    {     
		      
// 		      //vector<HepMC::GenParticle*> bychildren = (*aDaughter)->listChildren();
// 		      // vector<HepMC::GenParticle*>::const_iterator byaDaughter;  
// 		      //for (byaDaughter = bychildren.begin();byaDaughter != bychildren.end();byaDaughter++)
		      
// 		      for( HepMC::GenVertex::particle_iterator byaDaughter=(*aDaughter)->end_vertex()->particles_begin(HepMC::descendants); byaDaughter !=(*aDaughter)->end_vertex()->particles_end(HepMC::descendants);byaDaughter++)		  
// 			{
			  
// 			  if ((*byaDaughter)->status()==2 && (*byaDaughter)->pdg_id()==15) 
// 			    {  
// 			      foundtaup=true;
			      
// 			      //  vector<HepMC::GenParticle*> tauchildren = (*byaDaughter)->listChildren();
// 			      // vector<HepMC::GenParticle*>::const_iterator tauDaughter;			    
// 			      // for (tauDaughter = tauchildren.begin();tauDaughter != tauchildren.end();tauDaughter++)
			      
// 			      for( HepMC::GenVertex::particle_iterator tauDaughter=(*byaDaughter)->end_vertex()->particles_begin(HepMC::descendants); tauDaughter !=(*byaDaughter)->end_vertex()->particles_end(HepMC::descendants);tauDaughter++)
				
// 				{
// 				  if ( (*tauDaughter)->pdg_id()==13) 
// 				    {     
// 				      foundmufromtaup=true;
				      
// 				      mu.SetPxPyPzE((*tauDaughter)->momentum().px(),(*tauDaughter)->momentum().py(),
// 						    (*tauDaughter)->momentum().pz(),(*tauDaughter)->momentum().e());
				      
// 				      muener=mu.E();
// 				      mupx=mu.Px();
// 				      mupy=mu.Py();
// 				      mupz=mu.Pz();
// 				      muenertr=mu.Et();
// 				      mupt=mu.Pt();
// 				      mueta=mu.Eta();
// 				      mutheta=mu.Theta();
// 				      mumass=mu.M();
// 				      mumasstr=sqrt((muenertr)*(muenertr)-(mupt)*(mupt));
// 				      barcodemulept=abs((*tauDaughter)->barcode());
// 				    }
// 				  else if ( (*tauDaughter)->pdg_id()==-14) 
// 				    {
				      
// 				      neumu.SetPxPyPzE((*tauDaughter)->momentum().px(),
// 						       (*tauDaughter)->momentum().py(),(*tauDaughter)->momentum().pz(),
// 						       (*tauDaughter)->momentum().e());
				      
// 				      neumuener=neumu.E();
// 				      neumuenertr=neumu.Et();
// 				      neumupx=neumu.Px();
// 				      neumupy=neumu.Py();
// 				      neumupz=neumu.Pz();
// 				      neumupt=neumu.Pt();
				      
// 				      foundneumufromtaup=true;
// 				    }
				  
// 				  else if ( (*tauDaughter)->pdg_id()==16)
// 				    { 
// 				      foundneutaufromtaup=true;
// 				    }      
// 				  else if((*tauDaughter)->pdg_id()==11)
// 				    { 
// 				      notfoundelectronp=false;
// 				    } 
				  
// 				  if (foundneutaufromtaup && foundneumufromtaup && foundmufromtaup)
// 				    {  
// 				      foundlepttaup=true;
	     
// 				      if ( (*tauDaughter)->pdg_id()==16){ 
					
// 					neutau.SetPxPyPzE((*tauDaughter)->momentum().px(),
// 							  (*tauDaughter)->momentum().py(),(*tauDaughter)->momentum().pz(),
// 							  (*tauDaughter)->momentum().e());
					
// 					neutauener=neutau.E();
// 					neutauenertr=neutau.Et();
// 					neutaupx=neutau.Px();
// 					neutaupy=neutau.Py();
// 					neutaupz=neutau.Pz();
// 					neutaupt=neutau.Pt();
// 				      }
				      
// 				      taul_ap.SetPxPyPzE((*byaDaughter)->momentum().px(),
// 							 (*byaDaughter)->momentum().py(),(*byaDaughter)->momentum().pz(),
// 							 (*byaDaughter)->momentum().e());
				      
// 				      taulener_ap=taul_ap.E();
// 				      taulenertr_ap=taul_ap.Et();
// 				      taulpt_ap=taul_ap.Pt();
// 				      tauleta_ap=taul_ap.Eta();
// 				      taulmass_ap=taul_ap.M();
// 				      taulmasstr_ap=sqrt((taulenertr_ap)*(taulenertr_ap)-(taulpt_ap)*(taulpt_ap));
				      
// 				      pxtaul_bp=(*aDaughter)->momentum().px();
// 				      pytaul_bp=(*aDaughter)->momentum().py();
// 				      pztaul_bp=(*aDaughter)->momentum().pz();
// 				      etaul_bp=(*aDaughter)->momentum().e();
				      
// 				      pxtaul_ap=(*byaDaughter)->momentum().px();
// 				      pytaul_ap=(*byaDaughter)->momentum().py();
// 				      pztaul_ap=(*byaDaughter)->momentum().pz();
// 				      etaul_ap=(*byaDaughter)->momentum().e();  		      
				      
// 				    }		    
				  
// 				  else if ((foundneutaufromtaup==1)&&(foundmufromtaup==0)&&(notfoundelectronp==1)) 
// 				    {
// 				      foundjetfromtaup=true;
				      
// 				      if ( (*tauDaughter)->pdg_id()==16)
// 					{  
// 					  tauneut.SetPxPyPzE((*tauDaughter)->momentum().px(),
// 							     (*tauDaughter)->momentum().py(),(*tauDaughter)->momentum().pz(),
// 							     (*tauDaughter)->momentum().e());
					  
// 					  tauneutener=tauneut.E();
// 					  tauneutenertr=tauneut.Et();
// 					  tauneutpx=tauneut.Px();
// 					  tauneutpy=tauneut.Py();
// 					  tauneutpz=tauneut.Pz();
// 					  tauneutpt=tauneut.Pt();
// 					}
				      
// 				      pxtauh_bp=(*aDaughter)->momentum().px();
// 				      pytauh_bp=(*aDaughter)->momentum().py();
// 				      pztauh_bp=(*aDaughter)->momentum().pz();
// 				      etauh_bp=(*aDaughter)->momentum().e();
				      
// 				      pxtauh_ap=(*byaDaughter)->momentum().px();
// 				      pytauh_ap=(*byaDaughter)->momentum().py();
// 				      pztauh_ap=(*byaDaughter)->momentum().pz();
// 				      etauh_ap=(*byaDaughter)->momentum().e();
				      
// 				      tauh_ap.SetPxPyPzE((*byaDaughter)->momentum().px(),
// 							 (*byaDaughter)->momentum().py(),(*byaDaughter)->momentum().pz(),
// 							 (*byaDaughter)->momentum().e());				      			 
// 				      tauhener_ap=tauh_ap.E();
// 				      tauhenertr_ap=tauh_ap.Et();
// 				      tauhpt_ap=tauh_ap.Pt();
// 				      tauheta_ap=tauh_ap.Eta();
// 				      tauhmass_ap=tauh_ap.M();
// 				      tauhmasstr_ap=sqrt((tauhenertr_ap)*(tauhenertr_ap)-(tauhpt_ap)*(tauhpt_ap));
				      
// 				      jetMom=tauh_ap-tauneut; 
// 				      jetMomener=jetMom.E();
// 				      jetMompx=jetMom.Px();
// 				      jetMompy=jetMom.Py();
// 				      jetMompz=jetMom.Pz();
// 				      jetMomenertr=jetMom.Et();
// 				      jetMompt=jetMom.Pt();
// 				      jetMometa=jetMom.Eta();
// 				      jetMomtheta=jetMom.Theta();
// 				      jetMommass=sqrt((jetMomener)*(jetMomener)-(jetMompx)*(jetMompx)-(jetMompy)*(jetMompy)-(jetMompz)*(jetMompz));
// 				      jetMommasstr=sqrt((jetMomenertr)*(jetMomenertr)-(jetMompt)*(jetMompt));
				      
// 				    }  
// 				}
// 			    }//if byaD
// 			}//for byaD
// 		    }//if aD
		  
// 		  //////////////////////////////////////////////////////////////////////////////////////////////////////
		  
// 		  if( (*aDaughter)->pdg_id()==-15 ) 
// 		    {     
		      
// 		      //vector<HepMC::GenParticle*> bychildren = (*aDaughter)->listChildren();
// 		      //vector<HepMC::GenParticle*>::const_iterator byaDaughter;
// 		      //for (byaDaughter = bychildren.begin();byaDaughter != bychildren.end();byaDaughter++)
		      
// 		      for( HepMC::GenVertex::particle_iterator byaDaughter=(*aDaughter)->end_vertex()->particles_begin(HepMC::descendants); byaDaughter !=(*aDaughter)->end_vertex()->particles_end(HepMC::descendants);byaDaughter++)
// 			{
	  
// 			  if ((*byaDaughter)->status()==2 && (*byaDaughter)->pdg_id()==-15) 
// 			    {  
// 			      foundtaun=true;
			      
// 			      //vector<HepMC::GenParticle*> tauchildren = (*byaDaughter)->listChildren();
// 			      //vector<HepMC::GenParticle*>::const_iterator tauDaughter;    
// 			      //for (tauDaughter = tauchildren.begin();tauDaughter != tauchildren.end();tauDaughter++)
			      
// 			      for( HepMC::GenVertex::particle_iterator tauDaughter=(*byaDaughter)->end_vertex()->particles_begin(HepMC::descendants); tauDaughter !=(*byaDaughter)->end_vertex()->particles_end(HepMC::descendants);tauDaughter++)		
// 				{
// 				  if ( (*tauDaughter)->pdg_id()==-13) 
// 				    {     
// 				      foundmufromtaun=true;
				      
// 				      mu.SetPxPyPzE((*tauDaughter)->momentum().px(),(*tauDaughter)->momentum().py(),
// 						    (*tauDaughter)->momentum().pz(),(*tauDaughter)->momentum().e());
				      
// 				      muener=mu.E();
// 				      mupx=mu.Px();
// 				      mupy=mu.Py();
// 				      mupz=mu.Pz();
// 				      muenertr=mu.Et();
// 				      mupt=mu.Pt();
// 				      mueta=mu.Eta();
// 				      mumass=mu.M();
// 				      mutheta=mu.Theta();
// 				      mumasstr=sqrt((muenertr)*(muenertr)-(mupt)*(mupt));
// 				      barcodemulept=abs((*tauDaughter)->barcode());
// 				    }
// 				  else if ( (*tauDaughter)->pdg_id()==14) 
// 				    {
				      
// 				      neumu.SetPxPyPzE((*tauDaughter)->momentum().px(),
// 						       (*tauDaughter)->momentum().py(),(*tauDaughter)->momentum().pz(),
// 						       (*tauDaughter)->momentum().e());
				      
// 				      neumuener=neumu.E();
// 				      neumuenertr=neumu.Et();
// 				      neumupx=neumu.Px();
// 				      neumupy=neumu.Py();
// 				      neumupz=neumu.Pz();
// 				      neumupt=neumu.Pt();
				     
// 				      foundneumufromtaun=true;
// 				    }
				  
// 				  else if ( (*tauDaughter)->pdg_id()==-16)
// 				    { 
// 				      foundneutaufromtaun=true;
// 				    }      
// 				  else if((*tauDaughter)->pdg_id()==-11)
// 				    { 
// 				      notfoundelectronn=false;
// 				    } 
				  
// 				  if (foundneutaufromtau && foundneumufromtau && foundmufromtau)
// 				    {
     
// 				      foundlepttaun=true;
	 
// 				      if ( (*tauDaughter)->pdg_id()==-16){ 
					
// 					neutau.SetPxPyPzE((*tauDaughter)->momentum().px(),
// 							  (*tauDaughter)->momentum().py(),(*tauDaughter)->momentum().pz(),
// 							  (*tauDaughter)->momentum().e());
					
// 					neutauener=neutau.E();
// 					neutauenertr=neutau.Et();
// 					neutaupx=neutau.Px();
// 					neutaupy=neutau.Py();
// 					neutaupz=neutau.Pz();
// 					neutaupt=neutau.Pt();
// 				      }
				      
// 				      taul_ap.SetPxPyPzE((*byaDaughter)->momentum().px(),
// 							 (*byaDaughter)->momentum().py(),(*byaDaughter)->momentum().pz(),
// 							 (*byaDaughter)->momentum().e());
				      
// 				      taulener_ap=taul_ap.E();
// 				      taulenertr_ap=taul_ap.Et();
// 				      taulpt_ap=taul_ap.Pt();
// 				      tauleta_ap=taul_ap.Eta();
// 				      taulmass_ap=taul_ap.M();
// 				      taulmasstr_ap=sqrt((taulenertr_ap)*(taulenertr_ap)-(taulpt_ap)*(taulpt_ap));
				      
// 				      pxtaul_bp=(*aDaughter)->momentum().px();
// 				      pytaul_bp=(*aDaughter)->momentum().py();
// 				      pztaul_bp=(*aDaughter)->momentum().pz();
// 				      etaul_bp=(*aDaughter)->momentum().e();
				      
// 				      pxtaul_ap=(*byaDaughter)->momentum().px();
// 				      pytaul_ap=(*byaDaughter)->momentum().py();
// 				      pztaul_ap=(*byaDaughter)->momentum().pz();
// 				      etaul_ap=(*byaDaughter)->momentum().e();  
// 				    }		    
				  
// 				  else if ((foundneutaufromtaun==1)&&(foundmufromtaun==0)&&(notfoundelectronn==1)) 
// 				    {
				   
// 				      foundjetfromtaun=true;
				      
// 				      if ( (*tauDaughter)->pdg_id()==-16)
// 					{  
// 					  tauneut.SetPxPyPzE((*tauDaughter)->momentum().px(),
// 							     (*tauDaughter)->momentum().py(),(*tauDaughter)->momentum().pz(),
// 							     (*tauDaughter)->momentum().e());
					  
// 					  tauneutener=tauneut.E();
// 					  tauneutenertr=tauneut.Et();
// 					  tauneutpx=tauneut.Px();
// 					  tauneutpy=tauneut.Py();
// 					  tauneutpz=tauneut.Pz();
// 					  tauneutpt=tauneut.Pt();
// 					}
				      
// 				      pxtauh_bp=(*aDaughter)->momentum().px();
// 				      pytauh_bp=(*aDaughter)->momentum().py();
// 				      pztauh_bp=(*aDaughter)->momentum().pz();
// 				      etauh_bp=(*aDaughter)->momentum().e();
				      
// 				      pxtauh_ap=(*byaDaughter)->momentum().px();
// 				      pytauh_ap=(*byaDaughter)->momentum().py();
// 				      pztauh_ap=(*byaDaughter)->momentum().pz();
// 				      etauh_ap=(*byaDaughter)->momentum().e();
				      
// 				      tauh_ap.SetPxPyPzE((*byaDaughter)->momentum().px(),
// 							 (*byaDaughter)->momentum().py(),(*byaDaughter)->momentum().pz(),
// 							 (*byaDaughter)->momentum().e());				      			 
// 				      tauhener_ap=tauh_ap.E();
// 				      tauhenertr_ap=tauh_ap.Et();
// 				      tauhpt_ap=tauh_ap.Pt();
// 				      tauheta_ap=tauh_ap.Eta();
// 				      tauhmass_ap=tauh_ap.M();
// 				      tauhmasstr_ap=sqrt((tauhenertr_ap)*(tauhenertr_ap)-(tauhpt_ap)*(tauhpt_ap));
				      
// 				      jetMom=tauh_ap-tauneut; 
// 				      jetMomener=jetMom.E();
// 				      jetMompx=jetMom.Px();
// 				      jetMompy=jetMom.Py();
// 				      jetMompz=jetMom.Pz();
// 				      jetMomenertr=jetMom.Et();
// 				      jetMompt=jetMom.Pt();
// 				      jetMometa=jetMom.Eta();
// 				      jetMomtheta=jetMom.Theta();
// 				      jetMommass=sqrt((jetMomener)*(jetMomener)-(jetMompx)*(jetMompx)-(jetMompy)*(jetMompy)-(jetMompz)*(jetMompz));
// 				      jetMommasstr=sqrt((jetMomenertr)*(jetMomenertr)-(jetMompt)*(jetMompt));
// 				    }  
// 				}
// 			    }//if byaD
			  
// 			}//for byaD
// 		    }//if aD
// 		  ///////////////////////////////////////////////////////////////////////////////////
 
// 		} //for aD
	      
// 	      if (foundZ==1&&foundtaup==1&&foundtaun==1)
// 		{
		  
// 		  if ((foundlepttaup&&foundjetfromtaun)||(foundlepttaun&&foundjetfromtaup)) 
// 		    {
// 		      std::cout<<"°°°°°°°°°°°°°°°°°°°°°°°°°°GOOD EVENT°°°°°°°°°°°°°°°°°°°°°"<<endl;
		      
// 		      GenEnergyZ=muener+neumuener+ neutauener+jetMomener+tauneutener;
// 		      //GenTrEnergyZ=muenertr+neumuenertr+neutauenertr+jetMomenertr+tauneutenertr;
// 		      //energiatrasvZ=GenEnergyZ*sin(Zeta_fin.Theta()); 
		      
// 		      GenP_ZZ=mupz+neumupz+neutaupz+jetMompz+tauneutpz;
// 		      GenP_XZ=mupx+neumupx+neutaupx+jetMompx+tauneutpx;
// 		      GenP_YZ=mupy+neumupy+neutaupy+jetMompy+tauneutpy;
		      
// 		      GenTrEnergyZ=sqrt((GenEnergyZ*GenEnergyZ)-(GenP_ZZ*GenP_ZZ));
// 		      GenVisEnergyZ=muener+jetMomener;
// 		      GenVisTrEnergyZ=sqrt((GenVisEnergyZ*GenVisEnergyZ)-((mupz+jetMompz)*(mupz+jetMompz)));		      
		     
 		     //  GenP_TZ=sqrt((GenP_XZ*GenP_XZ)+(GenP_YZ*GenP_YZ));
		      
// 		      GenInvMassZ=sqrt((GenEnergyZ*GenEnergyZ)-(GenP_XZ*GenP_XZ)-(GenP_YZ*GenP_YZ)-(GenP_ZZ*GenP_ZZ));
		     
// 		      GenTrMassZ_rel=sqrt((GenEnergyZ*GenEnergyZ)-(GenP_ZZ*GenP_ZZ));
		      
// 		      GenTrMassZ_A=sqrt((GenTrEnergyZ*GenTrEnergyZ)-(GenP_XZ*GenP_XZ)-(GenP_YZ*GenP_YZ));
// 		      //GenTrMassZ_B=sqrt((energiatrasvZ*energiatrasvZ)-(GenP_XZ*GenP_XZ)-(GenP_YZ*GenP_YZ));
		     
// 		      GenVisMassZ=sqrt(((muener+jetMomener)*(muener+jetMomener))-((mupx+jetMompx)*(mupx+jetMompx))-((mupy+jetMompy)*(mupy+jetMompy))-((mupz+jetMompz)*(mupz+jetMompz)));

// 		      GenVisTrMassZ_A=sqrt(((GenVisTrEnergyZ)*(GenVisTrEnergyZ))-((mupx+jetMompx)*(mupx+jetMompx))-((mupy+jetMompy)*(mupy+jetMompy)));
// 		      //GenVisTrMassZ_B=sqrt((energiatrasvvisZ*energiatrasvvisZ)-(mupx+jetMompx)*(mupx+jetMompx)-(mupy+jetMompy)*(mupy+jetMompy));
// 		      GenVisTrMassZ_rel=sqrt((muener+jetMomener)*(muener+jetMomener)-(mupz+jetMompz)*(mupz+jetMompz));

// 		      TLorentzVector Zeta_fin=tauneut+mu+jetMom+neutau+neumu;
// 		      Zeta_fin_eta=Zeta_fin.Eta();
// 		      Zeta_fin_rapidity=Zeta_fin.Rapidity();
		     
// 		      TLorentzVector Zeta=tauh_ap+taul_ap;
// 		      Z_eta=Zeta.Eta();
// 		      Z_rapidity=Zeta.Rapidity();
// 		      Z_rap=0.5*log(((GenEnergyZ+GenP_ZZ)/(GenEnergyZ-GenP_ZZ)));
		      
		      
		     
// 		      /////////////SBAGLIATA		      
// 		      Zvisibletransversemass_relat2=sqrt((jetMomener*muener)-(jetMompz*mupz));
		      		      		      

		      
		      
// 		      ///////masse trasverse visibili
		      
// 		      trvismass1=sqrt(sqrt(mupx*mupx+mupy*mupy)*sqrt(jetMompx*jetMompx+jetMompy*jetMompy)-((mupx*mupx)+(jetMompx*jetMompx))-((mupy*mupy)+(jetMompy*jetMompy)));
		      
		      
// 		    } 
// 		}  //primo if
// 	      //std::cout << "found2SimTaus: " << found2taus << std::endl;
// 	    } //if condizione su 2 figli      
// 	  //}    
// 	} //if riconoscimento Z     	
//     }  //for (p = generated_event->particles_begin(); p!=generated_event->particles_end(); p++)
  
  
//   //       // //   //  /// Charged particles
  
  
//   countchpartjet=0;
  
//   ptleadingtrack=-100.;
//   ptparticle=-100.;    
//   charged=0;

//   for (HepMC::GenEvent::particle_iterator q = generated_event->particles_begin(); q!= generated_event->particles_end();++q) 
//     {
      
//       if ( (*q)->status() == 1 && 
// 	   (*q)->pdg_id() != 22 && 
// 	   abs((*q)->pdg_id()) != 16 && 
// 	   abs((*q)->pdg_id()) != 14 && 
// 	   abs((*q)->pdg_id()) != 12 &&
// 	   abs((*q)->pdg_id()) != 3122 &&
// 	   abs((*q)->pdg_id()) != 3322 && 
// 	   abs((*q)->pdg_id()) != 2112 &&
// 	   abs((*q)->pdg_id()) != 4112 &&
// 	   abs((*q)->pdg_id()) != 3212 &&
// 	   abs((*q)->pdg_id()) != 4132 &&
// 	   abs((*q)->pdg_id()) != 4312 &&
// 	   abs((*q)->pdg_id()) != 4332 &&
// 	   abs((*q)->pdg_id()) != 5122 &&
// 	   abs((*q)->pdg_id()) != 5212 &&
// 	   abs((*q)->pdg_id()) != 2114 &&
// 	   abs((*q)->pdg_id()) != 3214 &&
// 	   abs((*q)->pdg_id()) != 3324 &&
// 	   abs((*q)->pdg_id()) != 4114 &&
// 	   abs((*q)->pdg_id()) != 4314 &&
// 	   abs((*q)->pdg_id()) != 4334 &&
// 	   abs((*q)->pdg_id()) != 5214 &&
// 	   abs((*q)->pdg_id()) != 130 &&
// 	   abs((*q)->pdg_id()) != 311 &&
// 	   abs((*q)->pdg_id()) != 421 &&
// 	   abs((*q)->pdg_id()) != 511 &&
// 	   abs((*q)->pdg_id()) != 531 &&
// 	   abs((*q)->pdg_id()) != 111 &&
// 	   abs((*q)->pdg_id()) != 313 &&
// 	   abs((*q)->pdg_id()) != 423 &&
// 	   abs((*q)->pdg_id()) != 513 &&
// 	   abs((*q)->pdg_id()) != 533 &&
// 	   abs((*q)->pdg_id()) != 113 &&
// 	   abs((*q)->pdg_id()) != 310 &&
// 	   abs((*q)->pdg_id()) != 10313 &&
// 	   abs((*q)->pdg_id()) != 10423 &&
// 	   abs((*q)->pdg_id()) != 10113 &&
// 	   abs((*q)->pdg_id()) != 10223 &&
// 	   abs((*q)->pdg_id()) != 10333 &&
// 	   abs((*q)->pdg_id()) != 10443 &&
// 	   abs((*q)->pdg_id()) != 20313 &&
// 	   abs((*q)->pdg_id()) != 20423 &&
// 	   abs((*q)->pdg_id()) != 20113 &&
// 	   abs((*q)->pdg_id()) != 20223 &&
// 	   abs((*q)->pdg_id()) != 20333 &&
// 	   abs((*q)->pdg_id()) != 20443 &&
// 	   abs((*q)->pdg_id()) != 10311 &&
// 	   abs((*q)->pdg_id()) != 10421 &&
// 	   abs((*q)->pdg_id()) != 10111 &&
// 	   abs((*q)->pdg_id()) != 10221 &&
// 	   abs((*q)->pdg_id()) != 10331 &&
// 	   abs((*q)->pdg_id()) != 10441 &&
// 	   abs((*q)->pdg_id()) != 315 &&
// 	   abs((*q)->pdg_id()) != 425 &&
// 	   abs((*q)->pdg_id()) != 115 &&
// 	   abs((*q)->pdg_id()) != 335 &&
// 	   abs((*q)->pdg_id()) != 445 )
// 	{ 	
	  
// 	  // 	//vector<HepMC::GenParticle*> ancestor = (*p)->listAncestors();
// 	  // 	//vector<HepMC::GenParticle*>::const_iterator ancestors_iter;
	  
// 	 bool foundtauancestor=false;
//  	  bool noleptmuons=true;
	  
	  
// // 	  // 	//for (ancestors_iter = ancestor.begin(); ancestors_iter != ancestor.end(); ancestors_iter++) 

// //for( HepMC::GenVertex::particle_iterator aDaughter=(*p)->end_vertex()->particles_begin(HepMC::descendants); aDaughter !=(*p)->end_vertex()->particles_end(HepMC::descendants);aDaughter++)

	  
// 	  for( HepMC::GenVertex::particle_iterator ancestors_iter=(*q)->production_vertex()->particles_begin(HepMC::parents); ancestors_iter!=(*q)->production_vertex()->particles_end(HepMC::parents); ancestors_iter++)
// 	  	{
// 		  if ( abs((*ancestors_iter)->pdg_id()) == 15) 
// 		    {
// 		      if (abs((*q)->pdg_id())==13)
// 			{
// 			  barcodemujet=abs((*q)->barcode());
// 			  //cout<<"BARCODE mu JET= "<<barcodemulept<<endl;
// 			  if (barcodemulept==barcodemujet)
// 			    {
// 			      noleptmuons=false;
// 			    }
// 			}	
		      
// 		      foundtauancestor=true;
// 		    }
		  
// 		  }
	      
	  
// 	  if ((foundtauancestor == true)&&(noleptmuons==true))
	    
// 	    {
// 	      countchpartjet++; 
	      
// 	      ptparticle=(*q)->momentum().perp();
// 	      if(ptparticle>ptleadingtrack)
// 		{
// 		  ptleadingtrack=ptparticle;
// 		}
// 	    }
// 	  charged=countchpartjet;
// 	}
 
//     }     //for calcolo particelle cariche
 
 
  
  //  INIZIA RECO

  cout<<"pronti 1?"<<endl;  
   
   
  //  TRACCE 

  //struct SortTrackByDecreasingPt{
 // bool operator()( const Track &t1, const Track &t2) const {
//    return t1.pt()>t2.pt();
//}
//};


//ComparePt ptComparator;

float NumRecHitstmp=-1000;
  float NumValRecHitstmp=-1000;

  NTrackCand=0;
Handle<TrackCollection> tracks;
  iEvent.getByLabel(TracksTag, tracks);
  const TrackCollection& myTrackCollection=*(tracks.product());
  // cout<<"dimensione size tracce "<<myTrackCollection.size()<<endl;
  TrackSize=myTrackCollection.size();   
  TrackCollection MyTrackCollection=*(tracks.product());

std::sort(MyTrackCollection.begin(),MyTrackCollection.end(),ComparePt());

  //std::sort((*tracks.product()).begin(),(*tracks.product()).end(),ComparePt());
//std::sort((MyTrackCollection).begin(),(MyTrackCollection).end(),ComparePt());

//cout<<"tracce size "<<tracce.size()<<endl;
 // for(TrackCollection::const_iterator iM=MyTrackCollection.begin();iM!=MyTrackCollection.end(); iM++) 
//     {  
      
//       cout<<" pt traccia  ordinati?: "<<iM->pt()<<endl;      
//      }

//////////////////////////////////////////////// usa std::sort


  
  // for(TrackCollection::const_iterator iM=MyTrackCollection.begin();iM!=MyTrackCollection.end(); iM++) 
//     {  
//       cout<<" pt traccia non ordinati?: "<<iM->pt()<<endl;      
//     }

//cout<<"dimensione tracce dopo sort poco prima ciclo"<<MyTrackCollection.size()<<endl;;  
  for(TrackCollection::const_iterator iM=MyTrackCollection.begin();iM!=MyTrackCollection.end()&&NTrackCand<NTRACKMAX; iM++) 
    {

      // if (iM->pt()<((--iM)->pt()))cout<<"attenzione"<<endl;

      Tk_ValRecHits[NTrackCand]=iM->numberOfValidHits();
      //cout<<" NumValRecHits traccia: "<<iM->numberOfValidHits()<<endl;

      Tk_NumRecHits[NTrackCand] =iM->recHitsSize();
      //cout<<" NumRecHits traccia: "<<iM->recHitsSize()<<endl;

      Tk_ChiSqr[NTrackCand]=iM->chi2();
      //cout<<" chi2 traccia: "<<iM->chi2()<<endl;
      Tk_Pt[NTrackCand]=iM->pt();
      // cout<<" pt traccia: "<<iM->pt()<<endl;
      Tk_d0[NTrackCand]=iM->d0(); 
      Tk_dxy[NTrackCand]=iM->dxy();
      //cout<<" dxy traccia "<<iM->dxy()<<endl;  
      Tk_dz[NTrackCand]=iM->dz();  
      //cout<<" dz traccia "<<iM->dz()<<endl;  
      Tk_LostHits[NTrackCand]= iM->numberOfLostHits(); 
      //cout<<" NumLostHits traccia: "<<iM->numberOfLostHits()<<endl;
      //cout<<"ener traccia "<<iM->energy();
      //Tk_vertex[NTrackCand]=iM->;       
     // cout<<"Tk_ValRecHits "<<Tk_ValRecHits[NTrackCand]<<" Tk_NumRecHits "<<Tk_NumRecHits[NTrackCand]<<" Tk_ChiSqr "<<Tk_ChiSqr[NTrackCand]<<" Tk_Pt "<<Tk_Pt[NTrackCand]<<" Tk_d0 "<<Tk_d0[NTrackCand]<<" Tk_dxy "<<Tk_dxy[NTrackCand]<<endl;
      //cout<<" Tk_dz "<<Tk_dz[NTrackCand]<<" Tk_LostHits "<<Tk_LostHits[NTrackCand]<<" NTrackCand "<<NTrackCand<<endl; 



 
      NTrackCand++;

      NumRecHitstmp=iM->recHitsSize();
      if(NumRecHitstmp<NumRecHitsMin)NumRecHitsMin=NumRecHitstmp;     
      if(NumRecHitstmp>NumRecHitsMax)NumRecHitsMax=NumRecHitstmp;        
      //cout<<"Num Rec Hits "<<NumRecHitstmp<<" NumRecHitsMin "<<NumRecHitsMin<<" NumRecHitsMax "<<NumRecHitsMax<<endl;
      
      NumValRecHitstmp=iM->numberOfValidHits();
      if(NumValRecHitstmp<NumValRecHitsMin)NumValRecHitsMin=NumValRecHitstmp;     
      if(NumValRecHitstmp>NumValRecHitsMax)NumValRecHitsMax=NumValRecHitstmp;        
      //cout<<"Num Val Rec Hits "<<NumValRecHitstmp<<" NumValRecHitsMin "<<NumValRecHitsMin<<" NumValRecHitsMax "<<NumValRecHitsMax<<endl;
          
      //ChiSqr=iM->chi2();
      
      
      if ((iM->recHitsSize()>=8)&&(iM->chi2()<10)&&(iM->pt()>1))
	{ 
	  SumTrackPt8+=iM->pt();
	  NumTracks_8++;
	}
      if ((iM->recHitsSize()>=10)&&(iM->chi2()<10)&&(iM->pt()>1)) 
	{ 
	  SumTrackPt10+=iM->pt();
	  NumTracks_10++;	 
	}
    }

   

////////////  MUON
  
  //cout<<"pronti 2?"<<endl;

   Handle<reco::MuIsoDepositAssociationMap> hcalIso;
   iEvent.getByLabel (theHCALIsoDepositLabel, hcalIso);
   
   Handle<reco::MuIsoDepositAssociationMap> ecalIso;
   iEvent.getByLabel (theECALIsoDepositLabel, ecalIso);
   
    edm::Handle<reco::MuIsoDepositAssociationMap> hocalIso;
   iEvent.getByLabel(theHOCALIsoDepositLabel, hocalIso);
   
   Handle<reco::MuIsoDepositAssociationMap> trackerIso;
   iEvent.getByLabel (theTrackerIsoDepositLabel, trackerIso);

   Handle<MuonCollection> muonHandle;
   iEvent.getByLabel(MuonTag,muonHandle);
   const MuonCollection& myMuonCollection=*(muonHandle.product()); 
 
   CandidateCollection muons;
   muons.clear();
   for(reco::MuonCollection::const_iterator tr= muonHandle->begin();tr!=muonHandle->end();++tr){
     muons.push_back(tr->clone());
   }
   muons.sort(ComparePtMu());
   
   delpx=0;
   delpy=0;
   delpz=0; 
   
   //double extRadius25=0.25;
   //double extRadius40=0.40;
   double extRadius[5]={0.15,0.20,0.25,0.30,0.35};
   //double extRadius[5]={0.15,0.20,0.25,0.30,0.35}
   
   NMuCand=0; 
   MuSize=myMuonCollection.size();
   for(MuonCollection::const_iterator iT =myMuonCollection.begin();iT !=myMuonCollection.end()&&NMuCand<NMUMAX;++iT){ 
     //cout<<"inizia cal isolation"<<endl;
     delpx += iT->px();
     delpy +=iT ->py();
     delpz +=iT ->pz();
     delsumet +=iT ->et();
     const reco::MuIsoDeposit& depEcal     = (*ecalIso)[iT->combinedMuon()];
     const reco::MuIsoDeposit& depHcal     = (*hcalIso)[iT->combinedMuon()];
     const reco::MuIsoDeposit& depHOcal   = (*hocalIso)[iT->combinedMuon()];
     const reco::MuIsoDeposit& depTracker = (*trackerIso)[iT->combinedMuon()];
     
     //  const reco::MuIsoDeposit& depEcal     = (*ecalIso)[iT->track()];
     //         const reco::MuIsoDeposit& depHcal     = (*hcalIso)[iT->track()];
     //         const reco::MuIsoDeposit& depHOcal   = (*hocalIso)[iT->track()];
     //         const reco::MuIsoDeposit& depTracker = (*trackerIso)[iT->track()];
     
     Muon_px[NMuCand]=iT->px();
     Muon_py[NMuCand]=iT->py();
     Muon_pz[NMuCand]=iT->pz();
     Muon_pt[NMuCand]=iT->pt();
     // cout<<" pt muone: "<<iT->pt()<<endl;
     Muon_p[NMuCand]=iT->p();
     Muon_en[NMuCand]=iT->energy();
     Muon_tren[NMuCand]=iT->et();
     Muon_eta[NMuCand]=iT->eta();
     Muon_phi[NMuCand]=iT->phi();
     Muon_theta[NMuCand]=iT->theta();
     Muon_charge[NMuCand]=iT->charge();
     TrackRef trk = iT->combinedMuon();
     //TrackRef trk = iT->track();
     MuonDz[NMuCand]=trk->dz();
     MuonIP[NMuCand]=trk->dxy();
     Muonvtx[NMuCand]=trk->vertex().x();
     Muonvty[NMuCand]=trk->vertex().y();
     Muonvtz[NMuCand]=trk->vertex().z();
     
     
     EcalM_Tot[NMuCand] = depEcal.muonEnergy();
     HcalM_Tot[NMuCand] = depHcal.muonEnergy();
     HoM_Tot[NMuCand] = depHOcal.muonEnergy();
     TrkM_Tot[NMuCand] = depTracker.muonEnergy();
     //cout<<"EcalM_Tot "<<EcalM_Tot<<" HcalM_Tot "<<HcalM_Tot<<" HoM_Tot "<<HoM_Tot<<" TrkM_Tot "<<TrkM_Tot<<endl;
     
     for(size_t ocs=0;ocs<5;ocs++){
       EcalM_Frac[NMuCand][ocs] = depEcal.depositWithin(extRadius[ocs]);
       HcalM_Frac[NMuCand][ocs] = depHcal.depositWithin(extRadius[ocs]);
       HoM_Frac[NMuCand][ocs]   = depHOcal.depositWithin(extRadius[ocs]);      
       std::pair<double, int> sumPtAndNTracks = depTracker.depositAndCountWithin(extRadius[ocs]);
       TrkM_Frac[NMuCand][ocs] = sumPtAndNTracks.first;
       TrkIsoM_N[NMuCand][ocs] = sumPtAndNTracks.second;
       
       
}


Handle <CandidateCollection> CollM;
iEvent.getByLabel(MuonCand,CollM);

Handle<CandMatchMap> matchm;
iEvent.getByLabel(GenMatch,matchm);

  sizeRM=CollM->size();
  // cout<<"sizeRefMu"<<sizeRM<<endl;
 for(int c=0;c<sizeRM;++c){
   CandidateRef Ref(CollM,c);
   CandMatchMap::const_iterator trueIter=matchm->find(Ref);
   if(trueIter!=matchm->end()){
     CandidateRef trueRef=trueIter->val;
     if(trueRef.isNonnull()){
    
   const CandidateRef MCTruthMatchRef=trueRef;
  
   if(MCTruthMatchRef.isNonnull()){
   const Candidate & MCTruthMatch=*MCTruthMatchRef; 

    const Candidate*  Mother=(MCTruthMatch).mother();
  
    const Candidate & mother=*Mother; 
  
 pdgId_candM[c]=(MCTruthMatch).pdgId();
 
 size_mothM[c]=(MCTruthMatch).numberOfMothers();
 
 pdgId_mothM[c]=mother.pdgId();

 //cout<<"pdg Id mother"<<pdgId_mothM[c]<<endl;
}   
 
}
}

       //  cout<<" EcalM_Frac25 "<<EcalM_Frac25<<" HcalM_Frac25"<<HcalM_Frac25<<" HoM_Frac25 "<<HoM_Frac25<<endl; 
       //cout<<" TrkM_Frac25 "<<TrkM_Frac25<<" TrkIsoM_N25 "<<TrkIsoM_N25<<endl;
  
       // mhcalfrac25=HcalM_Frac25/(iT->pt());
//        mtrackfrac25=TrkM_Frac25/(iT->pt());
//        mhofrac25=HoM_Frac25/(iT->pt());
//        mecalfrac25=EcalM_Frac25/(iT->pt());
//        calfrac25= mhcalfrac25+mhofrac25+mecalfrac25;
       
//        cout<<"pt "<<iT->pt()<<" mhcalfrac25 "<<mhcalfrac25<<" mhofrac25 "<<mhofrac25<<" mecalfrac25 "<<mecalfrac25<<endl;
//        cout<<"calfrac25 "<<calfrac25<<" mtrackfrac25 "<<mtrackfrac25<<endl;         
       

//        EcalM_Frac40 = depEcal.depositWithin(extRadius40);
//        HcalM_Frac40= depHcal.depositWithin(extRadius40);
//        HoM_Frac40   = depHOcal.depositWithin(extRadius40);
// std::pair<double, int> sumPtAndNTracks40 = depTracker.depositAndCountWithin(extRadius40);
//        TrkM_Frac40 = sumPtAndNTracks40.first;
//        TrkIsoM_N40 = sumPtAndNTracks40.second;

//        cout<<" EcalM_Frac40 "<<EcalM_Frac40<<" HcalM_Frac40"<<HcalM_Frac40<<" HoM_Frac40 "<<HoM_Frac40<<endl; 
//        cout<<" TrkM_Frac40 "<<TrkM_Frac40<<" TrkIsoM_N40 "<<TrkIsoM_N40<<endl;     
  
//        mhcalfrac40=HcalM_Frac40/(iT->pt());
//        mtrackfrac40=TrkM_Frac40/(iT->pt());
//        mhofrac40=HoM_Frac40/(iT->pt());
//        mecalfrac40=EcalM_Frac40/(iT->pt());
//        calfrac40= mhcalfrac40+mhofrac40+mecalfrac40;
       
//        cout<<"pt "<<iT->pt()<<" mhcalfrac40 "<<mhcalfrac40<<" mhofrac40 "<<mhofrac40<<" mecalfrac40 "<<mecalfrac40<<endl;
//        cout<<"calfrac40 "<<calfrac40<<" mtrackfrac40 "<<mtrackfrac40<<endl;   

 
//  if((calfrac25<0.05)&&(mtrackfrac25<0.05))
// 	 {
// 	   NumNewIsolMu++;
// 	   FlagNewMuIsol=true;
// 	   cout<<"NumNewIsolMu "<<NumNewIsolMu<<endl;
// 	   cout<<" trovato un mu isolato nel tracker+calor "<<endl;
// 	   NewIsoPXMu=iT->px();
// 	   NewIsoPYMu=iT->py();
// 	   NewIsoPZMu=iT->pz();
// 	   NewIsoEtaMu=iT->eta();
// 	   NewIsoTrMassMu=iT->mt();
// 	   NewIsoTrEnMu=iT->et();
// 	   NewIsoEnMu=iT->energy();
// 	   NewIsoPTMu=iT->pt();
// 	   NewIsoChMu=iT->charge();
// 	   const Muon& Newmuon = (*iT);
// 	   NewMuon_=(Newmuon);
// 	   TrackRef NewMuonTrack=(&(*iT))->track();
// 	   NewIsoImpParMu=NewMuonTrack->d0();
// 	   cout<<"MuonImpPar"<<NewIsoImpParMu<<endl;
	   
// 	   NewIsoInvMassMu=sqrt((NewIsoEnMu*NewIsoEnMu)-(NewIsoPXMu*NewIsoPXMu)-(NewIsoPYMu*NewIsoPYMu)-(NewIsoPZMu*NewIsoPZMu)); 
// 	   NewIsoPhiMu=iT->phi();
	   
// 	 }
       }
 NMuCand++;
   }




 Muon OldMuon_, NewMuon_;
 const TrackCollection& trackCollection =*(tracks.product());
 
 bool IsolValid=false;
 
 float pxmuons=0;
 float pymuons=0;  
 
 for(MuonCollection::const_iterator iT =myMuonCollection.begin();iT !=myMuonCollection.end()&&NMuCand<NMUMAX;++iT)
   {
     
     if((iT->pt()>3)&&(fabs(iT->eta())<2.5)){
       pxmuons+=iT->px();
       pymuons+=iT->py();
     }
     //cout <<" LOOP MUONI"<<endl;    
     NumMuons++;
     MuTkIsolation myTkIsolation(&(*iT),&(trackCollection));
     myTkIsolation.setExtRadius (radiusConeExt_); 
     myTkIsolation.setIntRadius (radiusConeInt_);
     myTkIsolation.setPtLow (pTMin_);
     myTkIsolation.setLip (lip_);
     NumIsolTracks=myTkIsolation.getNumberTracks();
     //RecoP_TMuon=iT->pt();
     //cout<<"RecoP_TMuon "<<RecoP_TMuon<<endl;
     if ((myTkIsolation.getPtTracks()>0.)){
       PTSum = myTkIsolation.getPtTracks();
       //NumIsolTracks=myTkIsolation.getNumberTracks();
       //cout<<"NumIsolTracks"<<NumIsolTracks<<"and PTSum"<<PTSum<< endl;
       isoValue=PTSum/(iT->pt());
       //cout<<"PTSum "<<PTSum<<" and pt "<<iT->pt()<<" and isoValue "<<isoValue<<endl;
       if ( (PTSum/(iT->pt()))>=0 && (PTSum/(iT->pt()))<=cut_ ){ 
	 selected_.push_back(&(*iT) );
	 IsolValid=true;
	 NumOldIsolMu++;
	 //cout<<"NumOldIsolMu "<<NumOldIsolMu<<endl;
	 //cout<<" trovato un mu isolato nel tracker "<<endl;
	 OldIsoPXMu=iT->px();
	 OldIsoPYMu=iT->py();
	 OldIsoPZMu=iT->pz();
	 OldIsoEtaMu=iT->eta();
	 OldIsoTrMassMu=iT->mt();
	 OldIsoTrEnMu=iT->et();
	 OldIsoEnMu=iT->energy();
	 OldIsoPTMu=iT->pt();
	 OldIsoChMu=iT->charge();
	 const Muon& Oldmuon = (*iT);
	 OldMuon_=(Oldmuon);
	 TrackRef OldMuonTrack=(&(*iT))->track();
	 OldIsoImpParMu=OldMuonTrack->d0();
	 //cout<<"MuonImpPar"<<OldIsoImpParMu<<endl;
	 
	 OldIsoInvMassMu=sqrt((OldIsoEnMu*OldIsoEnMu)-(OldIsoPXMu*OldIsoPXMu)-(OldIsoPYMu*OldIsoPYMu)-(OldIsoPZMu*OldIsoPZMu)); 
	 OldIsoPhiMu=iT->phi();
       }
         
     }
     
   }

   if(NumOldIsolMu==1)FlagOldRecoMu=true;
   if(NumNewIsolMu==1)FlagNewRecoMu=true;


   /////////// numero minimo di rec hits nella collezione di tracce ctf o gs






   
   //   RECO CALO MET
   
   Handle<CaloMETCollection> calometHandle;
   iEvent.getByLabel(CaloMet_,calometHandle);
   const CaloMETCollection& myCaloMetCollection=*(calometHandle.product());
   
     
   for(CaloMETCollection::const_iterator iT =myCaloMetCollection.begin();iT !=myCaloMetCollection.end();iT++){ 
     FlagRecoMET=true;  
     if(myCaloMetCollection.size()>1)
       {
	 //cout<<"TROVATA PIÙ DI UNA MET"<<endl;
	 CaloMETMissTrEner=0;
	 break;
       } 
     //cout<<"trovata una met"<<endl;
     
     
     //scalar sum of transverse energy over all objects   
     CaloMETSumMissTrEner=iT->sumEt();
     
     CaloMET_x=((iT->px())-pxmuons);
     CaloMET_y=((iT->py())-pymuons);
     CaloMETMissTrEner=sqrt((CaloMET_x*CaloMET_x)+(CaloMET_y*CaloMET_y));


     CaloMET_time=iT->pt();
   
     //cout<<"calo met "<<CaloMETMissTrEner<<endl;
     //MET significance =MET/ sqrt(SumET);
     CaloMETSignificance=iT->mEtSig();
     
     //longitudinal component of the vector sum of energy over all objects
     CaloMET_E_long=iT->e_longitudinal();
     phiMET=iT->phi();
   }
 
 
  
  

   float energiatrastmpT=-1000;
   bool _flagRecoTau=false;
   TrackRefVector TauTr; 
   Candidate* Tau_;
   NPFJetCand=0;
   Handle<PFJetCollection> pfJetHandle;
   iEvent.getByLabel("iterativeCone5PFJets", pfJetHandle);
     const PFJetCollection& pfJetColl = *(pfJetHandle.product()); 
     //cout << "Found " << pfJetColl.size() << " PF Jet candidates" << endl; 
     PFJetSize= pfJetColl.size();  
     for (PFJetCollection::const_iterator iter  = pfJetColl.begin(); iter != pfJetColl.end()&&NPFJetCand<NPFJETMAX; ++iter)
       {

	 PFJet_Pt[NPFJetCand]=iter->pt();
	// cout<<" pt pf jet: "<<iter->pt()<<endl; 
	 PFJet_P[NPFJetCand]=iter->p();
	 PFJet_E[NPFJetCand]=iter->energy();
	 PFJet_Et[NPFJetCand]=iter->et(); 

	 
	 //cout<<"PFJet_Pt "<<PFJet_Pt[NPFJetCand]<<endl;
	 //cout<<"PFJet_P "<<PFJet_P[NPFJetCand]<<endl;
	 //cout<<"PFJet_E "<<PFJet_E[NPFJetCand]<<endl;
	 //cout<<"PFJet_Et "<<PFJet_Et[NPFJetCand]<<endl;
	 //cout<<"NPFJetCand "<<NPFJetCand<<endl;
	 NPFJetCand++;
	 const PFJet& obj = (*iter);
	 PFJetET=obj.et();
	 nPFjet++;	 
       }
  
    


   
   Handle<PFTauCollection> thePFTauHandle;
   iEvent.getByLabel(PFTau_,thePFTauHandle);
   const PFTauCollection& myTauCollection = *(thePFTauHandle.product()); 
   
   
   Handle<PFTauDiscriminatorByIsolation> thePFTauDiscriminatorByIsolation;
   iEvent.getByLabel(PFTauDiscriminatorByIsolation_,thePFTauDiscriminatorByIsolation);
   
   math::XYZTLorentzVector P1;
   n_tau_candidate = myTauCollection.size();

  
  
   for (PFTauCollection::size_type iTau = 0; iTau < thePFTauHandle->size(); iTau++)  
     
     {
 
       // //dr=1000;	   
	   PFTauRef theTau(thePFTauHandle, iTau);
	   const PFTau& tau = (*theTau);
	   //cout<<"Inizio ciclo tau"<<endl;
	   energiatrastmpT=tau.et();
	   //cout<<"energiatrastmpT "<<energiatrastmpT<<" tau_et "<<tau_et<<endl;
	   PFCandidateRef theLeadPFCand = tau.leadPFChargedHadrCand();

	   ///////////
// // 	   if (!theLeadPFCand) {      
// // 	     cerr << "No Lead PFCand " << endl;
// // 	     continue;
// // 	   }   
 	   tau_disc = (*thePFTauDiscriminatorByIsolation)[theTau];
	   //  cout << "DiscriminatorByIsolation value: " << tau_disc << endl;
// // 	   if(tau_disc!=1)
// // 	     {
// // 	       cout<<"tau non isolato: "<<tau_disc<<endl;
// // 	       continue;
	       
// // 	     }
// // 	   P1 = tau.p4();
// // 	   phiT=P1.Phi();

	   
	   
// 	   cout<<"dr "<<dr<<endl;
// 	   if(dr<0.1)
// 	     {
// 	       cout<<"dr minore di 0.1 e vale "<<dr<<endl;
// 	       continue;
// 	     }

// 	   ///////
	   dr_a=DeltaRMin(tau, myMuonCollection);
	   //cout<<"silmarillion  dr_a "<<dr_a<<"  ldg trk "<<"  disc "<<tau_disc<<endl;
	   if((dr_a>0.1)&&(!theLeadPFCand==0)&&(tau_disc==1))
	   //if((dr_a>0.1)&&(tau_disc==1))
	   {
	     
	     dr=dr_a;
	    // cout<<" dr minimo tra tau e muon collection "<<dr<<endl;	   
	      P1 = tau.p4();
	     phiT=P1.Phi();
	     //cout<<"energiatrastmpT "<<energiatrastmpT<<" tau_et "<<tau_et<<endl;
	     if(energiatrastmpT<=tau_et)
	       {
		 // cout<<"ATTENZIONE ENERGIA BASSA: deve skippare tau"<<endl;
		 continue;
	       }
	     

	     
	     tau_et=energiatrastmpT;
	     //cout<<"energiatrastmpT "<<energiatrastmpT<<" tau_et "<<tau_et<<endl;
	   //PFCandidateRef theLeadPFCand = tau.leadPFChargedHadrCand();
	     
	   //cout<<"energiatrastmpT "<<energiatrastmpT<<" tau_et "<<tau_et<<endl;
	   

	   // if(abs(cosDeltaPhiTM)<0.95)
	   // {
	   // cout<<"letizia"<<endl;
	   //continue;
	   //}
	   //if((energiatrastmpT>tau_et
     // cout<<"energiatrastmpT "<<energiatrastmpT<<" tau_et "<<tau_et<<endl;
     tau_leading_trk_pt       = (*theLeadPFCand).pt();
     TauImpPar=0;
      TrackRef TauLdgTrack=tau.leadTrack();

     //TrackRef TauLdgTrack = (*theLeadPFCand).track();
     //TauImpPar=(*theLeadPFCand).d0();
//MuonImpPar=MuonTrack->d0();

//TauImpPar=TauLdgTrack->d0();
//cout<<"TauImpPar"<<TauImpPar<<endl;     

     tau_zvertex              = tau.vz();      // What is this quantity?
    tau_charge               = tau.charge();
   tau_had_energy           = tau.maximumHCALPFClusterEt();

   const PFTauTagInfo& tagInfo     = (*(tau.pfTauTagInfoRef()));
 TauTr= tau.signalTracks();
 //cout<<"size tracce nel signal cone "<<TauTr.size()<<endl;

    leading_trk_sipt         = fabs(tau.leadPFChargedHadrCandsignedSipt());
   
    tau_inv_mass             = tau.p4().M();
    tau_inv_mass2            = tau.alternatLorentzVect().M();

    tau_ch_hadrons              = tagInfo.PFChargedHadrCands().size();
    tau_neu_hadrons             = tagInfo.PFNeutrHadrCands().size();
    tau_photons                 = tagInfo.PFGammaCands().size();

   
     tau_sig_ch_hadrons          = tau.signalPFChargedHadrCands().size();
     // for(PFTauCollection:: const_iterator iC=0; iC<tau.signalPFChargedHadrCands().size(), iC++)
//        {
// 	 TauCharge+=iC->tau.signalPFChargedHadrCands().charge();
//        }

    //  if(tau.signalPFChargedHadrCands().size()==1)
//        {
//  	 TauCharge=tau.signalPFChargedHadrCands().charge();
//        }
//      if(tau.signalPFChargedHadrCands().size()==3)
//        {

// 	 for(PFTau:: const_iterator iC=0; iC<tau.signalPFChargedHadrCands().size(), iC++)
// 	   {
// 	     TauCharge+=iC->signalPFChargedHadrCands().charge();
// 	   }
//        }
     
     
     tau_sig_photons             = tau.signalPFGammaCands().size();
    
     tau_sig_neu_hadrons         = tau.signalPFNeutrHadrCands().size();

     tau_iso_ch_hadrons          = tau.isolationPFChargedHadrCands().size();
     tau_iso_photons             = tau.isolationPFGammaCands().size();
     tau_iso_neu_hadrons         = tau.isolationPFNeutrHadrCands().size();

     tau_sumpt_iso_ch_hadrons = tau.isolationPFChargedHadrCandsPtSum();
     tau_sumpt_iso_photons    = tau.isolationPFGammaCandsEtSum();

    const PFJet& pfJet              = (*(tagInfo.pfjetRef()));
    tau_em_energy            = pfJet.chargedEmEnergyFraction()+pfJet.neutralEmEnergyFraction();

     chHadEnergyJet           = pfJet.chargedHadronEnergy();
     neuHadEnergyJet          = pfJet.neutralHadronEnergy();
    chEMEnergyJet            = pfJet.chargedEmEnergy();
   chMuEnergyJet            = pfJet.chargedMuEnergy();
     neuEMEnergyJet           = pfJet.neutralEmEnergy();
      chMultJet                = pfJet.chargedMultiplicity();
      neuMultJet               = pfJet.neutralMultiplicity();
    muMultJet                = pfJet.muonMultiplicity();
    tau_eta = tau.eta();
    tau_px=tau.px();
    tau_py=tau.py();
    tau_pz=tau.pz();
    tau_pt=tau.pt();
    tau_energy=tau.energy();

    ////////// tau charge true

     tau_chargeTrue=getTauCharge(tau);



    //tau_et=tau.et();
      cout << "     (old tau          Lead PFCand pt: " << tau_leading_trk_pt       << endl
           << "             InvariantMass of the Tau: " << tau_inv_mass             << endl
           << "                    Vertex of the Tau: " << tau_zvertex              << endl
           << "                    Charge of the Tau: " << tau_charge               << endl
           << "                   Em energy fraction: " << tau_em_energy            << endl
           << "                    Max Hadron energy: " << tau_had_energy           << endl
           << "          # of PF charged hadr. cands: " << tau_ch_hadrons           << endl
           << "          # of PF neutral hadr. cands: " << tau_neu_hadrons          << endl
           << "                  # of PF gamma cands: " << tau_photons              << endl
           << "          # of SignalPFChargedHadrons: " << tau_sig_ch_hadrons       << endl
           << "       # of IsolationPFChargedHadrons: " << tau_iso_ch_hadrons       << endl
           << "          # of SignalPFGammaCandidate: " << tau_sig_photons          << endl
           << "       # of IsolationPFGammaCandidate: " << tau_iso_photons          << endl
           << "  Sum pT of Isolation Charged Hadrons: " << tau_sumpt_iso_ch_hadrons << endl
           << "Sum E_T of Isolation Gamma Candidates: " << tau_sumpt_iso_photons    << endl
           << "                     PF Jet EM Energy: " << chEMEnergyJet+neuEMEnergyJet << endl
           <<"                     Transverse energy: "<<tau_et                     <<endl    
           << endl;

      _flagRecoTau=true;
      
      P1 = tau.p4();
      cout<<" tau.p4() "<<P1<<endl;
      cout<<" tau tr energy "<<tau_et<<endl;
      cout<<" tau  energy "<<tau_energy<<endl;
      //cout<<" px, py, pz, energia tau: "<<tau_px<<" "<<tau_py<<" "<<tau_pz<<" "<<tau_energy<<endl;
      phiT=P1.Phi(); 
      Tau=tau;
      Tau_=tau.clone();
      //     }
}

}   
   

   deltaphiMuMET=OldIsoPhiMu-phiMET;
   deltaphiTauMET=phiT-phiMET;
   deltaphiTM=phiT-OldIsoPhiMu;
   cosDeltaPhiMuMET=cos(deltaphiMuMET);
   cosDeltaPhiTauMET=cos(deltaphiTauMET);
   cosDeltaPhiTM=cos(deltaphiTM);

   float drtauemuisol=DeltaR(Tau.momentum(),OldMuon_.momentum());
  // cout<<" drtauemuisol "<<drtauemuisol<<endl;
  

   TauSize=0;
   NTauCand=0;
  // cout<<"collezione tau originale"<<thePFTauHandle->size()<<endl;
   for (PFTauCollection::size_type iTau = 0; iTau < thePFTauHandle->size()&&NTauCand<NTAUMAX; iTau++) {
     
     PFTauRef theTau(thePFTauHandle, iTau);
     if (!theTau) {
       cout << "Strange! Should have a valid PFTauRef for iTau=" << iTau << endl;
       continue;
     }
     const PFTau& tau = (*theTau);
     PFCandidateRef theLeadPFCand = tau.leadPFChargedHadrCand();
     if (!theLeadPFCand) {      
       cout << "EWKTauAnalyser::analyze(): No Lead PFCand, iTau=" << iTau << endl;
       continue;
     }
     TauSize++;
     Tau_disc[NTauCand]=(*thePFTauDiscriminatorByIsolation)[theTau];
     const PFTauTagInfo& tagInfo     = (*(tau.pfTauTagInfoRef()));
     const PFJet& pfJet = (*(tagInfo.pfjetRef()));
     TrackRef TauLdgTrack=tau.leadTrack();
     

     if(tau.signalTracks().size()>0) TauNTr[NTauCand]= tau.signalTracks().size();
     else TauNTr[NTauCand]=0;
     //cout<<"size tracce nel signal cone "<<tau.signalTracks().size()<<endl;
     Tau_eta[NTauCand]=tau.eta();
     Tau_ImpPar[NTauCand]=TauLdgTrack->d0();
     Tau_leading_trk_pt[NTauCand]= (*theLeadPFCand).pt();
     Tau_had_energy [NTauCand]= tau.maximumHCALPFClusterEt();
     Tau_inv_mass[NTauCand]= tau.p4().M(); 
     Leading_trk_sipt[NTauCand]= fabs(tau.leadPFChargedHadrCandsignedSipt());
     Tau_inv_mass2[NTauCand]= tau.alternatLorentzVect().M(); 
     
     Tau_ch_hadrons[NTauCand]= tagInfo.PFChargedHadrCands().size();
     Tau_neu_hadrons[NTauCand]= tagInfo.PFNeutrHadrCands().size();
     Tau_photons [NTauCand]= tagInfo.PFGammaCands().size();
     Tau_sig_ch_hadrons[NTauCand]= tau.signalPFChargedHadrCands().size();
     Tau_iso_ch_hadrons[NTauCand]= tau.isolationPFChargedHadrCands().size();
     Tau_iso_photons[NTauCand]= tau.isolationPFGammaCands().size();
     Tau_iso_neu_hadrons[NTauCand]= tau.isolationPFNeutrHadrCands().size();
     Tau_sig_photons[NTauCand] = tau.signalPFGammaCands().size();
     Tau_sig_neu_hadrons[NTauCand] = tau.signalPFNeutrHadrCands().size(); 
     ChHadEnergyJet[NTauCand]= pfJet.chargedHadronEnergy();
     Tau_sumpt_iso_ch_hadrons[NTauCand]= tau.isolationPFChargedHadrCandsPtSum();
     Tau_sumpt_iso_photons[NTauCand]= tau.isolationPFGammaCandsEtSum();    
     Tau_pt[NTauCand]=tau.pt();
     Tau_px[NTauCand]=tau.px(); 
     Tau_py[NTauCand]=tau.py();
     Tau_pz[NTauCand]=tau.pz();
     Tau_energy[NTauCand]=tau.energy();
     Tau_et[NTauCand]=tau.et();
     
     Tau_zvertex[NTauCand]=tau.vz();  
     Tau_charge[NTauCand]=getTauCharge(tau);
     Tau_em_energy[NTauCand]          = pfJet.chargedEmEnergyFraction()+pfJet.neutralEmEnergyFraction();
     NeuHadEnergyJet[NTauCand] = pfJet.neutralHadronEnergy(); 
     ChEMEnergyJet[NTauCand]= pfJet.chargedEmEnergy();
     ChMuEnergyJet[NTauCand]= pfJet.chargedMuEnergy();
     NeuEMEnergyJet[NTauCand]= pfJet.neutralEmEnergy();
     ChMultJet[NTauCand]= pfJet.chargedMultiplicity();
     NeuMultJet[NTauCand] = pfJet.neutralMultiplicity();
     MuMultJet[NTauCand]= pfJet.muonMultiplicity();
     //MaximumHCALPFClusterEt
     
     math::XYZTLorentzVector  P4T=tau.p4();
     PhiT[NTauCand]=P4T.Phi();
     
     cout <<                  "  iTau: " << NTauCand << endl
	  << "                       Lead PFCand pt: " << Tau_leading_trk_pt[NTauCand]       << endl
	  << "             InvariantMass of the Tau: " << Tau_inv_mass[NTauCand]             << endl
	  << "                    Vertex of the Tau: " << Tau_zvertex[NTauCand]              << endl
	  << "                    Charge of the Tau: " << Tau_charge[NTauCand]               << endl
	  << "                   Em energy fraction: " << Tau_em_energy[NTauCand]            << endl
	  << "                    Max Hadron energy: " << Tau_had_energy[NTauCand]           << endl
	  << "          # of PF charged hadr. cands: " << Tau_ch_hadrons[NTauCand]           << endl
	  << "          # of PF neutral hadr. cands: " << Tau_neu_hadrons[NTauCand]          << endl
	  << "                  # of PF gamma cands: " << Tau_photons[NTauCand]              << endl
	  << "          # of SignalPFChargedHadrons: " << Tau_sig_ch_hadrons[NTauCand]       << endl
	  << "       # of IsolationPFChargedHadrons: " << Tau_iso_ch_hadrons[NTauCand]       << endl
	  << "          # of SignalPFGammaCandidate: " << Tau_sig_photons[NTauCand]          << endl
	  << "       # of IsolationPFGammaCandidate: " << Tau_iso_photons[NTauCand]          << endl
	  << "  Sum pT of Isolation Charged Hadrons: " << Tau_sumpt_iso_ch_hadrons[NTauCand] << endl
	  << "Sum E_T of Isolation Gamma Candidates: " << Tau_sumpt_iso_photons[NTauCand]    << endl
	  << "                     PF Jet EM Energy: " << ChEMEnergyJet[NTauCand]+NeuEMEnergyJet[NTauCand] << endl
	  <<"                     Transverse energy: "<<Tau_et[NTauCand]                     <<endl    
	  << endl;
        
     NTauCand++;

}


   cout<<"tau size "<<TauSize<<endl;


 // B-TAGGING   

   edm::Handle<reco::JetTagCollection> bTagHandleSM;
   iEvent.getByLabel("softMuonJetTags", bTagHandleSM);
   const reco::JetTagCollection & bTagsSM = *(bTagHandleSM.product());
   
   edm::Handle<reco::JetTagCollection> bTagHandleJP;
   iEvent.getByLabel("jetProbabilityJetTags", bTagHandleJP);
   const reco::JetTagCollection & bTagsJP = *(bTagHandleJP.product());

   edm::Handle<reco::JetTagCollection> bTagHandleSMNoIP;
   iEvent.getByLabel("softMuonNoIPJetTags", bTagHandleSMNoIP);
   const reco::JetTagCollection & bTagsSMNoIP = *(bTagHandleSMNoIP.product());
 
   edm::Handle<reco::JetTagCollection> bTagHandleTCHE;
   iEvent.getByLabel("trackCountingHighEffJetTags", bTagHandleTCHE);
   const reco::JetTagCollection & bTagsTCHE= *(bTagHandleTCHE.product());
  
   edm::Handle<reco::JetTagCollection> bTagHandleTCHP;
   iEvent.getByLabel("trackCountingHighEffJetTags", bTagHandleTCHP);
   const reco::JetTagCollection & bTagsTCHP= *(bTagHandleTCHP.product());
   
   BtaggingModule myBtagging(iEvent);
   
   b_discSM=myBtagging.getDiscriminator_SoftMuons(iEvent, Tau, OldMuon_,_flagRecoTau);
   BDiscSumSM=myBtagging.getDiscSum_SoftMuons(iEvent, Tau);

   b_discSMNoIP=myBtagging.getDiscriminator_SoftMuonsNoIP(iEvent, Tau);
   BDiscSumSMNoIP=myBtagging.getDiscSum_SoftMuonsNoIP(iEvent, Tau);
   
   b_discJP=myBtagging.getDiscriminator_JetProbability (iEvent, Tau);
   BDiscSumJP=myBtagging.getDiscSum_JetProbability (iEvent, Tau);
 
   b_discTCHE=myBtagging.getDiscriminator_TrackCountingHE (iEvent, Tau);
   BDiscSumTCHE=myBtagging.getDiscSum_TrackCountingHE (iEvent, Tau);
   
   b_discTCHP=myBtagging.getDiscriminator_TrackCountingHP (iEvent, Tau);
   BDiscSumTCHP=myBtagging.getDiscSum_TrackCountingHP (iEvent, Tau);
   
  
   DRMINTBSM=myBtagging.getDrMinTB_SoftMuons(iEvent, Tau, OldMuon_);
   DRMAXMBSM=myBtagging.getDrMaxMB_SoftMuons (iEvent, Tau,OldMuon_ ) ;
   
   DRMINTBSMNoIP=myBtagging.getDrMinTB_SoftMuonsNoIP(iEvent, Tau, OldMuon_);
   DRMINMBSMNoIP=myBtagging.getDrMinMB_SoftMuonsNoIP (iEvent, Tau,OldMuon_ ) ;
   
   DRMINTBJP=myBtagging.getDrMinTB_JetProbability(iEvent, Tau, OldMuon_);
   DRMINMBJP=myBtagging.getDrMinMB_JetProbability(iEvent, Tau,OldMuon_ ) ;
   
   DRMINTBTCHE=myBtagging.getDrMinTB_TrackCountingHE(iEvent, Tau, OldMuon_);
   DRMINMBTCHE=myBtagging.getDrMinMB_TrackCountingHE(iEvent, Tau,OldMuon_ ) ;
   
   DRMINTBTCHP=myBtagging.getDrMinTB_TrackCountingHP(iEvent, Tau, OldMuon_);
   DRMINMBTCHP=myBtagging.getDrMinMB_TrackCountingHP(iEvent, Tau,OldMuon_ ) ;
  

   // nJetBSM=myBtagging.getNJetsFromTagInfo_SoftMuons(iEvent);
   //nJetBSMNoIP=myBtagging.getNJetsFromTagInfo_SoftMuonsNoIP(iEvent);
   //nJetBJP=myBtagging.getNJetsFromTagInfo_JetProbability(iEvent);
   //nJetBTCHE=myBtagging.getNJetsFromTagInfo_TrackCountingHE(iEvent);
   //nJetBTCHP=myBtagging.getNJetsFromTagInfo_TrackCountingHP(iEvent);
  

   /////////// Caratteristiche DEI CALO JET con vettori inclusi b_disc e dr e dphi tra mu e tau
   NCaloJetCand=0; 
   CaloJetSize=bTagsSM.size();
   for (size_t i = 0; i != bTagsSM.size()&&NCaloJetCand<NCALOJETMAX; ++i) { 

     edm::RefToBase<reco::Jet>  BRefJet=bTagsSM[i].jet();     
     Jet const*  BJet=BRefJet.get();
     
     CaloJet_Pt[NCaloJetCand]=BJet->pt(); 
     //cout<<" pt calo jet: "<<BJet->pt()<<endl;  
     CaloJet_P[NCaloJetCand]=BJet->p();  
     CaloJet_E[NCaloJetCand]  =BJet->energy(); 
     CaloJet_Et[NCaloJetCand]=BJet->et(); 
     NCaloJetCand++;
     njetITC5++;   
     if (BJet->pt()>15) njet15ITC5++;
   }


  
   NCaloJetCandSM=0;
   NMuCandSM=0;
   for (size_t i = 0; i != bTagsSM.size()&&NCaloJetCandSM<NCALOJETMAX; ++i) 
     {
        edm::RefToBase<reco::Jet>  BRefJet=bTagsSM[i].jet();     
	Jet const*  BJet=BRefJet.get();    
        SMCaloJet_Bdisc[NCaloJetCandSM]=bTagsSM[i].discriminator();
       // cout<<"SMCaloJet_Bdisc["<<NCaloJetCandSM<<"] "<<SMCaloJet_Bdisc[NCaloJetCandSM]<<endl;
        NMuCandSM=0;
	for(MuonCollection::const_iterator iT =myMuonCollection.begin();iT !=myMuonCollection.end()&&NMuCandSM<NMUMAX;iT++)
	  {
	    SMCaloJet_DrM[NCaloJetCandSM][NMuCandSM]=DeltaR(iT->p4(),BJet->p4());
	    SMCaloJet_DPhiM[NCaloJetCandSM][NMuCandSM]=(BJet->phi())-(iT->phi());
	  //  cout<<"SMCaloJet_DrM["<<NCaloJetCandSM<<"]["<<NMuCandSM<<"] "<<SMCaloJet_DrM[NCaloJetCandSM][NMuCandSM]<<endl;
	  //  cout<<"SMCaloJet_DPhiM["<<NCaloJetCandSM<<"]["<<NMuCandSM<<"] "<<SMCaloJet_DPhiM[NCaloJetCandSM]<<endl;
	    NMuCandSM++;
	  }
	SMCaloJet_DrT[NCaloJetCandSM]=DeltaR(BJet->p4(),Tau.p4());
	SMCaloJet_DPhiT[NCaloJetCandSM]=(BJet->phi())-(Tau.phi()); 
       // cout<<"SMCaloJet_DrT["<<NCaloJetCandSM<<"] "<<SMCaloJet_DrT[NCaloJetCandSM]<<endl;
       // cout<<"SMCaloJet_DPhiT["<<NCaloJetCandSM<<"] "<<SMCaloJet_DPhiT[NCaloJetCandSM]<<endl;
        NCaloJetCandSM++;  
     }
   

   NCaloJetCandSMNoIP=0;
   NMuCandSMNoIP=0;
   for (size_t i = 0; i != bTagsSMNoIP.size()&&NCaloJetCandSMNoIP<NCALOJETMAX; ++i) 
     {
        edm::RefToBase<reco::Jet>  BRefJet=bTagsSMNoIP[i].jet();     
	Jet const*  BJet=BRefJet.get();    
        SMNoIPCaloJet_Bdisc[NCaloJetCandSMNoIP]=bTagsSMNoIP[i].discriminator();
      //  cout<<"SMNoIPCaloJet_Bdisc["<<NCaloJetCandSMNoIP<<"] "<<SMNoIPCaloJet_Bdisc[NCaloJetCandSMNoIP]<<endl;
        NMuCandSMNoIP=0;
	for(MuonCollection::const_iterator iT =myMuonCollection.begin();iT !=myMuonCollection.end()&&NMuCandSMNoIP<NMUMAX;iT++)
	  {
	    SMNoIPCaloJet_DrM[NCaloJetCandSMNoIP][NMuCandSMNoIP]=DeltaR(iT->p4(),BJet->p4());
	    SMNoIPCaloJet_DPhiM[NCaloJetCandSMNoIP][NMuCandSMNoIP]=(BJet->phi())-(iT->phi());
	 //   cout<<"SMNoIPCaloJet_DrM["<<NCaloJetCandSMNoIP<<"]["<<NMuCandSMNoIP<<"] "<<SMNoIPCaloJet_DrM[NCaloJetCandSMNoIP][NMuCandSMNoIP]<<endl;
	 //   cout<<"SMNoIPCaloJet_DPhiM["<<NCaloJetCandSMNoIP<<"]["<<NMuCandSMNoIP<<"] "<<SMNoIPCaloJet_DPhiM[NCaloJetCandSMNoIP]<<endl;
	    NMuCandSMNoIP++;
	  }
	SMNoIPCaloJet_DrT[NCaloJetCandSMNoIP]=DeltaR(BJet->p4(),Tau.p4());
	SMNoIPCaloJet_DPhiT[NCaloJetCandSMNoIP]=(BJet->phi())-(Tau.phi()); 
      //  cout<<"SMNoIPCaloJet_DrT["<<NCaloJetCandSMNoIP<<"] "<<SMNoIPCaloJet_DrT[NCaloJetCandSMNoIP]<<endl;
      //  cout<<"SMNoIPCaloJet_DPhiT["<<NCaloJetCandSMNoIP<<"] "<<SMNoIPCaloJet_DPhiT[NCaloJetCandSMNoIP]<<endl;
        NCaloJetCandSMNoIP++;  
     }
   
   NCaloJetCandJP=0;
   NMuCandJP=0;
   for (size_t i = 0; i != bTagsJP.size()&&NCaloJetCandJP<NCALOJETMAX; ++i) 
     {
        edm::RefToBase<reco::Jet>  BRefJet=bTagsJP[i].jet();     
	Jet const*  BJet=BRefJet.get();    
        JPCaloJet_Bdisc[NCaloJetCandJP]=bTagsJP[i].discriminator();
       // cout<<"JPCaloJet_Bdisc["<<NCaloJetCandJP<<"] "<<JPCaloJet_Bdisc[NCaloJetCandJP]<<endl;
        NMuCandJP=0;
	for(MuonCollection::const_iterator iT =myMuonCollection.begin();iT !=myMuonCollection.end()&&NMuCandJP<NMUMAX;iT++)
	  {
	    JPCaloJet_DrM[NCaloJetCandJP][NMuCandJP]=DeltaR(iT->p4(),BJet->p4());
	    JPCaloJet_DPhiM[NCaloJetCandJP][NMuCandJP]=(BJet->phi())-(iT->phi());
	    //cout<<"JPCaloJet_DrM["<<NCaloJetCandJP<<"]["<<NMuCandJP<<"] "<<JPCaloJet_DrM[NCaloJetCandJP][NMuCandJP]<<endl;
	   // cout<<"JPCaloJet_DPhiM["<<NCaloJetCandJP<<"]["<<NMuCandJP<<"] "<<JPCaloJet_DPhiM[NCaloJetCandJP]<<endl;
	    NMuCandJP++;
	  }
	JPCaloJet_DrT[NCaloJetCandJP]=DeltaR(BJet->p4(),Tau.p4());
	JPCaloJet_DPhiT[NCaloJetCandJP]=(BJet->phi())-(Tau.phi()); 
       // cout<<"JPCaloJet_DrT["<<NCaloJetCandJP<<"] "<<JPCaloJet_DrT[NCaloJetCandJP]<<endl;
       // cout<<"JPCaloJet_DPhiT["<<NCaloJetCandJP<<"] "<<JPCaloJet_DPhiT[NCaloJetCandJP]<<endl;
        NCaloJetCandJP++;  
     }

   
   NCaloJetCandTCHE=0;
   NMuCandTCHE=0;
   for (size_t i = 0; i != bTagsTCHE.size()&&NCaloJetCandTCHE<NCALOJETMAX; ++i) 
     {
        edm::RefToBase<reco::Jet>  BRefJet=bTagsTCHE[i].jet();     
	Jet const*  BJet=BRefJet.get();    
        TCHECaloJet_Bdisc[NCaloJetCandTCHE]=bTagsTCHE[i].discriminator();
       // cout<<"TCHECaloJet_Bdisc["<<NCaloJetCandTCHE<<"] "<<TCHECaloJet_Bdisc[NCaloJetCandTCHE]<<endl;
        NMuCandTCHE=0;
	for(MuonCollection::const_iterator iT =myMuonCollection.begin();iT !=myMuonCollection.end()&&NMuCandTCHE<NMUMAX;iT++)
	  {
	    TCHECaloJet_DrM[NCaloJetCandTCHE][NMuCandTCHE]=DeltaR(iT->p4(),BJet->p4());
	    TCHECaloJet_DPhiM[NCaloJetCandTCHE][NMuCandTCHE]=(BJet->phi())-(iT->phi());
	 //   cout<<"TCHECaloJet_DrM["<<NCaloJetCandTCHE<<"]["<<NMuCandTCHE<<"] "<<TCHECaloJet_DrM[NCaloJetCandTCHE][NMuCandTCHE]<<endl;
	 //   cout<<"TCHECaloJet_DPhiM["<<NCaloJetCandTCHE<<"]["<<NMuCandTCHE<<"] "<<TCHECaloJet_DPhiM[NCaloJetCandTCHE]<<endl;
	    NMuCandTCHE++;
	  }
	TCHECaloJet_DrT[NCaloJetCandTCHE]=DeltaR(BJet->p4(),Tau.p4());
	TCHECaloJet_DPhiT[NCaloJetCandTCHE]=(BJet->phi())-(Tau.phi()); 
       // cout<<"TCHECaloJet_DrT["<<NCaloJetCandTCHE<<"] "<<TCHECaloJet_DrT[NCaloJetCandTCHE]<<endl;
       // cout<<"TCHECaloJet_DPhiT["<<NCaloJetCandTCHE<<"] "<<TCHECaloJet_DPhiT[NCaloJetCandTCHE]<<endl;
        NCaloJetCandTCHE++;  
     }
     

  NCaloJetCandTCHP=0;
   NMuCandTCHP=0;
   for (size_t i = 0; i != bTagsTCHP.size()&&NCaloJetCandTCHP<NCALOJETMAX; ++i) 
     {
        edm::RefToBase<reco::Jet>  BRefJet=bTagsTCHP[i].jet();     
	Jet const*  BJet=BRefJet.get();    
        TCHPCaloJet_Bdisc[NCaloJetCandTCHP]=bTagsTCHP[i].discriminator();
      //  cout<<"TCHPCaloJet_Bdisc["<<NCaloJetCandTCHP<<"] "<<TCHPCaloJet_Bdisc[NCaloJetCandTCHP]<<endl;
        NMuCandTCHP=0;
	for(MuonCollection::const_iterator iT =myMuonCollection.begin();iT !=myMuonCollection.end()&&NMuCandTCHP<NMUMAX;iT++)
	  {
	    TCHPCaloJet_DrM[NCaloJetCandTCHP][NMuCandTCHP]=DeltaR(iT->p4(),BJet->p4());
	    TCHPCaloJet_DPhiM[NCaloJetCandTCHP][NMuCandTCHP]=(BJet->phi())-(iT->phi());
	 //   cout<<"TCHPCaloJet_DrM["<<NCaloJetCandTCHP<<"]["<<NMuCandTCHP<<"] "<<TCHPCaloJet_DrM[NCaloJetCandTCHP][NMuCandTCHP]<<endl;
	 //   cout<<"TCHPCaloJet_DPhiM["<<NCaloJetCandTCHP<<"]["<<NMuCandTCHP<<"] "<<TCHPCaloJet_DPhiM[NCaloJetCandTCHP]<<endl;
	    NMuCandTCHP++;
	  }
	TCHPCaloJet_DrT[NCaloJetCandTCHP]=DeltaR(BJet->p4(),Tau.p4());
	TCHPCaloJet_DPhiT[NCaloJetCandTCHP]=(BJet->phi())-(Tau.phi()); 
       // cout<<"TCHPCaloJet_DrT["<<NCaloJetCandTCHP<<"] "<<TCHPCaloJet_DrT[NCaloJetCandTCHP]<<endl;
      //   cout<<"TCHPCaloJet_DPhiT["<<NCaloJetCandTCHP<<"] "<<TCHPCaloJet_DPhiT[NCaloJetCandTCHP]<<endl;
        NCaloJetCandTCHP++;  
     }
     
   
   Jet   BJet1,BJet2,BJet3,BJet4 ;  
   
   for (size_t i = 0; i != bTagsSM.size(); ++i) {
    // cout<<" jet "<<i<<endl;
    // cout<<"b disc "<<bTagsSM[i].discriminator()<<endl;
     edm::RefToBase<reco::Jet>  BRefJet=bTagsSM[i].jet();     
     Jet const*  BJet=BRefJet.get();
    // cout<<" pt jet "<<BJet->pt()<<endl;
     
       //nJetBSM++;
       const Jet& bjet=*BJet;
       
       if((bTagsSM[i].discriminator()>0)&&(BJet->pt()>10))Njet10SM++;
       if((bTagsSM[i].discriminator()>0)&&(BJet->pt()>15))Njet15SM++;
       if((bTagsSM[i].discriminator()>0)&&(BJet->pt()>20))Njet20SM++;
       
       if (BJet->pt()>15){      
	 if((DRMINTBSM<DRMINMBSM)&&(_flagRecoTau==1)&&(DeltaR(Tau.p4(),BJet->p4())<DRMINMBSM))
	   {
	     b_discTSM=bTagsSM[i].discriminator();
	     drTBMintmpSM=DeltaR(Tau.p4(),BJet->p4());
	   } 
       }   
      // cout<<"DeltaR Mu e jet "<<DeltaR(OldMuon_.p4(),BJet->p4())<<endl;
       
       ///// ordiniamo i jet cominciando da quelli + vicini al mu
       
       drMB1tmpSM=DeltaR(OldMuon_.p4(),BJet->p4());
       drMB2tmpSM=DeltaR(OldMuon_.p4(),BJet->p4());
       drMB3tmpSM=DeltaR(OldMuon_.p4(),BJet->p4());
       drMB4tmpSM=DeltaR(OldMuon_.p4(),BJet->p4());
       
       
       if((drMB1tmpSM<drMB1SM)){       
	 drMB4SM=drMB3SM;
	 drMB3SM=drMB2SM;
	 drMB2SM=drMB1SM;
	 drMB1SM=drMB1tmpSM;
	 
	 bdiscM4SM=bdiscM3SM;
	 bdiscM3SM=bdiscM2SM;
	 bdiscM2SM=bdiscM1SM;
	 bdiscM1SM=bTagsSM[i].discriminator(); 
	 
	 BJet4=BJet3;
	 BJet3=BJet2;
	 BJet2=BJet1;
	 BJet1=bjet;
       }
       
       if((drMB2tmpSM<drMB2SM)&&(drMB2tmpSM>drMB1SM)){
	 drMB4SM=drMB3SM;
	 drMB3SM=drMB2SM;
	 drMB2SM=drMB2tmpSM;
	 
	 bdiscM4SM=bdiscM3SM;
	 bdiscM3SM=bdiscM2SM;
	 bdiscM2SM=bTagsSM[i].discriminator();  
	 
	 BJet4=BJet3;
	 BJet3=BJet2;
	 BJet2=bjet;
	 
       }
       
       if ((drMB3tmpSM<drMB3SM)&&(drMB3tmpSM>drMB2SM)){
	 drMB4SM= drMB3SM;
	 drMB3SM=drMB3tmpSM;
	 
	 bdiscM4SM=bdiscM3SM;
	 bdiscM3SM=bTagsSM[i].discriminator();
	 
	 BJet4=BJet3;
	 BJet3=bjet;
	   
       }  
       
       if ((drMB4tmpSM<drMB4SM)&&(drMB4tmpSM>drMB3SM)){
	 drMB4SM=drMB4tmpSM;
	 bdiscM4SM=bTagsSM[i].discriminator();
	 BJet4=bjet;
       } 
      //  cout<<" drMB1SM "<<drMB1SM<<" drMB2SM "<<drMB2SM<<" drMB3SM "<<drMB3SM<<" drMB4SM "<<drMB4SM<<endl;
      //  cout<<" bdiscM1SM "<<bdiscM1SM<<" bdiscM2SM "<<bdiscM2SM<<" bdiscM3SM "<<bdiscM3SM<<" bdiscM4SM "<<bdiscM4SM<<endl;      
     //   cout<<" pt dei 4 "<<BJet1.pt()<<" "<<BJet2.pt()<<" "<<BJet3.pt()<<" "<<BJet4.pt()<<endl;
       
       
       // if(bTagsSM[i].discriminator()>0)
// 	 {
// 	   if(b_discPosFirstSM==-1000)
// 	     {
// 	       b_discPosFirstSM=bTagsSM[i].discriminator();
// 	     }
// 	  else if(b_discPosSecondSM==-1000)
// 	    {
// 	      b_discPosSecondSM=bTagsSM[i].discriminator();
// 	      cout<<"DUE B JET TAGGATI"<<endl;
// 	    }
// 	  else if (b_discPosThirdSM==-1000)
// 	    {
// 	      b_discPosThirdSM=bTagsSM[i].discriminator();
// 	    }
// 	 }
//        else b_discPosFourSM=bTagsSM[i].discriminator();	     
//     }
   }
   
   int nBjet1MSM=0;
   int nBjet2MSM=0;
   int nBjet3MSM=0;
   int nBjet4MSM=0;
   
   if((bdiscM1SM>0))nBjet1MSM++;
   if((bdiscM2SM>0))nBjet2MSM++;
   if((bdiscM3SM>0))nBjet3MSM++;
   if((bdiscM4SM>0))nBjet4MSM++; 
   
   nBjetMSM=nBjet1MSM+nBjet2MSM+nBjet3MSM+nBjet4MSM;
   PT1=BJet1.pt();
   PT2=BJet2.pt();
   PT3=BJet3.pt();
   PT4=BJet4.pt();

//   cout<<" drMB1SM "<<drMB1SM<<" drMB2SM "<<drMB2SM<<" drMB3SM "<<drMB3SM<<" drMB4SM "<<drMB4SM<<endl;
//   cout<<" bdiscM1SM "<<bdiscM1SM<<" bdiscM2SM "<<bdiscM2SM<<" bdiscM3SM "<<bdiscM3SM<<" bdiscM4SM "<<bdiscM4SM<<endl;
 //  cout<<"nBjet1MSM "<<nBjet1MSM<<" nBjet2MSM "<<nBjet2MSM<<" nBjet3MSM "<<nBjet3MSM<<" nBjet4MSM "<<nBjet4MSM<<" nBjetMSM "<<nBjetMSM<<endl;   
//   cout<<" pt dei 4 "<<PT1<<" "<<PT2<<" "<<PT3<<" "<<PT4<<endl;

   if(bTagsSM.size()>0)
     {
       size_t ix=0;
       edm::RefToBase<reco::Jet>  BRefJet1=bTagsSM[ix].jet();
       const Jet *  BJet1=BRefJet1.get();
       enerTrBjetSM1=BJet1->et();  
       b_discSM1=bTagsSM[0].discriminator();
       drTBSM1=DeltaR(Tau.p4(),(BJet1->p4()));
       
       if(bTagsSM.size()>1)
	 {
	   size_t iy=1;
	   edm::RefToBase<reco::Jet>  BRefJet2=bTagsSM[iy].jet();
	   const Jet *  BJet2=BRefJet2.get();
	   enerTrBjetSM2=BJet2->et();
	   b_discSM2=bTagsSM[1].discriminator();
	   drTBSM2=DeltaR(Tau.p4(),BJet2->p4());
	   
	   if(bTagsSM.size()>2)
	     {
	       size_t iz=2;
	       edm::RefToBase<reco::Jet>  BRefJet3=bTagsSM[iz].jet();
	       const Jet *  BJet3=BRefJet3.get();
	       enerTrBjetSM3=BJet3->et();
	       b_discSM3=bTagsSM[2].discriminator();
	       drTBSM3=DeltaR(Tau.p4(),BJet3->p4());
	       
	       if(bTagsSM.size()>3)
		 {
		   size_t iw=3;
		   edm::RefToBase<reco::Jet>  BRefJet4=bTagsSM[iw].jet();
		   const Jet *  BJet4=BRefJet4.get();
		   enerTrBjetSM4=BJet4->et();  
		   b_discSM4=bTagsSM[3].discriminator();
		   drTBSM4=DeltaR(Tau.p4(),BJet4->p4()); 
		 }
	     }
	 }  
       
     }
  



   float drTBMintmpSMNoIP=-1000; 
  
   
   for (size_t i = 0; i != bTagsSMNoIP.size(); ++i) {
     edm::RefToBase<reco::Jet>  BRefJet=bTagsSMNoIP[i].jet();
     Jet const*  BJet=BRefJet.get();

     if((bTagsSMNoIP[i].discriminator()>0)&&(BJet->pt()>10))Njet10SMNoIP++;
     if((bTagsSMNoIP[i].discriminator()>0)&&(BJet->pt()>15))Njet15SMNoIP++;
     if((bTagsSMNoIP[i].discriminator()>0)&&(BJet->pt()>20))Njet20SMNoIP++;

  
     
     if((DRMINTBSMNoIP<DRMINMBSMNoIP)&&(_flagRecoTau==1)&&(DeltaR(Tau.p4(),BJet->p4())<DRMINMBSMNoIP))
       {
//	 cout<<" ATTENZIONE STA TAGGANDO TAU-JET?"<<endl;
	 b_discTSMNoIP=bTagsSMNoIP[i].discriminator();
	 drTBMintmpSMNoIP=DeltaR(Tau.p4(),BJet->p4());
       } 

     
      
	 if(bTagsSMNoIP[i].discriminator()>0)
	   {
	     if(b_discPosFirstSMNoIP==-1000)
	       {
		 b_discPosFirstSMNoIP=bTagsSMNoIP[i].discriminator();
	       }
	     else if(b_discPosSecondSMNoIP==-1000)
	       {
		 b_discPosSecondSMNoIP=bTagsSMNoIP[i].discriminator();
		// cout<<"DUE B JET TAGGATI"<<endl;
	       }
	     else if (b_discPosThirdSMNoIP==-1000)
	       {
		 b_discPosThirdSMNoIP=bTagsSMNoIP[i].discriminator();
	       }
	   }
	 else b_discPosFourSMNoIP=bTagsSMNoIP[i].discriminator();	     
   }
   
 //  cout<<" per il tau jet: b_disc "<<b_discTSMNoIP<<" delta r tra tau e questo jet "<<drTBMintmpSMNoIP<<" e DRMINTB "<<DRMINTBSMNoIP<<endl;  

   if(bTagsSMNoIP.size()>0)
     {
       size_t ix=0;
       edm::RefToBase<reco::Jet>  BRefJet1=bTagsSMNoIP[ix].jet();
       const Jet *  BJet1=BRefJet1.get();
       enerTrBjetSMNoIP1=BJet1->et();  
       b_discSMNoIP1=bTagsSMNoIP[0].discriminator();
       drTBSMNoIP1=DeltaR(Tau.p4(),(BJet1->p4()));
       
       if(bTagsSMNoIP.size()>1)
	 {
	   size_t iy=1;
	   edm::RefToBase<reco::Jet>  BRefJet2=bTagsSMNoIP[iy].jet();
	   const Jet *  BJet2=BRefJet2.get();
	   enerTrBjetSMNoIP2=BJet2->et();
	   b_discSMNoIP2=bTagsSMNoIP[1].discriminator();
	   drTBSMNoIP2=DeltaR(Tau.p4(),BJet2->p4());
	   
	   if(bTagsSMNoIP.size()>2)
	     {
	       size_t iz=2;
	       edm::RefToBase<reco::Jet>  BRefJet3=bTagsSMNoIP[iz].jet();
	       const Jet *  BJet3=BRefJet3.get();
	       enerTrBjetSMNoIP3=BJet3->et();
	       b_discSMNoIP3=bTagsSMNoIP[2].discriminator();
	       drTBSMNoIP3=DeltaR(Tau.p4(),BJet3->p4());
	       
	       if(bTagsSMNoIP.size()>3)
		 {
		   size_t iw=3;
		   edm::RefToBase<reco::Jet>  BRefJet4=bTagsSMNoIP[iw].jet();
		   const Jet *  BJet4=BRefJet4.get();
		   enerTrBjetSMNoIP4=BJet4->et();  
		   b_discSMNoIP4=bTagsSMNoIP[3].discriminator();
		   drTBSMNoIP4=DeltaR(Tau.p4(),BJet4->p4()); 
		 }
	     }
	 }  
       
     }
  




 float drTBMintmpJP=-1000;   
  
 for (size_t i = 0; i != bTagsJP.size(); ++i) {
   edm::RefToBase<reco::Jet>  BRefJet=bTagsJP[i].jet();     
   Jet const*  BJet=BRefJet.get();
   
   
   if((bTagsJP[i].discriminator()>0)&&(BJet->pt()>10))Njet10JP++;
   if((bTagsJP[i].discriminator()>0)&&(BJet->pt()>15))Njet15JP++;
   if((bTagsJP[i].discriminator()>0)&&(BJet->pt()>20))Njet20JP++;

     if((DRMINTBJP<DRMINMBJP)&&(_flagRecoTau==1)&&(DeltaR(Tau.p4(),BJet->p4())<DRMINMBJP))
       {
// 	 cout<<" ATTENZIONE STA TAGGANDO TAU-JET?"<<endl;
	 b_discTJP=bTagsJP[i].discriminator();
	 drTBMintmpJP=DeltaR(Tau.p4(),BJet->p4());
       } 

     
      
	 if(bTagsJP[i].discriminator()>0)
	   {
	     if(b_discPosFirstJP==-1000)
	       {
		 b_discPosFirstJP=bTagsJP[i].discriminator();
	       }
	     else if(b_discPosSecondJP==-1000)
	       {
		 b_discPosSecondJP=bTagsJP[i].discriminator();
		// cout<<"DUE B JET TAGGATI"<<endl;
	       }
	     else if (b_discPosThirdJP==-1000)
	       {
		 b_discPosThirdJP=bTagsJP[i].discriminator();
	       }
	   }
	 else b_discPosFourJP=bTagsJP[i].discriminator();	     
   }
   
 // cout<<" per il tau jet: b_disc "<<b_discTJP<<" delta r tra tau e questo jet "<<drTBMintmpJP<<" e DRMINTB "<<DRMINTBJP<<endl;  

   if(bTagsJP.size()>0)
     {
       size_t ix=0;
       edm::RefToBase<reco::Jet>  BRefJet1=bTagsJP[ix].jet();
       const Jet *  BJet1=BRefJet1.get();
       enerTrBjetJP1=BJet1->et();  
       b_discJP1=bTagsJP[0].discriminator();
       drTBJP1=DeltaR(Tau.p4(),(BJet1->p4()));
       
       if(bTagsJP.size()>1)
	 {
	   size_t iy=1;
	   edm::RefToBase<reco::Jet>  BRefJet2=bTagsJP[iy].jet();
	   const Jet *  BJet2=BRefJet2.get();
	   enerTrBjetJP2=BJet2->et();
	   b_discJP2=bTagsJP[1].discriminator();
	   drTBJP2=DeltaR(Tau.p4(),BJet2->p4());
	   
	   if(bTagsJP.size()>2)
	     {
	       size_t iz=2;
	       edm::RefToBase<reco::Jet>  BRefJet3=bTagsJP[iz].jet();
	       const Jet *  BJet3=BRefJet3.get();
	       enerTrBjetJP3=BJet3->et();
	       b_discJP3=bTagsJP[2].discriminator();
	       drTBJP3=DeltaR(Tau.p4(),BJet3->p4());
	       
	       if(bTagsJP.size()>3)
		 {
		   size_t iw=3;
		   edm::RefToBase<reco::Jet>  BRefJet4=bTagsJP[iw].jet();
		   const Jet *  BJet4=BRefJet4.get();
		   enerTrBjetJP4=BJet4->et();  
		   b_discJP4=bTagsJP[3].discriminator();
		   drTBJP4=DeltaR(Tau.p4(),BJet4->p4()); 
		 }
	     }
	 }  
       
     }
  


   float drTBMintmpTCHE=-1000;   
   
 
   for (size_t i = 0; i != bTagsTCHE.size(); ++i) {
     edm::RefToBase<reco::Jet>  BRefJet=bTagsTCHE[i].jet();
     Jet const*  BJet=BRefJet.get();
     
     if((bTagsTCHE[i].discriminator()>0)&&(BJet->pt()>10))Njet10TCHE++;
     if((bTagsTCHE[i].discriminator()>0)&&(BJet->pt()>15))Njet15TCHE++;
     if((bTagsTCHE[i].discriminator()>0)&&(BJet->pt()>20))Njet20TCHE++;
     
     if((DRMINTBTCHE<DRMINMBTCHE)&&(_flagRecoTau==1)&&(DeltaR(Tau.p4(),BJet->p4())<DRMINMBTCHE))
       {
	 // cout<<" ATTENZIONE STA TAGGANDO TAU-JET?"<<endl;
	 b_discTTCHE=bTagsTCHE[i].discriminator();
	 drTBMintmpTCHE=DeltaR(Tau.p4(),BJet->p4());
       } 

     
      
	 if(bTagsTCHE[i].discriminator()>0)
	   {
	     if(b_discPosFirstTCHE==-1000)
	       {
		 b_discPosFirstTCHE=bTagsTCHE[i].discriminator();
	       }
	     else if(b_discPosSecondTCHE==-1000)
	       {
		 b_discPosSecondTCHE=bTagsTCHE[i].discriminator();
	//	 cout<<"DUE B JET TAGGATI"<<endl;
	       }
	     else if (b_discPosThirdTCHE==-1000)
	       {
		 b_discPosThirdTCHE=bTagsTCHE[i].discriminator();
	       }
	   }
	 else b_discPosFourTCHE=bTagsTCHE[i].discriminator();	     
   }
   
  // cout<<" per il tau jet: b_disc "<<b_discTTCHE<<" delta r tra tau e questo jet "<<drTBMintmpTCHE<<" e DRMINTB "<<DRMINTBTCHE<<endl;  

   if(bTagsTCHE.size()>0)
     {
       size_t ix=0;
       edm::RefToBase<reco::Jet>  BRefJet1=bTagsTCHE[ix].jet();
       const Jet *  BJet1=BRefJet1.get();
       enerTrBjetTCHE1=BJet1->et();  
       b_discTCHE1=bTagsTCHE[0].discriminator();
       drTBTCHE1=DeltaR(Tau.p4(),(BJet1->p4()));
       
       if(bTagsTCHE.size()>1)
	 {
	   size_t iy=1;
	   edm::RefToBase<reco::Jet>  BRefJet2=bTagsTCHE[iy].jet();
	   const Jet *  BJet2=BRefJet2.get();
	   enerTrBjetTCHE2=BJet2->et();
	   b_discTCHE2=bTagsTCHE[1].discriminator();
	   drTBTCHE2=DeltaR(Tau.p4(),BJet2->p4());
	   
	   if(bTagsTCHE.size()>2)
	     {
	       size_t iz=2;
	       edm::RefToBase<reco::Jet>  BRefJet3=bTagsTCHE[iz].jet();
	       const Jet *  BJet3=BRefJet3.get();
	       enerTrBjetTCHE3=BJet3->et();
	       b_discTCHE3=bTagsTCHE[2].discriminator();
	       drTBTCHE3=DeltaR(Tau.p4(),BJet3->p4());
	       
	       if(bTagsTCHE.size()>3)
		 {
		   size_t iw=3;
		   edm::RefToBase<reco::Jet>  BRefJet4=bTagsTCHE[iw].jet();
		   const Jet *  BJet4=BRefJet4.get();
		   enerTrBjetTCHE4=BJet4->et();  
		   b_discTCHE4=bTagsTCHE[3].discriminator();
		   drTBTCHE4=DeltaR(Tau.p4(),BJet4->p4()); 
		 }
	     }
	 }  
       
     }
  

float drTBMintmpTCHP=-1000;   
   

   for (size_t i = 0; i != bTagsTCHP.size(); ++i) {
     edm::RefToBase<reco::Jet>  BRefJet=bTagsTCHP[i].jet();   
     Jet const*  BJet=BRefJet.get();
   
     if((bTagsTCHP[i].discriminator()>0)&&(BJet->pt()>10))Njet10TCHP++;
     if((bTagsTCHP[i].discriminator()>0)&&(BJet->pt()>15))Njet15TCHP++;
     if((bTagsTCHP[i].discriminator()>0)&&(BJet->pt()>20))Njet20TCHP++;


     if((DRMINTBTCHP<DRMINMBTCHP)&&(_flagRecoTau==1)&&(DeltaR(Tau.p4(),BJet->p4())<DRMINMBTCHP))
       {
//	 cout<<" ATTENZIONE STA TAGGANDO TAU-JET?"<<endl;
	 b_discTTCHP=bTagsTCHP[i].discriminator();
	 drTBMintmpTCHP=DeltaR(Tau.p4(),BJet->p4());
       } 

     
      
	 if(bTagsTCHP[i].discriminator()>0)
	   {
	     if(b_discPosFirstTCHP==-1000)
	       {
		 b_discPosFirstTCHP=bTagsTCHP[i].discriminator();
	       }
	     else if(b_discPosSecondTCHP==-1000)
	       {
		 b_discPosSecondTCHP=bTagsTCHP[i].discriminator();
		 //cout<<"DUE B JET TAGGATI"<<endl;
	       }
	     else if (b_discPosThirdTCHP==-1000)
	       {
		 b_discPosThirdTCHP=bTagsTCHP[i].discriminator();
	       }
	   }
	 else b_discPosFourTCHP=bTagsTCHP[i].discriminator();	     
   }
   
 //  cout<<" per il tau jet: b_disc "<<b_discTTCHP<<" delta r tra tau e questo jet "<<drTBMintmpTCHP<<" e DRMINTB "<<DRMINTBTCHP<<endl;  

   if(bTagsTCHP.size()>0)
     {
       size_t ix=0;
       edm::RefToBase<reco::Jet>  BRefJet1=bTagsTCHP[ix].jet();
       const Jet *  BJet1=BRefJet1.get();
       enerTrBjetTCHP1=BJet1->et();  
       b_discTCHP1=bTagsTCHP[0].discriminator();
       drTBTCHP1=DeltaR(Tau.p4(),(BJet1->p4()));
       
       if(bTagsTCHP.size()>1)
	 {
	   size_t iy=1;
	   edm::RefToBase<reco::Jet>  BRefJet2=bTagsTCHP[iy].jet();
	   const Jet *  BJet2=BRefJet2.get();
	   enerTrBjetTCHP2=BJet2->et();
	   b_discTCHP2=bTagsTCHP[1].discriminator();
	   drTBTCHP2=DeltaR(Tau.p4(),BJet2->p4());
	   
	   if(bTagsTCHP.size()>2)
	     {
	       size_t iz=2;
	       edm::RefToBase<reco::Jet>  BRefJet3=bTagsTCHP[iz].jet();
	       const Jet *  BJet3=BRefJet3.get();
	       enerTrBjetTCHP3=BJet3->et();
	       b_discTCHP3=bTagsTCHP[2].discriminator();
	       drTBTCHP3=DeltaR(Tau.p4(),BJet3->p4());
	       
	       if(bTagsTCHP.size()>3)
		 {
		   size_t iw=3;
		   edm::RefToBase<reco::Jet>  BRefJet4=bTagsTCHP[iw].jet();
		   const Jet *  BJet4=BRefJet4.get();
		   enerTrBjetTCHP4=BJet4->et();  
		   b_discTCHP4=bTagsTCHP[3].discriminator();
		   drTBTCHP4=DeltaR(Tau.p4(),BJet4->p4()); 
		 }
	     }
	 }  
       
     }
  

//      cout<<"b_discSM1 "<<b_discSM1<<" b_discSM2 "<<b_discSM2<<" b_discSM3 "<<b_discSM3<<" b_discSM4 "<<b_discSM4<<endl; 
//      cout<<"energie trasverse da 1 a 4 "<<enerTrBjetSM1<<" "<<enerTrBjetSM2<<" "<<enerTrBjetSM3<<" "<<enerTrBjetSM4<<endl; 
//     cout<<"dr da 1 a 4 "<<drTBSM1<<" "<<drTBSM2<<" "<<drTBSM3<<" "<<drTBSM4<<endl; 

//    cout<<"b_discSMNoIP1 "<<b_discSMNoIP1<<" b_discSMNoIP2 "<<b_discSMNoIP2<<" b_discSMNoIP3 "<<b_discSMNoIP3<<" b_discSMNoIP4 "<<b_discSMNoIP4<<endl; 
//      cout<<"energie trasverse da 1 a 4 "<<enerTrBjetSMNoIP1<<" "<<enerTrBjetSMNoIP2<<" "<<enerTrBjetSMNoIP3<<" "<<enerTrBjetSMNoIP4<<endl; 
//     cout<<"dr da 1 a 4 "<<drTBSMNoIP1<<" "<<drTBSMNoIP2<<" "<<drTBSMNoIP3<<" "<<drTBSMNoIP4<<endl; 

//    cout<<"b_discJP1 "<<b_discJP1<<" b_discJP2 "<<b_discJP2<<" b_discJP3 "<<b_discJP3<<" b_discJP4 "<<b_discJP4<<endl; 
//      cout<<"energie trasverse da 1 a 4 "<<enerTrBjetJP1<<" "<<enerTrBjetJP2<<" "<<enerTrBjetJP3<<" "<<enerTrBjetJP4<<endl; 
//     cout<<"dr da 1 a 4 "<<drTBJP1<<" "<<drTBJP2<<" "<<drTBJP3<<" "<<drTBJP4<<endl; 

//    cout<<"b_discTCHE1 "<<b_discTCHE1<<" b_discTCHE2 "<<b_discTCHE2<<" b_discTCHE3 "<<b_discTCHE3<<" b_discTCHE4 "<<b_discTCHE4<<endl; 
//      cout<<"energie trasverse da 1 a 4 "<<enerTrBjetTCHE1<<" "<<enerTrBjetTCHE2<<" "<<enerTrBjetTCHE3<<" "<<enerTrBjetTCHE4<<endl; 
//     cout<<"dr da 1 a 4 "<<drTBTCHE1<<" "<<drTBTCHE2<<" "<<drTBTCHE3<<" "<<drTBTCHE4<<endl; 

//    cout<<"b_discTCHP1 "<<b_discTCHP1<<" b_discTCHP2 "<<b_discTCHP2<<" b_discTCHP3 "<<b_discTCHP3<<" b_discTCHP4 "<<b_discTCHP4<<endl; 
//      cout<<"energie trasverse da 1 a 4 "<<enerTrBjetTCHP1<<" "<<enerTrBjetTCHP2<<" "<<enerTrBjetTCHP3<<" "<<enerTrBjetTCHP4<<endl; 
//     cout<<"dr da 1 a 4 "<<drTBTCHP1<<" "<<drTBTCHP2<<" "<<drTBTCHP3<<" "<<drTBTCHP4<<endl; 

 
//cout<<"Num Found Muons= "<<NumMuons<<" and Num Old Isol Muons= "<<NumOldIsolMu<<" and Num New Isol Muons= "<<NumNewIsolMu<<endl;
 
  




  

   ///////////////////        MC MATCHING

   if(NumOldIsolMu==1)
{
   const CandidateRef MCTruthMatchRef=MCTruthMatching(iEvent);
  
   if(MCTruthMatchRef.isNonnull()){
   const Candidate & MCTruthMatch=*MCTruthMatchRef; 
   //cout<<"2"<<endl;
    const Candidate*  Mother=(MCTruthMatch).mother();
    //cout<<"3"<<endl;
    const Candidate & mother=*Mother; 
    //cout<<"4"<<endl;
    //unsigned int size_mc=(MCTruthMatch).numberOf();
 pdgId_cand=(MCTruthMatch).pdgId();
 //cout<<"5"<<endl;
 size_moth=(MCTruthMatch).numberOfMothers();
 //cout<<"6"<<endl;
 pdgId_moth=mother.pdgId();
 //cout<<"7"<<endl;
// cout<<"pdg Id mother"<<pdgId_moth<<endl;
 //cout<<"8"<<endl;
}
}   
   else cout<<"no isol muons"<<endl;


   ///////////////////   MC MATCHING LEAD TRACK TAU

   int k=0;  
     
  CandidateRef  MCtrueCandRef; 
Handle <CandidateCollection> CollT;
iEvent.getByLabel(TrackCand,CollT);

Handle<CandMatchMap> matchl;
iEvent.getByLabel(GenMatchL,matchl);


//size_t size=CollT->size();

 for (PFTauCollection::size_type iTau = 0; iTau < thePFTauHandle->size()&&NTauCand<NTAUMAX; iTau++) {
   
     PFTauRef theTau(thePFTauHandle,iTau );
     if(!theTau)continue;
     const PFTau& tau = (*theTau);
      PFCandidateRef LeadPFCand = tau.leadPFChargedHadrCand();
      if (!LeadPFCand) continue;  
      TrackRef TauLdgTrack=tau.leadTrack();
      
      Track LdgTrack=*TauLdgTrack;
     // cout<<"lead track pt "<<(*LeadPFCand).pt()<<endl;;
      
    
  
 
  sizeT=CollT->size();
  
// cout<<"sizeCollLT "<<sizeT<<endl;   

 //TrackCollection MyTrackCollection=*(tracks.product());

 //std::sort(CollT.begin(),CollT.end(),ComparePt());

 for( int c=0; c<sizeT; ++c){      
   CandidateRef Ref(CollT, c);
   float ptRef=Ref->pt();
   //cout<<" pt ref "<<Ref->pt()<<endl;
   if(fabs((*LeadPFCand).pt()-ptRef)<0.1)
{

 // cout<<" pt: "<<ptRef<<" "<<(*LeadPFCand).pt()<<endl;


   CandMatchMap::const_iterator trueIter = matchl->find(Ref);
   //if(trueIter==matchl->end())cout<<"caso 1"<<endl;
   //if(trueIter!=matchl->end())cout<<"caso 2"<<endl;

   if(trueIter!=matchl->end()){
     

     CandidateRef trueRef=trueIter->val;
     //cout<<" val del match "<<&(trueIter->val)<<endl;
     if(trueRef.isNonnull()){
       MCtrueCandRef=trueRef;
       const CandidateRef MCTruthMatchRef=MCtrueCandRef;
       
       if(MCTruthMatchRef.isNonnull()){
	 const Candidate & MCTruthMatch=*MCTruthMatchRef; 
	 
	 const Candidate*  Mother=(MCTruthMatch).mother();
	 
	 const Candidate & mother=*Mother; 
	 
	 pdgId_candLT[k]=(MCTruthMatch).pdgId();
	 
	 size_mothLT[k]=(MCTruthMatch).numberOfMothers();
	 
	 pdgId_mothLT[k]=mother.pdgId();
	 
	// cout<<"pdg Id mother   LDG TRACK"<<pdgId_mothLT[k]<<endl;
	 k++;
	 
} 	 
       }
     }
   }
 }

}

PFRecoVisEnergyZ=-500.;
if((_flagRecoTau==true)&&(FlagOldRecoMu==true)){
  QualityEvent=true;

    PFRecoEnergyZ=tau_energy+OldIsoEnMu+CaloMET_time;     
    PFRecoP_XZ=tau_px+OldIsoPXMu+CaloMET_x;
    PFRecoP_YZ=tau_py+OldIsoPYMu+CaloMET_y;  
    PFRecoP_ZZ=tau_pz+OldIsoPZMu;

    PFRecoVisP_XZ=tau_px+OldIsoPXMu;
    PFRecoVisP_YZ=tau_py+OldIsoPYMu;
    PFRecoVisP_ZZ=tau_pz+OldIsoPZMu;
     
    PFRecoP_Z=sqrt((PFRecoP_XZ*PFRecoP_XZ)+(PFRecoP_YZ*PFRecoP_YZ)+(PFRecoP_ZZ*PFRecoP_ZZ));
    PFRecoVisP_Z=sqrt((PFRecoVisP_XZ*PFRecoVisP_XZ)+(PFRecoVisP_YZ*PFRecoVisP_YZ)+(PFRecoVisP_ZZ*PFRecoVisP_ZZ));
    PFRecoP_TZ=sqrt((PFRecoP_XZ*PFRecoP_XZ)+(PFRecoP_YZ*PFRecoP_YZ));
    
    PFRecoTrEnergyZ=sqrt((PFRecoEnergyZ*PFRecoEnergyZ)-(PFRecoP_ZZ*PFRecoP_ZZ));

    PFRecoVisEnergyZ=tau_energy+OldIsoEnMu;
    
    PFRecoTrVisEnergyZ=sqrt((PFRecoVisEnergyZ*PFRecoVisEnergyZ)-(PFRecoP_ZZ*PFRecoP_ZZ));
     
    
     ///////////MASSE    
    PFRecoTrMassZ_B=sqrt((PFRecoEnergyZ*PFRecoEnergyZ)-(PFRecoP_ZZ*PFRecoP_ZZ));   
    
    PFRecoTrVisMassZ_rel=sqrt((PFRecoVisEnergyZ*PFRecoVisEnergyZ)-(PFRecoVisP_ZZ*PFRecoVisP_ZZ));    
    
    
    PFRecoInvMassZ_A=sqrt((PFRecoEnergyZ*PFRecoEnergyZ)-(PFRecoP_XZ*PFRecoP_XZ)-(PFRecoP_YZ*PFRecoP_YZ)-(PFRecoP_ZZ*PFRecoP_ZZ));
    
    PFRecoInvMassZ_C=sqrt((PFRecoTrEnergyZ*PFRecoTrEnergyZ)-(PFRecoP_TZ*PFRecoP_TZ));
    
    PFRecoInvMassZ_B=sqrt(((tau_energy+OldIsoEnMu+CaloMET_time)*(tau_energy+OldIsoEnMu+CaloMET_time))-((tau_px+OldIsoPXMu+CaloMET_x)*(tau_px+OldIsoPXMu+CaloMET_x))-((tau_py+OldIsoPYMu+CaloMET_y)*(tau_py+OldIsoPYMu+CaloMET_y))-((tau_pz+OldIsoPZMu)*(tau_pz+OldIsoPZMu)));
    
    
    PFRecoTrMassZ_A=sqrt((PFRecoTrEnergyZ*PFRecoTrEnergyZ)-(PFRecoP_XZ*PFRecoP_XZ)-(PFRecoP_YZ*PFRecoP_YZ));
    
    PFRecoTrMassZ_C=sqrt((PFRecoTrEnergyZ*PFRecoTrEnergyZ)-(PFRecoP_TZ*PFRecoP_TZ));
    
    PFRecoTrVisMassZ=sqrt((PFRecoTrVisEnergyZ*PFRecoTrVisEnergyZ)-(PFRecoVisP_XZ*PFRecoVisP_XZ)-(PFRecoVisP_YZ*PFRecoVisP_YZ));
    
    
    PFRecoVisMassZ=sqrt((tau_energy+OldIsoEnMu)*(tau_energy+OldIsoEnMu)-(tau_px+OldIsoPXMu)*(tau_px+OldIsoPXMu)-(tau_py+OldIsoPYMu)*(tau_py+OldIsoPYMu)-(tau_pz+OldIsoPZMu)*(tau_pz+OldIsoPZMu));   
  }

//deltaphiMuMET=OldIsoPhiMu-phiMET;
//   deltaphiTauMET=phiT-phiMET;
//   deltaphiTM=phiT-OldIsoPhiMu;
//   cosDeltaPhiMuMET=cos(deltaphiMuMET);
//   cosDeltaPhiTauMET=cos(deltaphiTauMET);
//   cosDeltaPhiTM=cos(deltaphiTM);


 for(NMuCand=0;NMuCand<MuSize;NMuCand++){
   DeltaphiMuMET[NMuCand]=Muon_phi[NMuCand]-phiMET;
   CosDeltaPhiMuMET[NMuCand]=cos(DeltaphiMuMET[NMuCand]);
   //cout<<"1"<<endl;
 }
 for(NTauCand=0;NTauCand<TauSize;NTauCand++){           
   DeltaphiTauMET[NTauCand]=PhiT[NTauCand]-phiMET;
   CosDeltaPhiTauMET[NTauCand]=cos(DeltaphiTauMET[NTauCand]);
   //cout<<"2"<<endl;
 }



for(MuonCollection::const_iterator iT =myMuonCollection.begin();iT !=myMuonCollection.end()&&NMuCand<NMUMAX;++iT){ 
 for (PFTauCollection::size_type iTau = 0; iTau < thePFTauHandle->size()&&NTauCand<NTAUMAX; iTau++) {
     
     PFTauRef theTau(thePFTauHandle, iTau);
     if (!theTau) {
       continue;
     }
     const PFTau& tau = (*theTau);
     PFCandidateRef theLeadPFCand = tau.leadPFChargedHadrCand();
     if (!theLeadPFCand) {      
           continue;
     }

     ////////metti tracce
 }
 }


 cout<<"mu size "<<MuSize<<" and tau size "<<TauSize<<endl;
 
 for(NMuCand=0;NMuCand<MuSize;NMuCand++){
   for(NTauCand=0;NTauCand<TauSize;NTauCand++){  
     //cout<<"nel ciclo"<<endl; 
     DeltaphiTM[NMuCand][NTauCand]=PhiT[NTauCand]-Muon_phi[NMuCand];
     CosDeltaPhiTM[NMuCand][NTauCand]=cos(DeltaphiTM[NMuCand][NTauCand]);
     
     EnZ[NMuCand][NTauCand]=Tau_energy[NTauCand]+Muon_en[NMuCand]+CaloMET_time;     
     PXZ[NMuCand][NTauCand]=Tau_px[NTauCand]+Muon_px[NMuCand]+CaloMET_x;
     PYZ[NMuCand][NTauCand]=Tau_py[NTauCand]+Muon_py[NMuCand]+CaloMET_y;  
     PZZ[NMuCand][NTauCand]=Tau_pz[NTauCand]+Muon_pz[NMuCand];
     
     VisPXZ[NMuCand][NTauCand]=Tau_px[NTauCand]+Muon_px[NMuCand];
     VisPYZ[NMuCand][NTauCand]=Tau_py[NTauCand]+Muon_py[NMuCand];
     VisPZZ[NMuCand][NTauCand]=Tau_pz[NTauCand]+Muon_pz[NMuCand];
     
     PZ[NMuCand][NTauCand]=sqrt((PXZ[NMuCand][NTauCand]*PXZ[NMuCand][NTauCand])+(PYZ[NMuCand][NTauCand]*PYZ[NMuCand][NTauCand])+(PZZ[NMuCand][NTauCand]*PZZ[NMuCand][NTauCand]));
     VisPZ[NMuCand][NTauCand]=sqrt((VisPXZ[NMuCand][NTauCand]*VisPXZ[NMuCand][NTauCand])+(VisPYZ[NMuCand][NTauCand]*VisPYZ[NMuCand][NTauCand])+(VisPZZ[NMuCand][NTauCand]*VisPZZ[NMuCand][NTauCand]));
     PTZ[NMuCand][NTauCand]=sqrt((PXZ[NMuCand][NTauCand]*PXZ[NMuCand][NTauCand])+(PYZ[NMuCand][NTauCand]*PYZ[NMuCand][NTauCand]));
     
     TrEnZ[NMuCand][NTauCand]=sqrt((EnZ[NMuCand][NTauCand]*EnZ[NMuCand][NTauCand])-(PZZ[NMuCand][NTauCand]*PZZ[NMuCand][NTauCand]));
     VisEnZ[NMuCand][NTauCand]=Tau_energy[NTauCand]+Muon_en[NMuCand];    
     TrVisEnZ[NMuCand][NTauCand]=sqrt((VisEnZ[NMuCand][NTauCand]*VisEnZ[NMuCand][NTauCand])-(PZZ[NMuCand][NTauCand]*PZZ[NMuCand][NTauCand]));
     
     
     ///////////MASSE    
      TrMassZ_B[NMuCand][NTauCand]=sqrt((EnZ[NMuCand][NTauCand]*EnZ[NMuCand][NTauCand])-(PZZ[NMuCand][NTauCand]*PZZ[NMuCand][NTauCand]));       
      TrVisMassZ_rel[NMuCand][NTauCand]=sqrt((VisEnZ[NMuCand][NTauCand]*VisEnZ[NMuCand][NTauCand])-(VisPZZ[NMuCand][NTauCand]*VisPZZ[NMuCand][NTauCand]));    
     
     
   InvMassZ_A[NMuCand][NTauCand]=sqrt((EnZ[NMuCand][NTauCand]*EnZ[NMuCand][NTauCand])-(PXZ[NMuCand][NTauCand]*PXZ[NMuCand][NTauCand])-(PYZ[NMuCand][NTauCand]*PYZ[NMuCand][NTauCand])-(PZZ[NMuCand][NTauCand]*PZZ[NMuCand][NTauCand]));    
     InvMassZ_C[NMuCand][NTauCand]=sqrt((TrEnZ[NMuCand][NTauCand]*TrEnZ[NMuCand][NTauCand])-(PTZ[NMuCand][NTauCand]*PTZ[NMuCand][NTauCand]));
     
      InvMassZ_B[NMuCand][NTauCand]=sqrt(((Tau_energy[NTauCand]+Muon_en[NMuCand]+CaloMET_time)*(Tau_energy[NTauCand]+Muon_en[NMuCand]+CaloMET_time))-((Tau_px[NTauCand]+Muon_px[NMuCand]+CaloMET_x)*(Tau_px[NTauCand]+Muon_px[NMuCand]+CaloMET_x))-((Tau_py[NTauCand]+Muon_py[NMuCand]+CaloMET_y)*(Tau_py[NTauCand]+Muon_py[NMuCand]+CaloMET_y))-((Tau_pz[NTauCand]+Muon_pz[NMuCand])*(Tau_pz[NTauCand]+Muon_pz[NMuCand])));
     
    
    TrMassZ_A[NMuCand][NTauCand]=sqrt((TrEnZ[NMuCand][NTauCand]*TrEnZ[NMuCand][NTauCand])-(PXZ[NMuCand][NTauCand]*PXZ[NMuCand][NTauCand])-(PYZ[NMuCand][NTauCand]*PYZ[NMuCand][NTauCand]));
   TrMassZ_C[NMuCand][NTauCand]=sqrt((TrEnZ[NMuCand][NTauCand]*TrEnZ[NMuCand][NTauCand])-(PTZ[NMuCand][NTauCand]*PTZ[NMuCand][NTauCand]));
    TrVisMassZ[NMuCand][NTauCand]=sqrt((TrVisEnZ[NMuCand][NTauCand]*TrVisEnZ[NMuCand][NTauCand])-(VisPXZ[NMuCand][NTauCand]*VisPXZ[NMuCand][NTauCand])-(VisPYZ[NMuCand][NTauCand]*VisPYZ[NMuCand][NTauCand]));   
    VisMassZ[NMuCand][NTauCand]=sqrt((Tau_energy[NTauCand]+Muon_en[NMuCand])*(Tau_energy[NTauCand]+Muon_en[NMuCand])-(Tau_px[NTauCand]+Muon_px[NMuCand])*(Tau_px[NTauCand]+Muon_px[NMuCand])-(Tau_py[NTauCand]+Muon_py[NMuCand])*(Tau_py[NTauCand]+Muon_py[NMuCand])-(Tau_pz[NTauCand]+Muon_pz[NMuCand])*(Tau_pz[NTauCand]+Muon_pz[NMuCand]));   
    }
  }

 std::cout << "\n now filling the tree" <<std::endl;      
 tree_->Fill();
 
}

void ZTauTauAnalysis::beginJob(const edm::EventSetup&)
{
  std::cout << "ZTauTauAnalysis Starting! \n "; 
  
 
}


void ZTauTauAnalysis::endJob() {
  cout<<"Analysis completed"<<endl;
}



float ZTauTauAnalysis::DeltaRMin(const reco::PFTau& tau, const reco::MuonCollection& myMuonCollection)
{
float  drcla=1000;

 for(MuonCollection::const_iterator iM =myMuonCollection.begin();iM !=myMuonCollection.end();iM++)	 
	 {
	   const Muon& MuonObj=(*iM);
	   TrackRef MuonTrack=MuonObj.track();
	   math::XYZVector MuonMomentumAtVtx =(*MuonTrack).innerMomentum(); 
	   OldIsoPhiMu=MuonMomentumAtVtx.Phi();
	   
	   //PFTauRef theTau(thePFTauHandle, iTau);
	   //const PFTau& tau = (*theTau);
	   //cout<<"Inizio ciclo tau"<<endl;
	   //energiatrastmpT=tau.et();
	   //cout<<"energiatrastmpT "<<energiatrastmpT<<" tau_et "<<tau_et<<endl;
	   
	   //tau_disc = (*thePFTauDiscriminatorByIsolation)[theTau];
	   //cout << "DiscriminatorByIsolation value: " << tau_disc << endl;
	   //if(tau_disc!=1)
	   //{
	   // cout<<"tau non isolato: "<<tau_disc<<endl;
	   //  continue;
	       
	   //}

	   math::XYZVector P1 = tau.momentum();
	   // phiT=P1.Phi();
	   drtmp = DeltaR(MuonMomentumAtVtx,P1);
	   if(drtmp<drcla) 
	     {
	       drcla=drtmp;
	     }


}
 
 return drcla;
 
}






CandidateRef ZTauTauAnalysis::MCTruthMatching(const edm::Event& iEvent)
{
CandidateRef  MCtrueCandRef;
Handle <CandidateCollection> Coll;
 iEvent.getByLabel(MuonCand,Coll);


 Handle<CandMatchMap> match;
 
 iEvent.getByLabel(GenMatch,match);
 
 
   size_t size=Coll->size();
   for( size_t c=0; c<size; ++c){
     
     
     CandidateRef Ref(Coll, c);
    
     CandMatchMap::const_iterator trueIter = match->find(Ref);
     
      if(trueIter!=match->end()){
       
     CandidateRef trueRef=trueIter->val;
       if(trueRef.isNonnull()){
 	 //cout<<"MONTECARLO"<<endl;
 
	 //const Candidate &MCtrue =*trueRef;
 MCtrueCandRef=trueRef;	 
       }
       else cout<<"trueRef non valida"<<endl;
      }
   }
 
   return MCtrueCandRef;
}


////////////////delta r tra jet che viene b-taggato e tau-jet

float ZTauTauAnalysis::DeltaRMinTBjet(const reco::PFTau& tau, const reco::JetTagCollection& bjets)
{
float  drtb=1000;



 cout<<"entrato nella funzione"<<endl;
 return drtb; 
}




float ZTauTauAnalysis::getTauCharge(const PFTau& tau) const
{
 float tch = 0.0;
 PFCandidateRefVector chHadCands = tau.signalPFChargedHadrCands();
 for (PFCandidateRefVector::const_iterator it  = chHadCands.begin();
                                           it != chHadCands.end();
                                           it++)
 {
   tch += (**it).charge();
 }

 return tch;
}
