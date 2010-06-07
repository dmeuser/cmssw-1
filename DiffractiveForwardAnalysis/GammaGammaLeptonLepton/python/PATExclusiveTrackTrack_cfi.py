import FWCore.ParameterSet.Config as cms

excltrktrkanalysis = cms.EDFilter("ExclusiveTrackTrack",
    outfilename = cms.untracked.string('tracktrack.pat.root'),
    CaloTowerLabel = cms.InputTag("towerMaker"),
    CastorTowerLabel = cms.InputTag("CastorTowerReco"),
    CastorRecHitsLabel = cms.InputTag("castorreco"),
    ZDCRecHitsLabel = cms.InputTag("zdcreco"),                                  
    RecoTrackLabel = cms.InputTag("generalTracks"),
    CaloTowerdR = cms.double(0.3),
    FillAllMCParticles = cms.bool(False)                                   
)



