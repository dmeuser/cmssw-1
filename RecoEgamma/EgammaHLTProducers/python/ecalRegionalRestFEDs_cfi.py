import FWCore.ParameterSet.Config as cms

# -- needed for regional unpacking:
from L1TriggerConfig.L1GeometryProducers.l1CaloGeometry_cfi import *
from L1TriggerConfig.L1GeometryProducers.l1CaloGeomRecordSource_cff import *
ecalRegionalRestFEDs = cms.EDProducer("EcalListOfFEDSProducer",
    debug = cms.untracked.bool(False),
    Muon = cms.untracked.bool(False),
    EGamma = cms.untracked.bool(False),
    OutputLabel = cms.untracked.string(''),
    Jets = cms.untracked.bool(False)
)


