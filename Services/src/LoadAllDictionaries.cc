// -*- C++ -*-
//
// Package:     Services
// Class  :     LoadAllDictionaries
// 
// Implementation:
//     <Notes on implementation>
//
// Original Author:  Chris Jones
//         Created:  Thu Sep 15 09:47:48 EDT 2005
// $Id$
//

// system include files
#include "Cintex/Cintex.h"

// user include files
#include "FWCore/Services/src/LoadAllDictionaries.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/PluginManager/interface/PluginManager.h"
#include "FWCore/PluginManager/interface/ModuleCache.h"
#include "FWCore/PluginManager/interface/Module.h"
#include "FWCore/PluginManager/interface/PluginCapabilities.h"

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
edm::service::LoadAllDictionaries::LoadAllDictionaries(const edm::ParameterSet& iConfig)
{
   bool doLoad(iConfig.getUntrackedParameter("doLoad",true));
   if(doLoad) {
     ROOT::Cintex::Cintex::Enable();

     edmplugin::PluginManager*db =  edmplugin::PluginManager::get();
     
     typedef edmplugin::PluginManager::CategoryToInfos CatToInfos;
     
     CatToInfos::const_iterator itFound = db->categoryToInfos().find("Capability");
     
     if(itFound == db->categoryToInfos().end()) {
       return;
     }
     std::string lastClass;
     const std::string cPrefix("LCGReflex/");
     const std::string mystring("edm::Wrapper");

     for (edmplugin::PluginManager::Infos::const_iterator itInfo = itFound->second.begin(),
          itInfoEnd = itFound->second.end(); 
          itInfo != itInfoEnd; ++itInfo)
     {
       if (lastClass == itInfo->name_) {
         continue;
       }
       
       lastClass = itInfo->name_;
       if (lastClass.find(mystring) != std::string::npos) { 
         edmplugin::PluginCapabilities::get()->load(lastClass);
       }
       //NOTE: since we have the library already, we could be more efficient if we just load it ourselves
     }
   }
}

