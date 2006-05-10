/**\class EgammaSimpleAnalyzer

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Shahram Rahatlou
//         Created:  10 May 2006
// $Id: $
//

#include "RecoEcal/EgammaClusterProducers/interface/EgammaSimpleAnalyzer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/Handle.h"

#include "TFile.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"


//========================================================================
EgammaSimpleAnalyzer::EgammaSimpleAnalyzer( const edm::ParameterSet& ps )
//========================================================================
{

  islandBasicClusterCollection_ = ps.getParameter<std::string>("islandBasicClusterCollection");
  islandBasicClusterProducer_   = ps.getParameter<std::string>("islandBasicClusterProducer");

  islandSuperClusterCollection_ = ps.getParameter<std::string>("islandSuperClusterCollection");
  islandSuperClusterProducer_   = ps.getParameter<std::string>("islandSuperClusterProducer");

  correctedIslandSuperClusterCollection_ = ps.getParameter<std::string>("correctedIslandSuperClusterCollection");
  correctedIslandSuperClusterProducer_   = ps.getParameter<std::string>("correctedIslandSuperClusterProducer");

  hybridSuperClusterCollection_ = ps.getParameter<std::string>("hybridSuperClusterCollection");
  hybridSuperClusterProducer_   = ps.getParameter<std::string>("hybridSuperClusterProducer");

  correctedHybridSuperClusterCollection_ = ps.getParameter<std::string>("correctedHybridSuperClusterCollection");
  correctedHybridSuperClusterProducer_   = ps.getParameter<std::string>("correctedHybridSuperClusterProducer");

  outputFile_   = ps.getParameter<std::string>("outputFile");

}


//========================================================================
EgammaSimpleAnalyzer::~EgammaSimpleAnalyzer()
//========================================================================
{
}

//========================================================================
void
EgammaSimpleAnalyzer::beginJob(edm::EventSetup const&) {
//========================================================================

  int nbins = 200;
  h1_islandBCEnergy_ = TH1F("islandBCEnergy","Energy of basic clusters with island algo",nbins,0.,20.);
  h1_islandSCEnergy_ = TH1F("islandSCEnergy","Energy of super clusters with island algo",nbins,0.,20.);
  h1_corrIslandSCEnergy_ = TH1F("corrIslandSCEnergy","Corrected Energy of super clusters with island algo",nbins,0.,20.);

  h1_hybridSCEnergy_ = TH1F("hybridSCEnergy","Energy of super clusters with hybrid algo",nbins,0.,20.);
  h1_corrHybridSCEnergy_ = TH1F("corrHybridSCEnergy","Corrected Energy of super clusters with hybrid algo",nbins,0.,20.);

}


//========================================================================
void
EgammaSimpleAnalyzer::analyze( const edm::Event& evt, const edm::EventSetup& es ) {
//========================================================================

  using namespace edm; // needed for all fwk related classes

  // Get island basic clusters
  Handle<reco::BasicClusterCollection> pIslandBasicClusters;
  try {
    evt.getByLabel(islandBasicClusterProducer_, islandBasicClusterCollection_, pIslandBasicClusters);
  } catch ( cms::Exception& ex ) {
    edm::LogError("EgammaSimpleAnalyzer") << "Error! can't get collection with label " << islandBasicClusterCollection_.c_str() ;
  }
  const reco::BasicClusterCollection* islandBasicClusters = pIslandBasicClusters.product();

  // loop over the Basic clusters and fill the histogram
  for(reco::BasicClusterCollection::const_iterator aClus = islandBasicClusters->begin();
                                                    aClus != islandBasicClusters->end(); aClus++) {
    h1_islandBCEnergy_.Fill( aClus->energy() );
  }

  // Get island super clusters
  Handle<reco::SuperClusterCollection> pIslandSuperClusters;
  try {
    evt.getByLabel(islandSuperClusterProducer_, islandSuperClusterCollection_, pIslandSuperClusters);
  } catch ( cms::Exception& ex ) {
    edm::LogError("EgammaSimpleAnalyzer") << "Error! can't get collection with label " << islandSuperClusterCollection_.c_str() ;
  }
  const reco::SuperClusterCollection* islandSuperClusters = pIslandSuperClusters.product();

  // loop over the super clusters and fill the histogram
  for(reco::SuperClusterCollection::const_iterator aClus = islandSuperClusters->begin();
                                                    aClus != islandSuperClusters->end(); aClus++) {
    h1_islandSCEnergy_.Fill( aClus->energy() );
  }


  // Get island super clusters after energy correction
  Handle<reco::SuperClusterCollection> pCorrectedIslandSuperClusters;
  try {
    evt.getByLabel(correctedIslandSuperClusterProducer_, correctedIslandSuperClusterCollection_, pCorrectedIslandSuperClusters);
  } catch ( cms::Exception& ex ) {
    edm::LogError("EgammaSimpleAnalyzer") << "Error! can't get collection with label " << correctedIslandSuperClusterCollection_.c_str() ;
  }
  const reco::SuperClusterCollection* correctedIslandSuperClusters = pCorrectedIslandSuperClusters.product();

  // loop over the super clusters and fill the histogram
  for(reco::SuperClusterCollection::const_iterator aClus = correctedIslandSuperClusters->begin();
                                                           aClus != correctedIslandSuperClusters->end(); aClus++) {
    h1_corrIslandSCEnergy_.Fill( aClus->energy() );
  }



  // Get hybrid super clusters
  Handle<reco::SuperClusterCollection> pHybridSuperClusters;
  try {
    evt.getByLabel(hybridSuperClusterProducer_, hybridSuperClusterCollection_, pHybridSuperClusters);
  } catch ( cms::Exception& ex ) {
    edm::LogError("EgammaSimpleAnalyzer") << "Error! can't get collection with label " << hybridSuperClusterCollection_.c_str() ;
  }
  const reco::SuperClusterCollection* hybridSuperClusters = pHybridSuperClusters.product();

  // loop over the super clusters and fill the histogram
  for(reco::SuperClusterCollection::const_iterator aClus = hybridSuperClusters->begin();
                                                    aClus != hybridSuperClusters->end(); aClus++) {
    h1_hybridSCEnergy_.Fill( aClus->energy() );
  }


  // Get hybrid super clusters after energy correction
  Handle<reco::SuperClusterCollection> pCorrectedHybridSuperClusters;
  try {
    evt.getByLabel(correctedHybridSuperClusterProducer_, correctedHybridSuperClusterCollection_, pCorrectedHybridSuperClusters);
  } catch ( cms::Exception& ex ) {
    edm::LogError("EgammaSimpleAnalyzer") << "Error! can't get collection with label " << correctedHybridSuperClusterCollection_.c_str() ;
  }
  const reco::SuperClusterCollection* correctedHybridSuperClusters = pCorrectedHybridSuperClusters.product();

  // loop over the super clusters and fill the histogram
  for(reco::SuperClusterCollection::const_iterator aClus = correctedHybridSuperClusters->begin();
                                                           aClus != correctedHybridSuperClusters->end(); aClus++) {
    h1_corrHybridSCEnergy_.Fill( aClus->energy() );
  }




}

//========================================================================
void
EgammaSimpleAnalyzer::endJob() {
//========================================================================

  TFile f(outputFile_.c_str(),"RECREATE"); // open output file to store histograms

  h1_islandBCEnergy_.Write();
  h1_islandSCEnergy_.Write();
  h1_corrIslandSCEnergy_.Write();

  h1_hybridSCEnergy_.Write();
  h1_corrHybridSCEnergy_.Write();
}
