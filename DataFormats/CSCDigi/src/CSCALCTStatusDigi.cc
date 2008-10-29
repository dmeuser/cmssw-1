/** \class CSCALCTStatusDigi
 * 
 *  Digi for CSC ALCT info available in DDU
 *
 *  $Date: 2008/02/12 17:41:10 $
 *  $Revision: 1.3 $
 *
 */
#include <DataFormats/CSCDigi/interface/CSCALCTStatusDigi.h>

#include<iostream>

CSCALCTStatusDigi::CSCALCTStatusDigi(const uint16_t * header, const uint16_t * trailer)
{
  uint16_t headerSizeInBytes =16;
  uint16_t trailerSizeInBytes =8;
  memcpy(header_, header, headerSizeInBytes);
  memcpy(trailer_, trailer, trailerSizeInBytes);
}

std::ostream & operator<<(std::ostream & o, const CSCALCTStatusDigi& digi) {
  o << " ";  
  o <<"\n";

  return o;
}

