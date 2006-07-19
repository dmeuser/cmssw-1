#include "TrackingTools/TrajectoryState/interface/TrajectoryStateClosestToPoint.h"

// Private constructor

TrajectoryStateClosestToPoint::
TrajectoryStateClosestToPoint(const FTS& originalFTS, const GlobalPoint& referencePoint) :
  theFTS(originalFTS), theFTSavailable(true), theRefPoint(referencePoint)
{
  theParameters = perigeeConversions.ftsToPerigeeParameters(originalFTS, referencePoint);
  thePt = fabs( originalFTS.signedInverseMomentum() );
  if (theFTS.hasError()) {
    thePerigeeError = perigeeConversions.ftsToPerigeeError(originalFTS);
    errorIsAvailable = true;
  } 
  else {
    errorIsAvailable = false;
  }
  theField = &(originalFTS.parameters().magneticField());
}


TrajectoryStateClosestToPoint::
TrajectoryStateClosestToPoint(const ParameterVector & perigeePar, double pt, 
			      const GlobalPoint& referencePoint, const MagneticField* field) :
  theField(field), theFTSavailable(false), theRefPoint(referencePoint),
  theParameters(perigeePar), thePt( pt ), errorIsAvailable(false)
{
//   theParameters = perigeeConversions.helixToPerigeeParameters(helixPar, referencePoint);
}


TrajectoryStateClosestToPoint::
TrajectoryStateClosestToPoint(const ParameterVector & perigeePar, double pt,
	const CovarianceMatrix & perigeeCov, const GlobalPoint& referencePoint,
	const MagneticField* field) :
  theField(field), theFTSavailable(false), theRefPoint(referencePoint), theParameters(perigeePar),
  thePt( pt ), thePerigeeError(perigeeCov), errorIsAvailable(true)
{
//   theParameters = perigeeConversions.helixToPerigeeParameters(helixPar, referencePoint);
//   thePerigeeError = perigeeConversions.helixToPerigeeError(helixPar, helixCov);
}

  /**
   * Public constructor, which is used to convert perigee 
   * parameters to a FreeTrajectoryState. For the case where
   * no error is provided.
   */

TrajectoryStateClosestToPoint::
TrajectoryStateClosestToPoint(const PerigeeTrajectoryParameters& perigeeParameters, double pt,
  const GlobalPoint& referencePoint, const MagneticField* field) :
    theField(field), theFTSavailable(false), theRefPoint(referencePoint), 
    theParameters(perigeeParameters), thePt( pt ), errorIsAvailable(false)
{}

  /**
   * Public constructor, which is used to convert perigee 
   * parameters to a FreeTrajectoryState. For the case where
   * an error is provided.
   */

TrajectoryStateClosestToPoint::
TrajectoryStateClosestToPoint(const PerigeeTrajectoryParameters& perigeeParameters, double pt,
  const PerigeeTrajectoryError& perigeeError, const GlobalPoint& referencePoint,
  const MagneticField* field):
    theField(field), theFTSavailable(false), theRefPoint(referencePoint), theParameters(perigeeParameters),
    thePt( pt ), thePerigeeError(perigeeError), errorIsAvailable(true)
    
{}


void TrajectoryStateClosestToPoint::calculateFTS() const
{
  GlobalTrajectoryParameters gtp(
	    perigeeConversions.positionFromPerigee(theParameters, theRefPoint),
	    perigeeConversions.momentumFromPerigee(theParameters, thePt, theRefPoint),
	    theParameters.charge(), theField);
  if (errorIsAvailable) {
    theFTS = FTS(gtp, perigeeConversions.curvilinearError(thePerigeeError, gtp));
  } else {
    theFTS = FTS(gtp);
  }
  theFTSavailable = true;
}
