#ifndef _dqm_sistripcommissioningsources_SiStripCommissioningBasicPrescaler_h_
#define _dqm_sistripcommissioningsources_SiStripCommissioningBasicPrescaler_h_

// system include files
#include <memory>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <FWCore/ParameterSet/interface/InputTag.h>


//
// class declaration
//

class SiStripCommissioningBasicPrescaler : public edm::EDFilter {
   public:
      explicit SiStripCommissioningBasicPrescaler(const edm::ParameterSet&);
      ~SiStripCommissioningBasicPrescaler() {}

   private:
      virtual void beginJob(const edm::EventSetup&) {}
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() {}
      // ----------member data ---------------------------
      uint32_t factor_;
};

#endif
