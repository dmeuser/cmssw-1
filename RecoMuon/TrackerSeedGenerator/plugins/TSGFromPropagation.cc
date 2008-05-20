#include "RecoMuon/TrackerSeedGenerator/plugins/TSGFromPropagation.h"

/** \class TSGFromPropagation
 *
 *  $Date: 2008/05/11 00:55:13 $
 *  $Revision: 1.26 $
 *  \author Chang Liu - Purdue University 
 */

#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/MeasurementDet/interface/LayerMeasurements.h"
#include "TrackingTools/KalmanUpdators/interface/Chi2MeasurementEstimator.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"
#include "TrackingTools/GeomPropagators/interface/StateOnTrackerBound.h"

#include "RecoTracker/Record/interface/TrackerRecoGeometryRecord.h"
#include "RecoTracker/Record/interface/CkfComponentsRecord.h"
#include "RecoTracker/MeasurementDet/interface/TkStripMeasurementDet.h"
#include "RecoTracker/MeasurementDet/interface/MeasurementTracker.h"
#include "RecoTracker/TkDetLayers/interface/GeometricSearchTracker.h"

#include "RecoMuon/GlobalTrackingTools/interface/DirectTrackerNavigation.h"
#include "TrackingTools/KalmanUpdators/interface/KFUpdator.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"


TSGFromPropagation::TSGFromPropagation(const edm::ParameterSet & iConfig) :theTkLayerMeasurements (0), theTracker(0), theMeasTracker(0), theNavigation(0), theService(0), theEstimator(0), theTSTransformer(0), theConfig (iConfig)
{
  theCategory = "Muon|RecoMuon|TSGFromPropagation";
}

TSGFromPropagation::TSGFromPropagation(const edm::ParameterSet & iConfig, const MuonServiceProxy* service) : theTkLayerMeasurements (0), theTracker(0), theMeasTracker(0), theNavigation(0), theService(service),theUpdator(0), theEstimator(0), theTSTransformer(0), theConfig (iConfig)
{
  theCategory = "Muon|RecoMuon|TSGFromPropagation";
}

TSGFromPropagation::~TSGFromPropagation()
{

  LogTrace(theCategory) << " TSGFromPropagation dtor called ";
  if ( theNavigation ) delete theNavigation;
  if ( theUpdator ) delete theUpdator;
  if ( theEstimator ) delete theEstimator;
  if ( theTkLayerMeasurements ) delete theTkLayerMeasurements;
  if ( theTSTransformer ) delete  theTSTransformer;
  if ( theErrorMatrixAdjuster ) delete theErrorMatrixAdjuster;

}

void TSGFromPropagation::trackerSeeds(const TrackCand& staMuon, const TrackingRegion& region, std::vector<TrajectorySeed> & result) {

  TrajectoryStateOnSurface staState = outerTkState(staMuon);

  if ( !staState.isValid() ) { 
    LogTrace(theCategory) << "Error: initial state from L2 muon is invalid.";
    return;
  }

  LogTrace(theCategory) << "begin of trackerSeed:\n staState pos: "<<staState.globalPosition()
                        << " mom: "<<staState.globalMomentum() 
                        <<"pos eta: "<<staState.globalPosition().eta()
                        <<"mom eta: "<<staState.globalMomentum().eta();

  std::vector<const DetLayer*> nls = theNavigation->compatibleLayers(*(staState.freeState()), oppositeToMomentum);

  LogTrace(theCategory) << " compatible layers: "<<nls.size();

  if ( nls.empty() ) return;

  int ndesLayer = 0;

  bool usePredictedState = false;

  if ( theUpdateStateFlag ) { //use updated states
     std::vector<TrajectoryMeasurement> alltm; 

     for (std::vector<const DetLayer*>::const_iterator inl = nls.begin();
         inl != nls.end(); inl++, ndesLayer++ ) {
         if ( (*inl == 0) ) break;
         alltm = findMeasurements_new(*inl, staState);
         if ( (!alltm.empty()) ) {
            LogTrace(theCategory) << "final compatible layer: "<<ndesLayer;
            break;
         }
     }

     if ( alltm.empty() ) {
        LogTrace(theCategory) << " NO Measurements Found: eta: "<<staState.globalPosition().eta() <<"pt "<<staState.globalMomentum().perp();
        usePredictedState = true;
     } else {
       LogTrace(theCategory) << " Measurements for seeds: "<<alltm.size();
       std::stable_sort(alltm.begin(),alltm.end(),increasingEstimate()); 
       if ( alltm.size() > 5 ) alltm.erase(alltm.begin() + 5, alltm.end());

       int i = 0;
       for (std::vector<TrajectoryMeasurement>::const_iterator itm = alltm.begin();
            itm != alltm.end(); itm++, i++) {
            TrajectoryStateOnSurface updatedTSOS = updator()->update(itm->predictedState(), *(itm->recHit()));
            if ( updatedTSOS.isValid() )  {
               edm::OwnVector<TrackingRecHit> container;
               container.push_back(itm->recHit()->hit()->clone());
               TrajectorySeed ts = createSeed(updatedTSOS, container, itm->recHit()->geographicalId());
               result.push_back(ts);
            }
       }
     return;
    }
  }

  if ( !theUpdateStateFlag || usePredictedState ) { //use predicted states
     LogTrace(theCategory) << "use predicted state: ";
     for (std::vector<const DetLayer*>::const_iterator inl = nls.begin();
         inl != nls.end(); inl++ ) {

         if ( !result.empty() || *inl == 0 ) {
            break;
         }
         std::vector<DetLayer::DetWithState> compatDets = (*inl)->compatibleDets(staState, *propagator(), *estimator());
         LogTrace(theCategory) << " compatDets "<<compatDets.size();
         if ( compatDets.empty() ) continue;
         TrajectorySeed ts = createSeed(compatDets.front().second, compatDets.front().first->geographicalId());
         result.push_back(ts);

     }
     LogTrace(theCategory) << "result: "<<result.size();
     return;
  } 
  return;
}

void TSGFromPropagation::init(const MuonServiceProxy* service) {

  theMaxChi2 = theConfig.getParameter<double>("MaxChi2");

  theErrorRescaling = theConfig.getParameter<double>("ErrorRescaling");

  theEstimator = new Chi2MeasurementEstimator(theMaxChi2);

  theCacheId_MT = 0;

  thePropagatorName = theConfig.getParameter<std::string>("Propagator");

  theService = service;

  theUseVertexStateFlag = theConfig.getParameter<bool>("UseVertexState");

  theUpdateStateFlag = theConfig.getParameter<bool>("UpdateState");

  theUseSecondMeasurementsFlag = theConfig.getParameter<bool>("UseSecondMeasurements");

  theUpdator = new KFUpdator();

  theTSTransformer = new TrajectoryStateTransform();

  edm::ParameterSet errorMatrixPset = theConfig.getParameter<edm::ParameterSet>("errorMatrixPset");
  if (!errorMatrixPset.empty()){
    theAdjustAtIp = errorMatrixPset.getParameter<bool>("atIP");
    theErrorMatrixAdjuster = new MuonErrorMatrix(errorMatrixPset);
  } else {
    theAdjustAtIp =false;
    theErrorMatrixAdjuster=0;
  }

  theService->eventSetup().get<TrackerRecoGeometryRecord>().get(theTracker); 
  theNavigation = new DirectTrackerNavigation(theTracker);

}

void TSGFromPropagation::setEvent(const edm::Event& iEvent) {

  bool measTrackerChanged = false;

  unsigned long long newCacheId_MT = theService->eventSetup().get<CkfComponentsRecord>().cacheIdentifier();

  if ( newCacheId_MT != theCacheId_MT ) {
    LogTrace(theCategory) << "Measurment Tracker Geometry changed!";
    theCacheId_MT = newCacheId_MT;
    theService->eventSetup().get<CkfComponentsRecord>().get(theMeasTracker);
    measTrackerChanged = true;
  }

  theMeasTracker->update(iEvent);

  if ( measTrackerChanged && (&*theMeasTracker) ) {
     if ( theTkLayerMeasurements ) delete theTkLayerMeasurements;
     theTkLayerMeasurements = new LayerMeasurements(&*theMeasTracker);
  }

}

TrajectoryStateOnSurface TSGFromPropagation::innerState(const TrackCand& staMuon) const {

  TrajectoryStateOnSurface innerTS;

  if ( staMuon.first && staMuon.first->isValid() ) {
    if (staMuon.first->direction() == alongMomentum) {
      innerTS = staMuon.first->firstMeasurement().updatedState();
    } 
    else if (staMuon.first->direction() == oppositeToMomentum) { 
      innerTS = staMuon.first->lastMeasurement().updatedState();
    }
  } else {
    innerTS = theTSTransformer->innerStateOnSurface(*(staMuon.second),*theService->trackingGeometry(), &*theService->magneticField());
  }
  //rescale the error
  if (theErrorMatrixAdjuster && !theAdjustAtIp) adjust(innerTS);
  else innerTS.rescaleError(theErrorRescaling);

  return  innerTS;

//    return theTSTransformer->innerStateOnSurface(*(staMuon.second),*theService->trackingGeometry(), &*theService->magneticField());
}

TrajectoryStateOnSurface TSGFromPropagation::outerTkState(const TrackCand& staMuon) const {

  TrajectoryStateOnSurface result;

  if ( theUseVertexStateFlag && staMuon.second->pt() > 1.0 ) {
    FreeTrajectoryState iniState = theTSTransformer->initialFreeState(*(staMuon.second), &*theService->magneticField());
    //rescale the error at IP
    if (theErrorMatrixAdjuster && theAdjustAtIp){ adjust(iniState); }
    else iniState.rescaleError(theErrorRescaling);

    StateOnTrackerBound fromInside(&*(theService->propagator("PropagatorWithMaterial")));
    result = fromInside(iniState);
  } else {
    StateOnTrackerBound fromOutside(&*propagator());
    result = fromOutside(innerState(staMuon));
  }
  return result;
}

TrajectorySeed TSGFromPropagation::createSeed(const TrajectoryStateOnSurface& tsos, const DetId& id) const {

  edm::OwnVector<TrackingRecHit> container;
  return createSeed(tsos, container, id);

}

TrajectorySeed TSGFromPropagation::createSeed(const TrajectoryStateOnSurface& tsos, const edm::OwnVector<TrackingRecHit>& container, const DetId& id) const {

  PTrajectoryStateOnDet* seedTSOS = theTSTransformer->persistentState(tsos,id.rawId());
  return TrajectorySeed(*seedTSOS,container,oppositeToMomentum);

}

/// further clean measurements
void TSGFromPropagation::selectMeasurements(std::vector<TrajectoryMeasurement>& tms) const {

}


void TSGFromPropagation::validMeasurements(std::vector<TrajectoryMeasurement>& tms) const {

  std::vector<TrajectoryMeasurement>::iterator tmsend = std::remove_if(tms.begin(), tms.end(), isInvalid());
  tms.erase(tmsend, tms.end());
  return;

}

std::vector<TrajectoryMeasurement> TSGFromPropagation::findMeasurements_new(const DetLayer* nl, const TrajectoryStateOnSurface& staState) const {

  std::vector<TrajectoryMeasurement> result;

  std::vector<DetLayer::DetWithState> compatDets = nl->compatibleDets(staState, *propagator(), *estimator());
  if ( compatDets.empty() )  return result;

  for (std::vector<DetLayer::DetWithState>::const_iterator idws = compatDets.begin(); idws != compatDets.end(); ++idws) {
     if ( idws->second.isValid() && (idws->first) )  {
         std::vector<TrajectoryMeasurement> tmptm = 
           theMeasTracker->idToDet(idws->first->geographicalId())->fastMeasurements(idws->second, idws->second, *propagator(), *estimator());
         validMeasurements(tmptm);
//         if ( tmptm.size() > 2 ) {
//            std::stable_sort(tmptm.begin(),tmptm.end(),increasingEstimate());
//            result.insert(result.end(),tmptm.begin(), tmptm.begin()+2);
//         } else {
            result.insert(result.end(),tmptm.begin(), tmptm.end());
//         }
     }
  }
  
  return result;

}

std::vector<TrajectoryMeasurement> TSGFromPropagation::findMeasurements(const DetLayer* nl, const TrajectoryStateOnSurface& staState) const {

  std::vector<TrajectoryMeasurement> result = tkLayerMeasurements()->measurements((*nl), staState, *propagator(), *estimator());
  validMeasurements(result);
  return result;
}

void TSGFromPropagation::findSecondMeasurements(std::vector<TrajectoryMeasurement>& tms, const std::vector<const DetLayer*>& dls) const {

}

bool TSGFromPropagation::passSelection(const TrajectoryStateOnSurface& tsos) const {

  double theSigmaZ = 20;
  double theDxyCut = 100;
  return ( (zDis(tsos) < theSigmaZ) && (dxyDis(tsos) < theDxyCut) );

}

double TSGFromPropagation::dxyDis(const TrajectoryStateOnSurface& tsos) const {
  return fabs(( - tsos.globalPosition().x() * tsos.globalMomentum().y() + tsos.globalPosition().y() * tsos.globalMomentum().x() )/tsos.globalMomentum().perp());
}

double TSGFromPropagation::zDis(const TrajectoryStateOnSurface& tsos) const {
  return fabs(tsos.globalPosition().z() - tsos.globalPosition().perp() * tsos.globalMomentum().z()/tsos.globalMomentum().perp());
}

void TSGFromPropagation::adjust(FreeTrajectoryState & state) const {
  CurvilinearTrajectoryError oMat = state.curvilinearError();
  CurvilinearTrajectoryError sfMat = theErrorMatrixAdjuster->get(state.momentum());//FIXME with position
  MuonErrorMatrix::multiply(oMat, sfMat);
  
  state = FreeTrajectoryState(state.parameters(),
			      oMat);
}

void TSGFromPropagation::adjust(TrajectoryStateOnSurface & state) const {
  CurvilinearTrajectoryError oMat = state.curvilinearError();
  CurvilinearTrajectoryError sfMat = theErrorMatrixAdjuster->get(state.globalMomentum());//FIXME with position
  MuonErrorMatrix::multiply(oMat, sfMat);
  
  state = TrajectoryStateOnSurface(state.globalParameters(),
				   oMat,
				   state.surface(),
				   state.surfaceSide(),
				   state.weight());
}

