#include <vector>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/BTauReco/interface/CombinedSVTagInfo.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/BTauReco/interface/TaggingVariable.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "RecoVertex/ConfigurableVertexReco/interface/ConfigurableVertexReconstructor.h"
#include "RecoVertex/AdaptiveVertexFit/interface/AdaptiveVertexFitter.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "RecoBTag/CombinedSV/interface/LikelihoodRatio.h"
#include "RecoBTag/CombinedSV/interface/NCategoriesDiscriminatorComputer.h"
#include "RecoBTag/CombinedSV/interface/FilterFactory.h"
#include "RecoBTag/CombinedSV/interface/CombinedSV.h"

using namespace std;
using namespace reco;
using namespace edm;

namespace {
  vector < reco::TaggingVariableName > noVars()
  {
    vector < reco::TaggingVariableName > ret;
    // NoVertex category has TrackIP2DSignificance only.
    ret.push_back ( reco::btau::vertexCategory );
    ret.push_back ( reco::btau::trackSip2dSig );
    return ret;
  }

  vector < reco::TaggingVariableName > pseudoVars()
  {
    vector < reco::TaggingVariableName > ret=noVars();
    // PseudoVertex category = NoVertex Variables plus the ones below
    ret.push_back ( reco::btau::vertexMass );
    ret.push_back ( reco::btau::vertexNTracks );
    ret.push_back ( reco::btau::vertexEnergyRatio );
    ret.push_back ( reco::btau::trackSip2dSigAboveCharm );
    ret.push_back ( reco::btau::trackEtaRel );
    return ret;
  }

  vector < reco::TaggingVariableName > recoVars()
  {
    vector < reco::TaggingVariableName > ret=pseudoVars();
    // RecoVertex category = PseudoVertex Variables plus flightdistance.
    ret.push_back ( reco::btau::flightDistance2dSig );
    return ret;
  }

  NCategoriesDiscriminatorComputer * getComputer ()
  {
    edm::FileInPath freco ( "RecoBTag/CombinedSV/data/reco.xml" );
    edm::FileInPath fpseudo ( "RecoBTag/CombinedSV/data/pseudo.xml" );
    edm::FileInPath fno ( "RecoBTag/CombinedSV/data/no.xml" );
    BTagAlgorithmCalibration * creco = new BTagAlgorithmCalibration ( freco.fullPath().c_str() );
    BTagAlgorithmCalibration * cpseudo = new BTagAlgorithmCalibration ( fpseudo.fullPath().c_str() );
    BTagAlgorithmCalibration * cno = new BTagAlgorithmCalibration ( fno.fullPath().c_str() );
    // edm::LogWarning("") << "FIXME need to fix AlgorithmCalibration ownership!";
    LikelihoodRatio * recovertex = new LikelihoodRatio ( creco, reco::btag::Vertices::RecoVertex );
    LikelihoodRatio * pseudovertex = new LikelihoodRatio ( cpseudo, reco::btag::Vertices::PseudoVertex );
    LikelihoodRatio * novertex = new LikelihoodRatio ( cno, reco::btag::Vertices::NoVertex );
    return new NCategoriesDiscriminatorComputer ( recovertex, pseudovertex, novertex );
  }
}

reco::Vertex CombinedSV::getPrimaryVertex ( const edm::Event & iEvent,
                                const edm::EventSetup& iSetup ) const
{
  // get primary vertex for this event
  // -> if multiple primary vertices found, take the one with highest pt**2
  //    (most likely the hard interaction)
  //    ==>> are they still pt**2 ordered, i.e. can we take the first one?
  //         Assume this for now
  // -> if no primary vertex found, take beamspot constraint
  //    build "fake" primary vertex
  try {
    edm::Handle<reco::VertexCollection> retColl;
    iEvent.getByLabel( vertexcoll_, retColl);
    int numVertices = retColl->size();
    if (numVertices > 0) {
      LogDebug("") << "Persistent primary vertex found, will use it";
      return *(retColl->begin());
    }

  } // try PrimaryVertex
  catch( edm::Exception & e ) {
    LogDebug("") << e.what() << " caught.";
  };

  vector<reco::TransientTrack> ttks;

  try {
    edm::Handle<reco::TrackCollection> tks;
    iEvent.getByLabel( trackcoll_, tks );
    AdaptiveVertexFitter fitter;
    edm::ESHandle<TransientTrackBuilder> builder;
    iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", builder );
    ttks = builder->build(tks);
    edm::LogWarning("CombinedSV::getPrimaryVertex") << "No primary vertex found, fitting all "
                        << tks->size() << " ttracks with avf";
    TransientVertex ret = fitter.vertex ( ttks );
    LogDebug ("") << "fitted vertex has weight map: " << ret.hasTrackWeight();
    return static_cast < reco::Vertex > (ret);
  } catch ( edm::Exception & e ) {
    LogDebug("") << e.what() << " caught.";
  };

  LogDebug ("") << "No primary vertex found, use geometric origin (beamspot)";
  /*
  BeamSpot s;
  TransientVertex vtx ( s.position(), s.error(), ttks, -1. );
  return static_cast < reco::Vertex > ( vtx );
  */
  reco::BeamSpot s;
  math::XYZPoint p ( s.position().x(), s.position().y(), s.position().z() );
  return reco::Vertex ( s.position(), s.covariance3D(), -1., -1., 0 );
}



void CombinedSV::instantiateAlgorithm( const edm::ParameterSet & iconfig )
{
  edm::ParameterSet vtxconfig = iconfig.getParameter<edm::ParameterSet>("vertexreco");
  ConfigurableVertexReconstructor vrec ( vtxconfig );
  NCategoriesDiscriminatorComputer * ncat = getComputer();
  CombinedSVTaggingVariables vars ( recoVars(), pseudoVars(), noVars() );
  double ipsig2d =
    iconfig.getParameter<edm::ParameterSet>("trackcuts").getParameter<double>("IpSignificanceMin2DMin");
  double charmcut =
    iconfig.getParameter<edm::ParameterSet>("vertexcuts").getParameter<double>("CharmCut");
  FilterFactory filters ( iconfig, 0 );
  algorithm_ = new combsv::CombinedSVAlgorithm ( 0, vrec, ncat, vars, filters, charmcut, ipsig2d );
}

CombinedSV::CombinedSV(const edm::ParameterSet& iconfig) : algorithm_ ( 0 ),
  vertexcoll_( iconfig.getParameter<string>("vertexcoll") ),
  trackcoll_( iconfig.getParameter<string>("trackcoll") ),
  associatorID_ ( iconfig.getParameter<string>("jetTracks") )
{
  instantiateAlgorithm ( iconfig );
  produces<reco::JetTagCollection>(); // produces the general tagging info .../
  produces<reco::CombinedSVTagInfoCollection>(); /// ... as well as special combined btagging info
}

CombinedSV::~CombinedSV() {
  if ( algorithm_ ) delete algorithm_;
}

void CombinedSV::produce(edm::Event& iEvent,
                           const edm::EventSetup& iSetup)
{
  int evt=iEvent.id().event();
  LogDebug("") << "next event: " << evt;
  edm::ESHandle<MagneticField> magneticField;
  iSetup.get<IdealMagneticFieldRecord>().get(magneticField);
  edm::ESHandle<TransientTrackBuilder> builder;
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",builder );

  algorithm_->setMagneticField ( &( *magneticField) );
  algorithm_->setTransientTrackBuilder ( &( *builder ) );

  std::auto_ptr<reco::JetTagCollection> baseCollection;
  std::auto_ptr<reco::CombinedSVTagInfoCollection> extCollection(new reco::CombinedSVTagInfoCollection());

  reco::Vertex primaryVertex = getPrimaryVertex ( iEvent, iSetup );
  LogDebug ("") << "primary vertex is at " << primaryVertex.position();

  edm::Handle<reco::JetTracksAssociationCollection> jetWithTrackColl;

  // obtain Jet with track associations from event,
  // loop over collection and run b-tagging
  try {
    iEvent.getByLabel(associatorID_, jetWithTrackColl);

    if (jetWithTrackColl.product()->size()) {
       baseCollection = std::auto_ptr<reco::JetTagCollection>(new JetTagCollection(jetWithTrackColl->keyProduct()));
    } else
       baseCollection = std::auto_ptr<reco::JetTagCollection>(new JetTagCollection());

    // int numJets = jetWithTrackColl->size();
    LogDebug("") << "need to analyze " << jetWithTrackColl->size() << " jets.";
    
    for ( reco::JetTracksAssociationCollection::const_iterator jetNTrks =
          jetWithTrackColl->begin(); jetNTrks != jetWithTrackColl->end(); jetNTrks++)
    {
      // get jet part from jet with tracks association
      // int i=jetNTrks->first.index();

      /*
      const edm::Ref<reco::JetTracksAssociationCollection> & jta =
        edm::Ref<reco::JetTracksAssociationCollection>(jetWithTrackColl,i);
        */

      /*
      edm::LogInfo("") << endl << endl
           << "[CombinedSV]     ---<  now analyzing jet #" << i << "  >---";
           */

      LogDebug ("") << "found " << jetNTrks->second.size() << " tracks in jet "
                    << jetNTrks - jetWithTrackColl->begin() << " pt="
                    << jetNTrks->first->pt() << " p=" << jetNTrks->first->p() << " eta="
                    << jetNTrks->first->eta();
      vector < reco::TransientTrack > trks;
      edm::ESHandle<TransientTrackBuilder> builder;
      iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", builder );
      for ( edm::RefVector < reco::TrackCollection >::const_iterator i=jetNTrks->second.begin();
            i!=jetNTrks->second.end() ; ++i )
      {
        trks.push_back ( builder->build ( *i ) );
      }
      LogDebug("") << "built " << trks.size() << " TransientTracks.";

      unsigned int index = jetNTrks - jetWithTrackColl->begin();
      reco::CombinedSVTagInfo btag = algorithm_->tag( primaryVertex, *( jetNTrks->first ), trks,
          		edm::Ref<JetTracksAssociationCollection>(jetWithTrackColl, index) );
      LogDebug("") << "jet " << index << " tag value: " << btag.discriminator();
      (*baseCollection)[jetNTrks->first]=btag.discriminator();
      extCollection->push_back(btag);
    }
  } catch ( edm::Exception & e ) {
    LogDebug("") << "EDM exception caught: " << e.what() << ", ignore this event";
  } catch ( cms::Exception & e ) {
    LogDebug("") << "CMS exception caught: " << e.what() << ", ignore this event";
  } catch ( std::exception & e ) {
    LogDebug("") << "std::exception caught: " << e.what() << ", ignore this event";
  }

  if (!baseCollection.get())
    baseCollection = std::auto_ptr<reco::JetTagCollection>(new JetTagCollection());

  iEvent.put(extCollection);
  iEvent.put(baseCollection);
}

//define this as a plug-in
DEFINE_FWK_MODULE(CombinedSV);
