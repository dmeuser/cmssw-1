import FWCore.ParameterSet.Config as cms

process = cms.Process("ana")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = ''
process.MessageLogger.cerr.FwkReport.reportEvery = 100


process.load("HeavyFlavorAnalysis.InclB.HFGenerator_cff")

process.load("HeavyFlavorAnalysis.InclB.HFPartons_cff")

process.load("HeavyFlavorAnalysis.InclB.HFTracks_cff")
process.trkDump.doTruthMatching = 0
process.trkDump.verbose = 0


process.load("HeavyFlavorAnalysis.InclB.HFSignal_cff")
process.signalDump.verbose = 0

process.load("HeavyFlavorAnalysis.InclB.HFTrackJets_cff")
process.trackjetDump.doflavortagging = 0

process.load("HeavyFlavorAnalysis.InclB.HFVertex_cff")
process.vertexDump.verbose = 0

process.load("HeavyFlavorAnalysis.InclB.HFTrigger_cff")
process.triggerDump.dol1 = 2
process.triggerDump.verbose = 0

process.load("HeavyFlavorAnalysis.InclB.HFCandidate_cff")

process.load("HeavyFlavorAnalysis.InclB.HFCandidateNew_cff")

process.load("Configuration.StandardSequences.MagneticField_cff")

process.load("Configuration.StandardSequences.Services_cff")

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "GR_R_35X_V7::All" # or "" or...
#process.GlobalTag.globaltag = "GR09_R_35X_V4::All" # or "" or...

process.source = cms.Source("PoolSource",
   
   
    fileNames = cms.untracked.vstring(
  
    'file:dcap://t3se01.psi.ch:22125//pnfs/psi.ch/cms/trivcat/store/user/caminada/cmssw/34ADB6C9-F36E-DF11-A1A4-0026189438D7.root'
    
    )
    
                          
   

)




process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)
process.tree = cms.EDAnalyzer("HFTree",
    fileName = cms.string('data.root')
)

process.histo_begin = cms.EDAnalyzer("HFHisto",
    fileName = cms.string('histo_begin.root')
)

process.histo_end = cms.EDAnalyzer("HFHisto",
    fileName = cms.string('histo_end.root')
)

process.filterpkame = cms.EDFilter("PKAME",
    tracksLabel = cms.untracked.string('generalTracks')
)

# this is for filtering on L1 technical trigger bit
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
# different choices of trigger conditions:
# bsc minbias
#process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(40 OR 41)')
# bsc minbias and veto on beam halo
#process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')
# bsc minbias in coincidence with bptx
#process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('0 AND (40 OR 41)')
# bsc minbias in coinidence with bptx and veto on beam halo
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('0 AND (40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')



#filter based on L1 decision
process.load('L1Trigger.Skimmer.l1Filter_cfi')
process.l1Filter.algorithms = cms.vstring('L1_SingleJet6','L1_SingleForJet2')


#process.p = cms.Path(process.hltLevel1GTSeed + process.filterpkame*process.histo_begin*process.CandidateDump*process.CandidateDumpNew*process.TrackJetDumpAOD*process.TrackDumpAOD*process.SignalDump*process.VertexDump*process.TriggerDump*process.tree*process.histo_end)

process.p = cms.Path(process.filterpkame*process.histo_begin*process.CandidateDump*process.CandidateDumpNew*process.TrackJetDumpAOD*process.TrackDumpAOD*process.SignalDump*process.VertexDump*process.TriggerDump*process.tree*process.histo_end)






