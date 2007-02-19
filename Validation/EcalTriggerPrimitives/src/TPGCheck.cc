// -*- C++ -*-
//
// Package:    TPGCheck
// Class:      TPGCheck
// 
/**\class TPGCheck TPGCheck.cc Validation/TPGSimulation/src/TPGCheck.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Muriel Cerutti
//         Created:  Thu Oct 26 10:47:17 CEST 2006
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"

#include <vector>
#include <string>

#include "TH1I.h"
#include "TFile.h"

using namespace edm;
using namespace std;

//
// class decleration
//

class TPGCheck : public edm::EDAnalyzer {
   public:
      explicit TPGCheck(const edm::ParameterSet&);
      ~TPGCheck();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
      TH1I *ecal_et_[2];
      TH1I *ecal_tt_[2];
      TH1I * ecal_fgvb_[2];
      
      TFile *histFile_;
      std::string label_;
      std::string producer_;
      std::vector<std::string> ecal_parts_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
TPGCheck::TPGCheck(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
   
  ecal_parts_.push_back("Barrel");
  ecal_parts_.push_back("Endcap");

  histFile_=new TFile("histos.root","RECREATE");
  for (unsigned int i=0;i<2;++i) {
    ecal_et_[i]=new TH1I(ecal_parts_[i].c_str(),"Et",255,0,255);
    char title[30];
    sprintf(title,"%s_ttf",ecal_parts_[i].c_str());
    ecal_tt_[i]=new TH1I(title,"TTF",10,0,10);
    sprintf(title,"%s_fgvb",ecal_parts_[i].c_str());
    ecal_fgvb_[i]=new TH1I(title,"FGVB",10,0,10);
  }
  
  label_= iConfig.getParameter<std::string>("Label");
  producer_= iConfig.getParameter<std::string>("Producer");
  
}


TPGCheck::~TPGCheck()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
   
   histFile_->Write();
   histFile_->Close();

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
TPGCheck::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  
  // Get input
  edm::Handle<EcalTrigPrimDigiCollection> tp;
  iEvent.getByLabel(label_,producer_,tp);
  for (unsigned int i=0;i<tp.product()->size();i++) {  
    EcalTriggerPrimitiveDigi d=(*(tp.product()))[i]; 
    int subdet=d.id().subDet()-1;
    // for endcap, regroup double TP-s that are generated for the 2 interior rings
    if (subdet==0) {
      ecal_et_[subdet]->Fill(d.compressedEt());
    }
    else {
      if (d.id().ietaAbs()==27 || d.id().ietaAbs()==28) {
	if (i%2) ecal_et_[subdet]->Fill(d.compressedEt()*2.);
      }
      else ecal_et_[subdet]->Fill(d.compressedEt());
    }
    ecal_tt_[subdet]->Fill(d.ttFlag());
    ecal_fgvb_[subdet]->Fill(d.fineGrain());
  }
}


// ------------ method called once each job just before starting event loop  ------------
void 
TPGCheck::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TPGCheck::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(TPGCheck);
