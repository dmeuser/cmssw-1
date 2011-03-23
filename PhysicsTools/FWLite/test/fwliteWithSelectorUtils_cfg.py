import FWCore.ParameterSet.Config as cms

process = cms.PSet()

process.fwliteInput = cms.PSet(
    fileNames   = cms.vstring('rfio:///castor/cern.ch/cms/store/relval/CMSSW_4_1_3/RelValTTbar/GEN-SIM-RECO/START311_V2-v1/0037/648B6AA5-C751-E011-8208-001A928116C6.root'),                                ## mandatory
    maxEvents   = cms.int32(-1),                             ## optional
    outputEvery = cms.uint32(10),                            ## optional
)

process.fwliteOutput = cms.PSet(
    fileName  = cms.string('analyzeFWLiteHistograms.root'),  ## mandatory
)

process.selection = cms.PSet(
        muonSrc      = cms.InputTag('muons'),
        metSrc       = cms.InputTag('metJESCorAK5CaloJetMuons'),
        muonPtMin    = cms.double(20.0),
        metMin       = cms.double(20.0),
       #cutsToIgnore = cms.vstring('MET')
)

