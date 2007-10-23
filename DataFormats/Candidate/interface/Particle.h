#ifndef Candidate_Particle_h
#define Candidate_Particle_h
/** \class reco::Particle
 *
 * Base class describing a generic reconstructed particle
 * its main subclass is Candidate
 *
 * \author Luca Lista, INFN
 *
 * \version $Id: Particle.h,v 1.24 2007/10/20 09:34:31 llista Exp $
 *
 */
#include "DataFormats/Math/interface/Point3D.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Common/interface/BoolCache.h"
#include "Rtypes.h"

namespace reco {
  
  class Particle {
  public:
    /// electric charge type
    typedef int Charge;
    /// Lorentz vector
    typedef math::XYZTLorentzVector LorentzVector;
    /// Lorentz vector
    typedef math::PtEtaPhiMLorentzVector PolarLorentzVector;
    /// point in the space
    typedef math::XYZPoint Point;
    /// point in the space
    typedef math::XYZVector Vector;
    /// default constructor
    Particle() : cachePolarFixed_( false ) { }
    /// constructor from values
    Particle( Charge q, const LorentzVector & p4, const Point & vertex = Point( 0, 0, 0 ),
	      int pdgId = 0, int status = 0, bool integerCharge = true ) : 
      qx3_( q ), pt_( p4.pt() ), eta_( p4.eta() ), phi_( p4.phi() ), mass_( p4.mass() ),
      vertex_( vertex ), pdgId_( pdgId ), status_( status ),
      cachePolarFixed_( false ), cacheCartesianFixed_( false ) { 
      if ( integerCharge ) qx3_ *= 3;
    }
    /// constructor from values
    Particle( Charge q, const PolarLorentzVector & p4, const Point & vertex = Point( 0, 0, 0 ),
	      int pdgId = 0, int status = 0, bool integerCharge = true ) : 
      qx3_( q ), pt_( p4.pt() ), eta_( p4.eta() ), phi_( p4.phi() ), mass_( p4.mass() ),
      vertex_( vertex ), pdgId_( pdgId ), status_( status ),
      cachePolarFixed_( false ), cacheCartesianFixed_( false ) { 
      if ( integerCharge ) qx3_ *= 3;
    }
    /// destructor
    virtual ~Particle() { }
    /// electric charge
    int charge() const { return qx3_ / 3; }
    /// set electric charge
    void setCharge( Charge q ) { qx3_ = q * 3; }
    /// electric charge
    int threeCharge() const { return qx3_; }
    /// set electric charge
    void setThreeCharge( Charge qx3 ) { qx3_ = qx3; }
    /// four-momentum Lorentz vector
    const LorentzVector & p4() const { cacheCartesian(); return p4Cartesian_; }
    /// four-momentum Lorentz vector
    const PolarLorentzVector & polarP4() const { cachePolar(); return p4Polar_; }
    /// spatial momentum vector
    Vector momentum() const { cacheCartesian(); return p4Cartesian_.Vect(); }
    /// boost vector to boost a Lorentz vector 
    /// to the particle center of mass system
    Vector boostToCM() const { cacheCartesian(); return p4Cartesian_.BoostToCM(); }
    /// magnitude of momentum vector
    double p() const { cacheCartesian(); return p4Cartesian_.P(); }
    /// energy
    double energy() const { cacheCartesian(); return p4Cartesian_.E(); }  
    /// transverse energy 
    double et() const { cachePolar(); return p4Polar_.Et(); }  
    /// mass
    double mass() const { return mass_; }
    /// mass squared
    double massSqr() const { return mass_ * mass_; }
    /// transverse mass
    double mt() const { cachePolar(); return p4Polar_.Mt(); }
    /// transverse mass squared
    double mtSqr() const { cachePolar(); return p4Polar_.Mt2(); }
    /// x coordinate of momentum vector
    double px() const { cacheCartesian(); return p4Cartesian_.Px(); }
    /// y coordinate of momentum vector
    double py() const { cacheCartesian(); return p4Cartesian_.Py(); }
    /// z coordinate of momentum vector
    double pz() const { cacheCartesian(); return p4Cartesian_.Pz(); }
    /// transverse momentum
    double pt() const { return pt_; }
    /// momentum azimuthal angle
    double phi() const { return phi_; }
    /// momentum polar angle
    double theta() const { cacheCartesian(); return p4Cartesian_.Theta(); }
    /// momentum pseudorapidity
    double eta() const { return eta_; }
    /// repidity
    double rapidity() const { cachePolar(); return p4Polar_.Rapidity(); }
    /// repidity
    double y() const { return rapidity(); }
    /// set 4-momentum
    void setP4( const LorentzVector & p4 ) { 
      p4Cartesian_ = p4;
      p4Polar_ = p4;
      pt_ = p4Polar_.pt();
      eta_ = p4Polar_.eta();
      phi_ = p4Polar_.phi();
      mass_ = p4Polar_.mass();
      cachePolarFixed_ = true;
      cacheCartesianFixed_ = true;      
    }
    /// set 4-momentum
    void setP4( const PolarLorentzVector & p4 ) { 
      p4Polar_ = p4;
      pt_ = p4Polar_.pt();
      eta_ = p4Polar_.eta();
      phi_ = p4Polar_.phi();
      mass_ = p4Polar_.mass();
      cachePolarFixed_ = true;
      cacheCartesianFixed_ = false;            
    }
    /// set particle mass
    void setMass( double m ) { 
      mass_ = m; 
      clearCache(); 
    }
    void setPz( double pz ) {
      cacheCartesian();
      p4Cartesian_.SetPz(pz);
      p4Polar_ = p4Cartesian_;
      pt_ = p4Polar_.pt();
      eta_ = p4Polar_.eta();
      phi_ = p4Polar_.phi();
      mass_ = p4Polar_.mass();
    }
    /// vertex position
    const Point & vertex() const { return vertex_; }
    /// x coordinate of vertex position
    double vx() const { return vertex_.X(); }
    /// y coordinate of vertex position
    double vy() const { return vertex_.Y(); }
    /// z coordinate of vertex position
    double vz() const { return vertex_.Z(); }
    /// set vertex
    void setVertex( const Point & vertex ) { vertex_ = vertex; }
    /// PDG identifier
    int pdgId() const { return pdgId_; }
    // set PDG identifier
    void setPdgId( int pdgId ) { pdgId_ = pdgId; }
    /// status word
    int status() const { return status_; }
    /// set status word
    void setStatus( int status ) { status_ = status; }

  protected:
    /// electric charge
    Charge qx3_;   
    /// four-momentum Lorentz vector
    Double32_t pt_, eta_, phi_, mass_;
    /// vertex position
    Point vertex_;
    /// PDG identifier
    int pdgId_;
    /// status word
    int status_;
    /// internal cache for p4
    mutable PolarLorentzVector p4Polar_;
    /// internal cache for p4
    mutable LorentzVector p4Cartesian_;
    /// has cache been set?
    mutable  edm::BoolCache cachePolarFixed_, cacheCartesianFixed_;
    /// set internal cache
    inline void cachePolar() const { 
      if ( cachePolarFixed_ ) return;
      p4Polar_ = PolarLorentzVector( pt_, eta_, phi_, mass_ );
      cachePolarFixed_ = true;
    }
    /// set internal cache
    inline void cacheCartesian() const { 
      if ( cacheCartesianFixed_ ) return;
      cachePolar();
      p4Cartesian_ = p4Polar_;
      cacheCartesianFixed_ = true;
    }
    /// clear internal cache
    inline void clearCache() const { 
      cachePolarFixed_ = false;
      cacheCartesianFixed_ = false;
    }
  };

}

#endif
