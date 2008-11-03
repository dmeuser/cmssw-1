#ifndef EgammaCandidates_Photon_h
#define EgammaCandidates_Photon_h
/** \class reco::Photon 
 *
 * Reco Candidates with an Photon component
 *
 * \author Luca Lista, INFN
 *
 * \version $Id: Photon.h,v 1.22 2008/10/28 18:50:05 nancy Exp $
 *
 */
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"

namespace reco {

  class Photon : public RecoCandidate {
  public:
    /// default constructor
    Photon() : RecoCandidate() { }
    /// constructor from values
    Photon( const LorentzVector & p4, Point caloPos, 
	    const SuperClusterRef scl, 
            float HoE,
	    bool hasPixelSeed=false, const Point & vtx = Point( 0, 0, 0 ) );
    /// destructor
    virtual ~Photon();
    /// returns a clone of the candidate
    virtual Photon * clone() const;
    /// reference to SuperCluster
    virtual reco::SuperClusterRef superCluster() const;
    /// vector of references to  Conversion's
    std::vector<reco::ConversionRef> conversions() const ; 
    /// set reference to SuperCluster
    void setSuperCluster( const reco::SuperClusterRef & r ) { superCluster_ = r; }
    /// add  single ConversionRef to the vector of Refs
    void addConversion( const reco::ConversionRef & r ) { conversions_.push_back(r); }
    /// set primary event vertex used to define photon direction
    void setVertex(const Point & vertex);
    /// set flags for photons in the ECAL fiducial volume
    void setFiducialVolumeFlags( const bool& a, 
				 const bool& b, 
				 const bool& c, 
				 const bool& d, 
				 const bool& e) 
      {  isEB_=a; isEE_=b; isEBGap_=c; isEEGap_=d; isEBEEGap_=e; } 
    /// set relevant shower shape variables 
    void setShowerShapeVariables ( const float& a, 
				   const float& b, 
				   const float& c, 
				   const float& d, 
				   const float& e, 
				   const float& f,
				   const float& g)
 
      { maxEnergyXtal_=a, e1x5_=b; e2x5_=c; e3x3_=d; e5x5_=e; covEtaEta_=f; covIetaIeta_=g;}
    /// set relevant isolation variables
    void setIsolationVariablesConeDR04 ( const float a, 
				 const float b, 
				 const float c, 
				 const float d, 
				 const int e, 
				 const int f  ) { 
      isolationEcalRecHitSumConeDR04_=a; 
      isolationHcalTowerSumConeDR04_=b;
      isolationTrkSolidConeDR04_=c;
      isolationTrkHollowConeDR04_=d;
      nTrkSolidConeDR04_=e;
      nTrkHollowConeDR04_=f;
    }
    void setIsolationVariablesConeDR03 ( const float a, 
				       const float b, 
				       const float c, 
				       const float d, 
				       const int e, 
				       const int f  ) { 
      isolationEcalRecHitSumConeDR03_=a; 
      isolationHcalTowerSumConeDR03_ =b;
      isolationTrkSolidConeDR03_ =c;
      isolationTrkHollowConeDR03_=d;
      nTrkSolidConeDR03_ =e;
      nTrkHollowConeDR03_=f;
    }


 
    /// set ID variables and output
    //    void setCutBasedIDOutput ( const bool a, const bool b, const bool c ) { cutBasedLooseEM_=a; cutBasedLoosePhoton_ =b;cutBasedTightPhoton_ =c;  } 
    ////////////  Retrieve quantities
    /// position in ECAL: this is th SC position if r9<0.93. If r8>0.93 is position of seed BasicCluster taking shower depth for unconverted photon
    math::XYZPoint caloPosition() const {return caloPosition_;}
    /// the hadronic over electromagnetic fraction
    float hadronicOverEm() const {return hadOverEm_;}
    /// Whether or not the SuperCluster has a matched GsfElectron pixel seed 
    bool hasPixelSeed() const { return pixelSeed_; }
    /// Bool flagging photons with a vector of refereces to conversions with size >0
    bool hasConversionTracks() const;
    /// Fiducial volume
    /// true if photon is in ECAL barrel
    bool isEB() const{return isEB_;}
    // true if photon is in ECAL endcap
    bool isEE() const{return isEE_;}
    /// true if photon is in EB, and inside the boundaries in super crystals/modules
    bool isEBGap() const{return isEBGap_;}
    /// true if photon is in EE, and inside the boundaries in supercrystal/D
    bool isEEGap() const{return isEEGap_;}
    /// true if photon is in boundary between EB and EE
    bool isEBEEGap() const{return isEBEEGap_;}
    ///  Shower shape variables
    float maxEnergyXtal() const {return maxEnergyXtal_;}
    float e1x5()          const {return e1x5_;}
    float e2x5()          const {return e2x5_;}
    float e3x3()          const {return e3x3_;}
    float e5x5()          const {return e5x5_;}
    float covEtaEta()     const {return covEtaEta_;}
    float covIetaIeta()   const {return covIetaIeta_;}
    float r1x5 ()         const {return e1x5_/e5x5_;}
    float r2x5 ()         const {return e2x5_/e5x5_;}
    float r9 ()           const {return e3x3_/this->superCluster()->rawEnergy();}  

    /// Isolation variables in cone dR=0.4
    ///Ecal isolation sum calculated from recHits
    float ecalRecHitSumConeDR04()      const{return isolationEcalRecHitSumConeDR04_;}
    /// Hcal isolation sum
    float hcalTowerSumConeDR04()      const{return isolationHcalTowerSumConeDR04_;}
    //  Track pT sum c
    float isolationTrkSolidConeDR04()    const{return  isolationTrkSolidConeDR04_;}
    //As above, excluding the core at the center of the cone
    float isolationTrkHollowConeDR04()   const{return  isolationTrkHollowConeDR04_;}
    //Returns number of tracks in a cone of dR
    int nTrkSolidConeDR04()              const{return  nTrkSolidConeDR04_;}
    //As above, excluding the core at the center of the cone
    int nTrkHollowConeDR04()            const{return  nTrkHollowConeDR04_;}
    //
    /// Isolation variables in cone dR=0.3
    float ecalRecHitSumConeDR03()      const{return isolationEcalRecHitSumConeDR03_;}
    /// Hcal isolation sum
    float hcalRecHitSumConeDR03()      const{return isolationHcalTowerSumConeDR03_;}
    //  Track pT sum c
    float isolationTrkSolidConeDR03()    const{return  isolationTrkSolidConeDR03_;}
    //As above, excluding the core at the center of the cone
    float isolationTrkHollowConeDR03()   const{return  isolationTrkHollowConeDR03_;}
    //Returns number of tracks in a cone of dR
    int nTrkSolidConeDR03()              const{return  nTrkSolidConeDR03_;}
    //As above, excluding the core at the center of the cone
    int nTrkHollowConeDR03()             const{return  nTrkHollowConeDR03_;}

  private:
    /// check overlap with another candidate
    virtual bool overlap( const Candidate & ) const;
    /// position of seed BasicCluster for shower depth of unconverted photon
    math::XYZPoint caloPosition_;
    /// reference to a SuperCluster
    reco::SuperClusterRef superCluster_;
    // vector of references to Conversions
    std::vector<reco::ConversionRef>  conversions_;

    float hadOverEm_;
    bool pixelSeed_;
    bool isEB_;
    bool isEE_;
    bool isEBGap_;
    bool isEEGap_;
    bool isEBEEGap_;
    /// shower shape variables
    float maxEnergyXtal_;
    float e1x5_;
    float e2x5_;
    float e3x3_;
    float e5x5_;
    float covEtaEta_;
    float covIetaIeta_;
    /// Isolation variables in cone dR=0.4
    float  isolationEcalRecHitSumConeDR04_;
    float  isolationHcalTowerSumConeDR04_;
    float  isolationTrkSolidConeDR04_;
    float  isolationTrkHollowConeDR04_;
    int  nTrkSolidConeDR04_;
    int  nTrkHollowConeDR04_;
    /// Isolation variables in cone dR=0.3
    float  isolationEcalRecHitSumConeDR03_;
    float  isolationHcalTowerSumConeDR03_;
    float  isolationTrkSolidConeDR03_;
    float  isolationTrkHollowConeDR03_;
    int  nTrkSolidConeDR03_;
    int  nTrkHollowConeDR03_;



  };
  
}

#endif
