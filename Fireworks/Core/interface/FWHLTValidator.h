#ifndef Fireworks_Core_FWHLTValidator_h
#define Fireworks_Core_FWHLTValidator_h
// -*- C++ -*-
//
// Package:     Core
// Class  :     FWHLTValidator
// $Id: FWHLTValidator.h,v 1.1 2009/10/02 17:54:40 dmytro Exp $
//

#include "Fireworks/Core/src/FWValidatorBase.h"
namespace fwlite{
  class Event;
}
namespace edm {
  class TriggerNames;
}

class FWHLTValidator: public FWValidatorBase {

public:
 FWHLTValidator(fwlite::Event& event):
  m_event(event), m_triggerNames(0){}
  virtual ~FWHLTValidator() {}

  virtual void fillOptions(const char* iBegin, const char* iEnd,
			   std::vector<std::pair<boost::shared_ptr<std::string>, std::string> >& oOptions) const;
private:
  FWHLTValidator(const FWHLTValidator&); // stop default
  const FWHLTValidator& operator=(const FWHLTValidator&); // stop default
  
  // ---------- member data --------------------------------
  mutable fwlite::Event& m_event;
  mutable std::vector<std::string> m_triggerNames;
};


#endif
