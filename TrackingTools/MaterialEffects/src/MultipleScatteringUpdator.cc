#include "TrackingTools/MaterialEffects/interface/MultipleScatteringUpdator.h"
#include "DataFormats/GeometrySurface/interface/MediumProperties.h"

//#define DBG_MSU

//
// Computation of contribution of multiple scatterning to covariance matrix 
//   of local parameters based on Highland formula for sigma(alpha) in plane.
//
void MultipleScatteringUpdator::compute (const TrajectoryStateOnSurface& TSoS, 
					 const PropagationDirection propDir) const
{
  //
  // Get surface
  //
  const Surface& surface = TSoS.surface();
  //
  // Initialise the update to the covariance matrix
  // (dP is constantly 0).
  //
  theDeltaCov(1,1) = 0.;
  theDeltaCov(1,2) = 0.;
  theDeltaCov(2,2) = 0.;
  //
  // Now get information on medium
  //
  if (surface.mediumProperties()) {
    // Momentum vector
    LocalVector d = TSoS.localMomentum();
    double p = d.mag();
    d *= 1./p;
    // MediumProperties mp(0.02, .5e-4);
    const MediumProperties& mp = *surface.mediumProperties();
    double xf = 1./fabs(d.z());         // increase of path due to angle of incidence
    // calculate general physics things
    const double amscon = 1.8496e-4;    // (13.6MeV)**2
    const double m = mass();            // use mass hypothesis from constructor
    double e     = sqrt(p*p + m*m);
    double beta  = p/e;
    // calculate the multiple scattering angle
    double radLen = mp.radLen()*xf;     // effective rad. length
    double sigt2 = 0.;                  // sigma(alpha)**2
    if (radLen > 0) {
      // Calculated rms scattering angle squared.
      double a = (1. + 0.038*log(radLen))/(beta*p); a *= a;
      sigt2 = amscon*radLen*a;
      if (thePtMin > 0) {
#ifdef DBG_MSU
        std::cout<<"Original rms scattering = "<<sqrt(sigt2);
#endif
        // Inflate estimated rms scattering angle, to take into account 
        // that 1/p is not known precisely.
        AlgebraicSymMatrix55 covMatrix = TSoS.localError().matrix();
        double error2_QoverP = covMatrix(0,0);
	// Formula valid for ultra-relativistic particles.
//      sigt2 *= (1. + (p*p) * error2_QoverP);
	// Exact formula
        sigt2 *= (1. + (p*p) * error2_QoverP *
		               (1. + 5*m*m/(e*e) + 3*m*m*beta*beta*error2_QoverP));
#ifdef DBG_MSU
	std::cout<<" new = "<<sqrt(sigt2);
#endif
	// Convert Pt constraint to P constraint, neglecting uncertainty in 
	// track angle.
	double pMin = thePtMin*(TSoS.globalMomentum().mag()/TSoS.globalMomentum().perp());       
        // Use P constraint to calculate rms maximum scattering angle.
        double betaMin = pMin/sqrt(pMin * pMin + m*m);
        double a_max = (1. + 0.038*log(radLen))/(betaMin * pMin); a_max *= a_max;
        double sigt2_max = amscon*radLen*a_max;
        if (sigt2 > sigt2_max) sigt2 = sigt2_max;
#ifdef DBG_MSU
	std::cout<<" after P constraint ("<<pMin<<") = "<<sqrt(sigt2);
	std::cout<<" for track with 1/p="<<1/p<<"+-"<<sqrt(error2_QoverP)<<std::endl;
#endif
      }
    }
    double sl = d.perp();
    double cl = d.z();
    double cf = d.x()/sl;
    double sf = d.y()/sl;
    // Create update (transformation of independant variations
    //   on angle in orthogonal planes to local parameters.
    theDeltaCov(1,1) = sigt2*(sf*sf*cl*cl + cf*cf)/(cl*cl*cl*cl);
    theDeltaCov(1,2) = sigt2*(cf*sf*sl*sl        )/(cl*cl*cl*cl);
    theDeltaCov(2,2) = sigt2*(cf*cf*cl*cl + sf*sf)/(cl*cl*cl*cl);
  }
  //
  // Save arguments to avoid duplication of computation
  //
  storeArguments(TSoS,propDir);
}
//
// Compare arguments with the ones of the previous call
//
bool MultipleScatteringUpdator::newArguments (const TrajectoryStateOnSurface& TSoS, 
					      const PropagationDirection propDir) const {
  LocalVector localP = TSoS.localMomentum(); // let's call localMomentum only once
  return 
    localP.mag() != theLastP || 
    //TSoS.localMomentum().unit().z()!=theLastDz ||   // if we get there,  TSoS.localMomentum().mag() = theLastP!
    localP.z() != theLastDz*theLastP   ||   // so we can just do this, I think
    propDir!=theLastPropDir ||
    TSoS.surface().mediumProperties()->radLen()!=theLastRadLength;
}
//
// Save arguments
//
void MultipleScatteringUpdator::storeArguments (const TrajectoryStateOnSurface& TSoS, 
						const PropagationDirection propDir) const {
  LocalVector localP = TSoS.localMomentum(); // let's call localMomentum only once
  theLastP = localP.mag();
  theLastDz = (theLastP == 0 ? 0 : localP.z()/theLastP);
  theLastPropDir = propDir;
  theLastRadLength = TSoS.surface().mediumProperties()->radLen();
}

