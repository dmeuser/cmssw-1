#ifndef RecoMuon_SeedGenerator_RPCSeedLayerFinder_H
#define RecoMuon_SeedGenerator_RPCSeedLayerFinder_H

/** \class RPCSeedLayerFinder
 *  
 *   \author Haiyun.Teng - Peking University
 *
 *  
 */


#include <RecoMuon/TransientTrackingRecHit/interface/MuonTransientTrackingRecHit.h>
#include "RecoMuon/SeedGenerator/src/RPCSeedrecHitFinder.h"
#include <FWCore/ParameterSet/interface/ParameterSet.h>


#ifndef RPCLayerNumber
#define RPCLayerNumber 12
#endif

#ifndef BarrelLayerNumber
#define BarrelLayerNumber 6
#endif

#ifndef EachEndcapLayerNumber
#define EachEndcapLayerNumber 3
#endif


class RPCSeedLayerFinder {

    typedef MuonTransientTrackingRecHit::MuonRecHitPointer MuonRecHitPointer;
    typedef MuonTransientTrackingRecHit::ConstMuonRecHitPointer ConstMuonRecHitPointer;
    typedef MuonTransientTrackingRecHit::MuonRecHitContainer MuonRecHitContainer;
    typedef MuonTransientTrackingRecHit::ConstMuonRecHitContainer ConstMuonRecHitContainer;

    public:
        RPCSeedLayerFinder();
        ~RPCSeedLayerFinder();
        void configure(const edm::ParameterSet& iConfig);
        void setInput(MuonRecHitContainer (&recHitsRPC)[RPCLayerNumber]);
        void unsetInput();
        void setOutput(RPCSeedrecHitFinder* Ref);
        void fillLayers();

    private:
        // create special N layers to fill to seeds
        void SpecialLayers(int last, unsigned int NumberofLayers, int type);
        bool checkConstrain();

        // ----------member data ---------------------------

        // The ref of RPCSeedrecHitFinder which will be call after gathering a set of layers 
        RPCSeedrecHitFinder* RPCrecHitFinderRef;
        // The parameters for configuration
        bool isMixBarrelwithEndcap;
        std::vector<unsigned int> RangeofLayersinBarrel;
        std::vector<unsigned int> RangeofLayersinEndcap;
        bool isSpecialLayers;
        std::vector<unsigned int> LayersinEndcap;
        std::vector<unsigned int> LayersinBarrel;
        std::vector<unsigned int> constrainedLayersinBarrel;
        // Signal for call fillLayers()
        bool isConfigured;
        bool isInputset;
        bool isOutputset;
        // Enable layers in Barrel and Endcap
        std::vector<unsigned int> LayersinRPC;
        // Information of recHits in each layer
        unsigned int recHitsinLayers[RPCLayerNumber];
};

#endif
