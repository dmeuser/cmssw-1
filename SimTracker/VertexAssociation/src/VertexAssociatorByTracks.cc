#include "DataFormats/Common/interface/EDProduct.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/EncodedEventId/interface/EncodedEventId.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertexContainer.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertex.h"

#include "SimTracker/TrackAssociation/interface/TrackAssociatorBase.h"
#include "SimTracker/VertexAssociation/interface/VertexAssociatorByTracks.h"

#include "CommonTools/Statistics/interface/ChiSquaredProbability.h"


/* Constructor */
VertexAssociatorByTracks::VertexAssociatorByTracks (const edm::ParameterSet & config) : config_(config)
{
    R2SMatchedSimRatio_ = config.getParameter<double>("R2SMatchedSimRatio");
    R2SMatchedRecoRatio_ = config.getParameter<double>("R2SMatchedRecoRatio");
    S2RMatchedSimRatio_ = config.getParameter<double>("S2RMatchedSimRatio");
    S2RMatchedRecoRatio_ = config.getParameter<double>("S2RMatchedRecoRatio");

    std::string trackQualityType = config.getParameter<std::string>("trackQuality");
    trackQuality_ = reco::TrackBase::qualityByName(trackQualityType);

    edm::ParameterSet param = config.getParameter<edm::ParameterSet>("trackingParticleSelector");

    selector_ = TrackingParticleSelector(
                    param.getParameter<double>("ptMinTP"),
                    param.getParameter<double>("minRapidityTP"),
                    param.getParameter<double>("maxRapidityTP"),
                    param.getParameter<double>("tipTP"),
                    param.getParameter<double>("lipTP"),
                    param.getParameter<int>("minHitTP"),
                    param.getParameter<bool>("signalOnlyTP"),
                    param.getParameter<bool>("chargedOnlyTP"),
                    param.getParameter<std::vector<int> >("pdgIdTP")
                );
}


/* Destructor */
VertexAssociatorByTracks::~VertexAssociatorByTracks()
{
    //do cleanup here
}


reco::VertexRecoToSimCollection VertexAssociatorByTracks::associateRecoToSim(
    edm::Handle<reco::VertexCollection> & recoVertexes,
    edm::Handle<TrackingVertexCollection> & trackingVertexes,
    const edm::Event& event,
    reco::RecoToSimCollection & associator
) const
{
    reco::VertexRecoToSimCollection  outputCollection;

    std::map<TrackingVertexRef,std::vector<reco::TrackBaseRef> > matches;

    std::cout << "reco::VertexCollection size = " << recoVertexes->size();
    std::cout << " ; TrackingVertexCollection size = " << trackingVertexes->size() << std::endl << std::endl;

    // Loop over RecoVertex
    for (std::size_t recoIndex = 0; recoIndex < recoVertexes->size(); ++recoIndex)
    {
        reco::VertexRef recoVertex(recoVertexes, recoIndex);

        matches.clear();

        std::size_t recoDaughterCounter = 0;

        // Loop over daughter tracks of RecoVertex
        for (
            reco::Vertex::trackRef_iterator recoDaughter = recoVertex->tracks_begin();
            recoDaughter != recoVertex->tracks_end();
            ++recoDaughter
        )
        {
            // Check the quality of the RecoDoughters
            if ( !(*recoDaughter)->quality(trackQuality_) ) continue;

            // Check for association for the given RecoDaughter
            if ( associator.numberOfAssociations(*recoDaughter) > 0 )
            {
                std::vector<std::pair<TrackingParticleRef, double> > associations = associator[*recoDaughter];

                // Loop over TrackingParticles associated with RecoDaughter
                for (
                    std::vector<std::pair<TrackingParticleRef, double> >::const_iterator association = associations.begin();
                    association != associations.end();
                    ++association
                )
                {
                    // Get a reference to parent vertex of TrackingParticle associated to the RecoDaughter
                    TrackingVertexRef trackingVertex = association->first->parentVertex();
                    // Store matched RecoDaughter to the trackingVertex
                    matches[trackingVertex].push_back(*recoDaughter);
                }
            }

            recoDaughterCounter++;
        } //loop over daughter tracks of RecoVertex

        std::size_t assoIndex = 0;

        // Loop over map between TrackingVertexes and matched RecoDaugther
        for (
            std::map<TrackingVertexRef,std::vector<reco::TrackBaseRef> >::const_iterator match = matches.begin();
            match != matches.end();
            ++match
        )
        {
            TrackingVertexRef trackingVertex = match->first;
            std::size_t matchedDaughterCounter = match->second.size();

            std::size_t simDaughterCounter = 0;

            // Count for only reconstructible SimDaughters
            for (
                TrackingVertex::tp_iterator simDaughter = trackingVertex->daughterTracks_begin();
                simDaughter != trackingVertex->daughterTracks_end();
                ++simDaughter
            )
                if ( selector_(**simDaughter) ) simDaughterCounter++;

            // Sanity condition in case that reconstructable condition is too tight
            if ( simDaughterCounter < matchedDaughterCounter )
                simDaughterCounter = matchedDaughterCounter;

            // Condition over S2RMatchedSimRatio
            if ( (double)matchedDaughterCounter/simDaughterCounter < R2SMatchedSimRatio_ ) continue;

            double quality = (double)matchedDaughterCounter/recoDaughterCounter;

            // Condition over R2SMatchedRecoRatio
            if (quality < R2SMatchedRecoRatio_) continue;

            outputCollection.insert(recoVertex, std::make_pair(trackingVertex, quality));

            std::cout << "R2S: INDEX " << assoIndex;
            std::cout << " ; n = " << simDaughterCounter << " ; m = " << recoDaughterCounter << " ; k = " << matchedDaughterCounter;
            std::cout << " ; quality = " << quality << std::endl;

            assoIndex++;
        }
    } // Loop on RecoVertex

    std::cout << std::endl;
    std::cout << "RecoToSim OUTPUT COLLECTION: outputCollection.size() = " << outputCollection.size() << std::endl << std::endl;

    return outputCollection;
}



reco::VertexSimToRecoCollection VertexAssociatorByTracks::associateSimToReco(
    edm::Handle<reco::VertexCollection> & recoVertexes,
    edm::Handle<TrackingVertexCollection> & trackingVertexes,
    const edm::Event& event,
    reco::SimToRecoCollection & associator
) const
{
    reco::VertexSimToRecoCollection  outputCollection;

    // Loop over TrackingVertexes
    std::map<reco::VertexRef,std::vector<reco::TrackBaseRef> > matches;

    // Loop over TrackingVertexes
    for (std::size_t simIndex = 0; simIndex < trackingVertexes->size(); ++simIndex)
    {
        TrackingVertexRef trackingVertex(trackingVertexes, simIndex);

        matches.clear();

        std::size_t simDaughterCounter = 0;

        // Loop over daughter tracks of TrackingVertex
        for (
            TrackingVertex::tp_iterator simDaughter = trackingVertex->daughterTracks_begin();
            simDaughter != trackingVertex->daughterTracks_end();
            ++simDaughter
        )
        {
            // Select only reconstructible SimDaughters
            if ( !selector_(**simDaughter) ) continue;

            // Check for association for the given RecoDaughter
            if ( associator.numberOfAssociations(*simDaughter) > 0 )
            {
                std::vector<std::pair<reco::TrackBaseRef, double> > associations = associator[*simDaughter];

                // Loop over RecoTracks associated with TrackingParticle
                for (
                    std::vector<std::pair<reco::TrackBaseRef,double> >::const_iterator association = associations.begin();
                    association != associations.end();
                    ++association
                )
                {
                    reco::TrackBaseRef recoTrack = association->first;

                    for (std::size_t recoIndex = 0; recoIndex < recoVertexes->size(); ++recoIndex)
                    {
                        reco::VertexRef recoVertex(recoVertexes, recoIndex);

                        for (
                            reco::Vertex::trackRef_iterator recoDaughter = recoVertex->tracks_begin();
                            recoDaughter != recoVertex->tracks_end();
                            ++recoDaughter
                        )
                        {
                            // Store matched RecoDaughter to the RecoVertex
                            if (
                                recoDaughter->id() == recoTrack.id() &&
                                recoDaughter->key() == recoTrack.key()
                            )
                                matches[recoVertex].push_back(recoTrack);
                        }

                    }
                } // loop a recotracks
            }
            simDaughterCounter++;
        } // loop a simDaughter

        std::size_t assoIndex = 0;

        // Loop over map, set score, add to outputCollection
        for (
            std::map<reco::VertexRef,std::vector<reco::TrackBaseRef> >::const_iterator match = matches.begin();
            match != matches.end();
            ++match
        )
        {
            reco::VertexRef recoVertex = match->first;
            std::size_t matchedDaughterCounter = match->second.size();

            std::size_t recoDaughterCounter = 0;

            // Count those tracks with a given quality of the RecoDoughters
            for (
                reco::Vertex::trackRef_iterator recoDaughter = recoVertex->tracks_begin();
                recoDaughter != recoVertex->tracks_end();
                ++recoDaughter
            )
                if ( (*recoDaughter)->quality(trackQuality_) ) recoDaughterCounter++;

            // Sanity condition in case that track quality condition is too tight
            if ( recoDaughterCounter < matchedDaughterCounter )
                recoDaughterCounter = matchedDaughterCounter;

            // Condition over S2RMatchedRecoRatio
            if ( (double)matchedDaughterCounter/recoDaughterCounter < S2RMatchedRecoRatio_ ) continue;

            double quality = (double)matchedDaughterCounter/simDaughterCounter;

            // Condition over S2RMatchedSimRatio
            if (quality < S2RMatchedSimRatio_) continue;

            outputCollection.insert(trackingVertex, std::make_pair(recoVertex, quality));

            std::cout << "R2S: INDEX " << assoIndex;
            std::cout << " ; n = " << simDaughterCounter << " ; m = " << recoDaughterCounter << " ; k = " << matchedDaughterCounter;
            std::cout << " ; quality = " << quality << std::endl;

            assoIndex++;
        }
    } // loop over TrackingVertexes

    std::cout << "SimToReco OUTPUT COLLECTION: outputCollection.size() = " << outputCollection.size() << std::endl << std::endl;

    return outputCollection;
}

