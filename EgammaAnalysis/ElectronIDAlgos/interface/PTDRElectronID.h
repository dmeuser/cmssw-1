#ifndef PTDRElectronID_H
#define PTDRElectronID_H

#include "EgammaAnalysis/ElectronIDAlgos/interface/ElectronIDAlgo.h"

class PTDRElectronID : public ElectronIDAlgo {

public:

  PTDRElectronID(){};

  virtual ~PTDRElectronID(){};

  void setup(const edm::ParameterSet& conf);
  double result(const reco::GsfElectron*, const edm::Event&);

 private:

  std::string quality_;

  std::vector<int> useEoverPIn_;
  std::vector<int> useDeltaEtaIn_;
  std::vector<int> useDeltaPhiIn_;
  std::vector<int> useHoverE_;
  std::vector<int> useE9overE25_;
  std::vector<int> useEoverPOut_;
  std::vector<int> useDeltaPhiOut_;
  std::vector<int> useInvEMinusInvP_;
  std::vector<int> useBremFraction_;
  std::vector<int> useSigmaEtaEta_;
  std::vector<int> useSigmaPhiPhi_;
  std::vector<int> acceptCracks_;
  
  edm::ParameterSet cuts_;

  int variables_;
};

#endif // PTDRElectronID_H
