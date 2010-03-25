import FWCore.ParameterSet.Config as cms

hcalDetDiagLaserMonitor=cms.EDAnalyzer("HcalDetDiagLaserMonitor",
                                   # base class stuff
                                   debug                  = cms.untracked.int32(0),
                                   online                 = cms.untracked.bool(False),
                                   AllowedCalibTypes      = cms.untracked.vint32(1,2,3,4,5),
                                   mergeRuns              = cms.untracked.bool(False),
                                   enableCleanup          = cms.untracked.bool(False),
                                   subSystemFolder        = cms.untracked.string("Hcal/"),
                                   TaskFolder             = cms.untracked.string("DetDiagLaserMonitor_Hcal/"),
                                   skipOutOfOrderLS       = cms.untracked.bool(True),
                                   NLumiBlocks            = cms.untracked.int32(4000),
                                   makeDiagnostics        = cms.untracked.bool(False),
                                   
                                   # DetDiag Laser Monitor-specific Info
                                   
                                   # Input collections
                                   digiLabel              = cms.untracked.InputTag("hcalDigis"),
                                   # reference dataset path + filename
                                   LaserReferenceData     = cms.untracked.string(""),
                                   # path to store datasets for current run
                                   OutputFilePath         = cms.untracked.string(""),
                                   # path to store HTML output
                                   BaseHTMLpath           = cms.untracked.string(""),
                                   enableHTML             = cms.untracked.bool(False),
                                   # thresholds
                                   LaserTimingThreshold   = cms.untracked.double(0.2),
                                   LaserEnergyThreshold   = cms.untracked.double(0.1)
                                   )
