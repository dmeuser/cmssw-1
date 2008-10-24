import FWCore.ParameterSet.Config as cms

#Analyzer taken from online dqm
from DQM.TrackingMonitor.MonitorTrackSTACosmicMuons_cff import *
from DQM.TrackingMonitor.MonitorTrackSTACosmicMuonsHLT_cff import *
from DQM.TrackingMonitor.MonitorTrackGLBCosmicMuons_cfi import *
from DQM.TrackingMonitor.MonitorTrackTKCosmicMuons_cfi import *
from DQMOffline.Muon.dtSegmTask_cfi import *
from DQMOffline.Muon.rpcSource_cfi import *
#dedicated analyzers for offline dqm
from RecoMuon.TrackingTools.MuonServiceProxy_cff import *
from DQMOffline.Muon.muonCosmicAnalyzer_cfi import *
from DQMOffline.Muon.CSCMonitor_cfi import *
import DQMOffline.Muon.muonAnalyzer_cfi
muonStandAloneCosmicAnalyzer = DQMOffline.Muon.muonAnalyzer_cfi.muonAnalyzer.clone()
#dedicated clients for offline dqm
from DQMOffline.Muon.muonQualityTests_cff import *
#analyzers from Validation subsystem
from Validation.MuonIdentification.muonIdVal_cff import *
muonIdVal.makeDQMPlots = True

muonTrackCosmicAnalyzers = cms.Sequence(standAloneCosmicMuonsMonitors*MonitorTrackTKCosmicMuons*MonitorTrackGLBCosmicMuons)
muonTrackCosmicAnalyzersHLT = cms.Sequence(MonitorTrackSTACosmicMuonsHLTDT*MonitorTrackSTACosmicMuonsHLTCSC)

muonCosmicMonitors = cms.Sequence(muonTrackCosmicAnalyzers*dtSegmentsMonitor*rpcSource*cscMonitor*muonCosmicAnalyzer*muonIdVal)
##muonCosmicMonitors = cms.Sequence(muonTrackCosmicAnalyzers*dtSegmentsMonitor*cscMonitor*muonCosmicAnalyzer)

muonCosmicMonitors_woCSC = cms.Sequence(cms.SequencePlaceholder("muonTrackAnalyzers")*dtSegmentsMonitor*rpcSource*muonCosmicMonitors)

muonStandAloneCosmicMonitors = cms.Sequence(MonitorTrackSTACosmicMuons*dtSegmentsMonitor*cscMonitor*rpcSource*muonStandAloneCosmicAnalyzer)

dqmInfoMuons = cms.EDFilter("DQMEventInfo",
                            subSystemFolder = cms.untracked.string('Muons')
                            )

muonCosmicMonitorsAndQualityTests = cms.Sequence(muonCosmicMonitors*muonQualityTests*dqmInfoMuons)

muonStandAloneCosmicAnalyzer.DoMuonRecoAnalysis = False


