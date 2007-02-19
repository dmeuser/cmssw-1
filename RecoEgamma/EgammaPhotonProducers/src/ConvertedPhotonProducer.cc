#include <iostream>
#include <vector>
#include <memory>

// Framework
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
//
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaTrackReco/interface/TrackSuperClusterAssociation.h"
#include "DataFormats/EgammaCandidates/interface/ConvertedPhoton.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
//
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
//
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
//
#include "RecoEgamma/EgammaPhotonAlgos/interface/ConversionTrackPairFinder.h"
#include "RecoEgamma/EgammaPhotonAlgos/interface/ConversionVertexFinder.h"
#include "RecoEgamma/EgammaPhotonProducers/interface/ConvertedPhotonProducer.h"
//
#include "RecoTracker/Record/interface/CkfComponentsRecord.h"
#include "RecoTracker/Record/interface/TrackerRecoGeometryRecord.h"
//
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TrackTransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"

ConvertedPhotonProducer::ConvertedPhotonProducer(const edm::ParameterSet& config) : 
  conf_(config), 
  theNavigationSchool_(0), 
  isInitialized(0)

{


  LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer CTOR " << "\n";

 

  // use onfiguration file to setup input collection names
 
  bcProducer_             = conf_.getParameter<std::string>("bcProducer");
  bcBarrelCollection_     = conf_.getParameter<std::string>("bcBarrelCollection");
  bcEndcapCollection_     = conf_.getParameter<std::string>("bcEndcapCollection");
  
  scHybridBarrelProducer_       = conf_.getParameter<std::string>("scHybridBarrelProducer");
  scIslandEndcapProducer_       = conf_.getParameter<std::string>("scIslandEndcapProducer");
  
  scHybridBarrelCollection_     = conf_.getParameter<std::string>("scHybridBarrelCollection");
  scIslandEndcapCollection_     = conf_.getParameter<std::string>("scIslandEndcapCollection");
  
  conversionOITrackProducer_ = conf_.getParameter<std::string>("conversionOITrackProducer");
  conversionIOTrackProducer_ = conf_.getParameter<std::string>("conversionIOTrackProducer");

  outInTrackSCAssociationCollection_ = conf_.getParameter<std::string>("outInTrackSCAssociation");
  inOutTrackSCAssociationCollection_ = conf_.getParameter<std::string>("inOutTrackSCAssociation");



  // use onfiguration file to setup output collection names
  ConvertedPhotonCollection_     = conf_.getParameter<std::string>("convertedPhotonCollection");


  // Register the product
  produces< reco::ConvertedPhotonCollection >(ConvertedPhotonCollection_);


  // instantiate the Track Pair Finder algorithm
  theTrackPairFinder_ = new ConversionTrackPairFinder ();
  // instantiate the Vertex Finder algorithm
  theVertexFinder_ = new ConversionVertexFinder ();


}

ConvertedPhotonProducer::~ConvertedPhotonProducer() {


  delete theTrackPairFinder_;
  delete theVertexFinder_;

}


void  ConvertedPhotonProducer::beginJob (edm::EventSetup const & theEventSetup) {

  // Inizilize my global event counter
  nEvt_=0;

  //get magnetic field
  edm::LogInfo("ConvertedPhotonProducer") << " get magnetic field" << "\n";
  theEventSetup.get<IdealMagneticFieldRecord>().get(theMF_);  


  theEventSetup.get<TrackerRecoGeometryRecord>().get( theGeomSearchTracker_ );


  // get the measurement tracker   
  edm::ESHandle<MeasurementTracker> measurementTrackerHandle;
  theEventSetup.get<CkfComponentsRecord>().get(measurementTrackerHandle);
  theMeasurementTracker_ = measurementTrackerHandle.product();
  
  theLayerMeasurements_  = new LayerMeasurements(theMeasurementTracker_);
  theNavigationSchool_   = new SimpleNavigationSchool( &(*theGeomSearchTracker_)  , &(*theMF_));
  NavigationSetter setter( *theNavigationSchool_);
  
  
}


void  ConvertedPhotonProducer::endJob () {

  edm::LogInfo("ConvertedPhotonProducer") << " Analyzed " << nEvt_  << "\n";
 LogDebug("ConvertedPhotonProducer") << "ConvertedPhotonProducer::endJob Analyzed " << nEvt_ << " events " << "\n";
 

}

void ConvertedPhotonProducer::produce(edm::Event& theEvent, const edm::EventSetup& theEventSetup) {
  
  using namespace edm;
  nEvt_++;  
  LogInfo("ConvertedPhotonProducer") << "Analyzing event number: " << theEvent.id() << " Global counter " << nEvt_  << "\n";
  LogDebug("ConvertedPhotonProducer") << "ConvertedPhotonProduce::produce event number " <<   theEvent.id() << " Global counter " << nEvt_ << "\n";
 
  
  //
  // create empty output collections
  //

  // Converted photon candidates
  reco::ConvertedPhotonCollection outputConvPhotonCollection;
  std::auto_ptr< reco::ConvertedPhotonCollection > outputConvPhotonCollection_p(new reco::ConvertedPhotonCollection);
 LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer Created empty ConvertedPhotonCollection size " <<   "\n";


  // Get the Super Cluster collection in the Barrel
  Handle<reco::SuperClusterCollection> scBarrelHandle;
  theEvent.getByLabel(scHybridBarrelProducer_,scHybridBarrelCollection_,scBarrelHandle);
 LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer Trying to access " << scHybridBarrelCollection_.c_str() << "  from my Producer " << "\n";
  
  reco::SuperClusterCollection scBarrelCollection = *(scBarrelHandle.product());
 LogDebug("ConvertedPhotonProducer") << "ConvertedPhotonProducer barrel  SC collection size  " << scBarrelCollection.size() << "\n";
  
  // Get the Super Cluster collection in the Endcap
  Handle<reco::SuperClusterCollection> scEndcapHandle;
  theEvent.getByLabel(scIslandEndcapProducer_,scIslandEndcapCollection_,scEndcapHandle);
 LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer Trying to access " <<scIslandEndcapCollection_.c_str() << "  from my Producer " << "\n";
  
  reco::SuperClusterCollection scEndcapCollection = *(scEndcapHandle.product());
 LogDebug("ConvertedPhotonProducer") << "ConvertedPhotonProducer Endcap SC collection size  " << scEndcapCollection.size() << "\n";

  //// Get the Out In CKF tracks from conversions
  Handle<reco::TrackCollection> outInTrkHandle;
  theEvent.getByLabel(conversionOITrackProducer_,  outInTrkHandle);
 LogDebug("ConvertedPhotonProducer") << "ConvertedPhotonProducer outInTrack collection size " << (*outInTrkHandle).size() << "\n";

 // Loop over Out In Tracks
  for( reco::TrackCollection::const_iterator  iTk =  (*outInTrkHandle).begin(); iTk !=  (*outInTrkHandle).end(); iTk++) {
   LogDebug("ConvertedPhotonProducer") << "ConvertedPhotonProducer Out In Track charge " << iTk->charge() << " Num of RecHits " << iTk->recHitsSize() << " inner momentum " << iTk->innerMomentum() << "\n";  
    
   LogDebug("ConvertedPhotonProducer") << "ConvertedPhotonProducer Out In Track Extra inner momentum  " << iTk->extra()->outerMomentum() << "\n";  
   
  }
  //// Get the association map between CKF out in tracks and the SC where they originated
  Handle<reco::TrackSuperClusterAssociationCollection> outInTrkSCAssocHandle;
  theEvent.getByLabel( conversionOITrackProducer_, outInTrackSCAssociationCollection_, outInTrkSCAssocHandle);
  reco::TrackSuperClusterAssociationCollection outInTrackSCAss = *outInTrkSCAssocHandle;  
  LogDebug("ConvertedPhotonProducer")  << " ConvertedPhotonProducer outInTrackSCAssoc collection size " << (*outInTrkSCAssocHandle).size() <<"\n";


  //// Get the In Out  CKF tracks from conversions
  Handle<reco::TrackCollection> inOutTrkHandle;
  theEvent.getByLabel(conversionIOTrackProducer_, inOutTrkHandle);
  LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer inOutTrack collection size " << (*inOutTrkHandle).size() << "\n";
  //// Get the association map between CKF in out tracks and the SC where they originated
  Handle<reco::TrackSuperClusterAssociationCollection> inOutTrkSCAssocHandle;
  theEvent.getByLabel( conversionIOTrackProducer_, inOutTrackSCAssociationCollection_, inOutTrkSCAssocHandle);
  reco::TrackSuperClusterAssociationCollection inOutTrackSCAss = *inOutTrkSCAssocHandle;  
  LogDebug("ConvertedPhotonProducer")  << " ConvertedPhotonProducer inOutTrackSCAssoc collection size " << (*inOutTrkSCAssocHandle).size() <<"\n";



  // Transform Track into TransientTrack (needed by the Vertex fitter)
  edm::ESHandle<TransientTrackBuilder> theTransientTrackBuilder;
  theEventSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theTransientTrackBuilder);
  //do the conversion:
  std::vector<reco::TransientTrack> t_outInTrk = ( *theTransientTrackBuilder ).build(outInTrkHandle );
  std::vector<reco::TransientTrack> t_inOutTrk = ( *theTransientTrackBuilder ).build(inOutTrkHandle );
  

  reco::ConvertedPhotonCollection myConvPhotons;

  //  Loop over SC in the barrel and reconstruct converted photons
  int myCands=0;
  int iSC=0; // index in photon collection
  int lSC=0; // local index on barrel
  reco::SuperClusterCollection::iterator aClus;
  for(aClus = scBarrelCollection.begin(); aClus != scBarrelCollection.end(); aClus++) {
    
    //    if ( abs( aClus->eta() ) > 0.9 ) return; 
   LogDebug("ConvertedPhotonProducer") << "ConvertedPhotonProducer SC energy " << aClus->energy() << " eta " <<  aClus->eta() << " phi " <<  aClus->phi() << "\n";
    
        
    ///// Find the +/- pairs
    std::map<std::vector<reco::TransientTrack>, reco::SuperCluster> allPairs = theTrackPairFinder_->run(t_outInTrk, outInTrkHandle, outInTrkSCAssocHandle, t_inOutTrk, inOutTrkHandle, inOutTrkSCAssocHandle  );


    LogDebug("ConvertedPhotonProducer") << "ConvertedPhotonProducer Barrel  allPairs.size " << allPairs.size() << "\n";
    std::vector<edm::Ref<reco::TrackCollection> > trackPairRef;

    //// Set here first quantities for the converted photon
    const reco::Particle::Point  vtx( 0, 0, 0 );
    math::XYZPoint convVtx(0.,0.,0.);

    math::XYZVector direction =aClus->position() - vtx;
    math::XYZVector momentum = direction.unit() * aClus->energy();
    const reco::Particle::LorentzVector  p4(momentum.x(), momentum.y(), momentum.z(), aClus->energy() );

    
    if ( allPairs.size() ) {
      
      //      for ( std::vector<std::vector<reco::TransientTrack> >::const_iterator iPair= allPairs.begin(); iPair!= allPairs.end(); ++iPair ) {
      for (  std::map<std::vector<reco::TransientTrack>, reco::SuperCluster>::const_iterator iPair= allPairs.begin(); iPair!= allPairs.end(); ++iPair ) {
	LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer Barrel single pair size " << (iPair->first).size() << " SC Energy " << (iPair->second).energy() << " eta " << (iPair->second).eta() << " phi " <<  (iPair->second).phi() << "\n";  
	
	if ( (iPair->second).energy() != aClus->energy() ) continue;        

	CachingVertex theConversionVertex;
	if (  allPairs.size() > 1 ) theConversionVertex=theVertexFinder_->run(iPair->first);

        if ( theConversionVertex.isValid() ) {	
	  convVtx.SetXYZ( theConversionVertex.position().x(), theConversionVertex.position().y(),  theConversionVertex.position().z() );
	 LogDebug("ConvertedPhotonProducer") << "  ConvertedPhotonProducer conversion vertex position " << theConversionVertex.position() << "\n";
	} else {
	 LogDebug("ConvertedPhotonProducer") << "  ConvertedPhotonProducer conversion vertex is not valid set the position to (0,0,0) " << "\n";
	}
	 
	  
	//// loop over tracks in the pair for creating a reference
	trackPairRef.clear();
	for ( std::vector<reco::TransientTrack>::const_iterator iTk=(iPair->first).begin(); iTk!= (iPair->first).end(); ++iTk) {
	 LogDebug("ConvertedPhotonProducer") << "  ConvertedPhotonProducer Transient Tracks in the pair  charge " << iTk->charge() << " Num of RecHits " << iTk->recHitsSize() << " inner momentum " << iTk->track().innerMomentum() << "\n";  
	  
	  const reco::TrackTransientTrack* ttt = dynamic_cast<const reco::TrackTransientTrack*>(iTk->basicTransientTrack());
	  reco::TrackRef myTkRef= ttt->persistentTrackRef(); 

	 LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer Ref to Rec Tracks in the pair  charge " << myTkRef->charge() << " Num of RecHits " << myTkRef->recHitsSize() << " inner momentum " << myTkRef->innerMomentum() << "\n";  
	  
	  
	  trackPairRef.push_back(myTkRef);
	  
	}
	  
	  
	LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer trackPairRef  " << trackPairRef.size() <<  "\n";
	LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer SC energy " <<  aClus->energy() << "\n";
	LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer photon p4 " << p4  << "\n";
	LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer vtx " << vtx.x() << " " << vtx.y() << " " << vtx.z() << "\n";
	LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer convVtx " << convVtx.x() << " " << convVtx.y() << " " << convVtx.z() << "\n";
	LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer trackPairRef  " << trackPairRef.size() <<  "\n";
	
	reco::ConvertedPhoton  newCandidate(0, p4, vtx, convVtx);

	outputConvPhotonCollection.push_back(newCandidate);
	// set the reference to the SC
	reco::SuperClusterRef scRef(reco::SuperClusterRef(scBarrelHandle, lSC));
	outputConvPhotonCollection[iSC].setSuperCluster(scRef);
	// set the reference to the tracks
	outputConvPhotonCollection[iSC].setTrackPairRef( trackPairRef);
	
	
	
	iSC++;	
	myCands++;
	LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer Put the ConvertedPhotonCollection a candidate in the Barrel " << "\n";
	  
      }
      
    } else {
      
      LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer GOLDEN PHOTON ?? Zero Tracks " <<  "\n";  
      LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer SC energy " <<  aClus->energy() << "\n";
      LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer photon p4 " << p4  << "\n";
      LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer vtx " << vtx.x() << " " << vtx.y() << " " << vtx.z() << "\n";
      LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer convVtx " << convVtx.x() << " " << convVtx.y() << " " << convVtx.z() << "\n";
      LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer trackPairRef  " << trackPairRef.size() <<  "\n";
      reco::ConvertedPhoton  newCandidate(0, p4, vtx, convVtx);
     
      outputConvPhotonCollection.push_back(newCandidate);
      reco::SuperClusterRef scRef(reco::SuperClusterRef(scBarrelHandle, lSC));
      outputConvPhotonCollection[iSC].setSuperCluster(scRef);
      
      iSC++;	
      myCands++;
     LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer Put the ConvertedPhotonCollection a candidate in the Barrel " << "\n";
      
    }
          
    
    lSC++;
    
  }
  

  //  Loop over SC in the Endcap and reconstruct converted photons

  lSC=0; // reset local index for endcap
  for(aClus = scEndcapCollection.begin(); aClus != scEndcapCollection.end(); aClus++) {
    
    //    if ( abs( aClus->eta() ) > 0.9 ) return; 
    LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer SC energy " << aClus->energy() << " eta " <<  aClus->eta() << " phi " <<  aClus->phi() << "\n";
    
    
    ///// Find the +/- pairs
    std::map<std::vector<reco::TransientTrack>, reco::SuperCluster> allPairs = theTrackPairFinder_->run(t_outInTrk, outInTrkHandle, outInTrkSCAssocHandle, t_inOutTrk, inOutTrkHandle, inOutTrkSCAssocHandle  );
    
    LogDebug("ConvertedPhotonProducer") << "ConvertedPhotonProducer Endcap  allPairs.size " << allPairs.size() << "\n";

    std::vector<edm::Ref<reco::TrackCollection> > trackPairRef;
    
    //// Set here first quantities for the converted photon
    const reco::Particle::Point  vtx( 0, 0, 0 );
    math::XYZPoint convVtx(0.,0.,0.);
    
    math::XYZVector direction =aClus->position() - vtx;
    math::XYZVector momentum = direction.unit() * aClus->energy();
    const reco::Particle::LorentzVector  p4(momentum.x(), momentum.y(), momentum.z(), aClus->energy() );
    
    
    if ( allPairs.size() ) {
      for (  std::map<std::vector<reco::TransientTrack>, reco::SuperCluster>::const_iterator iPair= allPairs.begin(); iPair!= allPairs.end(); ++iPair ) {
	LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer Endcap single pair size " << (iPair->first).size() << " SC Energy " << (iPair->second).energy() << " eta " << (iPair->second).eta() << " phi " <<  (iPair->second).phi() << "\n";  
	
	if ( (iPair->second).energy() != aClus->energy() ) continue;
	
	CachingVertex theConversionVertex;
	if (  allPairs.size() > 1 ) theConversionVertex=theVertexFinder_->run(iPair->first);

        if ( theConversionVertex.isValid() ) {	
	  convVtx.SetXYZ( theConversionVertex.position().x(), theConversionVertex.position().y(),  theConversionVertex.position().z() );
	 LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer conversion vertex position " << theConversionVertex.position() << "\n";
	} else {
	 LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer conversion vertex is not valid " << "\n";
	}
	 
	  
	//// loop over tracks in the pair for creating a reference
	trackPairRef.clear();
	for ( std::vector<reco::TransientTrack>::const_iterator iTk=(iPair->first).begin(); iTk!=(iPair->first).end(); ++iTk) {
	 LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer Transient Tracks in the pair  charge " << iTk->charge() << " Num of RecHits " << iTk->recHitsSize() << " inner momentum " << iTk->track().innerMomentum() << "\n";  
	  

	  const reco::TrackTransientTrack* ttt = dynamic_cast<const reco::TrackTransientTrack*>(iTk->basicTransientTrack());
	  reco::TrackRef myTkRef= ttt->persistentTrackRef(); 



	 LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer Ref to Rec Tracks in the pair  charge " << myTkRef->charge() << " Num of RecHits " << myTkRef->recHitsSize() << " inner momentum " << myTkRef->innerMomentum() << "\n";  
	  
	  
	  trackPairRef.push_back(myTkRef);
	  
	}
	  
	  
	LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer trackPairRef  " << trackPairRef.size() <<  "\n";
	LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer SC energy " <<  aClus->energy() << "\n";
	LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer photon p4 " << p4  << "\n";
	LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer vtx " << vtx.x() << " " << vtx.y() << " " << vtx.z() << "\n";
	LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer convVtx " << convVtx.x() << " " << convVtx.y() << " " << convVtx.z() << "\n";
	LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer trackPairRef  " << trackPairRef.size() <<  "\n";
	
	reco::ConvertedPhoton  newCandidate(0, p4, vtx, convVtx);
	newCandidate.setP4(p4);
	outputConvPhotonCollection.push_back(newCandidate);
	// set the reference to the SC
	reco::SuperClusterRef scRef(reco::SuperClusterRef(scEndcapHandle, lSC));
	outputConvPhotonCollection[iSC].setSuperCluster(scRef);
	// set the reference to the tracks
	outputConvPhotonCollection[iSC].setTrackPairRef( trackPairRef);
	
	
	
	iSC++;	
	myCands++;
	LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer Put the ConvertedPhotonCollection a candidate in the Endcap " << "\n";
	  
      }
      
    } else {
      
      LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer GOLDEN PHOTON ?? Zero Tracks " <<  "\n";  
      LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer SC energy " <<  aClus->energy() << "\n";
      LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer photon p4 " << p4  << "\n";
      LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer vtx " << vtx.x() << " " << vtx.y() << " " << vtx.z() << "\n";
      LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer convVtx " << convVtx.x() << " " << convVtx.y() << " " << convVtx.z() << "\n";
      LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer trackPairRef  " << trackPairRef.size() <<  "\n";
      reco::ConvertedPhoton  newCandidate(0, p4, vtx, convVtx);
      newCandidate.setP4(p4);
      outputConvPhotonCollection.push_back(newCandidate);
      reco::SuperClusterRef scRef(reco::SuperClusterRef(scEndcapHandle, lSC));
      outputConvPhotonCollection[iSC].setSuperCluster(scRef);
      
      iSC++;	
      myCands++;
     LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer Put the ConvertedPhotonCollection a candidate in the Endcap " << "\n";
      
    }
          
    
    lSC++;
    
  }
  



  // put the product in the event
  
  outputConvPhotonCollection_p->assign(outputConvPhotonCollection.begin(),outputConvPhotonCollection.end());
 LogDebug("ConvertedPhotonProducer") << " ConvertedPhotonProducer Putting in the event  " << myCands << "  converted photon candidates " << (*outputConvPhotonCollection_p).size() << "\n";  
  theEvent.put( outputConvPhotonCollection_p, ConvertedPhotonCollection_);
  




}
