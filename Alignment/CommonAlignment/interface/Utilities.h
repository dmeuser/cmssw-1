#ifndef Alignment_CommonAlignment_Utilities_H
#define Alignment_CommonAlignment_Utilities_H

/** \namespace align
 *
 *  Namespace for common type definitions and calculations in alignment.
 *
 *  $Date: 2007/02/28 01:53:40 $
 *  $Revision: 1.1 $
 *  \author Chung Khim Lae
 */

#include <vector>

#include "DataFormats/CLHEP/interface/AlgebraicObjects.h"
#include "DataFormats/GeometrySurface/interface/TkRotation.h"
#include "DataFormats/GeometryVector/interface/GlobalTag.h"
#include "DataFormats/GeometryVector/interface/LocalTag.h"
#include "DataFormats/GeometryVector/interface/Point3DBase.h"
#include "DataFormats/GeometryVector/interface/Vector3DBase.h"

namespace align
{
  typedef double Scalar;

  typedef   TkRotation<Scalar>            RotationType;
  typedef  Point3DBase<Scalar, GlobalTag> PositionType;
  typedef  Point3DBase<Scalar, GlobalTag> GlobalPoint;
  typedef  Point3DBase<Scalar,  LocalTag> LocalPoint;
  typedef Vector3DBase<Scalar, GlobalTag> GlobalVector;
  typedef Vector3DBase<Scalar,  LocalTag> LocalVector;

  typedef CLHEP::HepVector    EulerAngles;
  typedef CLHEP::HepSymMatrix ErrorMatrix;

  typedef std::vector<GlobalPoint>  GlobalPoints;
  typedef std::vector<GlobalVector> GlobalVectors;

  /// Convert rotation matrix to angles about x-, y-, z-axes.
  EulerAngles toAngles(
		       const RotationType&
		       );

  /// Convert rotation angles about x-, y-, z-axes to matrix.
  RotationType toMatrix(
			const EulerAngles&
			);

  /// Find matrix to rotate from nominal to current vectors.
  /// Assume both sets of vectors have the same size and order.
  RotationType diffRot(
		       const GlobalVectors& current,
		       const GlobalVectors& nominal
		       );

  /// Correct a rotation matrix for rounding errors.
  void rectify(
	       RotationType&
	       );
}

#endif
