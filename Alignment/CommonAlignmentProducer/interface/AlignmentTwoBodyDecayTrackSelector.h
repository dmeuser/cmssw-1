
#ifndef Alignment_CommonAlignmentAlgorithm_AlignmentTwoBodyDecayTrackSelector_h
#define Alignment_CommonAlignmentAlgorithm_AlignmentTwoBodyDecayTrackSelector_h

//Framework
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
//STL
#include <vector>
// forward declaration:
#include <DataFormats/TrackReco/interface/TrackFwd.h>
#include <DataFormats/METReco/interface/CaloMETFwd.h>

class AlignmentTwoBodyDecayTrackSelector
{
 public:

  typedef std::vector<const reco::Track*> Tracks; 

  /// constructor
  AlignmentTwoBodyDecayTrackSelector(const edm::ParameterSet & cfg);

  /// destructor
  ~AlignmentTwoBodyDecayTrackSelector();

  /// select tracks
  Tracks select(const Tracks& tracks, const edm::Event& iEvent);

  bool useThisFilter();
 private:
  ///checks if the mass of the mother is in the mass region
  Tracks checkMass(const Tracks& cands)const;
  ///checks if the mass of the mother is in the mass region adding missing E_T
  Tracks checkMETMass(const Tracks& cands,const edm::Event& iEvent)const;
  ///checks if the mother has charge = [theCharge]
  bool checkCharge(const reco::Track* trk1,const reco::Track* trk2 = 0)const;
  ///checks if the [cands] are acoplanar (returns empty set if not)
  bool checkAcoplanarity(const reco::Track* trk1,const reco::Track* trk2)const;
  ///checks if [cands] contains a acoplanar track w.r.t missing ET (returns empty set if not)
  bool checkMETAcoplanarity(const reco::Track* trk,const reco::CaloMET* met)const; 

  /// private data members

  //settings from conigfile
  bool theMassrangeSwitch;
  bool theChargeSwitch;
  bool theMissingETSwitch;
  bool theAcoplanarityFilterSwitch;
  //inv mass Cut
  double theMinMass;
  double theMaxMass;
  double theMass;
  double theDaughterMass;
  unsigned int theCandNumber;
  //charge filter
  int theCharge;
  bool theUnsignedSwitch;
  //missing ET Filter
  edm::InputTag theMissingETSource;
  //acoplanarity Filter
  double theAcoplanarDistance;
  //helpers
  ///print Information on Track-Collection
  void printTracks(const Tracks& col) const;
};

#endif

