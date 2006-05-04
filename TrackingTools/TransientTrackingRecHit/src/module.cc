#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"
#include "TrackingTools/TransientTrackingRecHit/interface/GenericTransientTrackingRecHitBuilderESProducer.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/ESProducer.h"
#include "TrackingTools/PatternTools/interface/TrajectoryStateUpdator.h" 
#include "FWCore/Framework/interface/eventsetupdata_registration_macro.h"

EVENTSETUP_DATA_REG(TransientTrackingRecHitBuilder);
DEFINE_FWK_EVENTSETUP_MODULE(GenericTransientTrackingRecHitBuilderESProducer)


