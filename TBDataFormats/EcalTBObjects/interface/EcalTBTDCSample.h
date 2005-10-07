#ifndef DIGIECAL_ECALTBTDCSAMPLE_H
#define DIGIECAL_ECALTBTDCSAMPLE_H 1


#include <ostream>
#include <boost/cstdint.hpp>

/** \class EcalTBTDCSample
 *  Simple container packer/unpacker for a single sample from the TB TDC raw data
 *
 *
 *  $Id: 
 */

class EcalTBTDCSample {
 public:
  EcalTBTDCSample() { theSample=0; }
  EcalTBTDCSample(uint32_t data) { theSample=data; }
  EcalTBTDCSample(int tdcChan, int tdcVal);
    
  /// get the raw word
  uint32_t raw() const { return theSample; }
  /// get the ADC sample (24 bits)
  int tdcValue() const { return theSample&0xFFFFFF; }
  /// get the gainId (8 bits)
  int tdcChannel() const { return (theSample>>24)&0xFF; }
  /// for streaming
  uint32_t operator()() { return theSample; }

 private:
  uint32_t theSample;
};

std::ostream& operator<<(std::ostream&, const EcalTBTDCSample&);
  
#endif
