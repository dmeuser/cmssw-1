// -*- C++ -*-
//
// Package:     ServiceRegistry
// Class  :     ActivityRegistry
// 
// Implementation:
//     <Notes on implementation>
//
// Original Author:  Chris Jones
//         Created:  Tue Sep  6 10:26:49 EDT 2005
// $Id$
//

// system include files

// user include files
#include "FWCore/ServiceRegistry/interface/ActivityRegistry.h"

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
//edm::ActivityRegistry::ActivityRegistry()
//{
//}
   
// ActivityRegistry::ActivityRegistry(const ActivityRegistry& rhs)
// {
//    // do actual copying here;
// }

//edm::ActivityRegistry::~ActivityRegistry()
//{
//}
   
//
// assignment operators
//
// const ActivityRegistry& ActivityRegistry::operator=(const ActivityRegistry& rhs)
// {
//   //An exception safe implementation is
//   ActivityRegistry temp(rhs);
//   swap(rhs);
//
//   return *this;
// }

//
// member functions
//
void 
edm::ActivityRegistry::connect(ActivityRegistry& iOther)
{
   postBeginJobSignal_.connect(iOther.postBeginJobSignal_);
   postEndJobSignal_.connect(iOther.postEndJobSignal_);

   jobFailureSignal_.connect(iOther.jobFailureSignal_);

   preProcessEventSignal_.connect(iOther.preProcessEventSignal_);
   postProcessEventSignal_.connect(iOther.postProcessEventSignal_);

   preModuleSignal_.connect(iOther.preModuleSignal_);
   postModuleSignal_.connect(iOther.postModuleSignal_);

   preModuleConstructionSignal_.connect(iOther.preModuleConstructionSignal_);
   postModuleConstructionSignal_.connect(iOther.postModuleConstructionSignal_);

   preSourceConstructionSignal_.connect(iOther.preSourceConstructionSignal_);
   postSourceConstructionSignal_.connect(iOther.postSourceConstructionSignal_);

}

//
// const member functions
//

//
// static member functions
//
