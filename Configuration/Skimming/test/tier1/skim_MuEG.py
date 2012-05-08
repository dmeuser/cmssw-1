# Auto generated configuration file
# using: 
# Revision: 1.372.2.1 
# Source: /local/reps/CMSSW.admin/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: skims -s SKIM:@MuEG --data --no_exec --dbs find file,file.parent where dataset=/MuEG/Run2012A-PromptReco-v1/RECO and run=191277 -n 100 --conditions auto:com10 --python_filename=skim_MuEG.py
import FWCore.ParameterSet.Config as cms

process = cms.Process('SKIM')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.Skims_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring('/store/data/Run2012A/MuEG/RAW/v1/000/191/277/BAA75812-1E87-E111-A51C-5404A63886EC.root', 
        '/store/data/Run2012A/MuEG/RAW/v1/000/191/277/8245B8F9-2A87-E111-AEF6-BCAEC5364C93.root', 
        '/store/data/Run2012A/MuEG/RAW/v1/000/191/277/FE1D68D5-3687-E111-8DB9-BCAEC5364CFB.root', 
        '/store/data/Run2012A/MuEG/RAW/v1/000/191/277/DCFADE0A-3E87-E111-B2EE-003048F024DA.root', 
        '/store/data/Run2012A/MuEG/RAW/v1/000/191/277/124126DD-2F87-E111-8959-5404A638868F.root', 
        '/store/data/Run2012A/MuEG/RAW/v1/000/191/277/9E165C85-2287-E111-896B-003048F1183E.root', 
        '/store/data/Run2012A/MuEG/RAW/v1/000/191/277/C89A332C-1987-E111-97E6-001D09F29533.root', 
        '/store/data/Run2012A/MuEG/RAW/v1/000/191/277/E08A3BAA-2687-E111-A892-0030486780B8.root', 
        '/store/data/Run2012A/MuEG/RAW/v1/000/191/277/C0106D98-4687-E111-B6B8-003048F11DE2.root'),
    fileNames = cms.untracked.vstring('/store/data/Run2012A/MuEG/RECO/PromptReco-v1/000/191/277/F6686E65-0189-E111-A988-BCAEC518FF8E.root', 
        '/store/data/Run2012A/MuEG/RECO/PromptReco-v1/000/191/277/D44E5334-F888-E111-A7C5-0019B9F4A1D7.root', 
        '/store/data/Run2012A/MuEG/RECO/PromptReco-v1/000/191/277/D22256DA-EB88-E111-B369-BCAEC53296F8.root', 
        '/store/data/Run2012A/MuEG/RECO/PromptReco-v1/000/191/277/9C493516-F488-E111-BB6B-0025901D5D90.root', 
        '/store/data/Run2012A/MuEG/RECO/PromptReco-v1/000/191/277/98AF6869-0189-E111-BB42-E0CB4E55367F.root', 
        '/store/data/Run2012A/MuEG/RECO/PromptReco-v1/000/191/277/780EAA52-0689-E111-879A-003048D37524.root', 
        '/store/data/Run2012A/MuEG/RECO/PromptReco-v1/000/191/277/4A965682-E988-E111-A705-0025B32035A2.root', 
        '/store/data/Run2012A/MuEG/RECO/PromptReco-v1/000/191/277/3A5167FB-FF88-E111-9420-BCAEC5364CFB.root', 
        '/store/data/Run2012A/MuEG/RECO/PromptReco-v1/000/191/277/303BDF08-E688-E111-842D-003048D2BC42.root')
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.372.2.1 $'),
    annotation = cms.untracked.string('skims nevts:100'),
    name = cms.untracked.string('PyReleaseValidation')
)

# Output definition

# Additional output definition
process.SKIMStreamHZZ = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('HZZ4ePath', 
            'HZZ2e2mPath', 
            'HZZ2m2ePath', 
            'HZZ4mPath', 
            'HZZem2ePath', 
            'HZZem2mPath')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_castorreco_*_*', 
        'keep *_reducedHcalRecHits_*_*', 
        'keep *_selectDigi_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEB_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEE_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsES_*_*', 
        'keep recoSuperClusters_correctedHybridSuperClusters_*_*', 
        'keep recoCaloClusters_hybridSuperClusters_*_*', 
        'keep recoSuperClusters_hybridSuperClusters_uncleanOnlyHybridSuperClusters_*', 
        'keep recoCaloClusters_multi5x5SuperClusters_multi5x5EndcapBasicClusters_*', 
        'keep recoSuperClusters_correctedMulti5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusterShapes_multi5x5PreshowerClusterShape_*_*', 
        'keep recoTracks_GsfGlobalElectronTest_*_*', 
        'keep recoGsfTracks_electronGsfTracks_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTracks_rsWithMaterialTracks_*_*', 
        'keep recoTracks_conversionStepTracks_*_*', 
        'keep recoTracks_beamhaloTracks_*_*', 
        'keep recoTracks_regionalCosmicTracks_*_*', 
        'keep recoTracks_ctfPixelLess_*_*', 
        'keep *_dedxHarmonic2_*_*', 
        'keep *_dedxDiscrimASmi_*_*', 
        'keep *_trackExtrapolator_*_*', 
        'keep *_kt4CaloJets_*_*', 
        'keep *_kt6CaloJets_*_*', 
        'keep *_ak5CaloJets_*_*', 
        'keep *_ak7CaloJets_*_*', 
        'keep *_kt4PFJets_*_*', 
        'keep *_kt6PFJets_*_*', 
        'keep *_ak5PFJets_*_*', 
        'keep *_ak7PFJets_*_*', 
        'keep *_JetPlusTrackZSPCorJetAntiKt5_*_*', 
        'keep *_ak5TrackJets_*_*', 
        'keep recoRecoChargedRefCandidates_trackRefsForJets_*_*', 
        'keep *_caloTowers_*_*', 
        'keep *_towerMaker_*_*', 
        'keep *_CastorTowerReco_*_*', 
        'keep *_ak5JetTracksAssociatorAtVertex_*_*', 
        'keep *_ak7JetTracksAssociatorAtVertex_*_*', 
        'keep *_kt4JetExtender_*_*', 
        'keep *_ak5JetExtender_*_*', 
        'keep *_ak7JetExtender_*_*', 
        'keep *_ak5JetID_*_*', 
        'keep *_ak7JetID_*_*', 
        'keep *_kt4JetID_*_*', 
        'keep *_kt6JetID_*_*', 
        'keep *_ak7BasicJets_*_*', 
        'keep *_ak7CastorJetID_*_*', 
        'keep double_kt6CaloJetsCentral_*_*', 
        'keep double_kt6PFJetsCentralChargedPileUp_*_*', 
        'keep double_kt6PFJetsCentralNeutral_*_*', 
        'keep double_kt6PFJetsCentralNeutralTight_*_*', 
        'keep *_fixedGridRho*_*_*', 
        'keep recoCaloMETs_met_*_*', 
        'keep recoCaloMETs_metNoHF_*_*', 
        'keep recoCaloMETs_metHO_*_*', 
        'keep recoCaloMETs_corMetGlobalMuons_*_*', 
        'keep recoMETs_htMetAK5_*_*', 
        'keep recoMETs_htMetAK7_*_*', 
        'keep recoMETs_htMetIC5_*_*', 
        'keep recoMETs_htMetKT4_*_*', 
        'keep recoMETs_htMetKT6_*_*', 
        'keep recoMETs_tcMet_*_*', 
        'keep recoMETs_tcMetWithPFclusters_*_*', 
        'keep recoPFMETs_pfMet_*_*', 
        'keep recoMuonMETCorrectionDataedmValueMap_muonMETValueMapProducer_*_*', 
        'keep recoMuonMETCorrectionDataedmValueMap_muonTCMETValueMapProducer_*_*', 
        'drop recoHcalNoiseRBXs_*_*_*', 
        'keep HcalNoiseSummary_hcalnoise_*_*', 
        'keep *GlobalHaloData_*_*_*', 
        'keep *BeamHaloSummary_BeamHaloSummary_*_*', 
        'keep *_muons_*_*', 
        'keep *_*_muons_*', 
        'drop *_muons_muons1stStep2muonsMap_*', 
        'keep recoTracks_standAloneMuons_*_*', 
        'keep recoTrackExtras_standAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneMuons_*_*', 
        'keep recoTracks_globalMuons_*_*', 
        'keep recoTrackExtras_globalMuons_*_*', 
        'keep recoTracks_tevMuons_*_*', 
        'keep recoTrackExtras_tevMuons_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTracksToOnerecoTracksAssociation_tevMuons_*_*', 
        'keep recoTracks_cosmicMuons_*_*', 
        'keep recoTracks_globalCosmicMuons_*_*', 
        'keep recoMuons_muonsFromCosmics_*_*', 
        'keep recoTracks_cosmicMuons1Leg_*_*', 
        'keep recoTracks_globalCosmicMuons1Leg_*_*', 
        'keep recoMuons_muonsFromCosmics1Leg_*_*', 
        'keep recoTracks_refittedStandAloneMuons_*_*', 
        'keep recoTrackExtras_refittedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_refittedStandAloneMuons_*_*', 
        'keep *_muIsoDepositTk_*_*', 
        'keep *_muIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muIsoDepositJets_*_*', 
        'keep *_trackCountingHighEffBJetTags_*_*', 
        'keep *_trackCountingHighPurBJetTags_*_*', 
        'keep *_jetProbabilityBJetTags_*_*', 
        'keep *_jetBProbabilityBJetTags_*_*', 
        'keep *_simpleSecondaryVertexBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighEffBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighPurBJetTags_*_*', 
        'keep *_combinedSecondaryVertexBJetTags_*_*', 
        'keep *_combinedSecondaryVertexMVABJetTags_*_*', 
        'keep *_ghostTrackBJetTags_*_*', 
        'keep *_softElectronBJetTags_*_*', 
        'keep *_softElectronByIP3dBJetTags_*_*', 
        'keep *_softElectronByPtBJetTags_*_*', 
        'keep *_softMuonBJetTags_*_*', 
        'keep *_softMuonByIP3dBJetTags_*_*', 
        'keep *_softMuonByPtBJetTags_*_*', 
        'keep *_combinedMVABJetTags_*_*', 
        'keep *_ak5PFJetsRecoTauPiZeros_*_*', 
        'keep *_hpsPFTauProducer_*_*', 
        'keep *_hpsPFTauDiscrimination*_*_*', 
        'keep *_shrinkingConePFTauProducer_*_*', 
        'keep *_shrinkingConePFTauDiscrimination*_*_*', 
        'keep *_hpsTancTaus_*_*', 
        'keep *_hpsTancTausDiscrimination*_*_*', 
        'keep *_TCTauJetPlusTrackZSPCorJetAntiKt5_*_*', 
        'keep *_caloRecoTauTagInfoProducer_*_*', 
        'keep recoCaloTaus_caloRecoTauProducer*_*_*', 
        'keep *_caloRecoTauDiscrimination*_*_*', 
        'keep  *_offlinePrimaryVertices__*', 
        'keep  *_offlinePrimaryVerticesWithBS_*_*', 
        'keep  *_offlinePrimaryVerticesFromCosmicTracks_*_*', 
        'keep  *_nuclearInteractionMaker_*_*', 
        'keep *_generalV0Candidates_*_*', 
        'keep recoGsfElectronCores_gsfElectronCores_*_*', 
        'keep recoGsfElectrons_gsfElectrons_*_*', 
        'keep recoGsfElectronCores_uncleanedOnlyGsfElectronCores_*_*', 
        'keep recoGsfElectrons_uncleanedOnlyGsfElectrons_*_*', 
        'keep floatedmValueMap_eidRobustLoose_*_*', 
        'keep floatedmValueMap_eidRobustTight_*_*', 
        'keep floatedmValueMap_eidRobustHighEnergy_*_*', 
        'keep floatedmValueMap_eidLoose_*_*', 
        'keep floatedmValueMap_eidTight_*_*', 
        'keep recoPhotonCores_photonCore_*_*', 
        'keep recoPhotons_photons_*_*', 
        'keep recoConversions_conversions_*_*', 
        'drop *_conversions_uncleanedConversions_*', 
        'keep recoConversions_allConversions_*_*', 
        'keep recoTracks_ckfOutInTracksFromConversions_*_*', 
        'keep recoTracks_ckfInOutTracksFromConversions_*_*', 
        'keep recoConversions_uncleanedOnlyAllConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep *_PhotonIDProd_*_*', 
        'keep *_hfRecoEcalCandidate_*_*', 
        'keep *_hfEMClusters_*_*', 
        'drop CaloTowersSorted_towerMakerPF_*_*', 
        'drop *_pfElectronTranslator_*_*', 
        'keep recoPFRecHits_particleFlowClusterECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFEM_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFHAD_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterPS_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitPS_Cleaned_*', 
        'keep recoPFCandidates_particleFlow_*_*', 
        'keep *_particleFlow_electrons_*', 
        'keep *_particleFlow_photons_*', 
        'keep recoCaloClusters_pfElectronTranslator_*_*', 
        'keep recoPreshowerClusters_pfElectronTranslator_*_*', 
        'keep recoSuperClusters_pfElectronTranslator_*_*', 
        'keep recoCaloClusters_pfPhotonTranslator_*_*', 
        'keep recoPreshowerClusters_pfPhotonTranslator_*_*', 
        'keep recoSuperClusters_pfPhotonTranslator_*_*', 
        'keep recoPhotons_pfPhotonTranslator_*_*', 
        'keep recoPhotonCores_pfPhotonTranslator_*_*', 
        'keep recoConversions_pfPhotonTranslator_*_*', 
        'keep *_offlineBeamSpot_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_l1GtRecord_*_*', 
        'keep *_l1GtTriggerMenuLite_*_*', 
        'keep *_conditionsInEdm_*_*', 
        'keep *_l1extraParticles_*_*', 
        'keep *_l1L1GtObjectMap_*_*', 
        'keep LumiSummary_lumiProducer_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'drop L1GlobalTriggerObjectMapRecord_hltL1GtObjectMap_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1TriggerScalerss_*_*_*', 
        'keep Level1TriggerScalerss_*_*_*', 
        'keep LumiScalerss_*_*_*', 
        'keep BeamSpotOnlines_*_*_*', 
        'keep DcsStatuss_*_*_*', 
        'keep *_logErrorHarvester_*_*', 
        'keep PileupSummaryInfos_*_*_*', 
        'drop *_MEtoEDMConverter_*_*', 
        'drop *_*_*_SKIM'),
    fileName = cms.untracked.string('HZZ.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('HZZ'),
        dataTier = cms.untracked.string('AOD')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880)
)

# Other statements
process.GlobalTag.globaltag = 'GR_R_52_V7::All'

# Path and EndPath definitions
process.SKIMStreamHZZOutPath = cms.EndPath(process.SKIMStreamHZZ)

# Schedule definition
process.schedule = cms.Schedule(process.HZZ4ePath,process.HZZ2e2mPath,process.HZZ2m2ePath,process.HZZ4mPath,process.HZZem2ePath,process.HZZem2mPath,process.SKIMStreamHZZOutPath)

