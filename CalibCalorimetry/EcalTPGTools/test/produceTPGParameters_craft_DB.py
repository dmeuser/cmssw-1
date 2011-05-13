import FWCore.ParameterSet.Config as cms

process = cms.Process("ProdTPGParam")

# Calo geometry service model
process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")
process.load("Geometry.CaloEventSetup.EcalTrigTowerConstituents_cfi")
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")

# ecal mapping
process.load("Geometry.EcalMapping.EcalMapping_cfi")
process.eegeom = cms.ESSource("EmptyESSource",
    recordName = cms.string('EcalMappingRcd'),
    iovIsRunNotTime = cms.bool(True),
    firstValid = cms.vuint32(1)
)

# to read from orcon
process.load("CondCore.DBCommon.CondDBCommon_cfi")
#process.CondDBCommon.connect = 'oracle://cms_orcon_prod/CMS_COND_ECAL'
process.CondDBCommon.connect = 'sqlite_file:DB.db'
process.CondDBCommon.DBParameters.authenticationPath = '/nfshome0/xiezhen/conddb'
 
 
process.PoolDBESSource = cms.ESSource("PoolDBESSource",
    process.CondDBCommon,
    timetype = cms.untracked.string('runnumber'),
    toGet = cms.VPSet(
     cms.PSet(
        record = cms.string('EcalPedestalsRcd'),
        tag = cms.string('EcalPedestals_mc')
     ),
     cms.PSet(
        record = cms.string('EcalADCToGeVConstantRcd'),
        tag = cms.string('EcalADCToGeVConstant_mc')
     ),
     cms.PSet(
        record = cms.string('EcalChannelStatusRcd'),
        tag = cms.string('EcalChannelStatus_mc')
     ),
     cms.PSet(
        record = cms.string('EcalIntercalibConstantsRcd'),
        tag = cms.string('EcalIntercalibConstants_mc')
     ),
     cms.PSet(
        record = cms.string('EcalGainRatiosRcd'),
        tag = cms.string('EcalGainRatios_mc')
     ),
        cms.PSet(
            record = cms.string('EcalWeightXtalGroupsRcd'),
            tag = cms.string('EcalWeightXtalGroups_mc')
        ), 
        cms.PSet(
            record = cms.string('EcalTBWeightsRcd'),
            tag = cms.string('EcalTBWeights_mc')
        ))
)



#########################
process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

process.TPGParamProducer = cms.EDFilter("EcalTPGParamBuilder",

    #### inputs/ouputs control ####
    writeToDB  = cms.bool(False),
    allowDBEE  = cms.bool(False),

    DBsid   = cms.string('cms_omds_lb'),
    DBuser  = cms.string('cms_ecal_conf_test'),
    DBpass  = cms.string('*************'),
    DBport  = cms.uint32(10121),

    writeToFiles = cms.bool(False),
    outFile = cms.string('TPG_startupTest.txt'),
   #### TPG config tag and version (if not given it will be automatically given ) ####
    TPGtag = cms.string('CRAFT'),
    TPGversion = cms.uint32(1),
                                        
   #### TPG calculation parameters ####
    useTransverseEnergy = cms.bool(True),   ## true when TPG computes transverse energy, false for energy
    Et_sat_EB = cms.double(64.0),           ## Saturation value (in GeV) of the TPG before the compressed-LUT (rem: with 35.84 the TPG_LSB = crystal_LSB)
    Et_sat_EE = cms.double(64.0),           ## Saturation value (in GeV) of the TPG before the compressed-LUT (rem: with 35.84 the TPG_LSB = crystal_LSB)

    sliding = cms.uint32(0),                ## Parameter used for the FE data format, should'nt be changed

    weight_sampleMax = cms.uint32(3),       ## position of the maximum among the 5 samples used by the TPG amplitude filter

    forcedPedestalValue = cms.int32(160),   ## use this value instead of getting it from DB or MC (-1 means use DB or MC)
    forceEtaSlice = cms.bool(True),         ## when true, same linearization coeff for all crystals belonging to a given eta slice (tower)

    LUT_option = cms.string('Linear'),      ## compressed LUT option can be: "Identity", "Linear", "EcalResolution"
    LUT_threshold_EB = cms.double(0.750),   ## All Trigger Primitives <= threshold (in GeV) will be set to 0 
    LUT_threshold_EE = cms.double(0.750),   ## All Trigger Primitives <= threshold (in GeV) will be set to 0 
    LUT_stochastic_EB = cms.double(0.03),   ## Stochastic term of the ECAL-EB ET resolution (used only if LUT_option="EcalResolution")
    LUT_noise_EB = cms.double(0.2),         ## noise term (GeV) of the ECAL-EB ET resolution (used only if LUT_option="EcalResolution")
    LUT_constant_EB = cms.double(0.005),    ## constant term of the ECAL-EB ET resolution (used only if LUT_option="EcalResolution")
    LUT_stochastic_EE = cms.double(0.03),   ## Stochastic term of the ECAL-EE ET resolution (used only if LUT_option="EcalResolution")
    LUT_noise_EE = cms.double(0.2),         ## noise term (GeV) of the ECAL-EE ET resolution (used only if LUT_option="EcalResolution")
    LUT_constant_EE = cms.double(0.005),    ## constant term of the ECAL-EE ET resolution (used only if LUT_option="EcalResolution")

    TTF_lowThreshold_EB = cms.double(0.6875),  ## EB Trigger Tower Flag low threshold in GeV
    TTF_highThreshold_EB = cms.double(0.6875), ## EB Trigger Tower Flag high threshold in GeV
    TTF_lowThreshold_EE = cms.double(0.6875),  ## EE Trigger Tower Flag low threshold in GeV
    TTF_highThreshold_EE = cms.double(0.6875), ## EE Trigger Tower Flag high threshold in GeV

    FG_lowThreshold_EB = cms.double(0.3125),   ## EB Fine Grain Et low threshold in GeV
    FG_highThreshold_EB = cms.double(0.3125),  ## EB Fine Grain Et high threshold in GeV
    FG_lowRatio_EB = cms.double(0.8),          ## EB Fine Grain low-ratio
    FG_highRatio_EB = cms.double(0.8),         ## EB Fine Grain high-ratio
    FG_lut_EB = cms.uint32(0x08),              ## EB Fine Grain Look-up table. Put something != 0 if you really know what you do!
    FG_Threshold_EE = cms.double(0.0),         ## EE Fine threshold in GeV
    FG_lut_strip_EE = cms.uint32(0),           ## EE Fine Grain strip Look-up table
    FG_lut_tower_EE = cms.uint32(0)            ## EE Fine Grain tower Look-up table
)

process.p = cms.Path(process.TPGParamProducer)

