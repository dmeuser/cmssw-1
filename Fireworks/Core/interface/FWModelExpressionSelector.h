#ifndef Fireworks_Core_FWModelExpressionSelector_h
#define Fireworks_Core_FWModelExpressionSelector_h
// -*- C++ -*-
//
// Package:     Core
// Class  :     FWModelExpressionSelector
// 
/**\class FWModelExpressionSelector FWModelExpressionSelector.h Fireworks/Core/interface/FWModelExpressionSelector.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Original Author:  Chris Jones
//         Created:  Wed Jan 23 10:37:13 EST 2008
// $Id: FWModelExpressionSelector.h,v 1.1 2008/01/24 00:30:30 chrjones Exp $
//

// system include files
#include <string>

// user include files

// forward declarations
class FWEventItem;

class FWModelExpressionSelector
{

   public:
      FWModelExpressionSelector() {}
      //virtual ~FWModelExpressionSelector();

      // ---------- const member functions ---------------------
      /** Throws an FWExpressionException if there is a problem */
      bool select(FWEventItem* iItem, const std::string& iExpression) const;
   
      // ---------- static member functions --------------------

      // ---------- member functions ---------------------------

   private:
      FWModelExpressionSelector(const FWModelExpressionSelector&); // stop default

      const FWModelExpressionSelector& operator=(const FWModelExpressionSelector&); // stop default

      // ---------- member data --------------------------------

};


#endif
