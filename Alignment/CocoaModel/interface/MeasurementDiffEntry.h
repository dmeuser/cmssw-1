// COCOA class header file
// Id:  MeasurementDiffEntry.h
// CAT: Model
//
// Class for measurements
// 
// History: v1.0 
// Authors:
//   Pedro Arce

#ifndef _MeasurementDiffEntry_HH
#define _MeasurementDiffEntry_HH

#include <vector>
#include "Alignment/CocoaModel/interface/Measurement.h"
#include "Alignment/CocoaUtilities/interface/CocoaGlobals.h"

class MeasurementDiffEntry : public Measurement
{ 
public:
  MeasurementDiffEntry( const ALIint measdim, std::vector<ALIstring>& wl ) : Measurement( measdim, wl ){ };
  MeasurementDiffEntry(){ };   
  ~MeasurementDiffEntry(){ };

  // separate OptO names and Entry names 
  virtual void buildOptONamesList( const std::vector<ALIstring>& wl );
    
  // Get simulated value (called every time a parameter is displaced)
  virtual void calculateSimulatedValue( ALIbool firstTime );

 private:
  ALIstring theEntryNameFirst;
  ALIstring theEntryNameSecond;
};

#endif

