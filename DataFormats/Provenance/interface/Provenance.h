#ifndef DataFormats_Provenance_Provenance_h
#define DataFormats_Provenance_Provenance_h

/*----------------------------------------------------------------------
  
Provenance: The full description of a product and how it came into
existence.

$Id: Provenance.h,v 1.6 2008/01/30 00:17:51 wmtan Exp $
----------------------------------------------------------------------*/
#include <iosfwd>

#include "DataFormats/Provenance/interface/BranchDescription.h"
#include "DataFormats/Provenance/interface/EntryDescription.h"
#include "DataFormats/Provenance/interface/ConstBranchDescription.h"
#include "DataFormats/Provenance/interface/ProvenanceDelayedReader.h"
#include "boost/shared_ptr.hpp"

/*
  Provenance

  definitions:
  Product: The EDProduct to which a provenance object is associated

  Creator: The EDProducer that made the product.

  Parents: The EDProducts used as input by the creator.
*/

namespace edm {
  class Provenance {
  public:
    Provenance(BranchDescription const& p, bool present);
    Provenance(ConstBranchDescription const& p, bool present);
    Provenance(BranchDescription const& p, boost::shared_ptr<EntryDescription> e, bool present);
    Provenance(ConstBranchDescription const& p, boost::shared_ptr<EntryDescription> e, bool present);
    Provenance(BranchDescription const& p, EntryDescription const& e, bool present);
    Provenance(ConstBranchDescription const& p, EntryDescription const& e, bool present);

    ~Provenance() {}

    void setEvent(boost::shared_ptr<EntryDescription> e) const;

    BranchDescription const& product() const {return product_.me();}
    EntryDescription const& event() const {if (event_.get()) return *event_; return resolve();}
    EntryDescription const& resolve() const;
    boost::shared_ptr<EntryDescription> entryDescription()  const {return event_;}
    std::string const& branchName() const {return product().branchName();}
    std::string const& className() const {return product().className();}
    std::string const& moduleLabel() const {return product().moduleLabel();}
    std::string const& moduleName() const {return event().moduleName();}
    PassID const& passID() const {return event().passID();}
    std::string const& processName() const {return product().processName();}
    ProductID const& productID() const {return product().productID();}
    std::string const& productInstanceName() const {return product().productInstanceName();}
    std::string const& friendlyClassName() const {return product().friendlyClassName();}
    std::set<ParameterSetID> const& psetIDs() const {return product().psetIDs();}
    ParameterSetID const& psetID() const {return event().psetID();}
    ReleaseVersion const& releaseVersion() const {return event().releaseVersion();}
    std::set<std::string> const& branchAliases() const {return product().branchAliases();}
    ModuleDescriptionID const& moduleDescriptionID() const {return event().moduleDescriptionID();}
    ModuleDescription const& moduleDescription() const {return event().moduleDescription();}
    bool const& isPresent() const {return isPresent_;}

    std::vector<ProductID> const& parents() const {return event().parents();}

    void write(std::ostream& os) const;
    void setStore(boost::shared_ptr<ProvenanceDelayedReader> store) {store_ = store;}

  private:
    ConstBranchDescription const product_;
    mutable boost::shared_ptr<EntryDescription> event_;
    mutable bool isPresent_;
    boost::shared_ptr<ProvenanceDelayedReader> store_;
  };
  
  inline
  std::ostream&
  operator<<(std::ostream& os, Provenance const& p) {
    p.write(os);
    return os;
  }

  bool operator==(Provenance const& a, Provenance const& b);
}
#endif
