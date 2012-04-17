autoCond = { 
    'mc'        : 'MC_60_V0::All',
    'startup'   : 'START60_V0::All',
    'com10'     : 'GR_R_60_V0::All',
     # 'hltonline' should be the same as same as 'com10' until a compatible GR_H tag is available, then it should point to the GR_H tag and override the connection string and pfnPrefix for use offline
    'hltonline' : 'GR_R_60_V0::All,frontier://FrontierProd/CMS_COND_31X_GLOBALTAG,frontier://FrontierProd/',
    'hltonline11' : ('GR_H_V29::All,frontier://FrontierProd/CMS_COND_31X_GLOBALTAG,frontier://FrontierProd/',
                     'L1GtTriggerMenu_L1Menu_Collisions2012_v0_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T'),
    'starthi'   : 'STARTHI60_V0::All'
}
