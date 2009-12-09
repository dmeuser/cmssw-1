#ifndef _RECOMET_METPRODUCER_HCALNOISEINFOPRODUCER_H_
#define _RECOMET_METPRODUCER_HCALNOISEINFOPRODUCER_H_

//
// HcalNoiseInfoProducer.h
//
//   description: Definition of the producer for the HCAL noise information.
//                Uses various algorithms to process digis, rechits, and calotowers
//                and produce a vector of HcalNoiseRBXs.
//                To minimize the time used to sort the data into HPD/RBX space, we fill
//                an rbxarray of size 72, and then pick which rbxs are interesting at
//                the end.
//
//   author: J.P. Chou, Brown
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoMET/METAlgorithms/interface/HcalNoiseRBXArray.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"

namespace reco {

  //
  // class declaration
  //
  
  class HcalNoiseInfoProducer : public edm::EDProducer {
  public:
    explicit HcalNoiseInfoProducer(const edm::ParameterSet&);
    ~HcalNoiseInfoProducer();
    
  private:
    
    //
    // methods inherited from EDProducer
    // produce(...) fills an HcalNoiseRBXArray with information from various places, and then
    // picks which rbxs are interesting, storing them to the EDM.
    //
    
    virtual void beginJob() ;
    virtual void endJob() ;
    virtual void produce(edm::Event&, const edm::EventSetup&);
    virtual void beginRun(edm::Run&, const edm::EventSetup&);
    virtual void endRun(edm::Run&, const edm::EventSetup&);
    
    //
    // more internal methods
    // fills an HcalNoiseRBXArray with various data
    // filldigis() depends on fillrechits() being called first
    //
    void fillrechits(edm::Event&, const edm::EventSetup&, HcalNoiseRBXArray&) const;
    void filldigis(edm::Event&, const edm::EventSetup&, HcalNoiseRBXArray&) const;
    void fillcalotwrs(edm::Event&, const edm::EventSetup&, HcalNoiseRBXArray&, HcalNoiseSummary&) const;
    void filltracks(edm::Event&, const edm::EventSetup&, HcalNoiseSummary&) const;

    // other helper functions
    bool isProblematicRBX(const HcalNoiseRBX& rbx) const;
    void fillOtherSummaryVariables(HcalNoiseSummary& summary, const HcalNoiseRBX& rbx) const;

    
    //
    // parameters
    //      
    
    bool fillDigis_;        // fill digi information into HcalNoiseRBXs
    bool fillRecHits_;      // fill rechit information into HcalNoiseRBXs and HcalNoiseSummary
    bool fillCaloTowers_;   // fill calotower information into HcalNoiseRBXs and HcalNoiseSummary
    bool fillJets_;         // fill jet information into HcalNoiseSummary
    bool fillTracks_;       // fill track information into HcalNoiseSummary

    bool refillRefVectors_; // find HcalNoiseRBXs already present in the event, and fill the RefVectors

    // These provide the requirements for writing an RBX to the event
    double RBXEnergyThreshold_; // minimum energy to even be considered for writing to edm
    double minRecHitEnergy_;    // Minimum rechit energy to consider
    int maxProblemRBXs_;        // maximum number of problematic RBXs to be written to the event record
    bool writeAllRBXs_;         // write all the RBXs (above the energy threshold) to the event, whether "noisy" or not

    // parameters for calculating summary variables
    int maxCaloTowerIEta_;      // maximum caloTower ieta
    double maxTrackEta_;        // maximum eta of the track
    double minTrackPt_;         // minimum track Pt
    
    std::string digiCollName_;         // name of the digi collection
    std::string recHitCollName_;       // name of the rechit collection
    std::string caloTowerCollName_;    // name of the caloTower collection
    std::string trackCollName_;        // name of the track collection
    std::string hcalNoiseRBXCollName_; // name of the HcalNoiseRBX collection that we're adding RefVector information to
    
  };
  
} // end of namespace

#endif
