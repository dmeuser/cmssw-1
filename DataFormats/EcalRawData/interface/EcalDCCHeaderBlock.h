#ifndef RAWECAL_ECALDCCHEADERBLOCK_H
#define RAWECAL_ECALDCCHEADERBLOCK_H

#include <boost/cstdint.hpp>

/** \class EcalDCCHeaderBlock
 *  Container for ECAL specific DCC Header information
 *
 *
 *  $Id: EcalDCCHeaderBlock.h,v 1.1 2006/01/30 16:11:14 meridian Exp $
 */

#include <vector>

#define MAX_TCC_SIZE 4
#define MAX_TT_SIZE 70
enum EcalDCCRuntype{
  PHYSICS = 0,
  LASER_STD = 1,
  LASER_POWER_SCAN = 2,
  LASER_DELAY_SCAN = 3,
  TESTPULSE_SCAN_MEM = 4,
  TESTPULSE_MGPA = 5,
  PEDESTAL_STD = 6,
  PEDESTAL_OFFSET_SCAN = 7,
  PEDESTAL_25NS_SCAN = 8,
  LED_STD= 9
};

struct EcalDCCEventSettings{
 short LaserPower;
 short LaserFilter;
 short wavelength;
 short delay;
 short MEMVinj;
 short mgpa_content;
 short ped_offset;
};

class EcalDCCHeaderBlock
{

 public:
  typedef int key_type; ///< For the sorted collection 

  EcalDCCHeaderBlock();
  EcalDCCHeaderBlock(const int& dccId);
  
  const int& id() const { return dccId_; }
  void setId(const int& dccId) { dccId_=dccId; };
  void setErrors(const int& dccErrors) { dccErrors_=dccErrors; };
  void setRunNumber(const int& run){runNumber_ = run;}
  void setLV1(const int& LV1){LV1event_ = LV1;}
  void setBX(const int& BX){BX_ = BX;}
  void setEventSettings(const  EcalDCCEventSettings& EventSettings) { EventSettings_=EventSettings; };
  void setRunType(const short& runType) { runType_=runType; };
  //void setSequence(const short& sequence) { sequence_=sequence; } ;
  void setRtHalf(const short& rtHalf) { rtHalf_=rtHalf; } ;
  void setMgpaGain(const short& mgpaGain) { mgpaGain_=mgpaGain; };
  void setMemGain(const short& memGain) { memGain_=memGain; };
  void setSelectiveReadout(const bool& selectiveReadout) { selectiveReadout_=selectiveReadout; };
  void setZeroSuppression(const bool& zeroSuppression) { zeroSuppression_=zeroSuppression; };
  void setTestZeroSuppression(const bool& testZeroSuppression) { testZeroSuppression_ = testZeroSuppression; };
  void setSrpStatus(const short& srpStatus) { srpStatus_=srpStatus; };
  void setTccStatus(const std::vector<short>& tccStatus) { tccStatus_=tccStatus; };
  void setTriggerTowerStatus(const std::vector<short>& triggerTowerStatus) { triggerTowerStatus_ = triggerTowerStatus; };

  //TODO add all the get methods
  
  int getDCCErrors() const{ return dccErrors_;}
  int gettRunNumber() const {return runNumber_ ;}
  int getLV1() const {return LV1event_ ;}
  int getBX() const {return BX_ ;}
  EcalDCCEventSettings getEventSettings() const { return EventSettings_;}
  short getRunType() const {return runType_ ;}

  short getRtHalf() const { return rtHalf_; } 
  short getMgpaGain() const { return mgpaGain_;}
  short getMemGain() const  { return memGain_;}
  short getSelectiveReadout() const { return selectiveReadout_;}
  bool getZeroSuppression() const { return zeroSuppression_;}
  bool getTestZeroSuppression() const {return testZeroSuppression_ ;}
  short getSrpStatus() const  { return srpStatus_;}
  std::vector<short> getTccStatus() const { return tccStatus_ ;}
  std::vector<short> getTriggerTowerStatus() const { return triggerTowerStatus_ ;}
 private:

  int dccId_;  //to be used as the Key
  int dccErrors_;
  long orbitNumber_; // do we need it here?
  short runType_;

  int LV1event_;
  int runNumber_;
  int BX_;
  EcalDCCEventSettings  EventSettings_;
  
  short rtHalf_;
  short mgpaGain_;
  short memGain_;
  bool selectiveReadout_;
  bool testZeroSuppression_;
  bool zeroSuppression_;

  short srpStatus_;
  std::vector<short> tccStatus_;
  std::vector<short> triggerTowerStatus_;
 
};

#endif
