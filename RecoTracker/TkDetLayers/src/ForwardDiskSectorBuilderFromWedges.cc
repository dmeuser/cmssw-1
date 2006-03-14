#include "RecoTracker/TkDetLayers/interface/ForwardDiskSectorBuilderFromWedges.h"
#include "TrackingTools/DetLayers/interface/PhiLess.h"

// Warning, remember to assign this pointer to a ReferenceCountingPointer!
BoundDiskSector* 
ForwardDiskSectorBuilderFromWedges::operator()( const vector<const TECWedge*>& wedges) const
{
  // check first that all wedges are about at the same phi and z !
  float phiStart = wedges.front()->position().phi();
  float zStart = wedges.front()->position().z();
  float wphimin, wphimax;
  for (vector<const TECWedge*>::const_iterator i = wedges.begin(); i != wedges.end(); i++){
    float zdiff = (**i).surface().position().z() - zStart;
    if ( fabs( zdiff) > 5.) cout << " ForwardDiskSectorBuilderFromWedges: Trying to build " 
				 << "Petal from Wedges at different z ! Delta Z = " << zdiff << endl;
    float wphi = (**i).surface().position().phi();
    if ( PhiLess()( phiStart, wphi)) {
      wphimin = phiStart;
      wphimax = wphi;
    } else {
      wphimin = wphi;
      wphimax = phiStart;
    }
    float phidiff = wphimax - wphimin;
    if ( phidiff < 0.) phidiff += 2.*Geom::pi();
    if ( phidiff > 0.3 ) cout << " ForwardDiskSectorBuilderFromWedges: Trying to build " 
			      << "Petal from Wedges at different phi ! Delta phi = " 
			      << phidiff << endl;
  }
  
  pair<DiskSectorBounds,GlobalVector> bo = 
    computeBounds( wedges );

  Surface::PositionType pos( bo.second.x(), bo.second.y(), bo.second.z() );
  Surface::RotationType rot = computeRotation( wedges, pos);
  return new BoundDiskSector( pos, rot, bo.first);
}

pair<DiskSectorBounds, GlobalVector>
ForwardDiskSectorBuilderFromWedges::computeBounds( const vector<const TECWedge*>& wedges) const
{

  // compute maximum and minimum radius and phi 
  float rmin((**(wedges.begin())).specificSurface().innerRadius());
  float rmax(rmin);
  float zmin((**(wedges.begin())).surface().position().z());
  float zmax(zmin);
  float phimin((**(wedges.begin())).surface().position().phi());
  float phimax(phimin);

  for (vector<const TECWedge*>::const_iterator iw=wedges.begin();
       iw != wedges.end(); iw++) {
    // cout << "---------------------------------------------" << endl;
    // cout <<   " Builder: Position of wedge     :" << (**iw).position() << endl; 
    float ri = (**iw).specificSurface().innerRadius();
    float ro = (**iw).specificSurface().outerRadius();
    float zmi = (**iw).surface().position().z() - (**iw).specificSurface().bounds().thickness()/2.;
    float zma = (**iw).surface().position().z() + (**iw).specificSurface().bounds().thickness()/2.;
    float phi1 = (**iw).surface().position().phi() - (**iw).specificSurface().phiExtension()/2.;
    float phi2 = (**iw).surface().position().phi() + (**iw).specificSurface().phiExtension()/2.;
    rmin = min( rmin, ri);
    rmax = max( rmax, ro);
    zmin = min( zmin, zmi);
    zmax = max( zmax, zma);
    if ( PhiLess()( phi1, phimin)) phimin = phi1;
    if ( PhiLess()( phimax, phi2)) phimax = phi2;
  }

  if (!PhiLess()(phimin, phimax)) cout << " ForwardDiskSectorBuilderFromWedges : " 
				       << "Something went wrong with Phi Sorting !" << endl;
  float zPos = (zmax+zmin)/2.;
  float phiWin = phimax - phimin;
  float phiPos = (phimax+phimin)/2.;
  float rmed = (rmin+rmax)/2.;
  if ( phiWin < 0. ) {
    if ( (phimin < Geom::pi() / 2.) || (phimax > -Geom::pi()/2.) ){
      cout << " Debug: something strange going on, please check " << endl;
    }
    // cout << " Petal at pi: phi " << phimin << " " << phimax << " " << phiWin 
    //	 << " " << 2.*Geom::pi()+phiWin << " " << endl;
    phiWin += 2.*Geom::pi();
    phiPos += Geom::pi(); 
  }
  
  GlobalVector pos( rmed*cos(phiPos), rmed*sin(phiPos), zPos);
  return make_pair(DiskSectorBounds(rmin,rmax,zmin-zPos,zmax-zPos,phiWin), pos);
}

Surface::RotationType 
ForwardDiskSectorBuilderFromWedges::computeRotation( const vector<const TECWedge*>& wedges,
						     Surface::PositionType pos) const {
  
  GlobalVector yAxis = ( GlobalVector( pos.x(), pos.y(), 0.)).unit();

  GlobalVector zAxis( 0., 0., 1.);
  GlobalVector xAxis = yAxis.cross( zAxis);

  return Surface::RotationType( xAxis, yAxis);
}

