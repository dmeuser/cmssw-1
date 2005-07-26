#ifndef EDM_FWD_H
#define EDM_FWD_H

/*----------------------------------------------------------------------
  
Forward declarations of types in the EDM.

$Id$

----------------------------------------------------------------------*/

namespace edm
{
  class BasicHandle;
  class BranchKey;
  class EDAnalyzer;
  class EDFilter;
  class EDProducer;
  class EDProduct;
  class Event;
  class EventAux;
  class EventPrincipal;
  class EventProvenance;
  class EventRegistry;
  class EventSetup;
  class InputService;
  class InputServiceDescription;
  class LuminositySection;
  class ModuleDescription;
  class ModuleDescriptionSelector;
  class OutputModule;
  class ParameterSet;
  class ProcessNameSelector;
  class ProductDescription;
  class ProductRegistry;
  class Provenance;
  class PS_ID;
  class RefBase;
  class RefVectorBase;
  class Retriever;
  class Run;
  class RunHandler;
  class Selector;

  template <class T> class Wrapper;
  template <class T> class Handle;
  template <class T> class Ref;
  template <class T> class RefVector;
  template <class T> class RefVectorIterator;
}

// The following are trivial enough so that the real headers can be included.
#include "FWCore/EDProduct/interface/CollisionID.h"
#include "FWCore/Framework/interface/ConditionsID.h"
#include "FWCore/EDProduct/interface/EDP_ID.h"
#include "FWCore/Framework/interface/PassID.h"
#include "FWCore/Framework/interface/VersionNumber.h"

#endif
