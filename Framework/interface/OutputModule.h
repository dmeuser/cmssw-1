#ifndef FWCore_Framework_OutputModule_h
#define FWCore_Framework_OutputModule_h

/*----------------------------------------------------------------------
  
OutputModule: The base class of all "modules" that write Events to an
output stream.

$Id$

----------------------------------------------------------------------*/

#include "boost/array.hpp"
#include <vector>

#include "DataFormats/Provenance/interface/BranchType.h"
#include "DataFormats/Provenance/interface/ModuleDescription.h"

#include "FWCore/Framework/interface/CachedProducts.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/GroupSelector.h"

namespace edm {

  typedef edm::detail::CachedProducts::handle_t Trig;
   
  std::vector<std::string> const& getAllTriggerNames();


  class OutputModule {
  public:
    friend class OutputWorker;
    typedef OutputModule ModuleType;
    typedef std::vector<BranchDescription const *> Selections;
    typedef boost::array<Selections, EndBranchType> SelectionsArray;

    explicit OutputModule(ParameterSet const& pset);
    virtual ~OutputModule();
    void doBeginJob(EventSetup const&);
    void doEndJob();
    void writeEvent(EventPrincipal const& e, ModuleDescription const& d,
		    CurrentProcessingContext const* c);
    void doBeginRun(RunPrincipal const& e, ModuleDescription const& d,
		    CurrentProcessingContext const* c);
    void doEndRun(RunPrincipal const& e, ModuleDescription const& d,
		    CurrentProcessingContext const* c);
    void doBeginLuminosityBlock(LuminosityBlockPrincipal const& e, ModuleDescription const& d,
		    CurrentProcessingContext const* c);
    void doEndLuminosityBlock(LuminosityBlockPrincipal const& e, ModuleDescription const& d,
		    CurrentProcessingContext const* c);
    bool selected(BranchDescription const& desc) const;

    unsigned int nextID() const;
    void selectProducts();
    int eventCount() const {return eventCount_;}

  protected:
    //const Trig& getTriggerResults(Event const& ep) const;
    Trig getTriggerResults(Event const& ep) const;

    // This function is needed for compatibility with older code. We
    // need to clean up the use of Event and EventPrincipal, to avoid
    // creation of multiple Event objects when handling a single
    // event.
    Trig getTriggerResults(EventPrincipal const& ep) const;

    // The returned pointer will be null unless the this is currently
    // executing its event loop function ('write').
    CurrentProcessingContext const* currentContext() const;

  private:

    unsigned int nextID_;
    // TODO: Make these data members private, and give OutputModule
    // an interface (protected?) that supplies client code with the
    // needed functionality *without* giving away implementation
    // details ... don't just return a reference to descVec_, because
    // we are looking to have the flexibility to change the
    // implementation of descVec_ without modifying clients. When this
    // change is made, we'll have a one-time-only task of modifying
    // clients (classes derived from OutputModule) to use the
    // newly-introduced interface.
    // ditto for droppedVec_.
    // TODO: Consider using shared pointers here?

    // descVec_ are pointers to the BranchDescription objects describing
    // the branches we are to write.
    // droppedVec_ are pointers to the BranchDescription objects describing
    // the branches we are NOT to write.
    // 
    // We do not own the BranchDescriptions to which we point.
  protected:
    SelectionsArray descVec_;
    SelectionsArray droppedVec_;
    boost::array<bool, EndBranchType> hasNewlyDroppedBranch_;

  private:
    virtual void write(EventPrincipal const& e) = 0;
    //bool wantEvent(Event const& e);
    virtual void beginJob(EventSetup const&){}
    virtual void endJob(){}
    virtual void beginRun(RunPrincipal const& r){}
    virtual void endRun(RunPrincipal const& r) = 0;
    virtual void beginLuminosityBlock(LuminosityBlockPrincipal const& lb){}
    virtual void endLuminosityBlock(LuminosityBlockPrincipal const& lb) = 0;

    std::string process_name_;
    GroupSelector groupSelector_;
    //std::vector<NamedEventSelector> eventSelectors_;
    //ProcessNameSelector selectResult_;
    
    void setModuleDescription(ModuleDescription const& md) {
      moduleDescription_ = md;
    }
    ModuleDescription moduleDescription_;

    // We do not own the pointed-to CurrentProcessingContext.
    CurrentProcessingContext const* current_context_;

    //This will store TriggerResults objects for the current event.
    // mutable std::vector<Trig> prods_;
    mutable bool prodsValid_;

    //Store the current Module Desc
    //  *** This should be superfluous, because current_context_->moduleDescription()
    // returns a pointer to the current ModuleDescription.
    ModuleDescription const* current_md_;  

    bool wantAllEvents_;
    mutable detail::CachedProducts selectors_;

    int eventCount_;
  };
}

#endif
