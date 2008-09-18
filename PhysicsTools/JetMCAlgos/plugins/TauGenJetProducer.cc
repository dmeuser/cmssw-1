#include "PhysicsTools/JetMCAlgos/plugins/TauGenJetProducer.h"


#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/Common/interface/RefToPtr.h"

#include "PhysicsTools/HepMCCandAlgos/interface/GenParticlesHelper.h"


using namespace std;
using namespace edm;
using namespace reco;

TauGenJetProducer::TauGenJetProducer(const edm::ParameterSet& iConfig) {
  

  inputTagGenParticles_ 
    = iConfig.getParameter<InputTag>("GenParticles");

  includeNeutrinos_ 
    = iConfig.getParameter<bool>("includeNeutrinos");

  verbose_ = 
    iConfig.getUntrackedParameter<bool>("verbose",false);



  produces<GenJetCollection>();
}



TauGenJetProducer::~TauGenJetProducer() { }



void TauGenJetProducer::beginJob(const edm::EventSetup & es) { }


void TauGenJetProducer::produce(Event& iEvent, 
				const EventSetup& iSetup) {
  
  Handle<GenParticleCollection> genParticles;

  bool found = iEvent.getByLabel( inputTagGenParticles_, genParticles);
  
  if(!found ) {
    std::ostringstream  err;
    err<<" cannot get collection: "
       <<inputTagGenParticles_<<std::endl;
    edm::LogError("TauGenJetProducer")<<err.str();
    throw cms::Exception( "MissingProduct", err.str());
  }

  std::auto_ptr<GenJetCollection> 
    pOutVisTaus(new GenJetCollection() );


  using namespace GenParticlesHelper;

  GenParticleRefVector allStatus2Taus;  
  findParticles( *genParticles,
		 allStatus2Taus, 15, 2);

  for( IGR iTau=allStatus2Taus.begin(); iTau!=allStatus2Taus.end(); ++iTau) {

    // look for all status 1 (stable) descendents 
    GenParticleRefVector descendents;
    findDescendents( *iTau, descendents, 1);
    
    // loop on descendents, and take all except neutrinos
    math::XYZTLorentzVector sumVisMom;
    Particle::Charge charge = 0;
    Jet::Constituents constituents;

    if(verbose_)
      cout<<"tau "<<(*iTau)<<endl;


    for(IGR igr = descendents.begin(); 
	igr!= descendents.end(); ++igr ) {
      
      int absPdgId = abs(  (*igr)->pdgId());
      
      // neutrinos
      if(!includeNeutrinos_ ) {
	if( absPdgId == 12 || 
	    absPdgId == 14 || 
	    absPdgId == 16  ) 
	  continue;
      }      

      if(verbose_) 
	cout<<"\t"<<(*igr)<<endl;
      
      charge += (*igr)->charge();
      sumVisMom += (*igr)->p4();
      
      // need to convert the vector of reference to the constituents
      // to a vector of pointers to build the genjet
      constituents.push_back( refToPtr( *igr) );
    }

    math::XYZPoint vertex;
    GenJet::Specific specific;
    
    GenJet jet( sumVisMom, vertex, specific, constituents); 
    pOutVisTaus->push_back( jet );
    
  }
  iEvent.put( pOutVisTaus );
}


DEFINE_FWK_MODULE( TauGenJetProducer );
