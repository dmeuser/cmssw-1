
#ifndef EventFilter_SiStripRawToDigi_SiStripRawToDigiUnpacker_H
#define EventFilter_SiStripRawToDigi_SiStripRawToDigiUnpacker_H

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/DetId/interface/DetIdCollection.h"
#include "DataFormats/SiStripCommon/interface/SiStripConstants.h"
#include "EventFilter/SiStripRawToDigi/interface/SiStripFEDBuffer.h"
#include "boost/cstdint.hpp"
#include <iostream>
#include <string>
#include <vector>
 
/// Fed9U classes
namespace Fed9U { class Fed9UEvent; }

/// sistrip classes
class OldSiStripRawToClustersLazyUnpacker; 
namespace sistrip { class RawToClustersLazyUnpacker; }
namespace sistrip { class RawToDigiUnpacker; }

/// other classes
class FEDRawDataCollection;
class FEDRawData;
class SiStripDigi;
class SiStripRawDigi;
class SiStripEventSummary;
class SiStripFedCabling;

/**
   @file EventFilter/SiStripRawToDigi/interface/SiStripRawToDigiUnpacker.h
   @class sistrip::RawToDigiUnpacker 
   @brief Takes collection of FEDRawData as input and creates digis.
*/

class OldSiStripRawToDigiUnpacker {
  
 public:
  
  typedef edm::DetSetVector<SiStripDigi> Digis;
  typedef edm::DetSetVector<SiStripRawDigi> RawDigis;
  
  /// constructor
  OldSiStripRawToDigiUnpacker( int16_t appended_bytes, int16_t fed_buffer_dump_freq, int16_t fed_event_dump_freq, int16_t trigger_fed_id, bool using_fed_key );
  
  /// default constructor
  ~OldSiStripRawToDigiUnpacker();
  
  /// creates digis
  void createDigis( const SiStripFedCabling&, const FEDRawDataCollection&, SiStripEventSummary&, RawDigis& scope_mode, RawDigis& virgin_raw, RawDigis& proc_raw, Digis& zero_suppr );
  
  /// sets the SiStripEventSummary 
  void updateEventSummary( const Fed9U::Fed9UEvent* const, SiStripEventSummary& );
  
  /// retrieve unpacker
  inline sistrip::RawToDigiUnpacker* unpacker();
  
 private:
  
  sistrip::RawToDigiUnpacker* unpacker_;
  Fed9U::Fed9UEvent* fedEvent_;

};

inline sistrip::RawToDigiUnpacker* OldSiStripRawToDigiUnpacker::unpacker() { return unpacker_; }

namespace sistrip {
  
  class RawToDigiUnpacker {
    
    friend class ::OldSiStripRawToDigiUnpacker;
    friend class RawToClustersLazyUnpacker;
    friend class ::OldSiStripRawToClustersLazyUnpacker;
    
  public:
    
    typedef edm::DetSetVector<SiStripDigi> Digis;
    typedef edm::DetSetVector<SiStripRawDigi> RawDigis;
    
    /// constructor
    RawToDigiUnpacker( int16_t appended_bytes, int16_t fed_buffer_dump_freq, int16_t fed_event_dump_freq, int16_t trigger_fed_id, bool using_fed_key );
    
    /// default constructor
    ~RawToDigiUnpacker();
    
    /// creates digis
    void createDigis( const SiStripFedCabling&, const FEDRawDataCollection&, SiStripEventSummary&, RawDigis& scope_mode, RawDigis& virgin_raw, RawDigis& proc_raw, Digis& zero_suppr, DetIdCollection& );
    
    /// trigger info
    void triggerFed( const FEDRawDataCollection&, SiStripEventSummary&, const uint32_t& event );
    
    /// Removes any data appended prior to FED buffer and reorders 32-bit words if swapped.
    void locateStartOfFedBuffer( const uint16_t& fed_id, const FEDRawData& input, FEDRawData& output );
    
    /// verbosity
    inline void quiet( bool );
    
    /// EventSummary update request -> not yet implemented for FEDBuffer class
    inline void useDaqRegister( bool );
    
  private:
    
    /// fill DetSetVectors using registries
    void update( RawDigis& scope_mode, RawDigis& virgin_raw, RawDigis& proc_raw, Digis& zero_suppr );
    
    /// private default constructor
    RawToDigiUnpacker();
    
    /// sets the SiStripEventSummary -> not yet implemented for FEDBuffer class
    void updateEventSummary( const sistrip::FEDBuffer&, SiStripEventSummary& );
    
    /// order of strips
    inline void readoutOrder( uint16_t& physical_order, uint16_t& readout_order );
    
    /// order of strips
    inline void physicalOrder( uint16_t& readout_order, uint16_t& physical_order ); 
    
    /// returns buffer format
    inline sistrip::FedBufferFormat fedBufferFormat( const uint16_t& register_value );
    
    /// returns buffer readout mode
    inline sistrip::FedReadoutMode fedReadoutMode( const uint16_t& register_value );
    
    /// dumps raw data to stdout (NB: payload is byte-swapped,headers/trailer are not).
    void dumpRawData( uint16_t fed_id, const FEDRawData&, std::stringstream& );
    
    /// catches all possible exceptions and rethrows them as cms::Exceptions  
    void handleException( std::string method_name, std::string extra_info = "" );
    
    /// method to clear registries and digi collections
    void cleanupWorkVectors();
    
    /// private class to register start and end index of digis in a collection
    class Registry {
    public:
      /// constructor
      Registry(uint32_t aDetid, uint16_t firstStrip, size_t indexInVector, uint16_t numberOfDigis) : 
	detid(aDetid), first(firstStrip), index(indexInVector), length(numberOfDigis) {}
      /// < operator to sort registries
      bool operator<(const Registry &other) const {return (detid != other.detid ? detid < other.detid : first < other.first);}
      /// public data members
      uint32_t detid;
      uint16_t first;
      size_t index;
      uint16_t length;
    };
    
    /// configurables
    int16_t headerBytes_;
    int16_t fedBufferDumpFreq_;
    int16_t fedEventDumpFreq_;
    int16_t triggerFedId_;
    bool useFedKey_;
    
    /// other values
    uint32_t event_;
    bool once_;
    bool first_;
    bool useDaqRegister_;
    bool quiet_;
    
    /// registries
    std::vector<Registry> zs_work_registry_;
    std::vector<Registry> virgin_work_registry_;
    std::vector<Registry> scope_work_registry_;
    std::vector<Registry> proc_work_registry_;
    
    /// digi collections
    std::vector<SiStripDigi> zs_work_digis_;
    std::vector<SiStripRawDigi> virgin_work_digis_; 
    std::vector<SiStripRawDigi> scope_work_digis_; 
    std::vector<SiStripRawDigi> proc_work_digis_;
  };
  
}

void sistrip::RawToDigiUnpacker::readoutOrder( uint16_t& physical_order, uint16_t& readout_order ) 
{
  readout_order = ( 4*((static_cast<uint16_t>((static_cast<float>(physical_order)/8.0)))%4) + static_cast<uint16_t>(static_cast<float>(physical_order)/32.0) + 16*(physical_order%8) );
}

void sistrip::RawToDigiUnpacker::physicalOrder( uint16_t& readout_order, uint16_t& physical_order ) 
{
  physical_order = ( (32 * (readout_order%4)) + (8 * static_cast<uint16_t>(static_cast<float>(readout_order)/4.0)) - (31 * static_cast<uint16_t>(static_cast<float>(readout_order)/16.0)) );
}

sistrip::FedBufferFormat sistrip::RawToDigiUnpacker::fedBufferFormat( const uint16_t& register_value ) 
{
  if ( (register_value&0xF) == 0x1 ) { return sistrip::FULL_DEBUG_FORMAT; }
  else if ( (register_value&0xF) == 0x2 ) { return sistrip::APV_ERROR_FORMAT; }
  else if ( (register_value&0xF) == 0x0 ) { return sistrip::UNDEFINED_FED_BUFFER_FORMAT; }
  else { return sistrip::UNKNOWN_FED_BUFFER_FORMAT; }
}

sistrip::FedReadoutMode sistrip::RawToDigiUnpacker::fedReadoutMode( const uint16_t& register_value ) 
{
  if ( ((register_value>>1)&0x7) == 0x0 ) { return sistrip::FED_SCOPE_MODE; }
  else if ( ((register_value>>1)&0x7) == 0x1 ) { return sistrip::FED_VIRGIN_RAW; }
  else if ( ((register_value>>1)&0x7) == 0x3 ) { return sistrip::FED_PROC_RAW; }
  else if ( ((register_value>>1)&0x7) == 0x5 ) { return sistrip::FED_ZERO_SUPPR; }
  else if ( ((register_value>>1)&0x7) == 0x6 ) { return sistrip::FED_ZERO_SUPPR_LITE; }
  else { return sistrip::UNKNOWN_FED_READOUT_MODE; }
}

void sistrip::RawToDigiUnpacker::quiet( bool quiet ) { quiet_ = quiet; }

void sistrip::RawToDigiUnpacker::useDaqRegister( bool use ) { useDaqRegister_ =  use; }


#endif // EventFilter_SiStripRawToDigi_SiStripRawToDigiUnpacker_H



