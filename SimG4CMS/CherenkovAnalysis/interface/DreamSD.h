#ifndef SimG4CMS_DreamSD_h
#define SimG4CMS_DreamSD_h

#include "SimG4CMS/Calo/interface/CaloSD.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "G4String.hh"
#include "G4PhysicsOrderedFreeVector.hh"

#include <map>

class G4LogicalVolume;

class DreamSD : public CaloSD {

public:    

  DreamSD(G4String, const DDCompactView &, SensitiveDetectorCatalog &, 
	  edm::ParameterSet const &, const SimTrackManager*);
  virtual ~DreamSD() {}
  virtual double                    getEnergyDeposit(G4Step*);
  virtual uint32_t                  setDetUnitId(G4Step*);

private:    

  void   initMap(G4String, const DDCompactView &);
  double curve_LY(G4Step*); 
  double crystalLength(G4LogicalVolume*);
  /// Returns the total energy due to Cherenkov radiation
  double cherenkovDeposit_( G4Step* aStep );
  /// Returns average number of photons created by track
  const double getAverageNumberOfPhotons_( const double charge,
                                           const double beta,
                                           const G4Material* aMaterial,
                                           const G4MaterialPropertyVector* rIndex ) const;
  /// Returns energy deposit for a given photon
  const double getPhotonEnergyDeposit_( const G4ParticleMomentum& p, const G4ThreeVector& x,
                                        const G4Step* aStep ) const;
  /// Sets material properties at run-time...
  bool setPbWO2MaterialProperties_( G4Material* aMaterial );

  bool                              useBirk,doCherenkov_;
  double                            birk1, birk2, birk3;
  double                            slopeLY;
  std::map<G4LogicalVolume*,double> xtalLMap;

  /// Table of Cherenkov angle integrals vs photon momentum
  std::auto_ptr<G4PhysicsOrderedFreeVector> chAngleIntegrals_;

};

#endif // DreamSD_h
