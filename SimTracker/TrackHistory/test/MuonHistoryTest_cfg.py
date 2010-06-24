import FWCore.ParameterSet.Config as cms

process = cms.Process("MuonHistoryTest")

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/MixingNoPileUp_cff')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/Generator_cff')
process.load('Configuration/StandardSequences/VtxSmearedEarly10TeVCollision_cff')
process.load('Configuration/StandardSequences/Sim_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

process.load("SimTracker.TrackHistory.Playback_cff")
process.load("SimTracker.TrackHistory.MuonClassifier_cff")

process.muonHistoryAnalyzer = cms.EDAnalyzer("TrackHistoryAnalyzer",
    process.MuonClassifier,
    pset = process.MuonClassifier
)

process.GlobalTag.globaltag = 'MC_36Y_V7A::All'

process.p = cms.Path(process.playback * process.muonHistoryAnalyzer)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(200) )
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)

readFiles.extend( [
   '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0009/FE6E09F4-8439-DF11-B704-001A928116CC.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0009/F02962F3-8439-DF11-9CAE-001BFCDBD176.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0009/C4976FEF-8439-DF11-B754-002618943986.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0009/C23B14EF-8439-DF11-B4C9-002618943865.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0009/A4799EEF-8439-DF11-83E6-003048678AFA.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0009/A017DAF1-8439-DF11-8D10-0026189437FD.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0009/963EF3F2-8439-DF11-8492-00304867C1BA.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0009/9001B3F3-8439-DF11-A139-001A928116F2.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0009/8CF5C4F1-8439-DF11-99DD-0018F3D09614.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0009/7C4FF7F2-8439-DF11-9762-001A92971AAA.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0009/780909F4-8439-DF11-9FCC-001A92811734.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0009/628A62F3-8439-DF11-A977-0018F3D096F8.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0009/5EB04CF1-8439-DF11-9A46-002618FDA26D.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0009/40996AEF-8439-DF11-A569-0026189438B5.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0009/2E786739-8539-DF11-B144-002618943854.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0009/26318563-8739-DF11-81BE-003048678F02.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0009/22E8B1EF-8439-DF11-87A6-00261894384A.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0007/B89A2C9C-1138-DF11-BD56-002618943894.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0006/FC91559F-D737-DF11-92A4-0018F3D0969C.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0006/FAAD86BD-D237-DF11-BB05-002618943979.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0006/A4F64512-D137-DF11-B435-003048678B76.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0006/7E0C689B-D137-DF11-85BC-00261894393D.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0006/64EAAFA4-D037-DF11-8520-003048678CA2.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0006/2E1BF5CF-D337-DF11-A497-0026189438AE.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-RECO/START3X_V25-v1/0006/10469EA9-D037-DF11-8BF7-003048678FE0.root'] )

secFiles.extend( [
   '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/F6FEF62D-8539-DF11-9FAE-00261894398A.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/EE226634-8539-DF11-B936-003048678FEA.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/DE1014EF-8439-DF11-AB64-003048678AE4.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/D2B53AF3-8439-DF11-99F7-0018F3D0968E.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/D09DC130-8539-DF11-8328-002354EF3BE3.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/C6189A37-8539-DF11-8088-001A9281173C.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/C24701F1-8439-DF11-8DB0-001A92971B12.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/BE03B4EF-8439-DF11-9054-003048678BE6.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/B6C6D42C-8539-DF11-99C1-003048678AE4.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/B6606AF3-8439-DF11-8DB0-00261894398A.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/B2EB3636-8539-DF11-9558-0018F3D096B4.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/A00424F2-8439-DF11-9E89-0018F3D096D8.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/905BE3F0-8439-DF11-8D1E-001A928116C6.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/82DFF7ED-8439-DF11-B386-0026189438AD.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/7CA2E934-8539-DF11-A9DA-0018F3D096D8.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/741693F2-8439-DF11-AD1C-001A92810A94.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/74168DEE-8439-DF11-8238-00261894396F.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/727745F0-8439-DF11-AB4A-0018F3D096D2.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/700362EF-8439-DF11-A371-00304867C1BA.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/6E9452F2-8439-DF11-B531-0018F3D09684.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/6AB06A30-8539-DF11-8CB6-003048678BE6.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/66519725-8539-DF11-AD8E-0026189438F4.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/5E20FB27-8539-DF11-BCD1-0026189438AD.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/5A4C59F2-8439-DF11-ACA1-001A928116FC.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/582A61F0-8439-DF11-8E99-002618943834.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/56684BF2-8439-DF11-8FAF-001A928116FC.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/489E0AF2-8439-DF11-AE1E-003048678FEA.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/46C34EEF-8439-DF11-B386-00261894387B.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/46AE9A30-8539-DF11-8776-00261894396F.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/40FD45F1-8439-DF11-AA45-001A92811706.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/40BCC2F1-8439-DF11-ACC0-001A92810AA6.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/3A2584F0-8439-DF11-B516-001A9281173C.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/3026742C-8539-DF11-B640-002618943834.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/2E77FFED-8439-DF11-86C6-0026189438F4.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/2CC114F0-8439-DF11-90AC-0018F3D0967E.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/2C1B6AF0-8439-DF11-B8E3-002354EF3BE3.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/247F85F1-8439-DF11-BFE6-001A92811706.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/10993363-8739-DF11-888C-0018F3D0967E.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/0C939CF2-8439-DF11-AF8E-002618943854.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/08C4702D-8539-DF11-9310-00304867C1BA.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0009/0626F8F0-8439-DF11-8F25-0018F3D096B4.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0006/FCBC48A0-D637-DF11-B42D-001A92810AA4.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0006/F69C5BA5-D037-DF11-8D1A-0018F3D09708.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0006/E4BC42A1-D037-DF11-85ED-002618943863.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0006/BED1B391-1138-DF11-AF7F-002618FDA265.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0006/847A2E16-D137-DF11-94A9-001A92811720.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0006/6EA53D10-D137-DF11-96AD-002618943905.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0006/62D6D8A5-D037-DF11-B5A0-001A928116DA.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0006/58359B97-D137-DF11-9659-0026189437F5.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0006/56AB8636-D337-DF11-A37D-002618943826.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0006/5463AAA0-D037-DF11-BF69-0018F3D0967A.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0006/4063D99F-D437-DF11-9FD2-0026189438AE.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0006/3E5A4D87-CF37-DF11-B831-0018F3D09660.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0006/343F1FA5-D237-DF11-98CD-0026189438DB.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0006/340187A2-D337-DF11-8FA4-001A9281172E.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0006/1EAA0D95-D137-DF11-8647-0026189438BC.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0006/1E8D8995-D137-DF11-9080-0026189438D2.root',
        '/store/relval/CMSSW_3_5_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V25-v1/0006/1695AC17-D237-DF11-8D51-00261894390A.root'])


