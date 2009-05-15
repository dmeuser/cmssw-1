import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
# define names of different Monte Carlo processes
#--------------------------------------------------------------------------------

# Z --> tau+ tau- generated with Pythia + Tauola (all decay modes)
process_Ztautau = cms.PSet(
  config_dqmFileLoader = cms.PSet(
    inputFileNames = cms.vstring(''),
    scaleFactor = cms.double(1.),
    dqmDirectory_store = cms.string('Ztautau')
  ),
  config_dqmHistPlotter = cms.PSet(
    dqmDirectory = cms.string('Ztautau'),
    legendEntry = cms.string('Z #rightarrow #tau^{+} #tau^{-}'),
    type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
  )
)

# Z --> mu+ mu- generated with Pythia
process_Zmumu = cms.PSet(
  config_dqmFileLoader = cms.PSet(
    inputFileNames = cms.vstring(''),
    scaleFactor = cms.double(1.),
    dqmDirectory_store = cms.string('Zmumu')
  ),
  config_dqmHistPlotter = cms.PSet(
    dqmDirectory = cms.string('Zmumu'),
    legendEntry = cms.string('Z #rightarrow #mu^{+} #mu^{-}'),
    type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
  )
)

# Z --> e+ e- generated with Pythia
process_Zee = cms.PSet(
  config_dqmFileLoader = cms.PSet(
    inputFileNames = cms.vstring(''),
    scaleFactor = cms.double(1.),
    dqmDirectory_store = cms.string('Zee')
  ),
  config_dqmHistPlotter = cms.PSet(
    dqmDirectory = cms.string('Zee'),
    legendEntry = cms.string('Z #rightarrow e^{+} e^{-}'),
    type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
  )
)

# Z + jets generated with Madgraph
process_ZplusJets = cms.PSet(
  config_dqmFileLoader = cms.PSet(
    inputFileNames = cms.vstring(''),
    scaleFactor = cms.double(1.),
    dqmDirectory_store = cms.string('ZplusJets')
  ),
  config_dqmHistPlotter = cms.PSet(
    dqmDirectory = cms.string('ZplusJets'),
    legendEntry = cms.string('Z + jets'),
    type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
  )
)

# W + jets generated with Madgraph
process_WplusJets = cms.PSet(
  config_dqmFileLoader = cms.PSet(
    inputFileNames = cms.vstring(''),
    scaleFactor = cms.double(1.),
    dqmDirectory_store = cms.string('WplusJets')
  ),
  config_dqmHistPlotter = cms.PSet(
    dqmDirectory = cms.string('WplusJets'),
    legendEntry = cms.string('W + jets'),
    type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
  )
)

process_TTplusJets = cms.PSet(
  config_dqmFileLoader = cms.PSet(
    inputFileNames = cms.vstring(''),
    scaleFactor = cms.double(1.),
    dqmDirectory_store = cms.string('TTplusJets')
  ),
  config_dqmHistPlotter = cms.PSet(
    dqmDirectory = cms.string('TTplusJets'),
    legendEntry = cms.string('TT + jets'),
    type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
  )
)

# electron enriched QCD generated with Pythia
# in the range 20 GeV < Pt(hat) < 30 GeV
process_QCD_BCtoE_Pt20to30 = cms.PSet(
  config_dqmFileLoader = cms.PSet(
    inputFileNames = cms.vstring(''),
    scaleFactor = cms.double(1.),
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
process_QCD_BCtoE_Pt30to80 = cms.PSet(
  config_dqmFileLoader = cms.PSet(
    inputFileNames = cms.vstring(''),
    scaleFactor = cms.double(1.),
    dqmDirectory_store = cms.string('QCD_BCtoE_Pt30to80')
  ),
  config_dqmHistPlotter = cms.PSet(
    dqmDirectory = cms.string('QCD_BCtoE_Pt30to80'),
    legendEntry = cms.string('eQCD 30 < #hat{P}_T < 80 GeV'),
    type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
  )
)

# muon enriched QCD generated with Pythia
process_InclusivePPmuX = cms.PSet(
  config_dqmFileLoader = cms.PSet(
    inputFileNames = cms.vstring(''),
    scaleFactor = cms.double(1.),
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
process_PPmuXptGt20 = cms.PSet(
  config_dqmFileLoader = cms.PSet(
    inputFileNames = cms.vstring(''),
    scaleFactor = cms.double(1.),
    dqmDirectory_store = cms.string('PPmuXptGt20')
  ),
  config_dqmHistPlotter = cms.PSet(
    dqmDirectory = cms.string('PPmuXptGt20'),
    legendEntry = cms.string('pp#muX #hat{P}_T > 20 GeV'),
    type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
  )
)

