#ifndef Framework_InputSourceDescription_h
#define Framework_InputSourceDescription_h

/*----------------------------------------------------------------------

InputSourceDescription : the stuff that is needed to configure an
input source that does not come in through the ParameterSet  

$Id$
----------------------------------------------------------------------*/
#include <string>
#include "DataFormats/Common/interface/PassID.h"

namespace edm {
  class ProductRegistry;

  struct InputSourceDescription {
    InputSourceDescription() : processName_(), pass(), preg_(0) { }
    InputSourceDescription(std::string const& name, PassID pid, 
			    ProductRegistry& preg) :
      processName_(name),
      pass(pid),
      preg_(&preg)
	 
    {}

    std::string const processName_;
    PassID      pass;
    ProductRegistry * preg_;
  };
}

#endif
