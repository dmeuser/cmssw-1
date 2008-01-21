#ifndef Fireworks_Core_FWViewManagerManager_h
#define Fireworks_Core_FWViewManagerManager_h
// -*- C++ -*-
//
// Package:     Core
// Class  :     FWViewManagerManager
// 
/**\class FWViewManagerManager FWViewManagerManager.h Fireworks/Core/interface/FWViewManagerManager.h

 Description: Manages all the FWViewManagerBase instances

 Usage:
    <usage>

*/
//
// Original Author:  Chris Jones
//         Created:  Tue Jan 15 10:26:23 EST 2008
// $Id: FWViewManagerManager.h,v 1.1 2008/01/15 19:48:34 chrjones Exp $
//

// system include files
#include <vector>
#include <boost/shared_ptr.hpp>

// user include files

// forward declarations
class FWViewManagerBase;
class FWEventItem;
class FWModelChangeManager;

class FWViewManagerManager
{

   public:
      FWViewManagerManager(FWModelChangeManager*);
      virtual ~FWViewManagerManager();

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

      // ---------- member functions ---------------------------
      void add( boost::shared_ptr<FWViewManagerBase>);
      void registerEventItem(const FWEventItem*iItem);
      void registerProxyBuilder(const std::string& type, 
                                const std::string& proxyBuilderName);
      void newEventAvailable();
   private:
      FWViewManagerManager(const FWViewManagerManager&); // stop default

      const FWViewManagerManager& operator=(const FWViewManagerManager&); // stop default

      // ---------- member data --------------------------------
      std::vector<boost::shared_ptr<FWViewManagerBase> > m_viewManagers;
      FWModelChangeManager* m_changeManager;
   
};


#endif
