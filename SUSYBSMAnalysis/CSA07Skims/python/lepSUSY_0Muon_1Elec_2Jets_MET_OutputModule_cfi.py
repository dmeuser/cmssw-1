import FWCore.ParameterSet.Config as cms

from Configuration.EventContent.EventContent_cff import *
from SUSYBSMAnalysis.CSA07Skims.lepSUSY_0Muon_1Elec_2Jets_MET_EventContent_cff import *
lepSUSY_0Muon_1Elec_2Jets_MET_OutputModule = cms.OutputModule("PoolOutputModule",
    lepSUSY_0Muon_1Elec_2Jets_MET_EventSelection,
    AODSIMEventContent,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('lepSUSY_0Muon_1Elec_2Jets_MET'),
        dataTier = cms.untracked.string('USER')
    ),
    fileName = cms.untracked.string('lepSUSY_0Muon_1Elec_2Jets_MET.root')
)


