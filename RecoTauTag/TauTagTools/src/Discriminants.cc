#include "RecoTauTag/TauTagTools/interface/Discriminants.h"

namespace PFTauDiscriminants {
using namespace std;

typedef reco::Particle::LorentzVector LorentzVector;

void
DecayMode::doComputation(PFTauDiscriminantManager* input, vector<int>& result)
{
   // convert to int for TTree
   result.push_back(static_cast<int>(input->getDecayMode()->getDecayMode()));
}

void
OutlierNCharged::doComputation(PFTauDiscriminantManager* input, vector<int>& result)
{
   size_t output = 0;
   const vector<const reco::Candidate*>& theOutlierObjects = input->outlierObjectsSortedByPt();
   for(vector<const reco::Candidate*>::const_iterator iObject  = theOutlierObjects.begin();
         iObject != theOutlierObjects.end();
         ++iObject)
   {
      const reco::Candidate* currentObject = *iObject;
      if (currentObject && currentObject->charge() != 0 )
         output++;
   }
   // convert to int for TTree
   result.push_back(static_cast<int>(output));
}

void
Pt::doComputation(PFTauDiscriminantManager* input, vector<double>& result)
{
   result.push_back(input->getDecayMode()->pt());
}

void
Eta::doComputation(PFTauDiscriminantManager* input, vector<double>& result)
{
   result.push_back(input->getDecayMode()->eta());
}

void
MainTrackPt::doComputation(PFTauDiscriminantManager* input, vector<double>& result)
{
   const reco::Candidate* theMainTrack = input->mainTrack();
   if (theMainTrack)
      result.push_back(theMainTrack->pt());
}

void
MainTrackAngle::doComputation(PFTauDiscriminantManager* input, vector<double>& result)
{
   math::XYZVector signalObjectsAxis = input->getDecayMode()->momentum();

   const reco::Candidate* theMainTrack = input->mainTrack();

   DeltaR<math::XYZVector> myDRComputer;

   if (theMainTrack)
      result.push_back(myDRComputer(theMainTrack->momentum(), signalObjectsAxis));
}

void
TrackPt::doComputation(PFTauDiscriminantManager* input, vector<double>& result)
{
   const vector<const reco::Candidate*>& theSignalObjects = input->signalObjectsSortedByPt();

   const reco::Candidate* theMainTrack = input->mainTrack();

   for(vector<const reco::Candidate*>::const_iterator iObject  = theSignalObjects.begin();
         iObject != theSignalObjects.end();
         ++iObject)
   {
      const reco::Candidate* currentObject = *iObject;
      if (currentObject->charge() && currentObject != theMainTrack)
         result.push_back(currentObject->pt());
   }
}

void
PiZeroPt::doComputation(PFTauDiscriminantManager* input, vector<double>& result)
{
   const vector<const reco::Candidate*>& theSignalObjects = input->signalObjectsSortedByPt();

   for(vector<const reco::Candidate*>::const_iterator iObject  = theSignalObjects.begin();
         iObject != theSignalObjects.end();
         ++iObject)
   {
      const reco::Candidate* currentObject = *iObject;
      if (!currentObject->charge())
         result.push_back(currentObject->pt());
   }
}

void
FilteredObjectPt::doComputation(PFTauDiscriminantManager* input, vector<double>& result)
{
   const reco::PFTauDecayMode* theDecayMode = input->getDecayMode();
   if (!theDecayMode)
      return;

   const vector<const reco::Candidate*> theFilteredObjects = theDecayMode->filteredObjectCandidates();

   for(vector<const reco::Candidate*>::const_iterator iObject  = theFilteredObjects.begin();
                                                      iObject != theFilteredObjects.end();
                                                    ++iObject)
   {
      const reco::Candidate* currentObject = *iObject;
      result.push_back(currentObject->pt());
   }
}

void
GammaOccupancy::doComputation(PFTauDiscriminantManager* input, vector<double>& result)
{
   const vector<const reco::Candidate*>& theSignalObjects = input->signalObjectsSortedByPt();

   for(vector<const reco::Candidate*>::const_iterator iObject  = theSignalObjects.begin();
         iObject != theSignalObjects.end();
         ++iObject)
   {
      const reco::Candidate* currentObject = *iObject;
      if (!currentObject->charge())
         result.push_back(input->getLeafDaughters(currentObject).size());
   }
}

void
GammaPt::doComputation(PFTauDiscriminantManager* input, vector<double>& result)
{
   const vector<const reco::Candidate*>& theSignalObjects = input->signalObjectsSortedByPt();

   for(vector<const reco::Candidate*>::const_iterator iObject  = theSignalObjects.begin();
         iObject != theSignalObjects.end();
         ++iObject)
   {
      const reco::Candidate* currentObject = *iObject;
      if (!currentObject->charge())
      {
         vector<const reco::Candidate*> daughters = input->getLeafDaughters(currentObject);
         for(vector<const reco::Candidate*>::const_iterator iDaughter  = daughters.begin();
                                                            iDaughter != daughters.end();
                                                          ++iDaughter)
         {
            result.push_back((*iDaughter)->pt());
         }
      }
   }
}


void
TrackAngle::doComputation(PFTauDiscriminantManager* input, vector<double>& result)
{
   const vector<const reco::Candidate*>& theSignalObjects = input->signalObjectsSortedByPt();

   math::XYZVector signalObjectsAxis = input->getDecayMode()->momentum();

   const reco::Candidate* theMainTrack = input->mainTrack();

   DeltaR<math::XYZVector> myDRComputer;

   for(vector<const reco::Candidate*>::const_iterator iObject  = theSignalObjects.begin();
         iObject != theSignalObjects.end();
         ++iObject)
   {
      const reco::Candidate* currentObject = *iObject;
      if (currentObject->charge() && currentObject != theMainTrack)
         result.push_back(myDRComputer(currentObject->momentum(), signalObjectsAxis));
   }
}

void
PiZeroAngle::doComputation(PFTauDiscriminantManager* input, vector<double>& result)
{
   const vector<const reco::Candidate*>& theSignalObjects = input->signalObjectsSortedByPt();

   math::XYZVector signalObjectsAxis = input->getDecayMode()->momentum();

   DeltaR<math::XYZVector> myDRComputer;

   for(vector<const reco::Candidate*>::const_iterator iObject  = theSignalObjects.begin();
         iObject != theSignalObjects.end();
         ++iObject)
   {
      const reco::Candidate* currentObject = *iObject;
      if (!currentObject->charge())
         result.push_back(myDRComputer(currentObject->momentum(), signalObjectsAxis));
   }
}

void
Dalitz::doComputation(PFTauDiscriminantManager* input, vector<double>& result)
{
   const vector<const reco::Candidate*>& theSignalObjects = input->signalObjectsSortedByPt();
   const reco::Candidate* theMainTrack = input->mainTrack();
   if (!theMainTrack)
      return;
   LorentzVector mainTrackFourVector = theMainTrack->p4();

   for(vector<const reco::Candidate*>::const_iterator iObject  = theSignalObjects.begin();
         iObject != theSignalObjects.end();
         ++iObject)
   {
      const reco::Candidate* currentObject = *iObject;
      if (currentObject != theMainTrack)
      {
         LorentzVector combinedFourVector = mainTrackFourVector + currentObject->p4();
         result.push_back(combinedFourVector.mass());
      }
   }
}

// takes invariant mass of all objects in signal cone
void
InvariantMassOfSignal::doComputation(PFTauDiscriminantManager* input, vector<double>& result)
{
   result.push_back(input->getDecayMode()->mass());
}

// returns vector of invariant masses of larger and larger subsets of all signal objects e.g. result[2] is
// the invariant mass of the lead track with the next highest Pt object

void
InvariantMass::doComputation(PFTauDiscriminantManager* input, vector<double>& result)
{
   const vector<const reco::Candidate*>& theSignalObjects = input->signalObjectsSortedByPt();
   const reco::Candidate* theMainTrack = input->mainTrack();
   if (!theMainTrack)
      return;
   LorentzVector fourVectorSoFar = theMainTrack->p4();

   for(vector<const reco::Candidate*>::const_iterator iObject  = theSignalObjects.begin();
         iObject != theSignalObjects.end();
         ++iObject)
   {
      const reco::Candidate* currentObject = *iObject;
      if (currentObject != theMainTrack)
      {
         fourVectorSoFar += currentObject->p4();
         result.push_back(fourVectorSoFar.mass());
      }
   }
}

void
OutlierPt::doComputation(PFTauDiscriminantManager* input, vector<double>& result)
{
   const vector<const reco::Candidate*>& theOutlierObjects = input->outlierObjectsSortedByPt();
   for(vector<const reco::Candidate*>::const_iterator iObject  = theOutlierObjects.begin();
         iObject != theOutlierObjects.end();
         ++iObject)
   {
      const reco::Candidate* currentObject = *iObject;
      if (currentObject)
         result.push_back(currentObject->pt());
   }
}

void
OutlierAngle::doComputation(PFTauDiscriminantManager* input, vector<double>& result)
{
   const vector<const reco::Candidate*>& theoutlierObjects = input->outlierObjectsSortedByPt();
   math::XYZVector signalObjectsAxis = input->getDecayMode()->momentum();
   DeltaR<math::XYZVector> myDRComputer;
   for(vector<const reco::Candidate*>::const_iterator iObject  = theoutlierObjects.begin();
         iObject != theoutlierObjects.end();
         ++iObject)
   {
      const reco::Candidate* currentObject = *iObject;
      if (currentObject)
         result.push_back(myDRComputer(currentObject->momentum(), signalObjectsAxis));
   }
}

void
ChargedOutlierPt::doComputation(PFTauDiscriminantManager* input, vector<double>& result)
{
   const vector<const reco::Candidate*>& theOutlierObjects = input->outlierObjectsSortedByPt();
   for(vector<const reco::Candidate*>::const_iterator iObject  = theOutlierObjects.begin();
         iObject != theOutlierObjects.end();
         ++iObject)
   {
      const reco::Candidate* currentObject = *iObject;
      if (currentObject && currentObject->charge())
         result.push_back(currentObject->pt());
   }
}

void
ChargedOutlierAngle::doComputation(PFTauDiscriminantManager* input, vector<double>& result)
{
   const vector<const reco::Candidate*>& theoutlierObjects = input->outlierObjectsSortedByPt();
   math::XYZVector signalObjectsAxis = input->getDecayMode()->momentum();
   DeltaR<math::XYZVector> myDRComputer;
   for(vector<const reco::Candidate*>::const_iterator iObject  = theoutlierObjects.begin();
         iObject != theoutlierObjects.end();
         ++iObject)
   {
      const reco::Candidate* currentObject = *iObject;
      if (currentObject && currentObject->charge())
         result.push_back(myDRComputer(currentObject->momentum(), signalObjectsAxis));
   }
}

void
NeutralOutlierPt::doComputation(PFTauDiscriminantManager* input, vector<double>& result)
{
   const vector<const reco::Candidate*>& theOutlierObjects = input->outlierObjectsSortedByPt();
   for(vector<const reco::Candidate*>::const_iterator iObject  = theOutlierObjects.begin();
         iObject != theOutlierObjects.end();
         ++iObject)
   {
      const reco::Candidate* currentObject = *iObject;
      if (currentObject && !currentObject->charge())
         result.push_back(currentObject->pt());
   }
}

void
NeutralOutlierAngle::doComputation(PFTauDiscriminantManager* input, vector<double>& result)
{
   const vector<const reco::Candidate*>& theoutlierObjects = input->outlierObjectsSortedByPt();
   math::XYZVector signalObjectsAxis = input->getDecayMode()->momentum();
   DeltaR<math::XYZVector> myDRComputer;
   for(vector<const reco::Candidate*>::const_iterator iObject  = theoutlierObjects.begin();
         iObject != theoutlierObjects.end();
         ++iObject)
   {
      const reco::Candidate* currentObject = *iObject;
      if (currentObject && !currentObject->charge())
         result.push_back(myDRComputer(currentObject->momentum(), signalObjectsAxis));
   }
}


}

