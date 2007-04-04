//FAMOS Headers
#include "FastSimulation/MaterialEffects/interface/BremsstrahlungSimulator.h"

//CLHEP Headers
#include "CLHEP/Geometry/Vector3D.h"
#include "CLHEP/Geometry/Transform3D.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/Units/PhysicalConstants.h"

#include <iostream>
#include <cmath>

BremsstrahlungSimulator::BremsstrahlungSimulator(double photonEnergyCut, 
					     double photonFractECut,
					     const RandomEngine* engine) : 
  MaterialEffectsSimulator(engine) 
{

  // Set the minimal photon energy for a Brem from e+/-
  photonEnergy = photonEnergyCut;
  photonFractE = photonFractECut;

}


void BremsstrahlungSimulator::compute(ParticlePropagator &Particle)
{

  // Protection : Just stop the electron if more that 1 radiation lengths.
  // This case corresponds to an electron entering the layer parallel to 
  // the layer axis - no reliable simulation can be done in that case...
  if ( radLengths > 1. ) {
    Particle.setPx(0.);
    Particle.setPy(0.);
    Particle.setPz(0.);
    Particle.setE (0.);
  }

  // Hard brem probability with a photon Energy above photonEnergy.
  xmin = std::max(photonEnergy/Particle.e(),photonFractE);
  if ( xmin >=1. || xmin <=0. ) return;

  double bremProba = radLengths * ( 4./3. * log(1./xmin)
				  - 4./3. * (1.-xmin)
				  + 1./2. * (1.-xmin*xmin) );

  
  // Number of photons to be radiated.
  unsigned int nPhotons = poisson(bremProba);

  // Energy of these photons
  for ( unsigned int i=0; i<nPhotons; ++i ) {

    // Check that there is enough energy left.
    if ( Particle.e() < photonEnergy ) break;

    // Create a New Photon
    HepLorentzVector PartP = brem(Particle);
    
    //Rotate to the lab frame
    double chi = Particle.theta();
    double psi = Particle.phi();
    
    PartP = PartP.rotateY(chi);
    PartP = PartP.rotateZ(psi);
    
	
    // Put the brem photon in the list
    RawParticle *thePhoton = new RawParticle(22,PartP);
    _theUpdatedState.push_back(thePhoton);
	
    // Update the original e+/-
    Particle.setPx(Particle.px()-PartP.x());
    Particle.setPy(Particle.py()-PartP.y());
    Particle.setPz(Particle.pz()-PartP.z());
    Particle.setE (Particle.e() -PartP.e());
      
  }	
}

HepLorentzVector BremsstrahlungSimulator::brem(HepLorentzVector pp) {

  // This is a simple version (a la PDG) of a Brem generator.
  // It replaces the buggy GEANT3 -> C++ former version.
  // Author : Patrick Janot - 25-Dec-2003
  double emass = 0.0005109990615;
  double xp=0;
  double weight = 0.;
  
  do {
    xp = xmin * exp ( -log(xmin) * random->flatShoot() );
    weight = 1. - xp + 3./4.*xp*xp;
  } while ( weight < random->flatShoot() );
  
  
  // Have photon energy. Now generate angles with respect to the z axis 
  // defined by the incoming particle's momentum.

  // Isotropic in phi
  const double phi = random->flatShoot()*2*M_PI;
  // theta from universal distribution
  const double theta = gbteth(pp.e(),emass,xp)*emass/pp.e(); 
  
  // Make momentum components
  double stheta = std::sin(theta);
  double ctheta = std::cos(theta);
  double sphi   = std::sin(phi);
  double cphi   = std::cos(phi);
  
  return xp * pp.e() * HepLorentzVector(stheta*cphi,stheta*sphi,ctheta,1.);
  
}

double BremsstrahlungSimulator::gbteth(const double ener,
					  const double partm,
 					  const double efrac) const {
  const double alfa = 0.625;

  const double d = 0.13*(0.8+1.3/theZ())*(100.0+(1.0/ener))*(1.0+efrac);
  const double w1 = 9.0/(9.0+d);
  const double umax = ener*M_PI/partm;
  double u;
  
  do {
    double beta = (random->flatShoot()<=w1) ? alfa : 3.0*alfa;
    u = -log(random->flatShoot()*random->flatShoot())/beta;
  } while (u>=umax);

  return u;
}


unsigned int 
BremsstrahlungSimulator::poisson(double ymu) {

  unsigned int n = 0;
  double prob = std::exp(-ymu);
  double proba = prob;
  double x = random->flatShoot();
  
  while ( proba <= x ) {
    prob *= ymu / double(++n);
    proba += prob;
  }
  
  return n;                                                        
  
}

