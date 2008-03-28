import FWCore.ParameterSet.Config as cms

PixelCPEInitialESProducer = cms.ESProducer("PixelCPEInitialESProducer",
    ComponentName = cms.string('PixelCPEInitial'),
    PixelErrorParametrization = cms.string('NOTcmsim'),
    Alpha2Order = cms.bool(True)
)


