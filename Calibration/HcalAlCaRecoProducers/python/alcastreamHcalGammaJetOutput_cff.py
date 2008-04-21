import FWCore.ParameterSet.Config as cms

# output block for alcastream HCAL Dijets
# output module 
#  module alcastreamHcalGammaJetOutput = PoolOutputModule
alcastreamHcalGammaJetOutput = cms.PSet(
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_GammaJetProd_*_*')
)

