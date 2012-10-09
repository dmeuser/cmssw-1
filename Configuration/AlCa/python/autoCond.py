autoCond = { 
    # GlobalTag for MC production with perfectly aligned and calibrated detector
    'mc'                :   'MC_61_V1::All',
    # GlobalTag for MC production with realistic alignment and calibrations
    'startup'           :   'START61_V1::All',
    # GlobalTag for MC production of Heavy Ions events with realistic alignment and calibrations
    'starthi'           :   'STARTHI61_V3::All',
    # GlobalTag for data reprocessing: this should always be the GR_R tag
    'com10'             : ( 'GR_R_60_V7::All',
                            # HLT particle flow jet energy corrections
                            'JetCorrectorParametersCollection_AK5PF_2012_V8_hlt_mc,JetCorrectionsRecord,frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS,AK5PFHLT',
                            'JetCorrectorParametersCollection_AK5PFchs_2012_V8_hlt_mc,JetCorrectionsRecord,frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS,AK5PFchsHLT',
                          ),
    # GlobalTag for running HLT on recent data: this should be the same as 'com10' until a compatible GR_H tag is available, 
    # then it should point to the GR_H tag and override the connection string and pfnPrefix for use offline
    'hltonline'         : ( 'GR_P_V42::All',
                            # HLT particle flow jet energy corrections
                            'JetCorrectorParametersCollection_AK5PF_2012_V8_hlt_mc,JetCorrectionsRecord,frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS,AK5PFHLT',
                            'JetCorrectorParametersCollection_AK5PFchs_2012_V8_hlt_mc,JetCorrectionsRecord,frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS,AK5PFchsHLT',
                          ),
    # GlobalTag for running HLT on 2011 data: same as 'hltonline', override the L1 menu with 2012 v3
    'hltonline11'       : ( 'GR_P_V42::All',
                            # L1 menu 2012 v3
                            'L1GtTriggerMenu_L1Menu_Collisions2012_v3_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                            # HLT particle flow jet energy corrections
                            'JetCorrectorParametersCollection_AK5PF_2012_V8_hlt_mc,JetCorrectionsRecord,frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS,AK5PFHLT',
                            'JetCorrectorParametersCollection_AK5PFchs_2012_V8_hlt_mc,JetCorrectionsRecord,frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS,AK5PFchsHLT',
                          ),

    # dedicated GlobalTags for MC production with the frozen HLT menus
    'startup_5E33v4'    : ( 'START61_V1::All',
                            # L1 menu 2012 v0
                            'L1GtTriggerMenu_L1Menu_Collisions2012_v0_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                            # old L1 CSCTF configuration
                            'L1MuCSCPtLut_key-10_mc,L1MuCSCPtLutRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                            # L1 DTTF settings used up to Run2012B
                            'L1MuDTTFParameters_dttf11_TSC_09_17_col_mc,L1MuDTTFParametersRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                            # old L1 GCT configuration, without jet seed threshold
                            'L1GctJetFinderParams_GCTPhysics_2011_09_01_B_mc,L1GctJetFinderParamsRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                            'L1HfRingEtScale_GCTPhysics_2011_09_01_B_mc,L1HfRingEtScaleRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                            'L1HtMissScale_GCTPhysics_2011_09_01_B_mc,L1HtMissScaleRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                            'L1JetEtScale_GCTPhysics_2011_09_01_B_mc,L1JetEtScaleRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                          ),
    'startup_7E33v2'    : ( 'START61_V1::All',
                            # L1 menu 2012 v1
                            'L1GtTriggerMenu_L1Menu_Collisions2012_v1a_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                            # L1 DTTF settings used up to Run2012B
                            'L1MuDTTFParameters_dttf11_TSC_09_17_col_mc,L1MuDTTFParametersRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                            # old L1 GCT configuration, without jet seed threshold
                            'L1GctJetFinderParams_GCTPhysics_2011_09_01_B_mc,L1GctJetFinderParamsRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                            'L1HfRingEtScale_GCTPhysics_2011_09_01_B_mc,L1HfRingEtScaleRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                            'L1HtMissScale_GCTPhysics_2011_09_01_B_mc,L1HtMissScaleRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                            'L1JetEtScale_GCTPhysics_2011_09_01_B_mc,L1JetEtScaleRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                          ),
    'startup_7E33v3'    : ( 'START61_V1::All',
                            # L1 menu 2012 v2
                            'L1GtTriggerMenu_L1Menu_Collisions2012_v2_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                            # L1 DTTF settings used since Run2012C
                            'L1MuDTTFParameters_dttf12_TSC_03_csc_col_mc,L1MuDTTFParametersRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                          ),
    'startup_7E33v4'    : ( 'START61_V1::All',
                            # L1 menu 2012 v2
                            'L1GtTriggerMenu_L1Menu_Collisions2012_v2_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                            # L1 DTTF settings used since Run2012C
                            'L1MuDTTFParameters_dttf12_TSC_03_csc_col_mc,L1MuDTTFParametersRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                          ),
    'startup_8E33v1'      : ( 'START61_V1::All',
                            # L1 menu 2012 v3
                            'L1GtTriggerMenu_L1Menu_Collisions2012_v3_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                            # L1 DTTF settings used since Run2012C
                            'L1MuDTTFParameters_dttf12_TSC_03_csc_col_mc,L1MuDTTFParametersRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                          ),
    'startup_GRun'      : ( 'START61_V1::All',
                            # L1 menu 2012 v3
                            'L1GtTriggerMenu_L1Menu_Collisions2012_v3_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                            # L1 DTTF settings used since Run2012C
                            'L1MuDTTFParameters_dttf12_TSC_03_csc_col_mc,L1MuDTTFParametersRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                          ),
    'starthi_HIon'      : ( 'STARTHI61_V3::All',
                            # L1 heavy ions menu 2011 v0
                            'L1GtTriggerMenu_L1Menu_CollisionsHeavyIons2011_v0_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                          ),

    # dedicated GlobalTags for running the frozen HLT menus on data
    'hltonline_5E33v4'  : ( 'GR_P_V42::All',
                            # L1 menu 2012 v0
                            'L1GtTriggerMenu_L1Menu_Collisions2012_v0_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                          ),
    'hltonline_7E33v2'  : ( 'GR_P_V42::All',
                            # L1 menu 2012 v1
                            'L1GtTriggerMenu_L1Menu_Collisions2012_v1a_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                          ),
    'hltonline_7E33v3'  : ( 'GR_P_V42::All',
                            # L1 menu 2012 v2
                            'L1GtTriggerMenu_L1Menu_Collisions2012_v2_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                          ),
    'hltonline_7E33v4'  : ( 'GR_P_V42::All',
                            # L1 menu 2012 v2
                            'L1GtTriggerMenu_L1Menu_Collisions2012_v2_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                            # HLT particle flow jet energy corrections
                            'JetCorrectorParametersCollection_AK5PF_2012_V8_hlt_mc,JetCorrectionsRecord,frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS,AK5PFHLT',
                            'JetCorrectorParametersCollection_AK5PFchs_2012_V8_hlt_mc,JetCorrectionsRecord,frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS,AK5PFchsHLT',
                          ),
    'hltonline_8E33v1'  : ( 'GR_P_V42::All',
                            # L1 menu 2012 v3
                            'L1GtTriggerMenu_L1Menu_Collisions2012_v3_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                            # HLT particle flow jet energy corrections
                            'JetCorrectorParametersCollection_AK5PF_2012_V8_hlt_mc,JetCorrectionsRecord,frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS,AK5PFHLT',
                            'JetCorrectorParametersCollection_AK5PFchs_2012_V8_hlt_mc,JetCorrectionsRecord,frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS,AK5PFchsHLT',
                          ),
    'hltonline_GRun'    : ( 'GR_P_V42::All',
                            # L1 menu 2012 v3
                            'L1GtTriggerMenu_L1Menu_Collisions2012_v3_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                            # HLT particle flow jet energy corrections
                            'JetCorrectorParametersCollection_AK5PF_2012_V8_hlt_mc,JetCorrectionsRecord,frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS,AK5PFHLT',
                            'JetCorrectorParametersCollection_AK5PFchs_2012_V8_hlt_mc,JetCorrectionsRecord,frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS,AK5PFchsHLT',
                          ),
    'hltonline_HIon'    : ( 'GR_P_V42::All',
                            # L1 heavy ions menu 2011 v0
                            'L1GtTriggerMenu_L1Menu_CollisionsHeavyIons2011_v0_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                            # HLT particle flow jet energy corrections
                            'JetCorrectorParametersCollection_AK5PF_2012_V8_hlt_mc,JetCorrectionsRecord,frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS,AK5PFHLT',
                            'JetCorrectorParametersCollection_AK5PFchs_2012_V8_hlt_mc,JetCorrectionsRecord,frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS,AK5PFchsHLT',
                          ),

    # dedicated GlobalTags for running RECO and the frozen HLT menus on data
    'com10_5E33v4'      : ( 'GR_R_60_V7::All',
                            # L1 menu 2012 v0
                            'L1GtTriggerMenu_L1Menu_Collisions2012_v0_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                          ),
    'com10_7E33v2'      : ( 'GR_R_60_V7::All',
                            # L1 menu 2012 v1
                            'L1GtTriggerMenu_L1Menu_Collisions2012_v1a_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                          ),
    'com10_7E33v3'      : ( 'GR_R_60_V7::All',
                            # L1 menu 2012 v2
                            'L1GtTriggerMenu_L1Menu_Collisions2012_v2_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                          ),
    'com10_7E33v4'      : ( 'GR_R_60_V7::All',
                            # L1 menu 2012 v2
                            'L1GtTriggerMenu_L1Menu_Collisions2012_v2_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                            # HLT particle flow jet energy corrections
                            'JetCorrectorParametersCollection_AK5PF_2012_V8_hlt_mc,JetCorrectionsRecord,frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS,AK5PFHLT',
                            'JetCorrectorParametersCollection_AK5PFchs_2012_V8_hlt_mc,JetCorrectionsRecord,frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS,AK5PFchsHLT',
                          ),
    'com10_8E33v1'      : ( 'GR_R_60_V7::All',
                            # L1 menu 2012 v3
                            'L1GtTriggerMenu_L1Menu_Collisions2012_v3_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                            # HLT particle flow jet energy corrections
                            'JetCorrectorParametersCollection_AK5PF_2012_V8_hlt_mc,JetCorrectionsRecord,frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS,AK5PFHLT',
                            'JetCorrectorParametersCollection_AK5PFchs_2012_V8_hlt_mc,JetCorrectionsRecord,frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS,AK5PFchsHLT',
                          ),
    'com10_GRun'        : ( 'GR_R_60_V7::All',
                            # L1 menu 2012 v3
                            'L1GtTriggerMenu_L1Menu_Collisions2012_v3_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                            # HLT particle flow jet energy corrections
                            'JetCorrectorParametersCollection_AK5PF_2012_V8_hlt_mc,JetCorrectionsRecord,frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS,AK5PFHLT',
                            'JetCorrectorParametersCollection_AK5PFchs_2012_V8_hlt_mc,JetCorrectionsRecord,frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS,AK5PFchsHLT',
                          ),
    'com10_HIon'        : ( 'GR_R_60_V7::All',
                            # L1 heavy ions menu 2011 v0
                            'L1GtTriggerMenu_L1Menu_CollisionsHeavyIons2011_v0_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T',
                            # HLT particle flow jet energy corrections
                            'JetCorrectorParametersCollection_AK5PF_2012_V8_hlt_mc,JetCorrectionsRecord,frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS,AK5PFHLT',
                            'JetCorrectorParametersCollection_AK5PFchs_2012_V8_hlt_mc,JetCorrectionsRecord,frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS,AK5PFchsHLT',
                          )
}
