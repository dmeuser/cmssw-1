#include "SimMuon/MCTruth/plugins/MuonAssociatorEDProducer.h"
#include "SimMuon/MCTruth/src/CSCTruthTest.h"
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_SEAL_MODULE ();
DEFINE_FWK_MODULE(MuonAssociatorEDProducer);
DEFINE_FWK_MODULE(CSCTruthTest);
