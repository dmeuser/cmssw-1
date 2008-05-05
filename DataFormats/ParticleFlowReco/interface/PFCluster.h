#ifndef DataFormats_ParticleFlowReco_PFCluster_h
#define DataFormats_ParticleFlowReco_PFCluster_h

#include "DataFormats/CaloRecHit/interface/CaloCluster.h"

#include "Math/GenVector/PositionVector3D.h"
#include "DataFormats/Math/interface/Point3D.h"
#include "Rtypes.h" 

#include "DataFormats/ParticleFlowReco/interface/PFRecHitFraction.h"
#include "DataFormats/ParticleFlowReco/interface/PFLayer.h"

#include <iostream>
#include <vector>



class PFClusterAlgo;

namespace reco {

  /**\class PFCluster
     \brief Particle flow cluster, see clustering algorithm in PFClusterAlgo
     
     A particle flow cluster is defined by its energy and position, which are 
     calculated from a vector of PFRecHitFraction. This calculation is 
     performed in PFClusterAlgo.

     \todo Clean up this class to a common base (talk to Paolo Meridiani)
     the extra internal stuff (like the vector of PFRecHitFraction's)
     could be moved to a PFClusterExtra.
     
     \todo Now that PFRecHitFraction's hold a reference to the PFRecHit's, 
     put back the calculation of energy and position to PFCluster. 


     \todo Add an operator+=

     \author Colin Bernet
     \date   July 2006
  */
  class PFCluster : public CaloCluster {
  public:


    typedef ROOT::Math::PositionVector3D<ROOT::Math::CylindricalEta3D<Double32_t> > REPPoint;
  
    /// default constructor
    PFCluster() :  color_(1) {}

    /// constructor
    PFCluster(PFLayer::Layer layer, double energy,
              double x, double y, double z );



    /// resets clusters parameters
    void reset();
    
    /// add a given fraction of the rechit
    void addRecHitFraction( const reco::PFRecHitFraction& frac);
                                                
    /// vector of rechit fractions
    const std::vector< reco::PFRecHitFraction >& recHitFractions() const 
      { return rechits_; }

    /// set layer
    void setLayer( PFLayer::Layer layer);
    
    /// cluster layer, see PFLayer.h in this directory
    PFLayer::Layer  layer() const;     

    /// cluster energy
    double        energy() const {return energy_;}

    /// cluster position: rho, eta, phi
    const REPPoint&       positionREP() const {return posrep_;}

    /// calculates posrep_ once and for all
    void calculatePositionREP() {
      posrep_.SetCoordinates( position_.Rho(), position_.Eta(), position_.Phi() ); 
    }

    /// \todo move to PFClusterTools
    static double getDepthCorrection(double energy, bool isBelowPS = false,
                                     bool isHadron = false);

    /// set cluster color (for the PFRootEventManager display)
    void         setColor(int color) {color_ = color;}

    /// \return color
    int          color() const {return color_;}
  

    PFCluster& operator=(const PFCluster&);

    friend    std::ostream& operator<<(std::ostream& out, 
                                       const PFCluster& cluster);
    /// counter
    static unsigned     instanceCounter_;

    /// \todo move to PFClusterTools
    static void setDepthCorParameters(int mode, 
                                      double a, double b, 
                                      double ap, double bp ) {
      depthCorMode_ = mode;
      depthCorA_ = a; 
      depthCorB_ = b; 
      depthCorAp_ = ap; 
      depthCorBp_ = bp; 
    } 
    

  private:
  
    /// vector of rechit fractions (transient)
    std::vector< reco::PFRecHitFraction >  rechits_;

    /// cluster position: rho, eta, phi (transient)
    REPPoint            posrep_;


    /// \todo move to PFClusterTools
    static int    depthCorMode_;
    /// \todo move to PFClusterTools
    static double depthCorA_;
    /// \todo move to PFClusterTools
    static double depthCorB_ ;
    /// \todo move to PFClusterTools
    static double depthCorAp_;
    /// \todo move to PFClusterTools
    static double depthCorBp_;


    /// color (transient)
    int                 color_;
    
    friend class ::PFClusterAlgo;
  };
}

#endif
