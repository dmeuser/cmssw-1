#include "treeReader01.hh"

#include "TRandom.h"

#include "treeReader01.icc"


// ----------------------------------------------------------------------
// Run with: ./runTreeReader01 -c chains/bg-test -D root 
//           ./runTreeReader01 -f test.root 
// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
void treeReader01::startAnalysis() {
  cout << "startAnalysis: ..." << endl;
}



// ----------------------------------------------------------------------
void treeReader01::eventProcessing() {
  TAnaTrack *pTrack;
  TAnaCand *pCand;

  fRun = fpEvt->fRunNumber;

  cout << "----------------------------------------------------------------------" << endl;
  cout << "Found " << fpEvt->nGenCands() << " gen cands in event" << endl;
  cout << "Found " << fpEvt->nSigTracks() << " sig tracks in event" << endl;
  cout << "Found " << fpEvt->nRecTracks() << " rec tracks in event" << endl;
  ((TH1D*)fpHistFile->Get("h1"))->Fill(fpEvt->nRecTracks()); 
  
  cout << "------------------------------" << endl;
  ((TH1D*)fpHistFile->Get("h20"))->Fill(fpEvt->nRecTracks());
  for (int it = 0; it < fpEvt->nRecTracks(); ++it) {
    pTrack = fpEvt->getRecTrack(it);
    ((TH1D*)fpHistFile->Get("h10"))->Fill(pTrack->fPlab.Perp()); 
    cout << "R: "; pTrack->dump(); 
  }


  cout << "------------------------------" << endl;
  for (int it = 0; it < fpEvt->nCands(); ++it) {
    pCand = fpEvt->getCand(it);
    cout << "C: " << pCand->fType << " "; pCand->dump(); 
    ((TH1D*)fpHistFile->Get("h100"))->Fill(pCand->fMass); 
  }

  fpHistFile->cd();
  fillHist(); 
  fTree->Fill();

}


// ----------------------------------------------------------------------
void treeReader01::fillHist() {


}

// ----------------------------------------------------------------------
void treeReader01::bookHist() {
  TH1 *h;
  cout << "-->bookHist> " << endl;

  h = new TH1D("h1", "nTrk", 40, 0., 40.);
  h = new TH1D("h10", "pT", 40, 0., 20.);
  h = new TH1D("h20", "ntrk", 20, 0, 20.);

  h = new TH1D("h100", "m", 40, 2.8, 3.4);

  // -- Reduced Tree
  fTree = new TTree("events", "events");
  fTree->Branch("run",    &fRun ,"run/I");

}


