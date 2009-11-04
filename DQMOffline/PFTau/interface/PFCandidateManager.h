#ifndef RecoParticleFlow_Benchmark_BenchmarkManager_h
#define RecoParticleFlow_Benchmark_BenchmarkManager_h

#include "DQMOffline/PFTau/interface/Benchmark.h"
#include "DQMOffline/PFTau/interface/CandidateBenchmark.h"
#include "DQMOffline/PFTau/interface/PFCandidateBenchmark.h"
#include "DQMOffline/PFTau/interface/MatchCandidateBenchmark.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include <vector>

/// A benchmark managing several benchmarks
class PFCandidateManager : public Benchmark {

 public:

  PFCandidateManager( float dRMax = 0.3,
		      float ptMin = 2,
		      bool matchCharge = true, 
		      Benchmark::Mode mode=Benchmark::DEFAULT) 
    : 
    Benchmark(mode), 
    candBench_(mode), pfCandBench_(mode), matchCandBench_(mode), 
    dRMax_(dRMax), ptMin_(ptMin), matchCharge_(matchCharge) {}

  virtual ~PFCandidateManager();
  
  /// set the benchmark parameters
  void setParameters( float dRMax = 0.3,
		      float ptMin = 2, 
		      bool matchCharge = true, 
		      Benchmark::Mode mode=Benchmark::DEFAULT );
  
  /// set directory (to use in ROOT)
  void setDirectory(TDirectory* dir);

  /// book histograms
  void setup();
  
  /// fill histograms with all particle
  template< class C>
  void fill(const reco::PFCandidateCollection& candCollection,
	    const C& matchedCandCollection );

 protected:
  CandidateBenchmark      candBench_;
  PFCandidateBenchmark    pfCandBench_;
  MatchCandidateBenchmark matchCandBench_;

  float dRMax_;
  float ptMin_;
  bool  matchCharge_;

};


#include "DQMOffline/PFTau/interface/Matchers.h"

template< class C>
void PFCandidateManager::fill(const reco::PFCandidateCollection& candCollection,
			      const C& matchCandCollection) {
  

  std::vector<int> matchIndices;
  PFB::match( candCollection, matchCandCollection, matchIndices, 
	      matchCharge_, dRMax_ );

  for (unsigned int i = 0; i < candCollection.size(); i++) {
    const reco::PFCandidate& cand = candCollection[i];

    if( cand.pt() < ptMin_ ) break;
    
    int iMatch = matchIndices[i];

    assert(iMatch< static_cast<int>(matchCandCollection.size()));
 
    //COLIN how to handle efficiency plots?

    // filling the histograms in CandidateBenchmark only in case 
    // of a matching. 
    if( iMatch!=-1 ) {
      candBench_.fillOne(cand);
      pfCandBench_.fillOne(cand);
      matchCandBench_.fillOne(cand, matchCandCollection[ iMatch ]);
    }
  }
}

 

#endif 
