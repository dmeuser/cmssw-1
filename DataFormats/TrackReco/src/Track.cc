#include "DataFormats/TrackReco/interface/Track.h"

using namespace reco;

Track::Track( double chi2, double ndof, const Point & vertex, const Vector & momentum, int charge,
	      const CovarianceMatrix & cov) :
  TrackBase( chi2, ndof, vertex, momentum, charge, cov ) {
}

Track::~Track() {
}
