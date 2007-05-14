// -*- C++ -*-
//
// Package:    MuEnrichType1Filter
// Class:      MuEnrichType1Filter
// 
/**\class MuEnrichType1Filter MuEnrichType1Filter.cc HLTrigger/MuEnrichType1Filter/src/MuEnrichType1Filter.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Muriel VANDER DONCKT *:0
//         Created:  Fri Apr 27 17:05:15 CEST 2007
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

 
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "HLTrigger/Muon/test/MuEnrichType1Filter.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"



//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
MuEnrichType1Filter::MuEnrichType1Filter(const edm::ParameterSet& iConfig)
{
  type = iConfig.getParameter<int>("type");
   //now do what ever initialization is needed
  nrejected=0;
  naccepted=0;
}


MuEnrichType1Filter::~MuEnrichType1Filter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
MuEnrichType1Filter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;
   using namespace HepMC;
   if (type !=1 ) return true;

   Handle< HepMCProduct > EvtHandle ;
   iEvent.getByLabel( "VtxSmeared", EvtHandle ) ;
   const GenEvent* Evt = EvtHandle->GetEvent() ;
   if (Evt != 0 ) {   
     edm::LogVerbatim ("MuEnrichFltr")  << "------------------------------";
     for ( HepMC::GenEvent::particle_const_iterator
	     part=Evt->particles_begin(); part!=Evt->particles_end(); ++part )
       {
	 if ( abs((*part)->pdg_id()) == 13 ) {
	   double pt=(*part)->momentum().perp();
           edm::LogVerbatim ("MuEnrichFltr")  << "Found a muon with pt"<<pt;
	   if ( pt>4 ) {
	     nrejected++;
	     return false;
	     break;
	   }
	 }
       }
   }
   naccepted++;
   return true;
}

// ------------ method called once each job just before starting event loop  ------------
void 
MuEnrichType1Filter::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MuEnrichType1Filter::endJob() {
  edm::LogVerbatim ("MuEnrichFltr")  << "Total events"<<naccepted+nrejected;
  edm::LogVerbatim ("MuEnrichFltr")  << "Acccepted events"<<naccepted;
  edm::LogVerbatim ("MuEnrichFltr")  << "Rejected events"<<nrejected;
}

//define this as a plug-in
