#include "RecoTracker/TrackProducer/interface/TrackProducerAlgorithm.h"

#include "FWCore/Framework/interface/OrphanHandle.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"

#include "DataFormats/TrackCandidate/interface/TrackCandidate.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHitFwd.h"

#include "TrackingTools/PatternTools/interface/TrajectoryFitter.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"
#include "RecoTracker/TransientTrackingRecHit/interface/TkTransientTrackingRecHitBuilder.h"
#include "TrackingTools/PatternTools/interface/TransverseImpactPointExtrapolator.h"

void TrackProducerAlgorithm::run(const TrackingGeometry * theG,
				 const MagneticField * theMF,
				 TrackCandidateCollection& theTCCollection,
				 const TrajectoryFitter * theFitter,
				 const Propagator * thePropagator,
				 AlgoProductCollection& algoResults)
{
  int cont = 0;
  for (TrackCandidateCollection::iterator i=theTCCollection.begin(); i!=theTCCollection.end();i++)
    {
      TrackCandidate * theTC = &(*i);//maybe it is better to clone
      //convert PTrajectoryStateOnDet to TrajectoryStateOnSurface
      TrajectoryStateTransform transformer;
      PTrajectoryStateOnDet state = theTC->trajectoryStateOnDet();
      DetId * detId = new DetId(state.detId());
      TrajectoryStateOnSurface theTSOS = transformer.transientState( state,
								     &(theG->idToDet(*detId)->surface()), 
								     theMF);
      
      //convert the TrackingRecHit vector to a TransientTrackingRecHit vector
      //meanwhile computes the number of degrees of freedom
      edm::OwnVector<TransientTrackingRecHit> hits;
      TransientTrackingRecHitBuilder * builder;

      //
      // temporary!
      //
      builder = new TkTransientTrackingRecHitBuilder( theG);

      int ndof=0;
      
      for (edm::OwnVector<TrackingRecHit>::const_iterator i=theTC->recHits().first;
	   i!=theTC->recHits().second; i++){
	hits.push_back(builder->build(&(*i) ));
	ndof = ndof + int( (i->dimension())*(i->weight()) );
      }
      
      delete builder;

      ndof = ndof - 5;
      
      //variable declarations
      std::vector<Trajectory> trajVec;
      reco::Track * theTrack;
      Trajectory * theTraj; 
      
      //perform the fit: the result's size is 1 if it succeded, 0 if fails
      trajVec = theFitter->fit(theTC->seed(), hits, theTSOS);
      
      TransverseImpactPointExtrapolator * tipe;
      TrajectoryStateOnSurface tsos;
      TrajectoryStateOnSurface innertsos;

      
      if (trajVec.size() != 0){
	
	tipe = new TransverseImpactPointExtrapolator(*thePropagator);
	
	theTraj = &( trajVec.front() );
	
	if (theTraj->direction() == alongMomentum) {
	  innertsos = theTraj->firstMeasurement().updatedState();
	} else { 
	  innertsos = theTraj->lastMeasurement().updatedState();
	}

	
	//extrapolate the innermost state to the point of closest approach to the beamline
	tsos = tipe->extrapolate(*(innertsos.freeState()), 
				 GlobalPoint(0,0,0) );
	
	//compute parameters needed to build a Track from a Trajectory    
	int charge = tsos.charge();
	const GlobalTrajectoryParameters& gp = tsos.globalParameters();
	GlobalPoint v = gp.position();
	GlobalVector p = gp.momentum();
	const CartesianTrajectoryError& cte = tsos.cartesianError();
	AlgebraicSymMatrix m = cte.matrix();
	math::Error<6> cov;
	for( int i = 0; i < 6; ++i )
	  for( int j = 0; j <= i; ++j )
	    cov( i, j ) = m.fast( i + 1 , j + 1 );
	math::XYZVector mom( p.x(), p.y(), p.z() );
	math::XYZPoint  vtx( v.x(), v.y(), v.z() );   
	
	//build the Track(chiSquared, ndof, found, invalid, lost, q, vertex, momentum, covariance)
	theTrack = new reco::Track(theTraj->chiSquared(), 
				   ndof,//FIXME fix weight() in TrackingRecHit
				   theTraj->foundHits(),//FIXME to be fixed in Trajectory.h
				   0, //FIXME no corresponding method in trajectory.h
				   theTraj->lostHits(),//FIXME to be fixed in Trajectory.h
				   charge, 
				   vtx,
				   mom,
				   cov);
	AlgoProduct aProduct(theTraj,theTrack);
	algoResults.push_back(aProduct);


	cont++;
      }
    }

  std::cout << "Number of Tracks found: " << cont << std::endl;

}

