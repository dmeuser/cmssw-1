#ifndef CALORIMETRYMANAGER_H
#define CALORIMETRYMANAGER_H

#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"

// FastSimulation headers
#include "FastSimulation/Particle/interface/RawParticle.h"
#include "FastSimulation/Calorimetry/interface/HCALResponse.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "FastSimulation/Utilities/interface/FamosDebug.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"

// For the uint32_t
//#include <boost/cstdint.hpp>
#include <map>

class FSimEvent;
class FSimTrack;
class RawParticle;
class CaloGeometryHelper;
class Histos;
class HSParameters;
class RandomEngine;
class LandauFluctuationGenerator;
class GammaFunctionGenerator;
class MaterialEffects;
//Gflash
class GflashHadronShowerProfile;
class GflashPiKShowerProfile;
class GflashProtonShowerProfile;
class GflashAntiProtonShowerProfile;

namespace edm { 
  class ParameterSet;
}

class CalorimetryManager{

 public:
  CalorimetryManager();
  CalorimetryManager(FSimEvent* aSimEvent, 
		     const edm::ParameterSet& fastCalo,
		     const edm::ParameterSet& MuonECALPars,
		     const edm::ParameterSet& MuonHCALPars,
                     const edm::ParameterSet& fastGflash,
		     const RandomEngine* engine);
  ~CalorimetryManager();

  // Does the real job
  void reconstruct();

    // Return the address of the Calorimeter 
  CaloGeometryHelper * getCalorimeter() const {return myCalorimeter_;}

  // load container from edm::Event
  void loadFromEcalBarrel(edm::PCaloHitContainer & c) const;

  void loadFromEcalEndcap(edm::PCaloHitContainer & c) const;

  void loadFromHcal(edm::PCaloHitContainer & c) const;

  void loadFromPreshower(edm::PCaloHitContainer & c) const;

  void loadMuonSimTracks(edm::SimTrackContainer & m) const;

 private:
  // Simulation of electromagnetic showers in PS, ECAL, HCAL
  void EMShowerSimulation(const FSimTrack& myTrack);
  
  // Simulation of electromagnetic showers in VFCAL
  void reconstructECAL(const FSimTrack& track) ;

  void reconstructHCAL(const FSimTrack& myTrack);

  void MuonMipSimulation(const FSimTrack & myTrack);
 
  /// Hadronic Shower Simulation
  void HDShowerSimulation(const FSimTrack& myTrack);

  // Read the parameters 
  void readParameters(const edm::ParameterSet& fastCalo);

  void updateMap(uint32_t cellid,float energy,int id,std::map<uint32_t,std::vector<std::pair<int,float> > >& mymap);

  void updateMap(int hi,float energy,int id,std::vector<std::vector<std::pair<int,float> > > & mymap,std::vector<int> & firedCells);

  void respCorr(double);

  void clean(); 

 private:

  FSimEvent* mySimEvent;
  CaloGeometryHelper* myCalorimeter_;

  Histos * myHistos;

  HCALResponse* myHDResponse_;
  HSParameters * myHSParameters_;

  // In the not unfolded case (standard) the most inner vector will be of size = 1 
  // the preshower does not have hashed_indices, hence the map 
  std::vector<std::vector<std::pair<int,float> > > EBMapping_;
  std::vector<std::vector<std::pair<int,float> > > EEMapping_;
  std::vector<std::vector<std::pair<int,float> > > HMapping_;
  std::map<uint32_t,std::vector<std::pair<int,float> > > ESMapping_;

  std::vector<int> firedCellsEB_;
  std::vector<int> firedCellsEE_;
  std::vector<int> firedCellsHCAL_;

  // this is bad, the same information already exists in CaloRecHitsProducers
  // should make a es_producer of CaloGeometryTools 
  std::vector<DetId> theDetIds_;
  bool debug_;

  bool unfoldedMode_;

  /// A few pointers to save time
  RawParticle myElec;
  RawParticle myPosi;
  RawParticle myPart;

  // Parameters 
  double pulledPadSurvivalProbability_;
  double crackPadSurvivalProbability_;
  double spotFraction_;
  double radiusFactor_;
  double radiusFactorBehindPreshower_;
  int gridSize_;
  std::vector<double> theCoreIntervals_,theTailIntervals_;
  double RCFactor_,RTFactor_;
  //FR
  int optionHDSim_, hdGridSize_, hdSimMethod_;
  bool simulatePreshower_;
  //RF 

  // Famos Random Engine
  const RandomEngine* random;
  const LandauFluctuationGenerator* aLandauGenerator;
  GammaFunctionGenerator* aGammaGenerator;

  static std::vector<std::pair<int, float> > myZero_;
  bool initialized_;

  // RespCorrP p, k_e(p), k_h(p) vectors  and evaluated for each p
  // ecorr and hcorr  
  std::vector<double> rsp;
  std::vector<double> p_knots;
  std::vector<double> k_e;
  std::vector<double> k_h;
  double ecorr;
  double hcorr;

  std::vector<FSimTrack> muonSimTracks;
  MaterialEffects* theMuonEcalEffects; // material effects for muons in ECAL
  MaterialEffects* theMuonHcalEffects; // material effects for muons in HCAL

  //Gflash
  GflashHadronShowerProfile *theProfile;
  GflashPiKShowerProfile *thePiKProfile;
  GflashProtonShowerProfile *theProtonProfile;
  GflashAntiProtonShowerProfile *theAntiProtonProfile;
};
#endif
