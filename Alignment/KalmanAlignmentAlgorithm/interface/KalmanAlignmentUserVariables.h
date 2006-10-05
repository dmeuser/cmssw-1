
#ifndef Alignment_KalmanAlignmentAlgorithm_KalmanAlignmentUserVariables_h
#define Alignment_KalmanAlignmentAlgorithm_KalmanAlignmentUserVariables_h

#include "Alignment/CommonAlignment/interface/AlignmentUserVariables.h"
#include "Alignment/CommonAlignment/interface/Alignable.h"
#include "Alignment/TrackerAlignment/interface/TrackerAlignableId.h"

#include <string>

using namespace std;

/// User variables used by the KalmanAlignmentAlgorithm. The evolution of the estimated alignment
/// parameters is stored in graphs using the DataCollector.

class KalmanAlignmentUserVariables : public AlignmentUserVariables
{

public:

  /// Create new user variables by specifying the associated Alignable, the Alignable's Id and how
  /// often the evolution of the estimated parameters should be updated.
  KalmanAlignmentUserVariables( Alignable* parent,
				TrackerAlignableId* alignableId = 0,
				int frequency = 100 );

  KalmanAlignmentUserVariables( void ) :
    theParentAlignable( 0 ),
    theNumberOfHits( 0 ),
    theNumberOfUpdates( 0 ),
    theUpdateFrequency( 0 ),
    theFirstUpdate( true ) {}

  virtual ~KalmanAlignmentUserVariables( void ) {}

  virtual KalmanAlignmentUserVariables* clone( void ) const { return new KalmanAlignmentUserVariables( *this ); }

  /// Return the number of hits.
  int numberOfHits( void ) { return theNumberOfHits; }
  /// Call this function in case the associated Alignable was hit by a particle.
  void hit( void ) { theNumberOfHits++; }

  /// Return the number of updates.
  int numberOfUpdates( void ) { return theNumberOfUpdates; }
  /// Call this function in case the associated Alignable was updated by the alignment algorithm.
  void update( void );

protected:

  const AlgebraicVector extractTrueParameters( void ) const;

  const string selectedParameter( const int& selected ) const;
  const float selectedScaling( const int& selected ) const;

  const string toString( const int& i ) const;

  Alignable* theParentAlignable;

  int theNumberOfHits;
  int theNumberOfUpdates;
  int theUpdateFrequency;

  bool theFirstUpdate;

  string theIdentifier;
};


#endif
