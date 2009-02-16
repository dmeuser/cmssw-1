import FWCore.ParameterSet.Config as cms

process = cms.Process("CALIB")
process.load("Geometry.MuonCommonData.muonIdealGeometryXML_cfi")

process.load("Geometry.DTGeometry.dtGeometry_cfi")
process.DTGeometryESModule.applyAlignment = False

process.load("Geometry.MuonNumbering.muonNumberingInitialization_cfi")

process.load("DQMServices.Core.DQM_cfg")

from CalibTracker.Configuration.Common.PoolDBESSource_cfi import poolDBESSource
poolDBESSource.connect = "frontier://FrontierDev/CMS_COND_ALIGNMENT"
poolDBESSource.toGet = cms.VPSet(cms.PSet(
        record = cms.string('GlobalPositionRcd'),
        tag = cms.string('IdealGeometry')
    )) 
process.glbPositionSource = poolDBESSource

process.source = cms.Source("EmptySource",
    numberEventsInRun = cms.untracked.uint32(1),
    firstRun = cms.untracked.uint32(70680)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.tzeroRef = cms.ESSource("PoolDBESSource",
    DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(0),
        authenticationPath = cms.untracked.string('/afs/cern.ch/cms/DB/conddb')
    ),
    timetype = cms.string('runnumber'),
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('DTT0Rcd'),
        tag = cms.string('t0_CRAFT_V01_offline'),
        label = cms.untracked.string('tzeroRef')
    ), 
        cms.PSet(
            record = cms.string('DTT0Rcd'),
            tag = cms.string('t0'),
            connect = cms.untracked.string('sqlite_file:/tmp/giorgia/CMSSW_3_0_0_pre6/src/DQMOffline/CalibMuon/test/t0_70195.db'),
            label = cms.untracked.string('tzeroToValidate')
        )),
    connect = cms.string('frontier://FrontierProd/CMS_COND_30X_DT'),
    siteLocalConfig = cms.untracked.bool(False)
)

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('dtT0Analyzer'),
    cout = cms.untracked.PSet(
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        t0dbValidation = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000)
        ),
        noLineBreaks = cms.untracked.bool(True),
        threshold = cms.untracked.string('DEBUG'),
        FwkJob = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        DEBUG = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        )
    ),
    categories = cms.untracked.vstring('t0dbValidation'),
    destinations = cms.untracked.vstring('cout')
)

process.dtT0Analyzer = cms.EDFilter("DTt0DBValidation",
    labelDBRef = cms.untracked.string('tzeroRef'),
    t0TestName = cms.untracked.string('t0DifferenceInRange'),
    OutputFileName = cms.untracked.string('MuonTestMonitoring.root'),
    labelDB = cms.untracked.string('tzeroToValidate'),
    minT0Limit = cms.untracked.int32(10),
    maxT0Limit = cms.untracked.int32(20)
)

process.qTester = cms.EDFilter("QualityTester",
    prescaleFactor = cms.untracked.int32(1),
    qtList = cms.untracked.FileInPath('DQMOffline/CalibMuon/data/QualityTests.xml')
)

process.p = cms.Path(process.dtT0Analyzer*process.qTester)
process.DQM.collectorHost = ''


