import FWCore.ParameterSet.Config as cms

process = cms.Process("ProcessOne")
process.load("CondCore.DBCommon.CondDBCommon_cfi")

process.CondDBCommon.connect = 'sqlite_file:pop_test.db'

process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO')
    ),
    destinations = cms.untracked.vstring('cout')
)

process.source = cms.Source("EmptyIOVSource",
    firstValue = cms.uint64(1),
    lastValue = cms.uint64(1),
    timetype = cms.string('runnumber'),
    interval = cms.uint64(1)
)

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    process.CondDBCommon,
    logconnect = cms.untracked.string('sqlite_file:log.db'),
    timetype = cms.untracked.string('runnumber'),
    toPut = cms.VPSet(cms.PSet(
        record = cms.string('ParamEffEtaRcd'),
        tag = cms.string('Example_tag1')
    ),
        cms.PSet(
            record = cms.string('ParamEffPtRcd'),
            tag = cms.string('Example_tag1')
        ))
)

process.Test1 = cms.EDFilter("ExPopConEfficiency",
    SinceAppendMode = cms.bool(True),
    record = cms.string('ParamEffPtRcd'),
    Source = cms.PSet(
        params = cms.untracked.vdouble(0.1, 0.95, 1.0, 5.5),
        since = cms.untracked.int64(200),
        type = cms.untracked.string('Pt')
    ),
    loggingOn = cms.untracked.bool(True),
    IsDestDbCheckedInQueryLog = cms.untracked.bool(True)
)

process.Test2 = cms.EDFilter("ExPopConEfficiency",
    SinceAppendMode = cms.bool(True),
    record = cms.string('ParamEffEtaRcd'),
    Source = cms.PSet(
        params = cms.untracked.vdouble(0.85, 0.0, 0.9, 2.3),
        since = cms.untracked.int64(351),
        type = cms.untracked.string('Eta')
    ),
    loggingOn = cms.untracked.bool(True),
    IsDestDbCheckedInQueryLog = cms.untracked.bool(True)
)

process.Test3 = cms.EDFilter("ExPopConEfficiency",
    SinceAppendMode = cms.bool(True),
    record = cms.string('ParamEffEtaRcd'),
    Source = cms.PSet(
        params = cms.untracked.vdouble(0.92, 0.0, 0.8, 2.5),
        since = cms.untracked.int64(531),
        type = cms.untracked.string('Eta')
    ),
    loggingOn = cms.untracked.bool(True),
    IsDestDbCheckedInQueryLog = cms.untracked.bool(True)
)

process.Test4 = cms.EDFilter("ExPopConEfficiency",
    SinceAppendMode = cms.bool(True),
    record = cms.string('ParamEffPtRcd'),
    Source = cms.PSet(
        params = cms.untracked.vdouble(0.1, 0.95, 1.0, 9.5),
        since = cms.untracked.int64(701),
        type = cms.untracked.string('Pt')
    ),
    loggingOn = cms.untracked.bool(True),
    IsDestDbCheckedInQueryLog = cms.untracked.bool(True)
)

## process.p = cms.Path(process.Test1 +
##                      process.Test2 +
##                      process.Test3 +
##                      process.Test4
##                      )


process.p = cms.Path(process.TestN) 















