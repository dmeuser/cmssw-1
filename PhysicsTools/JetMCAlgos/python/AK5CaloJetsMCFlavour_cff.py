import FWCore.ParameterSet.Config as cms

# Flavour byReference
AK5byRef = cms.EDFilter("JetPartonMatcher",
    jets = cms.InputTag("ak5CaloJets"),
    coneSizeToAssociate = cms.double(0.3),
    partons = cms.InputTag("myPartons")
)

# Flavour byValue PhysDef
AK5byValPhys = cms.EDFilter("JetFlavourIdentifier",
    srcByReference = cms.InputTag("AK5byRef"),
    physicsDefinition = cms.bool(True)
)

# Flavour byValue AlgoDef
AK5byValAlgo = cms.EDFilter("JetFlavourIdentifier",
    srcByReference = cms.InputTag("AK5byRef"),
    physicsDefinition = cms.bool(False)
)


