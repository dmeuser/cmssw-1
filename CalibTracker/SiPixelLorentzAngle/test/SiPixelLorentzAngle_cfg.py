import FWCore.ParameterSet.Config as cms

process = cms.Process("LA")

process.load("Configuration.StandardSequences.Services_cff")

#process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration/StandardSequences/GeometryExtended_cff')

#process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')

# process.load("Configuration.StandardSequences.FakeConditions_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
# check for the correct tag on https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions
#process.GlobalTag.globaltag = "GR09_PV7::All"
process.GlobalTag.globaltag = "GR_R_52_V8::All"


process.load("RecoTracker.Configuration.RecoTracker_cff")

from RecoVertex.BeamSpotProducer.BeamSpot_cff import *
process.offlineBeamSpot = offlineBeamSpot 


process.load("RecoTracker/TrackProducer/TrackRefitters_cff")
# put here the tag of the tracks you want to use
# alcareco samples have special names for the tracks, in normal reco samples generalTracks can be used
process.TrackRefitter.src = "generalTracks"
# process.TrackRefitter.src = "ALCARECOTkAlZMuMu"
process.TrackRefitter.TrajectoryInEvent = True
process.load("RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilderWithoutRefit_cfi")



process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('simul', 
        'cout'),
    simul = cms.untracked.PSet(
        threshold = cms.untracked.string('ERROR')
    ),
)

process.lorentzAngle = cms.EDAnalyzer("PixelLorentzAngle",
	src = cms.string("TrackRefitter"),
	fileName = cms.string("lorentzangle.root"),
	fileNameFit	= cms.string("lorentzFit.txt"),
	binsDepth	= cms.int32(50),
	binsDrift =	cms.int32(200),
         # generally used cuts:
	ptMin = cms.double(2.0),#default is 3.0
	#in case of MC set this to true to save the simhits (does not work currently, Mixing Module needs to be included correctly)
	simData = cms.bool(False),
  	normChi2Max = cms.double(3),#default is 2
	clustSizeYMin = cms.int32(3),# default is 4
	residualMax = cms.double(0.01),#default is 0.005
	clustChargeMax = cms.double(200000) #default is 120000
)

process.myout = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('LA_CMSSW.root')
)

process.p = cms.Path(process.offlineBeamSpot*process.TrackRefitter*process.lorentzAngle)

# uncomment this if you want to write out the new CMSSW root file (very large)
# process.outpath = cms.EndPath(process.myout)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

process.source = cms.Source("PoolSource",
	#put here the sample you want to use
    fileNames = cms.untracked.vstring(
    '/store/data/Run2011A/MuOnia/RECO/PromptReco-v1/000/161/312/EC43E47F-DC57-E011-9497-001D09F253D4.root'
	),   
#   skipEvents = cms.untracked.uint32(100) 
)
