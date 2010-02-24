/** \class MuonMillepedeTrackRefitter
 *  
 *
 *  $Date: 2009/12/15 15:07:16 $
 *  $Revision: 1.1 $
 *  \author P. Martinez Ruiz del Arbol, IFCA (CSIC-UC)  <Pablo.Martinez@cern.ch>
 */

#include "Alignment/MuonAlignmentAlgorithms/plugins/MuonMillepedeTrackRefitter.h"

// Collaborating Class Header
#include "Alignment/MuonAlignmentAlgorithms/interface/SegmentToTrackAssociator.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"

#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "TrackingTools/GeomPropagators/interface/Propagator.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"

#include "DataFormats/TrajectorySeed/interface/PropagationDirection.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/GeometrySurface/interface/Cylinder.h"
#include "DataFormats/GeometrySurface/interface/Plane.h"
#include "DataFormats/GeometrySurface/interface/Cone.h"

#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"

#include "RecoMuon/TransientTrackingRecHit/interface/MuonTransientTrackingRecHitBuilder.h"
#include "RecoMuon/TransientTrackingRecHit/interface/MuonTransientTrackingRecHit.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"


#include <vector>
#include "TH2D.h"

using namespace std;
using namespace edm;




/// Constructor
MuonMillepedeTrackRefitter::MuonMillepedeTrackRefitter( const ParameterSet& pset )
{
  
  SACollectionTag = pset.getParameter<edm::InputTag>( "SATrackCollectionTag" );
  
  //Products
  produces<vector<Trajectory> >();
  produces<TrajTrackAssociationCollection>();

}

// Destructor
MuonMillepedeTrackRefitter::~MuonMillepedeTrackRefitter()
{
}


void MuonMillepedeTrackRefitter::produce( Event & event, const EventSetup& eventSetup )
{
 
  //Get collections from the event

  edm::Handle<reco::TrackCollection> tracksSA;
  event.getByLabel( SACollectionTag, tracksSA );

  ESHandle<MagneticField> theMGField;
  eventSetup.get<IdealMagneticFieldRecord>().get( theMGField );

  ESHandle<GlobalTrackingGeometry> theTrackingGeometry;
  eventSetup.get<GlobalTrackingGeometryRecord>().get( theTrackingGeometry );

  RefProd<vector<Trajectory> > trajectoryCollectionRefProd 
    = event.getRefBeforePut<vector<Trajectory> >();

  //Allocate collection of tracks
  auto_ptr<vector<Trajectory> > trajectoryCollection( new vector<Trajectory> );
  // Association map between Trajectory and Track
  auto_ptr<TrajTrackAssociationCollection> trajTrackMap( new TrajTrackAssociationCollection );
 

  //Create the propagator
 
  std::map<edm::Ref<std::vector<Trajectory> >::key_type, edm::Ref<reco::TrackCollection>::key_type> trajToTrack_map;
  
  Ref<vector<Trajectory> >::key_type trajectoryIndex = 0;
     
  reco::TrackRef::key_type trackIndex = 0;

  for (reco::TrackCollection::const_iterator trackSA = tracksSA->begin();  trackSA != tracksSA->end();  ++trackSA ) {
   
    reco::TransientTrack tTrackSA( *trackSA, &*theMGField, theTrackingGeometry );

    //Create an empty trajectory
    Trajectory myTraj;

    TrajectoryStateOnSurface innerTSOS = tTrackSA.innermostMeasurementState();
    

    for(trackingRecHit_iterator theHit = tTrackSA.recHitsBegin(); theHit != tTrackSA.recHitsEnd(); ++theHit) {
  
      TrackingRecHit *myClone = (*theHit)->clone(); 
      const GeomDet* myDet = theTrackingGeometry->idToDet( (*theHit)->geographicalId() );
      TrajectoryMeasurement myMeas(innerTSOS, &* MuonTransientTrackingRecHit::specificBuild(myDet, (TrackingRecHit *) &*myClone));
      myTraj.push(myMeas);

    }
 
    trajectoryCollection->push_back(myTraj);
    trajToTrack_map[trajectoryIndex] = trackIndex;          
    ++trajectoryIndex;
    ++trackIndex;
  } 

  edm::OrphanHandle<std::vector<Trajectory> > trajsRef = event.put(trajectoryCollection);
  
  for( trajectoryIndex = 0; trajectoryIndex < tracksSA->size(); ++trajectoryIndex) 
  {      
    edm::Ref<reco::TrackCollection>::key_type trackCounter = trajToTrack_map[trajectoryIndex];
    trajTrackMap->insert(edm::Ref<std::vector<Trajectory> >(trajsRef, trajectoryIndex), edm::Ref<reco::TrackCollection>(tracksSA, trackCounter));
  }
  
  event.put(trajTrackMap);
 
}



DEFINE_FWK_MODULE(MuonMillepedeTrackRefitter);



