#include "EventFilter/EcalRawToDigiDev/interface/DCCTowerBlock.h"
#include "EventFilter/EcalRawToDigiDev/interface/DCCEventBlock.h"
#include "EventFilter/EcalRawToDigiDev/interface/DCCDataUnpacker.h"
#include <stdio.h>
#include "EventFilter/EcalRawToDigiDev/interface/EcalElectronicsMapper.h"



DCCTowerBlock::DCCTowerBlock( DCCDataUnpacker * u, EcalElectronicsMapper * m, DCCEventBlock * e, bool unpack )
: DCCFEBlock(u,m,e,unpack){}


void DCCTowerBlock::updateCollectors(){

  DCCFEBlock::updateCollectors();
  
  // needs to be update for eb/ee
  digis_                 = unpacker_->ebDigisCollection();
   
  invalidGains_          = unpacker_->invalidGainsCollection();
  invalidGainsSwitch_    = unpacker_->invalidGainsSwitchCollection();
  invalidChIds_          = unpacker_->invalidChIdsCollection();

}



int DCCTowerBlock::unpackXtalData(uint expStripID, uint expXtalID){
  
  bool errorOnXtal(false);
 
  uint16_t * xData_= reinterpret_cast<uint16_t *>(data_);

  // Get xtal data ids
  uint stripId  = (*xData_)                     & TOWER_STRIPID_MASK;
  uint xtalId   = ((*xData_)>>TOWER_XTALID_B )  & TOWER_XTALID_MASK;

  // check id in case data are not 0suppressed
  if( !zs_ && (expStripID != stripId || expXtalID != xtalId)){ 
    if(! DCCDataUnpacker::silentMode_){ 
      edm::LogWarning("EcalRawToDigiDevChId")
        <<"\n For event L1A: "<<event_->l1A()<<", fed "<<mapper_->getActiveDCC()<<" and tower "<<towerId_
        <<"\n The expected strip is "<<expStripID<<" and "<<stripId<<" was found"
        <<"\n The expected xtal  is "<<expXtalID <<" and "<<xtalId<<" was found";	
    } 
    // using expected cry_di to raise warning about xtal_id problem
    pDetId_ = (EBDetId*) mapper_->getDetIdPointer(towerId_,expStripID,expXtalID);
    (*invalidChIds_)->push_back(*pDetId_);
    
    stripId    = expStripID;
    xtalId     = expXtalID;
    errorOnXtal= true;
    
    // return here, so to skip all following checks
    lastXtalId_++;
    if (lastXtalId_ > NUMB_XTAL) 	{lastXtalId_=1; lastStripId_++;}
    data_ += numbDWInXtalBlock_;
    return BLOCK_UNPACKED;
  }


  // check id in case of 0suppressed data

  else if(zs_){

    // Check for valid Ids 1) values out of range

    if(stripId == 0 || stripId > 5 || xtalId == 0 || xtalId > 5){
      if( ! DCCDataUnpacker::silentMode_ ){
        edm::LogWarning("EcalRawToDigiDevChId")
          <<"\n For event L1A: "<<event_->l1A()<<", fed "<<mapper_->getActiveDCC()<<" and tower "<<towerId_
          <<"\n Invalid strip : "<<stripId<<" or xtal : "<<xtalId
          <<" ids ( last strip was: " << lastStripId_ << " last ch was: " << lastXtalId_ << ")";
      }
      
      int st = lastStripId_;
      int ch = lastXtalId_;
      ch++;
      if (ch > NUMB_XTAL) 	{ch=1; st++;}
      if (st > NUMB_STRIP)	{ch=1; st=1;}
      
      // adding channel following the last valid
      //pDetId_ = (EBDetId*) mapper_->getDetIdPointer(towerId_,st,ch);
      //(*invalidChIds_)->push_back(*pDetId_);
      fillEcalElectronicsError(invalidZSXtalIds_);

      errorOnXtal = true;

      lastStripId_ = st;
      lastXtalId_  = ch;
      
      // return here, so to skip all following checks
      return SKIP_BLOCK_UNPACKING;
      
    }else{

      // Check for zs valid Ids 2) if channel-in-strip has increased wrt previous xtal
      
      
      // Check for zs valid Ids 2) if channel-in-strip has increased wrt previous xtal
      //                        3) if strip has increased wrt previous xtal
      if( ( stripId == lastStripId_ && xtalId <= lastXtalId_ ) ||
	  (stripId < lastStripId_))
	{
	  if( ! DCCDataUnpacker::silentMode_ ){  
  	    edm::LogWarning("EcalRawToDigiDevChId")
              <<"\n For event L1A: "<<event_->l1A()<<", fed "<<mapper_->getActiveDCC()<<" and tower "<<towerId_
              <<"\n Xtal id was expected to increase but it didn't. "
              <<"\n Last valid unpacked xtal was "<<lastXtalId_<<" while current xtal is "<<xtalId<<".";
          }
	  
	  int st = lastStripId_;
	  int ch = lastXtalId_;
	  ch++;
	  if (ch > NUMB_XTAL)	{ch=1; st++;}
	  if (st >  NUMB_STRIP)	{ch=1; st=1;}
	  
	  // adding channel following the last valid
	  //pDetId_ = (EBDetId*) mapper_->getDetIdPointer(towerId_,st,ch);
	  //(*invalidChIds_)->push_back(*pDetId_);
          fillEcalElectronicsError(invalidZSXtalIds_);
	  
	  errorOnXtal = true;
	  lastStripId_ = st;
	  lastXtalId_  = ch;

	  // return here, so to skip all following checks
	  return SKIP_BLOCK_UNPACKING;
	  
	}
      
      // if channel id not proven wrong, update lastStripId_ and lastXtalId_
      lastStripId_  = stripId;
      lastXtalId_   = xtalId;
    }//end else
  }// end if (zs_)


  bool frameAdded=false;

  // if there is an error on xtal id ignore next error checks  
  // otherwise, assume channel_id is valid and proceed with making and checking the data frame
  if(errorOnXtal) return SKIP_BLOCK_UNPACKING;

  pDetId_ = (EBDetId*) mapper_->getDetIdPointer(towerId_,stripId,xtalId);
  (*digis_)->push_back(*pDetId_);
  EBDataFrame df( (*digis_)->back() );
  frameAdded=true;  
  bool wrongGain(false);
  
       //set samples in the frame
      for(uint i =0; i< nTSamples_ ;i++){ 
        xData_++;
        uint data =  (*xData_) & TOWER_DIGI_MASK;
        uint gain =  data>>12;
        xtalGains_[i]=gain;
        if(gain == 0){ 
	    wrongGain = true; 
	    // continue here to skip part of the loop
	    // as well as add the error to the collection and write the message 
	    break;
	} 
	
	df.setSample(i,data);
      }
	
    
      if(wrongGain){ 
        if( ! DCCDataUnpacker::silentMode_ ){
          edm::LogWarning("EcalRawToDigiDevGainZero")
	    <<"\n For event L1A: "<<event_->l1A()<<", fed "<<mapper_->getActiveDCC()<<" and tower "<<towerId_
	    <<"\n Gain zero was found in strip "<<stripId<<" and xtal "<<xtalId;   
         }
	
	(*invalidGains_)->push_back(*pDetId_);
        errorOnXtal = true;
	
	//return here, so to skip all the rest
	//make special collection for gain0 data frames when due to saturation
	//Point to begin of next xtal Block
	data_ += numbDWInXtalBlock_;
	
	return BLOCK_UNPACKED;

      }
      
      short firstGainWrong=-1;
      short numGainWrong=0;
	    
      for (uint i=1; i<nTSamples_; i++ ) {
        if (i>0 && xtalGains_[i-1]>xtalGains_[i]) {
          numGainWrong++;
          if (firstGainWrong == -1) { firstGainWrong=i;}
        }
      }


      if (numGainWrong>0) {
        if( ! DCCDataUnpacker::silentMode_ ){
          edm::LogWarning("EcalRawToDigiDevGainSwitch")
            <<"\n For event L1A: "<<event_->l1A()<<", fed "<<mapper_->getActiveDCC()<<" and tower "<<towerId_
            <<"\n A wrong gain transition switch was found in strip "<<stripId<<" and xtal "<<xtalId;    
        }

        (*invalidGainsSwitch_)->push_back(*pDetId_);

         errorOnXtal = true;
      } 

      //Add frame to collection only if all data format and gain rules are respected
      if(errorOnXtal&&frameAdded) {
	(*digis_)->pop_back();
      }
 
      //Point to begin of next xtal Block
      data_ += numbDWInXtalBlock_;

      return BLOCK_UNPACKED;
}



void DCCTowerBlock::fillEcalElectronicsError( std::auto_ptr<EcalElectronicsIdCollection> * errorColection ){

   int activeDCC = mapper_->getActiveSM();

   if(NUMB_SM_EB_MIN_MIN<=activeDCC && activeDCC<=NUMB_SM_EB_PLU_MAX){
     EcalElectronicsId  *  eleTp = mapper_->getTTEleIdPointer(activeDCC+TCCID_SMID_SHIFT_EB,expTowerID_);
     (*errorColection)->push_back(*eleTp);
   }else{
      if( ! DCCDataUnpacker::silentMode_ ){
          edm::LogWarning("EcalRawToDigiDevChId")
            <<"\n For event "<<event_->l1A()<<" there's fed: "<< activeDCC
            <<" activeDcc: "<<mapper_->getActiveSM()
            <<" but that activeDcc is not valid in EB.";
        }

   }

}


