#include "SimCalorimetry/HcalZeroSuppressionAlgos/interface/HcalZeroSuppressionAlgo.h"
#include "Geometry/HcalTowerAlgo/interface/HcalTrigTowerGeometry.h"

HcalZeroSuppressionAlgo::HcalZeroSuppressionAlgo(ZSMode mode) : m_mode(mode) {
}


void HcalZeroSuppressionAlgo::suppress(const HBHEDigiCollection& input, HBHEDigiCollection& output) {
  HBHEDigiCollection::const_iterator i;
  if (m_mode==zs_SingleChannel) {
    for (i=input.begin(); i!=input.end(); ++i) 
      if (shouldKeep(*i)) output.push_back(*i);
  } else if (m_mode==zs_TriggerTowerOR || m_mode==zs_AllDepthsOR) {
    HcalTrigTowerGeometry ttg;
    std::set<HcalTrigTowerDetId> passed;
    // get list of above-threshold trigger tower ids
    for (i=input.begin(); i!=input.end(); ++i) 
      if (shouldKeep(*i)) {
	std::vector<HcalTrigTowerDetId> tids=ttg.towerIds(i->id());
	if (!tids.empty()) passed.insert(tids[0]); // only use the first (order is stable for all channels)
      }
    // loop over digis again, checking to see if trig tower had passed
    for (i=input.begin(); i!=input.end(); ++i) {
      std::vector<HcalTrigTowerDetId> tids=ttg.towerIds(i->id());
      if (!tids.empty() && passed.find(tids[0])!=passed.end()) output.push_back(*i);
    }
  }
}

void HcalZeroSuppressionAlgo::suppress(const HFDigiCollection& input, HFDigiCollection& output) {
  HFDigiCollection::const_iterator i;
  if (m_mode==zs_SingleChannel) {   
    for (i=input.begin(); i!=input.end(); ++i) 
      if (shouldKeep(*i)) output.push_back(*i);
  } else if (m_mode==zs_TriggerTowerOR) {
    HcalTrigTowerGeometry ttg;
    std::set<HcalTrigTowerDetId> passed;
    // get list of above-threshold trigger tower ids
    for (i=input.begin(); i!=input.end(); ++i) 
      if (shouldKeep(*i)) {
	std::vector<HcalTrigTowerDetId> tids=ttg.towerIds(i->id());
	if (!tids.empty()) passed.insert(tids[0]); // only use the first (order is stable for all channels)
      }
    // loop over digis again, checking to see if trig tower had passed
    for (i=input.begin(); i!=input.end(); ++i) {
      std::vector<HcalTrigTowerDetId> tids=ttg.towerIds(i->id());
      if (!tids.empty() && passed.find(tids[0])!=passed.end()) output.push_back(*i);
    }
  } else if (m_mode==zs_AllDepthsOR) {
    std::set<HcalDetId> passed;
    // get list of above-threshold trigger tower ids
    for (i=input.begin(); i!=input.end(); ++i) 
      if (shouldKeep(*i)) {
	passed.insert(i->id()); 
      }
    // loop over digis again, checking to see if trig tower had passed
    for (i=input.begin(); i!=input.end(); ++i) {
      HcalDetId a(HcalForward,i->id().ieta(),i->id().iphi(),1); // depth 1
      HcalDetId b(HcalForward,i->id().ieta(),i->id().iphi(),2); // depth 2
      if (passed.find(a)!=passed.end() || passed.find(b)!=passed.end()) output.push_back(*i);
    }
  }  
}

void HcalZeroSuppressionAlgo::suppress(const HODigiCollection& input, HODigiCollection& output) {
  if (m_mode==zs_SingleChannel || m_mode==zs_TriggerTowerOR || m_mode==zs_AllDepthsOR) {
    HODigiCollection::const_iterator i;
    for (i=input.begin(); i!=input.end(); ++i) 
      if (shouldKeep(*i)) output.push_back(*i);
  }
}




