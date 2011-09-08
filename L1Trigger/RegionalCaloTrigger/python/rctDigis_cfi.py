import FWCore.ParameterSet.Config as cms

rctDigis = cms.EDProducer("L1RCTProducer",
    hcalDigis = cms.VInputTag(cms.InputTag("hcalTriggerPrimitiveDigis")),
    useEcal = cms.bool(True),
    useHcal = cms.bool(True),
    ecalDigis = cms.VInputTag(cms.InputTag("ecalTriggerPrimitiveDigis")),
    BunchCrossings = cms.vint32(0),
    getFedsFromOmds = cms.bool(False),
#    getFedsFromOmds = cms.bool(True),
    queryDelayInLS = cms.uint32(10),
    connectionString = cms.string("oracle://cms_orcoff_prod/CMS_RUNINFO"),
    authpath = cms.string("/afs/cern.ch/cms/DB/conddb"),
    tableToRead = cms.string("RUNSESSION_PARAMETER")
)



