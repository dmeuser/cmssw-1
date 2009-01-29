import FWCore.ParameterSet.Config as cms



from DQMOffline.EGamma.photonAnalyzer_cfi import *
from DQMOffline.EGamma.piZeroAnalyzer_cfi import *
from DQMOffline.EGamma.electronAnalyzerSequence_cff import *

photonAnalysis.OutputMEsInRootFile = cms.bool(False)
photonAnalysis.Verbosity = cms.untracked.int32(0)
photonAnalysis.standAlone = cms.bool(False)

piZeroAnalysis.OutputMEsInRootFile = cms.bool(False)
piZeroAnalysis.Verbosity = cms.untracked.int32(0)
piZeroAnalysis.standAlone = cms.bool(False)



gsfElectronAnalysis.OutputMEsInRootFile = cms.bool(False)
gsfElectronAnalysis.Verbosity = cms.untracked.int32(0)

egammaDQMOffline = cms.Sequence(photonAnalysis*piZeroAnalysis*electronAnalyzerSequence)
