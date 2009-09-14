import FWCore.ParameterSet.Config as cms

rechivalidation = cms.EDFilter("DTRecHitQuality",
    doStep2 = cms.untracked.bool(False),
    # Switches for analysis at various steps
    doStep1 = cms.untracked.bool(True),
    # Lable to retrieve RecHits from the event
    recHitLabel = cms.untracked.InputTag('dt1DRecHits'),
    doStep3 = cms.untracked.bool(True),
    simHitLabel = cms.untracked.InputTag('g4SimHits',"MuonDTHits"),
    segment2DLabel = cms.untracked.InputTag('dt2DSegments'),
    debug = cms.untracked.bool(False),
    segment4DLabel = cms.untracked.InputTag('dt4DSegments'),
    doall = cms.untracked.bool(True)

)

seg2dvalidation = cms.EDFilter("DTSegment2DQuality",
    sigmaResPos = cms.double(0.013),
    simHitLabel = cms.untracked.InputTag('g4SimHits',"MuonDTHits"),
    segment2DLabel = cms.untracked.InputTag('dt2DSegments'),
    debug = cms.untracked.bool(False),
    sigmaResAngle = cms.double(0.008)
)

seg2dsuperphivalidation = cms.EDFilter("DTSegment2DSLPhiQuality",
    sigmaResPos = cms.double(0.013),
    simHitLabel = cms.untracked.InputTag('g4SimHits',"MuonDTHits"),
    sigmaResAngle = cms.double(0.008),
    debug = cms.untracked.bool(False),
    segment4DLabel = cms.untracked.InputTag('dt4DSegments'),
    doall = cms.untracked.bool(True)
)

seg4dvalidation = cms.EDFilter("DTSegment4DQuality",
    #resolution on angle
    sigmaResAlpha = cms.double(0.001),
    sigmaResBeta = cms.double(0.007),
    simHitLabel = cms.untracked.InputTag('g4SimHits',"MuonDTHits"),
    rootFileName = cms.untracked.string(''),
    debug = cms.untracked.bool(False),
    #resolution on position
    sigmaResX = cms.double(0.01),
    sigmaResY = cms.double(0.05),
    segment4DLabel = cms.untracked.InputTag('dt4DSegments'),
    doall = cms.untracked.bool(True)
)

dtLocalRecoValidation = cms.Sequence(rechivalidation*seg2dvalidation*seg2dsuperphivalidation*seg4dvalidation)
dtLocalRecoValidation_no2D = cms.Sequence(rechivalidation*seg2dsuperphivalidation*seg4dvalidation)

