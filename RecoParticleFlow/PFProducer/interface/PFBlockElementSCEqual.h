#ifndef RecoParticleFlow_PFProducer_PFBlockElementSCEqual
#define RecoParticleFlow_PFProducer_PFBlockElementSCEqual

#include "DataFormats/ParticleFlowReco/interface/PFBlockElementSuperCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElement.h"
//#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"

class PFBlockElementSCEqual {
 public:
  PFBlockElementSCEqual(reco::SuperClusterRef scRef):ref_(scRef) {;}
    ~PFBlockElementSCEqual(){;}
    inline bool operator() (const reco::PFBlockElement* el) {
      return (el->type()==reco::PFBlockElement::SC && (dynamic_cast<const reco::PFBlockElementSuperCluster*>(el))->superClusterRef()==ref_);
    }
 private:
    reco::SuperClusterRef ref_;
};

#endif


