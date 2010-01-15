#ifndef EwkElecDQM_H
#define EwkElecDQM_H

/** \class EwkElecDQM
 *
 *  DQM offline for EWK Electrons
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

class DQMStore;
class MonitorElement;
class EwkElecDQM : public edm::EDAnalyzer {
public:
  EwkElecDQM (const edm::ParameterSet &);
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void beginJob(const edm::EventSetup&);
  virtual void endJob();
  virtual void beginRun(const edm::Run&, const edm::EventSetup&);
  virtual void endRun(const edm::Run&, const edm::EventSetup&);

  void init_histograms();
private:

  edm::InputTag trigTag_;
  //  edm::InputTag muonTag_;
  edm::InputTag elecTag_;
  //  edm::InputTag metTag_;
  //  bool metIncludesMuons_;
  //  edm::InputTag jetTag_;

  //  const std::string muonTrig_;
  const std::string elecTrig_;
  double ptCut_;
  double etaCut_;

  double sieieCutBarrel_;
  double sieieCutEndcap_;
  double detainCutBarrel_;
  double detainCutEndcap_;

  //  bool isRelativeIso_;
  //  bool isCombinedIso_;

  //  double isoCut03_;
  double ecalIsoCutBarrel_;
  double ecalIsoCutEndcap_;
  double hcalIsoCutBarrel_;
  double hcalIsoCutEndcap_;
  double trkIsoCutBarrel_;
  double trkIsoCutEndcap_;
  //  double mtMin_;
  //  double mtMax_;
  //  double metMin_;
  //  double metMax_;
  //  double acopCut_;

  //  double dxyCut_;
  //  double normalizedChi2Cut_;
  //  int trackerHitsCut_;
  //  bool isAlsoTrackerMuon_;

  //  double ptThrForZ1_;
  //  double ptThrForZ2_;

  //  double eJetMin_;
  //  int nJetMax_;

  unsigned int nall;
  unsigned int nrec;
  unsigned int neid;
  unsigned int niso;
/*   unsigned int nhlt; */
/*   unsigned int nmet; */
  unsigned int nsel;

  //  unsigned int nRecoElectrons;
  unsigned int nGoodElectrons;

  DQMStore* theDbe;

  MonitorElement* pt_before_;
  MonitorElement* pt_after_;

  MonitorElement* eta_before_;
  MonitorElement* eta_after_;

  MonitorElement* sieiebarrel_before_;
  MonitorElement* sieiebarrel_after_;

  MonitorElement* sieieendcap_before_;
  MonitorElement* sieieendcap_after_;

  MonitorElement* detainbarrel_before_;
  MonitorElement* detainbarrel_after_;

  MonitorElement* detainendcap_before_;
  MonitorElement* detainendcap_after_;

/*   MonitorElement* dxy_before_; */
/*   MonitorElement* dxy_after_; */

/*   MonitorElement* chi2_before_; */
/*   MonitorElement* chi2_after_; */

/*   MonitorElement* nhits_before_; */
/*   MonitorElement* nhits_after_; */

/*   MonitorElement* tkmu_before_; */
/*   MonitorElement* tkmu_after_; */

  MonitorElement* ecalisobarrel_before_;
  MonitorElement* ecalisobarrel_after_;

  MonitorElement* ecalisoendcap_before_;
  MonitorElement* ecalisoendcap_after_;

  MonitorElement* hcalisobarrel_before_;
  MonitorElement* hcalisobarrel_after_;

  MonitorElement* hcalisoendcap_before_;
  MonitorElement* hcalisoendcap_after_;

  MonitorElement* trkisobarrel_before_;
  MonitorElement* trkisobarrel_after_;

  MonitorElement* trkisoendcap_before_;
  MonitorElement* trkisoendcap_after_;

  MonitorElement* trig_before_;
  MonitorElement* trig_after_;

  MonitorElement* invmass_before_;
  MonitorElement* invmass_after_;

  MonitorElement* nelectrons_before_;
  MonitorElement* nelectrons_after_;

/*   MonitorElement* mt_before_; */
/*   MonitorElement* mt_after_; */

/*   MonitorElement* met_before_; */
/*   MonitorElement* met_after_; */

/*   MonitorElement* acop_before_; */
/*   MonitorElement* acop_after_; */

/*   MonitorElement* nz1_before_; */
/*   MonitorElement* nz1_after_; */

/*   MonitorElement* nz2_before_; */
/*   MonitorElement* nz2_after_; */

/*   MonitorElement* njets_before_; */
/*   MonitorElement* njets_after_; */
};

#endif
