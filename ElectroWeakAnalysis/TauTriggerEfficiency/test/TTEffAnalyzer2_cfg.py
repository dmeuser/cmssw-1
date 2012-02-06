import FWCore.ParameterSet.Config as cms
import copy

isData = False
runL1Emulator = False
hltType = "HLT"
#hltType = "REDIGI38X"

process = cms.Process("TTEff")

### Add HLT stuff (it may contain maxEvents and MessageLogger, so it
### should be loaded first before or maxEvents nad MessageLogger would
### be reset)
process.load("ElectroWeakAnalysis.TauTriggerEfficiency.TTEffAnalysisHLT_cfi")
process.prefer("magfield")
process.hltGctDigis.hltMode = cms.bool(False) # Making L1CaloRegions

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(10)
)

process.load("FWCore/MessageService/MessageLogger_cfi")
process.MessageLogger.categories.append("TTEffAnalyzer")
process.MessageLogger.cerr.FwkReport.reportEvery = 100 # print the event number for every 100th event
process.MessageLogger.cerr.TTEffAnalyzer = cms.untracked.PSet(limit = cms.untracked.int32(100)) # print max 100 warnings from TTEffAnalyzer
# process.MessageLogger.debugModules = cms.untracked.vstring("TTEffAnalyzer")
# process.MessageLogger.cerr.threshold = cms.untracked.string("DEBUG")   # pring LogDebugs and above
# process.MessageLogger.cerr.threshold = cms.untracked.string("INFO")    # print LogInfos and above
# process.MessageLogger.cerr.threshold = cms.untracked.string("WARNING") # print LogWarnings and above

process.options = cms.untracked.PSet(
#    wantSummary = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(False),
    SkipEvent = cms.untracked.vstring("TrajectoryState") # FIXME: problem with cmssw42X TSGFromL2Muon
)

#Mike needs Calo Geometry
process.load('Configuration/StandardSequences/GeometryPilot2_cff')


if(isData):
    process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(
        "file:TTEffSkim.root"
#	"/store/user/luiggi/MinimumBias/TTEffSkimRun2011A_GoldenPlusESIgnoredJSON/a6b050dc4acb87f74e46528e006dff64/TTEffSkim_1_1_Zd8.root",
#	"/store/user/luiggi/MinimumBias/TTEffSkimRun2011A_GoldenPlusESIgnoredJSON/a6b050dc4acb87f74e46528e006dff64/TTEffSkim_2_1_IA6.root",
#	"/store/user/luiggi/MinimumBias/TTEffSkimRun2011A_GoldenPlusESIgnoredJSON/a6b050dc4acb87f74e46528e006dff64/TTEffSkim_3_1_I9j.root"
	)
    )
else:
    process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring(
	"file:TTEffSkim.root"
#        "file:/tmp/slehti/TTToHplusBWB_M_160_7TeV_pythia6_tauola_Fall11_E7TeV_Ave23_50ns_v2_RAW_RECO_TTEffSkim_160_1_OZG.root"
	)
    )

process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
if (isData):
    process.GlobalTag.globaltag = 'GR_H_V24::All'
#    process.GlobalTag.globaltag = 'TESTL1_GR_P::All'
else:
    process.GlobalTag.globaltag = 'START44_V12::All'
    #process.GlobalTag.globaltag = 'MC_38Y_V14::All'
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_COND_31X_GLOBALTAG'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
print process.GlobalTag.globaltag

#MET cleaning flag
process.load('CommonTools/RecoAlgos/HBHENoiseFilterResultProducer_cfi')
process.runMETCleaning = cms.Path(process.HBHENoiseFilterResultProducer)


#Physics bit ON
process.load('HLTrigger.special.hltPhysicsDeclared_cfi')
process.hltPhysicsDeclared.L1GtReadoutRecordTag = 'gtDigis'

process.commonSequence = cms.Sequence()
if not isData:
    process.TauMCProducer = cms.EDProducer("HLTTauMCProducer",
        GenParticles  = cms.untracked.InputTag("genParticles"),
        ptMinTau      = cms.untracked.double(3),
        ptMinMuon     = cms.untracked.double(3),
        ptMinElectron = cms.untracked.double(3),
        BosonID       = cms.untracked.vint32(23),
        EtaMax         = cms.untracked.double(2.5)
    )

    process.commonSequence += (
        process.hltPhysicsDeclared+
	process.TauMCProducer
    )

# Reproduce the mu-tau pair objects, as they are not kept in the skim at the moment
import ElectroWeakAnalysis.TauTriggerEfficiency.ZtoMuTauFilter_cfi as zmutau
process.muTauPairs = zmutau.muTauPairs.clone()
process.commonSequence *= process.muTauPairs

# Correction of reco::PFJets (from 5xy configuration due to JetMETCorrections/Modules ...
process.ak5PFL1Offset = cms.ESProducer('L1OffsetCorrectionESProducer',
    level = cms.string('L1Offset'),
    algorithm = cms.string('AK5PF'),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    minVtxNdof = cms.int32(4)
)
process.ak5PFL2Relative = cms.ESProducer('LXXXCorrectionESProducer',
    level     = cms.string('L2Relative'),
    algorithm = cms.string('AK5PF')
)
process.ak5PFL3Absolute = cms.ESProducer('LXXXCorrectionESProducer',
    level     = cms.string('L3Absolute'),
    algorithm = cms.string('AK5PF')
)
process.ak5PFL1L2L3 = cms.ESProducer('JetCorrectionESChain',
    correctors = cms.vstring('ak5PFL1Offset','ak5PFL2Relative','ak5PFL3Absolute')
)
process.ak5PFJetsL1L2L3   = cms.EDProducer('PFJetCorrectionProducer',
     src         = cms.InputTag('ak5PFJets'),
    correctors  = cms.vstring('ak5PFL1L2L3')
)
process.commonSequence *= process.ak5PFJetsL1L2L3

# Good vertex selection
process.goodPrimaryVertices = cms.EDFilter("VertexSelector",
    filter = cms.bool(False),
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string("!isFake && ndof > 4 && abs(z) < 24.0 && position.rho < 2.0")
)
process.commonSequence *= process.goodPrimaryVertices

# Analyzer definition
process.TTEffAnalysisHLTPFTauHPS = cms.EDAnalyzer("TTEffAnalyzer2",
        LoopingOver	        = cms.InputTag("selectedPatTausHpsPFTau"),
	PFTauDiscriminators     = cms.vstring(
            "decayModeFinding",
            "againstMuonLoose",
            "againstMuonMedium",
            "againstMuonTight",
            "againstElectronLoose",
            "againstElectronMedium",
            "againstElectronTight",
            "byVLooseIsolation",
            "byLooseIsolation",
            "byMediumIsolation",
            "byTightIsolation",
            "byVLooseIsolationDeltaBetaCorr",
            "byLooseIsolationDeltaBetaCorr",
            "byMediumIsolationDeltaBetaCorr",
            "byTightIsolationDeltaBetaCorr",
            "byVLooseCombinedIsolationDeltaBetaCorr",
            "byLooseCombinedIsolationDeltaBetaCorr",
            "byMediumCombinedIsolationDeltaBetaCorr",
            "byTightCombinedIsolationDeltaBetaCorr",
        ),
        Counters                = cms.VInputTag(cms.InputTag("TTEffSkimCounterAllEvents"),
                                                cms.InputTag("TTEffSkimCounterSavedEvents")
                                                ),

        METs = cms.PSet(
            PFMET = cms.InputTag("pfMet"),
            HLTMET = cms.InputTag("hltMet"),
            HLTMHT = cms.InputTag("hltPFMHTProducer"),
        ),
        Jets = cms.InputTag("ak5PFJetsL1L2L3"),
        offlineVertexSrc = cms.InputTag("goodPrimaryVertices"),

	L1extraTauJetSource			= cms.InputTag("l1extraParticles", "Tau"),
	L1extraCentralJetSource			= cms.InputTag("l1extraParticles", "Central"),

	L1extraMETSource			= cms.InputTag("l1extraParticles", "MET"),
	L1extraMHTSource			= cms.InputTag("l1extraParticles", "MHT"),

        L1GtReadoutRecord       		= cms.InputTag("gtDigis",""),
        L1GtObjectMapRecord     		= cms.InputTag("hltL1GtObjectMap","",hltType),
        L1Paths = cms.vstring(
            "L1_SingleMu7",
            "L1_SingleMu10",
            "L1_SingleMu12",
            "L1_SingleMu14_Eta2p1",
            "L1_SingleMu16_Eta2p1",

            "L1_SingleTauJet52", "L1_SingleJet68",
            "L1_SingleJet52_Central",
            "L1_Jet52_Central_ETM30",
            "L1_TripleJet28_Central",
            "L1_ETM30",
        ),
        HltResults              		= cms.InputTag("TriggerResults","",hltType),
        HltPaths = cms.vstring(
            "HLT_IsoMu17_v5", "HLT_IsoMu17_v6", "HLT_IsoMu17_v8", "HLT_IsoMu17_v9", "HLT_IsoMu17_v10", "HLT_IsoMu17_v11", "HLT_IsoMu17_v13", "HLT_IsoMu17_v14",
            "HLT_IsoMu17_eta2p1_v1",
            "HLT_IsoMu20_v8", "HLT_IsoMu20_v9", "HLT_IsoMu20_v12", "HLT_IsoMu20_v13",
            "HLT_IsoMu20_eta2p1_v1",
            "HLT_IsoMu24_v1", "HLT_IsoMu24_v2", "HLT_IsoMu24_v4", "HLT_IsoMu24_v5", "HLT_IsoMu24_v6", "HLT_IsoMu24_v7", "HLT_IsoMu24_v8", "HLT_IsoMu24_v9", "HLT_IsoMu24_v12", "HLT_IsoMu24_v13",
            "HLT_IsoMu24_eta2p1_v3", "HLT_IsoMu24_eta2p1_v6", "HLT_IsoMu24_eta2p1_v7"
            "HLT_IsoMu30_v1", "HLT_IsoMu30_v2", "HLT_IsoMu30_v4", "HLT_IsoMu30_v5", "HLT_IsoMu30_v6", "HLT_IsoMu30_v7", "HLT_IsoMu30_v8",
            "HLT_IsoMu30_eta2p1_v3", "HLT_IsoMu30_eta2p1_v6", "HLT_IsoMu30_eta2p1_v7",
            "HLT_IsoMu34_eta2p1_v1",


            "HLT_IsoPFTau35_Trk20_MET45_v1", "HLT_IsoPFTau35_Trk20_MET45_v2", "HLT_IsoPFTau35_Trk20_MET45_v4", "HLT_IsoPFTau35_Trk20_MET45_v6",
            "HLT_IsoPFTau35_Trk20_v2", "HLT_IsoPFTau35_Trk20_v3", "HLT_IsoPFTau35_Trk20_v4", "HLT_IsoPFTau35_Trk20_v6",
            "HLT_IsoPFTau35_Trk20_MET60_v2", "HLT_IsoPFTau35_Trk20_MET60_v3", "HLT_IsoPFTau35_Trk20_MET60_v4", "HLT_IsoPFTau35_Trk20_MET60_v6",
            "HLT_IsoPFTau45_Trk20_MET60_v2", "HLT_IsoPFTau45_Trk20_MET60_v3", "HLT_IsoPFTau45_Trk20_MET60_v4",
            "HLT_IsoPFTau35_Trk20_MET70_v2",
            "HLT_MediumIsoPFTau35_Trk20_v1", "HLT_MediumIsoPFTau35_Trk20_v5", "HLT_MediumIsoPFTau35_Trk20_v6",
            "HLT_MediumIsoPFTau35_Trk20_MET60_v1", "HLT_MediumIsoPFTau35_Trk20_MET60_v5", "HLT_MediumIsoPFTau35_Trk20_MET60_v6",
            "HLT_MediumIsoPFTau35_Trk20_MET70_v1", "HLT_MediumIsoPFTau35_Trk20_MET70_v5", "HLT_MediumIsoPFTau35_Trk20_MET70_v6",
        ),
        L1TauTriggerSource      		= cms.InputTag("tteffL1GTSeed"),
	L1JetMatchingCone			= cms.double(0.5),
	L1JetMatchingMode			= cms.string("nearestDR"), # "nearestDR", "highestEt"
        L1IsolationThresholds   		= cms.vuint32(1,2,3,4), # count regions with "et() < threshold", these are in GeV

	L2AssociationCollection 		= cms.InputTag("openhltL2TauIsolationProducer"),
        L2matchingDeltaR        		= cms.double(0.2),

        hltVertexSrc                            = cms.InputTag("hltPixelVertices"),
        L25TauSource                            = cms.InputTag("hltPFTaus"),
        L25MatchingCone                         = cms.double(0.3),
        L25Discriminators = cms.PSet(
        ),
        L3IsoQualityCuts                        = process.hltPFTauLooseIsolationDiscriminator.qualityCuts.isolationQualityCuts.clone(),

        PileupSummaryInfoSource                 = cms.InputTag("addPileupInfo"),
        outputFileName          		= cms.string("tteffAnalysis-hltpftau-hpspftau.root")
)

# Reference is HPS tau
process.TTEffAnalysisHLTPFTauTightHPS = process.TTEffAnalysisHLTPFTauHPS.clone(
    L25TauSource = "hltPFTausTightIso",
    L3IsoQualityCuts = process.hltPFTauTightIsoIsolationDiscriminator.qualityCuts.isolationQualityCuts.clone(),
    outputFileName = "tteffAnalysis-hltpftautight-hpspftau.root",
)
process.TTEffAnalysisHLTPFTauMediumHPS = process.TTEffAnalysisHLTPFTauHPS.clone(
    L25TauSource = "hltPFTausMediumIso",
    L3IsoQualityCuts = process.hltPFTauMediumIsoIsolationDiscriminator.qualityCuts.isolationQualityCuts.clone(),
    outputFileName = "tteffAnalysis-hltpftaumedium-hpspftau.root",
)
process.TTEffAnalysisHLTPFTauMediumHPS.L25Discriminators.MediumIso = cms.InputTag("hltPFTauMediumIsoIsolationDiscriminator")
process.TTEffAnalysisHLTPFTauMediumHPS.L25Discriminators.TrackFinding = cms.InputTag("hltPFTauMediumIsoTrackFindingDiscriminator")
process.TTEffAnalysisHLTPFTauMediumHPS.L25Discriminators.TrackPt20 = cms.InputTag("hltPFTauMediumIsoTrackPt20Discriminator")
#process.TTEffAnalysisHLTPFTauTightHPS.L25Discriminators.MediumIso = cms.InputTag("hltPFTauMediumIsoIsolationDiscriminator")
process.TTEffAnalysisHLTPFTauMediumHPSL2Global = process.TTEffAnalysisHLTPFTauHPS.clone(
    L2AssociationCollection = "openhltL2TauGlobalIsolationProducer",
    outputFileName = "tteffAnalysis-hltpftaumediuml2global-hpspftau.root"
)

process.runTTEffAna = cms.Path(process.commonSequence)
process.runTTEffAna += process.TTEffAnalysisHLTPFTauHPS
process.runTTEffAna += process.TTEffAnalysisHLTPFTauTightHPS
process.runTTEffAna += process.TTEffAnalysisHLTPFTauMediumHPS
process.runTTEffAna += process.TTEffAnalysisHLTPFTauMediumHPSL2Global

# The high purity selection (mainly for H+)
# process.load("ElectroWeakAnalysis.TauTriggerEfficiency.HighPuritySelection_cff")
# process.TTEffAnalysisHLTPFTauTightHPSHighPurity = process.TTEffAnalysisHLTPFTauTightHPS.clone(
#     LoopingOver = "selectedPatTausHpsPFTauHighPurity",
#     MuonSource = "selectedPatMuonsHighPurity",
#     MuonTauPairSource = "muTauPairsHighPurity",
#     outputFileName = "tteffAnalysis-hltpftautight-hpspftau-highpurity.root"
# )
# process.runTTEffAnaHighPurity = cms.Path(
#     process.commonSequence +
#     process.highPuritySequence +
#     process.TTEffAnalysisHLTPFTauTightHPSHighPurity
# )

# L1 emulator
process.L1simulation_step = cms.Path()
if runL1Emulator:
    if isData:
        process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
    else:
        process.load('Configuration.StandardSequences.RawToDigi_cff')
    process.L1simulation_step = cms.Path(
        process.RawToDigi *
        process.SimL1Emulator
    )
    import L1Trigger.Configuration.customise_l1EmulatorFromRaw as l1emulator
    l1emulator.customise(process)

    process.l1emuL1extraParticles = process.hltL1extraParticles.clone(
        muonSource = cms.InputTag( 'simGmtDigis' ),
        isolatedEmSource = cms.InputTag( 'simGctDigis','isoEm' ),
        nonIsolatedEmSource = cms.InputTag( 'simGctDigis','nonIsoEm' ),
        centralJetSource = cms.InputTag( 'simGctDigis','cenJets' ),
        forwardJetSource = cms.InputTag( 'simGctDigis','forJets' ),
        tauJetSource = cms.InputTag( 'simGctDigis','tauJets' ),
        etTotalSource = cms.InputTag( 'simGctDigis' ),
        etHadSource = cms.InputTag( 'simGctDigis' ),
        etMissSource = cms.InputTag( 'simGctDigis' )
    )
    process.l1emuL1GtObjectMap = process.hltL1GtObjectMap.clone(
        GmtInputTag = "simGmtDigis",
        GctInputTag = "simGctDigis"
    )
    process.L1simulation_step *= (process.l1emuL1extraParticles *
                                  process.l1emuL1GtObjectMap)


    def setL1Emu(module):
        module.L1extraTauJetSource = cms.InputTag("l1emuL1extraParticles", "Tau")
        module.L1extraCentralJetSource = cms.InputTag("l1emuL1extraParticles", "Central")
        module.L1extraMETSource = cms.InputTag("l1emuL1extraParticles", "MET")
        module.L1extraMHTSource = cms.InputTag("l1emuL1extraParticles", "MHT")
        module.L1CaloRegionSource = "simRctDigis"
        module.L1GtReadoutRecord = "simGtDigis"
        module.L1GtObjectMapRecord = "l1emuL1GtObjectMap"

    process.TEffAnalysisHLTPFTauTightHPSL1Emu = process.TTEffAnalysisHLTPFTauTightHPS.clone(
        outputFileName = "tteffAnalysis-hltpftautight-hpspftau-l1emu.root"
    )
    setL1Emu(process.TEffAnalysisHLTPFTauTightHPSL1Emu)
    process.runTTEffAna += process.TEffAnalysisHLTPFTauTightHPSL1Emu

    process.TTEffAnalysisHLTPFTauTightHPSHighPurityL1Emu = process.TTEffAnalysisHLTPFTauTightHPSHighPurity.clone(
        outputFileName = "tteffAnalysis-hltpftautight-hpspftau-l1emu-highpurity.root"
    )
    setL1Emu(process.TTEffAnalysisHLTPFTauTightHPSHighPurityL1Emu)
    process.runTTEffAnaHighPurity *= process.TTEffAnalysisHLTPFTauTightHPSHighPurityL1Emu

process.o1 = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring("keep *"),
    fileName = cms.untracked.string('cmssw.root')
)
#process.outpath = cms.EndPath(process.o1)

process.HLTPFTauSequence+= process.hltPFTausTightIso
process.DoMiscHLT = cms.Path(process.hltPFMHTProducer)
process.schedule = cms.Schedule(process.DoHLTJets,
#				process.DoHltMuon,
				process.DoHLTPhoton,
				process.DoHLTElectron,
				process.DoHLTTau,
				process.DoHLTMinBiasPixelTracks,
                                process.DoMiscHLT,
				process.runMETCleaning,
                                process.L1simulation_step,
				process.runTTEffAna,
#                                process.runTTEffAnaHighPurity
#				,process.outpath
)

if isData:  # replace all instances of "rawDataCollector" with "source" in In$
    from FWCore.ParameterSet import Mixins
    for module in process.__dict__.itervalues():
        if isinstance(module, Mixins._Parameterizable):
            for parameter in module.__dict__.itervalues():
                if isinstance(parameter, cms.InputTag):
                    if parameter.moduleLabel == 'rawDataCollector':
                        parameter.moduleLabel = 'source'

