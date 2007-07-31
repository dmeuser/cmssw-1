#include "DataFormats/GeometrySurface/interface/RectangularPlaneBounds.h"
#include "DataFormats/GeometrySurface/interface/LocalError.h" 
#include <cmath>

RectangularPlaneBounds::RectangularPlaneBounds( float w, float h, float t) : 
  halfWidth(w), halfLength(h), halfThickness(t) {}


bool RectangularPlaneBounds::inside( const Local2DPoint& p) const {
  return fabs(p.x()) <= halfWidth && fabs(p.y()) <= halfLength;
}

bool RectangularPlaneBounds::inside( const Local3DPoint& p) const {
  return 
    fabs(p.x()) < halfWidth && 
    fabs(p.y()) < halfLength &&
    fabs(p.z()) < halfThickness;
}

bool RectangularPlaneBounds::inside(const Local3DPoint& p, const LocalError& err,
				    float scale) const {
  return 
    fabs(p.z()) < halfThickness &&
    (fabs(p.x()) < halfWidth  || fabs(p.x()) < halfWidth  + std::sqrt(err.xx())*scale) &&
    (fabs(p.y()) < halfLength || fabs(p.y()) < halfLength + std::sqrt(err.yy())*scale);
}
    
bool RectangularPlaneBounds::inside( const Local2DPoint& p, const LocalError& err, 
				     float scale) const {
  return 
    (fabs(p.x()) < halfWidth  || fabs(p.x()) < halfWidth  + std::sqrt(err.xx())*scale) &&
    (fabs(p.y()) < halfLength || fabs(p.y()) < halfLength + std::sqrt(err.yy())*scale);
}

Bounds* RectangularPlaneBounds::clone() const { 
  return new RectangularPlaneBounds(*this);
}


