#!/usr/bin/env python
"""
_pp_

Scenario supporting proton collisions

"""

import os
import sys

from Configuration.DataProcessing.Reco import Reco
import FWCore.ParameterSet.Config as cms
from Configuration.PyReleaseValidation.ConfigBuilder import ConfigBuilder
from Configuration.PyReleaseValidation.ConfigBuilder import Options
from Configuration.PyReleaseValidation.ConfigBuilder import defaultOptions
from Configuration.DataProcessing.RecoTLR import customisePrompt,customiseExpress

class pp(Reco):
    """
    _pp_

    Implement configuration building for data processing for proton
    collision data taking

    """


    def promptReco(self, globalTag, **args):
        """
        _promptReco_

        Proton collision data taking prompt reco

        """
        if not 'skims' in args:
            args['skims']=['TkAlMinBias',
                           'TkAlMuonIsolated',
                           'MuAlCalIsolatedMu',
                           'MuAlOverlaps',
                           'HcalCalIsoTrk',
                           'HcalCalDijets',
                           'SiStripCalMinBias',
                           'EcalCalElectron',
                           'DtCalib',
                           'TkAlJpsiMuMu',
                           'TkAlUpsilonMuMu',
                           'TkAlZMuMu']
        process = Reco.promptReco(self,globalTag, **args)

        #add the former top level patches here
        customisePrompt(process)
        
        return process


    def expressProcessing(self, globalTag, **args):
        """
        _expressProcessing_

        Proton collision data taking express processing

        """
        if not 'skims' in args:
            args['skims']=['SiStripCalZeroBias',
                           'TkAlMinBias',
                           'DtCalib',
                           'MuAlCalIsolatedMu',
                           'SiStripPCLHistos']
        process = Reco.expressProcessing(self,globalTag, **args)
        
        customiseExpress(process)
                
        return process


    def alcaHarvesting(self, globalTag, datasetName, **args):
        """
        _alcaHarvesting_

        Proton collisions data taking AlCa Harvesting

        """
        if not 'skims' in args:
            args['skims']=['BeamSpotByRun',
                           'BeamSpotByLumi',
                           'SiStripQuality']
            
        return Reco.alcaHarvesting(self, globalTag, datasetName, **args)

