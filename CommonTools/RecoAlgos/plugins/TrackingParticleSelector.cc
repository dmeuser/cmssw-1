/** \class reco::modules::TrackingParticleSelector
 *
 *  Filter to select TrackingParticles according to pt, rapidity, tip, lip, number of hits, pdgId
 *
 *  \author Giuseppe Cerati, INFN
 *
 *  $Date: 2007/11/13 10:46:05 $
 *  $Revision: 1.2 $
 */

#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/RecoAlgos/interface/TrackingParticleSelector.h"

namespace reco {
  namespace modules {
    typedef SingleObjectSelector<TrackingParticleCollection,::TrackingParticleSelector> 
    TrackingParticleSelector ;

    DEFINE_ANOTHER_FWK_MODULE( TrackingParticleSelector );
  }
}
