import FWCore.ParameterSet.Config as cms
muonFilter = cms.EDFilter("TestMuL1L2Filter",
    L2CandTag = cms.InputTag("standAloneMuons"),
    #L2CandTag = cms.InputTag("L2Muons"),
    PrimaryVertexTag = cms.InputTag("pixelVertices"),
    NavigationPSet = cms.PSet(
        ComponentName = cms.string('SimpleNavigationSchool')
    ),
    rphiRecHits = cms.InputTag("siStripMatchedRecHits","rphiRecHit"),
    TTRHBuilder = cms.string('WithoutRefit')
)
