// CaloJet.cc
// Fedor Ratnikov UMd
// $Id: CaloJet.cc,v 1.18 2008/04/30 22:05:16 fedor Exp $
#include <sstream>

#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/RecoCandidate/interface/RecoCaloTowerCandidate.h"

//Own header file
#include "DataFormats/JetReco/interface/CaloJet.h"

using namespace reco;

 CaloJet::CaloJet (const LorentzVector& fP4, const Point& fVertex, 
		   const Specific& fSpecific)
   : Jet (fP4, fVertex),
     m_specific (fSpecific)
 {}

 CaloJet::CaloJet (const LorentzVector& fP4, const Point& fVertex, 
 		  const Specific& fSpecific, 
 		  const Jet::Constituents& fConstituents)
   : Jet (fP4, fVertex, fConstituents),
     m_specific (fSpecific)
 {}

CaloJet::CaloJet (const LorentzVector& fP4, 
		  const Specific& fSpecific, 
		  const Jet::Constituents& fConstituents)
  : Jet (fP4, Point(0,0,0), fConstituents),
    m_specific (fSpecific)
{}


/// Physics Eta (use reference Z and jet kinematics only)
float CaloJet::physicsEtaQuick (float fZVertex) const {
  return Jet::physicsEta (fZVertex, eta());
}

/// Physics Eta (loop over constituents)
float CaloJet::physicsEtaDetailed (float fZVertex) const {
  Jet::LorentzVector correctedMomentum;
  std::vector<const Candidate*> towers = getJetConstituentsQuick ();
  for (unsigned i = 0; i < towers.size(); ++i) {
    const Candidate* c = towers[i];
    double etaRef = Jet::physicsEta (fZVertex, c->eta());
    math::PtEtaPhiMLorentzVectorD p4 (c->p()/cosh(etaRef), etaRef, c->phi(), c->mass());
    correctedMomentum += p4;
  }
  return correctedMomentum.eta();
}

  /// Physics p4 (use jet Z and kinematics only)
CaloJet::LorentzVector CaloJet::physicsP4 (float fZVertex) const {
  double physicsEta = Jet::physicsEta (fZVertex, eta());
  math::PtEtaPhiMLorentzVectorD p4 (p()/cosh(physicsEta), physicsEta, phi(), mass());
  return CaloJet::LorentzVector (p4);
}


CaloTowerRef CaloJet::getConstituent (unsigned fIndex) const {
  Constituent dau = daughterPtr (fIndex);
  return CaloTowerRef (dau.id(), dau.key(), dau.productGetter());
}


std::vector <CaloTowerRef> CaloJet::getConstituents () const {
  std::vector <CaloTowerRef> result;
  for (unsigned i = 0;  i <  numberOfDaughters (); i++) result.push_back (getConstituent (i));
  return result;
}


CaloJet* CaloJet::clone () const {
  return new CaloJet (*this);
}

bool CaloJet::overlap( const Candidate & ) const {
  return false;
}

std::string CaloJet::print () const {
  std::ostringstream out;
  out << Jet::print () // generic jet info
      << "    CaloJet specific:" << std::endl
      << "      energy fractions em/had: " << emEnergyFraction () << '/' << energyFractionHadronic () << std::endl
      << "      em energy in EB/EE/HF: " << emEnergyInEB() << '/' << emEnergyInEE() << '/' << emEnergyInHF() << std::endl
      << "      had energy in HB/HO/HE/HF: " << hadEnergyInHB() << '/' << hadEnergyInHO() << '/' << hadEnergyInHE() << '/' << hadEnergyInHF() << std::endl
      << "      constituent towers area: " << towersArea() << std::endl;
  out << "      Towers:" << std::endl;
  std::vector <CaloTowerRef> towers = getConstituents ();
  for (unsigned i = 0; i < towers.size (); i++) {
    if (towers[i].get ()) {
      out << "      #" << i << " " << *(towers[i]) << std::endl;
    }
    else {
      out << "      #" << i << " tower is not available in the event"  << std::endl;
    }
  }
  return out.str ();
}

//----------------------------------------------------------
// here are methods extracting information from constituents
//----------------------------------------------------------

std::vector<CaloTowerDetId> CaloJet::getTowerIndices() const {
  std::vector<CaloTowerDetId> result;
  std::vector <CaloTowerRef> towers = getConstituents ();
  for (unsigned i = 0; i < towers.size(); ++i) {
    result.push_back (towers[i]->id());
  }
  return result;
}
