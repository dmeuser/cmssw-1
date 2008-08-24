#ifndef HcalLutGenerator_h
#define HcalLutGenerator_h

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class HcalLutGenerator : public edm::EDAnalyzer {
public:
  explicit HcalLutGenerator(const edm::ParameterSet&);
  ~HcalLutGenerator();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  void endJob();

private:
  std::string _tag;
};


#endif
