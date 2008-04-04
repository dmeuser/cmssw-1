//
// $Id: Muon.h,v 1.8 2008/04/03 12:29:08 gpetrucc Exp $
//

#ifndef DataFormats_PatCandidates_Muon_h
#define DataFormats_PatCandidates_Muon_h

/**
  \class    pat::Muon Muon.h "DataFormats/PatCandidates/interface/Muon.h"
  \brief    Analysis-level muon class

   Muon implements the analysis-level muon class within the 'pat' namespace.

  \author   Steven Lowette
  \version  $Id: Muon.h,v 1.8 2008/04/03 12:29:08 gpetrucc Exp $
*/

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/PatCandidates/interface/Lepton.h"


namespace pat {


  typedef reco::Muon MuonType;
  typedef reco::MuonCollection MuonTypeCollection;


  class Muon : public Lepton<MuonType> {

    public:

      Muon();
      Muon(const MuonType & aMuon);
      Muon(const edm::RefToBase<MuonType> & aMuonRef);
      virtual ~Muon();

      virtual Muon * clone() const { return new Muon(*this); }
      float leptonID() const;

      void setLeptonID(float id);

    protected:

      float leptonID_;

  };


}

#endif
