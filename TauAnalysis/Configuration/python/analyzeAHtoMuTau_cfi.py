import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
# import configuration parameters of Z --> muon + tau-jet channel
#
# WARNING: definitions from analyzeZtoMuTau_cfi.py need to be imported before any
#          A/H --> muon + tau-jet channel specific configuration parameters are defined;
#          otherwise the import of analyzeZtoMuTau_cfi.py will **overwrite**
#          A/H --> muon + tau-jet channel specific definitions
#         (due to the fact that e.g. histogram managers of 'ZtoMuTau' channel
#          and 'AHtoMuTau' channel have the same name '...ForMuTau')
#
from TauAnalysis.Configuration.analyzeZtoMuTau_cfi import *
#--------------------------------------------------------------------------------

# import config for histogram manager filling information about phase-space simulated in Monte Carlo sample
from TauAnalysis.Core.genPhaseSpaceEventInfoHistManager_cfi import *

# import config for event weight histogram manager
from TauAnalysis.Core.eventWeightHistManager_cfi import *

# import config for muon histogram manager
from TauAnalysis.Core.muonHistManager_cfi import *
muonHistManager.src = cms.InputTag('selectedPatMuonsTrkIPcumulative')

# import config for tau-jet histogram manager
from TauAnalysis.Core.pftauHistManager_cfi import *
tauHistManager.src = cms.InputTag('selectedPatTausForMuTauElectronVetoCumulative')
tauHistManager.useHPSpTaNCalgorithm = cms.bool(True)

# import config for di-tau histogram manager
from TauAnalysis.Core.diTauCandidateHistManager_cfi import *
diTauCandidateHistManagerForMuTau = copy.deepcopy(diTauCandidateHistManager)
diTauCandidateHistManagerForMuTau.pluginName = cms.string('diTauCandidateHistManagerForMuTau')
diTauCandidateHistManagerForMuTau.pluginType = cms.string('PATMuTauPairHistManager')
diTauCandidateHistManagerForMuTau.diTauCandidateSource = cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeCumulative')
diTauCandidateHistManagerForMuTau.visMassHypothesisSource = cms.InputTag('')

from TauAnalysis.Core.diTauCandidateNSVfitHistManager_cfi import *
diTauCandidateNSVfitHistManagerForMuTau = copy.deepcopy(diTauCandidateNSVfitHistManager)
diTauCandidateNSVfitHistManagerForMuTau.pluginName = cms.string('diTauCandidateNSVfitHistManagerForMuTau')
diTauCandidateNSVfitHistManagerForMuTau.pluginType = cms.string('PATMuTauPairNSVfitHistManager')
diTauCandidateNSVfitHistManagerForMuTau.diTauCandidateSource = cms.InputTag('selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative')
diTauCandidateNSVfitHistManagerForMuTau.nSVfitEventHypotheses = cms.PSet(
    psKine_MEt_logM_fit = cms.string('psKine_MEt_logM_fit'),
    psKine_MEt_logM_int = cms.string('psKine_MEt_logM_int')
)

from TauAnalysis.Core.diTauCandidateZllHypothesisHistManager_cfi import *
diTauCandidateZmumuHypothesisHistManagerForMuTau = copy.deepcopy(ZllHypothesisHistManager)
diTauCandidateZmumuHypothesisHistManagerForMuTau.pluginName = cms.string('diTauCandidateZmumuHypothesisHistManagerForMuTau')
diTauCandidateZmumuHypothesisHistManagerForMuTau.pluginType = cms.string('ZllHypothesisMuTauHistManager')
diTauCandidateZmumuHypothesisHistManagerForMuTau.ZllHypothesisSource = cms.InputTag('muTauPairZmumuHypotheses')
diTauCandidateZmumuHypothesisHistManagerForMuTau.dqmDirectory_store = cms.string('DiTauCandidateZmumuHypothesisQuantities')

# import config for Z/gamma* --> mu+ mu- veto histogram manager
muPairHistManagerZmumuHypothesesByLooseIsolation = diTauCandidateHistManager.clone(
    pluginName = cms.string('muPairHistManagerZmumuHypothesesByLooseIsolation'),
    pluginType = cms.string('PATDiMuPairHistManager'),
    diTauCandidateSource = cms.InputTag('allDiMuPairZmumuHypothesesByLooseIsolation'),
    dqmDirectory_store = cms.string('DiMuonFromZmumuQuantities')
)
muPairHistManagerDYmumuHypotheses = diTauCandidateHistManager.clone(
    pluginName = cms.string('muPairHistManagerDYmumuHypotheses'),
    pluginType = cms.string('PATDiMuPairHistManager'),
    diTauCandidateSource = cms.InputTag('allDiMuPairDYmumuHypotheses'),
    dqmDirectory_store = cms.string('DiMuonFromDYmumuQuantities')
)

# import config for VBF event hypothesis histogram manager
from TauAnalysis.Core.vbfDiTauCandidateEventHistManager_cfi import *
vbfEventHistManagerForMuTau = copy.deepcopy(vbfDiTauCandidateEventHistManager)
vbfEventHistManagerForMuTau.pluginName = cms.string('vbfEventHistManagerForMuTau')
vbfEventHistManagerForMuTau.pluginType = cms.string('PATMuTauPairVBFEventHistManager')
vbfEventHistManagerForMuTau.vbfEventSource = cms.InputTag('allVBFEventHypothesesForAHtoMuTau')

# import config for central jet veto histogram manager
from TauAnalysis.Core.jetHistManager_cfi import *

# import config for missing-Et histogram managers
from TauAnalysis.Core.caloMEtHistManager_cfi import *
caloMEtHistManager.leg1Source = muonHistManager.src
caloMEtHistManager.leg2Source = tauHistManager.src
from TauAnalysis.Core.pfMEtHistManager_cfi import *
pfMEtHistManager.leg1Source = muonHistManager.src
pfMEtHistManager.leg2Source = tauHistManager.src

# import config for particle multiplicity histogram manager
from TauAnalysis.Core.particleMultiplicityHistManager_cfi import *

# import config for primary event vertex histogram manager
from TauAnalysis.Core.vertexHistManager_cfi import *

# import config for L1 & HLT histogram manager
from TauAnalysis.Core.triggerHistManager_cfi import *
triggerHistManagerForMuTau = copy.deepcopy(triggerHistManager)
triggerHistManagerForMuTau.pluginName = cms.string('triggerHistManagerForMuTau')
triggerHistManagerForMuTau.l1Bits = cms.vstring(
    'L1_SingleMu3',
    'L1_SingleMu5',
    'L1_SingleMu7',
    'L1_SingleMu10',
    'L1_SingleMu14'
)

triggerHistManagerForMuTau.hltPaths = cms.vstring(
    'HLT_Mu9',
    'HLT_IsoMu9',
    'HLT_Mu11',
    'HLT_Mu15'
)

# import config for event weight histogram manager
from TauAnalysis.Core.eventWeightHistManager_cfi import *

# import config for binning results
# used for keeping track of number of events passing all selection criteria
from TauAnalysis.Core.dataBinner_cfi import *

# import config for binning results
# used to estimate acceptance of event selection
from TauAnalysis.Core.modelBinner_cfi import *
modelBinnerForMuTauGenTauLeptonPairAcc = modelBinner.clone(
    pluginName = cms.string('modelBinnerForMuTauGenTauLeptonPairAcc'),
    srcGenFlag = cms.InputTag("isGenAHtoMuTau"),
    srcRecFlag = cms.InputTag("isGenAHtoMuTauWithinAcceptance"),
    dqmDirectory_store = cms.string('modelBinnerForMuTauGenTauLeptonPairAcc')
)
modelBinnerForMuTauCentralJetVetoWrtGenTauLeptonPairAcc = modelBinner.clone(
    pluginName = cms.string('modelBinnerForMuTauCentralJetVetoWrtGenTauLeptonPairAcc'),
    srcGenFlag = cms.InputTag("isGenAHtoMuTauWithinAcceptance"),
    srcRecFlag = cms.InputTag("isRecAHtoMuTauCentralJetVeto"),
    dqmDirectory_store = cms.string('modelBinnerForMuTauCentralJetVetoWrtGenTauLeptonPairAcc')
)
modelBinnerForMuTauCentralJetBtagWrtGenTauLeptonPairAcc = modelBinner.clone(
    pluginName = cms.string('modelBinnerForMuTauCentralJetBtagWrtGenTauLeptonPairAcc'),
    srcGenFlag = cms.InputTag("isGenAHtoMuTauWithinAcceptance"),
    srcRecFlag = cms.InputTag("isRecAHtoMuTauCentralJetBtag"),
    dqmDirectory_store = cms.string('modelBinnerForMuTauCentralJetBtagWrtGenTauLeptonPairAcc')
)

# import config for binning results
# used to estimate systematic uncertainties
from TauAnalysis.Core.sysUncertaintyBinner_cfi import *
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *
sysUncertaintyNames = [ "CENTRAL_VALUE", ]
sysUncertaintyNames.extend(
    getSysUncertaintyNames(
        [ muonSystematics,
          tauSystematics,
          muTauPairSystematics,
          jetSystematics,
          theorySystematics ]
    )
)
sysUncertaintyBinnerForMuTauAccCentralJetVeto = sysUncertaintyBinner.clone(
    pluginName = cms.string('sysUncertaintyBinnerForMuTauAccCentralJetVeto'),
    binnerPlugins = cms.VPSet(
        modelBinnerForMuTauGenTauLeptonPairAcc,
        modelBinnerForMuTauCentralJetVetoWrtGenTauLeptonPairAcc
    ),
    systematics = cms.vstring(sysUncertaintyNames)
)
sysUncertaintyBinnerForMuTauAccCentralJetBtag = sysUncertaintyBinner.clone(
    pluginName = cms.string('sysUncertaintyBinnerForMuTauAccCentralJetBtag'),
    binnerPlugins = cms.VPSet(
        modelBinnerForMuTauGenTauLeptonPairAcc,
        modelBinnerForMuTauCentralJetBtagWrtGenTauLeptonPairAcc
    ),
    systematics = cms.vstring(sysUncertaintyNames)
)
sysUncertaintyBinnerForMuTauEff = sysUncertaintyBinner.clone(
    pluginName = cms.string('sysUncertaintyBinnerForMuTauEff'),
    binnerPlugins = cms.VPSet(
        dataBinner
    ),
    systematics = cms.vstring(sysUncertaintyNames)
)

sysUncertaintyHistManagerForMuTau = cms.PSet(
    pluginName = cms.string('sysUncertaintyHistManagerForMuTau'),
    pluginType = cms.string('SysUncertaintyHistManager'),
    histManagers = cms.VPSet(
        cms.PSet(
            config = diTauCandidateHistManagerForMuTau,
            systematics = cms.PSet(
                diTauCandidateSource = getSysUncertaintyParameterSets(
                    [ muTauPairSystematics ]
                )
            )
        ),
        cms.PSet(
            config = diTauCandidateNSVfitHistManagerForMuTau,
            systematics = cms.PSet(
                diTauCandidateSource = getSysUncertaintyParameterSets(
                    [ muTauPairSystematics ]
                )
            )
        )
    ),
    dqmDirectory_store = cms.string('sysUncertaintyHistManagerResults')
)

diTauLeg1ChargeBinGridHistManager = cms.PSet(
    pluginName = cms.string('diTauLeg1ChargeBinGridHistManager'),
    pluginType = cms.string('BinGridHistManager'),
    binning = cms.PSet(
        name = cms.string("diTauLeg1ChargeBinning"),
        config = cms.VPSet(
            cms.PSet(
                extractor = cms.PSet(
                    pluginType = cms.string("PATMuTauPairValExtractor"),
                    src = cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeCumulative'),
                    value = cms.string('leg1.charge')
                ),
                branchName = cms.string('diTauLeg1Charge'),
                binning = cms.PSet(
                    boundaries = cms.vdouble(0.),
                    min = cms.double(-2.),
                    max = cms.double(+2.)
                )
            )
        )
    ),
    histManagers = cms.VPSet(
        diTauCandidateHistManagerForMuTau,
        diTauCandidateNSVfitHistManagerForMuTau
    ),
    dqmDirectory_store = cms.string('diTauLeg1ChargeBinnedHistograms')
)

#--------------------------------------------------------------------------------
# define event selection criteria
#--------------------------------------------------------------------------------

# di-tau candidate selection
evtSelDiTauCandidateForAHtoMuTauAntiOverlapVeto = evtSelDiTauCandidateForMuTauAntiOverlapVeto.clone(
    pluginName = cms.string('evtSelDiTauCandidateForAHtoMuTauAntiOverlapVeto'),
    src_cumulative = cms.InputTag('diTauCandidateForAHtoMuTauAntiOverlapVeto', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoMuTauAntiOverlapVeto', 'individual')
)
evtSelDiTauCandidateForAHtoMuTauMt1MET = evtSelDiTauCandidateForMuTauMt1MET.clone(
    pluginName = cms.string('evtSelDiTauCandidateForAHtoMuTauMt1MET'),
    src_cumulative = cms.InputTag('diTauCandidateForAHtoMuTauMt1METcut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoMuTauMt1METcut', 'individual')
)
evtSelDiTauCandidateForAHtoMuTauPzetaDiff = evtSelDiTauCandidateForMuTauPzetaDiff.clone(
    pluginName = cms.string('evtSelDiTauCandidateForAHtoMuTauPzetaDiff'),
    src_cumulative = cms.InputTag('diTauCandidateForAHtoMuTauPzetaDiffCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoMuTauPzetaDiffCut', 'individual')
)

# "final" selection of di-tau candidates for "OppositeSign" signal region
evtSelDiTauCandidateForAHtoMuTauZeroCharge = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForAHtoMuTauZeroCharge'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForAHtoMuTauZeroChargeCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoMuTauZeroChargeCut', 'individual'),
    systematics = cms.vstring(muTauPairSystematics.keys())
)

# "final" selection of di-tau candidates for "SameSign" background dominated control region
evtSelDiTauCandidateForAHtoMuTauNonZeroCharge = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForAHtoMuTauNonZeroCharge'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForAHtoMuTauNonZeroChargeCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoMuTauNonZeroChargeCut', 'individual'),
    systematics = cms.vstring(muTauPairSystematics.keys())
)

# central jet veto/b-jet candidate selection
evtSelNonCentralJetEt20bTag = cms.PSet(
    pluginName = cms.string('evtSelNonCentralJetEt20bTag'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('centralJetEt20bTagVeto', 'cumulative'),
    src_individual = cms.InputTag('centralJetEt20bTagVeto', 'individual'),
    systematics = cms.vstring(jetSystematics.keys())
)
evtSelCentralJetEt20 = cms.PSet(
    pluginName = cms.string('evtSelCentralJetEt20'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('centralJetEt20Cut', 'cumulative'),
    src_individual = cms.InputTag('centralJetEt20Cut', 'individual'),
    systematics = cms.vstring(jetSystematics.keys())
)
evtSelCentralJetEt20bTag = cms.PSet(
    pluginName = cms.string('evtSelCentralJetEt20bTag'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('centralJetEt20bTagCut', 'cumulative'),
    src_individual = cms.InputTag('centralJetEt20bTagCut', 'individual'),
    systematics = cms.vstring(jetSystematics.keys())
)

# VBF event selection
evtSelVBFtag = cms.PSet(
    pluginName = cms.string('evtSelVBFtag'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('vbfTagCut', 'cumulative'),
    src_individual = cms.InputTag('vbfTagCut', 'individual'),
    #systematics = cms.vstring(muTauPairVBFEventSystematics.keys())
)
evtSelVBFdEta35 = cms.PSet(
    pluginName = cms.string('evtSelVBFdEta35'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('vbfDEta35Cut', 'cumulative'),
    src_individual = cms.InputTag('vbfDEta35Cut', 'individual'),
    #systematics = cms.vstring(muTauPairVBFEventSystematics.keys())
)
evtSelVBFmass350 = cms.PSet(
    pluginName = cms.string('evtSelVBFmass350'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('vbfMass350Cut', 'cumulative'),
    src_individual = cms.InputTag('vbfMass350Cut', 'individual'),
    #systematics = cms.vstring(muTauPairVBFEventSystematics.keys())
)
evtSel3rdTagJetVeto = cms.PSet(
    pluginName = cms.string('evtSel3rdTagJetVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('vbf3rdTagJetVeto'),
    #systematics = cms.vstring(muTauPairVBFEventSystematics.keys())
)

#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

muTauEventDump_woBtag = muTauEventDump.clone(
    diTauCandidateSource = cms.InputTag('selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative'),
    muTauZmumuHypothesisSource = cms.InputTag('muTauPairZmumuHypothesesForAHtoMuTau'),
    triggerConditions = cms.vstring("evtSelNonCentralJetEt20bTag: passed_cumulative")
)

patTauDebugEventDump = cms.PSet(
    pluginName = cms.string('patTauDebugEventDump'),
    pluginType = cms.string('PATTauDebugEventDump'),

    src = cms.InputTag('selectedPatTausForMuTauLeadTrkPtCumulative'),

    #values = cms.vstring("tauID('pt_index')"),
    values = cms.vstring("decayMode()"),

    separator = cms.string(":"),

    output = cms.string("patTauDebugEventDump.txt"),

    triggerConditions = cms.vstring("evtSelTauLeadTrkPt: passed_cumulative")
)

muTauEventDump_wBtag = muTauEventDump.clone(
    diTauCandidateSource = cms.InputTag('selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative'),
    muTauZmumuHypothesisSource = cms.InputTag('muTauPairZmumuHypothesesForAHtoMuTau'),
    triggerConditions = cms.vstring("evtSelCentralJetEt20bTag: passed_cumulative")
)

muTauEventDump_wVBFtag = muTauEventDump.clone(
    diTauCandidateSource = cms.InputTag('selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative'),
    muTauZmumuHypothesisSource = cms.InputTag('muTauPairZmumuHypothesesForAHtoMuTau'),
    triggerConditions = cms.vstring("evtSel3rdTagJetVeto: passed_cumulative")
)

#--------------------------------------------------------------------------------
# define analysis sequences
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

muTauAnalysisSequenceOS_woBtag = cms.VPSet(
    # fill histograms for full event sample
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',            
            'muonHistManager',
            'tauHistManager',
            'triggerHistManagerForMuTau'
        )
    ),

    # generator level phase-space selection
    #
    # NOTE:
    #     (1) to be used in case of Monte Carlo samples
    #         overlapping in simulated phase-space only !!
    #     (2) binning objects for computation of signal acceptance and systematic uncertainties
    #         need to be filled at this stage of the analysis sequence,
    #         so that the number of generator level events within detector acceptance get counted
    #         regardless of whether the event passes or fails the final event selection on reconstruction level !!
    #
    cms.PSet(
        filter = cms.string('genPhaseSpaceCut'),
        title = cms.string('gen. Phase-Space'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau',
            'modelBinnerForMuTauGenTauLeptonPairAcc',
            'modelBinnerForMuTauCentralJetVetoWrtGenTauLeptonPairAcc',
            'sysUncertaintyBinnerForMuTauAccCentralJetVeto'
        )
    ),

    # trigger selection
    cms.PSet(
        filter = cms.string('evtSelTrigger'),
        title = cms.string('Muon Trigger'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        )
    ),

    # data-quality selection
    cms.PSet(
        filter = cms.string('evtSelDataQuality'),
        title = cms.string('Data quality'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        )
    ),

    # muon acceptance cuts
    cms.PSet(
        filter = cms.string('evtSelGlobalMuon'),
        title = cms.string('global Muon'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'triggerHistManagerForMuTau'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsGlobalCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonEta'),
        title = cms.string('-2.1 < eta(Muon) < +2.1'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsEta21Cumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonPt'),
        title = cms.string('Pt(Muon) > 15 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsPt15Cumulative')
    ),

    # tau acceptance cuts
    cms.PSet(
        filter = cms.string('evtSelTauAntiOverlapWithMuonsVeto'),
        title = cms.string('Tau not overlapping w. Muon'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsPt15Cumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauAntiOverlapWithMuonsVetoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauEta'),
        title = cms.string('-2.3 < eta(Tau) < +2.3'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
           'muonHistManager.muonSource = selectedPatMuonsPt15Cumulative',
           'tauHistManager.tauSource = selectedPatTausForMuTauEta23Cumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauPt'),
        title = cms.string('Pt(Tau) > 20 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'triggerHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsPt15Cumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauPt20Cumulative'
        )
    ),

    # selection of muon candidate (isolation & id.)
    # produced in muonic tau decay
    cms.PSet(
        filter = cms.string('evtSelMuonVbTfId'),
        title = cms.string('Muon VBTF id.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsVbTfIdCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonPFRelIso'),
        title = cms.string('Muon iso.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsPFRelIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonTrkIP'),
        title = cms.string('Muon Track IP'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative')
    ),

    # selection of tau-jet candidate (id.)
    # produced in hadronic tau decay
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrk'),
        title = cms.string('Tau lead. Track find.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauLeadTrkCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrkPt'),
        title = cms.string('Tau lead. Track Pt'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauLeadTrkPtCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauTaNCdiscr'),
        title = cms.string('Tau TaNC discr.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauTaNCdiscrCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauProng'),
        title = cms.string('Tau 1||3-Prong'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauProngCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauCharge'),
        title = cms.string('Charge(Tau) = +/-1'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauChargeCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauMuonVeto'),
        title = cms.string('Tau mu-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauChargeCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauElectronVeto'),
        title = cms.string('Tau e-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative'
        )
    ),

    # selection of muon + tau-jet combinations
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauAntiOverlapVeto'),
        title = cms.string('dR(Muon-Tau) > 0.7'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'vertexHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauAntiOverlapVetoCumulative'
        )
    ),

    # primary event vertex selection
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexForMuTau'),
        title = cms.string('common Muon+Tau Vertex'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'vertexHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauAntiOverlapVetoCumulative',
            'vertexHistManager.vertexSource = selectedPrimaryVertexForMuTau'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexQualityForMuTau'),
        title = cms.string('Vertex quality'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'vertexHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauAntiOverlapVetoCumulative',
            'vertexHistManager.vertexSource = selectedPrimaryVertexQualityForMuTau'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexPositionForMuTau'),
        title = cms.string('-24 < zVertex < +24 cm'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'vertexHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauAntiOverlapVetoCumulative',
            'vertexHistManager.vertexSource = selectedPrimaryVertexPositionForMuTau'
        )
    ),

    # selection of muon + tau-jet combinations (continued)
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauMt1MET'),
        title = cms.string('M_{T}(Muon-MET) < 40 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'vertexHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauMt1METcumulative',
            'vertexHistManager.vertexSource = selectedPrimaryVertexHighestPtTrackSumForMuTau'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauPzetaDiff'),
        title = cms.string('P_{#zeta} - 1.5*P_{#zeta}^{vis} > -20 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'muPairHistManagerZmumuHypothesesByLooseIsolation'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'muPairHistManagerZmumuHypothesesByLooseIsolation.diTauCandidateSource = allDiMuPairZmumuHypothesesByLooseIsolation'
        )
    ),

    # veto events compatible with Z/gamma* --> mu+ mu- hypothesis
    cms.PSet(
        filter = cms.string('evtSelDiMuPairZmumuHypothesisVetoByLooseIsolation'),
        title = cms.string('not Charge(isoMuon+isoMuon) = 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau',
            'muPairHistManagerZmumuHypothesesByLooseIsolation',
            'muPairHistManagerDYmumuHypotheses',
            'jetHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'diTauCandidateNSVfitHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'muPairHistManagerZmumuHypothesesByLooseIsolation.diTauCandidateSource = selectedDiMuPairZmumuHypothesesByLooseIsolation',
            'jetHistManager.jetSource = selectedPatJetsForAHtoMuTauAntiOverlapWithLeptonsVetoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelDiMuPairDYmumuHypothesisVeto'),
        title = cms.string('not Charge(isoMuon+nearbyMuon) = 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau',
            'muPairHistManagerDYmumuHypotheses'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'diTauCandidateHistManagerForMuTau.visMassHypothesisSource = muTauPairVisMassHypotheses',
            'diTauCandidateNSVfitHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'muPairHistManagerDYmumuHypotheses.diTauCandidateSource = selectedDiMuPairDYmumuHypotheses'            
        )
    ),

    # veto events containing b-tagged jets with Et > 20 GeV
    # in order to avoid overlap with "b-tag" analysis path
    cms.PSet(
        filter = cms.string('evtSelNonCentralJetEt20bTag'),
        title = cms.string('no E_{T} > 20 GeV central Jet with b-Tag'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau',
            'jetHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'diTauCandidateHistManagerForMuTau.visMassHypothesisSource = muTauPairVisMassHypothesesForAHtoMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'jetHistManager.jetSource = selectedPatJetsForAHtoMuTauAntiOverlapWithLeptonsVetoCumulative'
        )
    ),

    # apply "final" selection of "OS" di-tau candidates
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauZeroCharge'),
        title = cms.string('Charge(Muon+Tau) = 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'eventWeightHistManager',
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau',
            'diTauLeg1ChargeBinGridHistManager',
            'vertexHistManager',
            'muPairHistManagerZmumuHypothesesByLooseIsolation',
            'muPairHistManagerDYmumuHypotheses',
            'jetHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'particleMultiplicityHistManager',          
            'triggerHistManagerForMuTau',
            'sysUncertaintyHistManagerForMuTau',
            'dataBinner',
            'sysUncertaintyBinnerForMuTauEff'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauZeroChargeCumulative',
            'diTauCandidateHistManagerForMuTau.visMassHypothesisSource = muTauPairVisMassHypothesesForAHtoMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauZeroChargeCumulative',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau.ZllHypothesisSource = muTauPairZmumuHypothesesForAHtoMuTau',
            'vertexHistManager.vertexSource = selectedPrimaryVertexHighestPtTrackSumForMuTau',
            'muPairHistManagerZmumuHypothesesByLooseIsolation.diTauCandidateSource = selectedDiMuPairZmumuHypothesesByLooseIsolation',
            'muPairHistManagerDYmumuHypotheses.diTauCandidateSource = selectedDiMuPairDYmumuHypotheses',
            'jetHistManager.jetSource = selectedPatJetsForAHtoMuTauAntiOverlapWithLeptonsVetoCumulative'
        )
    )
)

muTauAnalysisSequenceOS_wBtag = cms.VPSet(
    # fill histograms for full event sample
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'muonHistManager',
            'tauHistManager',
            'triggerHistManagerForMuTau'
        )
    ),

    # generator level phase-space selection
    #
    # NOTE:
    #     (1) to be used in case of Monte Carlo samples
    #         overlapping in simulated phase-space only !!
    #     (2) binning objects for computation of signal acceptance and systematic uncertainties
    #         need to be filled at this stage of the analysis sequence,
    #         so that the number of generator level events within detector acceptance get counted
    #         regardless of whether the event passes or fails the final event selection on reconstruction level !!
    #
    cms.PSet(
        filter = cms.string('genPhaseSpaceCut'),
        title = cms.string('gen. Phase-Space'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau',
            'modelBinnerForMuTauGenTauLeptonPairAcc',
            'modelBinnerForMuTauCentralJetBtagWrtGenTauLeptonPairAcc',
            'sysUncertaintyBinnerForMuTauAccCentralJetBtag'
        )
    ),

    # trigger selection
    cms.PSet(
        filter = cms.string('evtSelTrigger'),
        title = cms.string('Muon Trigger'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        )
    ),

    # data-quality selection
    cms.PSet(
        filter = cms.string('evtSelDataQuality'),
        title = cms.string('Data quality'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        )
    ),

    # muon acceptance cuts
    cms.PSet(
        filter = cms.string('evtSelGlobalMuon'),
        title = cms.string('global Muon'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'triggerHistManagerForMuTau'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsGlobalCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonEta'),
        title = cms.string('-2.1 < eta(Muon) < +2.1'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsEta21Cumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonPt'),
        title = cms.string('Pt(Muon) > 15 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsPt15Cumulative')
    ),

    # tau acceptance cuts
    cms.PSet(
        filter = cms.string('evtSelTauAntiOverlapWithMuonsVeto'),
        title = cms.string('Tau not overlapping w. Muon'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsPt15Cumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauAntiOverlapWithMuonsVetoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauEta'),
        title = cms.string('-2.3 < eta(Tau) < +2.3'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
           'muonHistManager.muonSource = selectedPatMuonsPt15Cumulative',
           'tauHistManager.tauSource = selectedPatTausForMuTauEta23Cumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauPt'),
        title = cms.string('Pt(Tau) > 20 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'triggerHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsPt15Cumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauPt20Cumulative'
        )
    ),

    # selection of muon candidate (isolation & id.)
    # produced in muonic tau decay
    cms.PSet(
        filter = cms.string('evtSelMuonVbTfId'),
        title = cms.string('Muon VBTF id.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsVbTfIdCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonPFRelIso'),
        title = cms.string('Muon iso.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsPFRelIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonTrkIP'),
        title = cms.string('Muon Track IP'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative')
    ),

    # selection of tau-jet candidate (id.)
    # produced in hadronic tau decay
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrk'),
        title = cms.string('Tau lead. Track find.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauLeadTrkCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrkPt'),
        title = cms.string('Tau lead. Track Pt'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauLeadTrkPtCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauTaNCdiscr'),
        title = cms.string('Tau TaNC discr.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauTaNCdiscrCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauProng'),
        title = cms.string('Tau 1||3-Prong'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauProngCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauCharge'),
        title = cms.string('Charge(Tau) = +/-1'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauChargeCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauMuonVeto'),
        title = cms.string('Tau mu-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
     cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauCaloMuonVetoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauElectronVeto'),
        title = cms.string('Tau e-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative'
        )
    ),

    # selection of muon + tau-jet combinations
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauAntiOverlapVeto'),
        title = cms.string('dR(Muon-Tau) > 0.7'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'vertexHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauAntiOverlapVetoCumulative'
        )
    ),

    # primary event vertex selection
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexForMuTau'),
        title = cms.string('common Muon+Tau Vertex'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'vertexHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauAntiOverlapVetoCumulative',
            'vertexHistManager.vertexSource = selectedPrimaryVertexForMuTau'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexQualityForMuTau'),
        title = cms.string('Vertex quality'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'vertexHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauAntiOverlapVetoCumulative',
            'vertexHistManager.vertexSource = selectedPrimaryVertexQualityForMuTau'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexPositionForMuTau'),
        title = cms.string('-24 < zVertex < +24 cm'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'vertexHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauAntiOverlapVetoCumulative',
            'vertexHistManager.vertexSource = selectedPrimaryVertexPositionForMuTau'
        )
    ),

    # selection of muon + tau-jet combinations (continued)
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauMt1MET'),
        title = cms.string('M_{T}(Muon-MET) < 40 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'vertexHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauMt1METcumulative',
            'vertexHistManager.vertexSource = selectedPrimaryVertexHighestPtTrackSumForMuTau'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauPzetaDiff'),
        title = cms.string('P_{#zeta} - 1.5*P_{#zeta}^{vis} > -20 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'muPairHistManagerZmumuHypothesesByLooseIsolation'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'muPairHistManagerZmumuHypothesesByLooseIsolation.diTauCandidateSource = allDiMuPairZmumuHypothesesByLooseIsolation'
        )
    ),

    # veto events compatible with Z/gamma* --> mu+ mu- hypothesis
    cms.PSet(
        filter = cms.string('evtSelDiMuPairZmumuHypothesisVetoByLooseIsolation'),
        title = cms.string('not Charge(isoMuon+isoMuon) = 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau',
            'muPairHistManagerZmumuHypothesesByLooseIsolation',
            'muPairHistManagerDYmumuHypotheses',
            'jetHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'diTauCandidateNSVfitHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'muPairHistManagerByLooseIsolation.diTauCandidateSource = selectedDiMuPairZmumuHypothesesByLooseIsolation',
            'jetHistManager.jetSource = selectedPatJetsForAHtoMuTauAntiOverlapWithLeptonsVetoCumulative'
        )
    ),

    # require at least one b-tagged jet with Et > 20 GeV in the event
    cms.PSet(
        filter = cms.string('evtSelCentralJetEt20'),
        title = cms.string('E_{T} > 20 GeV central Jet'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'jetHistManager',
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'diTauCandidateHistManagerForMuTau.visMassHypothesisSource = muTauPairVisMassHypothesesForAHtoMuTau',
            'jetHistManager.jetSource = selectedPatJetsForAHtoMuTauAntiOverlapWithLeptonsVetoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelCentralJetEt20bTag'),
        title = cms.string('E_{T} > 20 GeV central Jet with b-Tag'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau',
            'jetHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'diTauCandidateHistManagerForMuTau.visMassHypothesisSource = muTauPairVisMassHypothesesForAHtoMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'jetHistManager.jetSource = selectedPatJetsForAHtoMuTauBtagCumulative'
        )
    ),

    # apply "final" selection of "OS" di-tau candidates
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauZeroCharge'),
        title = cms.string('Charge(Muon+Tau) = 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'eventWeightHistManager',
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau',
            'diTauLeg1ChargeBinGridHistManager',
            'vertexHistManager',
            'muPairHistManagerZmumuHypothesesByLooseIsolation',
            'muPairHistManagerDYmumuHypotheses',
            'jetHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'particleMultiplicityHistManager',
            'triggerHistManagerForMuTau',
            'sysUncertaintyHistManagerForMuTau',
            'dataBinner',
            'sysUncertaintyBinnerForMuTauEff'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauZeroChargeCumulative',
            'diTauCandidateHistManagerForMuTau.visMassHypothesisSource = muTauPairVisMassHypothesesForAHtoMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauZeroChargeCumulative',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau.ZllHypothesisSource = muTauPairZmumuHypothesesForAHtoMuTau',
            'vertexHistManager.vertexSource = selectedPrimaryVertexHighestPtTrackSumForMuTau',
            'muPairHistManagerZmumuHypothesesByLooseIsolation.diTauCandidateSource = selectedDiMuPairZmumuHypothesesByLooseIsolation',
            'muPairHistManagerDYmumuHypotheses.diTauCandidateSource = selectedDiMuPairDYmumuHypotheses',
            'jetHistManager.jetSource = selectedPatJetsForAHtoMuTauBtagCumulative'
        )
    )
)

muTauAnalysisSequenceOS_wVBFtag = cms.VPSet(
    # fill histograms for full event sample
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'muonHistManager',
            'tauHistManager',
            'triggerHistManagerForMuTau'
        )
    ),

    # generator level phase-space selection
    #
    # NOTE:
    #     (1) to be used in case of Monte Carlo samples
    #         overlapping in simulated phase-space only !!
    #     (2) binning objects for computation of signal acceptance and systematic uncertainties
    #         need to be filled at this stage of the analysis sequence,
    #         so that the number of generator level events within detector acceptance get counted
    #         regardless of whether the event passes or fails the final event selection on reconstruction level !!
    #
    cms.PSet(
        filter = cms.string('genPhaseSpaceCut'),
        title = cms.string('gen. Phase-Space'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau',
            'modelBinnerForMuTauGenTauLeptonPairAcc',
            'modelBinnerForMuTauCentralJetBtagWrtGenTauLeptonPairAcc',
            'sysUncertaintyBinnerForMuTauAccCentralJetBtag'
        )
    ),

    # trigger selection
    cms.PSet(
        filter = cms.string('evtSelTrigger'),
        title = cms.string('Muon Trigger'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        )
    ),

    # data-quality selection
    cms.PSet(
        filter = cms.string('evtSelDataQuality'),
        title = cms.string('Data quality'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        )
    ),

    # muon acceptance cuts
    cms.PSet(
        filter = cms.string('evtSelGlobalMuon'),
        title = cms.string('global Muon'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'triggerHistManagerForMuTau'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsGlobalCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonEta'),
        title = cms.string('-2.1 < eta(Muon) < +2.1'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsEta21Cumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonPt'),
        title = cms.string('Pt(Muon) > 15 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsPt15Cumulative')
    ),

    # tau acceptance cuts
    cms.PSet(
        filter = cms.string('evtSelTauAntiOverlapWithMuonsVeto'),
        title = cms.string('Tau not overlapping w. Muon'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsPt15Cumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauAntiOverlapWithMuonsVetoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauEta'),
        title = cms.string('-2.3 < eta(Tau) < +2.3'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
           'muonHistManager.muonSource = selectedPatMuonsPt15Cumulative',
           'tauHistManager.tauSource = selectedPatTausForMuTauEta23Cumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauPt'),
        title = cms.string('Pt(Tau) > 20 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'triggerHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsPt15Cumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauPt20Cumulative'
        )
    ),

    # selection of muon candidate (isolation & id.)
    # produced in muonic tau decay
    cms.PSet(
        filter = cms.string('evtSelMuonVbTfId'),
        title = cms.string('Muon VBTF id.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsVbTfIdCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonPFRelIso'),
        title = cms.string('Muon iso.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsPFRelIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonTrkIP'),
        title = cms.string('Muon Track IP'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative')
    ),

    # selection of tau-jet candidate (id.)
    # produced in hadronic tau decay
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrk'),
        title = cms.string('Tau lead. Track find.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauLeadTrkCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrkPt'),
        title = cms.string('Tau lead. Track Pt'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauLeadTrkPtCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauTaNCdiscr'),
        title = cms.string('Tau TaNC discr.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauTaNCdiscrCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauProng'),
        title = cms.string('Tau 1||3-Prong'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauProngCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauCharge'),
        title = cms.string('Charge(Tau) = +/-1'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauChargeCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauMuonVeto'),
        title = cms.string('Tau mu-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
     cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauCaloMuonVetoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauElectronVeto'),
        title = cms.string('Tau e-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative'
        )
    ),

    # selection of muon + tau-jet combinations
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauAntiOverlapVeto'),
        title = cms.string('dR(Muon-Tau) > 0.7'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'vertexHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauAntiOverlapVetoCumulative'
        )
    ),

    # primary event vertex selection
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexForMuTau'),
        title = cms.string('common Muon+Tau Vertex'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'vertexHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauAntiOverlapVetoCumulative',
            'vertexHistManager.vertexSource = selectedPrimaryVertexForMuTau'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexQualityForMuTau'),
        title = cms.string('Vertex quality'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'vertexHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauAntiOverlapVetoCumulative',
            'vertexHistManager.vertexSource = selectedPrimaryVertexQualityForMuTau'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexPositionForMuTau'),
        title = cms.string('-24 < zVertex < +24 cm'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'vertexHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauAntiOverlapVetoCumulative',
            'vertexHistManager.vertexSource = selectedPrimaryVertexPositionForMuTau'
        )
    ),

    # selection of muon + tau-jet combinations (continued)
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauMt1MET'),
        title = cms.string('M_{T}(Muon-MET) < 40 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'vertexHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauMt1METcumulative',
            'vertexHistManager.vertexSource = selectedPrimaryVertexHighestPtTrackSumForMuTau'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauPzetaDiff'),
        title = cms.string('P_{#zeta} - 1.5*P_{#zeta}^{vis} > -20 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'muPairHistManagerZmumuHypothesesByLooseIsolation'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'muPairHistManagerZmumuHypothesesByLooseIsolation.diTauCandidateSource = allDiMuPairZmumuHypothesesByLooseIsolation'
        )
    ),

    # veto events compatible with Z/gamma* --> mu+ mu- hypothesis
    cms.PSet(
        filter = cms.string('evtSelDiMuPairZmumuHypothesisVetoByLooseIsolation'),
        title = cms.string('not Charge(isoMuon+isoMuon) = 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau',
            'muPairHistManagerZmumuHypothesesByLooseIsolation',
            'muPairHistManagerDYmumuHypotheses',
            'jetHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'diTauCandidateNSVfitHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'muPairHistManagerByLooseIsolation.diTauCandidateSource = selectedDiMuPairZmumuHypothesesByLooseIsolation',
            'jetHistManager.jetSource = selectedPatJetsForAHtoMuTauAntiOverlapWithLeptonsVetoCumulative'
        )
    ),

    # require at least one combination of Et > 30 GeV forward/backward jets
    cms.PSet(
        filter = cms.string('evtSelVBFtag'),
        title = cms.string('VBF tag'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'vbfEventHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'diTauCandidateHistManagerForMuTau.visMassHypothesisSource = muTauPairVisMassHypothesesForAHtoMuTau',
            'vbfEventHistManagerForMuTau.vbfEventSource = selectedVBFEventHypothesesForAHtoMuTauTagJetOpposHemisphereCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelVBFdEta35'),
        title = cms.string('dEta(tagJet1,tagJet2) > 3.5'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau',
            'vbfEventHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'diTauCandidateHistManagerForMuTau.visMassHypothesisSource = muTauPairVisMassHypothesesForAHtoMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'vbfEventHistManagerForMuTau.vbfEventSource = selectedVBFEventHypothesesForAHtoMuTauTagJetDEta35Cumulative'            
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelVBFmass350'),
        title = cms.string('M(tagJet1,tagJet2) > 350 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau',
            'vbfEventHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'diTauCandidateHistManagerForMuTau.visMassHypothesisSource = muTauPairVisMassHypothesesForAHtoMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'vbfEventHistManagerForMuTau.vbfEventSource = selectedVBFEventHypothesesForAHtoMuTauTagJetMass350Cumulative'            
        )
    ),
    cms.PSet(
        filter = cms.string('evtSel3rdTagJetVeto'),
        title = cms.string('no 3rd E_{T} > 30 GeV Jet'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau',
            'vbfEventHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'diTauCandidateHistManagerForMuTau.visMassHypothesisSource = muTauPairVisMassHypothesesForAHtoMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'vbfEventHistManagerForMuTau.vbfEventSource = selectedVBFEventHypothesesForAHtoMuTauTagJetMass350Cumulative'            
        )
    ),

    # apply "final" selection of "OS" di-tau candidates
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauZeroCharge'),
        title = cms.string('Charge(Muon+Tau) = 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'eventWeightHistManager',
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau',
            'diTauLeg1ChargeBinGridHistManager',
            'vertexHistManager',
            'muPairHistManagerZmumuHypothesesByLooseIsolation',
            'muPairHistManagerDYmumuHypotheses',
            'vbfEventHistManagerForMuTau',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'particleMultiplicityHistManager',
            'triggerHistManagerForMuTau',
            'sysUncertaintyHistManagerForMuTau',
            'dataBinner',
            'sysUncertaintyBinnerForMuTauEff'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauZeroChargeCumulative',
            'diTauCandidateHistManagerForMuTau.visMassHypothesisSource = muTauPairVisMassHypothesesForAHtoMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauZeroChargeCumulative',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau.ZllHypothesisSource = muTauPairZmumuHypothesesForAHtoMuTau',
            'vertexHistManager.vertexSource = selectedPrimaryVertexHighestPtTrackSumForMuTau',
            'muPairHistManagerZmumuHypothesesByLooseIsolation.diTauCandidateSource = selectedDiMuPairZmumuHypothesesByLooseIsolation',
            'muPairHistManagerDYmumuHypotheses.diTauCandidateSource = selectedDiMuPairDYmumuHypotheses',
            'vbfEventHistManagerForMuTau.vbfEventSource = selectedVBFEventHypothesesForAHtoMuTauTagJetMass350Cumulative' 
        )
    )
)

muTauAnalysisSequenceSS_woBtag = cms.VPSet(
    cms.PSet(
        filter = cms.string('genPhaseSpaceCut'),
        title = cms.string('gen. Phase-Space'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTrigger'),
        title = cms.string('Muon Trigger'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDataQuality'),
        title = cms.string('Data quality'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelGlobalMuon'),
        title = cms.string('global Muon'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonEta'),
        title = cms.string('-2.1 < eta(Muon) < +2.1'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonPt'),
        title = cms.string('Pt(Muon) > 15 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauAntiOverlapWithMuonsVeto'),
        title = cms.string('Tau not overlapping w. Muon'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauEta'),
        title = cms.string('-2.3 < eta(Tau) < +2.3'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauPt'),
        title = cms.string('Pt(Tau) > 20 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonVbTfId'),
        title = cms.string('Muon VBTF id.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonPFRelIso'),
        title = cms.string('Muon iso.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonTrkIP'),
        title = cms.string('Muon Track IP'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrk'),
        title = cms.string('Tau lead. Track find.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrkPt'),
        title = cms.string('Tau lead. Track Pt'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauTaNCdiscr'),
        title = cms.string('Tau TaNC discr.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauProng'),
        title = cms.string('Tau 1||3-Prong'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauCharge'),
        title = cms.string('Charge(Tau) = +/-1'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauMuonVeto'),
        title = cms.string('Tau mu-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauElectronVeto'),
        title = cms.string('Tau e-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauAntiOverlapVeto'),
        title = cms.string('dR(Muon-Tau) > 0.7'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexForMuTau'),
        title = cms.string('common Muon+Tau Vertex'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexQualityForMuTau'),
        title = cms.string('Vertex quality'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexPositionForMuTau'),
        title = cms.string('-24 < zVertex < +24 cm'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauMt1MET'),
        title = cms.string('M_{T}(Muon-MET) < 40 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauPzetaDiff'),
        title = cms.string('P_{#zeta} - 1.5*P_{#zeta}^{vis} > -20 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiMuPairZmumuHypothesisVetoByLooseIsolation'),
        title = cms.string('not Charge(isoMuon+isoMuon) = 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiMuPairDYmumuHypothesisVeto'),
        title = cms.string('not Charge(isoMuon+nearbyMuon) = 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelNonCentralJetEt20bTag'),
        title = cms.string('no E_{T} > 20 GeV central Jet with b-Tag'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauNonZeroCharge'),
        title = cms.string('Charge(Muon+Tau) != 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau',
            'diTauLeg1ChargeBinGridHistManager',
            'vertexHistManager',
            'muPairHistManagerZmumuHypothesesByLooseIsolation',
            'muPairHistManagerDYmumuHypotheses',
            'jetHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'triggerHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauNonZeroChargeCumulative',
            'diTauCandidateHistManagerForMuTau.visMassHypothesisSource = muTauPairVisMassHypothesesForAHtoMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauNonZeroChargeCumulative',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau.ZllHypothesisSource = muTauPairZmumuHypothesesForAHtoMuTau',
            'vertexHistManager.vertexSource = selectedPrimaryVertexHighestPtTrackSumForMuTau',
            'muPairHistManagerZmumuHypothesesByLooseIsolation.diTauCandidateSource = selectedDiMuPairZmumuHypothesesByLooseIsolation',
            'muPairHistManagerDYmumuHypotheses.diTauCandidateSource = selectedDiMuPairDYmumuHypotheses',
            'jetHistManager.jetSource = selectedPatJetsForAHtoMuTauAntiOverlapWithLeptonsVetoCumulative'
        )
    )
)

muTauAnalysisSequenceSS_wBtag = cms.VPSet(
    cms.PSet(
        filter = cms.string('genPhaseSpaceCut'),
        title = cms.string('gen. Phase-Space'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTrigger'),
        title = cms.string('Muon Trigger'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDataQuality'),
        title = cms.string('Data quality'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelGlobalMuon'),
        title = cms.string('global Muon'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonEta'),
        title = cms.string('-2.1 < eta(Muon) < +2.1'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonPt'),
        title = cms.string('Pt(Muon) > 15 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauAntiOverlapWithMuonsVeto'),
        title = cms.string('Tau not overlapping w. Muon'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauEta'),
        title = cms.string('-2.3 < eta(Tau) < +2.3'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauPt'),
        title = cms.string('Pt(Tau) > 20 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonVbTfId'),
        title = cms.string('Muon VBTF id.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonPFRelIso'),
        title = cms.string('Muon iso.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonTrkIP'),
        title = cms.string('Muon Track IP'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrk'),
        title = cms.string('Tau lead. Track find.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrkPt'),
        title = cms.string('Tau lead. Track Pt'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauTaNCdiscr'),
        title = cms.string('Tau TaNC discr.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauProng'),
        title = cms.string('Tau 1||3-Prong'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauCharge'),
        title = cms.string('Charge(Tau) = +/-1'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauMuonVeto'),
        title = cms.string('Tau mu-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauElectronVeto'),
        title = cms.string('Tau e-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauAntiOverlapVeto'),
        title = cms.string('dR(Muon-Tau) > 0.7'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexForMuTau'),
        title = cms.string('common Muon+Tau Vertex'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexQualityForMuTau'),
        title = cms.string('Vertex quality'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexPositionForMuTau'),
        title = cms.string('-24 < zVertex < +24 cm'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauMt1MET'),
        title = cms.string('M_{T}(Muon-MET) < 40 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauPzetaDiff'),
        title = cms.string('P_{#zeta} - 1.5*P_{#zeta}^{vis} > -20 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiMuPairZmumuHypothesisVetoByLooseIsolation'),
        title = cms.string('not Charge(isoMuon+isoMuon) = 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiMuPairDYmumuHypothesisVeto'),
        title = cms.string('not Charge(isoMuon+nearbyMuon) = 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelCentralJetEt20'),
        title = cms.string('E_{T} > 20 GeV central Jet'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelCentralJetEt20bTag'),
        title = cms.string('E_{T} > 20 GeV central Jet with b-Tag'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauNonZeroCharge'),
        title = cms.string('Charge(Muon+Tau) != 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau',
            'vertexHistManager',
            'muPairHistManagerZmumuHypothesesByLooseIsolation',
            'muPairHistManagerDYmumuHypotheses',
            'jetHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'triggerHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauNonZeroChargeCumulative',
            'diTauCandidateHistManagerForMuTau.visMassHypothesisSource = muTauPairVisMassHypothesesForAHtoMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauNonZeroChargeCumulative',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau.ZllHypothesisSource = muTauPairZmumuHypothesesForAHtoMuTau',
            'vertexHistManager.vertexSource = selectedPrimaryVertexHighestPtTrackSumForMuTau',
            'muPairHistManagerZmumuHypothesesByLooseIsolation.diTauCandidateSource = selectedDiMuPairZmumuHypothesesByLooseIsolation',
            'muPairHistManagerDYmumuHypotheses.diTauCandidateSource = selectedDiMuPairDYmumuHypotheses',
            'jetHistManager.jetSource = selectedPatJetsForAHtoMuTauBtagCumulative'
        )
    )
)

