#ifndef CSC_NONSLANTED_WIRE_GEOMETRY_H
#define CSC_NONSLANTED_WIRE_GEOMETRY_H

/** \class CSCNonslantedWireGeometry
 * A concrete CSCWireGeometry in which the wires are not slanted,
 * i.e. they are all parallel to the local x axis.
 *
 * \author Tim Cox
 *
 */

#include "Geometry/CSCGeometry/interface/CSCWireGeometry.h"
#include "Geometry/CSCGeometry/interface/nint.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"

class CSCNonslantedWireGeometry : public CSCWireGeometry {
 public:
  virtual ~CSCNonslantedWireGeometry() {}
  
  /**
   * Constructor from wire spacing
   */
  CSCNonslantedWireGeometry( double wireSpacing, double yOfFirstWire ) :
       CSCWireGeometry( wireSpacing, yOfFirstWire ) {}

  /**
   * The angle of the wires w.r.t local x axis (in radians)
   */
  float wireAngle() const { return 0.; }

  /**
   * The nearest (virtual) wire to a given LocalPoint.
   * Beware that this wire might not exist or be read out!
   */
  int nearestWire(const LocalPoint& lp) const {
    return 1 + nint( (lp.y()-yOfFirstWire())/wireSpacing() ) ;
  }

  /**
   * Local y of a given wire 'number' (float) at given x
   * For nonslanted wires this y is independent of x.
   */
  float yOfWire(float wire, float x=0.) const {
    return yOfFirstWire() + (wire-1.)*wireSpacing();
  }

  /**
   * Clone to handle correct copy of component objects referenced
   * by base class pointer.
   */
  CSCWireGeometry* clone() const {
    return new CSCNonslantedWireGeometry(*this);
  }

};

#endif
