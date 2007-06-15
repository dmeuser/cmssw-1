#ifndef Common_TriggerResults_h
#define Common_TriggerResults_h

/** \class TriggerResults
 *
 *  Original Authors: Jim Kowalkowski 13-01-06
 *                    Martin Grunewald
 *  $Id: TriggerResults.h,v 1.6 2007/03/04 04:59:59 wmtan Exp $
 *
 *  The trigger path results are maintained here as a sequence of
 *  entries, one per trigger path.  They are assigned in the order
 *  they appeared in the process-level pset.  (They are actually
 *  stored in the base class HLTGlobalStatus)
 *
 *  The ParameterSetID can be used to get a ParameterSet from
 *  the registry of parameter sets.  This ParameterSet contains
 *  a vector<string> named "trigger_paths" that contains the
 *  trigger path names in the same order as the trigger path
 *  results stored here.
 *
 *  The vector<string> contained in this class is empty and
 *  no longer used.  It is kept for backward compatibility reasons.
 *  In early versions of the code, the trigger results paths names
 *  were stored there.
 *
 */

#include "DataFormats/Common/interface/HLTGlobalStatus.h"
#include "DataFormats/Provenance/interface/ParameterSetID.h"

#include <string>
#include <vector>

namespace edm
{
  class TriggerResults : public HLTGlobalStatus {

    typedef std::vector<std::string> Strings;

  private:
    edm::ParameterSetID psetid_;

    // Not used anymore
    Strings             names_;

  public:
    TriggerResults() : HLTGlobalStatus(), psetid_(), names_() { }

    TriggerResults(const HLTGlobalStatus& hlt, const edm::ParameterSetID& psetid)
      : HLTGlobalStatus(hlt), psetid_(psetid), names_() { }

    TriggerResults(const HLTGlobalStatus& hlt, const Strings& names)
      : HLTGlobalStatus(hlt), psetid_(), names_(names) { }

    const ParameterSetID& parameterSetID() const { return psetid_; }

    // The next three functions are OBSOLETE and should only be used for backward
    // compatibility to older data.  The names_ vector is always empty in new data.

    const std::vector<std::string>& getTriggerNames() const { return names_; }

    const std::string& name(unsigned int i) const {return names_.at(i);}

    unsigned int find (const std::string& name) const {
      const unsigned int n(size());
      for (unsigned int i = 0; i != n; ++i) if (names_[i] == name) return i;
      return n;
    }
  };
}

#endif
