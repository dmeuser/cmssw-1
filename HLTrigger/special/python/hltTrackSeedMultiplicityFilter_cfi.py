import FWCore.ParameterSet.Config as cms

hltTrackSeedMultiplicityFilter = cms.EDFilter( "HLTTrackSeedMultiplicityFilter",
   inputTag    = cms.InputTag( "hltRegionalCosmicTrackerSeeds" ),
   saveTags = cms.bool( False ),
   minSeeds = cms.uint32( 0 ),
   maxSeeds = cms.uint32( 10000 )                                    
)
