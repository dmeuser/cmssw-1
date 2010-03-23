import FWCore.ParameterSet.Config as cms

process = cms.Process("dbtest")

import FWCore.Framework.test.cmsExceptionsFatalOption_cff
process.options = cms.untracked.PSet(
#  wantSummary = cms.untracked.bool(True),
  Rethrow = FWCore.Framework.test.cmsExceptionsFatalOption_cff.Rethrow
)

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(20)
)

process.source = cms.Source("EmptySource",
     numberEventsInRun = cms.untracked.uint32(10),
     firstRun = cms.untracked.uint32(124025),
     numberEventsInLuminosityBlock = cms.untracked.uint32(1),
     firstLuminosityBlock = cms.untracked.uint32(1)
)

process.DBService=cms.Service("DBService",
           authPath=cms.untracked.string('/afs/cern.ch/user/x/xiezhen')
)
process.lumiProducer=cms.EDProducer("LumiProducer",
           connect=cms.string('oracle://devdb10/cms_xiezhen_dev'),
           lumiversion=cms.untracked.string('0001') 
)
process.test = cms.EDAnalyzer("TestLumiProducer")
process.out = cms.OutputModule("PoolOutputModule",
           fileName=cms.untracked.string("testLumiProd.root")
)
process.p1 = cms.Path(process.lumiProducer * process.test)
process.e = cms.EndPath(process.out)
