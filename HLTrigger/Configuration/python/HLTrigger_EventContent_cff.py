import FWCore.ParameterSet.Config as cms

# EventContent for HLT related products.

# This file exports the following five EventContent blocks:
#   HLTriggerRAW  HLTriggerRECO  HLTriggerAOD (without DEBUG products)
#   HLTDebugRAW   HLTDebugFEVT                (with    DEBUG products)
#
# as these are used in Configuration/EventContent
#
HLTriggerRAW  = cms.PSet(
    outputCommands = cms.vstring( *(
        'drop *_hlt*_*_*',
        'keep *_hltL1GtObjectMap_*_*',
        'keep FEDRawDataCollection_rawDataCollector_*_*',
        'keep FEDRawDataCollection_source_*_*',
        'keep edmTriggerResults_*_*_*',
        'keep triggerTriggerEvent_*_*_*'
    ) )
)

HLTriggerRECO = cms.PSet(
    outputCommands = cms.vstring( *(
        'drop *_hlt*_*_*',
        'keep *_hltL1GtObjectMap_*_*',
        'keep edmTriggerResults_*_*_*',
        'keep triggerTriggerEvent_*_*_*'
    ) )
)

HLTriggerAOD  = cms.PSet(
    outputCommands = cms.vstring( *(
        'drop *_hlt*_*_*',
        'keep *_hltL1GtObjectMap_*_*',
        'keep edmTriggerResults_*_*_*',
        'keep triggerTriggerEvent_*_*_*'
    ) )
)

HLTDebugRAW   = cms.PSet(
    outputCommands = cms.vstring( *(
        'drop *_hlt*_*_*',
        'keep *_hltAlCaEtaRecHitsFilter_*_*',
        'keep *_hltAlCaPhiSymStream_*_*',
        'keep *_hltAlCaPi0RecHitsFilter_*_*',
        'keep *_hltAntiKT5CaloJets_*_*',
        'keep *_hltAntiKT5PFJetsForTaus_*_*',
        'keep *_hltAntiKT5PFJets_*_*',
        'keep *_hltBSoftMuonDiJet110Mu5L3FilterByDR_*_*',
        'keep *_hltBSoftMuonDiJet110Mu5SelL3BJetTagsByDR_*_*',
        'keep *_hltBSoftMuonDiJet20Mu5L3FilterByDR_*_*',
        'keep *_hltBSoftMuonDiJet20Mu5SelL3BJetTagsByDR_*_*',
        'keep *_hltBSoftMuonDiJet40Mu5L3FilterByDR_*_*',
        'keep *_hltBSoftMuonDiJet40Mu5SelL3BJetTagsByDR_*_*',
        'keep *_hltBSoftMuonDiJet70Mu5L3FilterByDR_*_*',
        'keep *_hltBSoftMuonDiJet70Mu5SelL3BJetTagsByDR_*_*',
        'keep *_hltBSoftMuonMu5L3_*_*',
        'keep *_hltCaloJetCorrectedRegional_*_*',
        'keep *_hltCaloJetCorrected_*_*',
        'keep *_hltCalojetCorrectedNoId_*_*',
        'keep *_hltConvPFTauTightIsoTrackPt20Isolation_*_*',
        'keep *_hltConvPFTauTightIsoTrackPt20_*_*',
        'keep *_hltConvPFTausTightIsoTrackFindingIsolation_*_*',
        'keep *_hltConvPFTausTightIsoTrackFinding_*_*',
        'keep *_hltConvPFTausTightIsoTrackPt5Isolation_*_*',
        'keep *_hltConvPFTausTightIsoTrackPt5_*_*',
        'keep *_hltConvPFTausTightIso_*_*',
        'keep *_hltConvPFTausTrackFindingLooseIsolation_*_*',
        'keep *_hltConvPFTausTrackFinding_*_*',
        'keep *_hltConvPFTaus_*_*',
        'keep *_hltCsc2DRecHits_*_*',
        'keep *_hltCscSegments_*_*',
        'keep *_hltDoublePFTauTightIso25Track5_*_*',
        'keep *_hltDoublePFTauTightIso25Track_*_*',
        'keep *_hltDoublePFTauTightIso40Track5_*_*',
        'keep *_hltDoublePFTauTightIso40Track_*_*',
        'keep *_hltDt4DSegments_*_*',
        'keep *_hltEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTDphiFilter_*_*',
        'keep *_hltEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter_*_*',
        'keep *_hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8TrackIsolFilter_*_*',
        'keep *_hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter_*_*',
        'keep *_hltFilterDoubleIsoPFTau25Trk5LeadTrack5IsolationL1HLTMatched_*_*',
        'keep *_hltFilterDoubleIsoPFTau40Trk5LeadTrack5IsolationL1HLTMatched_*_*',
        'keep *_hltFilterL2EtCutDoublePFIsoTau25Trk5_*_*',
        'keep *_hltFilterL2EtCutDoublePFIsoTau40Trk5_*_*',
        'keep *_hltFilterL2EtCutSingleIsoPFTau35Trk20MET45_*_*',
        'keep *_hltFilterSingleIsoPFTau35Trk20LeadTrackPt20_*_*',
        'keep *_hltFilterSingleIsoPFTau35Trk20MET60LeadTrack20IsolationL1HLTMatched_*_*',
        'keep *_hltHITIPTCorrectorHB_*_*',
        'keep *_hltHITIPTCorrectorHE_*_*',
        'keep *_hltIsolPixelTrackProdHB_*_*',
        'keep *_hltIsolPixelTrackProdHE_*_*',
        'keep *_hltL1GtObjectMap_*_*',
        'keep *_hltL1HLTDoubleIsoPFTau25Trk5JetsMatch_*_*',
        'keep *_hltL1HLTDoubleIsoPFTau40Trk5JetsMatch_*_*',
        'keep *_hltL1HLTSingleIsoPFTau35Trk20Met60JetsMatch_*_*',
        'keep *_hltL1IsoElectronTrackIsol_*_*',
        'keep *_hltL1NonIsoElectronTrackIsol_*_*',
        'keep *_hltL1extraParticles_*_*',
        'keep *_hltL1sDoubleIsoTau25Trk5_*_*',
        'keep *_hltL1sDoubleIsoTau40Trk5_*_*',
        'keep *_hltL1sL1SingleEG12_*_*',
        'keep *_hltL1sL1SingleMu10_*_*',
        'keep *_hltL1sSingleIsoTau35Trk20MET45_*_*',
        'keep *_hltL2MuonCandidatesNoVtx_*_*',
        'keep *_hltL2MuonCandidates_*_*',
        'keep *_hltL2MuonIsolations_*_*',
        'keep *_hltL2MuonSeeds_*_*',
        'keep *_hltL2Muons_*_*',
        'keep *_hltL2TauJets_*_*',
        'keep *_hltL3MuonCandidatesNoVtx_*_*',
        'keep *_hltL3MuonCandidates_*_*',
        'keep *_hltL3MuonIsolations_*_*',
        'keep *_hltL3MuonsIOHit_*_*',
        'keep *_hltL3MuonsLinksCombination_*_*',
        'keep *_hltL3MuonsOIHit_*_*',
        'keep *_hltL3MuonsOIState_*_*',
        'keep *_hltL3Muons_*_*',
        'keep *_hltL3TkFromL2OICombination_*_*',
        'keep *_hltL3TkTracksFromL2IOHit_*_*',
        'keep *_hltL3TkTracksFromL2OIHit_*_*',
        'keep *_hltL3TkTracksFromL2OIState_*_*',
        'keep *_hltL3TkTracksFromL2_*_*',
        'keep *_hltL3TrackCandidateFromL2IOHit_*_*',
        'keep *_hltL3TrackCandidateFromL2OIHit_*_*',
        'keep *_hltL3TrackCandidateFromL2OIState_*_*',
        'keep *_hltL3TrajSeedIOHit_*_*',
        'keep *_hltL3TrajSeedOIHit_*_*',
        'keep *_hltL3TrajSeedOIState_*_*',
        'keep *_hltL3TrajectorySeed_*_*',
        'keep *_hltMet_*_*',
        'keep *_hltMuTrackJpsiCtfTrackCands_*_*',
        'keep *_hltMuTrackJpsiPixelTrackCands_*_*',
        'keep *_hltMuonCSCDigis_*_*',
        'keep *_hltMuonCSCDigis_MuonCSCStripDigi_*',
        'keep *_hltMuonCSCDigis_MuonCSCWireDigi_*',
        'keep *_hltMuonDTDigis_*_*',
        'keep *_hltMuonRPCDigis_*_*',
        'keep *_hltOfflineBeamSpot_*_*',
        'keep *_hltOnlineBeamSpot_*_*',
        'keep *_hltOverlapFilterIsoEle15IsoPFTau15_*_*',
        'keep *_hltOverlapFilterIsoEle18LooseIsoPFTau20_*_*',
        'keep *_hltOverlapFilterIsoMu15IsoPFTau15_*_*',
        'keep *_hltPFTau15TrackLooseIso_*_*',
        'keep *_hltPFTau15Track_*_*',
        'keep *_hltPFTau15_*_*',
        'keep *_hltPFTau20TrackLooseIso_*_*',
        'keep *_hltPFTau20Track_*_*',
        'keep *_hltPFTau20_*_*',
        'keep *_hltPFTauJetTracksAssociator_*_*',
        'keep *_hltPFTauTagInfo_*_*',
        'keep *_hltPFTauTightIso20TrackTightIso_*_*',
        'keep *_hltPFTauTightIso20Track_*_*',
        'keep *_hltPFTauTightIso20_*_*',
        'keep *_hltPFTauTightIso35Track_*_*',
        'keep *_hltParticleFlowForTaus_*_*',
        'keep *_hltParticleFlow_*_*',
        'keep *_hltPixelMatchElectronsL1Iso_*_*',
        'keep *_hltPixelMatchElectronsL1NonIso_*_*',
        'keep *_hltRecoEcalSuperClusterActivityCandidate_*_*',
        'keep *_hltRpcRecHits_*_*',
        'keep *_hltSiStripRawToClustersFacility_*_*',
        'keep *_hltSingleMu15L3Filtered15_*_*',
        'keep *_hltSingleMuIsoL3IsoFiltered15_*_*',
        'keep *_hltTowerMakerForMuons_*_*',
        'keep FEDRawDataCollection_rawDataCollector_*_*',
        'keep FEDRawDataCollection_source_*_*',
        'keep L1GlobalTriggerReadoutRecord_*_*_*',
        'keep L1GlobalTriggerReadoutRecord_hltGtDigis_*_*',
        'keep L1MuGMTCands_hltGtDigis_*_*',
        'keep L1MuGMTReadoutCollection_hltGtDigis_*_*',
        'keep SiPixelClusteredmNewDetSetVector_hltSiPixelClusters_*_*',
        'keep TrackingRecHitsOwned_hltL3Muons_*_*',
        'keep edmTriggerResults_*_*_*',
        'keep recoCaloMETs_hltMet_*_*',
        'keep recoElectronSeeds_hltL1IsoStartUpElectronPixelSeeds_*_*',
        'keep recoElectronSeeds_hltL1NonIsoStartUpElectronPixelSeeds_*_*',
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsoHLTClusterShape_*_*',
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonEcalIsol_*_*',
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonHcalForHE_*_*',
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonHcalIsol_*_*',
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsoHLTClusterShape_*_*',
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonEcalIsol_*_*',
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonHcalForHE_*_*',
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonHcalIsol_*_*',
        'keep recoRecoEcalCandidates_hltL1IsoRecoEcalCandidate_*_*',
        'keep recoRecoEcalCandidates_hltL1NonIsoRecoEcalCandidate_*_*',
        'keep triggerTriggerEventWithRefs_*_*_*',
        'keep triggerTriggerEvent_*_*_*'
    ) )
)

HLTDebugFEVT  = cms.PSet(
    outputCommands = cms.vstring( *(
        'drop *_hlt*_*_*',
        'keep *_hltAlCaEtaRecHitsFilter_*_*',
        'keep *_hltAlCaPhiSymStream_*_*',
        'keep *_hltAlCaPi0RecHitsFilter_*_*',
        'keep *_hltAntiKT5CaloJets_*_*',
        'keep *_hltAntiKT5PFJetsForTaus_*_*',
        'keep *_hltAntiKT5PFJets_*_*',
        'keep *_hltBSoftMuonDiJet110Mu5L3FilterByDR_*_*',
        'keep *_hltBSoftMuonDiJet110Mu5SelL3BJetTagsByDR_*_*',
        'keep *_hltBSoftMuonDiJet20Mu5L3FilterByDR_*_*',
        'keep *_hltBSoftMuonDiJet20Mu5SelL3BJetTagsByDR_*_*',
        'keep *_hltBSoftMuonDiJet40Mu5L3FilterByDR_*_*',
        'keep *_hltBSoftMuonDiJet40Mu5SelL3BJetTagsByDR_*_*',
        'keep *_hltBSoftMuonDiJet70Mu5L3FilterByDR_*_*',
        'keep *_hltBSoftMuonDiJet70Mu5SelL3BJetTagsByDR_*_*',
        'keep *_hltBSoftMuonMu5L3_*_*',
        'keep *_hltCaloJetCorrectedRegional_*_*',
        'keep *_hltCaloJetCorrected_*_*',
        'keep *_hltCalojetCorrectedNoId_*_*',
        'keep *_hltConvPFTauTightIsoTrackPt20Isolation_*_*',
        'keep *_hltConvPFTauTightIsoTrackPt20_*_*',
        'keep *_hltConvPFTausTightIsoTrackFindingIsolation_*_*',
        'keep *_hltConvPFTausTightIsoTrackFinding_*_*',
        'keep *_hltConvPFTausTightIsoTrackPt5Isolation_*_*',
        'keep *_hltConvPFTausTightIsoTrackPt5_*_*',
        'keep *_hltConvPFTausTightIso_*_*',
        'keep *_hltConvPFTausTrackFindingLooseIsolation_*_*',
        'keep *_hltConvPFTausTrackFinding_*_*',
        'keep *_hltConvPFTaus_*_*',
        'keep *_hltCsc2DRecHits_*_*',
        'keep *_hltCscSegments_*_*',
        'keep *_hltDoublePFTauTightIso25Track5_*_*',
        'keep *_hltDoublePFTauTightIso25Track_*_*',
        'keep *_hltDoublePFTauTightIso40Track5_*_*',
        'keep *_hltDoublePFTauTightIso40Track_*_*',
        'keep *_hltDt4DSegments_*_*',
        'keep *_hltEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTDphiFilter_*_*',
        'keep *_hltEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter_*_*',
        'keep *_hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8TrackIsolFilter_*_*',
        'keep *_hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter_*_*',
        'keep *_hltFilterDoubleIsoPFTau25Trk5LeadTrack5IsolationL1HLTMatched_*_*',
        'keep *_hltFilterDoubleIsoPFTau40Trk5LeadTrack5IsolationL1HLTMatched_*_*',
        'keep *_hltFilterL2EtCutDoublePFIsoTau25Trk5_*_*',
        'keep *_hltFilterL2EtCutDoublePFIsoTau40Trk5_*_*',
        'keep *_hltFilterL2EtCutSingleIsoPFTau35Trk20MET45_*_*',
        'keep *_hltFilterSingleIsoPFTau35Trk20LeadTrackPt20_*_*',
        'keep *_hltFilterSingleIsoPFTau35Trk20MET60LeadTrack20IsolationL1HLTMatched_*_*',
        'keep *_hltHITIPTCorrectorHB_*_*',
        'keep *_hltHITIPTCorrectorHE_*_*',
        'keep *_hltIsolPixelTrackProdHB_*_*',
        'keep *_hltIsolPixelTrackProdHE_*_*',
        'keep *_hltL1GtObjectMap_*_*',
        'keep *_hltL1HLTDoubleIsoPFTau25Trk5JetsMatch_*_*',
        'keep *_hltL1HLTDoubleIsoPFTau40Trk5JetsMatch_*_*',
        'keep *_hltL1HLTSingleIsoPFTau35Trk20Met60JetsMatch_*_*',
        'keep *_hltL1IsoElectronTrackIsol_*_*',
        'keep *_hltL1NonIsoElectronTrackIsol_*_*',
        'keep *_hltL1extraParticles_*_*',
        'keep *_hltL1sDoubleIsoTau25Trk5_*_*',
        'keep *_hltL1sDoubleIsoTau40Trk5_*_*',
        'keep *_hltL1sL1SingleEG12_*_*',
        'keep *_hltL1sL1SingleMu10_*_*',
        'keep *_hltL1sSingleIsoTau35Trk20MET45_*_*',
        'keep *_hltL2MuonCandidatesNoVtx_*_*',
        'keep *_hltL2MuonCandidates_*_*',
        'keep *_hltL2MuonIsolations_*_*',
        'keep *_hltL2MuonSeeds_*_*',
        'keep *_hltL2Muons_*_*',
        'keep *_hltL2TauJets_*_*',
        'keep *_hltL3MuonCandidatesNoVtx_*_*',
        'keep *_hltL3MuonCandidates_*_*',
        'keep *_hltL3MuonIsolations_*_*',
        'keep *_hltL3MuonsIOHit_*_*',
        'keep *_hltL3MuonsLinksCombination_*_*',
        'keep *_hltL3MuonsOIHit_*_*',
        'keep *_hltL3MuonsOIState_*_*',
        'keep *_hltL3Muons_*_*',
        'keep *_hltL3TkFromL2OICombination_*_*',
        'keep *_hltL3TkTracksFromL2IOHit_*_*',
        'keep *_hltL3TkTracksFromL2OIHit_*_*',
        'keep *_hltL3TkTracksFromL2OIState_*_*',
        'keep *_hltL3TkTracksFromL2_*_*',
        'keep *_hltL3TrackCandidateFromL2IOHit_*_*',
        'keep *_hltL3TrackCandidateFromL2OIHit_*_*',
        'keep *_hltL3TrackCandidateFromL2OIState_*_*',
        'keep *_hltL3TrajSeedIOHit_*_*',
        'keep *_hltL3TrajSeedOIHit_*_*',
        'keep *_hltL3TrajSeedOIState_*_*',
        'keep *_hltL3TrajectorySeed_*_*',
        'keep *_hltMet_*_*',
        'keep *_hltMuTrackJpsiCtfTrackCands_*_*',
        'keep *_hltMuTrackJpsiPixelTrackCands_*_*',
        'keep *_hltMuonCSCDigis_*_*',
        'keep *_hltMuonCSCDigis_MuonCSCStripDigi_*',
        'keep *_hltMuonCSCDigis_MuonCSCWireDigi_*',
        'keep *_hltMuonDTDigis_*_*',
        'keep *_hltMuonRPCDigis_*_*',
        'keep *_hltOfflineBeamSpot_*_*',
        'keep *_hltOnlineBeamSpot_*_*',
        'keep *_hltOverlapFilterIsoEle15IsoPFTau15_*_*',
        'keep *_hltOverlapFilterIsoEle18LooseIsoPFTau20_*_*',
        'keep *_hltOverlapFilterIsoMu15IsoPFTau15_*_*',
        'keep *_hltPFTau15TrackLooseIso_*_*',
        'keep *_hltPFTau15Track_*_*',
        'keep *_hltPFTau15_*_*',
        'keep *_hltPFTau20TrackLooseIso_*_*',
        'keep *_hltPFTau20Track_*_*',
        'keep *_hltPFTau20_*_*',
        'keep *_hltPFTauJetTracksAssociator_*_*',
        'keep *_hltPFTauTagInfo_*_*',
        'keep *_hltPFTauTightIso20TrackTightIso_*_*',
        'keep *_hltPFTauTightIso20Track_*_*',
        'keep *_hltPFTauTightIso20_*_*',
        'keep *_hltPFTauTightIso35Track_*_*',
        'keep *_hltParticleFlowForTaus_*_*',
        'keep *_hltParticleFlow_*_*',
        'keep *_hltPixelMatchElectronsL1Iso_*_*',
        'keep *_hltPixelMatchElectronsL1NonIso_*_*',
        'keep *_hltRecoEcalSuperClusterActivityCandidate_*_*',
        'keep *_hltRpcRecHits_*_*',
        'keep *_hltSiStripRawToClustersFacility_*_*',
        'keep *_hltSingleMu15L3Filtered15_*_*',
        'keep *_hltSingleMuIsoL3IsoFiltered15_*_*',
        'keep *_hltTowerMakerForMuons_*_*',
        'keep FEDRawDataCollection_rawDataCollector_*_*',
        'keep FEDRawDataCollection_source_*_*',
        'keep L1GlobalTriggerReadoutRecord_*_*_*',
        'keep L1GlobalTriggerReadoutRecord_hltGtDigis_*_*',
        'keep L1MuGMTCands_hltGtDigis_*_*',
        'keep L1MuGMTReadoutCollection_hltGtDigis_*_*',
        'keep SiPixelClusteredmNewDetSetVector_hltSiPixelClusters_*_*',
        'keep TrackingRecHitsOwned_hltL3Muons_*_*',
        'keep edmTriggerResults_*_*_*',
        'keep recoCaloMETs_hltMet_*_*',
        'keep recoElectronSeeds_hltL1IsoStartUpElectronPixelSeeds_*_*',
        'keep recoElectronSeeds_hltL1NonIsoStartUpElectronPixelSeeds_*_*',
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsoHLTClusterShape_*_*',
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonEcalIsol_*_*',
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonHcalForHE_*_*',
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonHcalIsol_*_*',
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsoHLTClusterShape_*_*',
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonEcalIsol_*_*',
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonHcalForHE_*_*',
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonHcalIsol_*_*',
        'keep recoRecoEcalCandidates_hltL1IsoRecoEcalCandidate_*_*',
        'keep recoRecoEcalCandidates_hltL1NonIsoRecoEcalCandidate_*_*',
        'keep triggerTriggerEventWithRefs_*_*_*',
        'keep triggerTriggerEvent_*_*_*'
    ) )
)

