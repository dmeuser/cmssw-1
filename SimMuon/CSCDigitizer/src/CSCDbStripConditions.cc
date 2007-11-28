#include "SimMuon/CSCDigitizer/src/CSCDbStripConditions.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "CondFormats/DataRecord/interface/CSCDBGainsRcd.h"
#include "CondFormats/DataRecord/interface/CSCDBPedestalsRcd.h"
#include "CondFormats/DataRecord/interface/CSCDBNoiseMatrixRcd.h"
#include "CondFormats/DataRecord/interface/CSCDBCrosstalkRcd.h"


CSCDbStripConditions::CSCDbStripConditions(const edm::ParameterSet & pset) 
: CSCStripConditions(),
  theNoiseMatrix(0),
  theGains(0),
  thePedestals(0),
  theCrosstalk(0),
  theCapacitiveCrosstalk(pset.getParameter<double>("capacativeCrosstalk")),
  theGainsConstant(pset.getParameter<double>("gainsConstant")),
  doCorrelatedNoise_(pset.getParameter<bool>("doCorrelatedNoise"))
{
//  theCapacitiveCrosstalk = = 1/maxslope/maxsignal) = 1/ (0.00231/0.143);
// Howoever, need a bit more.  Maybe the slope gets smeared?
}


CSCDbStripConditions::~CSCDbStripConditions()
{
  //delete theNoiseMatrix;
  //delete theGains;
  //delete thePedestals;
  //delete theCrosstalk;
  if(theNoisifier != 0) delete theNoisifier;
}


void CSCDbStripConditions::initializeEvent(const edm::EventSetup & es)
{
  // Strip gains
  edm::ESHandle<CSCDBGains> hGains;
  es.get<CSCDBGainsRcd>().get( hGains );
  theGains = &*hGains.product();
  // Strip X-talk
  edm::ESHandle<CSCDBCrosstalk> hCrosstalk;
  es.get<CSCDBCrosstalkRcd>().get( hCrosstalk );
  theCrosstalk = &*hCrosstalk.product();
  // Strip pedestals
  edm::ESHandle<CSCDBPedestals> hPedestals;
  es.get<CSCDBPedestalsRcd>().get( hPedestals );
  thePedestals = &*hPedestals.product();

  // Strip autocorrelation noise matrix
  edm::ESHandle<CSCDBNoiseMatrix> hNoiseMatrix;
  es.get<CSCDBNoiseMatrixRcd>().get(hNoiseMatrix);
  theNoiseMatrix = &*hNoiseMatrix.product();

//  print();
}


void CSCDbStripConditions::print() const
{
/*
  std::cout << "SIZES: GAINS: " << theGains->gains.size()
            << "   PEDESTALS: " << thePedestals->pedestals.size()
            << "   NOISES "  << theNoiseMatrix->matrix.size() << std::endl;;

  std::map< int,std::vector<CSCDBGains::Item> >::const_iterator layerGainsItr = theGains->gains.begin(), 
      lastGain = theGains->gains.end();
  for( ; layerGainsItr != lastGain; ++layerGainsItr)
  {
    std::cout << "GAIN " << layerGainsItr->first 
              << " STRIPS " << layerGainsItr->second.size() << " "
              << layerGainsItr->second[0].gain_slope 
              << " " << layerGainsItr->second[0].gain_intercept << std::endl;
  }

  std::map< int,std::vector<CSCDBPedestals::Item> >::const_iterator pedestalItr = thePedestals->pedestals.begin(), 
                                                                  lastPedestal = thePedestals->pedestals.end();
  for( ; pedestalItr != lastPedestal; ++pedestalItr)
  {
    std::cout << "PEDS " << pedestalItr->first << " " 
              << " STRIPS " << pedestalItr->second.size() << " ";
    for(int i = 1; i < 80; ++i)
    {
       std::cout << pedestalItr->second[i-1].rms << " " ;
     }
     std::cout << std::endl;
  }

  std::map< int,std::vector<CSCDBCrosstalk::Item> >::const_iterator crosstalkItr = theCrosstalk->crosstalk.begin(),
                                                                  lastCrosstalk = theCrosstalk->crosstalk.end();
  for( ; crosstalkItr != lastCrosstalk; ++crosstalkItr)
  {
    std::cout << "XTALKS " << crosstalkItr->first 
      << " STRIPS " << crosstalkItr->second.size() << " "  
     << crosstalkItr->second[5].xtalk_slope_left << " " 
     << crosstalkItr->second[5].xtalk_slope_right << " " 
     << crosstalkItr->second[5].xtalk_intercept_left << " " 
     << crosstalkItr->second[5].xtalk_intercept_right << std::endl;
  }
*/
}


float CSCDbStripConditions::gain(const CSCDetId & detId, int channel) const
{
  assert(theGains != 0);
  return theGains->item(detId, channel).gain_slope * theGainsConstant;
}


CSCDBPedestals::Item CSCDbStripConditions::pedestalObject(const CSCDetId & detId, int channel) const
{
  assert(thePedestals != 0);
  return thePedestals->item(detId, channel);
}


float CSCDbStripConditions::pedestal(const CSCDetId & detId, int channel) const
{
  return pedestalObject(detId, channel).ped;
}


float CSCDbStripConditions::pedestalSigma(const CSCDetId&detId, int channel) const
{
  return pedestalObject(detId, channel).rms;
}


void CSCDbStripConditions::crosstalk(const CSCDetId&detId, int channel,
                 double stripLength, bool leftRight,
                 float & capacitive, float & resistive) const
{
  assert(theCrosstalk != 0);
  const CSCDBCrosstalk::Item & item = theCrosstalk->item(detId, channel);

  // resistive fraction is at the peak, where t=0
  resistive = leftRight ? item.xtalk_intercept_right 
                        : item.xtalk_intercept_left;
  
  // ns before the peak where slope is max
  float maxSlopeTime = 60.; 
  float slope = leftRight ? item.xtalk_slope_right
                          : item.xtalk_slope_left;
  // some confusion about +/-
  float capacitiveFraction = fabs(slope)*maxSlopeTime;
  // theCapacitiveCrosstalk is the number needed for 100% xtalk, so
  capacitive = theCapacitiveCrosstalk * capacitiveFraction;
}



void CSCDbStripConditions::fetchNoisifier(const CSCDetId & detId, int istrip)
{
  assert(theNoiseMatrix != 0);
  const CSCDBNoiseMatrix::Item & item = theNoiseMatrix->item(detId, istrip);

  CSCCorrelatedNoiseMatrix matrix;
  //TODO get the pedestals right
  matrix(3,3) = item.elem33;
  matrix(4,4) = item.elem44;
  matrix(5,5) = item.elem55;
  matrix(6,6) = item.elem66;
  matrix(7,7) = item.elem77;
  
  if(doCorrelatedNoise_)
  {
    matrix(3,4) = item.elem34;
    matrix(3,5) = item.elem35;
    matrix(4,5) = item.elem45;
    matrix(4,6) = item.elem46;
    matrix(5,6) = item.elem56;
    matrix(5,7) = item.elem57;
    matrix(6,7) = item.elem67;
  }

  // the other diagonal elements can just come from the pedestal sigma, I guess
  float sigma = pedestalSigma(detId, istrip);
  float scaVariance = 2 * sigma * sigma;
  matrix(0,0) = matrix(1,1) = matrix(2,2) = scaVariance;
  if(theNoisifier != 0) delete theNoisifier;
  theNoisifier = new CSCCorrelatedNoisifier(matrix);
}


int CSCDbStripConditions::dbIndex(const CSCDetId & id, int & channel)
{
  int ec = id.endcap();
  int st = id.station();
  int rg = id.ring();
  int ch = id.chamber();
  int la = id.layer();

  // there isn't really an ME1A.  It's channels 65-80 of ME11.
  if(st == 1 && rg == 4)
  {
    rg = 1;
    if(channel <= 64) channel += 64;
  }
  return ec*100000 + st*10000 + rg*1000 + ch*10 + la;
}



