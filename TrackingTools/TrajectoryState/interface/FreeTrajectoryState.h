#ifndef _TRACKER_FREETRAJECTORYSTATE_H_
#define _TRACKER_FREETRAJECTORYSTATE_H_

// base trajectory state class
// track parameters and error covariance matrix

#include "TrackingTools/TrajectoryParametrization/interface/GlobalTrajectoryParameters.h"
#include "TrackingTools/TrajectoryParametrization/interface/CartesianTrajectoryError.h"
#include "TrackingTools/TrajectoryParametrization/interface/CurvilinearTrajectoryError.h"
#include "DataFormats/TrajectoryState/interface/TrackCharge.h"
#include "TrackingTools/TrajectoryParametrization/interface/TrajectoryStateExceptions.h"

#include <iosfwd>

#include "DataFormats/GeometrySurface/interface/BlockWipedAllocator.h"


#include "FWCore/Utilities/interface/Visibility.h"
#include "FWCore/Utilities/interface/Likely.h"


/** A 6-dimensional state vector of a helix given at some point in 
 *  space along the helix, and the associated error matrix.
 *  The error can be obtained in two different parametrizations:
 *  CurvilinearTrajectoryError and CartesianTrajectoryError
 *  (see descriptions of corresponding classes).
 *  The FreeTrajectoryState can be instantiated with either of these
 *  error parametrisations; it converts from one to the other internally.
 */

class FreeTrajectoryState : public BlockWipedAllocated<FreeTrajectoryState> {
public:
// construst
//default constructor - needed for Persistency

  FreeTrajectoryState():
    theCartesianErrorValid(false),
    theCurvilinearErrorValid(false) {};

  FreeTrajectoryState(const GlobalTrajectoryParameters& aGlobalParameters) :
    theGlobalParameters(aGlobalParameters),
    theCartesianErrorValid(false),
    theCurvilinearErrorValid(false)
  {
  }

  FreeTrajectoryState(const GlobalPoint& aX,
                      const GlobalVector& aP,
                      TrackCharge aCharge, 
                      const MagneticField* fieldProvider) :
    theGlobalParameters(aX, aP, aCharge, fieldProvider),
    theCartesianErrorValid(false),
    theCurvilinearErrorValid(false)
  {
  }


  FreeTrajectoryState(const GlobalTrajectoryParameters& aGlobalParameters,
                      const CurvilinearTrajectoryError& aCurvilinearError) :
    theGlobalParameters(aGlobalParameters),
    theCurvilinearError(aCurvilinearError),
    theCartesianErrorValid(false),
    theCurvilinearErrorValid(true)
  {
  }


#ifdef ENABLE_CACHE_CARTESIAN
  FreeTrajectoryState(const GlobalTrajectoryParameters& aGlobalParameters,
                      const CartesianTrajectoryError& aCartesianError) :
    theGlobalParameters(aGlobalParameters),
    theCartesianError(aCartesianError),
    theCartesianErrorValid(true),
    theCurvilinearErrorValid(false)
  {  }
#else 
  FreeTrajectoryState(const GlobalTrajectoryParameters& aGlobalParameters,
                      const CartesianTrajectoryError& aCartesianError) :
    theGlobalParameters(aGlobalParameters),
    theCartesianErrorValid(false),
    theCurvilinearErrorValid(false)
  { createCurvilinearError(aCartesianError);  }
#endif


#ifdef ENABLE_CACHE_CARTESIAN
  FreeTrajectoryState(const GlobalTrajectoryParameters& aGlobalParameters,
                      const CartesianTrajectoryError& aCartesianError,
                      const CurvilinearTrajectoryError& aCurvilinearError) :
    theGlobalParameters(aGlobalParameters),
    theCartesianError(aCartesianError),
    theCurvilinearError(aCurvilinearError),
    theCartesianErrorValid(true),
    theCurvilinearErrorValid(true)
  {
  }
#else
  FreeTrajectoryState(const GlobalTrajectoryParameters& aGlobalParameters,
                      const CartesianTrajectoryError&,
                      const CurvilinearTrajectoryError& aCurvilinearError) :
    theGlobalParameters(aGlobalParameters),
    theCurvilinearError(aCurvilinearError),
    theCartesianErrorValid(false),
    theCurvilinearErrorValid(true)
  {
  }
#endif

// access
// propagate access to parameters
  GlobalPoint position() const {
    return theGlobalParameters.position();
  }
  GlobalVector momentum() const {
    return theGlobalParameters.momentum();
  }
  TrackCharge charge() const {
    return theGlobalParameters.charge();
  }
  double signedInverseMomentum() const {
    return theGlobalParameters.signedInverseMomentum();
  }
  double transverseCurvature() const {
    return theGlobalParameters.transverseCurvature();
  }

// direct access
  bool hasCartesianError() const {return theCartesianErrorValid;}

  bool hasCurvilinearError() const {return theCurvilinearErrorValid;}

  bool hasError() const {
    return theCurvilinearErrorValid || theCartesianErrorValid;
  }


  const GlobalTrajectoryParameters& parameters() const {
    return theGlobalParameters;
  }

#ifdef ENABLE_CACHE_CARTESIAN
  const CartesianTrajectoryError& cartesianError() const {
    if unlikely(!hasError()) missingError();
    if  unlikely(!theCartesianErrorValid)
      createCartesianError(theCartesianError);
    return theCartesianError;
  }

  const CurvilinearTrajectoryError& curvilinearError() const {
    if  unlikely(!hasError()) missingError();
    if  unlikely(!theCurvilinearErrorValid)
      createCurvilinearError();
    return theCurvilinearError;
  }
#else
  CartesianTrajectoryError cartesianError() const {
    if unlikely(!hasError()) missingError();
    CartesianTrajectoryError aCartesianError;
    createCartesianError(aCartesianError);
    return aCartesianError;
  }

  const CurvilinearTrajectoryError& curvilinearError() const {
    if  unlikely(!hasError()) missingError();
    return theCurvilinearError;
  }
#endif



  void rescaleError(double factor);

#ifdef ENABLE_CACHE_CARTESIAN
  void setCartesianError(const CartesianTrajectoryError &err) {
        theCartesianError = err; theCartesianErrorValid = true;
  }
  void setCartesianError(const AlgebraicSymMatrix66 &err) {
        theCartesianError = CartesianTrajectoryError(err); theCartesianErrorValid = true;
  }
#else
  void setCartesianError(const CartesianTrajectoryError &err) {
    createCurvilinearError(err);
  }
  void setCartesianError(const AlgebraicSymMatrix66 &err) {
    createCurvilinearError(CartesianTrajectoryError(err)); 
  }
#endif

  void setCurvilinearError(const CurvilinearTrajectoryError &err) {
        theCurvilinearError = err; theCurvilinearErrorValid = true;
  }
  void setCurvilinearError(const AlgebraicSymMatrix55 &err) {
        theCurvilinearError = CurvilinearTrajectoryError(err); theCurvilinearErrorValid = true;
  }


// properties
  bool canReach(double radius) const;
private:


  static void missingError(); // dso_internal;

// convert curvilinear errors to cartesian
  void createCartesianError(CartesianTrajectoryError & aCartesianError) const; // dso_internal;


// convert cartesian errors to curvilinear
  void createCurvilinearError(CartesianTrajectoryError const & aCartesianError) const; // dso_internal;

private:

  GlobalTrajectoryParameters  theGlobalParameters;
#ifdef ENABLE_CACHE_CARTESIAN
  mutable CartesianTrajectoryError    theCartesianError;
#endif
  mutable CurvilinearTrajectoryError  theCurvilinearError;
  mutable bool                        theCartesianErrorValid;
  mutable bool                        theCurvilinearErrorValid;

};

std::ostream& operator<<(std::ostream& os, const FreeTrajectoryState& fts);

#endif
