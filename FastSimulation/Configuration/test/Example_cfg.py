import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")

# The number of events to be processed.
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
    
# For valgrind studies
# process.ProfilerService = cms.Service("ProfilerService",
#    lastEvent = cms.untracked.int32(13),
#    firstEvent = cms.untracked.int32(3),
#    paths = cms.untracked.vstring('p1')
#)

# Include the RandomNumberGeneratorService definition
process.load("FastSimulation/Configuration/RandomServiceInitialization_cff")

# Generate H -> ZZ -> l+l- l'+l'- (l,l'=e or mu), with mH=180GeV/c2
process.load("FastSimulation/Configuration/HZZllll_cfi")
# Generate ttbar events
#  process.load("FastSimulation/Configuration/data/ttbar.cfi")
# Generate multijet events with different ptHAT bins
#  process.load("FastSimulation/Configuration/data/QCDpt80-120.cfi")
#  process.load("FastSimulation/Configuration/data/QCDpt600-800.cfi")
# Generate Minimum Bias Events
#  process.load("FastSimulation/Configuration/data/MinBiasEvents.cfi")
# Generate muons with a flat pT particle gun, and with pT=10.
# process.load("FastSimulation/Configuration/data/FlatPtMuonGun.cfi")
# replace FlatRandomPtGunSource.PGunParameters.PartID={130}
# Generate di-electrons with pT=35 GeV
# process.load("FastSimulation/Configuration/data/DiElectrons.cfi")

# Famos sequences (Frontier conditions)
process.load("FastSimulation/Configuration/CommonInputsFake_cff")
process.load("FastSimulation/Configuration/FamosSequences_cff")

# Parametrized magnetic field (new mapping, 4.0 and 3.8T)
process.load("Configuration.StandardSequences.MagneticField_40T_cff")
#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True

# If you want to turn on/off pile-up
process.famosPileUp.PileUpSimulator.averageNumber = 5.0    
# You may not want to simulate everything for your study
process.famosSimHits.SimulateCalorimetry = True
process.famosSimHits.SimulateTracking = True
# process.famosSimHits.SimulateMuons = False

# Produce Tracks and Clusters
process.p1 = cms.Path(process.famosWithTracksAndEcalClusters)

# To write out events (not need: FastSimulation _is_ fast!)
process.o1 = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string("MyFirstFamosFile.root"),
    outputCommands = cms.untracked.vstring("keep *",
                                           "drop *_mix_*_*")
    )

process.outpath = cms.EndPath(process.o1)

# Keep the logging output to a nice level #
# process.Timing =  cms.Service("Timing")
# process.load("FWCore/MessageService/MessageLogger_cfi")
# process.MessageLogger.destinations = cms.untracked.vstring("pyDetailedInfo.txt")

