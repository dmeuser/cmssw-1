import FWCore.ParameterSet.Config as cms

from PhysicsTools.PFCandProducer.pfAllChargedHadrons_cfi import *
from PhysicsTools.PFCandProducer.pfAllNeutralHadrons_cfi import *
from PhysicsTools.PFCandProducer.pfAllPhotons_cfi import *

patPFCandidateIsoDepositSelection = cms.Sequence(pfAllChargedHadrons * pfAllNeutralHadrons * pfAllPhotons)

