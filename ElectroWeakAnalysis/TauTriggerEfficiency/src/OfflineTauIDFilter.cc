// -*- C++ -*-
//
// Package:    OfflineTauIDFilter
// Class:      OfflineTauIDFilter
// 
/**\class OfflineTauIDFilter OfflineTauIDFilter.cc ElectroWeakAnalysis/OfflineTauIDFilter/src/OfflineTauIDFilter.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Matti Kortelainen
//         Created:  Thu Sep 18 11:07:16 CEST 2008
// $Id: OfflineTauIDFilter.cc,v 1.5 2008/11/20 14:08:59 slehti Exp $
//
//

#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/OfflineTauIDFilter.h"

#include "DataFormats/TauReco/interface/CaloTau.h"
#include "DataFormats/TauReco/interface/PFTau.h"

#include "RecoTauTag/TauTagTools/interface/CaloTauElementsOperators.h"
#include "RecoTauTag/TauTagTools/interface/PFTauElementsOperators.h"

OfflineTauIDFilter::OfflineTauIDFilter(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
  _PfTauSource = iConfig.getParameter<edm::InputTag>("PfTauSource");
  _CaloTauSource = iConfig.getParameter<edm::InputTag>("CaloTauSource");

  _nEvents = 0;
  _nPassedEvents = 0;
}


OfflineTauIDFilter::~OfflineTauIDFilter()
{
}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
OfflineTauIDFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  edm::Handle<reco::PFTauCollection> thePFTauHandle;
  iEvent.getByLabel(_PfTauSource,thePFTauHandle);

  edm::Handle<reco::CaloTauCollection> theCaloTauHandle;
  iEvent.getByLabel(_CaloTauSource,theCaloTauHandle);

  bool passed = false;
  ++_nEvents;

  if(thePFTauHandle.isValid() && thePFTauHandle->size() >= 1)
    passed = true;
  if(theCaloTauHandle.isValid() && theCaloTauHandle->size() >= 1)
    passed = true;
  
  if(passed)
    ++_nPassedEvents;

  return passed;
}

// ------------ method called once each job just before starting event loop  ------------
void 
OfflineTauIDFilter::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
OfflineTauIDFilter::endJob() {
  edm::LogInfo("OffLineTauIDFilter") << "Total " << _nEvents << " events, "
                                << "from which " << _nPassedEvents << " events passed the filter" << std::endl; 
}

//define this as a plug-in
DEFINE_FWK_MODULE(OfflineTauIDFilter);
