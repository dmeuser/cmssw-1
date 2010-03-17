import FWCore.ParameterSet.Config as cms

simEcalDigis = cms.EDProducer("EcalSelectiveReadoutProducer",
    # Label of input EB and EE digi collections
    digiProducer = cms.string('simEcalUnsuppressedDigis'),

    # Instance name of input EB digi collections
    EBdigiCollection = cms.string(''),

    # Instance name of input EB digi collections
    EEdigiCollection = cms.string(''),

    # Instance name of output EB SR flags collection
    EBSrFlagCollection = cms.string('ebSrFlags'),

    # Instance name of output EE SR flags collection
    EESrFlagCollection = cms.string('eeSrFlags'),

    # Instance name of output EB digis collection
    EBSRPdigiCollection = cms.string('ebDigis'),

    # Instance name of output EE digis collection
    EESRPdigiCollection = cms.string('eeDigis'),

    # Label name of input ECAL trigger primitive collection
    trigPrimProducer = cms.string('simEcalTriggerPrimitiveDigis'),

    # Instance name of ECAL trigger primitive collection
    trigPrimCollection = cms.string(''),

    # Neighbour eta range, neighborhood: (2*deltaEta+1)*(2*deltaPhi+1)
    deltaEta = cms.int32(1),

    # Neighbouring eta range, neighborhood: (2*deltaEta+1)*(2*deltaPhi+1)
    deltaPhi = cms.int32(1),

    # Index of time sample (staring from 1) the first DCC weights is implied
    ecalDccZs1stSample = cms.int32(3),

    # ADC to GeV conversion factor used in ZS filter for EB
    ebDccAdcToGeV = cms.double(0.035),

    # ADC to GeV conversion factor used in ZS filter for EE
    eeDccAdcToGeV = cms.double(0.06),

    #DCC ZS FIR weights.
    #d-efault value set of DCC firmware used in CRUZET and CRAFT
    dccNormalizedWeights = cms.vdouble(-1.1865, 0.0195, 0.2900, 0.3477, 0.3008,
                                        0.2266),

    # Switch to use a symetric zero suppression (cut on absolute value). For
    # studies only, for time being it is not supported by the hardware.
    symetricZS = cms.bool(False),

    # ZS energy threshold in GeV to apply to low interest channels of barrel
    srpBarrelLowInterestChannelZS = cms.double(2.25*.035),

    # ZS energy threshold in GeV to apply to low interest channels of endcap
    srpEndcapLowInterestChannelZS = cms.double(3.75*0.06),

    # ZS energy threshold in GeV to apply to high interest channels of barrel
    srpBarrelHighInterestChannelZS = cms.double(-1.e9),

    # ZS energy threshold in GeV to apply to high interest channels of endcap
    srpEndcapHighInterestChannelZS = cms.double(-1.e9),

    #switch to run w/o trigger primitive. For debug use only
    trigPrimBypass = cms.bool(False),
                              
    #for debug mode only:
    trigPrimBypassLTH = cms.double(1.0),

    #for debug mode only:
    trigPrimBypassHTH = cms.double(1.0),

    #for debug mode only
    trigPrimBypassWithPeakFinder = cms.bool(True),
                              
    #number of events whose TT and SR flags must be dumped (for debug purpose):
    dumpFlags = cms.untracked.int32(0),
                              
    #logical flag to write out SrFlags
    writeSrFlags = cms.untracked.bool(True)
)



