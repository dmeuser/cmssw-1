// -*- C++ -*-
//
// Package:     LibraryLoader
// Class  :     AutoLibraryLoader
// 
// Implementation:
//     <Notes on implementation>
//
// Original Author:  
//         Created:  Wed Nov 30 14:55:01 EST 2005
// $Id$
//

// system include files
#include <iostream>
#include "TROOT.h"
#include "TInterpreter.h"

// user include files
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/RootAutoLibraryLoader/interface/RootAutoLibraryLoader.h"
#include "FWCore/FWLite/src/BareRootProductGetter.h"
#include "FWCore/PluginManager/interface/PluginManager.h"
#include "FWCore/PluginManager/interface/standard.h"

#include "FWCore/FWLite/interface/setRefStreamer.h"
//
// constants, enums and typedefs
//

//
// static data member definitions
//



//
// constructors and destructor
//
AutoLibraryLoader::AutoLibraryLoader()
{
}


//
// member functions
//

void
AutoLibraryLoader::enable()
{
   edmplugin::PluginManager::configure(edmplugin::standard::config());
   static BareRootProductGetter s_getter;
   static edm::EDProductGetter::Operate s_op(&s_getter);
   edm::RootAutoLibraryLoader::enable();
   //this function must be called after enabling the autoloader
   // so that the Reflex dictionaries will be converted to ROOT 
   // dictionaries and the TClass we need will be available
   fwlite::setRefStreamer(&s_getter);
   
   //Make it easy to load our headers
   TInterpreter* intrp= gROOT->GetInterpreter();
   bool foundCMSIncludes = false;
   const char* env = getenv("CMSSW_BASE");
   if( 0 != env) {
     foundCMSIncludes = true;
     std::string dir(env);
     dir += "/src";
     intrp->AddIncludePath(dir.c_str());
   }

   env = getenv("CMSSW_RELEASE_BASE");
   if( 0 != env) {
     foundCMSIncludes = true;
     std::string dir(env);
     dir += "/src";
     intrp->AddIncludePath(dir.c_str());
   }
   if( not foundCMSIncludes) {
     std::cerr <<"Could not find the environment variables \n"
     <<"  CMSSW_BASE or\n"
     <<"  CMSSW_RELEASE_BASE\n"
     <<" therefore attempting to '#include' any CMS headers will not work"<<std::endl;
   }
   
}




void
AutoLibraryLoader::loadAll()
{
  // std::cout <<"LoadAllDictionaries"<<std::endl;
  enable();
  edm::RootAutoLibraryLoader::loadAll();
}


ClassImp(AutoLibraryLoader)
