#ifndef TutorialAtDESY_MuonAnalyzer_H
#define TutorialAtDESY_MuonAnalyzer_H

/** \class MuonAnalyzer
 *  Analyzer of the muon objects
 *
 *  $Date: 2009/06/11 $
 *  $Revision: 1.2 $
 *  \author R. Bellan - CERN <riccardo.bellan@cern.ch>
 */

// Base Class Headers
#include "FWCore/Framework/interface/EDAnalyzer.h"

namespace edm {
  class ParameterSet;
  class Event;
  class EventSetup;
}

class TH1I;
class TH1F;
class TH2F;

class MuonAnalyzer: public edm::EDAnalyzer {
public:
  /// Constructor
  MuonAnalyzer(const edm::ParameterSet& pset);

  /// Destructor
  virtual ~MuonAnalyzer();

  // Operations

  void analyze(const edm::Event & event, const edm::EventSetup& eventSetup);

  virtual void beginJob(const edm::EventSetup& eventSetup) ;
  virtual void endJob() ;
protected:

private:
  std::string theMuonLabel;

  // Histograms
  TH1I *hNMuons;
  TH1F *hPtRec;
  TH2F *hPtReso;
  TH1F *hEHcal;

  TH1I *hMuonType;
  TH1F *hPtSTATKDiff;

  // ID
  TH1F *hMuCaloCompatibility;
  TH1F *hMuSegCompatibility;
  TH1I *hChamberMatched;
  TH1I *hMuIdAlgo;

  // Isolation
  TH1F *hMuIso03SumPt;
  TH1F *hMuIso03CaloComb;
  TH1F *TEST;


};
#endif

