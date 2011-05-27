## September 8, 2010, veverka@caltech.edu
import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.trigTools import *

def removeTriggerPathsForAllBut(matchHltPaths, notToRemove):
    """Take a dictionary matchHltPaths of the form
    value: key = collection: HLT path list
    and remove all the HLT path lists (set value = []) for all collections
    that are not listed in 'notToRemove' which is a list of strings
    """
    for key in matchHltPaths.keys():
        if not key in notToRemove:
            matchHltPaths[key] = []


def getTargetBaseName(name):
  'Return e.g. baseName = "electron" for name = "cleanPatElectrons".'
  ## Frist make all lower case and remove trailing "s".
  baseName = name.lower()
  ## Remove various PAT prefixes.
  baseName = baseName.replace("selected", "")
  baseName = baseName.replace("clean", "")
  baseName = baseName.replace("pat", "")
  baseName = baseName.replace("pf", "")
  baseName = baseName.replace("tc", "")
  baseName = baseName.rstrip("s")
  if baseName == "met":
    baseName = "MET"
  else:
    baseName = baseName.title()
  return baseName


def embedTriggerMatches(process, hltPaths):
  """
  Defines default PATTriggerMatchers for all combinations of target collections
  (like cleanPatMuons) and hlt paths (like HLT_Mu9 and HLT_Mu11) defined by the hltPaths
  dictionary (like {"cleanPatMuons" : ["HLT_Mu9", "HLT_Mu11"]}) and tweaks the pat
  trigger settings and sequence to embed these matches.
  """

  # Template trigger match producer
  triggerMatchTemplate = cms.EDProducer( "PATTriggerMatcherDRLessByR",
    src     = cms.InputTag( "dummySource" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( "*" ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( "*" ),
    pathNames      = cms.vstring( "dummyPathName" ),
    collectionTags = cms.vstring( "*" ),
    #maxDPtRel = cms.double( 0.5 ),
    matchedCuts = cms.string('path('')'),
    maxDeltaR = cms.double( 0.3 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( True )
    )

  ## Initialize relevant workflow items
  process.patTriggerEvent.patTriggerMatches = cms.VInputTag()
  if hasattr(process, "patTriggerMatcher"):
    delattr(process, "patTriggerMatcher")
  if hasattr(process, "patTriggerMatchEmbedder"):
    delattr(process, "patTriggerMatchEmbedder")

  for target in hltPaths.keys():
    ## Get the target base name, e.g. "muon" for target = "cleanPatMuons"
    baseName = getTargetBaseName(target)
    ## Get the target trigger match label, e.g. "cleanPatMuonsTriggerMatch"
    triggerMatchLabel = target + "TriggerMatch"
    ## Get the target trigger match module name, e.g. "PATTriggerMatchMuonEmbedder"
    triggerMatchModule = "PATTriggerMatch" + baseName + "Embedder"
    ## Attach the target trigger match module to the process, e.g.
    ##+ process.cleanPatMuonsTriggerMatch = cms.EDProducer("PATTriggerMatchMuonEmbedder",
    ##+   matches = cms.VInputTag(),
    ##+   src = cms.InputTag("cleanPatMuons")
    ##+   )
    setattr(process,
      triggerMatchLabel,
      cms.EDProducer(triggerMatchModule,
        matches = cms.VInputTag(),
        src = cms.InputTag(target),
        )
      )
    targetTriggerMatch = getattr(process, triggerMatchLabel)
    for path in hltPaths[target]:
      ## Get the module name, e.g. "cleanPatMuonsTriggerMatchHLTMu9"
      ##+ for target = "cleanPatMuons" and path = "HLT_Mu9"
      ## L. Gray 27.4.2011 add in change to deal with versioned triggers      
      moduleLabel = baseName.lower() + "TriggerMatch" + path.replace("_", "").replace("v*","")
      setattr(process,
        moduleLabel,
        triggerMatchTemplate.clone(
          src = target,
          pathNames = [path],
          matchedCuts = cms.string('path("'+path+'")'),
          )
        )
      module = getattr(process, moduleLabel)
      process.patTriggerEvent.patTriggerMatches.append(moduleLabel)
      targetTriggerMatch.matches.append(moduleLabel)
      if not hasattr(process, "patTriggerMatcher"):
        process.patTriggerMatcher = cms.Sequence(module)
      else:
        process.patTriggerMatcher += module
    # for path in hltPaths[target]: <-------------------------------------------------
    if not hasattr(process, "patTriggerMatchEmbedder"):
      process.patTriggerMatchEmbedder = cms.Sequence(targetTriggerMatch)
    else:
      process.patTriggerMatchEmbedder += targetTriggerMatch
  # for target in hltPaths.keys(): <-------------------------------------------
  process.patTriggerSequence = cms.Sequence(
    process.patTrigger *
    process.patTriggerMatcher *
    process.patTriggerEvent *
    process.patTriggerMatchEmbedder
  )
# def embedTriggerMatches(process, hltPaths): <--------------------------------

###############################################################################
def addPhotonReReco(process):
    """Include the photon re-reco sequence in the patDefaultSequence
    See https://hypernews.cern.ch/HyperNews/CMS/get/egamma/960.html
    and
    https://hypernews.cern.ch/HyperNews/CMS/get/egamma/958/1/1/1/1/1/1/1/1/1/1/1.html
    """
    process.photonReReco = cms.Sequence(process.ckfTracksFromConversions *
                                        process.conversionSequence *
                                        process.photonSequence *
                                        process.photonIDSequence)

    # Edit the pat sequence to do the rereco
    process.patDefaultSequence = cms.Sequence(process.photonReReco*
                                              process.patDefaultSequence)
# def addPhotonReReco(process): <-----------------------------------------------------


###############################################################################
def addRhoFromFastJet(process, after):
    """Add rho from FastJet for pile-up corrected isolation"""

    ## Load and configure the producer
    process.load('RecoJets.JetProducers.kt4PFJets_cfi')
    process.kt6PFJets = process.kt4PFJets.clone( rParam = 0.6,
                                                 doRhoFastjet = True )
    process.kt6PFJets.Rho_EtaMax = cms.double(2.5)

    ## Append it to the processing sequence after the sequence `after'
    after *= process.kt6PFJets

    ## Add ot the the output
    process.out.outputCommands.append( "keep *_*_rho_*" )
    process.out.outputCommands.append( "keep *_*_sigma_*" )
# def addRhoFromFastJet(process, before): <------------------------------------

###############################################################################
def switchOnDAVertices(process, after):
    """Switch On Deterministic Annealing Vertices"""

    ## Load and configure the producer
    process.load('RecoVertex.PrimaryVertexProducer.OfflinePrimaryVerticesDA_cfi')
    process.offlinePrimaryVertices = process.offlinePrimaryVerticesDA.clone()
    process.offlinePrimaryVertices.TkClusParameters.TkDAClusParameters.Tmin = cms.double(4.)
    process.offlinePrimaryVertices.TkClusParameters.vertexSize = cms.double(0.01)
    after *= process.offlinePrimaryVertices
# def switchOnDAVertices(process, after): <------------------------------------
