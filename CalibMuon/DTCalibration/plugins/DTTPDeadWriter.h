#ifndef DTTPDeadWriter_H
#define DTTPDeadWriter_H

/* Class to find test-pulse dead channels from a t0 databases:
 * wires without t0 value are tp-dead.
 
 *  $Date: $
 *  $Revision: $
 *  \author S. Bolognesi
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include <string>

namespace edm {
  class ParameterSet;
  class Event;
  class EventSetup;
}

class DTT0;
class DTDeadFlag;

class DTTPDeadWriter : public edm::EDAnalyzer {
public:
  /// Constructor
  DTTPDeadWriter(const edm::ParameterSet& pset);

  /// Destructor
  virtual ~DTTPDeadWriter();

  // Operations

  ///Read t0 map from event
  void beginJob(const edm::EventSetup& setup);

  /// Compute the ttrig by fiting the TB rising edge
  void analyze(const edm::Event & event, const edm::EventSetup& eventSetup);

  /// Write ttrig in the DB
  void endJob();

 
protected:

private:
  // Debug flag
  bool debug;

  //The map of t0 to be read from event
  const DTT0* tZeroMap;

  // The object to be written to DB
  DTDeadFlag* tpDeadList;
 
  //The DTGeometry
  edm::ESHandle<DTGeometry> muonGeom;
};
#endif
