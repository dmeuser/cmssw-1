#ifndef ParticleFlowReco_PFRecHit_h
#define ParticleFlowReco_PFRecHit_h
/** 
 */
#include <vector>
#include <map>
#include <iostream>

#include "DataFormats/Math/interface/Point3D.h"
#include "DataFormats/Math/interface/Vector3D.h"
// #include "DataFormats/DetId/interface/DetId.h"
#include "Math/GenVector/PositionVector3D.h"


//C decide what is the default rechit index. 
//C maybe 0 ? -> compression 
//C then the position is index-1. 
//C provide a helper class to access the rechit. 


namespace reco {

  /**\class PFRecHit
     \brief Particle flow rechit (rechit + geometry and topology information). See clustering algorithm in PFClusterAlgo
          
     \author Colin Bernet
     \date   July 2006
  */
  class PFRecHit {

  public:
    
    enum {
      NONE=0
    };

    typedef ROOT::Math::PositionVector3D<ROOT::Math::CylindricalEta3D<Double32_t> > REPPoint;

    /// default constructor. Sets energy and position to zero
    PFRecHit();

    /// constructor from values
    PFRecHit(unsigned detId,
	     int layer,
	     double energy, 
	     const math::XYZPoint& posxyz, 
	     const math::XYZVector& axisxyz, 
	     const std::vector< math::XYZPoint >& cornersxyz);

    PFRecHit(unsigned detId,
	     int layer,
	     double energy, 
	     double posx, double posy, double posz, 
	     double axisx, double axisy, double axisz);    

    /// copy
    PFRecHit(const PFRecHit& other);

    /// destructor
    virtual ~PFRecHit();

    /// calculates rho eta phi position once and for all
    void calculatePositionREP();

    //C neighbours must be initialized correctly !!
/*     void setNeighbours( const std::vector< unsigned >& neighbours ); */
    void add4Neighbour( unsigned index );
    void add8Neighbour( unsigned index );
    
    

    /// \brief search for pointers to neighbours, using neighbours' DetId.
    /// 
    /// pointers to neighbours are not persistent, in contrary to the DetId's 
    /// of the neighbours. This function searches a map of rechits 
    /// for the DetId's stored in neighboursIds4_ and  neighboursIds8_. 
    /// The corresponding pointers are stored in neighbours4_ and neighbours8_.
    // void      findPtrsToNeighbours( const std::map<unsigned,  reco::PFRecHit* >& allhits );

    void      setNWCorner( double posx, double posy, double posz );
    void      setSWCorner( double posx, double posy, double posz );
    void      setSECorner( double posx, double posy, double posz );
    void      setNECorner( double posx, double posy, double posz );

    /// rechit detId
    unsigned detId() const {return detId_;}

    /// rechit layer
    int layer() const { return layer_; }

    /// rechit energy
    double energy() const { return energy_; }

    //C remove cause I want to be able to run on const rechits
    /// \return seed state (-1:unknown, 0:no, 1 yes)
    // int  seedState() const { return seedState_; }
    
    /// is seed ? 
    // bool isSeed() const { return (seedState_>0) ? true : false; }

    /// set seed status
    // void youAreSeed(int seedstate=1) {seedState_ = seedstate;} 

    /// rechit cell centre x, y, z
    const math::XYZPoint& positionXYZ() const { return posxyz_; }

    /// rechit cell centre rho, eta, phi. call calculatePositionREP before !
    const REPPoint& positionREP() const;

    /// rechit cell axis x, y, z
    const math::XYZVector& getAxisXYZ() const { return axisxyz_; }    

    /// rechit corners
    const std::vector< math::XYZPoint >& getCornersXYZ() const 
      { return cornersxyz_; }    

    const std::vector< unsigned >& neighbours4() const 
      {return neighbours4_;}  

    const std::vector< unsigned >& neighbours8() const 
      {return neighbours8_;}  

/*     const std::vector< unsigned >& getNeighboursIds4() const  */
/*       {return neighboursIds4_;}   */

/*     const std::vector< unsigned >& getNeighboursIds8() const  */
/*       {return neighboursIds8_;}   */

    /// is rechit 'id' a direct neighbour of this ? 
    /// id is the rechit index ! not the detId
    bool  isNeighbour4(unsigned id) const;

    /// is rechit 'id' a neighbour of this ? 
    /// id is the rechit index ! not the detId
    bool  isNeighbour8(unsigned id) const;
    

    void size(double& deta, double& dphi) const;

    /// comparison >= operator
    bool operator>=(const PFRecHit& rhs) const { return (energy_>=rhs.energy_); }

    /// comparison > operator
    bool operator> (const PFRecHit& rhs) const { return (energy_> rhs.energy_); }

    /// comparison <= operator
    bool operator<=(const PFRecHit& rhs) const { return (energy_<=rhs.energy_); }

    /// comparison < operator
    bool operator< (const PFRecHit& rhs) const { return (energy_< rhs.energy_); }

    friend std::ostream& operator<<(std::ostream& out, 
				    const reco::PFRecHit& hit);

  private:

    ///C cell detid - should be detid or index in collection ?
    unsigned            detId_;             

    /// rechit layer
    int                 layer_;

    /// rechit energy
    double              energy_;

    /// is this a seed ? (-1:unknown, 0:no, 1 yes) (transient)
    // int                 seedState_;
 
    /// rechit cell centre: x, y, z
    math::XYZPoint      posxyz_;

    /// rechit cell centre: rho, eta, phi (transient)
    REPPoint            posrep_;

    /// rechit cell axisxyz
    math::XYZVector     axisxyz_;

    /// rechit cell corners
    std::vector< math::XYZPoint > cornersxyz_;

  
    /// indices to existing neighbours (1 common side)
    std::vector< unsigned >   neighbours4_;

    /// indices to existing neighbours (1 common side or diagonal) 
    std::vector< unsigned >   neighbours8_;

    /// number of neighbours
    static const unsigned    nNeighbours_;
    
    /// number of corners
    static const unsigned    nCorners_;

    /// set position of one of the corners
    void      setCorner( unsigned i, double posx, double posy, double posz );
  };
  
}

#endif
