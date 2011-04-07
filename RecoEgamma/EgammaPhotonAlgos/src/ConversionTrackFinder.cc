//
#include "RecoEgamma/EgammaPhotonAlgos/interface/ConversionTrackFinder.h"
//
#include "RecoTracker/Record/interface/CkfComponentsRecord.h"
#include "RecoTracker/CkfPattern/interface/TransientInitialStateEstimator.h"
//

#include "TrackingTools/KalmanUpdators/interface/KFUpdator.h"
//
//

#include <sstream>


ConversionTrackFinder::ConversionTrackFinder(const edm::EventSetup& es, 
					     const edm::ParameterSet& conf ) :  
  conf_(conf), 
  theCkfTrajectoryBuilder_(0), 
  theInitialState_(0),
  theTrackerGeom_(0),
  theUpdator_(0),
  thePropagator_(0) 
{
  //  std::cout << " ConversionTrackFinder base CTOR " << std::endl;

  edm::ParameterSet tise_params = conf_.getParameter<edm::ParameterSet>("TransientInitialStateEstimatorParameters") ;
  theInitialState_       = new TransientInitialStateEstimator( es,  tise_params);
  useSplitHits_ =  conf_.getParameter<bool>("useHitsSplitting");


}


ConversionTrackFinder::~ConversionTrackFinder() {


  delete theInitialState_;

}


void ConversionTrackFinder::setEventSetup(const edm::EventSetup& es )   {

  edm::ESHandle<MeasurementTracker> measurementTrackerHandle;
  es.get<CkfComponentsRecord>().get(measurementTrackerHandle);
  theMeasurementTracker_ = measurementTrackerHandle.product();

 // set the TrajectoryBuilder
  std::string trajectoryBuilderName = conf_.getParameter<std::string>("TrajectoryBuilder");
  //  std::cout << " ConversionTrackFinderBase tbName" <<  trajectoryBuilderName << std::endl;
  edm::ESHandle<TrajectoryBuilder> theTrajectoryBuilderHandle;
  es.get<CkfComponentsRecord>().get(trajectoryBuilderName,theTrajectoryBuilderHandle);
  theCkfTrajectoryBuilder_ = theTrajectoryBuilderHandle.product();

  edm::ESHandle<TrackerGeometry> trackerHandle;
  es.get<TrackerDigiGeometryRecord>().get(trackerHandle);
  theTrackerGeom_= trackerHandle.product();

  es.get<TrackingComponentsRecord>().get("AnyDirectionAnalyticalPropagator",
					thePropagator_);

  theInitialState_->setEventSetup( es );
}

void ConversionTrackFinder::setEvent(const edm::Event& e )   {
  theMeasurementTracker_->update( e );



}
