#ifndef TauAnalysis_Core_DiTauCandidateHistManager_h  
#define TauAnalysis_Core_DiTauCandidateHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TauAnalysis/Core/interface/HistManagerBase.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

#include <vector>
#include <string>

template<typename T1, typename T2>
class CompositePtrCandidateT1T2MEtHistManager : public HistManagerBase 
{
 public:
  
  explicit CompositePtrCandidateT1T2MEtHistManager(const edm::ParameterSet&);
  ~CompositePtrCandidateT1T2MEtHistManager();
  
  void bookHistograms(const edm::EventSetup&);
  void fillHistograms(const edm::Event&, const edm::EventSetup&);

 private:

//--- configuration parameters
  edm::InputTag diTauCandidateSrc_;

  std::string dqmDirectory_store_;

  bool requireGenMatch_;

//--- histograms
  MonitorElement* hDiTauCandidatePt_;
  MonitorElement* hDiTauCandidateEta_;
  MonitorElement* hDiTauCandidatePhi_;
  MonitorElement* hDiTauCandidateCharge_;
  MonitorElement* hDiTauCandidateMass_;

  MonitorElement* hVisPt_;
  MonitorElement* hVisPhi_;
  MonitorElement* hVisMass_;

  MonitorElement* hCollinearApproxEta_;
  MonitorElement* hCollinearApproxMass_;
  MonitorElement* hCollinearApproxMassVsPt_;
  MonitorElement* hCollinearApproxMassVsDPhi12_;
  MonitorElement* hCollinearApproxX1_;
  MonitorElement* hCollinearApproxX2_;
  MonitorElement* hCollinearApproxX1vsX2_;

  MonitorElement* hCDFmethodMass_;

  MonitorElement* hMt12MET_;

  MonitorElement* hMt1MET_;
  MonitorElement* hMt2MET_;
  
  MonitorElement* hDPhi12_;
  MonitorElement* hDR12_;

  MonitorElement* hVisEtaMin_;
  MonitorElement* hVisEtaMax_;

  MonitorElement* hDPhi1MET_;
  MonitorElement* hDPhi2MET_;
  MonitorElement* hDPhi1METvsDPhi2MET_;
};

#endif  


