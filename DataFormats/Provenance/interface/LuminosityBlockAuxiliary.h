#ifndef DataFormats_Provenance_LuminosityBlockAuxiliary_h
#define DataFormats_Provenance_LuminosityBlockAuxiliary_h

#include <iosfwd>

#include "DataFormats/Provenance/interface/ProcessHistoryID.h"
#include "DataFormats/Provenance/interface/LuminosityBlockID.h"
#include "DataFormats/Provenance/interface/RunID.h"
#include "DataFormats/Provenance/interface/Timestamp.h"

// Auxiliary luminosity block data that is persistent

namespace edm
{
  struct LuminosityBlockAuxiliary {
    LuminosityBlockAuxiliary() :
	processHistoryID_(),
	id_(),
	time_() {}
    LuminosityBlockAuxiliary(LuminosityBlockID const& theId, Timestamp const& theTime) :
	processHistoryID_(),
	id_(theId),
	time_(theTime) {}
    LuminosityBlockAuxiliary(RunNumber_t const& theRun, LuminosityBlockNumber_t const& theLumi, Timestamp const& theTime) :
	processHistoryID_(),
	id_(theRun, theLumi),
	time_(theTime) {}
    ~LuminosityBlockAuxiliary() {}
    void write(std::ostream& os) const;
    ProcessHistoryID& processHistoryID() const {return processHistoryID_;}
    LuminosityBlockNumber_t luminosityBlock() const {return id().luminosityBlock();}
    RunNumber_t run() const {return id().run();}
    LuminosityBlockID const& id() const {return id_;}
    Timestamp const& time() const {return time_;}
    // most recent process that processed this lumi block
    // is the last on the list, this defines what "latest" is
    mutable ProcessHistoryID processHistoryID_;
    // LuminosityBlock ID
    LuminosityBlockID id_;
    // Time from DAQ
    Timestamp time_;
  };

  inline
  std::ostream&
  operator<<(std::ostream& os, const LuminosityBlockAuxiliary& p) {
    p.write(os);
    return os;
  }

}
#endif
