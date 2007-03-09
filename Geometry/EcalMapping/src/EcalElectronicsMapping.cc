#include "Geometry/EcalMapping/interface/EcalElectronicsMapping.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h"
#include "DataFormats/DetId/interface/DetId.h"

#include "DataFormats/EcalDetId/interface/EcalElectronicsId.h"
#include "DataFormats/EcalDetId/interface/EcalTriggerElectronicsId.h"

#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "FWCore/Utilities/interface/Exception.h"



EcalElectronicsMapping::EcalElectronicsMapping() {
}


int EcalElectronicsMapping::DCCid(const EBDetId& id) const 

	// SM id, between 1 (phi = -10 deg) and 18 in EB+
	// between 19 (phi = -10 deg) and 27 in EB-.
	// returns DCCid, currently between 10 and 27 (EB-), 28 and 45 (EB+).
	// For the EE case, use getElectronicsId.
{
        int dcc = id.ism();      
        if (id.zside() < 0) {
                dcc  += DCCID_PHI0_EBM - 19;
        }
        else {
                dcc  += DCCID_PHI0_EBP -1;
        }
	return dcc;
}


int EcalElectronicsMapping::TCCid(const EBDetId& id) const
                                                                                                                     
        // SM id, between 1 (phi = -10 deg) and 18 in EB+
        // between 19 (phi = -10 deg) and 27 in EB-.
	// returns TCCid, currently between 37 and 54 (EB-), 55 and 72 (EB+).
	// For the EE case, use getEcalTriggerElectronicsId.
{
        int tcc = id.ism();
        if (id.zside() < 0) {
                tcc  += TCCID_PHI0_EBM - 19;
        }
        else {
                tcc  += TCCID_PHI0_EBP -1;
        }
        return tcc;
}



int EcalElectronicsMapping::iTT(const EcalTrigTowerDetId& id) const

	// returns the index of a Trigger Tower within its TCC.
	// This is between 1 and 68 for the Barrel, and between
   	// 1 and 32 to 34 (t.b.c.) for the EndCap.

{
  if ( id.subDet() == EcalBarrel )
    {
      int ie = id.ietaAbs() -1;
      int ip;
      int phi = id.iphi();
      phi += 2;
      if (phi > 72) phi = phi-72;
      if (id.zside() < 0) {
        ip = (( phi -1 ) % kEBTowersInPhi ) + 1;
      } else {
        ip = kEBTowersInPhi - ((phi -1 ) % kEBTowersInPhi );
      }
                                                                                                                     
      return (ie * kEBTowersInPhi) + ip;
    }
  else if ( id.subDet() == EcalEndcap)   {
        int ie = id.ietaAbs();
        bool inner = (ie >= iEEEtaMinInner);
        if (inner) {
                ie = ie - iEEEtaMinInner;
                ie = ie % kEETowersInEtaPerInnerTCC;
        }
        else {
                ie = ie - iEEEtaMinOuter;
                ie = ie % kEETowersInEtaPerOuterTCC;
        }
                                                                                                                     
        int ip = id.iphi();
        ip = (ip + 1) % (kEETowersInPhiPerQuadrant*4); 
				// now iphi between 0 and 71,
                                // with iphi=0,1,2,3 in 1st Phi sector
        ip = ip % kEETowersInPhiPerTCC;
        int itt = kEETowersInPhiPerTCC * ie + ip + 1;
        return itt;
  }
  else {
	throw cms::Exception("InvalidDetId") << " Wrong EcalTrigTowerDetId in EcalElectronicsMapping::iTT. " ;
	return 0;
  }

}


int EcalElectronicsMapping::TCCid(const EcalTrigTowerDetId& id) const  {

  if ( id.subDet() == EcalBarrel )
    {
	int phi = id.iphi() +2;
      	if (phi > 72) phi = phi-72;
      	int tcc = ( phi - 1 ) / kEBTowersInPhi + 1;
      	if ( id.zside() < 0 ) tcc += 18;      // now id is the SMid
        	if (id.zside() < 0) {
                	tcc  += TCCID_PHI0_EBM - 19;
        	}
        	else {
                	tcc  += TCCID_PHI0_EBP -1;
        	}
	return tcc;
    }

  else if ( id.subDet() == EcalEndcap )
    {
        int ie = id.ietaAbs();
        bool inner = (ie >= iEEEtaMinInner);
        int ip = id.iphi();    // iphi = 1 corresponds to 0 < phi < 5 degrees
        ip = (ip + 1) % (kEETowersInPhiPerQuadrant*4);  
				// now iphi between 0 and 71,
                                // with iphi=0,1,2,3 in 1st Phi sector
        int Phiindex = ip / 4;
        if (inner) {
                if (id.ieta()> 0) Phiindex += TCCID_PHI0_EEP_IN;
                else Phiindex += TCCID_PHI0_EEM_IN;
        }
        else {
                if (id.ieta() > 0) Phiindex += TCCID_PHI0_EEP_OUT;
                else Phiindex += TCCID_PHI0_EEM_OUT;
        }
        return Phiindex;
    }
  else {
	throw cms::Exception("InvalidDetId") << " Wrong EcalTrigTowerDetId in EcalElectronicsMapping::TCCid.";
	return 0;
  }
}



int EcalElectronicsMapping::DCCid(const EcalTrigTowerDetId& id) const  {

	// This is needed for digitoraw. For a given Trigger Tower,
	// one needs to know to which FED it gets written.

  if ( id.subDet() == EcalBarrel )
    {
        int phi = id.iphi() +2;
        if (phi > 72) phi = phi-72;
        int dcc = ( phi - 1 ) / kEBTowersInPhi + 1;
        if ( id.zside() < 0 ) dcc += 18;      // now id is the SMid
                if (id.zside() < 0) {
                        dcc  += DCCID_PHI0_EBM - 19;
                }
                else {
                        dcc  += DCCID_PHI0_EBP -1;
                }
        return dcc;
    }
  else if ( id.subDet() == EcalEndcap) 
    {			//FIXME :  yes I need to improve this part of the code...
        int tccid = TCCid(id);
        int dcc=0;
        int offset = 0;
        if (tccid >= 73) {
                tccid = tccid-72;
                offset = 45;
        }
        if (tccid == 24 || tccid == 25 || tccid == 6 || tccid == 7)  dcc=4;
        if (tccid == 26 || tccid == 27 || tccid == 8 || tccid == 9)  dcc=5;
        if (tccid == 28 || tccid == 29 || tccid == 10 || tccid == 11)  dcc=6;
        if (tccid == 30 || tccid == 31 || tccid == 12 || tccid == 13)  dcc=7;
        if (tccid == 32 || tccid == 33 || tccid == 14 || tccid == 15)  dcc=8;
        if (tccid == 34 || tccid == 35 || tccid == 16 || tccid == 17)  dcc=9;
        if (tccid == 36 || tccid == 19 || tccid == 18 || tccid == 1)  dcc=1;
        if (tccid == 20 || tccid == 21 || tccid == 2 || tccid == 3)  dcc=2;
        if (tccid == 22 || tccid == 23 || tccid == 4 || tccid == 5)  dcc=3;
        dcc += offset;
        return dcc;
    }
  else {
        throw cms::Exception("InvalidDetId") << " Wrong EcalTrigTowerDetId in EcalElectronicsMapping::DCCid.";
        return 0;
  }

}

EcalTrigTowerDetId EcalElectronicsMapping::getTrigTowerDetId(int TCCid, int iTT) const {
	
	// needed for unpacking code.

   EcalSubdetector sub = subdet(TCCid, TCCMODE);
   int zIndex = zside(TCCid, TCCMODE);

   if (sub == EcalBarrel) {

	int DCCid =0;
	int jtower = iTT-1;
	if (zIndex > 0) DCCid = TCCid - TCCID_PHI0_EBP + DCCID_PHI0_EBP;
	else DCCid = TCCid - TCCID_PHI0_EBM + DCCID_PHI0_EBM;
        int SMid = (zIndex > 0) ? DCCid - 27 : DCCid + 9;

        int etaTT = jtower / kTowersInPhi +1;   // between 1 and 17
        int phiTT;
                                                                                                                     
        if (zIndex > 0)
           phiTT=(SMid - 1) * kTowersInPhi + (kTowersInPhi -(jtower % kTowersInPhi)) -1;
        else
           phiTT=(SMid - 19)* kTowersInPhi + jtower % kTowersInPhi;
        phiTT ++;
        phiTT = phiTT -2;
        if (phiTT <= 0) phiTT = 72+phiTT;
        EcalTrigTowerDetId tdetid(zIndex, EcalBarrel, etaTT, phiTT,EcalTrigTowerDetId::SUBDETIJMODE);
	return tdetid;
   }

   else if (sub == EcalEndcap) {

	bool EEminus = (zIndex <0);
 	bool EEplus  = (zIndex >0);	
        if ( (!EEminus) && (!EEplus) )
                throw cms::Exception("InvalidDetId") <<
                "EcalElectronicsMapping:  Cannot create EcalTrigTowerDetId object. " ;
        int iz = 0;
	int tcc = TCCid;
        if (tcc <= TCCID_PHI0_EEM_OUT+kTCCinPhi) iz = -1;
        else if (tcc >= TCCID_PHI0_EEP_OUT) iz = +1;

        bool inner = false;
        if (iz < 0 && tcc >= TCCID_PHI0_EEM_IN && tcc <= TCCID_PHI0_EEM_IN+kTCCinPhi) inner=true;
        if (iz > 0 && tcc >= TCCID_PHI0_EEP_IN && tcc <= TCCID_PHI0_EEP_IN+kTCCinPhi) inner=true;
        bool outer = !inner;

        int ieta = iTT / kEETowersInPhiPerTCC;
        int iphi = (iTT-1) % kEETowersInPhiPerTCC;
        if (inner) ieta += iEEEtaMinInner;
        else ieta += iEEEtaMinOuter;
        if (iz < 0) ieta = -ieta;

        int TCC_origin = 0;
        if (inner && iz < 0) TCC_origin=TCCID_PHI0_EEM_IN;
        if (outer && iz < 0) TCC_origin=TCCID_PHI0_EEM_OUT;
        if (inner && iz > 0) TCC_origin=TCCID_PHI0_EEP_IN;
        if (outer && iz > 0) TCC_origin=TCCID_PHI0_EEP_OUT;
        tcc = tcc - TCC_origin;

        iphi += kEETowersInPhiPerTCC * tcc;
        iphi = (iphi -2 + 4*kEETowersInPhiPerQuadrant) % (4*kEETowersInPhiPerQuadrant) + 1;

        int tower_i = abs(ieta);
        int tower_j = iphi;
	EcalTrigTowerDetId tdetid(zIndex, EcalEndcap, tower_i, tower_j,EcalTrigTowerDetId::SUBDETIJMODE);
	return tdetid;

   }
  else {
        throw cms::Exception("InvalidDetId") << 
		" Wrong indices in EcalElectronicsMapping::getTrigTowerDetId. TCCid = " << TCCid << " iTT = " << iTT << ".";
  }


}





EcalElectronicsId EcalElectronicsMapping::getElectronicsId(const DetId& id) const {

  EcalSubdetector subdet = EcalSubdetector(id.subdetId());
  if (subdet == EcalBarrel) {
	const EBDetId ebdetid = EBDetId(id);

	int dcc = DCCid(ebdetid);

	EcalTrigTowerDetId trigtower = ebdetid.tower();
	// int tower = trigtower.iTT();
	int tower = iTT(trigtower);

	int ieta = EBDetId(id).ietaAbs();
	int iphi = EBDetId(id).iphi();
	int strip;
	int channel;
	bool RightTower = rightTower(tower);
 	if (RightTower) {
		strip = (ieta-1)%5;
		if (strip%2 == 0) channel = (iphi-1) %5;
		else channel = 4 -( (iphi-1) %5 ); 
	}
	else {
		strip = 4 - ( (ieta-1)%5 );
		if (strip%2 == 0) channel = 4 -( (iphi-1) %5 );
		else channel = (iphi-1) %5;
	}
	strip += 1;
	channel += 1;
	
	EcalElectronicsId elid = EcalElectronicsId(dcc,tower,strip,channel);
	
  	return elid;
  }
  else if (subdet == EcalEndcap) {
	EcalElectronicsMap_by_DetId::const_iterator it=get<0>(m_items).find(id);
	EcalElectronicsId elid = it -> elid;
	return elid;
  }
  else {
        throw cms::Exception("InvalidDetId") <<
                " Wrong DetId in EcalElectronicsMapping::getElectronicsId.";
  }

}


EcalTriggerElectronicsId EcalElectronicsMapping::getTriggerElectronicsId(const DetId& id) const {
  EcalSubdetector subdet = EcalSubdetector(id.subdetId());

  if (subdet == EcalBarrel) {

	const EcalElectronicsId& elid = getElectronicsId(id);
	EcalTriggerElectronicsId trelid = getTriggerElectronicsId(elid);
	return trelid;
  }
  else if (subdet == EcalEndcap) {
	EcalElectronicsMap_by_DetId::const_iterator it=get<0>(m_items).find(id);
	EcalTriggerElectronicsId trelid = it -> trelid;
	return trelid;
  }
  else {
        throw cms::Exception("InvalidDetId") <<
                " Wrong DetId in EcalElectronicsMapping::getTriggerElectronicsId.";
  }
}


DetId EcalElectronicsMapping::getDetId(const EcalElectronicsId& id) const {
  EcalSubdetector subdet = id.subdet();

  if (subdet == EcalBarrel) {
        int dcc = id.dccId();
        int tower = id.towerId();
        int strip = id.stripId();
        int channel = id.xtalId();
                                                                                                           
        int smid = 0;
        int iphi = 0;
        if (id.zside() < 0) {
                smid = dcc + 19 - DCCID_PHI0_EBM;
                iphi = (smid - 19) * kCrystalsInPhi;
		iphi += 5 * ( (tower-1) % kTowersInPhi );
        }
        else {
                smid = dcc +1 - DCCID_PHI0_EBP;
                iphi = (smid - 1) * kCrystalsInPhi;
		iphi += 5 * (
			kTowersInPhi - ( (tower-1) % kTowersInPhi ) -1
			);
        }
        bool RightTower = rightTower(tower);
	int ieta = 5 * ((tower-1) / kTowersInPhi) + 1;
        if (RightTower) {
		ieta += (strip-1);
                if (strip%2 == 1) iphi += (channel-1) +1;
                else iphi += (4 - (channel-1)) +1;
        }
        else {
		ieta += 4 - (strip-1);
                if (strip%2 == 1) iphi += (4 - (channel-1)) +1;
                else iphi += (channel-1) +1;
        }
        if (id.zside() < 0) ieta = -ieta;

        EBDetId e(ieta,iphi,EBDetId::ETAPHIMODE);
        return e;
  }

  else if (subdet == EcalEndcap) {
	EcalElectronicsMap_by_ElectronicsId::const_iterator it=get<1>(m_items).find(id);
	DetId cell = it -> cell;
	return cell;
  }
  else throw cms::Exception("InvalidDetId") << "Wrong EcalElectronicsId in EcalElectronicsMapping::getDetId." ;
}


EcalTriggerElectronicsId EcalElectronicsMapping::getTriggerElectronicsId(const EcalElectronicsId& id) const {

  EcalSubdetector subdet = id.subdet();

  if (subdet == EcalBarrel) {
	int strip = id.stripId();
	int xtal = id.xtalId();
	int tower = id.towerId();
	int tcc = id.dccId();
        if (id.zside() < 0) {
                tcc  += TCCID_PHI0_EBM - DCCID_PHI0_EBM;
        }
        else {
                tcc  += TCCID_PHI0_EBP - DCCID_PHI0_EBP;
        }
	EcalTriggerElectronicsId trelid(tcc,tower,strip,xtal);
        return trelid;

  }
  else if (subdet == EcalEndcap) {
	EcalElectronicsMap_by_ElectronicsId::const_iterator it=get<1>(m_items).find(id);
	EcalTriggerElectronicsId trelid = it -> trelid;
	return trelid;
  }
  else throw cms::Exception("InvalidDetId") << "Wrong EcalElectronicsId in EcalElectronicsMapping::getTriggerElectronicsId.";
}


DetId EcalElectronicsMapping::getDetId(const EcalTriggerElectronicsId& id) const {

  EcalSubdetector subdet = id.subdet();

  if (subdet == EcalBarrel) {
	const EcalElectronicsId& elid = getElectronicsId(id); 
	DetId cell = getDetId(elid);	
        return cell;
  }
  else if (subdet == EcalEndcap) {
	EcalElectronicsMap_by_TriggerElectronicsId::const_iterator it=get<2>(m_items).find(id);
	DetId cell = it -> cell;
	return cell;
  }
  else throw cms::Exception("InvalidDetId") << "Wrong EcalTriggerElectronicsId in EcalElectronicsMapping::getDetId." ;
}



EcalElectronicsId EcalElectronicsMapping::getElectronicsId(const EcalTriggerElectronicsId& id) const {

  EcalSubdetector subdet = id.subdet();

  if (subdet == EcalBarrel) {
        int strip = id.pseudoStripId();
        int xtal = id.channelId();
        int tower = id.ttId();
        int dcc = id.tccId();
        if (id.zside() < 0) {
                dcc  -= TCCID_PHI0_EBM - DCCID_PHI0_EBM;
        }
        else {
                dcc  -= TCCID_PHI0_EBP - DCCID_PHI0_EBP;
        }
        EcalElectronicsId elid(dcc,tower,strip,xtal);
        return elid;
  }
  else if (subdet == EcalEndcap) {
	EcalElectronicsMap_by_TriggerElectronicsId::const_iterator it=get<2>(m_items).find(id);
	EcalElectronicsId elid = it -> elid;
	return elid;
  }
  else throw cms::Exception("InvalidDetId") << "Wrong EcalTriggerElectronicsId in EcalElectronicsMapping::getElectronicsId.";
}


std::vector<DetId> EcalElectronicsMapping::dccConstituents(int dccId) const {
  EcalSubdetector sub = subdet(dccId,DCCMODE);
  std::vector<DetId> items;
  if (sub == EcalBarrel) { 
  	return items; 
  }
  else if (sub == EcalEndcap) {
	EcalElectronicsMap_by_DccId::const_iterator lb,ub;
	boost::tuples::tie(lb,ub)=get<3>(m_items).equal_range(dccId);
	while (lb != ub) {
		DetId cell = lb -> cell;
		items.push_back(cell);
		++ lb;
	}
	return items;
  }
  else throw cms::Exception("InvalidDetId") << "Wrong dccId = " << dccId << " in EcalElectronicsMapping::dccConstituents. ";
}

std::vector<DetId> EcalElectronicsMapping::dccTowerConstituents(int dccId, int tower) const {
  EcalSubdetector sub = subdet(dccId,DCCMODE);
  std::vector<DetId> items;
  if (sub == EcalBarrel) {
  	return items;
  }
  else if (sub == EcalEndcap) {
	EcalElectronicsMap_by_DccId_and_TowerId::const_iterator lb,ub;
	boost::tuples::tie(lb,ub)=get<4>(m_items).equal_range(boost::make_tuple(int(dccId), int(tower)));
	while (lb != ub) {
		DetId cell = lb -> cell;
		items.push_back(cell);
		++ lb;
	}
	return items;
  }
  else throw cms::Exception("InvalidDetId") << 
	"Wrong dccId = " << dccId << " tower = " << tower << " in EcalElectronicsMapping::dccTowerConstituents.";
}

std::vector<DetId> EcalElectronicsMapping::stripConstituents(int dccId, int tower, int strip) const {
  EcalSubdetector sub = subdet(dccId,DCCMODE);
  std::vector<DetId> items;
  if (sub == EcalBarrel) {
        return items;
  }
  else {
	EcalElectronicsMap_by_DccId_TowerId_and_StripId::const_iterator lb,ub;
	boost::tuples::tie(lb,ub)=get<5>(m_items).equal_range(boost::make_tuple(int(dccId), int(tower), int(strip)));
	while (lb != ub) {
		DetId cell = lb -> cell;
		items.push_back(cell);
		++ lb;
	}
	return items;
  }
  
}

std::vector<DetId> EcalElectronicsMapping::tccConstituents(int tccId) const {
  EcalSubdetector sub = subdet(tccId,TCCMODE);
  std::vector<DetId> items;
  if (sub == EcalBarrel) {
        return items;
  }
  else {
        EcalElectronicsMap_by_TccId::const_iterator lb,ub;
        boost::tuples::tie(lb,ub)=get<6>(m_items).equal_range(tccId);
        while (lb != ub) {
                DetId cell = lb -> cell;
                items.push_back(cell);
		++ lb;
        }
        return items;
  }
}

std::vector<DetId> EcalElectronicsMapping::ttConstituents(int tccId, int tt) const {
  EcalSubdetector sub = subdet(tccId,TCCMODE);
  std::vector<DetId> items;
  if (sub == EcalBarrel) {
        return items;
  }
  else {
        EcalElectronicsMap_by_TccId_and_TtId::const_iterator lb,ub;
        boost::tuples::tie(lb,ub)=get<7>(m_items).equal_range(boost::make_tuple(int(tccId), int(tt)));
        while (lb != ub) {
                DetId cell = lb -> cell;
                items.push_back(cell);
		++ lb;
        }
        return items;
  }
}

std::vector<DetId> EcalElectronicsMapping::pseudoStripConstituents(int tccId, int tt, int pseudostrip) const {
  EcalSubdetector sub = subdet(tccId,TCCMODE);
  std::vector<DetId> items;
  if (sub == EcalBarrel) {
        return items;
  }
  else {
        EcalElectronicsMap_by_TccId_TtId_and_PseudostripId::const_iterator lb,ub;
        boost::tuples::tie(lb,ub)=get<8>(m_items).equal_range(boost::make_tuple(int(tccId), int(tt), int(pseudostrip)));
        while (lb != ub) {
                DetId cell = lb -> cell;
                items.push_back(cell);
		++ lb;
        }
        return items;
  }
}

void EcalElectronicsMapping::assign(const DetId& cell, const EcalElectronicsId& elid, const EcalTriggerElectronicsId& tower) {

	m_items.insert(MapItem(cell, elid, tower));
}


std::pair<int, int> EcalElectronicsMapping::getDCCandSC(EcalScDetId id) const {

// pair.first = DCC id
// pair.second = DCC_channel
// For digi2raw, read the SRflags and write the SR block :
// need to find out, for an EcalScDetId, which is the DCC and the DCC_channel

	std::pair<int,int> ind;
	EEDetId dum;
	int ix = id.ix();
	int iy = id.iy();
	int zside = id.zside();
	ix = (ix-1)*5 +1;
	iy = (iy-1)*5 +1;
	ix = 5 * (ix/5) +1;
	iy = 5 * (iy/5) +1;
 	int ix_c = ix;
	int iy_c = iy;
	if (! dum.validDetId(ix_c,iy_c,zside) ) {
	    ix_c = ix +4;
            iy_c = iy ;
            if ( ! dum.validDetId(ix_c,iy_c,zside) ) {
	     ix_c = ix +4;
	     iy_c = iy +4;
	     if ( ! dum.validDetId(ix_c,iy_c,zside) ) {
	      ix_c = ix;
	      iy_c = iy +4;
	     }
	    }
	}
	EEDetId eedetid(ix_c,iy_c,zside,EEDetId::XYMODE);
	EcalElectronicsId elid = getElectronicsId(eedetid);
	int Dccid = elid.dccId();
	int DCC_Channel = elid.towerId();
	ind.first = Dccid;
	ind.second = DCC_Channel;
	return ind;
}



EcalScDetId EcalElectronicsMapping::getEcalScDetId(int DCCid, int DCC_Channel) const {

// For unpacking of ST flags.

	std::vector<DetId> xtals = dccTowerConstituents(DCCid, DCC_Channel);
	if (xtals.size() == 0) throw cms::Exception("InvalidDetId") << 
		"EcalElectronicsMapping : can not create EcalScDetId for DCC " << DCCid << 
		" and DCC_Channel " << DCC_Channel << ".";

	EEDetId eedetid = xtals[0];
	int ix = eedetid.ix();
	int iy = eedetid.iy();
	int iz = eedetid.zside();
	int ix_SC = (ix-1)%5 + 1;
	int iy_SC = (iy-1)%5 + 1;
	EcalScDetId scdetid(ix_SC,iy_SC,iz);
	return scdetid;

}



EcalSubdetector EcalElectronicsMapping::subdet(int dcctcc, int mode) const {
  if (mode == DCCMODE) {
        if ( (dcctcc >= MIN_DCCID_EBM && dcctcc <= MAX_DCCID_EBM) ||
             (dcctcc >= MIN_DCCID_EBP && dcctcc <= MAX_DCCID_EBP) ) return EcalBarrel;
        else return EcalEndcap;
  }
  else if (mode == TCCMODE) {
        if ( (dcctcc >= MIN_TCCID_EBM && dcctcc <= MAX_TCCID_EBM) ||
             (dcctcc >= MIN_TCCID_EBP && dcctcc <= MAX_TCCID_EBP) ) return EcalBarrel;
        else return EcalEndcap;
  }
  else throw cms::Exception("InvalidDetId") << " Wrong mode in EcalElectronicsMapping::subdet " << mode << ".";
}


int EcalElectronicsMapping::zside(int dcctcc, int mode) const {
  if (mode == DCCMODE) {
	if (dcctcc >= MIN_DCCID_EBM && dcctcc <= MAX_DCCID_EBM) return -1;
	if (dcctcc >= MIN_DCCID_EBP && dcctcc <= MAX_DCCID_EBP) return +1;
	if (dcctcc >= MIN_DCCID_EEM && dcctcc <= MAX_DCCID_EEM) return -1;
        if (dcctcc >= MIN_DCCID_EEP && dcctcc <= MAX_DCCID_EEP) return +1; 
  }
  else if (mode == TCCMODE) {
        if (dcctcc >= MIN_TCCID_EBM && dcctcc <= MAX_TCCID_EBM) return -1;
        if (dcctcc >= MIN_TCCID_EBP && dcctcc <= MAX_TCCID_EBP) return +1;
        if (dcctcc >= MIN_TCCID_EEM && dcctcc <= MAX_TCCID_EEM) return -1;
        if (dcctcc >= MIN_TCCID_EEP && dcctcc <= MAX_TCCID_EEP) return +1;
  }
  else {
	throw cms::Exception("InvalidDetId") << " Wrong mode in EcalElectronicsMapping::zside " << mode << ".";
  }
  return 0;
}



bool EcalElectronicsMapping::rightTower(int tower) const {
	// for EB, two types of tower (LVRB top/bottom)

  if ((tower>12 && tower<21) || (tower>28 && tower<37) ||
       (tower>44 && tower<53) || (tower>60 && tower<69))
    return true;
  else
    return false;
}








