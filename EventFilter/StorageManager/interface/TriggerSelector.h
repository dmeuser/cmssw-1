// $Id: TriggerSelector.h,v 1.3 2009/12/01 17:56:15 smorovic Exp $
/// @file: TriggerSelector.h 

#ifndef EventFilter_StorageManager_TriggerSelector_h
#define EventFilter_StorageManager_TriggerSelector_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/HLTPathStatus.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/Provenance/interface/ParameterSetID.h"
#include "FWCore/Framework/interface/EventSelector.h"

#include "boost/shared_ptr.hpp"

#include <vector>
#include <string>

namespace stor
{

  /**
   * Event selector allowing for and/not combination of triggers/paths
   *
   * $Author: smorovic $
   * $Revision: 1.3 $
   * $Date: 2009/12/01 17:56:15 $
   */

  class TriggerSelector
  {
    public:

      typedef std::vector<std::string> Strings;

      /**
       * Obsolete: Initializes TriggerSelector to use edm::EventSelector for selection.
       */
      TriggerSelector(Strings const& pathspecs,
	  Strings const& names);

      /**
       * Takes ParameterSet wth TriggerSelector string or EventSelection list, and a list of triggers.
       * if old_ is true, it is forced to use EventSelection.
       */
      TriggerSelector(edm::ParameterSet const& pset,
	  Strings const& triggernames, bool old_ = false);

      /**
       * Takes selection string and list of triggers
       */
      TriggerSelector(std::string const& expression, Strings const& triggernames);

      ~TriggerSelector() {};

      /**
       * Returns status of always positive bit
       */
      bool wantAll() const { return accept_all_; }

      /**
       * Evaluates if trigger results pass selection
       */
      bool acceptEvent(edm::TriggerResults const&) const;

      /*
       * Takes array of trigger results and a number of triggers in array and returns
       * if it passes selection
       */
      bool acceptEvent(unsigned char const*, int) const;

      /*
       * Returns if HLTGlobalStatus passes selection
       */
      bool returnStatus(edm::HLTGlobalStatus const& trStatus) const {
	return masterElement_->returnStatus(trStatus);
      }

      /*
       * Does XMl compatible formatting of the selection string
       */
      static std::string makeXMLString(std::string const& input);

      /*
       * Obsolete: Returns SelectedEvents vector from ParameterSet
       */
      static std::vector<std::string>
	getEventSelectionVString(edm::ParameterSet const& pset);

    private:

      bool accept_all_;

      /*
       * Starts parsing selection string
       */
      void init(std::string const& path, Strings const& triggernames);

      /* 
       * Removes extra spaces from string
       */
      static std::string trim(std::string input);

      /*
       * Class used for storing internal representation of the selection string
       */
      class TreeElement {

	enum TreeOperator {
	  NonInit = 0,
	  AND = 1,
	  OR  = 2,
	  NOT = 3,
	  BR = 4
	};

	public:

	/*
	 * Parser of selection string. Splits string into tokens and initializes new elements to parse them.
	 */
	TreeElement(std::string const& inputString,Strings const& tr,TreeElement* parentElement = NULL);
	~TreeElement();

	/*
	 * Returns selection status of current element calculated recursively from it's child elements
	 */
	bool returnStatus(edm::HLTGlobalStatus const& trStatus) const;

	/*
	 * Returns operator type of the element
	 */
	TreeOperator op() const {return op_;}

	/*
	 * Returns parent element
	 */
	TreeElement * parent() const {return parent_;}

	private:

	TreeElement * parent_;
	std::vector<TreeElement*> children_;
	TreeOperator op_;
	int trigBit_;
      };

      boost::shared_ptr<TreeElement> masterElement_;

      //keep a copy of initialization string
      std::string expression_;

      boost::shared_ptr<edm::EventSelector> eventSelector_;
      bool useOld_;

      static const bool debug_ = false;

  };
}

#endif // EventFilter_StorageManager_TriggerSelector_h


/// emacs configuration
/// Local Variables: -
/// mode: c++ -
/// c-basic-offset: 2 -
/// indent-tabs-mode: nil -
/// End: -
