# -*- coding: utf-8 -*-
import FWCore.ParameterSet.Config as cms

  
from Configuration.Generator.PythiaUESettings_cfi import *

TauolaNoPolar = cms.PSet(
    UseTauolaPolarization = cms.bool(False)
)
TauolaPolar = cms.PSet(
   UseTauolaPolarization = cms.bool(True)
)


from TauAnalysis.MCEmbeddingTools.MCParticleReplacer_cfi import *
newSource.algorithm = "ZTauTau"
newSource.ZTauTau.TauolaOptions.InputCards.mdtau = cms.int32(0)
newSource.ZTauTau.minVisibleTransverseMomentum = cms.untracked.double(0)


source = cms.Source("PoolSource",
        skipEvents = cms.untracked.uint32(0),
        fileNames = cms.untracked.vstring('file:/tmp/fruboes/Zmumu/patLayer1_fromAOD_PF2PAT_full.root')
)

filterEmptyEv = cms.EDFilter("EmptyEventsFilter",
    minEvents = cms.untracked.int32(1),
    target =  cms.untracked.int32(1) 
)

adaptedMuonsFromWmunu = cms.EDProducer("PATMuonMETAdapter",
   patMuCands = cms.untracked.InputTag("patMuonsPFlow"),
   patMet = cms.untracked.InputTag("patMETsPFlow")
)


dimuonsGlobal = cms.EDProducer('ZmumuPFEmbedder',
    tracks = cms.InputTag("generalTracks"),
    selectedMuons = cms.InputTag("adaptedMuonsFromWmunu","zMusExtracted"),
    keepMuonTrack = cms.bool(False)
)

generator = newSource.clone()
generator.src = cms.InputTag("adaptedMuonsFromWmunu","zMusExtracted")

ProductionFilterSequence = cms.Sequence(adaptedMuonsFromWmunu*dimuonsGlobal*generator*filterEmptyEv)
