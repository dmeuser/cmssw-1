import FWCore.ParameterSet.Config as cms

# $Id: L1Scalers_cfi.py,v 1.9 2010/03/31 22:20:35 wteo Exp $

l1s = cms.EDAnalyzer("L1Scalers",
                   l1GtData = cms.InputTag("l1GtUnpack","","HLT"),
                   dqmFolder = cms.untracked.string("L1T/L1Scalers_EvF"),
                   verbose = cms.untracked.bool(False),
                   firstFED = cms.untracked.uint32(0),
                   lastFED = cms.untracked.uint32(39),
                   fedRawData = cms.InputTag("source","", ""),
                   maskedChannels = cms.untracked.vint32(),
                   HFRecHitCollection = cms.InputTag("hfreco", "", ""),
		   denomIsTech = cms.untracked.bool(False),
		   denomBit = cms.untracked.uint32(124),
		   tfIsTech = cms.untracked.bool(True),
		   tfBit = cms.untracked.uint32(41),
		   algoMonitorBits = cms.untracked.vuint32(8,9,15,46,54,55,100,124),
		   techMonitorBits = cms.untracked.vuint32(0,9)
                   )
