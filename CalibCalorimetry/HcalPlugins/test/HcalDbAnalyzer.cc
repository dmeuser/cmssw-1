// -*- C++ -*-
//
// Package:    WhatsItAnalyzer
// Class:      WhatsItAnalyzer
// 
/**\class WhatsItAnalyzer WhatsItAnalyzer.cc test/WhatsItAnalyzer/src/WhatsItAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Chris Jones
//         Created:  Fri Jun 24 19:13:25 EDT 2005
// $Id: HcalDbAnalyzer.cc,v 1.5 2005/12/05 00:25:32 fedor Exp $
//
//


// system include files
#include <memory>
#include <time.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalDbServiceHardcode.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"


class HcalDbAnalyzer : public edm::EDAnalyzer {
   public:
      explicit HcalDbAnalyzer( const edm::ParameterSet& );
      ~HcalDbAnalyzer ();


      virtual void analyze( const edm::Event&, const edm::EventSetup& );
   private:
      // ----------member data ---------------------------
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
HcalDbAnalyzer::HcalDbAnalyzer( const edm::ParameterSet& iConfig )
{
  std::cout << "HcalDbAnalyzer::HcalDbAnalyzer->..." << std::endl;
   //now do what ever initialization is needed

}


HcalDbAnalyzer::~HcalDbAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
HcalDbAnalyzer::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  std::cout << "HcalDbAnalyzer::analyze->..." << std::endl;
  edm::ESHandle<HcalDbService> pSetup;
  iSetup.get<HcalDbRecord>().get( pSetup );
  std::cout << "HcalDbAnalyzer::analyze-> got HcalDbRecord: " << std::endl;
  std::cout << "HcalDbAnalyzer::analyze-> getting information for HB channel eta=1, phi=1, depth=1..." << std::endl;
  HcalDetId cell (HcalBarrel, 1, 1, 1);
  std::auto_ptr<HcalCalibrations> calibrations = pSetup->getHcalCalibrations (cell);
  std::auto_ptr<HcalCalibrationWidths> widths = pSetup->getHcalCalibrationWidths (cell);
  std::auto_ptr<HcalChannelCoder> coder = pSetup->getChannelCoder (cell);
  const QieShape* shape = pSetup->getBasicShape ();
  
  std::cout << "Values-> pedestals: " 
	    << calibrations->pedestal (0) << '/'
	    << calibrations->pedestal (1) << '/'
	    << calibrations->pedestal (2) << '/'
	    << calibrations->pedestal (3)
	    << ",  gains: "
	    << calibrations->gain (0) << '/'
	    << calibrations->gain (1) << '/'
	    << calibrations->gain (2) << '/'
	    << calibrations->gain (3)
	    << std::endl;
  std::cout << "Widths-> pedestals: " 
	    << widths->pedestal (0) << '/'
	    << widths->pedestal (1) << '/'
	    << widths->pedestal (2) << '/'
	    << widths->pedestal (3)
	    << ",  gains: "
	    << widths->gain (0) << '/'
	    << widths->gain (1) << '/'
	    << widths->gain (2) << '/'
	    << widths->gain (3) 
	    << std::endl;
  
  std::cout << "QIE shape:" << std::endl;
  for (int i = 0; i < 128; i++) {
    double q0 = coder->charge (*shape, i, 0);
    double q1 = coder->charge (*shape, i, 1);
    double q2 = coder->charge (*shape, i, 2);
    double q3 = coder->charge (*shape, i, 3);
    std::cout << " ADC: " << i << " q1:" << q0 << " q2:" << q1 << " q3:" << q2 << " q4:" << q3 << std::endl;
  }
  
  // dump mapping
//   std::auto_ptr <HcalMapping> emap = pSetup->getHcalMapping ();
//   std::cout << "Mapping: all Hcal IDs:" << std::endl;
//   std::vector <HcalElectronicsId> detIds = emap->allElectronicsId ();
//   std::vector <HcalElectronicsId>::iterator id = detIds.begin ();
//   for (; id != detIds.end (); id++) {
//     std::cout << "ElectronicsID: " << *id << " , Detector ID: " << emap->lookup (*id) 
// 	      << " , Trigger ID: " << emap->lookupTrigger (*id) << std::endl;
//   }
  
//   int nTryes = 100000000;
//   std::cout << "Check pefermance of HcalCalibrations for " << nTryes << " accesses..." << std::endl;
//   int iTry = nTryes;
//   time_t time0 = time (0);
//   double ped = 0;
//   double gain = 0;
//   while (--iTry >= 0) {
//     //    std::auto_ptr<HcalCalibrations> calibrations2 = pSetup->getHcalCalibrations (cell);
//     HcalCalibrations calibrations2_o = *calibrations;
//     HcalCalibrations* calibrations2 = &calibrations2_o;
//     ped = calibrations2->pedestal (0);
//     gain = calibrations2->gain (0);
//     if (iTry > nTryes) std::cout << ped + gain << std::endl;
//   }
//   time_t time1 = time (0);
//   std::cout << "took " << time1 - time0 << " sec for " << nTryes << " accesses, i.e. " << float (time1 - time0) / nTryes << " sec/access" << std::endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(HcalDbAnalyzer)
