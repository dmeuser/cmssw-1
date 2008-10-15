#ifndef EgammaIsolationProducers_EgammaElectronTkIsolationProducer_h
#define EgammaIsolationProducers_EgammaElectronTkIsolationProducer_h

//*****************************************************************************
// File:      EgammaElectronTkIsolationProducer.h
// ----------------------------------------------------------------------------
// OrigAuth:  Matthias Mozer
// Institute: IIHE-VUB
//=============================================================================
//*****************************************************************************

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

class EgammaElectronTkIsolationProducer : public edm::EDProducer {
 public:
  explicit EgammaElectronTkIsolationProducer(const edm::ParameterSet&);
  ~EgammaElectronTkIsolationProducer();
  
  virtual void produce(edm::Event&, const edm::EventSetup&);

 private:
  edm::InputTag electronProducer_;
  edm::InputTag trackProducer_;
  edm::InputTag beamspotProducer_;

  double ptMin_;
  double intRadius_;
  double extRadius_;
  double maxVtxDist_;
  double drb_;
  
  edm::ParameterSet conf_;

};


#endif
