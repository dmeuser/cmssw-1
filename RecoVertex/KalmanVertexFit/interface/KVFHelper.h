#ifndef KVFHelper_H
#define KVFHelper_H

#include "RecoVertex/VertexPrimitives/interface/CachingVertex.h"

  /**
   * Helper class to calculate chi2 increments for the Kalman Filter
   */

class KVFHelper {

public:

  /**
   *  Methode which calculates the chi**2-increment due to the vertices
   *  E.g. between the prior and the fitted vertex.
   *  The covariance matrix used is that of the first vertex (vertexA).
   *  This method will not take into account multiple states, so in case one of
   *  the VertexStates is a multi-state vertex, only the mean will be used.
   *  \param vertexA: The prior vertex state
   *  \param VertexB: The fitted vertex state
   */
  double vertexChi2(const VertexState & vertexA,
	const VertexState & VertexB) const;

  double vertexChi2(const CachingVertex & vertexA,
	const CachingVertex & vertexB) const;

  /**
   *  Methode which calculates the chi**2 between the prior and the fitted
   *   track parameters.
   *  \param linTrack	The track as linearized
   *  \param refittedTrackState The refitted track
   */
  float trackParameterChi2(const RefCountedLinearizedTrackState linTrack,
	const RefCountedRefittedTrackState refittedTrackState) const;

  /**
   *  Methode which calculates the chi**2 between the prior and the fitted
   *   track parameters. The track must have been smoothed before calling this method.
   *   If not, an Exception will be thrown.
   */
  float trackParameterChi2(const RefCountedVertexTrack track) const;

};

#endif
