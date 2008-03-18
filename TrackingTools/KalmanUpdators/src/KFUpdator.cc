#include "TrackingTools/KalmanUpdators/interface/KFUpdator.h"
#include "TrackingTools/PatternTools/interface/MeasurementExtractor.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"
#include "DataFormats/GeometrySurface/interface/BoundPlane.h"
#include "DataFormats/TrackingRecHit/interface/KfComponentsHolder.h"

TrajectoryStateOnSurface KFUpdator::update(const TrajectoryStateOnSurface& tsos,
				           const TransientTrackingRecHit& aRecHit) const {
    switch (aRecHit.dimension()) {
        case 1: return update<1>(tsos,aRecHit);
        case 2: return update<2>(tsos,aRecHit);
        case 3: return update<3>(tsos,aRecHit);
        case 4: return update<4>(tsos,aRecHit);
        case 5: return update<5>(tsos,aRecHit);
    }
    throw cms::Exception("Rec hit of invalid dimension (not 1,2,3,4,5)") <<
         "The value was " << aRecHit.dimension() << 
        ", type is " << typeid(aRecHit).name() << 
        ", persistent rechit type " << (aRecHit.hit() ? typeid(*aRecHit.hit()).name() : "NULL") << "\n";
}

#define NEW
#ifdef NEW
template <unsigned int D>
TrajectoryStateOnSurface KFUpdator::update(const TrajectoryStateOnSurface& tsos,
				           const TransientTrackingRecHit& aRecHit) const {

  typedef typename AlgebraicROOTObject<D,5>::Matrix MatD5;
  typedef typename AlgebraicROOTObject<5,D>::Matrix Mat5D;
  typedef typename AlgebraicROOTObject<D,D>::SymMatrix SMatDD;
  typedef typename AlgebraicROOTObject<D>::Vector VecD;
  double pzSign = tsos.localParameters().pzSign();

  //MeasurementExtractor me(tsos);

  AlgebraicVector5 x = tsos.localParameters().vector();
  const AlgebraicSymMatrix55 &C = (tsos.localError().matrix());
  // Measurement matrix
  MatD5 H; 
  VecD r, rMeas; 
  SMatDD V, VMeas;

  KfComponentsHolder holder; 
  holder.template setup<D>(&r, &V, &H, &rMeas, &VMeas, x, C);
  aRecHit.getKfComponents(holder);
  
  //MatD5 H = asSMatrix<D,5>(aRecHit.projectionMatrix());

  // Residuals of aPredictedState w.r.t. aRecHit, 
  //VecD r = asSVector<D>(aRecHit.parameters()) - me.measuredParameters<D>(aRecHit);
  //r -= me.measuredParameters<D>(aRecHit);
  r -= rMeas;

  // and covariance matrix of residuals
  //SMatDD V = asSMatrix<D>(aRecHit.parametersError());
  //SMatDD R = V + me.measuredError<D>(aRecHit);
  SMatDD R = V + VMeas;
  int ierr = ! R.Invert(); // if (ierr != 0) throw exception;
  //R.Invert();

  // Compute Kalman gain matrix
  Mat5D K = C * ROOT::Math::Transpose(H) * R;

  // Compute local filtered state vector
  AlgebraicVector5 fsv = x + K * r;

  // Compute covariance matrix of local filtered state vector
  AlgebraicMatrix55 I = AlgebraicMatrixID();
  AlgebraicMatrix55 M = I - K * H;
  AlgebraicSymMatrix55 fse = ROOT::Math::Similarity(M, C) + ROOT::Math::Similarity(K, V);

  return TrajectoryStateOnSurface( LocalTrajectoryParameters(fsv, pzSign),
				   LocalTrajectoryError(fse), tsos.surface(),&(tsos.globalParameters().magneticField()));
}
#endif

#ifndef NEW
template <unsigned int D>
TrajectoryStateOnSurface KFUpdator::update(const TrajectoryStateOnSurface& tsos,
				           const TransientTrackingRecHit& aRecHit) const {

  typedef typename AlgebraicROOTObject<D,5>::Matrix MatD5;
  typedef typename AlgebraicROOTObject<5,D>::Matrix Mat5D;
  typedef typename AlgebraicROOTObject<D,D>::SymMatrix SMatDD;
  typedef typename AlgebraicROOTObject<D>::Vector VecD;
  double pzSign = tsos.localParameters().pzSign();

  MeasurementExtractor me(tsos);

  AlgebraicVector5 x = tsos.localParameters().vector();
  const AlgebraicSymMatrix55 &C = (tsos.localError().matrix());
  // Measurement matrix
  MatD5 H = asSMatrix<D,5>(aRecHit.projectionMatrix());

  // Residuals of aPredictedState w.r.t. aRecHit, 
  VecD r = asSVector<D>(aRecHit.parameters()) - me.measuredParameters<D>(aRecHit);

  // and covariance matrix of residuals
  SMatDD V = asSMatrix<D>(aRecHit.parametersError());
  SMatDD R = V + me.measuredError<D>(aRecHit);
  int ierr = ! R.Invert(); // if (ierr != 0) throw exception;
  //R.Invert();

  // Compute Kalman gain matrix
  Mat5D K = C * ROOT::Math::Transpose(H) * R;

  // Compute local filtered state vector
  AlgebraicVector5 fsv = x + K * r;

  // Compute covariance matrix of local filtered state vector
  AlgebraicMatrix55 I = AlgebraicMatrixID();
  AlgebraicMatrix55 M = I - K * H;
  AlgebraicSymMatrix55 fse = ROOT::Math::Similarity(M, C) + ROOT::Math::Similarity(K, V);

  return TrajectoryStateOnSurface( LocalTrajectoryParameters(fsv, pzSign),
				   LocalTrajectoryError(fse), tsos.surface(),&(tsos.globalParameters().magneticField()));
}
#endif
