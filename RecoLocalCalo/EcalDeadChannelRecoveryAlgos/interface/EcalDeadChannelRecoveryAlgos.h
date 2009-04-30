#ifndef RecoLocalCalo_EcalDeadChannelRecoveryAlgos_EcalDeadChannelRecoveryAlgos_HH
#define RecoLocalCalo_EcalDeadChannelRecoveryAlgos_EcalDeadChannelRecoveryAlgos_HH
 
/** \class EcalDeadChannelRecoveryAlgos
  *
  *  $Date: 2009/04/09 13:36:27 $
  *  $Revision: 1.4 $
  */
 

// Geometry
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloTopology/interface/EcalEndcapTopology.h"
#include "Geometry/CaloTopology/interface/EcalBarrelTopology.h"
#include "Geometry/CaloTopology/interface/EcalBarrelHardcodedTopology.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloTopology/interface/CaloSubdetectorTopology.h"
#include "RecoEcal/EgammaCoreTools/interface/PositionCalc.h"
//#include "Geometry/Vector/interface/GlobalPoint.h"

// Reconstruction Classes
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"

#include <string>
using namespace cms;
using namespace std;




class EcalDeadChannelRecoveryAlgos  {
   public:
  EcalDeadChannelRecoveryAlgos(const CaloTopology theCaloTopology);
  EcalDeadChannelRecoveryAlgos();
  ~EcalDeadChannelRecoveryAlgos();
 
  EcalRecHit Correct(const EBDetId Id, const EcalRecHitCollection* hit_collection, string algo_, double Sum8Cut);
  EcalRecHit correct(const EBDetId Id, const EcalRecHitCollection* hit_collection, string algo_, double Sum8Cut);
  
 private:
  
  
  CaloTopology calotopo;
  double MakeNxNMatrice(EBDetId itID,const EcalRecHitCollection* hit_collection, int *IndDeadChannel, double *MNxN);
  

  
  // ----------member data ---------------------------
  
};
#endif
