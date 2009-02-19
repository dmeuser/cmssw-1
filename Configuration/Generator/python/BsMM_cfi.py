import FWCore.ParameterSet.Config as cms

source = cms.Source("EmptySource")

from Configuration.Generator.PythiaUESettings_cfi import *
generator = cms.EDFilter("Pythia6GeneratorFilter",
    pythiaPylistVerbosity = cms.untracked.int32(0),
    # put here the efficiency of your filter (1. if no filter)
    filterEfficiency = cms.untracked.double(0.00045),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    comEnergy = cms.double(10000.0),
    # put here the cross section of your process (in pb)
    crossSection = cms.untracked.double(54700000000.0),
    maxEventsToPrint = cms.untracked.int32(0),
    PythiaParameters = cms.PSet(
        pythiaUESettingsBlock,
        processParameters = cms.vstring('PMAS(5,1)=4.8          ! b quark mass', 
            'MSEL=1                 ! Min Bias', 
            'MDME(953,2) = 0        ! PHASE SPACE', 
            'BRAT(953)   = 1.       ! BRANCHING FRACTION', 
            'KFDP(953,1) =  13      ! mu-', 
            'KFDP(953,2) = -13      ! mu+', 
            'KFDP(953,3) = 0        ! nada', 
            'KFDP(953,4) = 0        ! nada', 
            'KFDP(953,5) = 0        ! nada', 
            'PMAS(140,1) = 5.369', 
            'MDME(953,1) = 0        ', 
            'MDME(954,1) = 0        ', 
            'MDME(955,1) = 0        ', 
            'MDME(956,1) = 0        ', 
            'MDME(957,1) = 0        ', 
            'MDME(958,1) = 0        ', 
            'MDME(959,1) = 0        ', 
            'MDME(960,1) = 0        ', 
            'MDME(961,1) = 0        ', 
            'MDME(962,1) = 0        ', 
            'MDME(963,1) = 0        ', 
            'MDME(964,1) = 0        ', 
            'MDME(965,1) = 0        ', 
            'MDME(966,1) = 0        ', 
            'MDME(967,1) = 0        ', 
            'MDME(968,1) = 0        ', 
            'MDME(969,1) = 0        ', 
            'MDME(970,1) = 0        ', 
            'MDME(971,1) = 0        ', 
            'MDME(972,1) = 0        ', 
            'MDME(973,1) = 0        ', 
            'MDME(974,1) = 0        ', 
            'MDME(975,1) = 0        ', 
            'MDME(976,1) = 0        ', 
            'MDME(977,1) = 0        ', 
            'MDME(978,1) = 0        ', 
            'MDME(979,1) = 0        ', 
            'MDME(980,1) = 0        ', 
            'MDME(981,1) = 0        ', 
            'MDME(982,1) = 0        ', 
            'MDME(983,1) = 0        ', 
            'MDME(984,1) = 0        ', 
            'MDME(985,1) = 0        ', 
            'MDME(986,1) = 0        ', 
            'MDME(987,1) = 0        ', 
            'MDME(988,1) = 0        ', 
            'MDME(989,1) = 0        ', 
            'MDME(990,1) = 0        ', 
            'MDME(991,1) = 0        ', 
            'MDME(953,1) = 1       !  Bs -> mu mu '),
        # This is a vector of ParameterSet names to be read, in this order 
        # The first one sets some defaults 
        # The second one are the user specific parameters 
        parameterSets = cms.vstring('pythiaUESettings', 
            'processParameters')
    )
)

ProductionFilterSequence = cms.Sequence(generator)
