import FWCore.ParameterSet.Config as cms

# RPC DQM
from DQM.HLTEvF.rpcSourceHLT_cfi import *
from DQM.HLTEvF.rpcClientHLT_cfi import *

hltHighLevel = cms.EDFilter("HLTHighLevel",
    TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
    HLTPaths = cms.vstring('HLT_L1Mu[^_]*$',
    			'HLT_L2Mu[^_]*&', 
    			'HLT_Mu[^_]*&', 
    			'HLT_IsoMu[^_]*&', 
    			'HLT_DoubleMu[^_]*&', 
			),
    #HLTPaths = cms.vstring('HLT_L1MuOpen','HLT_L1Mu', 'HLT_L1Mu20',
    #			'HLT_L2Mu9','HLT_L2Mu11',
#			'HLT_Mu3', 'HLT_Mu5', 'HLT_Mu9'),
    eventSetupPathsKey = cms.string(''), # not empty => use read paths from AlCaRecoTriggerBitsRcd via this key
    andOr = cms.bool(True), # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true
    throw = cms.bool(True)    # throw exception on unknown path names
)

hltLocalRecoDT = cms.Path(hltHighLevel*rpcClientHLT)

