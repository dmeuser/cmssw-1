import FWCore.ParameterSet.Config as cms

from DQM.EcalPreshowerMonitorModule.ESRawDataTask_cfi import *
from DQM.EcalPreshowerMonitorModule.ESIntegrityTask_cfi import *
#from DQM.EcalPreshowerMonitorModule.ESFEDIntegrityTask_cfi import *
#from DQM.EcalPreshowerMonitorModule.ESPedestalTask_cfi import *
from DQM.EcalPreshowerMonitorModule.ESOccupancyTask_cfi import *
from DQM.EcalPreshowerMonitorModule.ESDcsInfoTask_cfi import *
from DQM.EcalPreshowerMonitorModule.ESDataCertificationTask_cfi import *
from DQM.EcalPreshowerMonitorModule.ESDaqInfoTask_cfi import *

dqmInfoES = cms.EDFilter("DQMEventInfo",
                         subSystemFolder = cms.untracked.string('EcalPreshower')
                         )

#ecalPreshowerDefaultTasksSequence = cms.Sequence(ecalPreshowerOccupancyTask*ecalPreshowerPedestalTask)
ecalPreshowerDefaultTasksSequence = cms.Sequence(ecalPreshowerRawDataTask*ecalPreshowerIntegrityTask*ecalPreshowerOccupancyTask)

ecalPreshowerCertificationSequence = cms.Sequence(ecalPreshowerDcsInfoTask*ecalPreshowerDataCertificationTask*ecalPreshowerDaqInfoTask)



