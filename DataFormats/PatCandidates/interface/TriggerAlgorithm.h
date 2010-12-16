#ifndef DataFormats_PatCandidates_TriggerAlgorithm_h
#define DataFormats_PatCandidates_TriggerAlgorithm_h


// -*- C++ -*-
//
// Package:    PatCandidates
// Class:      pat::TriggerAlgorithm
//
// $Id: TriggerAlgorithm.h,v 1.4 2010/12/15 19:44:26 vadler Exp $
//
/**
  \class    pat::TriggerAlgorithm TriggerAlgorithm.h "DataFormats/PatCandidates/interface/TriggerAlgorithm.h"
  \brief    Analysis-level L1 trigger algorithm class

   TriggerAlgorithm implements a container for L1 trigger algorithms' information within the 'pat' namespace.
   For detailed information, consult
   https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePATTrigger#TriggerAlgorithm

  \author   Volker Adler
  \version  $Id: TriggerAlgorithm.h,v 1.4 2010/12/15 19:44:26 vadler Exp $
*/


#include <string>
#include <vector>

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefProd.h"
#include "DataFormats/Common/interface/RefVector.h"
#include "DataFormats/Common/interface/RefVectorIterator.h"


namespace pat {

  class TriggerAlgorithm {

      /// Data Members

      /// L1 algorithm name
      std::string name_;
      /// L1 algorithm alias
      std::string alias_;
      /// Flag for technical L1 algorithms
      bool tech_;
      /// L1 algorithm bit number
      unsigned bit_;
      /// L1 algorithm pre-scale
      unsigned prescale_;
      /// L1 algorithm mask
      bool mask_;
      /// L1 algorithm decision, not considering the mask
      bool decisionBeforeMask_;
      /// L1 algorithm decision, considering the mask
      bool decisionAfterMask_;

    public:

      /// Constructors and Destructor

      /// Default constructor
      TriggerAlgorithm();
      /// Constructor from algorithm name only
      TriggerAlgorithm( const std::string & name );
      /// Constructor from values
      TriggerAlgorithm( const std::string & name, const std::string & alias, bool tech, unsigned bit, unsigned prescale, bool mask, bool decisionBeforeMask, bool decisionAfterMask );

      /// Destructor
      virtual ~TriggerAlgorithm() {};

      /// Methods

      /// Set L1 algorithm name
      void setName( const std::string & name ) { name_ = name; };
      /// Set L1 algorithm alias
      void setAlias( const std::string & alias ) { alias_ = alias; };
      /// Set flag for technical L1 algorithms
      void setTechTrigger( bool tech ) { tech_ = tech; };
      /// Set L1 algorithm bit number
      void setBit( unsigned bit ) { bit_ = bit; };
      /// Set L1 algorithm pre-scale
      void setPrescale( unsigned prescale )  { prescale_ = prescale; };
      /// Set L1 algorithm mask
      void setMask( bool mask ) { mask_ = mask; };
      /// Set L1 algorithm decision, not considering the mask
      void setDecisionBeforeMask( bool decisionBeforeMask ) { decisionBeforeMask_ = decisionBeforeMask; };
      /// Set L1 algorithm decision, considering the mask
      void setDecisionAfterMas( bool decisionAfterMask ) { decisionAfterMask_ = decisionAfterMask; };
      /// Get L1 algorithm name
      std::string name() const { return name_; };
      /// Get L1 algorithm alias
      std::string alias() const { return alias_; };
      /// Get flag for technical L1 algorithms
      bool techTrigger() const { return tech_; };
      /// Get L1 algorithm bit number
      unsigned bit() const { return bit_; };
      /// Get L1 algorithm pre-scale
      unsigned prescale() const { return prescale_; };
      /// Get L1 algorithm mask
      bool mask() const { return mask_; };
      /// Get L1 algorithm decision, not considering the mask
      bool decisionBeforeMask() const { return decisionBeforeMask_; };
      /// Get L1 algorithm decision, considering the mask
      bool decisionAfterMask() const { return decisionAfterMask_; };
      /// Get L1 algorithm decision as applied,
      /// identical to L1 algorithm decision, considering the mask
      bool decision() const { return decisionAfterMask(); };

  };


  /// Collection of TriggerAlgorithm
  typedef std::vector< TriggerAlgorithm >                      TriggerAlgorithmCollection;
  /// Persistent reference to an item in a TriggerAlgorithmCollection
  typedef edm::Ref< TriggerAlgorithmCollection >               TriggerAlgorithmRef;
  /// Persistent reference to a TriggerAlgorithmCollection product
  typedef edm::RefProd< TriggerAlgorithmCollection >           TriggerAlgorithmRefProd;
  /// Vector of persistent references to items in the same TriggerAlgorithmCollection
  typedef edm::RefVector< TriggerAlgorithmCollection >         TriggerAlgorithmRefVector;
  /// Const iterator over vector of persistent references to items in the same TriggerAlgorithmCollection
  typedef edm::RefVectorIterator< TriggerAlgorithmCollection > TriggerAlgorithmRefVectorIterator;

}


#endif
