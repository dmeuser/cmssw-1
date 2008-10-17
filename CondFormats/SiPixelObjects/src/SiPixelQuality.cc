// SiPixelQuality.cc
//
// class implementation to hold a list of disabled pixel modules
//
// M. Eads
// Apr 2008

#include "CondFormats/SiPixelObjects/interface/SiPixelQuality.h"

#include <algorithm>
#include <iostream>


      //////////////////////////////////////
      //  errortype "whole" = int 0 in DB //
      //  errortype "tbmA" = int 1 in DB  //
      //  errortype "tbmB" = int 2 in DB  //
      //  errortype "none" = int 3 in DB  //
      //////////////////////////////////////
    
      /////////////////////////////////////////////////
      //each bad roc correspond to a bit to 1: num=  //
      // 0 <-> all good rocs                         //
      // 1 <-> only roc 0 bad                        //
      // 2<-> only roc 1 bad                         //
      // 3<->  roc 0 and 1 bad                       //
      // 4 <-> only roc 2 bad                        //
      //  ...                                        //
      /////////////////////////////////////////////////


// add a list of modules to the vector of disabled modules
void SiPixelQuality::addDisabledModule(std::vector<SiPixelQuality::disabledModuleType> & idVector) {
  theDisabledModules.insert(theDisabledModules.end(),
			    idVector.begin(),
			    idVector.end());

} 


// bool SiPixelQuality::removeDisabledModule(const uint32_t & detid){
//   std::vector<disabledModuleType>::const_iterator iter = std::lower_bound(theDisabledModules.begin(),theDisabledModules.end(),detid,SiPixelQuality::BadComponentStrictWeakOrdering());
//   if (iter != theDisabledModules.end() && iter->DetID==detid){
//     theDisabledModules.erase(iter);
//     return true;
//   }
//   return false;
// }

// ask if module is OK
bool SiPixelQuality::IsModuleUsable(const uint32_t& detid) const {
   if(IsFedBad(detid))
     return true;
   std::vector<SiPixelQuality::disabledModuleType>disabledModules = theDisabledModules;
   std::sort(disabledModules.begin(),disabledModules.end(),SiPixelQuality::BadComponentStrictWeakOrdering());
   std::vector<disabledModuleType>::const_iterator iter = std::lower_bound(disabledModules.begin(),disabledModules.end(),detid,SiPixelQuality::BadComponentStrictWeakOrdering());
   if (iter != disabledModules.end() && iter->DetID==detid && iter->errorType ==0)
    return false;
  return true;
}

//ask if module is bad
 bool SiPixelQuality::IsModuleBad(const uint32_t & detid) const {
   if(IsFedBad(detid))
     return true;
   std::vector<SiPixelQuality::disabledModuleType>disabledModules = theDisabledModules;
   std::sort(disabledModules.begin(),disabledModules.end(),SiPixelQuality::BadComponentStrictWeakOrdering());
   std::vector<disabledModuleType>::const_iterator iter = std::lower_bound(disabledModules.begin(),disabledModules.end(),detid,SiPixelQuality::BadComponentStrictWeakOrdering());
   if (iter != disabledModules.end() && iter->DetID==detid && iter->errorType == 0) //errorType 0 corresponds to "whole" dead module
      return true;
   return false;
}

//ask if roc is bad
bool SiPixelQuality::IsRocBad(const uint32_t& detid, const short& rocNb) const {
  if(IsModuleBad(detid))
    return true;
   std::vector<SiPixelQuality::disabledModuleType>disabledModules = theDisabledModules;
   std::sort(disabledModules.begin(),disabledModules.end(),SiPixelQuality::BadComponentStrictWeakOrdering());
   std::vector<disabledModuleType>::const_iterator iter = std::lower_bound(disabledModules.begin(),disabledModules.end(),detid,SiPixelQuality::BadComponentStrictWeakOrdering());
   if (iter != disabledModules.end() && iter->DetID == detid){
     return ((iter->BadRocs >> rocNb)&0x1);}
  return false;
}

//ask for bad ROCS


short SiPixelQuality::getBadRocs(const uint32_t& detid) const{
  std::vector<SiPixelQuality::disabledModuleType>disabledModules = theDisabledModules;
  std::sort(disabledModules.begin(),disabledModules.end(),SiPixelQuality::BadComponentStrictWeakOrdering());
  std::vector<disabledModuleType>::const_iterator iter = std::lower_bound(disabledModules.begin(),disabledModules.end(),detid,SiPixelQuality::BadComponentStrictWeakOrdering());
  if (iter != disabledModules.end() && iter->DetID==detid)
    return iter->BadRocs;
  return 0;
}

const std::vector< std::pair <uint8_t, uint8_t> > SiPixelQuality::getBadRocPositions(const uint32_t & detid) const{
  std::vector< std::pair <uint8_t, uint8_t> > badrocpositions (0);
  std::pair<uint8_t, uint8_t> coord(1,1);
  //  badrocpositions.push_back(coord);
  return badrocpositions;
}


bool SiPixelQuality::IsFedBad(const uint32_t & detid) const{
  return false;
}



