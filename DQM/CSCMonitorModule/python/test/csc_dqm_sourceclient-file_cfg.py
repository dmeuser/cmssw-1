import FWCore.ParameterSet.Config as cms

process = cms.Process("CSCDQM")

#-------------------------------------------------
# DQM Module Configuration
#-------------------------------------------------

process.load("DQM.CSCMonitorModule.csc_dqm_sourceclient_cfi")
process.load("DQM.CSCMonitorModule.csc_daq_info_cfi")
process.load("DQM.CSCMonitorModule.csc_dcs_info_cfi")
process.load("DQM.CSCMonitorModule.csc_certification_info_cfi")

#-------------------------------------------------
# Offline DQM Module Configuration
#-------------------------------------------------

process.load("DQMOffline.Muon.CSCMonitor_cfi")
process.load("Configuration/StandardSequences/RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.csc2DRecHits.readBadChambers = cms.bool(False)

#----------------------------
# Event Source
#-----------------------------

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.source = cms.Source("PoolSource",
  fileNames  = cms.untracked.vstring(

    '/store/data/Commissioning10/Cosmics/RAW/v4/000/132/440/72DAEFC2-1A3C-DF11-A352-0030487A195C.root',
    '/store/data/Commissioning10/Cosmics/RAW/v4/000/131/884/DE3D970A-3437-DF11-A61A-000423D98EA8.root',
    '/store/data/Commissioning10/Cosmics/RAW/v4/000/131/884/D8FEA60B-3B37-DF11-8343-000423D98B5C.root',
    '/store/data/Commissioning10/Cosmics/RAW/v4/000/131/884/9823CFD2-2F37-DF11-8FC3-0030487C635A.root',
    '/store/data/Commissioning10/Cosmics/RAW/v4/000/131/884/56D6D791-2437-DF11-911C-000423D98B6C.root'

  ),
  #skipEvents = cms.untracked.uint32(1129)
)

#----------------------------
# DQM Environment
#-----------------------------

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")

#process.DQMStore.referenceFileName = '/home/dqmdevlocal/reference/csc_reference.root'
process.DQMStore.referenceFileName = '/afs/cern.ch/user/v/valdo/data/csc_reference.root'
#process.DQMStore.referenceFileName = '/nfshome0/valdo/CMSSW_2_1_0/src/DQM/CSCMonitorModule/data/csc_reference.root'

#----------------------------
# DQM Playback Environment
#-----------------------------

process.load("DQM.Integration.test.environment_cfi")
process.dqmEnv.subSystemFolder    = "CSC"

process.DQM.collectorPort = 19190
#process.DQM.collectorHost = 'cms-uflap03.dyndns.cern.ch'
process.DQM.collectorHost = 'localhost'
process.dqmSaver.convention = "Online"
process.dqmSaver.dirName = "/tmp/valdo"
process.dqmSaver.producer = "DQM"

#-----------------------------
# Magnetic Field
#-----------------------------

process.load("Configuration/StandardSequences/MagneticField_cff")

#-------------------------------------------------
# GEOMETRY
#-------------------------------------------------
process.load("Configuration.StandardSequences.Geometry_cff")

#-------------------------------------------------
# Global Tag
#-------------------------------------------------

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#from Configuration.StandardSequences.FrontierConditions_GlobalTag_cff import *
#process.GlobalTag.connect = "sqlite_file:/nfshome0/malgeri/public/globtag/CRZT210_V1H.db"
#process.GlobalTag.connect = "frontier://FrontierDev/CMS_COND_CSC"
#process.GlobalTag.connect ="frontier://(proxyurl=http://localhost:3128)(serverurl=http://frontier1.cms:8000/FrontierOnProd)(serverurl=http://frontier2.cms:8000/FrontierOnProd)(retrieve-ziplevel=0)/CMS_COND_21X_GLOBALTAG"
#process.GlobalTag.globaltag = "CRZT210_V1H::All"
#process.GlobalTag.globaltag = 'CRAFT_V3P::All'
#process.GlobalTag.globaltag = "CRAFT_30X::All"
#process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')
#process.GlobalTag.connect ="frontier://(proxyurl=http://localhost:3128)(serverurl=http://frontier1.cms:8000/FrontierOnProd)(serverurl=http://frontier2.cms:8000/FrontierOnProd)(retrieve-ziplevel=0)/CMS_COND_31X_GLOBALTAG"
#process.GlobalTag.globaltag = "CRAFT_V17H::All"
#process.GlobalTag.connect ="frontier://(proxyurl=http://localhost:3128)(serverurl=http://localhost:8000/FrontierOnProd)(serverurl=http://localhost:8000/FrontierOnProd)(retrieve-ziplevel=0)/CMS_COND_31X_GLOBALTAG"
#process.GlobalTag.globaltag = 'GR09_31X_V1H::All' 
process.GlobalTag.globaltag = 'GR10_P_V2::All' 
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')


#--------------------------
# Web Service
#--------------------------

process.ModuleWebRegistry = cms.Service("ModuleWebRegistry")
process.AdaptorConfig = cms.Service("AdaptorConfig")

#--------------------------
# Message Logger
#--------------------------

MessageLogger = cms.Service("MessageLogger",

# suppressInfo = cms.untracked.vstring('source'),
# suppressInfo = cms.untracked.vstring('*'),

  cout = cms.untracked.PSet(
    threshold = cms.untracked.string('DEBUG'),
    WARNING = cms.untracked.PSet(
      limit = cms.untracked.int32(0)
    ),
    noLineBreaks = cms.untracked.bool(False)
  ),

  detailedInfo = cms.untracked.PSet(
    threshold = cms.untracked.string('DEBUG')
  ),

  critical = cms.untracked.PSet(
    threshold = cms.untracked.string('ERROR')
  ),

  debugModules = cms.untracked.vstring('*'),

  destinations = cms.untracked.vstring(
    'detailedInfo', 
    'critical', 
    'cout'
  )

)

#--------------------------
# Sequences
#--------------------------

process.p = cms.Path(process.dqmCSCClient * process.cscDaqInfo * process.cscDcsInfo * process.cscCertificationInfo + process.dqmEnv + process.dqmSaver)
#process.p = cms.Path(process.muonCSCDigis * process.csc2DRecHits * process.cscSegments * process.cscMonitor * process.dqmCSCClient + process.dqmEnv + process.dqmSaver)


