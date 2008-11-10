import FWCore.ParameterSet.Config as cms

# Hardware Monitor ###
from DQM.SiStripMonitorHardware.buffer_hack_cfi import *
HardwareMonitor.rootFile = ''
HardwareMonitor.buildAllHistograms = False

# Pedestal Monitor ###
from DQM.SiStripMonitorPedestals.SiStripMonitorPedestals_cfi import *
PedsMon.OutputMEsInRootFile = False
PedsMon.StripQualityLabel = ''
PedsMon.RunTypeFlag = 'CalculatedPlotsOnly'

# Condition DB Monitor ###
from DQM.SiStripMonitorSummary.SiStripMonitorCondData_cfi import *

# SiStripQuality Monitor####
from DQM.SiStripMonitorPedestals.SiStripMonitorQuality_cfi import *
QualityMon.StripQualityLabel = ''

# Digi Monitor #####
from DQM.SiStripMonitorDigi.SiStripMonitorDigi_cfi import *
SiStripMonitorDigi.SelectAllDetectors = True

# Cluster Monitor ####
from DQM.SiStripMonitorCluster.SiStripMonitorCluster_cfi import *
SiStripMonitorCluster.OutputMEsInRootFile = False
SiStripMonitorCluster.SelectAllDetectors = True
SiStripMonitorCluster.StripQualityLabel = ''

# On/Off Track Cluster Monitor ####
# Clone for Sim data
import DQM.SiStripMonitorTrack.SiStripMonitorTrack_cfi
SiStripMonitorTrackSim = DQM.SiStripMonitorTrack.SiStripMonitorTrack_cfi.SiStripMonitorTrack.clone()
SiStripMonitorTrackSim.TrackProducer = 'TrackRefitter'
SiStripMonitorTrackSim.TrackLabel    = ''
SiStripMonitorTrackSim.Cluster_src   = 'siStripClusters'
SiStripMonitorTrackSim.FolderName    = 'SiStrip/Tracks'
SiStripMonitorTrackSim.Mod_On        = True

# Clone for Real Data
import DQM.SiStripMonitorTrack.SiStripMonitorTrack_cfi
SiStripMonitorTrackReal = DQM.SiStripMonitorTrack.SiStripMonitorTrack_cfi.SiStripMonitorTrack.clone()
SiStripMonitorTrackReal.TrackProducer = 'ctfWithMaterialTracksP5'
SiStripMonitorTrackReal.TrackLabel    = ''
SiStripMonitorTrackReal.Cluster_src   = 'siStripClusters'
SiStripMonitorTrackReal.FolderName    = 'SiStrip/Tracks'
SiStripMonitorTrackReal.Mod_On        = True

# Clone for Real Data (Collision)
import DQM.SiStripMonitorTrack.SiStripMonitorTrack_cfi
SiStripMonitorTrackColl = DQM.SiStripMonitorTrack.SiStripMonitorTrack_cfi.SiStripMonitorTrack.clone()
SiStripMonitorTrackColl.TrackProducer = 'generalTracks'
SiStripMonitorTrackColl.TrackLabel    = ''
SiStripMonitorTrackColl.Cluster_src   = 'siStripClusters'
SiStripMonitorTrackColl.FolderName    = 'SiStrip/Tracks'
SiStripMonitorTrackColl.Mod_On        = True


# Residual Monitor ####
# Clone for Sim Data
import DQM.TrackerMonitorTrack.MonitorTrackResiduals_cfi
MonitorTrackResidualsSim = DQM.TrackerMonitorTrack.MonitorTrackResiduals_cfi.MonitorTrackResiduals.clone()
# Clone for Real Data
MonitorTrackResidualsReal = DQM.TrackerMonitorTrack.MonitorTrackResiduals_cfi.MonitorTrackResiduals.clone()
import DQM.TrackerMonitorTrack.MonitorTrackResiduals_cfi
MonitorTrackResidualsReal.Tracks              = 'ctfWithMaterialTracksP5'
MonitorTrackResidualsReal.trajectoryInput     = 'ctfWithMaterialTracksP5'
MonitorTrackResidualsReal.OutputMEsInRootFile = False
# Clone for Real Data
MonitorTrackResidualsColl = DQM.TrackerMonitorTrack.MonitorTrackResiduals_cfi.MonitorTrackResiduals.clone()
import DQM.TrackerMonitorTrack.MonitorTrackResiduals_cfi
MonitorTrackResidualsColl.Tracks              = 'generalTracks'
MonitorTrackResidualsColl.trajectoryInput     = 'generalTracks'
MonitorTrackResidualsColl.OutputMEsInRootFile = False


# Tracking Monitor ####
# Clone for Sim Data
import DQM.TrackingMonitor.TrackingMonitor_cfi
TrackMonSim = DQM.TrackingMonitor.TrackingMonitor_cfi.TrackMon.clone()
TrackMonSim.FolderName = 'SiStrip/Tracks'
# Clone for Real Data (Cosmic)
import DQM.TrackingMonitor.TrackerCosmicsTrackingMonitor_cfi
TrackMonReal = DQM.TrackingMonitor.TrackerCosmicsTrackingMonitor_cfi.TrackerCosmicTrackMon.clone()
TrackMonReal.TrackProducer = 'ctfWithMaterialTracksP5'
TrackMonReal.FolderName = 'SiStrip/Tracks'
TrackMonReal.AlgoName = 'CKFTk'
# Clone for Real Data (Collison)
import DQM.TrackingMonitor.TrackingMonitor_cfi
TrackMonColl = DQM.TrackingMonitor.TrackingMonitor_cfi.TrackMon.clone()
TrackMonColl.TrackProducer = 'generalTracks'
TrackMonColl.FolderName = 'SiStrip/Tracks'
TrackMonColl.AlgoName = 'CKFTk'

# Sequences
SiStripSourcesSimData = cms.Sequence(SiStripMonitorDigi*SiStripMonitorCluster*SiStripMonitorTrackSim*MonitorTrackResidualsSim*TrackMonSim)
SiStripSourcesRealData = cms.Sequence(CondDataMonitoring*QualityMon*SiStripMonitorDigi*SiStripMonitorCluster*SiStripMonitorTrackReal*MonitorTrackResidualsReal*TrackMonReal)
SiStripSourcesRealDataCollision = cms.Sequence(CondDataMonitoring*QualityMon*SiStripMonitorDigi*SiStripMonitorCluster*SiStripMonitorTrackColl*MonitorTrackResidualsColl*TrackMonColl)




