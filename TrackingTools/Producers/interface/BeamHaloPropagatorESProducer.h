#ifndef TrackingTools_GeomPropagators_BeamHaloPropagatorESProducer_H
#define TrackingTools_GeomPropagators_BeamHaloPropagatorESProducer_H

/** \class BeamHaloPropagatorESProducer
 *  ES producer needed to put the BeamHaloPropagator inside the EventSetup
 *
 *  \author Jean-Roch VLIMANT UCSB
 */

#include "FWCore/Framework/interface/ESProducer.h"

#include "TrackingTools/GeomPropagators/interface/BeamHaloPropagator.h"
#include "DataFormats/TrajectorySeed/interface/PropagationDirection.h"

#include <boost/shared_ptr.hpp>
  

namespace edm {class ParameterSet;}

class TrackingComponentsRecord;

class  BeamHaloPropagatorESProducer: public edm::ESProducer{

 public:
  
  /// Constructor
  BeamHaloPropagatorESProducer(const edm::ParameterSet &);
  
  /// Destructor
  virtual ~BeamHaloPropagatorESProducer(); 
  
  // Operations
  boost::shared_ptr<Propagator> produce(const TrackingComponentsRecord &);
  
 private:
  boost::shared_ptr<Propagator> thePropagator;
  PropagationDirection thePropagationDirection;
  std::string myname;
  std::string theEndCapTrackerPropagatorName;
  std::string theCrossingTrackerPropagatorName;
};

#endif

