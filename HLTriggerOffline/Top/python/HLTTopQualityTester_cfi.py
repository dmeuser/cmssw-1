import FWCore.ParameterSet.Config as cms

hltTopQualityTester = cms.EDAnalyzer("QualityTester",
    qtList = cms.untracked.FileInPath(
        'HLTriggerOffline/Top/data/HLTTopQualityTest.xml'
    ),
    reportThreshold         = cms.untracked.string('black'),
    prescaleFactor          = cms.untracked.int32(1),
    getQualityTestsFromFile = cms.untracked.bool(True),
    qtestOnEndJob           = cms.untracked.bool(True),
    qtestOnEndLumi          = cms.untracked.bool(False),
    testInEventloop         = cms.untracked.bool(False),
    verboseQT               = cms.untracked.bool(False)
)
