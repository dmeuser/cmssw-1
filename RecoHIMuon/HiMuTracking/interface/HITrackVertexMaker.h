// -*- C++ -*-
//
// Package:    TestMuL1L2.h
// Class:      TestMuL1L2
/*/

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Dong Ho Moon
//         Created:  Wed May  9 06:22:36 CEST 2007
// $Id: HITrackVertexMaker.h,v 1.4 2009/06/22 15:53:59 kodolova Exp $
//
//

#ifndef HITRACKVERTEXMAKER_H
#define HITRACKVERTEXMAKER_H


// system include files

#include <memory>

// framework include files

#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"

// navigation school

#include "TrackingTools/DetLayers/interface/NavigationSetter.h"
#include "TrackingTools/DetLayers/interface/NavigationSchool.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"
#include "TrackingTools/TrackFitters/interface/KFTrajectoryFitter.h"
#include "TrackingTools/TrackFitters/interface/KFTrajectorySmoother.h"
#include "RecoTracker/TkDetLayers/interface/GeometricSearchTracker.h"
#include "RecoTracker/MeasurementDet/interface/MeasurementTracker.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "TrackingTools/TrajectoryFiltering/interface/MinPtTrajectoryFilter.h"
   
// HI reconstruction includes

#include "RecoHIMuon/HiMuSeed/interface/HICConst.h"
#include "RecoHIMuon/HiMuPropagator/interface/FmpConst.h"
#include "RecoHIMuon/HiMuTracking/interface/HICTrajectoryBuilder.h"
#include "RecoHIMuon/HiMuTracking/interface/HICMeasurementEstimator.h"
#include "RecoHIMuon/HiMuTracking/interface/HICMuonUpdator.h"

//
// class declaration
//
namespace cms{

class HITrackVertexMaker {


   public:

  //constructor

      explicit HITrackVertexMaker(const edm::ParameterSet&, const edm::EventSetup& es1);

  //destructor 
      ~HITrackVertexMaker();

  //produceTracks 
       bool produceTracks(const edm ::Event&, const edm::EventSetup&, HICConst*, FmpConst*);


   private:

  int                                           eventCount;
  edm::InputTag                                 L2candTag_; 
  edm::InputTag                                 rphirecHitsTag;
  edm::InputTag                                 primaryVertexTag;

  edm::ParameterSet                             pset_;
  std::string                                   builderName;
  std::vector<const NavigationSchool*>          theNavigationSchoolV;   

};
}
#endif
