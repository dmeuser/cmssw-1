#include "SimG4Core/Notification/interface/NewTrackAction.h"
#include "SimG4Core/Notification/interface/TrackInformation.h"
#include "SimG4Core/Notification/interface/SimG4Exception.h"
#include "SimG4Core/Notification/interface/TrackInformationExtractor.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "G4Track.hh"

NewTrackAction::NewTrackAction() {}

void NewTrackAction::primary(const G4Track * aTrack) const {
  primary(const_cast<G4Track *>(aTrack)); 
}

void NewTrackAction::primary(G4Track * aTrack) const {
  addUserInfoToPrimary(aTrack);
}

void NewTrackAction::secondary(const G4Track * aSecondary,const G4Track & mother, int flag) const {
  secondary(const_cast<G4Track *>(aSecondary),mother,flag); 
}

void NewTrackAction::secondary(G4Track * aSecondary,const G4Track & mother, int flag) const {
  if (aSecondary->GetParentID() != mother.GetTrackID()) 
    throw SimG4Exception("NewTrackAction: secondary parent ID does not match mother id");
  TrackInformationExtractor extractor;
  const TrackInformation & motherInfo(extractor(mother));
  addUserInfoToSecondary(aSecondary,motherInfo,flag);
}

void NewTrackAction::addUserInfoToPrimary(G4Track * aTrack) const {
  TrackInformation * trkInfo = new TrackInformation();
  trkInfo->isPrimary(true);
  trkInfo->storeTrack(true);
  trkInfo->putInHistory();
  aTrack->SetUserInformation(trkInfo);  
}

void NewTrackAction::addUserInfoToSecondary(G4Track * aTrack,const TrackInformation & motherInfo, int flag) const {

  TrackInformation * trkInfo = new TrackInformation();
  LogDebug("SimG4CoreApplication") << "NewTrackAction called for "
				   << aTrack->GetTrackID()
				   << " mother " << motherInfo.isPrimary()
				   << " flag " << flag;
  
  // Take care of cascade decays
  if (flag == 1)
    trkInfo->isPrimary(true);

  // Store if decay or conversion
  if (flag > 0) {
    trkInfo->storeTrack(true);
    trkInfo->putInHistory();
    trkInfo->setIDonCaloSurface(aTrack->GetTrackID(),
				motherInfo.getIDCaloVolume(),
				motherInfo.getIDLastVolume());
  } else {
    // transfer calo ID from mother (to be checked in TrackingAction)
    trkInfo->setIDonCaloSurface(motherInfo.getIDonCaloSurface(),
				motherInfo.getIDCaloVolume(),
				motherInfo.getIDLastVolume());
  }
  aTrack->SetUserInformation(trkInfo);  
}
