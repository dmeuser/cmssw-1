#ifndef TauAnalysis_BgEstimationTools_FakeRateJetWeightProducer_h  
#define TauAnalysis_BgEstimationTools_FakeRateJetWeightProducer_h

/** \class FakeRateJetWeightProducer
 *
 * Produce ValueMap associating reco::PFTau objects with pat::LookupTableRecords;
 * entries of the pat::LookupTableRecords represent the probability for a QCD-jet
 * with a given (jet-radius, eta and Et) to fake a tau-jet signature
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.3 $
 *
 * $Id: FakeRateJetWeightProducer.h,v 1.3 2009/10/16 12:45:59 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/BgEstimationTools/interface/FakeRateWeightProducerBase.h"

class FakeRateJetWeightProducer : public FakeRateWeightProducerBase
{
 public:
  
  explicit FakeRateJetWeightProducer(const edm::ParameterSet&);
  ~FakeRateJetWeightProducer();
  
 private:

  void beginJob(const edm::EventSetup&) {}
  void produce(edm::Event&, const edm::EventSetup&);
  void endJob() {}

};

#endif  


