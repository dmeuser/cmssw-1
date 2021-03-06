//
// filter module to run over events between firstEvent and lastEvent
// NB: the interval is inclusive! both firstEvent and lastEvent are selected OR rejected
// Author: Shahram Rahatlou, University of Rome & INFN
// Date:   16 Dec 2005
// $Id: EcalEventFilter.h,v 1.2 2006/04/21 01:45:25 wmtan Exp $
//
#ifndef RECOTBCALO_ECALHVSCAN_ECALEVENTFILTER_HH
#define RECOTBCALO_ECALHVSCAN_ECALEVENTFILTER_HH

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include <string>

class EcalEventFilter : public edm::EDFilter {

private:
  int firstEvt_;
  int lastEvt_;
  int rejectedEvt_;
  bool selectInBetween_;

public:
  explicit EcalEventFilter(const edm::ParameterSet& );
  virtual ~EcalEventFilter();
  virtual bool filter(edm::Event& e, edm::EventSetup const& c);
  virtual void endJob();


};
#endif
