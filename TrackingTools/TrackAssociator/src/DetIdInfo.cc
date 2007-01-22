#include "TrackingTools/TrackAssociator/interface/DetIdInfo.h"

#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/MuonDetId/interface/RPCDetId.h"
  
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"

#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h"

#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"

#include <sstream>

std::string DetIdInfo::info(const DetId& id) {
   std::ostringstream oss;
   
   oss << "DetId: " << id.rawId() << "\n";
   
   switch ( id.det() ) {
    
    case DetId::Muon:
      switch ( id.subdetId() ) {
       case MuonSubdetId::DT:
	   { 
	      DTChamberId detId(id.rawId());
	      oss << "DT chamber (wheel, station, sector): "
		<< detId.wheel() << ", "
		<< detId.station() << ", "
		<< detId.sector();
	   }
	 break;
       case MuonSubdetId::CSC:
	   {
	      CSCDetId detId(id.rawId());
	      oss << "CSC chamber (endcap, station, ring, chamber, layer): "
		<< detId.endcap() << ", "
		<< detId.station() << ", "
		<< detId.ring() << ", "
		<< detId.chamber() << ", "
		<< detId.layer();
	   }
	 break;
       case MuonSubdetId::RPC:
	   { 
	      RPCDetId detId(id.rawId());
	      oss << "RPC chamber ";
	      switch ( detId.region() ) {
	       case 0:
		 oss << "/ barrel / (wheel, station, sector, layer, subsector, roll): "
		   << detId.ring() << ", "
		   << detId.station() << ", "
		   << detId.sector() << ", "
		   << detId.layer() << ", "
		   << detId.subsector() << ", "
		   << detId.roll();
		 break;
	       case 1:
		 oss << "/ forward endcap / (wheel, station, sector, layer, subsector, roll): "
		   << detId.ring() << ", "
		   << detId.station() << ", "
		   << detId.sector() << ", "
		   << detId.layer() << ", "
		   << detId.subsector() << ", "
		   << detId.roll();
		 break;
	       case -1:
		 oss << "/ backward endcap / (wheel, station, sector, layer, subsector, roll): "
		   << detId.ring() << ", "
		   << detId.station() << ", "
		   << detId.sector() << ", "
		   << detId.layer() << ", "
		   << detId.subsector() << ", "
		   << detId.roll();
		 break;
	      }
	   }
	 break;
      }
      break;
    
    case DetId::Calo:
	{
	   CaloTowerDetId detId( id.rawId() );
	   oss << "CaloTower (ieta, iphi): "
	     << detId.ieta() << ", "
	     << detId.iphi();
	}
      break;
    
    case DetId::Ecal:
      switch ( id.subdetId() ) {
       case EcalBarrel:
	   {
	      EBDetId detId(id);
	      oss << "EcalBarrel (ieta, iphi, tower_ieta, tower_iphi): "
		<< detId.ieta() << ", "
		<< detId.iphi() << ", "
		<< detId.tower_ieta() << ", "
		<< detId.tower_iphi();
	   }
	 break;
       case EcalEndcap:
	   {
	      EEDetId detId(id);
	      oss << "EcalEndcap (ix, iy, SuperCrystal, crystal, quadrant): "
		<< detId.ix() << ", "
		<< detId.iy() << ", "
		<< detId.isc() << ", "
		<< detId.ic() << ", "
		<< detId.iquadrant();
	   }
	 break;
       case EcalPreshower:
	 oss << "EcalPreshower";
	 break;
       case EcalTriggerTower:
	 oss << "EcalTriggerTower";
	 break;
       case EcalLaserPnDiode:
	 oss << "EcalLaserPnDiode";
	 break;
      }
      break;
      
    case DetId::Hcal:
	{
	   HcalDetId detId(id);
	   switch ( detId.subdet() ) {
	    case HcalEmpty:
	      oss << "HcalEmpty ";
	      break;
	    case HcalBarrel:
	      oss << "HcalBarrel ";
	      break;
	    case HcalEndcap:
	      oss << "HcalEndcap ";
	      break;
	    case HcalOuter:
	      oss << "HcalOuter ";
	      break;
	    case HcalForward:
	      oss << "HcalForward ";
	      break;
	    case HcalTriggerTower:
	      oss << "HcalTriggerTower ";
	      break;
	    case HcalOther:
	      oss << "HcalOther ";
	      break;
	   }
	   oss << "(ieta, iphi, depth):"
	     << detId.ieta() << ", "
	     << detId.iphi() << ", "
	     << detId.depth();
	}
      break;
    default :;
   }
   return oss.str();
}

   
std::string DetIdInfo::info(const std::set<DetId>& idSet) {
   std::string text;
   for(std::set<DetId>::const_iterator id = idSet.begin(); id != idSet.end(); id++)
     {
	text += info(*id);
	text += "\n";
     }
   return text;
}

   
