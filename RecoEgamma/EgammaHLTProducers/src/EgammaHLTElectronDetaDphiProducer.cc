/** \class EgammaHLTElectronDetaDphiProducer
 *
 *  \author Roberto Covarelli (CERN)
 * 
 * $Id: EgammaHLTElectronDetaDphiProducer.cc,v 1.4 2011/12/19 11:17:28 sani Exp $
 *
 */

#include "RecoEgamma/EgammaHLTProducers/interface/EgammaHLTElectronDetaDphiProducer.h"

// Framework
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
#include "DataFormats/EgammaCandidates/interface/ElectronIsolationAssociation.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidateIsolation.h"

#include "DataFormats/Common/interface/RefToBase.h"

#include "DataFormats/Common/interface/RefProd.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"

//#include "CondFormats/DataRecord/interface/BeamSpotObjectsRcd.h"//needed?
//#include "CondFormats/BeamSpotObjects/interface/BeamSpotObjects.h"//needed?


#include "DataFormats/Math/interface/Point3D.h"

#include "RecoEgamma/EgammaTools/interface/ECALPositionCalculator.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"

EgammaHLTElectronDetaDphiProducer::EgammaHLTElectronDetaDphiProducer(const edm::ParameterSet& config) 
{

  electronProducer_             = config.getParameter<edm::InputTag>("electronProducer");
  bsProducer_                   = config.getParameter<edm::InputTag>("BSProducer");
  useTrackProjectionToEcal_     = config.getParameter<bool>("useTrackProjectionToEcal");
  recoEcalCandidateProducer_ = config.getParameter<edm::InputTag>("recoEcalCandidateProducer"); 
  useSCRefs_ = config.getParameter<bool>("useSCRefs");

  //register your products
  if(!useSCRefs_){
    produces < reco::ElectronIsolationMap >( "Deta" ).setBranchAlias( "deta" );
    produces < reco::ElectronIsolationMap >( "Dphi" ).setBranchAlias( "dphi" ); 
  }else{
    produces < reco::RecoEcalCandidateIsolationMap >( "Deta" ).setBranchAlias( "deta" );
    produces < reco::RecoEcalCandidateIsolationMap >( "Dphi" ).setBranchAlias( "dphi" ); 
  }
}

EgammaHLTElectronDetaDphiProducer::~EgammaHLTElectronDetaDphiProducer(){}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
EgammaHLTElectronDetaDphiProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // Get the HLT filtered objects
  edm::Handle<reco::ElectronCollection> electronHandle;
  iEvent.getByLabel(electronProducer_,electronHandle);

  edm::Handle<reco::BeamSpot> recoBeamSpotHandle;
  iEvent.getByLabel(bsProducer_,recoBeamSpotHandle);
  // gets its position
  const reco::BeamSpot::Point& bsPosition = recoBeamSpotHandle->position(); 

  edm::ESHandle<MagneticField> theMagField;
  iSetup.get<IdealMagneticFieldRecord>().get(theMagField);

  reco::ElectronIsolationMap detaMap;
  reco::ElectronIsolationMap dphiMap;
  reco::RecoEcalCandidateIsolationMap detaCandMap;
  reco::RecoEcalCandidateIsolationMap dphiCandMap;
  
  if(!useSCRefs_){
    for(reco::ElectronCollection::const_iterator iElectron = electronHandle->begin(); iElectron != electronHandle->end(); iElectron++){
      
      reco::ElectronRef eleref(reco::ElectronRef(electronHandle,iElectron - electronHandle->begin()));
      std::pair<float,float> dEtaDPhi = calDEtaDPhiSCTrk(eleref,bsPosition,theMagField.product());
      
      detaMap.insert(eleref, dEtaDPhi.first);
      dphiMap.insert(eleref, dEtaDPhi.second);
    }
  }else { //we loop over reco ecal candidates
     edm::Handle<reco::RecoEcalCandidateCollection> recoEcalCandHandle;
     iEvent.getByLabel(recoEcalCandidateProducer_,recoEcalCandHandle);
     for(reco::RecoEcalCandidateCollection::const_iterator iRecoEcalCand = recoEcalCandHandle->begin(); iRecoEcalCand != recoEcalCandHandle->end(); iRecoEcalCand++){
    
       reco::RecoEcalCandidateRef recoEcalCandRef(recoEcalCandHandle,iRecoEcalCand-recoEcalCandHandle->begin());
    
       reco::ElectronRef eleRef = getEleRef(recoEcalCandRef,electronHandle);
       std::pair<float,float> dEtaDPhi(999999,999999);
       if(eleRef.isNonnull()) dEtaDPhi =  calDEtaDPhiSCTrk(eleRef,bsPosition,theMagField.product());
       detaCandMap.insert(recoEcalCandRef, dEtaDPhi.first);
       dphiCandMap.insert(recoEcalCandRef, dEtaDPhi.second);
     }//end loop over reco ecal candidates
  }//end if between electrons or reco ecal candidates

  if(!useSCRefs_){
    std::auto_ptr<reco::ElectronIsolationMap> detMap(new reco::ElectronIsolationMap(detaMap));
    std::auto_ptr<reco::ElectronIsolationMap> dphMap(new reco::ElectronIsolationMap(dphiMap));
    iEvent.put(detMap, "Deta" );
    iEvent.put(dphMap, "Dphi" );
  }else{
    std::auto_ptr<reco::RecoEcalCandidateIsolationMap> detaCandMapForEvent(new reco::RecoEcalCandidateIsolationMap(detaCandMap));
    std::auto_ptr<reco::RecoEcalCandidateIsolationMap> dphiCandMapForEvent(new reco::RecoEcalCandidateIsolationMap(dphiCandMap));
    iEvent.put(detaCandMapForEvent, "Deta" );
    iEvent.put(dphiCandMapForEvent, "Dphi" );
  }     
}

std::pair<float,float> EgammaHLTElectronDetaDphiProducer::calDEtaDPhiSCTrk(reco::ElectronRef& eleref, const reco::BeamSpot::Point& bsPosition,const MagneticField *magField)
{
  const reco::SuperClusterRef theClus = eleref->superCluster();
  const math::XYZVector trackMom =  eleref->track()->momentum();
  
  math::XYZPoint SCcorrPosition(theClus->x()-bsPosition.x(), theClus->y()-bsPosition.y() , theClus->z()-eleref->track()->vz() );
  float deltaeta = fabs(SCcorrPosition.eta()-eleref->track()->eta());
  float deltaphi;
  
  if (useTrackProjectionToEcal_) { 
    
    ECALPositionCalculator posCalc;
    const math::XYZPoint vertex(bsPosition.x(),bsPosition.y(),eleref->track()->vz());
    
    float phi1= posCalc.ecalPhi(magField,trackMom,vertex,1);
    float phi2= posCalc.ecalPhi(magField,trackMom,vertex,-1);
    
    float deltaphi1=fabs( phi1 - theClus->position().phi() );
    if(deltaphi1>6.283185308) deltaphi1 -= 6.283185308;
    if(deltaphi1>3.141592654) deltaphi1 = 6.283185308-deltaphi1;

    float deltaphi2=fabs( phi2 - theClus->position().phi() );
    if(deltaphi2>6.283185308) deltaphi2 -= 6.283185308;
    if(deltaphi2>3.141592654) deltaphi2 = 6.283185308-deltaphi2;
    
    deltaphi = deltaphi1;
    if(deltaphi2<deltaphi1){ deltaphi = deltaphi2;}
  } else {
    
    deltaphi=fabs(eleref->track()->outerPosition().phi()-theClus->phi());
    if(deltaphi>6.283185308) deltaphi -= 6.283185308;
    if(deltaphi>3.141592654) deltaphi = 6.283185308-deltaphi;
  }
  
  return std::make_pair(deltaeta,deltaphi);
  
}

reco::ElectronRef EgammaHLTElectronDetaDphiProducer::getEleRef(const reco::RecoEcalCandidateRef& recoEcalCandRef,const edm::Handle<reco::ElectronCollection>& electronHandle)
{
  reco::ElectronRef eleRef;
  for(reco::ElectronCollection::const_iterator eleIt = electronHandle->begin(); eleIt != electronHandle->end(); eleIt++){
    if(eleIt->superCluster()==recoEcalCandRef->superCluster()){
      eleRef = reco::ElectronRef(electronHandle,eleIt - electronHandle->begin());
      break;
    }
  }
  return eleRef;
}
  

//define this as a plug-in
//DEFINE_FWK_MODULE(EgammaHLTTrackIsolationProducers);
