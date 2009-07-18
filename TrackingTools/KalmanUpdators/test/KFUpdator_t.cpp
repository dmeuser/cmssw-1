#include "TrackingTools/KalmanUpdators/interface/KFUpdator.h"
#include "TrackingTools/KalmanUpdators/interface/Chi2MeasurementEstimator.h"

#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "DataFormats/GeometrySurface/interface/Surface.h" 
#include "DataFormats/GeometrySurface/interface/BoundPlane.h"
#include <Geometry/CommonDetUnit/interface/GeomDet.h>

#include "MagneticField/Engine/interface/MagneticField.h"

#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h"


#include <iostream>

class ConstMagneticField : public MagneticField {
public:

  virtual GlobalVector inTesla ( const GlobalPoint& ) const {
    return GlobalVector(0,0,4);
  }

};


#include "TrackingTools/AnalyticalJacobians/interface/JacobianLocalToCartesian.h"
#include "TrackingTools/AnalyticalJacobians/interface/JacobianCartesianToLocal.h"
#include "TrackingTools/AnalyticalJacobians/interface/JacobianLocalToCurvilinear.h"


typedef ROOT::Math::SMatrix<double,5,5,ROOT::Math::MatRepSym<double,5> > Matrix5;
typedef ROOT::Math::SMatrix<double,6,6,ROOT::Math::MatRepSym<double,6> > Matrix6;

Matrix5 buildCovariance(float y) {

  // build a resonable covariance matrix as JIJ

  Basic3DVector<float>  axis(0.5,1.,1);
  
  Surface::RotationType rot(axis,0.5*M_PI);

  Surface::PositionType pos( 0., 0., 0.);

  Plane plane(pos,rot);
  LocalTrajectoryParameters tp(1., 1., y, 0.,0.,1.);

  JacobianLocalToCartesian jl2c(plane,tp);
  return ROOT::Math::SimilarityT(jl2c.jacobian(),Matrix6(ROOT::Math::SMatrixIdentity()));
  // return  ROOT::Math::Transpose(jl2c.jacobian())* jl2c.jacobian();

}

// A fake Det class

class MyDet : public GeomDet {
 public:
  MyDet(BoundPlane * bp) :
    GeomDet(bp){}
  
  virtual DetId geographicalId() const {return DetId();}
  virtual std::vector< const GeomDet*> components() const {
    return std::vector< const GeomDet*>();
  }
  
  /// Which subdetector
  virtual SubDetector subDetector() const {return GeomDetEnumerators::DT;}
  
};  


// a 2d hit

#include "TrackingTools/TransientTrackingRecHit/interface/GenericTransientTrackingRecHit.h"
#include "TrackingTools/TransientTrackingRecHit/interface/HelpertRecHit2DLocalPos.h"


class My2DHit : public GenericTransientTrackingRecHit {
public:

  My2DHit(const GeomDet * geom, TrackingRecHit* rh) :
    GenericTransientTrackingRecHit(geom,  rh) {}

  virtual void getKfComponents( KfComponentsHolder & holder ) const {
      HelpertRecHit2DLocalPos().getKfComponents(holder, *hit(), *det()); 
  }
};


void print(TrajectoryStateOnSurface const & ts) {
  using namespace std;

  cout << "transverseCurvature  " << ts.transverseCurvature() << endl;
  cout << "globalMomentum       " << ts.globalMomentum() << endl;
  cout << "localMomentum        " << ts.localMomentum() << endl;
  cout << "localPosition        " << ts.localPosition() << endl;
  cout << "localError           " << ts.localError().matrix() << endl;
  cout << endl;
}


#include "FWCore/Utilities/interface/HRRealTime.h"
#include<iostream>
#include<vector>

bool isAligned(const void* data, long alignment)
{
 // check that the alignment is a power of two
 assert((alignment & (alignment-1)) == 0); 
 return ((long)data & (alignment-1)) == 0;
}


void st(){}
void en(){}


struct KFUTest {
  TrajectoryStateUpdator const & tsu;

  KFUTest(TrajectoryStateUpdator const * itsu) : tsu(*itsu){}


  void print(const TrajectoryStateOnSurface& tsos,
	     const TransientTrackingRecHit& hit) const {
    TrajectoryStateOnSurface tsn =  tsu.update(tsos, hit);
    ::print (tsn);
  }
  

  void time(const TrajectoryStateOnSurface& tsos,
	     const TransientTrackingRecHit& hit) const {
    st();	
    edm::HRTimeType s= edm::hrRealTime();
    TrajectoryStateOnSurface tsn =  tsu.update(tsos, hit);
    edm::HRTimeType e = edm::hrRealTime();
    en();
    std::cout << e-s << std::endl;
  }

};

struct Chi2Test {
  MeasurementEstimator const & chi2;
  mutable std::pair<bool, double> res;
  Chi2Test(MeasurementEstimator const * ichi2) : chi2(*ichi2){}


  void print(const TrajectoryStateOnSurface& tsos,
	     const TransientTrackingRecHit& hit) const {
    res = chi2.estimate(tsos, hit);
    std::cout << "chi2 " << res.second << std::endl;
  }
  

  void time(const TrajectoryStateOnSurface& tsos,
	    const TransientTrackingRecHit& hit) const {
    st();	
    edm::HRTimeType s= edm::hrRealTime();
    res = chi2.estimate(tsos, hit);
    edm::HRTimeType e = edm::hrRealTime();
    en();
    std::cout << e-s << std::endl;
  }

};



int main() {

  MagneticField * field = new ConstMagneticField;
  GlobalPoint gp(0,0,0);
  GlobalVector gv(1,1,1);
  GlobalTrajectoryParameters gtp(gp,gv,1,field);
  CurvilinearTrajectoryError gerr(buildCovariance(1.));
  BoundPlane* plane = new BoundPlane( gp, Surface::RotationType());
  GeomDet *  det =  new MyDet(plane);

  

  TrajectoryStateOnSurface ts(gtp,gerr, *plane);
  print (ts);

  LocalPoint lp(0,0,0);
  LocalVector lv(1,1,1);
  LocalTrajectoryParameters ltp(lp,lv,1);
  LocalTrajectoryError ler(0.1,0.1,0.01,0.05,0.1);
  TrajectoryStateOnSurface ts2(ltp,ler,*plane, field);
  print (ts2);


  LocalPoint m(0.1,0.1,0);
  LocalError e(0.2,-0.05,0.1);
    
  SiStripRecHit2D dummy;
  TrackingRecHit * hit = new SiStripMatchedRecHit2D(m, e, DetId() , &dummy, &dummy);
  TransientTrackingRecHit * thit = new  My2DHit(det, hit);

  KFUTest kt(new KFUpdator());
  Chi2Test chi2(new Chi2MeasurementEstimator(10.));

  std::cout << "\n** KFU ** \n" << std::endl;
  
  kt.print(ts,*thit);
  kt.print(ts2,*thit);

  kt.time(ts,*thit);
  kt.time(ts2,*thit);

  std::cout << "\n** Chi2 ** \n" << std::endl;
  
  chi2.print(ts,*thit);
  chi2.print(ts2,*thit);

  chi2.time(ts,*thit);
  chi2.time(ts2,*thit);



  return 0;

}
