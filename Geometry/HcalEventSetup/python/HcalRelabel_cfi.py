import FWCore.ParameterSet.Config as cms

HcalReLabel = cms.PSet( 
    RelabelHits  = cms.untracked.bool(False),
    RelabelRules = cms.untracked.PSet(
    Eta1 = cms.untracked.vint32(1,2,2,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5),
    Eta17 = cms.untracked.vint32(1,1,1,2,2,2,2,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4)
    )
)
