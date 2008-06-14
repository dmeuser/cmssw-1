import FWCore.ParameterSet.Config as cms

process = cms.Process("eg")

# The number of events to be processed.
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(300000)
)

# Include the RandomNumberGeneratorService definition
process.load("FastSimulation.Configuration.RandomServiceInitialization_cff")

# Histograms
process.load("DQMServices.Core.DQM_cfg")

# Input
process.source = cms.Source(
    "PoolSource",
    debugVebosity = cms.untracked.uint32(10),
    debugFlag = cms.untracked.bool(True),
    # untracked vstring fileNames = {'file:ecal_digis.root'}
    # untracked vstring fileNames = {'/store/unmerged/RelVal/2006/9/24/RelVal101Z-EE/GEN-SIM-DIGI-RECO/0005/0AC156B5-094C-DB11-A4D3-000E0C4D2475.root'}
    # untracked uint32 skipEvents = 476
    fileNames = cms.untracked.vstring(
        'rfio:/castor/cern.ch/user/p/pjanot/CMSSW170pre12/SingleParticlePID-11-pT35/SingleParticlePID-11-pT35_0.root', 
        'rfio:/castor/cern.ch/user/p/pjanot/CMSSW170pre12/SingleParticlePID-11-pT35/SingleParticlePID-11-pT35_1.root', 
        'rfio:/castor/cern.ch/user/p/pjanot/CMSSW170pre12/SingleParticlePID-11-pT35/SingleParticlePID-11-pT35_2.root', 
        'rfio:/castor/cern.ch/user/p/pjanot/CMSSW170pre12/SingleParticlePID-11-pT35/SingleParticlePID-11-pT35_3.root', 
        'rfio:/castor/cern.ch/user/p/pjanot/CMSSW170pre12/SingleParticlePID-11-pT35/SingleParticlePID-11-pT35_4.root', 
        'rfio:/castor/cern.ch/user/p/pjanot/CMSSW170pre12/SingleParticlePID-11-pT35/SingleParticlePID-11-pT35_5.root', 
        'rfio:/castor/cern.ch/user/p/pjanot/CMSSW170pre12/SingleParticlePID-11-pT35/SingleParticlePID-11-pT35_6.root', 
        'rfio:/castor/cern.ch/user/p/pjanot/CMSSW170pre12/SingleParticlePID-11-pT35/SingleParticlePID-11-pT35_7.root', 
        'rfio:/castor/cern.ch/user/p/pjanot/CMSSW170pre12/SingleParticlePID-11-pT35/SingleParticlePID-11-pT35_8.root', 
        'rfio:/castor/cern.ch/user/p/pjanot/CMSSW170pre12/SingleParticlePID-11-pT35/SingleParticlePID-11-pT35_9.root', 
        'rfio:/castor/cern.ch/user/p/pjanot/CMSSW170pre12/SingleParticlePID11-pT35/SingleParticlePID11-pT35_0.root', 
        'rfio:/castor/cern.ch/user/p/pjanot/CMSSW170pre12/SingleParticlePID11-pT35/SingleParticlePID11-pT35_1.root', 
        'rfio:/castor/cern.ch/user/p/pjanot/CMSSW170pre12/SingleParticlePID11-pT35/SingleParticlePID11-pT35_2.root', 
        'rfio:/castor/cern.ch/user/p/pjanot/CMSSW170pre12/SingleParticlePID11-pT35/SingleParticlePID11-pT35_3.root', 
        'rfio:/castor/cern.ch/user/p/pjanot/CMSSW170pre12/SingleParticlePID11-pT35/SingleParticlePID11-pT35_4.root', 
        'rfio:/castor/cern.ch/user/p/pjanot/CMSSW170pre12/SingleParticlePID11-pT35/SingleParticlePID11-pT35_5.root', 
        'rfio:/castor/cern.ch/user/p/pjanot/CMSSW170pre12/SingleParticlePID11-pT35/SingleParticlePID11-pT35_6.root', 
        'rfio:/castor/cern.ch/user/p/pjanot/CMSSW170pre12/SingleParticlePID11-pT35/SingleParticlePID11-pT35_7.root', 
        'rfio:/castor/cern.ch/user/p/pjanot/CMSSW170pre12/SingleParticlePID11-pT35/SingleParticlePID11-pT35_8.root', 
        'rfio:/castor/cern.ch/user/p/pjanot/CMSSW170pre12/SingleParticlePID11-pT35/SingleParticlePID11-pT35_9.root'
    )
)

# Famos configuration
process.load("FastSimulation.Configuration.CommonInputsFake_cff")
process.load("FastSimulation.Configuration.FamosSequences_cff")
process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True
# No SimHits
process.famosSimHits.SimulateCalorimetry = False
process.famosSimHits.SimulateTracking = False
# No vertex smearing
process.famosSimHits.VertexGenerator.BetaStar = 0.00001
# Use CMMSW170 tuning
process.misalignedTrackerInteractionGeometry.TrackerMaterial.TrackerMaterialVersion = 2


process.testME = cms.EDFilter(
    "testMaterialEffects",

    # Full, Fast radii and lengths for plots
    BPCylinderRadius = cms.untracked.vdouble(3.2, 3.05),
    BPCylinderLength = cms.untracked.vdouble(999.0, 28.3),

    PXB1CylinderRadius = cms.untracked.vdouble(6.0, 6.0),
    PXB1CylinderLength = cms.untracked.vdouble(28.2, 28.3),

    PXB2CylinderRadius = cms.untracked.vdouble(8.5, 8.5),
    PXB2CylinderLength = cms.untracked.vdouble(28.2, 28.3),

    PXB3CylinderRadius = cms.untracked.vdouble(11.5, 11.5),
    PXB3CylinderLength = cms.untracked.vdouble(28.2, 28.3),

    PXBCablesCylinderRadius = cms.untracked.vdouble(16.9, 16.9),
    PXBCablesCylinderLength = cms.untracked.vdouble(30.0, 30.0),

    PXD1CylinderRadius = cms.untracked.vdouble(16.0, 17.0),
    PXD1CylinderLength = cms.untracked.vdouble(40.0, 40.0),

    PXD2CylinderRadius = cms.untracked.vdouble(16.0, 17.0),
    PXD2CylinderLength = cms.untracked.vdouble(52.0, 52.0),

    PXDCablesCylinderRadius = cms.untracked.vdouble(22.0, 20.2),
    PXDCablesCylinderLength = cms.untracked.vdouble(999.0, 100.0),

    TIB1CylinderRadius = cms.untracked.vdouble(30.0, 28.0),
    TIB1CylinderLength = cms.untracked.vdouble(68.0, 69.0),

    TIB2CylinderRadius = cms.untracked.vdouble(38.0, 37.0),
    TIB2CylinderLength = cms.untracked.vdouble(68.0, 69.0),

    TIB3CylinderRadius = cms.untracked.vdouble(46.0, 45.0),
    TIB3CylinderLength = cms.untracked.vdouble(68.0, 69.0),

    TIB4CylinderRadius = cms.untracked.vdouble(54.0, 53.0),
    TIB4CylinderLength = cms.untracked.vdouble(68.0, 69.0),

    TIBCablesCylinderRadius = cms.untracked.vdouble(53.0, 54.0),
    TIBCablesCylinderLength = cms.untracked.vdouble(74.0, 75.0),

    TID1CylinderRadius = cms.untracked.vdouble(52.0, 54.0),
    TID1CylinderLength = cms.untracked.vdouble(83.0, 83.0),

    TID2CylinderRadius = cms.untracked.vdouble(52.0, 54.0),
    TID2CylinderLength = cms.untracked.vdouble(95.0, 95.0),

    TID3CylinderRadius = cms.untracked.vdouble(52.0, 54.0),
    TID3CylinderLength = cms.untracked.vdouble(110.0, 106.0),

    TIDCablesCylinderRadius = cms.untracked.vdouble(58.2, 55.5),
    TIDCablesCylinderLength = cms.untracked.vdouble(122.0, 108.5),

    TOB1CylinderRadius = cms.untracked.vdouble(66.6, 65.0),
    TOB1CylinderLength = cms.untracked.vdouble(109.0, 109.0),

    TOB2CylinderRadius = cms.untracked.vdouble(75.5, 75.0),
    TOB2CylinderLength = cms.untracked.vdouble(109.0, 109.0),

    TOB3CylinderRadius = cms.untracked.vdouble(84.1, 83.0),
    TOB3CylinderLength = cms.untracked.vdouble(109.0, 109.0),

    TOB4CylinderLength = cms.untracked.vdouble(109.0, 109.0),
    TOB4CylinderRadius = cms.untracked.vdouble(94.0, 92.0),

    TOB5CylinderRadius = cms.untracked.vdouble(105.5, 103.0),
    TOB5CylinderLength = cms.untracked.vdouble(109.0, 109.0),

    TOB6CylinderRadius = cms.untracked.vdouble(113.0, 113.0),
    TOB6CylinderLength = cms.untracked.vdouble(109.0, 109.0),

    TOBCablesCylinderRadius = cms.untracked.vdouble(113.0, 113.0),
    TOBCablesCylinderLength = cms.untracked.vdouble(125.0, 125.0),

    TEC1CylinderRadius = cms.untracked.vdouble(110.0, 110.0),
    TEC1CylinderLength = cms.untracked.vdouble(138.0, 138.0),

    TEC2CylinderRadius = cms.untracked.vdouble(110.0, 110.0),
    TEC2CylinderLength = cms.untracked.vdouble(152.0, 152.0),

    TEC3CylinderRadius = cms.untracked.vdouble(110.0, 110.0),
    TEC3CylinderLength = cms.untracked.vdouble(166.0, 166.0),

    TEC4CylinderRadius = cms.untracked.vdouble(110.0, 110.0),
    TEC4CylinderLength = cms.untracked.vdouble(180.0, 180.0),

    TEC5CylinderRadius = cms.untracked.vdouble(110.0, 110.0),
    TEC5CylinderLength = cms.untracked.vdouble(195.0, 195.0),

    TEC6CylinderRadius = cms.untracked.vdouble(110.0, 110.0),
    TEC6CylinderLength = cms.untracked.vdouble(212.0, 212.0),

    TEC7CylinderRadius = cms.untracked.vdouble(110.0, 110.0),
    TEC7CylinderLength = cms.untracked.vdouble(232.0, 232.0),

    TEC8CylinderRadius = cms.untracked.vdouble(110.0, 110.0),
    TEC8CylinderLength = cms.untracked.vdouble(254.0, 254.0),
    
    TEC9CylinderRadius = cms.untracked.vdouble(110.0, 110.0),
    TEC9CylinderLength = cms.untracked.vdouble(272.0, 272.0),

    TrackerCablesCylinderRadius = cms.untracked.vdouble(125.0, 121.0),
    TrackerCablesCylinderLength = cms.untracked.vdouble(301.0, 301.0),

    TestParticleFilter = cms.PSet(
        # Particles with |eta| > etaMax (momentum direction at primary vertex) 
        # are not simulated 
        etaMax = cms.double(5.0),
        # Charged particles with pT < pTMin (GeV/c) are not simulated
        pTMin = cms.double(0.0),
        # Particles with energy smaller than EMin (GeV) are not simulated
        EMin = cms.double(0.0)
        # Protons with energy in excess of this value (GeV) will kept no matter what
        EProton = cms.double(99999.0),
    )
)

# Path to ryn what is needed
process.p = cms.Path(
    process.offlineBeamSpot+
    process.famosPileUp+
    process.famosSimHits+
    process.testME
)


# Keep the logging output to a nice level #
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['detailedInfo.txt']

