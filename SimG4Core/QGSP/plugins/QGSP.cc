#include "QGSP.hh"
#include "SimG4Core/QGSP/src/HadronPhysicsQGSP.hh"

#ifdef G4V7
#include "SimG4Core/Packaging/src/GeneralPhysics.hh"
#include "SimG4Core/Packaging/src/EMPhysics.hh"
#include "SimG4Core/Packaging/src/MuonPhysics.hh"
#include "SimG4Core/Packaging/src/IonPhysics.hh"
#include "SimG4Core/Packaging/src/G4DataQuestionaire.hh"
#else
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4QStoppingPhysics.hh"
#include "G4HadronElasticPhysics.hh" 
#include "G4DataQuestionaire.hh"
#endif

QGSP::QGSP(G4LogicalVolumeToDDLogicalPartMap& map, 
	   const edm::ParameterSet & p) : PhysicsList(map, p) {
    G4DataQuestionaire it(photon);
#ifdef G4V7
    std::cout << "You are using the simulation engine: QGSP 2.8" << std::endl;
  
    RegisterPhysics(new GeneralPhysics("general"));
    RegisterPhysics(new EMPhysics("EM"));
    RegisterPhysics(new MuonPhysics("muon"));
    RegisterPhysics(new HadronPhysicsQGSP("hadron"));
    RegisterPhysics(new IonPhysics("ion"));
#else
    std::cout << "You are using the simulation engine: QGSP 3.1" << std::endl;
  
    // EM Physics
    RegisterPhysics(new G4EmStandardPhysics("standard EM"));
    // Synchroton Radiation & GN Physics
    RegisterPhysics(new G4EmExtraPhysics("extra EM"));
    // Decays
    RegisterPhysics(new G4DecayPhysics("decay"));
    // Hadron Elastic scattering
    RegisterPhysics(new G4HadronElasticPhysics("elastic")); 
    // Hadron Physics
    RegisterPhysics(new HadronPhysicsQGSP("hadron"));
    // Stopping Physics
    RegisterPhysics(new G4QStoppingPhysics("stopping"));
    // Ion Physics
    RegisterPhysics(new G4IonPhysics("ion"));
#endif
}

