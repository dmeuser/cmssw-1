# Colin, March 2012

import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet

def getattrGenerator( process, postfix ):
    '''A function generator to simplify the getattr syntax'''
    def fun(name):
        return getattr(process, name+postfix)
    return fun


_PFBRECO_loaded = False

def loadPFBRECO(process):
    '''The particle-flow based reconstruction sequence should be loaded once in the process.
    Not optimal, should load it only if it is not detected (hasattr)'''
    global _PFBRECO_loaded
    if _PFBRECO_loaded is False: 
        _PFBRECO_loaded = True
        process.load("CommonTools.ParticleFlow.PFBRECO_cff")


def setupPFIso(process, leptonCollection, particleName, newpostfix='PFIso'):
    '''Generic function to setup particle-based isolation for a given lepton collection.
    Returns the isolation sequence.
    You are responsible for adding it to your path.

    leptonCollection could e.g. be "gsfElectrons" or "muons"
    particleName must be either "Electron" or "Muon".
    newpostfix can be specified to define several particle-flow isolation sequences
    '''
    lepshort = None
    if particleName=='Electron':
        lepshort='el'
    elif particleName=='Muon':
        lepshort='mu'
    else:
        raise ValueError('particleName should be equal to "Electron" or "Muon"')
    
    loadPFBRECO(process)

    postfix = ''
    # ADD VETOES IN ENDCAPS!
    fullpostfix = postfix+newpostfix
    ga = getattrGenerator( process, postfix )
    ganew = getattrGenerator( process, fullpostfix )

    leptonSeq = cms.Sequence(
        ga('pf{lepton}IsolationSequence'.format(lepton=particleName))  
        )
    setattr( process, 'std{lepton}Sequence{postfix}'.format(lepton=particleName,
                                                   postfix=postfix), leptonSeq)

    leptonSource = leptonCollection
    cloneProcessingSnippet(process,
                           ga('std{lepton}Sequence'.format(lepton=particleName)),
                           newpostfix)

    ganew("{lepshort}PFIsoDepositCharged".format(lepshort=lepshort) ).src = leptonSource
    ganew("{lepshort}PFIsoDepositChargedAll".format(lepshort=lepshort)).src = leptonSource
    ganew("{lepshort}PFIsoDepositNeutral".format(lepshort=lepshort)).src = leptonSource
    ganew("{lepshort}PFIsoDepositGamma".format(lepshort=lepshort)).src = leptonSource
    ganew("{lepshort}PFIsoDepositPU".format(lepshort=lepshort)).src = leptonSource

    return ganew('std{lepton}Sequence'.format(lepton=particleName))


def setupPFMuonIso(process, muonCollection, postfix='PFIso' ):
    '''Set up particle-based isolation for the muons in muonCollection.
    '''
    return setupPFIso( process, muonCollection, 'Muon', postfix)

def setupPFElectronIso(process, electronCollection, postfix='PFIso' ):
    '''Set up particle-based isolation for the electrons in electronCollection
    '''
    return setupPFIso( process, electronCollection, 'Electron', postfix)


