#ifndef TrackingTools_PatternTools_TrajectoryBuilder_h
#define TrackingTools_PatternTools_TrajectoryBuilder_h

#include "TrackingTools/PatternTools/interface/TrajectoryMeasurement.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

class Trajectory;
class TrajectorySeed;

/** The component of track reconstruction that, strating from a seed,
 *  reconstructs all possible trajectories.
 *  The resulting trajectories may be mutually exclusive and require
 *  cleaning by a TrajectoryCleaner.
 *  The Trajectories are normally not smoothed.
 */

class TrajectoryBuilder {
public:

  typedef std::vector<Trajectory> TrajectoryContainer;
  typedef TrajectoryContainer::iterator TrajectoryIterator;

  virtual ~TrajectoryBuilder() {};

  virtual TrajectoryContainer trajectories(const TrajectorySeed&) const = 0;

  /** Interface for trajectories re-building in the seeding region method.
      It has to be correctly implemented in the concrete class
  **/
  virtual void  rebuildSeedingRegion(const TrajectorySeed&,
				     TrajectoryContainer& result) const {    
    edm::LogWarning("TrajectoryBuilding") 
      << "WARNING: you are using a trajectory builder which is not overloading the rebuildSeedingRegion method. Default implementation is dummy: output TrajectoryContainer is equal to inputTrajectoryContainer";
  }

  virtual void setEvent(const edm::Event& event) const = 0;

};


#endif
