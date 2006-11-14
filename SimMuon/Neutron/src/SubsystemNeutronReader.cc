#include "SimMuon/Neutron/interface/SubsystemNeutronReader.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandPoisson.h"
#include "SimMuon/Neutron/src/NeutronReader.h"
#include "SimMuon/Neutron/src/RootNeutronReader.h"
#include "SimMuon/Neutron/src/AsciiNeutronReader.h"
#include <algorithm>

using namespace std;

SubsystemNeutronReader::SubsystemNeutronReader(const edm::ParameterSet & pset)
: theHitReader(0),
  theLuminosity(pset.getParameter<double>("luminosity")), // in units of 10^34
  theStartTime(pset.getParameter<double>("startTime")), 
  theEndTime(pset.getParameter<double>("endTime")),
  theEventOccupancy(pset.getParameter<vector<double> >("eventOccupancy"))
{
  // 17.3 collisions per live bx, 79.5% of bx live
  float collisionsPerCrossing = 13.75 * theLuminosity;
  int windowSize = (int)((theEndTime-theStartTime)/25.);
  theEventsInWindow = collisionsPerCrossing * windowSize;
  string reader = pset.getParameter<string>("reader");
  edm::FileInPath input = pset.getParameter<edm::FileInPath>("input");
  int nChamberTypes = pset.getParameter<int>("nChamberTypes");

  if(reader == "ASCII")
  {
    theHitReader = new AsciiNeutronReader(input.fullPath(), nChamberTypes);
  }
  else if (reader == "ROOT")
  {
    theHitReader = new RootNeutronReader(input.fullPath());
  }
}


SubsystemNeutronReader::~SubsystemNeutronReader() {
std::cout << "DELETING NEUTRONREADER " << std::endl;
  delete theHitReader;
}


void
SubsystemNeutronReader::generateChamberNoise(int chamberType, int chamberIndex, 
                                             edm::PSimHitContainer & result) 
{
  // make sure this chamber hasn't been done before
  if(find(theChambersDone.begin(), theChambersDone.end(), chamberIndex) 
     == theChambersDone.end()) 
  {
    float meanNumberOfEvents = theEventOccupancy[chamberType] 
                             * theEventsInWindow;
    int nEventsToAdd = RandPoisson::shoot(meanNumberOfEvents);
//    LogDebug("NeutronReader") << "Number of neutron events to add: " 
std::cout << "Number of neutron events to add: "
 << nEventsToAdd <<  " mean " << meanNumberOfEvents << std::endl;
//                   << nEventsToAdd <<  " mean " << meanNumberOfEvents;

    for(int i = 0; i < nEventsToAdd; ++i) {
      // find the time for this event
      float timeOffset = theStartTime
        + RandFlat::shoot() * (theEndTime-theStartTime);
      vector<PSimHit> neutronHits;
      theHitReader->readNextEvent(chamberType, neutronHits);

      for( vector<PSimHit>::const_iterator neutronHitItr = neutronHits.begin();
           neutronHitItr != neutronHits.end(); ++neutronHitItr)
      {
         const PSimHit & rawHit = *neutronHitItr;
         // do the time offset and local det id
         int det = detId(chamberIndex, rawHit.detUnitId());
         PSimHit hit(rawHit.entryPoint(), rawHit.exitPoint(), rawHit.pabs(),
                     rawHit.tof()+timeOffset,
                     rawHit.energyLoss(), rawHit.particleType(),
                     det, rawHit.trackId(),
                     rawHit.thetaAtEntry(),  rawHit.phiAtEntry(), rawHit.processType());
std::cout << "NEWHIT " << hit << std::endl;
         result.push_back(hit);
      }

    }
    theChambersDone.push_back(chamberIndex);
  }
}

