import FWCore.ParameterSet.Config as cms

from DQM.SiStripMonitorTrack.SiStripMonitorTrack_cfi import *
SiStripMonitorTrack.TrackProducer = 'TrackRefitter'
SiStripMonitorTrack.TrackLabel    = ''
SiStripMonitorTrack.OutputMEsInRootFile = True
SiStripMonitorTrack.OutputFileName = '/tmp/sistripmonitortrack_prova.root'
SiStripMonitorTrack.Cluster_src = 'siStripClusters'
SiStripMonitorTrack.Mod_On      = False
SiStripMonitorTrack.OffHisto_On = True
SiStripMonitorTrack.Trend_On    = False

#TrackRefitter With Material
from RecoTracker.TrackProducer.RefitterWithMaterial_cff import *
from RecoLocalTracker.SiPixelRecHits.PixelCPEESProducers_cff import *

#-----------------------
#  Reconstruction Modules
#-----------------------
from RecoLocalTracker.SiStripZeroSuppression.SiStripZeroSuppression_cfi import *
from EventFilter.SiStripRawToDigi.SiStripDigis_cfi import *

DQMSiStripMonitorTrack_Sim = cms.Sequence( siStripDigis
                                           *
                                           siStripZeroSuppression
                                           *
                                           TrackRefitter
                                           *
                                           SiStripMonitorTrack
                                           )


# reconstruction sequence for Cosmics
from Configuration.StandardSequences.ReconstructionCosmics_cff import *

DQMSiStripMonitorTrack_CosmicSim = cms.Sequence( trackerCosmics
                                                 *
                                                 TrackRefitter
                                                 *
                                                 SiStripMonitorTrack
                                                 )
