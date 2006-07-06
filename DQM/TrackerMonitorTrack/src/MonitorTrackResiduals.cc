// -*- C++ -*-
//
// Package:    TrackerMonitorTrack
// Class:      MonitorTrackResiduals
// 
/**\class MonitorTrackResiduals MonitorTrackResiduals.cc DQM/TrackerMonitorTrack/src/MonitorTrackResiduals.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Israel Goitom
//         Created:  Fri May 26 14:12:01 CEST 2006
// $Id: MonitorTrackResiduals.cc,v 1.12 2006/07/04 13:01:59 dkcira Exp $
//
//

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "CalibTracker/Records/interface/SiStripDetCablingRcd.h"
#include "CalibFormats/SiStripObjects/interface/SiStripDetCabling.h"

#include "DataFormats/SiStripDetId/interface/SiStripSubStructure.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidate.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHitFwd.h"

#include "DQM/SiStripCommon/interface/SiStripFolderOrganizer.h"
#include "DQM/SiStripCommon/interface/SiStripHistoId.h"
#include "DQM/TrackerMonitorTrack/interface/MonitorTrackResiduals.h"

#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "RecoTracker/TransientTrackingRecHit/interface/TkTransientTrackingRecHitBuilder.h"
#include "RecoTracker/TrackProducer/interface/TrackingRecHitLessFromGlobalPosition.h"
#include "TrackingTools/Records/interface/TransientRecHitRecord.h"

#include "TrackingTools/PatternTools/interface/TrajectoryFitter.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
#include "TrackingTools/Records/interface/TransientRecHitRecord.h" 



MonitorTrackResiduals::MonitorTrackResiduals(const edm::ParameterSet& iConfig)
{
  dbe = edm::Service<DaqMonitorBEInterface>().operator->();
  conf_ = iConfig;
}

MonitorTrackResiduals::~MonitorTrackResiduals()
{
}

void MonitorTrackResiduals::beginJob(edm::EventSetup const& iSetup)
{
  using namespace edm;
  using namespace std;

  dbe->setCurrentFolder("SiStrip/Detectors");

  // take from eventSetup the SiStripDetCabling object

  edm::ESHandle<SiStripDetCabling> tkmechstruct;
  iSetup.get<SiStripDetCablingRcd>().get(tkmechstruct);

  // get list of active detectors from SiStripDetCabling
  vector<uint32_t> activeDets;
  activeDets.clear(); // just in case
  tkmechstruct->addActiveDetectorsRawIds(activeDets);

  // use SiStripSubStructure for selecting certain regions
  SiStripSubStructure substructure;
  vector<uint32_t> DetIds = activeDets;
  vector<uint32_t> TIBDetIds;
  vector<uint32_t> TOBDetIds;
  vector<uint32_t> TIDDetIds;
  vector<uint32_t> TECDetIds;
  substructure.getTIBDetectors(activeDets, TIBDetIds); // this adds rawDetIds to SelectedDetIds
  substructure.getTOBDetectors(activeDets, TOBDetIds);    // this adds rawDetIds to SelectedDetIds
  substructure.getTIDDetectors(activeDets, TIDDetIds); // this adds rawDetIds to SelectedDetIds
  substructure.getTECDetectors(activeDets, TECDetIds); // this adds rawDetIds to SelectedDetIds
  
  // book TIB histo
  vector<uint32_t>::const_iterator detid_begin = TIBDetIds.begin();
  vector<uint32_t>::const_iterator detid_end = TIBDetIds.end() -1;
  int detBegin=(*detid_begin)&0x1ffffff;
  int detEnd=(*detid_end)&0x1ffffff;
  HitResidual["TIB"] = dbe->bookProfile("TIBHitResiduals", "TIB Hit residuals",  TIBDetIds.size(), detBegin, detEnd, 1, -4, 4);

  // book TOB histo
  detid_begin = TOBDetIds.begin();
  detid_end = TOBDetIds.end()-1;
  detBegin=(*detid_begin)&0x1ffffff;
  detEnd=(*detid_end)&0x1ffffff;
  HitResidual["TOB"] = dbe->bookProfile("TOBHitResiduals", "TOB Hit residuals", TOBDetIds.size(), detBegin, detEnd, 1, -4, 4);

  // book TID histo
  detid_begin = TIDDetIds.begin();
  detid_end = TIDDetIds.end()-1;
  detBegin=(*detid_begin)&0x1ffffff;
  detEnd=(*detid_end)&0x1ffffff;
  HitResidual["TID"] = dbe->bookProfile("TIDHitResiduals", "TID Hit residuals", TIDDetIds.size(), detBegin, detEnd, 1, -4, 4);

  // book TEC histo
  detid_begin = TECDetIds.begin();
  detid_end = TECDetIds.end()-1;
  detBegin=(*detid_begin)&0x1ffffff;
  detEnd=(*detid_end)&0x1ffffff;
  HitResidual["TEC"] = dbe->bookProfile("TECHitResiduals", "TEC Hit residuals", TECDetIds.size(), detBegin, detEnd, 1, -4, 4);
}

void MonitorTrackResiduals::endJob(void)
{
  dbe->showDirStructure();
  bool outputMEsInRootFile = conf_.getParameter<bool>("OutputMEsInRootFile");
  std::string outputFileName = conf_.getParameter<std::string>("OutputFileName");
  if(outputMEsInRootFile){
    edm::LogInfo("TrackerMonitorTrack|MonitorTrackResiduals")<<" writing out histograms to file "<<outputFileName;
    dbe->save(outputFileName);
  }
}


// ------------ method called to produce the data  ------------
void MonitorTrackResiduals::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  std::string TrackCandidateProducer = conf_.getParameter<std::string>("TrackCandidateProducer");
  std::string TrackCandidateLabel = conf_.getParameter<std::string>("TrackCandidateLabel");

  ESHandle<TrackerGeometry> theRG;
  iSetup.get<TrackerDigiGeometryRecord>().get(theRG);
  ESHandle<MagneticField> theRMF;
  iSetup.get<IdealMagneticFieldRecord>().get(theRMF);
  //
  std::string cpeName = conf_.getParameter<std::string>("TTRHBuilder");
  ESHandle<TransientTrackingRecHitBuilder> theBuilder;
  iSetup.get<TransientRecHitRecord>().get(cpeName,theBuilder);
  //
  std::string fitterName = conf_.getParameter<std::string>("Fitter");
  ESHandle<TrajectoryFitter> theRFitter;
  iSetup.get<TrackingComponentsRecord>().get(fitterName,theRFitter);

  const TransientTrackingRecHitBuilder* builder = theBuilder.product();
  const TrackingGeometry * theG = theRG.product();
  const MagneticField * theMF = theRMF.product();
  const TrajectoryFitter * theFitter = theRFitter.product();

  Handle<TrackCandidateCollection> trackCandidateCollection;
  iEvent.getByLabel(TrackCandidateProducer, TrackCandidateLabel, trackCandidateCollection);

  ESHandle<TrackerGeometry> pDD;
  iSetup.get<TrackerDigiGeometryRecord>().get( pDD );

  for (TrackCandidateCollection::const_iterator track = trackCandidateCollection->begin(); track!=trackCandidateCollection->end(); ++track)
    {
      const TrackCandidate * theTC = &(*track);
      PTrajectoryStateOnDet state = theTC->trajectoryStateOnDet();
      const TrackCandidate::range& recHitVec=theTC->recHits();
      const TrajectorySeed& seed = theTC->seed();

      //convert PTrajectoryStateOnDet to TrajectoryStateOnSurface
      TrajectoryStateTransform transformer;
      DetId detId(state.detId());

      TrajectoryStateOnSurface theTSOS = transformer.transientState( state, &(theG->idToDet(detId)->surface()), theMF);

//      OwnVector<TransientTrackingRecHit> hits;
      Trajectory::RecHitContainer hits;
      TrackingRecHitCollection::const_iterator hit;
      for (hit=recHitVec.first; hit!= recHitVec.second; ++hit)
	{
	  hits.push_back(builder->build(&(*hit)));

	  // do the fitting
	  std::vector<Trajectory> trajVec = theFitter->fit(seed,  hits, theTSOS);

	  TrajectoryStateOnSurface innertsos;  
	  if (trajVec.size() != 0)
	    {   
	      const Trajectory& theTraj = trajVec.front();
	      if (theTraj.direction() == alongMomentum)
		{      
		  innertsos = theTraj.firstMeasurement().updatedState();
		}
	      else
		{
		  innertsos = theTraj.lastMeasurement().updatedState();
		}
	    }

	  const LocalPoint & LocalHitPos = hit->localPosition();

	  const LocalPoint& LocalTrajPos = innertsos.localPosition();
	  const DetId & detId = hit->geographicalId();
	  const GeomDetUnit *detUnit = pDD->idToDetUnit(detId);
	  const StripGeomDetUnit* stripDet = dynamic_cast<const StripGeomDetUnit*>(&(*detUnit));
	  const StripTopology& topology = stripDet->specificTopology();

	  const float hitPositionInStrips = topology.strip(LocalHitPos);
	  const float trackPositionInStrips = topology.strip(LocalTrajPos);
	  
	  double residual = trackPositionInStrips - hitPositionInStrips;

	  DetId hit_detId = hit->geographicalId();
	  int CutRawDetId= ( hit_detId.rawId() ) & 0x1ffffff ;
	  if (hit_detId.subdetId() == int( StripSubdetector::TIB ))
	    {
	      HitResidual["TIB"]->Fill(CutRawDetId, residual);
	    }
	  if (hit_detId.subdetId() == int( StripSubdetector::TOB ))
	    {
	      HitResidual["TOB"]->Fill(CutRawDetId, residual);
	    }
	  if (hit_detId.subdetId() == int( StripSubdetector::TID ))
	    {
	      HitResidual["TID"]->Fill(CutRawDetId, residual);
	    }
	  if (hit_detId.subdetId() == int( StripSubdetector::TEC ))
	    {
	      HitResidual["TEC"]->Fill(CutRawDetId, residual);
	    }
	}
    }

}

//define this as a plug-in
//DEFINE_FWK_MODULE(MonitorTrackResiduals)
