import FWCore.ParameterSet.Config as cms

process = cms.Process('SKIM')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/EventContent/EventContent_cff')

# module to select events based on HLT Trigger bits
process.load('HLTrigger/HLTfilters/hltHighLevelDev_cfi')

# Loading "hltHighLevelDev_cfi" defines an EDFilter called hltHighLevelDev
# now we can configure it 

# All events from MinBias PD
process.hltHighLevelDev.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.hltHighLevelDev.HLTPaths = (
   'HLT_PhysicsDeclared',
    )

process.hltHighLevelDev.andOr = True # True = OR, False = AND
process.hltHighLevelDev.throw = False # throw exception on unknown path names
process.hltHighLevelDev.HLTPathsPrescales  = cms.vuint32(
   1,    #'HLT_PhysicsDeclared',
    )
process.hltHighLevelDev.HLTOverallPrescale = cms.uint32 (1)

# All events from MinBias PD, prescaled by a factor 10
process.hltHighLevelDev2 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev2.HLTPaths = (
   'HLT_PhysicsDeclared',
    )

process.hltHighLevelDev2.andOr = True # True = OR, False = AND
process.hltHighLevelDev2.throw = False # throw exception on unknown path names
process.hltHighLevelDev2.HLTPathsPrescales  = cms.vuint32(
   1,    #'HLT_PhysicsDeclared',
    )
process.hltHighLevelDev2.HLTOverallPrescale = cms.uint32 (10)


# All interesting events.
process.hltHighLevelDev3 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev3.HLTPaths = (
   'HLT_BTagIP_Jet50U',                
   'HLT_BTagMu_Jet10U',                
   'HLT_DiJetAve15U_8E29',             
   'HLT_DiJetAve30U_8E29',             
   'HLT_DoubleEle5_SW_L1R',            
   'HLT_DoubleLooseIsoTau15',          
   'HLT_DoubleMu0',                    
   'HLT_DoubleMu3',                    
   'HLT_DoublePhoton10_L1R',           
   'HLT_DoublePhoton5_Jpsi_L1R',       
   'HLT_DoublePhoton5_Upsilon_L1R',    
   'HLT_DoublePhoton5_eeRes_L1R',      
   'HLT_EgammaSuperClusterOnly_L1R',   
   'HLT_Ele10_LW_EleId_L1R',           
   'HLT_Ele10_LW_L1R',                 
   'HLT_Ele15_LW_L1R',                 
   'HLT_Ele15_SC10_LW_L1R',            
   'HLT_Ele15_SiStrip_L1R',            
   'HLT_Ele20_LW_L1R',                 
   'HLT_FwdJet20U',                    
   'HLT_HT100U',                       
   'HLT_HighMultiplicityBSC',          
   'HLT_IsoMu3',                       
   'HLT_IsoTrackHB_8E29',              
   'HLT_IsoTrackHE_8E29',              
   'HLT_Jet15U',                       
   'HLT_Jet30U',                       
   'HLT_Jet50U',                       
   'HLT_L1DoubleEG5',                  
   'HLT_L1DoubleMuOpen',               
   'HLT_L1Jet6U',                      
   'HLT_L1MET20',                      
   'HLT_L1Mu',                         
   'HLT_L1Mu14_L1ETM30',               
   'HLT_L1Mu14_L1SingleEG10',          
   'HLT_L1Mu14_L1SingleJet6U',         
   'HLT_L1Mu20',                       
   'HLT_L1MuOpen',                     
   'HLT_L1SingleEG20_NoBPTX',          
   'HLT_L1SingleEG5',                  
   'HLT_L1SingleEG5_NoBPTX',           
   'HLT_L1SingleEG8',                  
   'HLT_L1SingleForJet',               
   'HLT_L2Mu11',                       
   'HLT_L2Mu9',                        
   'HLT_MET100',                       
   'HLT_MET45',                        
   'HLT_Mu3',                          
   'HLT_Mu5',                          
   'HLT_Mu9',                          
   'HLT_Photon10_L1R',                 
   'HLT_Photon15_L1R',                 
   'HLT_Photon15_LooseEcalIso_L1R',    
   'HLT_Photon15_TrackIso_L1R',        
   'HLT_Photon20_L1R',                 
   'HLT_Photon30_L1R_8E29',            
   'HLT_QuadJet15U',                   
   'HLT_SingleLooseIsoTau20',          
   'HLT_StoppedHSCP_8E29'
    )

process.hltHighLevelDev3.andOr = True # True = OR, False = AND
process.hltHighLevelDev3.throw = False # throw exception on unknown path names
process.hltHighLevelDev3.HLTPathsPrescales  = cms.vuint32(
   1,   #'HLT_BTagIP_Jet50U',                
   1,   #'HLT_BTagMu_Jet10U',                
   1,   #'HLT_DiJetAve15U_8E29',             
   1,   #'HLT_DiJetAve30U_8E29',             
   1,   #'HLT_DoubleEle5_SW_L1R',            
   1,   #'HLT_DoubleLooseIsoTau15',          
   1,   #'HLT_DoubleMu0',                    
   1,   #'HLT_DoubleMu3',                    
   1,   #'HLT_DoublePhoton10_L1R',           
   1,   #'HLT_DoublePhoton5_Jpsi_L1R',       
   1,   #'HLT_DoublePhoton5_Upsilon_L1R',    
   1,   #'HLT_DoublePhoton5_eeRes_L1R',      
   1,   #'HLT_EgammaSuperClusterOnly_L1R',   
   1,   #'HLT_Ele10_LW_EleId_L1R',           
   1,   #'HLT_Ele10_LW_L1R',                 
   1,   #'HLT_Ele15_LW_L1R',                 
   1,   #'HLT_Ele15_SC10_LW_L1R',            
   1,   #'HLT_Ele15_SiStrip_L1R',            
   1,   #'HLT_Ele20_LW_L1R',                 
   1,   #'HLT_FwdJet20U',                    
   1,   #'HLT_HT100U',                       
   1,   #'HLT_HighMultiplicityBSC',          
   1,   #'HLT_IsoMu3',                       
   1,   #'HLT_IsoTrackHB_8E29',              
   1,   #'HLT_IsoTrackHE_8E29',              
   1,   #'HLT_Jet15U',                       
   1,   #'HLT_Jet30U',                       
   1,   #'HLT_Jet50U',                       
   1,   #'HLT_L1DoubleEG5',                  
   1,   #'HLT_L1DoubleMuOpen',               
   1,   #'HLT_L1Jet6U',                      
   1,   #'HLT_L1MET20',                      
   1,   #'HLT_L1Mu',                         
   1,   #'HLT_L1Mu14_L1ETM30',               
   1,   #'HLT_L1Mu14_L1SingleEG10',          
   1,   #'HLT_L1Mu14_L1SingleJet6U',         
   1,   #'HLT_L1Mu20',                       
   1,   #'HLT_L1MuOpen',                     
   1,   #'HLT_L1SingleEG20_NoBPTX',          
   1,   #'HLT_L1SingleEG5',                  
   1,   #'HLT_L1SingleEG5_NoBPTX',           
   1,   #'HLT_L1SingleEG8',                  
   1,   #'HLT_L1SingleForJet',               
   1,   #'HLT_L2Mu11',                       
   1,   #'HLT_L2Mu9',                        
   1,   #'HLT_MET100',                       
   1,   #'HLT_MET45',                        
   1,   #'HLT_Mu3',                          
   1,   #'HLT_Mu5',                          
   1,   #'HLT_Mu9',                          
   1,   #'HLT_Photon10_L1R',                 
   1,   #'HLT_Photon15_L1R',                 
   1,   #'HLT_Photon15_LooseEcalIso_L1R',    
   1,   #'HLT_Photon15_TrackIso_L1R',        
   1,   #'HLT_Photon20_L1R',                 
   1,   #'HLT_Photon30_L1R_8E29',            
   1,   #'HLT_QuadJet15U',                   
   1,   #'HLT_SingleLooseIsoTau20',          
   1    #'HLT_StoppedHSCP_8E29'
    )
process.hltHighLevelDev3.HLTOverallPrescale = cms.uint32 (1)

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    annotation = cms.untracked.string('skim'),
    name = cms.untracked.string('skim')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    'file:/data/rossin/CMS/store/mc/Summer09/MinBias/GEN-SIM-RAW/STARTUP3X_V8D_900GeV-v1/outputA_4_PhysDecl.root',
#    'file:/data/rossin/CMS/store/mc/Summer09/MinBias/GEN-SIM-RAW/STARTUP3X_V8D_900GeV-v1/outputA_3.root',
#    'file:/data/rossin/CMS/store/mc/Summer09/MinBias/GEN-SIM-RAW/STARTUP3X_V8D_900GeV-v1/outputA_4.root',
#    'file:/data/rossin/CMS/store/mc/Summer09/MinBias/GEN-SIM-RAW/STARTUP3X_V8D_900GeV-v1/outputA_5.root',
#    'file:/data/rossin/CMS/store/mc/Summer09/MinBias/GEN-SIM-RAW/STARTUP3X_V8D_900GeV-v1/outputA_6.root'
    )
)

process.hltTrigReport = cms.EDAnalyzer( "HLTrigReport",
    HLTriggerResults = cms.InputTag( 'TriggerResults','','HLT' )
)



# All minbias with PhysicsDeclared
process.output1 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_AllMinBias_900GeV_PhysDecl.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('FEVT'),
                                      filterName = cms.untracked.string('SD_AllMinBias')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim1')
                                      )
)

# 10% of minbias with PhysicsDeclared + all the interesting events
process.output2 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_InterestingEvents_MinBias10_900GeV_PhysDecl.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('FEVT'),
                                      filterName = cms.untracked.string('SD_InterestingEvents_MinBias10')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim2','skim3')
                                      )
)

# All the interesting events
process.output3 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_InterestingEvent.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('FEVT'),
                                      filterName = cms.untracked.string('SD_InterestingEvents')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim3')
                                      )
)


# the usage of trigger bits for selection is explained here:
## https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideEDMPathsAndTriggerBits#Selecting_Pass_for_Some_Trigger 

process.hlttrigreport = cms.Path(process.hltTrigReport)
process.skim1 = cms.Path(process.hltHighLevelDev)
process.skim2 = cms.Path(process.hltHighLevelDev2)
process.skim3 = cms.Path(process.hltHighLevelDev3)


process.out_step = cms.EndPath(process.output1+process.output2+process.output3)

process.schedule = cms.Schedule(process.hlttrigreport,process.skim1,process.skim2,process.skim3)
process.schedule.append(process.out_step)
