//
// Package:         RecoTracker/SingleTrackPattern
// Class:           CosmicTrajectoryBuilder
// Original Author:  Michele Pioppi-INFN perugia
#include <vector>
#include <iostream>
#include <cmath>

#include "RecoTracker/SingleTrackPattern/interface/CosmicTrajectoryBuilder.h"
#include "DataFormats/Common/interface/OwnVector.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"
#include "TrackingTools/TrajectoryState/interface/BasicSingleTrajectoryState.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Geometry/Vector/interface/GlobalPoint.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"


CosmicTrajectoryBuilder::CosmicTrajectoryBuilder(const edm::ParameterSet& conf) : conf_(conf) { 
  //minimum number of hits per tracks
  theMinHits=conf_.getParameter<int>("MinHits");
  //cut on chi2
  chi2cut=conf_.getParameter<double>("Chi2Cut");
  edm::LogInfo("CosmicTrackFinder")<<"Minimum number of hits "<<theMinHits<<" Cut on Chi2= "<<chi2cut;
}


CosmicTrajectoryBuilder::~CosmicTrajectoryBuilder() {
}


void CosmicTrajectoryBuilder::init(const edm::EventSetup& es, bool seedplus){


  //services
  es.get<IdealMagneticFieldRecord>().get(magfield);
   es.get<TrackerDigiGeometryRecord>().get(tracker);

 

  //trackingtools
  if (seedplus) {
    seed_plus=true;
    thePropagator=         new PropagatorWithMaterial(alongMomentum,0.1057,&(*magfield) );
    thePropagatorOp=    new PropagatorWithMaterial(oppositeToMomentum,0.1057,&(*magfield) );}
  else {
    seed_plus=false;

    thePropagator=         new PropagatorWithMaterial(oppositeToMomentum,0.1057,&(*magfield) );
    thePropagatorOp=    new PropagatorWithMaterial(alongMomentum,0.1057,&(*magfield) );
  }
  
  theUpdator=       new KFUpdator();
  theEstimator=     new Chi2MeasurementEstimator(chi2cut);
  RHBuilder=        new TkTransientTrackingRecHitBuilder(&(*tracker));

  theFitter=        new KFTrajectoryFitter(*thePropagator,
					   *theUpdator,	
					   *theEstimator) ;


  theSmoother=      new KFTrajectorySmoother(*thePropagatorOp,
					       *theUpdator,	
					       *theEstimator);
 
}

void CosmicTrajectoryBuilder::run(const TrajectorySeedCollection &collseed,
				  const SiStripRecHit2DLocalPosCollection &collstereo,
				  const SiStripRecHit2DLocalPosCollection &collrphi ,
				  const SiStripRecHit2DMatchedLocalPosCollection &collmatched,
				  const SiPixelRecHitCollection &collpixel,
				  const edm::EventSetup& es,
				  edm::Event& e,
				  vector<AlgoProduct> &algooutput)
{

  hits.clear();
  trajFit.clear();

  //order all the hits
  vector<const TrackingRecHit*> allHits= SortHits(collstereo,collrphi,collmatched,collpixel,collseed);
  
  std::vector<Trajectory> trajSmooth;
  std::vector<Trajectory>::iterator trajIter;
  
  
  TrajectorySeedCollection::const_iterator iseed;
  for(iseed=collseed.begin();iseed!=collseed.end();iseed++){
    Trajectory startingTraj = createStartingTrajectory(*iseed);
    AddHit(startingTraj,allHits);

    for (trajIter=trajFit.begin(); trajIter!=trajFit.end();trajIter++){
      trajSmooth=theSmoother->trajectories((*trajIter));
    }

    for (trajIter= trajSmooth.begin(); trajIter!=trajSmooth.end();trajIter++){
      AlgoProduct tk=makeTrack((*trajIter));
      algooutput.push_back(tk);
   
    }
  }
};

Trajectory CosmicTrajectoryBuilder::createStartingTrajectory( const TrajectorySeed& seed) const
{
 
  Trajectory result( seed, seed.direction());

  std::vector<TM> seedMeas = seedMeasurements(seed);
  if ( !seedMeas.empty()) {
    for (std::vector<TM>::const_iterator i=seedMeas.begin(); i!=seedMeas.end(); i++){
      result.push(*i);
    }
  }
 
  return result;
}


std::vector<TrajectoryMeasurement> 
CosmicTrajectoryBuilder::seedMeasurements(const TrajectorySeed& seed) const
{
  std::vector<TrajectoryMeasurement> result;

  TrajectorySeed::range hitRange = seed.recHits();
  for (TrajectorySeed::const_iterator ihit = hitRange.first; 
       ihit != hitRange.second; ihit++) {
 
    TransientTrackingRecHit* recHit = RHBuilder->build(&(*ihit));
    
    const GeomDet* hitGeomDet = (&(*tracker))->idToDet( ihit->geographicalId());
    TSOS invalidState( new BasicSingleTrajectoryState( hitGeomDet->surface()));

    if (ihit == hitRange.second - 1) {
      TSOS  updatedState=startingTSOS(seed);
      result.push_back(TM( invalidState, updatedState, recHit));
      
    } 
    else {
      result.push_back(TM( invalidState, recHit));
    }
    
  }

  return result;
};





vector<const TrackingRecHit*> 
CosmicTrajectoryBuilder::SortHits(const SiStripRecHit2DLocalPosCollection &collstereo,
				  const SiStripRecHit2DLocalPosCollection &collrphi ,
				  const SiStripRecHit2DMatchedLocalPosCollection &collmatched,
				  const SiPixelRecHitCollection &collpixel,
				  const TrajectorySeedCollection &collseed){

  //The Hits with global y more than the seed are discarded
  //The Hits correspondign to the seed are discarded
  //At the end all the hits are sorted in y
  vector<const TrackingRecHit*> allHits;
  vector<const TrackingRecHit*> seedHits;
  SiPixelRecHitCollection::const_iterator ipix;
  for(ipix=collpixel.begin();ipix!=collpixel.end();ipix++){
    allHits.push_back(&(*ipix));
  }

  SiStripRecHit2DLocalPosCollection::const_iterator istrip;
  TrajectorySeedCollection::const_iterator iseed;
  TrajectorySeedCollection::const_iterator seedbegin=collseed.begin();
  TrajectorySeed::range hRange= (*seedbegin).recHits();
  TrajectorySeed::const_iterator ihit;
  float ymin=0.;
  for (ihit = hRange.first; 
       ihit != hRange.second; ihit++) {
    ymin=RHBuilder->build(&(*ihit))->globalPosition().y();
  }
 
  
  TrajectorySeedCollection::const_iterator seedend=collseed.end();
  for(istrip=collrphi.begin();istrip!=collrphi.end();istrip++){
    bool differenthit= true;
    for (iseed=seedbegin;iseed!=seedend;iseed++){
      TrajectorySeed::range hitRange= (*iseed).recHits();
      for (ihit = hitRange.first; 
	   ihit != hitRange.second; ihit++) {
	if((*ihit).geographicalId()==(*istrip).geographicalId()) {

	  if(((*ihit).localPosition()-(*istrip).localPosition()).mag()<0.1)  differenthit=false;
	}
      }
    }


    if (differenthit) { 
      float ych= RHBuilder->build(&(*istrip))->globalPosition().y();
      if (ych<ymin)
	allHits.push_back(&(*istrip));   
    } 
    else  seedHits.push_back(&(*istrip)); 
  }

  hits.push_back((RHBuilder->build(seedHits.back()))); 
  hits.push_back((RHBuilder->build(seedHits.front()))); 

  LogDebug("CosmicTrackFinder")<<"SEED HITS"<<RHBuilder->build(seedHits.back())->globalPosition()
      <<" "<<RHBuilder->build(seedHits.front())->globalPosition();

  for(istrip=collstereo.begin();istrip!=collstereo.end();istrip++){

      allHits.push_back(&(*istrip));
  }

  SiStripRecHit2DMatchedLocalPosCollection::const_iterator istripm;
  for(istripm=collmatched.begin();istripm!=collmatched.end();istripm++){
    
  }

  if (seed_plus){
    stable_sort(allHits.begin(),allHits.end(),CompareHitY_plus(*tracker));
  }
  else {
    stable_sort(allHits.begin(),allHits.end(),CompareHitY(*tracker));
  }
  return allHits;
};

TrajectoryStateOnSurface
CosmicTrajectoryBuilder::startingTSOS(const TrajectorySeed& seed)const
{
  PTrajectoryStateOnDet pState( seed.startingState());
  const GeomDet* gdet  = (&(*tracker))->idToDet(DetId(pState.detId()));
  TSOS  State= tsTransform.transientState( pState, &(gdet->surface()), 
					   &(*magfield));
  return State;

}

void CosmicTrajectoryBuilder::AddHit(Trajectory &traj,
				     vector<const TrackingRecHit*>Hits){

  for (unsigned int icosmhit=0;icosmhit<Hits.size();icosmhit++){
    GlobalPoint gphit=RHBuilder->build(Hits[icosmhit])->globalPosition();
    unsigned int iraw= Hits[icosmhit]->geographicalId().rawId();
    LogDebug("CosmicTrackFinder")<<" HIT POSITION "<< gphit;

    TransientTrackingRecHit* tmphit=RHBuilder->build(Hits[icosmhit]);

    TSOS prSt= thePropagator->propagate(traj.lastMeasurement().updatedState(),
					tracker->idToDet(Hits[icosmhit]->geographicalId())->surface());
   
    if (prSt.isValid()){
      LogDebug("CosmicTrackFinder") <<"STATE PROPAGATED AT DET "<<iraw<<prSt;
      TSOS UpdatedState= theUpdator->update( prSt, *tmphit);
      
      
      if (UpdatedState.isValid()){
	LogDebug("CosmicTrackFinder") <<"STATE UPDATED WITH HIT AT POSITION "<<gphit<<UpdatedState;
	float contr= theEstimator->estimate(UpdatedState, *tmphit).second;

	if (contr<chi2cut)
	  {	 
	    traj.push(TM(prSt,UpdatedState,RHBuilder->build(Hits[icosmhit])
			 , contr));
	    hits.push_back(&(*tmphit));
	    LogDebug("CosmicTrackFinder") <<"HIT SELECTED  position" <<gphit
					  <<" traj Chi2= "<<traj.chiSquared();
	     
	  }
      }edm::LogError("CosmicTrackFinder")<<" State can not be updated with hit at position "<<gphit;
    }edm::LogError("CosmicTrackFinder")<<" State can not be propagated at det "<< iraw;

    
  }
  
  
  

  if ( qualityFilter( traj)){
     const TrajectorySeed& tmpseed=traj.seed();
     TSOS startingState=startingTSOS(tmpseed);     
     trajFit = theFitter->fit(tmpseed,hits, startingState );
   }

 
}


bool 
CosmicTrajectoryBuilder::qualityFilter(Trajectory traj){
  if ( traj.foundHits() >= theMinHits) {
    return true;
  }
  else {
    return false;
  }
}

std::pair<Trajectory, reco::Track>  CosmicTrajectoryBuilder::makeTrack(const Trajectory &traj){
  //MP must be checked
  TSOS innertsos = traj.lastMeasurement().updatedState();
  TSOS Fitsos = traj.firstMeasurement().updatedState();

  int charge = innertsos.charge();
  //MP
  int ndof =traj.foundHits()-5;
  if (ndof<0) ndof=0;

 
  GlobalPoint v(0.,0.,0.);
  GlobalVector p;
  //The momentum is always the one corresponding to the
  //upper hit
  if (seed_plus) p=innertsos.globalParameters().momentum();
  else p= Fitsos.globalParameters().momentum();

  const CartesianTrajectoryError& cte = innertsos.cartesianError();
  AlgebraicSymMatrix m = cte.matrix();
  math::Error<6>::type cov;
  for( int i = 0; i < 6; ++i )
    for( int j = 0; j <= i; ++j )
      cov( i, j ) = m.fast( i + 1 , j + 1 );
  math::XYZVector mom( p.x(), p.y(), p.z() );
  math::XYZPoint  vtx( v.x(), v.y(), v.z() );   
  
  LogDebug("CosmicTrackFinder") << " RESULT Momentum "<< p;
 
 

  reco::Track theTrack (traj.chiSquared(), 
			int(ndof),//FIXME fix weight() in TrackingRecHit 
			traj.foundHits(),//FIXME to be fixed in Trajectory.h
			0, //FIXME no corresponding method in trajectory.h
			traj.lostHits(),//FIXME to be fixed in Trajectory.h
			charge, 
			vtx,
			mom,
			cov);

  AlgoProduct aProduct(traj,theTrack);
  return aProduct;
  //  return theTrack; 
}
