#include "RecoBTag/TrackProbability/interface/TrackClassFilter.h" 
#include <iostream>

using namespace std;

bool TrackClassFilter::apply(const reco::Track & track, const reco::Jet & jet, const reco::Vertex & pv) const {

  double p=track.p();
  double eta=track.eta();
  double nhit=track.numberOfValidHits();
  double npix=track.hitPattern().numberOfValidPixelHits();
  double chi=track.normalizedChi2();
  bool chicut=(chi >= chiMin        &&       chi < chiMax ); 
  if(chiMin<=0.01 && chiMax<=0.01) chicut=true;
  bool result=(       p >  pMin       &&         p <  pMax       && 
           fabs(eta) >  etaMin     &&  fabs(eta) <  etaMax     &&
               nhit >= nHitsMin      &&      nhit <= nHitsMax      &&
	       npix >= nPixelHitsMin &&      npix <= nPixelHitsMax &&
	        chicut );
//  dump();
//  cout << "TRACK: p " << " eta " <<   eta << " #hit " << nhit << " #pix " << npix << " chi " << chi << "                         matched ?";
//  cout << result << endl;
  return result;
}

void TrackClassFilter::dump() const {

  cout<<"TrackClassFilter: "<<endl;
  cout<< pMin      <<" < P(GeV) < "                 <<pMax        <<endl;
  cout<< etaMin    <<" < |eta| < "                  <<etaMax      <<endl;
  cout<< nPixelHitsMin<<" =< number of Pixel Hits =< " << nPixelHitsMax <<endl;
  cout<< nHitsMin     <<" =< total number of hits  =< "<< nHitsMax      <<endl;
  cout<< chiMin       <<" =< chiSquare /dof  < "<< chiMax      <<endl;

}

