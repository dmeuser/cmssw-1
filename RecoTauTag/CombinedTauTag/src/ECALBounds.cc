#include "RecoTauTag/CombinedTauTag/interface/ECALBounds.h"
#include "Geometry/Surface/interface/BoundCylinder.h"
#include "Geometry/Surface/interface/BoundDisk.h"
#include "Geometry/Surface/interface/SimpleCylinderBounds.h"
#include "Geometry/Surface/interface/SimpleDiskBounds.h"

void ECALBounds::initialize(){
  const float epsilon = 0.001; // should not matter at all
  Surface::RotationType rot; // unit rotation matrix
  theCylinder=new BoundCylinder(Surface::PositionType(0,0,0),rot, 
				   SimpleCylinderBounds(barrel_innerradius()-epsilon, 
							 barrel_innerradius()+epsilon, 
							 -barrel_halfLength(), 
							 barrel_halfLength()));


  theNegativeDisk=new BoundDisk(Surface::PositionType(0,0,-endcap_innerZ()),rot, 
		   SimpleDiskBounds(0,endcap_outerradius(),-epsilon,epsilon));

  thePositiveDisk = 
    new BoundDisk( Surface::PositionType(0,0,endcap_innerZ()),rot, 
		   SimpleDiskBounds(0,endcap_outerradius(),-epsilon,epsilon));


  theInit = true;
}

// static initializers
ReferenceCountingPointer<Surface> ECALBounds::theCylinder = 0;
ReferenceCountingPointer<Surface> ECALBounds::theNegativeDisk = 0;
ReferenceCountingPointer<Surface> ECALBounds::thePositiveDisk = 0;
bool ECALBounds::theInit = false;

