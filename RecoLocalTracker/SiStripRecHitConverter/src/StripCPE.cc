#include "RecoLocalTracker/SiStripRecHitConverter/interface/StripCPE.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include <algorithm>
#include<cmath>

StripCPE::StripCPE( edm::ParameterSet & conf, 
		    const MagneticField * mag, 
		    const TrackerGeometry* geom, 
		    const SiStripLorentzAngle* LorentzAngle)
  : geom_(geom),
    magfield_(mag),
    LorentzAngleMap_(LorentzAngle)
{}

StripClusterParameterEstimator::LocalValues StripCPE::
localParameters( const SiStripCluster & cluster) const 
{
  StripCPE::Param const & p = param(DetId(cluster.geographicalId()));
  float position = cluster.barycenter();
  position -=  0.5 * p.drift.x() / p.topology->localPitch(p.topology->localPosition(position));

  return std::make_pair( p.topology->localPosition(position),
			 p.topology->localError(position, 1/12.) );
}

StripCPE::Param const & StripCPE::
param(DetId detId) const 
{
  Param & p = const_cast<StripCPE*>(this)->m_Params[detId.rawId()];
  if (p.topology) return p;
  else return const_cast<StripCPE*>(this)->fillParam(p, geom_->idToDetUnit(detId));
}

StripCPE::Param & StripCPE::
fillParam(StripCPE::Param & p, const GeomDetUnit *  det) 
{  
  const StripGeomDetUnit * stripdet=(const StripGeomDetUnit*)(det);
  const Bounds& bounds = stripdet->specificSurface().bounds();

  p.maxLength = std::sqrt( std::pow(bounds.length(),2)+std::pow(bounds.width(),2) );
  p.thickness = bounds.thickness();
  p.drift = driftDirection(stripdet) * p.thickness;
  p.topology=(StripTopology*)(&stripdet->topology());    
  p.nstrips = p.topology->nstrips(); 

  return p;
}

LocalVector StripCPE::
driftDirection(const StripGeomDetUnit* det) const
{ 
  LocalVector lbfield = (det->surface()).toLocal(magfield_->inTesla(det->surface().position()));  

  float tanLorentzAnglePerTesla = LorentzAngleMap_->getLorentzAngle(det->geographicalId().rawId());
    
  float dir_x = -tanLorentzAnglePerTesla * lbfield.y();
  float dir_y =  tanLorentzAnglePerTesla * lbfield.x();
  float dir_z =  1.; // E field always in z direction
  
  return LocalVector(dir_x,dir_y,dir_z);
}
