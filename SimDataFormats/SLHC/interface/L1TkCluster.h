/// ////////////////////////////////////////
/// Stacked Tracker Simulations          ///
///                                      ///
/// Nicola Pozzobon, UNIPD               ///
/// Emmanuele Salvati, Cornell           ///
///                                      ///
/// 2011, June                           ///
/// 2011, October                        ///
/// 2012, August                         ///
/// 2013, January                        ///
/// ////////////////////////////////////////

#ifndef STACKED_TRACKER_L1TK_CLUSTER_FORMAT_H
#define STACKED_TRACKER_L1TK_CLUSTER_FORMAT_H

#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/MeasurementPoint.h"

#include "SimDataFormats/TrackerDigiSimLink/interface/PixelDigiSimLink.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"

#include "Geometry/CommonTopologies/interface/Topology.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"

#include "Geometry/TrackerGeometryBuilder/interface/StackedTrackerGeometry.h"

  /** ************************ **/
  /**                          **/
  /**   DECLARATION OF CLASS   **/
  /**                          **/
  /** ************************ **/

  template< typename T >
  class L1TkCluster
  {
    public:
      /// Constructors
      L1TkCluster();
      L1TkCluster( std::vector< T > aHits, DetId aDetId, unsigned int aStackMember );

      /// Destructor
      ~L1TkCluster();

      /// Data members:   getABC( ... )
      /// Helper methods: findABC( ... )

      /// Hits in the Cluster
      std::vector< T > getHits() const;
      void             setHits( std::vector< T > aHits );

      /// Detector element
      DetId        getDetId() const;
      void         setDetId( DetId aDetId );
      unsigned int getStackMember() const;
      void         setStackMember( unsigned int aStackMember );

      /// MC Truth
      std::vector< edm::Ptr< SimTrack > > getSimTrackPtrs() const;
      bool                                isGenuine() const;
      bool                                isCombinatoric() const;
      bool                                isUnknown() const;
      int                                 findType() const;
      unsigned int                        findSimTrackId() const;

      /// Cluster width
      unsigned int findWidth() const;

      /// Single hit coordinates and position
      MeasurementPoint findHitLocalCoordinates( unsigned int hitIdx ) const;
      LocalPoint       findHitLocalPosition( const StackedTrackerGeometry *theStackedTracker, unsigned int hitIdx ) const;
      GlobalPoint      findHitGlobalPosition( const StackedTrackerGeometry *theStackedTracker, unsigned int hitIdx ) const;

      /// Average cluster coordinates and position
      MeasurementPoint findAverageLocalCoordinates() const;
      LocalPoint       findAverageLocalPosition( const StackedTrackerGeometry *theStackedTracker ) const;
      GlobalPoint      findAverageGlobalPosition( const StackedTrackerGeometry *theStackedTracker ) const;

      /// Collect MC truth
      void checkSimTrack( const StackedTrackerGeometry *theStackedTracker,
                          edm::Handle<edm::DetSetVector<PixelDigiSimLink> > thePixelDigiSimLinkHandle,
                          edm::Handle<edm::SimTrackContainer> simTrackHandle );

      /// Information
      std::string print( unsigned int i=0 ) const;

    private:
      /// Data members
      std::vector< T >                    theHits;
      DetId                               theDetId;
      unsigned int                        theStackMember;
      std::vector< edm::Ptr< SimTrack > > theSimTracks;

  }; /// Close class

  /** ***************************** **/
  /**                               **/
  /**   IMPLEMENTATION OF METHODS   **/
  /**                               **/
  /** ***************************** **/

  /// Default Constructor
  /// NOTE: to be used with setSomething(...) methods
  template< typename T >
  L1TkCluster< T >::L1TkCluster()
  {
    /// Set default data members
    theHits.clear();
    theDetId = 0;
    theStackMember = 0;
    theSimTracks.clear();
  }

  /// Another Constructor
  template< typename T >
  L1TkCluster< T >::L1TkCluster( std::vector< T > aHits, DetId aDetId, unsigned int aStackMember )
  {
    /// Set default data members
    theHits.clear();
    for ( unsigned int j = 0; j < aHits.size(); j++ )
      theHits.push_back( aHits.at(j) );
    theDetId = aDetId;
    theStackMember = aStackMember;
    theSimTracks.clear();
  }

  /// Destructor
  template< typename T >
  L1TkCluster< T >::~L1TkCluster(){}

  /// Hits composing the cluster
  template< typename T >
  std::vector< T > L1TkCluster< T >::getHits() const { return theHits; }

  template< typename T >
  void L1TkCluster< T >::setHits( std::vector< T > aHits )
  {
    for ( unsigned int j=0; j<aHits.size(); j++ )
      theHits.push_back( aHits.at(j) );
  }

  /// Detector element information
  template< typename T >
  DetId L1TkCluster< T >::getDetId() const { return theDetId; }

  template< typename T >
  void L1TkCluster< T >::setDetId( DetId aDetId ) { theDetId = aDetId; }

  template< typename T >
  unsigned int L1TkCluster< T >::getStackMember() const { return theStackMember; }

  template< typename T >
  void L1TkCluster< T >::setStackMember( unsigned int aStackMember ) { theStackMember = aStackMember; }

  /// MC truth information
  template< typename T >
  std::vector< edm::Ptr< SimTrack > > L1TkCluster< T >::getSimTrackPtrs() const { return theSimTracks; }

  template< typename T >
  bool L1TkCluster< T >::isGenuine() const
  {
    /// GENUINE means that ALL hits could be associated to a
    /// SimTrack stored in the corresponding collection, AND
    /// all of these SimTracks are actually the same
    int prevTrack = -99999; // SimTrackId storage
    for ( unsigned int k = 0; k < theSimTracks.size(); k++ )
    {
      edm::Ptr< SimTrack > curSimTrackPtr = theSimTracks.at(k);
      if ( curSimTrackPtr.isNull() )
        /// Unknown SimTrack means false
        return false;
      else
      {
        if ( theSimTracks.size() > 1 )
        {
          if ( prevTrack < 0 )
            prevTrack = curSimTrackPtr->trackId();

          if ( prevTrack != (int)curSimTrackPtr->trackId() )
            /// Two different known SimTracks means false
            return false;
 
          prevTrack = curSimTrackPtr->trackId();
        }
      }
    }
    /// If not false, then it is true
    return true;
  }

  template< typename T >
  bool L1TkCluster< T >::isCombinatoric() const
  {
    /// COMBINATORIC means that different SimTracks contribute
    /// to the cluster, which means that both a mixture of NULL
    /// pointers and good ones are present, or that all are
    /// good but there are more SimTracks associated to the cluster
    int prevTrack = -99999; // SimTrackId storage
    unsigned int numberNulls = 0; // Number of non-found SimTracks
    unsigned int numberGoods = 0; // Number of found SimTracks
    for ( unsigned int k = 0; k < theSimTracks.size(); k++ )
    {
      edm::Ptr< SimTrack > curSimTrackPtr = theSimTracks.at(k);
      if ( curSimTrackPtr.isNull() )
        numberNulls++;
      else
        numberGoods++;

      if ( numberNulls > 0 && numberGoods > 0 )
        /// Mixture of known and unknown SimTracks means true
        return true;

      if ( curSimTrackPtr.isNull() == false )
      {
        if ( theSimTracks.size() > 1 )
        {
          if ( prevTrack < 0 )
            prevTrack = curSimTrackPtr->trackId();

          if ( prevTrack != (int)curSimTrackPtr->trackId() )
            /// Two different known SimTracks means true
            return true;

          prevTrack = curSimTrackPtr->trackId();
        }
      }
    }

    if ( numberNulls > 0 && numberGoods == 0 )
      /// Only unknown SimTracks means unknown, hence false
      return false;

    /// If not true, then it is false
    return false;
  }

  template< typename T >
  bool L1TkCluster< T >::isUnknown() const
  {
    /// UNKNOWN means that all SimTracks pointers are NULL
    for ( unsigned int k = 0; k < theSimTracks.size(); k++ )
    {
      edm::Ptr< SimTrack > curSimTrackPtr = theSimTracks.at(k);
      if ( curSimTrackPtr.isNull() == false )
        /// A known SimTrack means false
        return false;
    }
    /// If not false, then it is true
    return true;
  }

  template< typename T >
  int L1TkCluster< T >::findType() const
  {
    if ( this->isGenuine() )
      return theSimTracks.at(0)->type();
    return 999999999;
  }

  template< typename T >
  unsigned int L1TkCluster< T >::findSimTrackId() const
  {
    if ( this->isGenuine() )
      return theSimTracks.at(0)->trackId();
    return 0;
  }

  /// Cluster width
  /// Specialize the template for PSimHits in *cc
  template<>
  unsigned int L1TkCluster< edm::Ref<edm::PSimHitContainer> >::findWidth() const;

  /// Cluster width
  /// Default template for PixelDigis
  template< typename T >
  unsigned int L1TkCluster< T >::findWidth() const
  {
    int rowMin = 99999999;
    int rowMax = 0;
    /// For broadside Clusters this is equivalent to theHits.size()
    /// but for 2d or neighbor Clusters this is only the actual size in RPhi
    for ( unsigned int i=0; i<theHits.size(); i++ )
    {
      if ( theHits.at(i)->row() < rowMin )
        rowMin = theHits.at(i)->row();
      if ( theHits.at(i)->row() > rowMax )
        rowMax = theHits.at(i)->row();
    }
    return abs( rowMax - rowMin + 1 ); /// This takes care of 1-Pixel clusters
  }

  /// Get hit local coordinates
  /// Specialize the template for PSimHits in *cc
  template<>
  MeasurementPoint L1TkCluster< edm::Ref< edm::PSimHitContainer > >::findHitLocalCoordinates( unsigned int hitIdx ) const;

  /// Get hit local coordinates
  /// Default template for PixelDigis
  template< typename T >
  MeasurementPoint L1TkCluster< T >::findHitLocalCoordinates( unsigned int hitIdx ) const
  {
    /// NOTE in this case, DO NOT add 0.5
    /// to get the center of the pixel
    MeasurementPoint mp( theHits.at(hitIdx)->row(), theHits.at(hitIdx)->column() );
    return mp;
  }

  /// Get hit local position
  /// Specialize the template for PSimHits in *cc
  template<>
  LocalPoint L1TkCluster< edm::Ref< edm::PSimHitContainer > >::findHitLocalPosition( const StackedTrackerGeometry *theStackedTracker, unsigned int hitIdx ) const;

  /// Get hit local position
  /// Default template for PixelDigis
  template< typename T >
  LocalPoint L1TkCluster< T >::findHitLocalPosition( const StackedTrackerGeometry *theStackedTracker, unsigned int hitIdx ) const
  {
    /// Add 0.5 to get the center of the pixel
    const GeomDetUnit* geomDetUnit = theStackedTracker->idToDetUnit( theDetId, theStackMember );
    MeasurementPoint mp( theHits.at(hitIdx)->row() + 0.5, theHits.at(hitIdx)->column() + 0.5 );
    return geomDetUnit->topology().localPosition( mp );
  }

  /// Get hit global position
  /// Specialize the template for PSimHits in *cc
  template<>
  GlobalPoint L1TkCluster< edm::Ref< edm::PSimHitContainer > >::findHitGlobalPosition( const StackedTrackerGeometry *theStackedTracker, unsigned int hitIdx ) const;

  /// Get hit global position
  /// Default template for PixelDigis
  template< typename T >
  GlobalPoint L1TkCluster< T >::findHitGlobalPosition( const StackedTrackerGeometry *theStackedTracker, unsigned int hitIdx ) const
  {
    /// Add 0.5 to get the center of the pixel
    const GeomDetUnit* geomDetUnit = theStackedTracker->idToDetUnit( theDetId, theStackMember );
    MeasurementPoint mp( theHits.at(hitIdx)->row() + 0.5, theHits.at(hitIdx)->column() + 0.5 );
    return geomDetUnit->surface().toGlobal( geomDetUnit->topology().localPosition( mp ) );
  }

  /// Unweighted average local cluster coordinates
  /// Specialize the template for PSimHits in *cc
  template<>
  MeasurementPoint L1TkCluster< edm::Ref< edm::PSimHitContainer > >::findAverageLocalCoordinates() const;

  /// Unweighted average local cluster coordinates
  /// Default template for PixelDigis
  template< typename T >
  MeasurementPoint L1TkCluster< T >::findAverageLocalCoordinates() const
  {
    double averageCol = 0.0;
    double averageRow = 0.0;

    /// Loop over the hits and calculate the average coordinates
    if ( theHits.size() != 0 )
    {
      typename std::vector< T >::const_iterator hitIter;
      for ( hitIter = theHits.begin();
            hitIter != theHits.end();
            hitIter++ )
      {
        averageCol += (*hitIter)->column();
        averageRow += (*hitIter)->row();
      }
      averageCol /= theHits.size();
      averageRow /= theHits.size();
    }
    return MeasurementPoint( averageRow, averageCol );
  }

  /// Unweighted average local cluster position
  template< typename T >
  LocalPoint L1TkCluster< T >::findAverageLocalPosition( const StackedTrackerGeometry *theStackedTracker ) const
  {
    double averageX = 0.0;
    double averageY = 0.0;

    /// Loop over the hits and calculate the average coordinates
    if ( theHits.size() != 0 )
    {
      for ( unsigned int i = 0; i < theHits.size(); i++ )
      {
        LocalPoint thisHitPosition = this->findHitLocalPosition( theStackedTracker, i );
        averageX += thisHitPosition.x();
        averageY += thisHitPosition.y();
      }
      averageX /= theHits.size();
      averageY /= theHits.size();
    }
    return LocalPoint( averageX, averageY );
  }

  /// Unweighted average cluster position
  template< typename T >
  GlobalPoint L1TkCluster< T >::findAverageGlobalPosition( const StackedTrackerGeometry *theStackedTracker ) const
  {
    double averageX = 0.0;
    double averageY = 0.0;
    double averageZ = 0.0;

    /// Loop over the hits and calculate the average coordinates
    if ( theHits.size() != 0 )
    {
      for ( unsigned int i = 0; i < theHits.size(); i++ )
      {
        GlobalPoint thisHitPosition = this->findHitGlobalPosition( theStackedTracker, i );
        averageX += thisHitPosition.x();
        averageY += thisHitPosition.y();
        averageZ += thisHitPosition.z();
      }
      averageX /= theHits.size();
      averageY /= theHits.size();
      averageZ /= theHits.size();
    }
    return GlobalPoint( averageX, averageY, averageZ );
  }

  /// Collect MC truth
  /// Specify Template for PSimHits in *.cc file
  template<>
  void L1TkCluster< edm::Ref< edm::PSimHitContainer > >::checkSimTrack( const StackedTrackerGeometry *theStackedTracker,
                                                                                     edm::Handle<edm::DetSetVector<PixelDigiSimLink> >  thePixelDigiSimLinkHandle,
                                                                                     edm::Handle<edm::SimTrackContainer>   simTrackHandle );

  /// Default template for PixelDigis
  template< typename T >
  void L1TkCluster< T >::checkSimTrack( const StackedTrackerGeometry *theStackedTracker,
                                                     edm::Handle<edm::DetSetVector<PixelDigiSimLink> >  thePixelDigiSimLinkHandle,
                                                     edm::Handle<edm::SimTrackContainer>   simTrackHandle )
  {
    /// Discrimination between MC and data in Builder!

    /// Get the PixelDigiSimLink
    const DetId detId = theStackedTracker->idToDet( theDetId, theStackMember )->geographicalId();
    edm::DetSet<PixelDigiSimLink> thisDigiSimLink = (*thePixelDigiSimLinkHandle)[detId.rawId()];
    edm::DetSet<PixelDigiSimLink>::const_iterator iterSimLink;

    /// Loop over all the hits composing the L1TkCluster
    for ( unsigned int i = 0; i < theHits.size(); i++ )
    {
      /// Loop over PixelDigiSimLink
      for ( iterSimLink = thisDigiSimLink.data.begin();
            iterSimLink != thisDigiSimLink.data.end();
            iterSimLink++ )
      {
        /// Threshold (redundant, already applied within L1TkClusterBuilder)
        //if ( theHit.adc() <= 30 ) continue;
        /// Find the link and, if there's not, skip
        if ( (int)iterSimLink->channel() != theHits.at(i)->channel() ) continue;

        /// Get SimTrack Id and type
        unsigned int curSimTrkId = iterSimLink->SimTrackId();

        /// This version of the collection of the SimTrack ID and PDG
        /// may not be fast and optimal, but is safer since the
        /// SimTrack ID is shifted by 1 wrt the index in the vector,
        /// and this may not be so true on a general basis...
        bool foundSimTrack = false;
        for ( unsigned int j = 0; j < simTrackHandle->size() && !foundSimTrack; j++ )
        {
          if ( simTrackHandle->at(j).trackId() == curSimTrkId )
          {
            foundSimTrack = true;
            edm::Ptr< SimTrack > testSimTrack( simTrackHandle, j );
            theSimTracks.push_back( testSimTrack );
          }
        }

        if ( !foundSimTrack )
        {
          edm::Ptr< SimTrack >* testSimTrack = new edm::Ptr< SimTrack >();
          theSimTracks.push_back( *testSimTrack );
        }
      }
    } /// End of Loop over all the hits composing the L1TkCluster
  }

  /// Information
  template< typename T >
  std::string L1TkCluster< T >::print( unsigned int i ) const
  {
    std::string padding("");
    for ( unsigned int j=0; j!=i; ++j )
      padding+="\t";
    std::stringstream output;
    output<<padding<<"L1TkCluster:\n";
    padding+='\t';
    output << padding << "DetId: " << theDetId.rawId() << '\n';
    output << padding << "member: " << theStackMember << ", cluster size: " << theHits.size() << '\n';
    return output.str();
  }

  template< typename T >
  std::ostream& operator << (std::ostream& os, const L1TkCluster< T >& aL1TkCluster) { return ( os<<aL1TkCluster.print() ); }



#endif

