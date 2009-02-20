#ifndef EVENTFILTER_CASTORRAWTODIGI_CASTORPACKER_H
#define EVENTFILTER_CASTORRAWTODIGI_CASTORPACKER_H 1

#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/FEDRawData/interface/FEDRawData.h"
#include "CondFormats/CastorObjects/interface/CastorElectronicsMap.h"
#include "EventFilter/CastorRawToDigi/interface/CastorCollections.h"

/** \class CastorPacker
  *  
  * $Date: 2008/06/19 09:03:17 $
  * $Revision: 1.1 $
  * \author J. Mans - Minnesota
  */
class CastorPacker {
public:

  void pack(int fedid, int dccnumber,
	    int nl1a, int orbitn, int bcn,
	    const CastorCollections& inputs, 
	    const CastorElectronicsMap& emap,
	    FEDRawData& output);
private:
  int findSamples(const DetId& did, const CastorCollections& inputs,
		  unsigned short* buffer, int &presamples);
};

#endif
