#include "DataFormats/HcalDetId/interface/HcalElectronicsId.h"
#include <boost/cstdint.hpp> 
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDetId/interface/HcalTrigTowerDetId.h"
#include "DataFormats/HcalDetId/interface/HcalOtherDetId.h"
#include "DataFormats/HcalDetId/interface/HcalCalibDetId.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h" 
#include "DataFormats/HcalDetId/interface/HcalZDCDetId.h"
#include "DataFormats/HcalDetId/interface/HcalCastorDetId.h"
#include "DataFormats/HcalDetId/interface/CastorElectronicsId.h"
#include <vector>

namespace { 
 namespace {
   std::vector<HcalElectronicsId> eids;
   std::vector<CastorElectronicsId> ceids;
 }
}

