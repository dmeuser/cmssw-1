#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/MuonReco/interface/Muon.h"

#include "SusyAnalysis/EventSelector/interface/inconsistentMuonSelector.h"

//________________________________________________________________________________________
inconsistentMuonSelector::inconsistentMuonSelector (const edm::ParameterSet& pset ) :
        SusyEventSelector(pset),
        muonTag_(pset.getParameter<edm::InputTag> ("muonTag")),
        ptMin_( pset.getParameter<double> ("ptMin")),
        maxPTDiff_( pset.getParameter<double> ("maxPTDiff")),
        verbose_( pset.getParameter<bool> ("verbose"))
{

    // Define variables to store
    // defineVariable("myVariable");

}

//________________________________________________________________________________________
bool
inconsistentMuonSelector::select (const edm::Event& event) const
{

    using namespace std;
    using namespace edm;

    // Input collections
    //edm::Handle<std::vector<pat::Muon> > muonHandle;
    edm::Handle<std::vector<reco::PFCandidate> > muonHandle;
    bool muonOK = event.getByLabel(muonTag_, muonHandle);

    // Reset cached variables
    resetVariables();

    // Compute variables

    if (!muonOK) return true;

    bool foundMuon = false;

    //for (std::vector<pat::Muon>::const_iterator im = (*muonHandle).begin(); im != (*muonHandle).end(); ++im) {
    for (std::vector<reco::PFCandidate>::const_iterator im = (*muonHandle).begin(); im != (*muonHandle).end(); ++im) {

        if (!im->muonRef()) continue;

        reco::Muon p = *(im->muonRef());

        if ( p.pt() < ptMin_ ) continue;

        if (!(p.isGlobalMuon())) continue;

        if ( fabs(p.innerTrack()->pt()/p.globalTrack()->pt() - 1) <= maxPTDiff_)
            continue;

        foundMuon = true;

        if ( verbose_ ) {
            cout << "\t" << "tracker pT = " << p.innerTrack()->pt() << endl;
            cout << "\t" << "global fit pT = " << p.globalTrack()->pt() << endl;
        }
    } // end loop over PF muons

    return !foundMuon;

    // Store variables
    // setVariable("myVariable",myValue);

    // Return selection
    // return (myValue > minValue);

}

//__________________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, inconsistentMuonSelector, "inconsistentMuonSelector");

