import FWCore.ParameterSet.Config as cms
import copy

dataIntLumi = float(200.)

#--------------------------------------------------------------------------------
# define names of different Monte Carlo processes
#--------------------------------------------------------------------------------

# Z --> tau+ tau- generated with Pythia + Tauola (all decay modes)
# integrated luminosity = 1289 pb^-1
process_Ztautau = cms.PSet(
  config_dqmFileLoader = cms.PSet(
    inputFileNames = cms.vstring(''),
    scaleFactor = cms.double(dataIntLumi/1289.),
    dqmDirectory_store = cms.string('Ztautau')
  ),
  config_dqmHistPlotter = cms.PSet(
    dqmDirectory = cms.string('Ztautau'),
    legendEntry = cms.string('Z #rightarrow #tau^{+} #tau^{-}'),
    type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
  )
)

# Z --> mu+ mu- generated with Pythia
# integrated luminosity = 905 pb^-1
process_Zmumu = cms.PSet(
  config_dqmFileLoader = cms.PSet(
    inputFileNames = cms.vstring(''),
    scaleFactor = cms.double(dataIntLumi/905.),
    dqmDirectory_store = cms.string('Zmumu')
  ),
  config_dqmHistPlotter = cms.PSet(
    dqmDirectory = cms.string('Zmumu'),
    legendEntry = cms.string('Z #rightarrow #mu^{+} #mu^{-}'),
    type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
  )
)

# Z --> e+ e- generated with Pythia
# integrated luminosity = 127 pb^-1
process_Zee = cms.PSet(
  config_dqmFileLoader = cms.PSet(
    inputFileNames = cms.vstring(''),
    scaleFactor = cms.double(dataIntLumi/127.),
    dqmDirectory_store = cms.string('Zee')
  ),
  config_dqmHistPlotter = cms.PSet(
    dqmDirectory = cms.string('Zee'),
    legendEntry = cms.string('Z #rightarrow e^{+} e^{-}'),
    type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
  )
)

# Z + jets generated with Madgraph
# integrated luminosity = 270 pb^-1
#
# (NOTE: for Monte Carlo samples generated by Madgraph,
#        the filter efficiency is already included in the cross-sections
#        listed at https://twiki.cern.ch/twiki/bin/view/CMS/ProductionSummer2008 !!)
#
process_ZplusJets = cms.PSet(
  config_dqmFileLoader = cms.PSet(
    inputFileNames = cms.vstring(''),
    scaleFactor = cms.double(dataIntLumi/270.),
    dqmDirectory_store = cms.string('ZplusJets')
  ),
  config_dqmHistPlotter = cms.PSet(
    dqmDirectory = cms.string('ZplusJets'),
    legendEntry = cms.string('Z + jets'),
    type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
  )
)

# W + jets generated with Madgraph
# integrated luminosity = 250 pb^-1
#
# (NOTE: for Monte Carlo samples generated by Madgraph,
#        the filter efficiency is already included in the cross-sections
#        listed at https://twiki.cern.ch/twiki/bin/view/CMS/ProductionSummer2008 !!)
#
process_WplusJets = cms.PSet(
  config_dqmFileLoader = cms.PSet(
    inputFileNames = cms.vstring(''),
    scaleFactor = cms.double(dataIntLumi/250.),
    dqmDirectory_store = cms.string('WplusJets')
  ),
  config_dqmHistPlotter = cms.PSet(
    dqmDirectory = cms.string('WplusJets'),
    legendEntry = cms.string('W + jets'),
    type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
  )
)

# electron enriched QCD generated with Pythia
# in the range 20 GeV < Pt(hat) < 30 GeV
# integrated luminosity = 9.15 pb^-1
process_QCD_BCtoE_Pt20to30 = cms.PSet(
  config_dqmFileLoader = cms.PSet(
    inputFileNames = cms.vstring(''),
    scaleFactor = cms.double(dataIntLumi/9.15),
    dqmDirectory_store = cms.string('QCD_BCtoE_Pt20to30')
  ),
  config_dqmHistPlotter = cms.PSet(
    dqmDirectory = cms.string('QCD_BCtoE_Pt20to30'),
    legendEntry = cms.string('eQCD 20 < #hat{P}_T < 30 GeV'),
    type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
  )
)

# electron enriched QCD generated with Pythia
# in the range 30 GeV < Pt(hat) < 80 GeV
# integrated luminosity = 7.34 pb^-1
process_QCD_BCtoE_Pt30to80 = cms.PSet(
  config_dqmFileLoader = cms.PSet(
    inputFileNames = cms.vstring(''),
    scaleFactor = cms.double(dataIntLumi/7.34),
    dqmDirectory_store = cms.string('QCD_BCtoE_Pt30to80')
  ),
  config_dqmHistPlotter = cms.PSet(
    dqmDirectory = cms.string('QCD_BCtoE_Pt30to80'),
    legendEntry = cms.string('eQCD 30 < #hat{P}_T < 80 GeV'),
    type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
  )
)

# muon enriched QCD generated with Pythia
# integrated luminosity = 0.044 pb^-1
process_InclusivePPmuX = cms.PSet(
  config_dqmFileLoader = cms.PSet(
    inputFileNames = cms.vstring(''),
    scaleFactor = cms.double(dataIntLumi/0.044),
    dqmDirectory_store = cms.string('InclusivePPmuX')
  ),
  config_dqmHistPlotter = cms.PSet(
    dqmDirectory = cms.string('InclusivePPmuX'),
    legendEntry = cms.string('pp#muX'),
    type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
  )
)

# muon enriched QCD generated with Pythia
# in the range Pt(hat) > 20 GeV
# integrated luminosity = 49.7 pb^-1
process_PPmuXptGt20 = cms.PSet(
  config_dqmFileLoader = cms.PSet(
    inputFileNames = cms.vstring(''),
    scaleFactor = cms.double(dataIntLumi/49.7),
    dqmDirectory_store = cms.string('PPmuXptGt20')
  ),
  config_dqmHistPlotter = cms.PSet(
    dqmDirectory = cms.string('PPmuXptGt20'),
    legendEntry = cms.string('pp#muX #hat{P}_T > 20 GeV'),
    type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
  )
)

