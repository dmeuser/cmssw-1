import FWCore.ParameterSet.Config as cms

particleFlowRecHitHCAL = cms.EDProducer("PFRecHitProducerHCAL",
    # verbosity 
    verbose = cms.untracked.bool(False),
    caloTowers = cms.InputTag("towerMakerPF"),
    hcalRecHitsHBHE = cms.InputTag("hbhereco"),
    hcalRecHitsHF = cms.InputTag("hfreco"),
    # cell threshold in barrel 
    thresh_Barrel = cms.double(0.4),
    # cell threshold in HF
    thresh_HF = cms.double(0.4),
    # cell threshold in endcap 
    thresh_Endcap = cms.double(0.4),
    # Navigation in HF: 
    # False = no real clustering in HF
    # True  = do clustering in HF
    navigation_HF = cms.bool(True),
#AUGUSTE: TO BE CHECKED:
    weight_HFem = cms.double(1.000),
    weight_HFhad = cms.double(1.000),
#   weight_HFem = cms.double(1.0),
#   weight_HFhad = cms.double(1.0)

# HCAL DPG RecHit calibration
    HCAL_Calib = cms.bool(True),
    HF_Calib = cms.bool(False),

# Cut short fibres if no long fibre energy
    ShortFibre_Cut = cms.double(60.),
    LongFibre_Fraction = cms.double(0.10),

# Cut long fibres if no short fibre energy
    LongFibre_Cut = cms.double(120.),
    ShortFibre_Fraction = cms.double(0.01),

# Also apply DPG cleaning
    ApplyLongShortDPG = cms.bool(True),

# Cut on timing if sufficient energy (in both long and short fibres)
    LongShortFibre_Cut = cms.double(1E9),
    #MinLongTiming_Cut = cms.double(-11.),
    #MaxLongTiming_Cut = cms.double(+8.),
    #MinShortTiming_Cut = cms.double(-10.),
    #MaxShortTiming_Cut = cms.double(+8.),
    MinLongTiming_Cut = cms.double(-5.),
    MaxLongTiming_Cut = cms.double(+5.),
    MinShortTiming_Cut = cms.double(-5.),
    MaxShortTiming_Cut = cms.double(+5.),

# Also apply DPG cleaning
    ApplyTimeDPG = cms.bool(False),
    ApplyPulseDPG = cms.bool(False),

# Compensate for ECAL dead channels                                        
    ECAL_Compensate = cms.bool(False),
    ECAL_Threshold = cms.double(10.),
    ECAL_Compensation = cms.double(0.5),
    ECAL_Dead_Code = cms.uint32(10),

# Depth correction (in cm) for hadronic and electromagnetic rechits
    EM_Depth = cms.double(22.),
    HAD_Depth = cms.double(47.)                              
                                  
)


