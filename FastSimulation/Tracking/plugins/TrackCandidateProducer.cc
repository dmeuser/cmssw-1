#include <memory>

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/OwnVector.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiTrackerGSRecHit2DCollection.h" 
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackExtraFwd.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"

#include "TrackingTools/TransientTrackingRecHit/interface/GenericTransientTrackingRecHit.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"

#include "FastSimulation/Tracking/interface/TrackerRecHit.h"

#include "FastSimulation/Tracking/plugins/TrackCandidateProducer.h"

#include <vector>
//

//for debug only 
//#define FAMOS_DEBUG

TrackCandidateProducer::TrackCandidateProducer(const edm::ParameterSet& conf) 
{  
#ifdef FAMOS_DEBUG
  std::cout << "TrackCandidateProducer created" << std::endl;
#endif

  // The main product is a track candidate collection.
  produces<TrackCandidateCollection>();

  // These products contain tracks already reconstructed at this level
  // (No need to reconstruct them twice!)
  produces<reco::TrackCollection>();
  produces<TrackingRecHitCollection>();
  produces<reco::TrackExtraCollection>();
  produces<std::vector<Trajectory> >();
  produces<TrajTrackAssociationCollection>();
  
  // The name of the seed producer
  seedProducer = conf.getParameter<edm::InputTag>("SeedProducer");

  // The name of the recHit producer
  hitProducer = conf.getParameter<edm::InputTag>("HitProducer");

  // The name of the track producer (tracks already produced need not be produced again!)
  trackProducer = conf.getParameter<edm::InputTag>("TrackProducer");

  // The minimum number of crossed layers
  minNumberOfCrossedLayers = conf.getParameter<unsigned int>("MinNumberOfCrossedLayers");

  // Reject overlapping hits?
  rejectOverlaps = conf.getParameter<bool>("overlapCleaning");

}

  
// Virtual destructor needed.
TrackCandidateProducer::~TrackCandidateProducer() {

  // do nothing
#ifdef FAMOS_DEBUG
  std::cout << "TrackCandidateProducer destructed" << std::endl;
#endif

} 
 
void 
TrackCandidateProducer::beginJob (edm::EventSetup const & es) {

  //services
  //  es.get<TrackerRecoGeometryRecord>().get(theGeomSearchTracker);

  edm::ESHandle<TrackerGeometry>        geometry;


  es.get<TrackerDigiGeometryRecord>().get(geometry);

  theGeometry = &(*geometry);

}
  
  // Functions that gets called by framework every event
void 
TrackCandidateProducer::produce(edm::Event& e, const edm::EventSetup& es) {        

#ifdef FAMOS_DEBUG
  std::cout << "################################################################" << std::endl;
  std::cout << " TrackCandidateProducer produce init " << std::endl;
#endif

  std::auto_ptr<TrackCandidateCollection> output(new TrackCandidateCollection);    
  std::auto_ptr<reco::TrackCollection> recoTracks(new reco::TrackCollection);    
  std::auto_ptr<TrackingRecHitCollection> recoHits(new TrackingRecHitCollection);
  std::auto_ptr<reco::TrackExtraCollection> recoTrackExtras(new reco::TrackExtraCollection);
  std::auto_ptr<std::vector<Trajectory> > recoTrajectories(new std::vector<Trajectory> );
  std::auto_ptr<TrajTrackAssociationCollection> recoTrajTrackMap( new TrajTrackAssociationCollection() );
  
  // Get the seeds
  edm::Handle<TrajectorySeedCollection> theSeeds;
  e.getByLabel(seedProducer,theSeeds);

  // No seed -> output an empty track collection
  if(theSeeds->size() == 0) {
    e.put(output);
    e.put(recoTracks);
    e.put(recoHits);
    e.put(recoTrackExtras);
    e.put(recoTrajectories);
    e.put(recoTrajTrackMap);
    return;
  }

  // Get the GS RecHits
  edm::Handle<SiTrackerGSRecHit2DCollection> theGSRecHits;
  e.getByLabel(hitProducer, theGSRecHits);

  // The input track collection + extra's
  edm::Handle<reco::TrackCollection> theTrackCollection;
  edm:: Handle<std::vector<Trajectory> > theTrajectoryCollection;
  edm::Handle<TrajTrackAssociationCollection> theAssoMap;  
  bool isTrackCollection = e.getByLabel(trackProducer,theTrackCollection); 
  if ( isTrackCollection ) { 
    e.getByLabel(trackProducer,theTrajectoryCollection);
    e.getByLabel(trackProducer,theAssoMap);
  }

  // The track collection iterators.
  TrajTrackAssociationCollection::const_iterator anAssociation;  
  TrajTrackAssociationCollection::const_iterator lastAssociation;
  if ( isTrackCollection ) { 
    anAssociation = theAssoMap->begin();
    lastAssociation = theAssoMap->end();
  }

  // Loop over the seeds
  TrajectorySeedCollection::const_iterator aSeed = theSeeds->begin();
  TrajectorySeedCollection::const_iterator lastSeed = theSeeds->end();
  for ( ; aSeed!=lastSeed; ++aSeed ) { 

    // Find the first hit of the Seed
    TrajectorySeed::range theSeedingRecHitRange = aSeed->recHits();
    const SiTrackerGSRecHit2D * theFirstSeedingRecHit = 
      dynamic_cast<const SiTrackerGSRecHit2D *> (&(*(theSeedingRecHitRange.first)));
    TrackerRecHit theFirstSeedingTrackerRecHit(theFirstSeedingRecHit,theGeometry);
    // SiTrackerGSRecHit2DCollection::const_iterator theSeedingRecHitEnd = theSeedingRecHitRange.second;

    // The SimTrack id associated to that recHit
    int simTrackId = theFirstSeedingRecHit->simtrackId();
    // std::cout << "The Sim Track Id : " << simTrackId << std::endl;
    // const SimTrack& theSimTrack = (*theSimTracks)[simTrackId]; 

    // A vector of TrackerRecHits belonging to the track and the number of crossed layers
    std::vector<TrackerRecHit> theTrackerRecHits;
    unsigned theNumberOfCrossedLayers = 0;
 
    // Check if the track has already been reconstructed
    int recoTrackId = -1;
    reco::TrackRef aTrackRef;
    edm::Ref<std::vector<Trajectory> > aTrajectoryRef;
    trackingRecHit_iterator aHit; 
    if ( isTrackCollection ) { 
      for ( ; anAssociation != lastAssociation && recoTrackId < simTrackId; ++anAssociation ) { 
	aTrajectoryRef = anAssociation->key;
	aTrackRef = anAssociation->val;
	aHit = aTrackRef->recHitsBegin();
	const SiTrackerGSRecHit2D * rechit = dynamic_cast<const SiTrackerGSRecHit2D *> (aHit->get());
	recoTrackId = rechit->simtrackId();
	// std::cout << "The Reco Track Id : " << recoTrackId << std::endl;
      }
    }

    // The track has indeed been reconstructed already -> Save the pertaining info
    if ( isTrackCollection && recoTrackId == simTrackId ) { 
      
      // A copy of the track
      reco::Track aRecoTrack(*aTrackRef);
      recoTracks->push_back(aRecoTrack);      
      
      // A copy of the hits
      trackingRecHit_iterator lastHit = aRecoTrack.recHitsEnd();
      for ( ; aHit != lastHit; ++aHit) {
	TrackingRecHit *hit = (*aHit)->clone();
	recoHits->push_back(hit);
      }

      // A copy of the trajectories
      recoTrajectories->push_back(*aTrajectoryRef);
      
    // The track was not saved -> create a track candidate.
    } else { 
      
      // Get all the rechits associated to this track
      SiTrackerGSRecHit2DCollection::range theRecHitRange = theGSRecHits->get(simTrackId);
      SiTrackerGSRecHit2DCollection::const_iterator theRecHitRangeIteratorBegin = theRecHitRange.first;
      SiTrackerGSRecHit2DCollection::const_iterator theRecHitRangeIteratorEnd   = theRecHitRange.second;
      SiTrackerGSRecHit2DCollection::const_iterator iterRecHit;
      
      bool firstRecHit = true;
      // 
      TrackerRecHit theCurrentRecHit, thePreviousRecHit;
      for ( iterRecHit = theRecHitRangeIteratorBegin; 
	    iterRecHit != theRecHitRangeIteratorEnd; 
	    ++iterRecHit) {
	
	// Get current and previous rechits
	thePreviousRecHit = theCurrentRecHit;
	theCurrentRecHit = TrackerRecHit(&(*iterRecHit),theGeometry);
	
	// Check that the first rechit is indeed the first seeding hit
	if ( firstRecHit && theCurrentRecHit != theFirstSeedingTrackerRecHit ) continue;
	
	// Count the number of crossed layers
	if ( !theCurrentRecHit.isOnTheSameLayer(thePreviousRecHit) ) 
	  ++theNumberOfCrossedLayers;
	
	// Add all rechits (Grouped Trajectory Builder) from this hit onwards
	// Always add the first seeding rechit anyway
	if ( !rejectOverlaps || firstRecHit ) {  
	  
	  theTrackerRecHits.push_back(theCurrentRecHit);
	  firstRecHit = false;
	  
	  // And now treat the following RecHits if hits in the same layer 
	  // have to be rejected
	} else { 
	  
	  // Not the same layer : Add the current hit
	  if ( theCurrentRecHit.subDetId()    != theTrackerRecHits.back().subDetId() || 
	       theCurrentRecHit.layerNumber() != theTrackerRecHits.back().layerNumber() ) {
	    theTrackerRecHits.push_back(theCurrentRecHit);
	    // Same layer : keep the current hit if better, and drop the other - otherwise do nothing  
	  } else if ( theCurrentRecHit.localError() < theTrackerRecHits.back().localError() ) { 
	    theTrackerRecHits.back() = theCurrentRecHit;
#ifdef FAMOS_DEBUG
	    std::cout << "Hit number " << theTrackerRecHits.size() 
		      << " : The local error is smaller than the previous hit " 
		      << theCurrentRecHit.localError() << " " 
		      <<  theTrackerRecHits.back().localError() << " in subdet/layer/ring " 
		      << theCurrentRecHit.subDetId() << " " 
		      << theCurrentRecHit.layerNumber() << " " 
		      << theCurrentRecHit.ringNumber() << " -> REPLACE " << std::endl;
#endif
	  } else { 
#ifdef FAMOS_DEBUG
	    std::cout << "Hit number " << theTrackerRecHits.size() 
		      << " : The local error is larger than the previous hit " 
		      << theCurrentRecHit.localError() << " " 
		      <<  theTrackerRecHits.back().localError() << " in subdet/layer/ring " 
		      << theCurrentRecHit.subDetId() << " " 
		      << theCurrentRecHit.layerNumber() << " " 
		      << theCurrentRecHit.ringNumber() << " -> IGNORE " << std::endl;
#endif
	  }
	}
      }
    // End of loop over the track rechits
    }

    // 1) Create the OwnWector of TrackingRecHits
    edm::OwnVector<TrackingRecHit> recHits;
    for ( unsigned ih=0; ih<theTrackerRecHits.size(); ++ih ) {
      TrackingRecHit* aTrackingRecHit = 
	  GenericTransientTrackingRecHit::build(theTrackerRecHits[ih].geomDet(),
						theTrackerRecHits[ih].hit())->hit()->clone();
      recHits.push_back(aTrackingRecHit);
#ifdef FAMOS_DEBUG
      const DetId& detId = theTrackerRecHits[ih].hit()->geographicalId();      
      std::cout << "Added RecHit from detid " << detId.rawId() 
		<< " subdet = " << theTrackerRecHits[ih].subDetId() 
		<< " layer = " << theTrackerRecHits[ih].layerNumber()
		<< " ring = " << theTrackerRecHits[ih].ringNumber()
		<< " error = " << theTrackerRecHits[ih].localError()
		<< std::endl;
      
      std::cout << "Track/z/r : "
		<< simTrackId << " " 
		<< theTrackerRecHits[ih].globalPosition().z() << " " 
		<< theTrackerRecHits[ih].globalPosition().perp() << std::endl;
#endif
    }

    // Check the number of crossed layers
    if ( theNumberOfCrossedLayers < minNumberOfCrossedLayers ) continue;

    // Create a track Candidate .
    TrackCandidate newTrackCandidate(recHits, *aSeed, aSeed->startingState());
    // std::cout << "Track kept for later fit!" << std::endl;
    
#ifdef FAMOS_DEBUG
    // Log
    std::cout << "\tSeed Information " << std::endl;
    std::cout << "\tSeed Direction = " << aSeed->direction() << std::endl;
    std::cout << "\tSeed StartingDet = " << aSeed->startingState().detId() << std::endl;
    
    std::cout << "\tTrajectory Parameters " 
	      << std::endl;
    std::cout << "\t\t detId  = " 
	      << newTrackCandidate.trajectoryStateOnDet().detId() 
	      << std::endl;
    std::cout << "\t\t loc.px = " 
	      << newTrackCandidate.trajectoryStateOnDet().parameters().momentum().x()    
	      << std::endl;
    std::cout << "\t\t loc.py = " 
	      << newTrackCandidate.trajectoryStateOnDet().parameters().momentum().y()    
	      << std::endl;
    std::cout << "\t\t loc.pz = " 
	      << newTrackCandidate.trajectoryStateOnDet().parameters().momentum().z()    
	      << std::endl;
    std::cout << "\t\t error  = ";
    for(std::vector< float >::const_iterator iElement = newTrackCandidate.trajectoryStateOnDet().errorMatrix().begin();
	iElement < newTrackCandidate.trajectoryStateOnDet().errorMatrix().end();
	++iElement) {
      std::cout << "\t" << *iElement;
    }
    std::cout << std::endl;
#endif

    output->push_back(newTrackCandidate);

  }
  
  // Save the track candidates in the event
#ifdef FAMOS_DEBUG
  std::cout << "Saving " 
	    << output->size() << " track candidates and " 
	    << recoTracks->size() << " reco::Tracks " << std::endl;
#endif
  // Save the track candidates
  e.put(output);

  // Save the tracking recHits
  edm::OrphanHandle <TrackingRecHitCollection> theRecoHits = e.put(recoHits );

  // Create the track extras and add the references to the rechits
  unsigned hits=0;
  for ( unsigned index = 0; index < recoTracks->size(); ++index ) { 
    reco::TrackExtra aTrackExtra;
    unsigned nHits = recoTracks->at(index).numberOfValidHits();
    for ( unsigned int ih=0; ih<nHits; ++ih) {
      aTrackExtra.add(TrackingRecHitRef(theRecoHits,hits++));
    }
    recoTrackExtras->push_back(aTrackExtra);
  }
  
  // Save the track extras
  edm::OrphanHandle<reco::TrackExtraCollection> theRecoTrackExtras = e.put(recoTrackExtras);

  // Add the reference to the track extra in the tracks
  unsigned nTracks = recoTracks->size();
  for ( unsigned index = 0; index<nTracks; ++index ) { 
    const reco::TrackExtraRef theTrackExtraRef(theRecoTrackExtras,index);
    (recoTracks->at(index)).setExtra(theTrackExtraRef);
  }

  // Save the tracks
  edm::OrphanHandle<reco::TrackCollection> theRecoTracks = e.put(recoTracks);

  // Save the trajectories
  edm::OrphanHandle<std::vector<Trajectory> > theRecoTrajectories = e.put(recoTrajectories);
  
  // Create and set the trajectory/track association map 
  for ( unsigned index = 0; index < theRecoTracks->size(); ++index ) { 
    edm::Ref<std::vector<Trajectory> > trajRef( theRecoTrajectories, index );
    edm::Ref<reco::TrackCollection>    tkRef( theRecoTracks, index );
    recoTrajTrackMap->insert(trajRef,tkRef);
  }

  // Save the association map.
  e.put(recoTrajTrackMap);

}


