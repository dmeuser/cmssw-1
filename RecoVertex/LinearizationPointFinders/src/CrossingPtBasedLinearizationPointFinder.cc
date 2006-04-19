#include "RecoVertex/LinearizationPointFinders/interface/CrossingPtBasedLinearizationPointFinder.h"
#include "RecoVertex/LinearizationPointFinders/interface/LinPtException.h"
#include "RecoVertex/VertexTools/interface/TwoTrackMinimumDistance.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoVertex/VertexTools/interface/ModeFinder3d.h"
#include "Geometry/Vector/interface/GlobalPoint.h"
#include "RecoVertex/LinearizationPointFinders/interface/FallbackLinearizationPointFinder.h"
#include <cmath>
#include <algorithm>

// #define Use_GraphicsHarvester
#ifdef Use_GraphicsHarvester
#include "RecoVertex/VertexSimpleVis/interface/PrimitivesHarvester.h"
#include "Utilities/UI/interface/SimpleConfigurable.h"
#include "RecoVertex/VertexTools/interface/SubsetHsmModeFinder3d.h"
#include "RecoVertex/VertexTools/interface/HsmModeFinder3d.h"
#include "RecoVertex/VertexTools/interface/LmsModeFinder3d.h"
#endif

namespace {
  inline GlobalPoint operator - ( const GlobalPoint & a, const GlobalPoint & b ){
    return GlobalPoint ( a.x() - b.x(), a.y() - b.y(), a.z() - b.z() );
  };

  inline GlobalPoint operator + ( const GlobalPoint & a, const GlobalPoint & b ){
    return GlobalPoint ( a.x() + b.x(), a.y() + b.y(), a.z() + b.z() );
  };

  inline GlobalPoint operator / ( const GlobalPoint & a, const double b ){
    return GlobalPoint ( a.x() / b, a.y() / b, a.z() / b );
  };

  inline GlobalPoint operator * ( const GlobalPoint & a, const double b ){
    return GlobalPoint ( a.x() * b, a.y() * b, a.z() * b );
  };

  inline GlobalPoint operator * ( const double b , const GlobalPoint & a ){
    return GlobalPoint ( a.x() * b, a.y() * b, a.z() * b );
  };

  inline unsigned int sum ( unsigned int nr )
  {
    /*
    int ret=0;
    for ( int i=1; i<= nr ; i++ )
    {
      ret+=i;
    };
    return ret;
    */
    return ( nr * ( nr + 1 ) ) / 2;
  };

  #ifdef Use_GraphicsHarvester
  void graphicsDebug ( const vector < PointAndDistance > & input )
  {
    bool sve = SimpleConfigurable<bool> (false,
            "CrossingPointStudy:Harvest").value();
    string fname = SimpleConfigurable<string>("crossingpoints.txt",
            "CrossingPointStudy:FileName").value();
    if (sve)
    {
      for ( vector< PointAndDistance >::const_iterator i=input.begin();
            i!=input.end() ; ++i )
      {
        map < string, MultiType > attrs;
        attrs["name"]="GlobalPoint";
        attrs["color"]="yellow";
        attrs["mag"]=.7;
        attrs["comment"]="Input for CrossingPtBasedLinearizationPointFinder";
        PrimitivesHarvester( fname )
          .save ( i->first, attrs );
      };
      map < string, MultiType > attrs;
      attrs["name"]="The Mode(*theAlgo)";
      attrs["color"]="green";
      attrs["comment"]="Output from CrossingPtBasedLinearizationPointFinder";
      PrimitivesHarvester( fname )
        .save ( ret, attrs );
      GlobalPoint subsethsm = SubsetHsmModeFinder3d()( input );
      attrs["name"]="The Mode(SubsetHSM)";
      attrs["color"]="red";
      PrimitivesHarvester( fname ).save ( subsethsm, attrs );
      GlobalPoint hsm = HsmModeFinder3d()( input );
      attrs["name"]="The Mode(HSM)";
      attrs["color"]="blue";
      PrimitivesHarvester( fname ).save ( hsm, attrs );
      GlobalPoint lms = LmsModeFinder3d()( input );
      attrs["name"]="The Mode(LMS)";
      attrs["color"]="gold";
      PrimitivesHarvester( fname ).save ( lms, attrs );
    };
  };
  #endif
};

CrossingPtBasedLinearizationPointFinder::CrossingPtBasedLinearizationPointFinder(
     const ModeFinder3d & algo, const signed int n_pairs ) :
  useMatrix ( false ) , theNPairs ( n_pairs ), theMatrix ( 0 ),
  theAlgo ( algo.clone() )
{};

CrossingPtBasedLinearizationPointFinder::CrossingPtBasedLinearizationPointFinder(
    const RecTracksDistanceMatrix * m, const ModeFinder3d & algo,
    const signed int n_pairs ) :
  useMatrix ( m->hasCrossingPoints() ) ,
  theNPairs ( n_pairs ), theMatrix ( m ), theAlgo ( algo.clone() ) 
{};

CrossingPtBasedLinearizationPointFinder::CrossingPtBasedLinearizationPointFinder
    ( const CrossingPtBasedLinearizationPointFinder & o ) :
  useMatrix ( o.useMatrix ), theNPairs ( o.theNPairs ),
  theMatrix ( o.theMatrix /* nope, we dont clone!! */ ), 
  theAlgo ( o.theAlgo->clone() ) {};

CrossingPtBasedLinearizationPointFinder::~CrossingPtBasedLinearizationPointFinder()
{
  delete theAlgo;
};

vector <reco::TransientTrack> CrossingPtBasedLinearizationPointFinder::getBestTracks (
    const vector<reco::TransientTrack> & tracks ) const
{
  unsigned int n_tracks = (2*(unsigned int) (theNPairs)) < tracks.size() ? 2*theNPairs : tracks.size();

  vector <reco::TransientTrack> newtracks; // FIXME ( n_tracks );
  newtracks.reserve ( n_tracks );

  partial_sort_copy ( tracks.begin(), tracks.end(), newtracks.begin(),
      newtracks.begin() + n_tracks  , CompareTwoTracks() ); 

  return newtracks;
};

typedef pair < GlobalPoint , float > PointAndDistance;

GlobalPoint CrossingPtBasedLinearizationPointFinder::useAllTracks(
    const vector<reco::TransientTrack> & tracks ) const
{
  vector< PointAndDistance > vgp;
  // vgp.reserve ( ( tracks.size() * ( tracks.size()-1 ) ) / 2 - 1 );
  TwoTrackMinimumDistance ttmd;
  vector<reco::TransientTrack>::const_iterator end=tracks.end();
  vector<reco::TransientTrack>::const_iterator endm1=(end-1);
  for ( vector<reco::TransientTrack>::const_iterator x=tracks.begin();
       x!=endm1 ; ++x ) {
    for ( vector<reco::TransientTrack>::const_iterator y=x+1;
        y!=end; ++y ) {
      try {
        pair < GlobalPoint, GlobalPoint > pts = ttmd.points
          ( (*x).impactPointState(), (*y).impactPointState() );
        pair < GlobalPoint , float > v ( ( pts.second + pts.first ) / 2. ,
            ( pts.second - pts.first ).mag() );
        vgp.push_back( v );
      }
      catch (...) {}; // If sth goes wrong, we just dont add. Who cares?
    };
  };
  if (! vgp.size() )
  {
    return FallbackLinearizationPointFinder().getLinearizationPoint ( tracks );
  };
  return find ( vgp );
};

GlobalPoint CrossingPtBasedLinearizationPointFinder::useFullMatrix(
    const vector<reco::TransientTrack> & tracks ) const
{
  vector< PointAndDistance > vgp;
  vgp.reserve ( (int) ( tracks.size() * ( tracks.size() -1 ) / 2. - 1 ) );
  vector<reco::TransientTrack>::const_iterator end=tracks.end();
  vector<reco::TransientTrack>::const_iterator endm1=(end-1);
  for ( vector<reco::TransientTrack>::const_iterator x=tracks.begin();
       x!=endm1 ; ++x ) {
    for ( vector<reco::TransientTrack>::const_iterator y=x+1;
        y!=end; ++y ) {
      PointAndDistance v ( theMatrix->crossingPoint ( *x , *y ),
          theMatrix->distance ( *x, *y ) );
      vgp.push_back ( v );
    };
  };
  if (! vgp.size() )
  {
    return FallbackLinearizationPointFinder().getLinearizationPoint ( tracks );
  };
  return find ( vgp );
};

GlobalPoint CrossingPtBasedLinearizationPointFinder::getLinearizationPoint(
    const vector<FreeTrajectoryState> & tracks ) const
{
  return LinearizationPointFinder::getLinearizationPoint ( tracks );
};

GlobalPoint CrossingPtBasedLinearizationPointFinder::find ( 
    const vector < PointAndDistance > & input ) const
{
  /*
  cout << "[CrossingPtBasedLinearizationPointFinder] input size="
       << input.size() << endl;*/
  GlobalPoint ret = (*theAlgo) (input);
  #ifdef Use_GraphicsHarvester
  graphicsDebug ( input );
  #endif
  return ret;
};

GlobalPoint CrossingPtBasedLinearizationPointFinder::getLinearizationPoint(
    const vector<reco::TransientTrack> & tracks ) const
{
  if ( tracks.size() < 2 ) throw LinPtException
     ("CrossingPtBasedLinPtFinder: too few tracks given.");
  try {
    vector < PointAndDistance > vgp;
    if ( theNPairs == -1 )
    {
      if ( useMatrix )
      {
        return useFullMatrix( tracks );
      } else {
        return useAllTracks ( tracks );
      };
    };

    if ( sum ( tracks.size() - 1 ) < ((unsigned int) (theNPairs)) )
    {
      /*
      cout << "[CrossingPtBasedLinearizationPointFinder] we exploit all track pairs"
           << endl;*/
      // we have fewer track pair options than is foreseen
      // in the quota.
      // so we exploit all tracks pairs.
      return useAllTracks ( tracks );
    };

    vector <reco::TransientTrack> goodtracks = getBestTracks ( tracks );

    // we sort according to momenta.
    if ( goodtracks.size() < 2 ) throw LinPtException (
        "CrossingPtBasedLinPtFinder: less than two tracks given");
    // vgp.reserve ( theNPairs - 1 );
    unsigned int t_first = 0;
    unsigned int t_interval = goodtracks.size() / 2;
    unsigned int lim = goodtracks.size() - 1;

    /*
    cout << "[CrossingPtBasedLinearizationPointFinder] we start: npairs=" << theNPairs
         << endl;
    cout << "[CrossingPtBasedLinearizationPointFinder] t_interval=" << t_interval << endl;
    cout << "[CrossingPtBasedLinearizationPointFinder goodtracks.size=" << goodtracks.size()
         << endl;*/

    // the 'direction' false: intervals will expand
    // true: intervals will shrink
    bool dir = false;

    while ( vgp.size() < ((unsigned int) (theNPairs)) )
    {
      reco::TransientTrack rt1 = goodtracks [ t_first ];
      reco::TransientTrack rt2 = goodtracks [ t_first + t_interval ];
      // cout << "Considering now: " << t_first << ", " << t_first+t_interval << endl;
      if ( useMatrix ) {
        PointAndDistance v ( theMatrix->crossingPoint ( rt1, rt2 ),
                             theMatrix->distance ( rt1, rt2 ) );
        vgp.push_back ( v );
      } else { // No DistanceMatrix available
        TwoTrackMinimumDistance ttmd;
        try {
          pair < GlobalPoint, GlobalPoint > pts = ttmd.points
            ( rt1.impactPointState(), rt2.impactPointState() );
          PointAndDistance v ( ( pts.second + pts.first ) / 2. ,
                             ( pts.second - pts.first ).mag() );
          vgp.push_back( v );
        }
        catch (...) { // If sth goes wrong, we just dont add. Who cares?
  //        cout << "[CrossingPtBasedLinearizationPointFinder] ttmd failed?" << endl;
        };
      };
      if ( ( t_first + t_interval ) < lim )
      {
        t_first++;
      } else if ( dir ) {
        t_first=0; t_interval--; if ( t_interval == 0 )
        {
          /* cout << "[CrossingPtBasedLinearizationPointFinder] t_interval=0. break." 
               << endl;*/
          break;
        };
      } else {
        t_first=0; t_interval++;
        if ( t_interval == goodtracks.size() )
        {
          /* cout << "[CrossingPtBasedLinearizationPointFinder] t_interval="
               << goodtracks.size() << "(max). start to decrease intervals"
               << endl; */
          dir=true; t_interval =  goodtracks.size() / 2 - 1;
        };
      };
    };
    if (! vgp.size() )
    {
      // no crossing points? Fallback to a crossingpoint-less lin pt finder!
      return FallbackLinearizationPointFinder().getLinearizationPoint ( tracks );
    };
    return find ( vgp );
  } catch (...) {};
  return GlobalPoint(0.,0.,0.); // if nothing else, then return 0,0,0
};
