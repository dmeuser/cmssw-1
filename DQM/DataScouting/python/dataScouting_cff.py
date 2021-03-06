import FWCore.ParameterSet.Config as cms

from DQM.DataScouting.razorScouting_cff import *
from DQM.DataScouting.dijetScouting_cff import *
from DQM.DataScouting.alphaTScouting_cff import *
from DQM.DataScouting.threejetScouting_cff import *
from DQM.DataScouting.dijetpairsScouting_cff import *

# DQM Services
dqmInfoDataScouting = cms.EDAnalyzer("DQMEventInfo",
                                     subSystemFolder = cms.untracked.string('DataScouting')
                                     )

#this file contains the sequence for data scouting
dataScoutingDQMSequence = cms.Sequence(dqmInfoDataScouting*scoutingRazorDQMSequence*scoutingDiJetDQMSequence*scoutingAlphaTDQMSequence*scoutingThreeJetDQMSequence*scoutingDiJetPairsDQMSequence)
