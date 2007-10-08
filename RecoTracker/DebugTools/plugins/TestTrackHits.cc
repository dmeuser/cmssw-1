#include "RecoTracker/DebugTools/interface/TestTrackHits.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/MaterialEffects/interface/PropagatorWithMaterial.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include <TDirectory.h>

#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"

typedef TrajectoryStateOnSurface TSOS;
typedef TransientTrackingRecHit::ConstRecHitPointer CTTRHp;
using namespace std;
using namespace edm;

TestTrackHits::TestTrackHits(const edm::ParameterSet& iConfig):
  conf_(iConfig){
  LogTrace("TestTrackHits") << conf_;
  propagatorName = conf_.getParameter<std::string>("Propagator");   
  builderName = conf_.getParameter<std::string>("TTRHBuilder");   
  srcName = conf_.getParameter<std::string>("src");   
  tpName = conf_.getParameter<std::string>("tpname");   
  updatorName = conf_.getParameter<std::string>("updator");
  mineta = conf_.getParameter<double>("mineta");
  maxeta = conf_.getParameter<double>("maxeta");
}

TestTrackHits::~TestTrackHits(){}

void TestTrackHits::beginJob(const edm::EventSetup& iSetup)
{
  iSetup.get<TrackerDigiGeometryRecord>().get(theG);
  iSetup.get<IdealMagneticFieldRecord>().get(theMF);  
  iSetup.get<TrackingComponentsRecord>().get(propagatorName,thePropagator);
  iSetup.get<TransientRecHitRecord>().get(builderName,theBuilder);
  iSetup.get<TrackingComponentsRecord>().get(updatorName,theUpdator);
  iSetup.get<TrackAssociatorRecord>().get("TrackAssociatorByHits",trackAssociator);
 
  file = new TFile("test_track_hits.root","recreate");
  for (int i=0; i!=6; i++)
    for (int j=0; j!=9; j++){
      if (i==0 && j>2) break;
      if (i==1 && j>1) break;
      if (i==2 && j>3) break;
      if (i==3 && j>2) break;
      if (i==4 && j>5) break;
      if (i==5 && j>8) break;

      title.str("");
      title << "Chi2Increment_" << i+1 << "-" << j+1 ;
      hChi2Increment[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,0,100);
      title.str("");
      title << "Chi2GoodHit_" << i+1 << "-" << j+1 ;
      hChi2GoodHit[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,0,100);
      title.str("");
      title << "Chi2BadHit_" << i+1 << "-" << j+1 ;
      hChi2BadHit[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,0,100);

      title.str("");
      title << "PullGP_X_" << i+1 << "-" << j+1 << "_ts";
      hPullGP_X_ts[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
      title.str("");
      title << "PullGP_Y_" << i+1 << "-" << j+1 << "_ts";
      hPullGP_Y_ts[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
      title.str("");
      title << "PullGP_Z_" << i+1 << "-" << j+1 << "_ts";
      hPullGP_Z_ts[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);

      title.str("");
      title << "PullGM_X_" << i+1 << "-" << j+1 << "_ts";
      hPullGM_X_ts[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
      title.str("");
      title << "PullGM_Y_" << i+1 << "-" << j+1 << "_ts";
      hPullGM_Y_ts[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
      title.str("");
      title << "PullGM_Z_" << i+1 << "-" << j+1 << "_ts";
      hPullGM_Z_ts[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);

      title.str("");
      title << "PullGP_X_" << i+1 << "-" << j+1 << "_tr";
      hPullGP_X_tr[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
      title.str("");
      title << "PullGP_Y_" << i+1 << "-" << j+1 << "_tr";
      hPullGP_Y_tr[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
      title.str("");
      title << "PullGP_Z_" << i+1 << "-" << j+1 << "_tr";
      hPullGP_Z_tr[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);

      title.str("");
      title << "PullGP_X_" << i+1 << "-" << j+1 << "_rs";
      hPullGP_X_rs[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
      title.str("");
      title << "PullGP_Y_" << i+1 << "-" << j+1 << "_rs";
      hPullGP_Y_rs[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
      title.str("");
      title << "PullGP_Z_" << i+1 << "-" << j+1 << "_rs";
      hPullGP_Z_rs[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);

      if ( ((i==2||i==4)&&(j==0||j==1)) || (i==3||i==5) ){
	//mono
	title.str("");
	title << "PullGP_X_" << i+1 << "-" << j+1 << "_ts_mono";
	hPullGP_X_ts_mono[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGP_Y_" << i+1 << "-" << j+1 << "_ts_mono";
	hPullGP_Y_ts_mono[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGP_Z_" << i+1 << "-" << j+1 << "_ts_mono";
	hPullGP_Z_ts_mono[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);

	title.str("");
	title << "PullGM_X_" << i+1 << "-" << j+1 << "_ts_mono";
	hPullGM_X_ts_mono[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGM_Y_" << i+1 << "-" << j+1 << "_ts_mono";
	hPullGM_Y_ts_mono[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGM_Z_" << i+1 << "-" << j+1 << "_ts_mono";
	hPullGM_Z_ts_mono[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);

	title.str("");
	title << "PullGP_X_" << i+1 << "-" << j+1 << "_tr_mono";
	hPullGP_X_tr_mono[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGP_Y_" << i+1 << "-" << j+1 << "_tr_mono";
	hPullGP_Y_tr_mono[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGP_Z_" << i+1 << "-" << j+1 << "_tr_mono";
	hPullGP_Z_tr_mono[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);

	title.str("");
	title << "PullGP_X_" << i+1 << "-" << j+1 << "_rs_mono";
	hPullGP_X_rs_mono[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGP_Y_" << i+1 << "-" << j+1 << "_rs_mono";
	hPullGP_Y_rs_mono[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGP_Z_" << i+1 << "-" << j+1 << "_rs_mono";
	hPullGP_Z_rs_mono[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);

	//stereo
	title.str("");
	title << "PullGP_X_" << i+1 << "-" << j+1 << "_ts_stereo";
	hPullGP_X_ts_stereo[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGP_Y_" << i+1 << "-" << j+1 << "_ts_stereo";
	hPullGP_Y_ts_stereo[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGP_Z_" << i+1 << "-" << j+1 << "_ts_stereo";
	hPullGP_Z_ts_stereo[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);

	title.str("");
	title << "PullGM_X_" << i+1 << "-" << j+1 << "_ts_stereo";
	hPullGM_X_ts_stereo[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGM_Y_" << i+1 << "-" << j+1 << "_ts_stereo";
	hPullGM_Y_ts_stereo[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGM_Z_" << i+1 << "-" << j+1 << "_ts_stereo";
	hPullGM_Z_ts_stereo[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);

	title.str("");
	title << "PullGP_X_" << i+1 << "-" << j+1 << "_tr_stereo";
	hPullGP_X_tr_stereo[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGP_Y_" << i+1 << "-" << j+1 << "_tr_stereo";
	hPullGP_Y_tr_stereo[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGP_Z_" << i+1 << "-" << j+1 << "_tr_stereo";
	hPullGP_Z_tr_stereo[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);

	title.str("");
	title << "PullGP_X_" << i+1 << "-" << j+1 << "_rs_stereo";
	hPullGP_X_rs_stereo[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGP_Y_" << i+1 << "-" << j+1 << "_rs_stereo";
	hPullGP_Y_rs_stereo[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGP_Z_" << i+1 << "-" << j+1 << "_rs_stereo";
	hPullGP_Z_rs_stereo[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
      }
    }
  hTotChi2Increment = new TH1F("TotChi2Increment","TotChi2Increment",1000,0,100);
  hTotChi2GoodHit = new TH1F("TotChi2GoodHit","TotChi2GoodHit",1000,0,100);
  hTotChi2BadHit = new TH1F("TotChi2BadHit","TotChi2BadHit",1000,0,100);
  hProcess_vs_Chi2  = new TH2F("Process_vs_Chi2","Process_vs_Chi2",1000,0,100,17,-0.5,16.5);  
  hClsize_vs_Chi2   = new TH2F("Clsize_vs_Chi2","Clsize_vs_Chi2",1000,0,100,17,-0.5,16.5);
  hGoodHit_vs_Chi2  = new TH2F("hGoodHit_vs_Chi2","hGoodHit_vs_Chi2",10000,0,1000,2,-0.5,1.5);
}

void TestTrackHits::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  LogDebug("TestTrackHits") << "new event" ;
  iEvent.getByLabel(srcName,trajCollectionHandle);
  iEvent.getByLabel(srcName,trackCollectionHandle);
  iEvent.getByLabel(srcName,trajTrackAssociationCollectionHandle);
  iEvent.getByLabel(tpName,trackingParticleCollectionHandle);

  hitAssociator = new TrackerHitAssociator::TrackerHitAssociator(iEvent);
  reco::RecoToSimCollection recSimColl=trackAssociator->associateRecoToSim(trackCollectionHandle,
									   trackingParticleCollectionHandle,
									   &iEvent);
  
  TrajectoryStateCombiner combiner;

  int i=0;
  for(vector<Trajectory>::const_iterator it = trajCollectionHandle->begin(); it!=trajCollectionHandle->end();it++){
    
    LogTrace("TestTrackHits") << "\n*****************new trajectory********************" ;
    double tchi2 = 0;

    vector<TrajectoryMeasurement> tmColl = it->measurements();

    edm::Ref<vector<Trajectory> >  traj(trajCollectionHandle, i);
    reco::TrackRef track = (*trajTrackAssociationCollectionHandle.product())[traj];
    if (track->eta()>maxeta || track->eta()<mineta) continue;

    std::vector<std::pair<TrackingParticleRef, double> > tP;
    if(recSimColl.find(track) != recSimColl.end()){
      tP = recSimColl[track];
    } else continue;//skip fake tracks
    if (tP.size()==0) continue;
    TrackingParticleRef tp = tP.begin()->first;
    LogTrace("TestTrackHits") << "a tp is associated with fraction=" << tP.begin()->second;
    vector<unsigned int> tpids;
    for (TrackingParticle::g4t_iterator g4T=tp->g4Track_begin(); g4T!=tp->g4Track_end(); ++g4T) {
      LogTrace("TestTrackHits") << "tp id=" << g4T->trackId();
      tpids.push_back(g4T->trackId());
    }

    for (vector<TrajectoryMeasurement>::iterator tm=tmColl.begin();tm!=tmColl.end();++tm){
      
      tchi2+=tm->estimate();

      LogTrace("TestTrackHits") << "+++++++++++++++++new hit+++++++++++++++++" ;
      CTTRHp rhit = tm->recHit();
      //TSOS state = tm->backwardPredictedState();
      //TSOS state = tm->forwardPredictedState();
      TSOS state = combiner(tm->backwardPredictedState(), tm->forwardPredictedState());

      if (rhit->isValid()==0 && rhit->det()!=0) continue;

      int subdetId = rhit->det()->geographicalId().subdetId();
      int layerId  = 0;
      DetId id = rhit->det()->geographicalId();
      if (id.subdetId()==3) layerId = ((TIBDetId)(id)).layer();
      if (id.subdetId()==5) layerId = ((TOBDetId)(id)).layer();
      if (id.subdetId()==1) layerId = ((PXBDetId)(id)).layer();
      if (id.subdetId()==4) layerId = ((TIDDetId)(id)).wheel();
      if (id.subdetId()==6) layerId = ((TECDetId)(id)).wheel();
      if (id.subdetId()==2) layerId = ((PXFDetId)(id)).disk();
      LogTrace("TestTrackHits") << "subdetId=" << subdetId << " layerId=" << layerId ;

      const Surface * surf = rhit->surface();
      if (surf==0) continue;

      double delta = 99999;
      LocalPoint rhitLPv = rhit->localPosition();
      vector<PSimHit> assSimHits = hitAssociator->associateHit(*(rhit)->hit());
      if (assSimHits.size()==0) continue;
      PSimHit shit;
      for(vector<PSimHit>::const_iterator m=assSimHits.begin(); m<assSimHits.end(); m++){
	if ((m->localPosition()-rhitLPv).mag()<delta) {
	  shit=*m;
	  delta = (m->localPosition()-rhitLPv).mag();
	}
      }

      //plot chi2 increment
      double chi2increment = tm->estimate();
      LogTrace("TestTrackHits") << "tm->estimate()=" << tm->estimate();
      title.str("");
      title << "Chi2Increment_" << subdetId << "-" << layerId;
      hChi2Increment[title.str()]->Fill( chi2increment );
      hTotChi2Increment->Fill( chi2increment );
      hProcess_vs_Chi2->Fill( chi2increment, shit.processType() );
      if (dynamic_cast<const SiPixelRecHit*>(rhit->hit()))	
	hClsize_vs_Chi2->Fill( chi2increment, ((const SiPixelRecHit*)(rhit->hit()))->cluster()->size() );
      if (dynamic_cast<const SiStripRecHit2D*>(rhit->hit()))	
	hClsize_vs_Chi2->Fill( chi2increment, ((const SiStripRecHit2D*)(rhit->hit()))->cluster()->amplitudes().size() );

      std::vector<SimHitIdpr> simTrackIds = hitAssociator->associateHitId(*(rhit)->hit());
      bool goodhit = false;
      for(size_t j=0; j<simTrackIds.size(); j++){
	LogTrace("TestTrackHits") << "hit id=" << simTrackIds[j].first;
	for (size_t jj=0; jj<tpids.size(); jj++){
	  if (simTrackIds[j].first == tpids[jj]) goodhit = true;
	  break;
	}
      }
      if (goodhit) {
	LogVerbatim("TestTrackHits") << "good hit" ;
	title.str("");
	title << "Chi2GoodHit_" << subdetId << "-" << layerId;
	hChi2GoodHit[title.str()]->Fill( chi2increment );
	hTotChi2GoodHit->Fill( chi2increment );
      } else {
	LogVerbatim("TestTrackHits") << "bad hit" ;
	title.str("");
	title << "Chi2BadHit_" << subdetId << "-" << layerId;
	hChi2BadHit[title.str()]->Fill( chi2increment );
	hTotChi2BadHit->Fill( chi2increment );
      }
      hGoodHit_vs_Chi2->Fill(chi2increment,goodhit);

      LocalVector shitLMom;
      LocalPoint shitLPos;
      if (dynamic_cast<const SiStripMatchedRecHit2D*>(rhit->hit())) {
	double rechitmatchedx = rhit->localPosition().x();
	double rechitmatchedy = rhit->localPosition().y();
        double mindist = 999999;
        float distx, disty;
        std::pair<LocalPoint,LocalVector> closestPair;
	const StripGeomDetUnit* stripDet =(StripGeomDetUnit*) ((const GluedGeomDet *)(rhit)->det())->stereoDet();
	const BoundPlane& plane = (rhit)->det()->surface();
	for(vector<PSimHit>::const_iterator m=assSimHits.begin(); m<assSimHits.end(); m++){
	  //project simhit;
	  std::pair<LocalPoint,LocalVector> hitPair = projectHit((*m),stripDet,plane);
	  distx = fabs(rechitmatchedx - hitPair.first.x());
	  disty = fabs(rechitmatchedy - hitPair.first.y());
	  double dist = distx*distx+disty*disty;
	  if(sqrt(dist)<mindist){
	    mindist = dist;
	    closestPair = hitPair;
	  }
	}
	shitLPos = closestPair.first;
	shitLMom = closestPair.second;
      } else {
	shitLPos = shit.localPosition();	
	shitLMom = shit.momentumAtEntry();
      }
      GlobalVector shitGMom = surf->toGlobal(shitLMom);
      GlobalPoint shitGPos = surf->toGlobal(shitLPos);

      GlobalVector tsosGMom = state.globalMomentum();
      GlobalError  tsosGMEr(state.cartesianError().matrix().Sub<AlgebraicSymMatrix33>(3,3));
      GlobalPoint  tsosGPos = state.globalPosition();
      GlobalError  tsosGPEr = state.cartesianError().position();

      GlobalPoint rhitGPos = (rhit)->globalPosition();
      GlobalError rhitGPEr = (rhit)->globalPositionError();

      LogVerbatim("TestTrackHits") << "assSimHits.size()=" << assSimHits.size() ;
      LogVerbatim("TestTrackHits") << "tsos globalPos   =" << tsosGPos ;
      LogVerbatim("TestTrackHits") << "sim hit globalPos=" << shitGPos ;
      LogVerbatim("TestTrackHits") << "rec hit globalPos=" << rhitGPos ;
      LogVerbatim("TestTrackHits") << "geographicalId   =" << rhit->det()->geographicalId().rawId() ;
      LogVerbatim("TestTrackHits") << "surface position =" << surf->position() ;

# if 0
      if (rhit->detUnit()) LogTrace("TestTrackHits") << "rhit->detUnit()->geographicalId()=" 
						     << rhit->detUnit()->geographicalId().rawId() ;
      LogTrace("TestTrackHits") << "rhit->det()->surface().position()=" 
				<< rhit->det()->surface().position() ;
      if (rhit->detUnit()) LogTrace("TestTrackHits") << "rhit->detUnit()->surface().position()="  
						     << rhit->detUnit()->surface().position() ;
      LogTrace("TestTrackHits") << "rhit->det()->position()=" << rhit->det()->position() ;
      if (rhit->detUnit()) LogTrace("TestTrackHits") << "rhit->detUnit()->position()="  
						     << rhit->detUnit()->position() ;
      LogTrace("TestTrackHits") << "rhit->det()->surface().bounds().length()=" 
				<< rhit->det()->surface().bounds().length() ;
      if (rhit->detUnit()) LogTrace("TestTrackHits") << "rhit->detUnit()->surface().bounds().length()="  
						     << rhit->detUnit()->surface().bounds().length() ;
      LogTrace("TestTrackHits") << "rhit->det()->surface().bounds().width()=" 
				<< rhit->det()->surface().bounds().width() ;
      if (rhit->detUnit()) LogTrace("TestTrackHits") << "rhit->detUnit()->surface().bounds().width()="  
						     << rhit->detUnit()->surface().bounds().width() ;
      LogTrace("TestTrackHits") << "rhit->det()->surface().bounds().thickness()=" 
				<< rhit->det()->surface().bounds().thickness() ;
      if (rhit->detUnit()) LogTrace("TestTrackHits") << "rhit->detUnit()->surface().bounds().thickness()="  
						     << rhit->detUnit()->surface().bounds().thickness() ;
#endif      

      double pullGPX_rs = (rhitGPos.x()-shitGPos.x())/sqrt(rhitGPEr.cxx());
      double pullGPY_rs = (rhitGPos.y()-shitGPos.y())/sqrt(rhitGPEr.cyy());
      double pullGPZ_rs = (rhitGPos.z()-shitGPos.z())/sqrt(rhitGPEr.czz());
      //double pullGPX_rs = (rhitGPos.x()-shitGPos.x());
      //double pullGPY_rs = (rhitGPos.y()-shitGPos.y());
      //double pullGPZ_rs = (rhitGPos.z()-shitGPos.z());

      LogTrace("TestTrackHits") << "rs" ;

      title.str("");
      title << "PullGP_X_" << subdetId << "-" << layerId << "_rs";
      hPullGP_X_rs[title.str()]->Fill( pullGPX_rs );
      title.str("");
      title << "PullGP_Y_" << subdetId << "-" << layerId << "_rs";
      hPullGP_Y_rs[title.str()]->Fill( pullGPY_rs );
      title.str("");
      title << "PullGP_Z_" << subdetId << "-" << layerId << "_rs";
      hPullGP_Z_rs[title.str()]->Fill( pullGPZ_rs );

      double pullGPX_tr = (tsosGPos.x()-rhitGPos.x())/sqrt(tsosGPEr.cxx()+rhitGPEr.cxx());
      double pullGPY_tr = (tsosGPos.y()-rhitGPos.y())/sqrt(tsosGPEr.cyy()+rhitGPEr.cyy());
      double pullGPZ_tr = (tsosGPos.z()-rhitGPos.z())/sqrt(tsosGPEr.czz()+rhitGPEr.czz());
      //double pullGPX_tr = (tsosGPos.x()-rhitGPos.x());
      //double pullGPY_tr = (tsosGPos.y()-rhitGPos.y());
      //double pullGPZ_tr = (tsosGPos.z()-rhitGPos.z());

      LogTrace("TestTrackHits") << "tr" ;

      title.str("");
      title << "PullGP_X_" << subdetId << "-" << layerId << "_tr";
      hPullGP_X_tr[title.str()]->Fill( pullGPX_tr );
      title.str("");
      title << "PullGP_Y_" << subdetId << "-" << layerId << "_tr";
      hPullGP_Y_tr[title.str()]->Fill( pullGPY_tr );
      title.str("");
      title << "PullGP_Z_" << subdetId << "-" << layerId << "_tr";
      hPullGP_Z_tr[title.str()]->Fill( pullGPZ_tr );

      double pullGPX_ts = (tsosGPos.x()-shitGPos.x())/sqrt(tsosGPEr.cxx());
      double pullGPY_ts = (tsosGPos.y()-shitGPos.y())/sqrt(tsosGPEr.cyy());
      double pullGPZ_ts = (tsosGPos.z()-shitGPos.z())/sqrt(tsosGPEr.czz());
      //double pullGPX_ts = (tsosGPos.x()-shitGPos.x());
      //double pullGPY_ts = (tsosGPos.y()-shitGPos.y());
      //double pullGPZ_ts = (tsosGPos.z()-shitGPos.z());

      LogTrace("TestTrackHits") << "ts1" ;

      title.str("");
      title << "PullGP_X_" << subdetId << "-" << layerId << "_ts";
      hPullGP_X_ts[title.str()]->Fill( pullGPX_ts );
      title.str("");
      title << "PullGP_Y_" << subdetId << "-" << layerId << "_ts";
      hPullGP_Y_ts[title.str()]->Fill( pullGPY_ts );
      title.str("");
      title << "PullGP_Z_" << subdetId << "-" << layerId << "_ts";
      hPullGP_Z_ts[title.str()]->Fill( pullGPZ_ts );

      double pullGMX_ts = (tsosGMom.x()-shitGMom.x())/sqrt(tsosGMEr.cxx());
      double pullGMY_ts = (tsosGMom.y()-shitGMom.y())/sqrt(tsosGMEr.cyy());
      double pullGMZ_ts = (tsosGMom.z()-shitGMom.z())/sqrt(tsosGMEr.czz());
      //double pullGMX_ts = (tsosGMom.x()-shitGMom.x());
      //double pullGMY_ts = (tsosGMom.y()-shitGMom.y());
      //double pullGMZ_ts = (tsosGMom.z()-shitGMom.z());

      LogTrace("TestTrackHits") << "ts2" ;

      title.str("");
      title << "PullGM_X_" << subdetId << "-" << layerId << "_ts";
      hPullGM_X_ts[title.str()]->Fill( pullGMX_ts );
      title.str("");
      title << "PullGM_Y_" << subdetId << "-" << layerId << "_ts";
      hPullGM_Y_ts[title.str()]->Fill( pullGMY_ts );
      title.str("");
      title << "PullGM_Z_" << subdetId << "-" << layerId << "_ts";
      hPullGM_Z_ts[title.str()]->Fill( pullGMZ_ts );
      if (dynamic_cast<const SiStripMatchedRecHit2D*>(rhit->hit())) {
	Propagator* thePropagatorAnyDir = new PropagatorWithMaterial(anyDirection,0.105,theMF.product(),1.6);
	//mono
	LogTrace("TestTrackHits") << "MONO HIT" ;
	CTTRHp tMonoHit = 
	  theBuilder->build(dynamic_cast<const SiStripMatchedRecHit2D*>(rhit->hit())->monoHit());
	if (tMonoHit==0) continue;
	vector<PSimHit> assMonoSimHits = hitAssociator->associateHit(*tMonoHit->hit());
	if (assMonoSimHits.size()==0) continue;
	const PSimHit sMonoHit = *(assMonoSimHits.begin());
	const Surface * monoSurf = &( tMonoHit->det()->surface() );
	if (monoSurf==0) continue;
	TSOS monoState = thePropagatorAnyDir->propagate(state,*monoSurf);
	if (monoState.isValid()==0) continue;

	LocalVector monoShitLMom = sMonoHit.momentumAtEntry();
	GlobalVector monoShitGMom = monoSurf->toGlobal(monoShitLMom);
	LocalPoint monoShitLPos = sMonoHit.localPosition();
	GlobalPoint monoShitGPos = monoSurf->toGlobal(monoShitLPos);

	//LogTrace("TestTrackHits") << "assMonoSimHits.size()=" << assMonoSimHits.size() ;
	//LogTrace("TestTrackHits") << "mono shit=" << monoShitGPos ;

	GlobalVector monoTsosGMom = monoState.globalMomentum();
	GlobalError  monoTsosGMEr(monoState.cartesianError().matrix().Sub<AlgebraicSymMatrix33>(3,3));
	GlobalPoint  monoTsosGPos = monoState.globalPosition();
	GlobalError  monoTsosGPEr = monoState.cartesianError().position();

	GlobalPoint monoRhitGPos = tMonoHit->globalPosition();
	GlobalError monoRhitGPEr = tMonoHit->globalPositionError();

	double pullGPX_rs_mono = (monoRhitGPos.x()-monoShitGPos.x())/sqrt(monoRhitGPEr.cxx());
	double pullGPY_rs_mono = (monoRhitGPos.y()-monoShitGPos.y())/sqrt(monoRhitGPEr.cyy());
	double pullGPZ_rs_mono = (monoRhitGPos.z()-monoShitGPos.z())/sqrt(monoRhitGPEr.czz());
	//double pullGPX_rs_mono = (monoRhitGPos.x()-monoShitGPos.x());
	//double pullGPY_rs_mono = (monoRhitGPos.y()-monoShitGPos.y());
	//double pullGPZ_rs_mono = (monoRhitGPos.z()-monoShitGPos.z());

	title.str("");
	title << "PullGP_X_" << subdetId << "-" << layerId << "_rs_mono";
	hPullGP_X_rs_mono[title.str()]->Fill( pullGPX_rs_mono );
	title.str("");
	title << "PullGP_Y_" << subdetId << "-" << layerId << "_rs_mono";
	hPullGP_Y_rs_mono[title.str()]->Fill( pullGPY_rs_mono );
	title.str("");
	title << "PullGP_Z_" << subdetId << "-" << layerId << "_rs_mono";
	hPullGP_Z_rs_mono[title.str()]->Fill( pullGPZ_rs_mono );

	double pullGPX_tr_mono = (monoTsosGPos.x()-monoRhitGPos.x())/sqrt(monoTsosGPEr.cxx()+monoRhitGPEr.cxx());
	double pullGPY_tr_mono = (monoTsosGPos.y()-monoRhitGPos.y())/sqrt(monoTsosGPEr.cyy()+monoRhitGPEr.cyy());
	double pullGPZ_tr_mono = (monoTsosGPos.z()-monoRhitGPos.z())/sqrt(monoTsosGPEr.czz()+monoRhitGPEr.czz());
	//double pullGPX_tr_mono = (monoTsosGPos.x()-monoRhitGPos.x());
	//double pullGPY_tr_mono = (monoTsosGPos.y()-monoRhitGPos.y());
	//double pullGPZ_tr_mono = (monoTsosGPos.z()-monoRhitGPos.z());

	title.str("");
	title << "PullGP_X_" << subdetId << "-" << layerId << "_tr_mono";
	hPullGP_X_tr_mono[title.str()]->Fill( pullGPX_tr_mono );
	title.str("");
	title << "PullGP_Y_" << subdetId << "-" << layerId << "_tr_mono";
	hPullGP_Y_tr_mono[title.str()]->Fill( pullGPY_tr_mono );
	title.str("");
	title << "PullGP_Z_" << subdetId << "-" << layerId << "_tr_mono";
	hPullGP_Z_tr_mono[title.str()]->Fill( pullGPZ_tr_mono );

	double pullGPX_ts_mono = (monoTsosGPos.x()-monoShitGPos.x())/sqrt(monoTsosGPEr.cxx());
	double pullGPY_ts_mono = (monoTsosGPos.y()-monoShitGPos.y())/sqrt(monoTsosGPEr.cyy());
	double pullGPZ_ts_mono = (monoTsosGPos.z()-monoShitGPos.z())/sqrt(monoTsosGPEr.czz());
	//double pullGPX_ts_mono = (monoTsosGPos.x()-monoShitGPos.x());
	//double pullGPY_ts_mono = (monoTsosGPos.y()-monoShitGPos.y());
	//double pullGPZ_ts_mono = (monoTsosGPos.z()-monoShitGPos.z());

	title.str("");
	title << "PullGP_X_" << subdetId << "-" << layerId << "_ts_mono";
	hPullGP_X_ts_mono[title.str()]->Fill( pullGPX_ts_mono );
	title.str("");
	title << "PullGP_Y_" << subdetId << "-" << layerId << "_ts_mono";
	hPullGP_Y_ts_mono[title.str()]->Fill( pullGPY_ts_mono );
	title.str("");
	title << "PullGP_Z_" << subdetId << "-" << layerId << "_ts_mono";
	hPullGP_Z_ts_mono[title.str()]->Fill( pullGPZ_ts_mono );

	double pullGMX_ts_mono = (monoTsosGMom.x()-monoShitGMom.x())/sqrt(monoTsosGMEr.cxx());
	double pullGMY_ts_mono = (monoTsosGMom.y()-monoShitGMom.y())/sqrt(monoTsosGMEr.cyy());
	double pullGMZ_ts_mono = (monoTsosGMom.z()-monoShitGMom.z())/sqrt(monoTsosGMEr.czz());
	//double pullGMX_ts_mono = (monoTsosGMom.x()-monoShitGMom.x());
	//double pullGMY_ts_mono = (monoTsosGMom.y()-monoShitGMom.y());
	//double pullGMZ_ts_mono = (monoTsosGMom.z()-monoShitGMom.z());

	title.str("");
	title << "PullGM_X_" << subdetId << "-" << layerId << "_ts_mono";
	hPullGM_X_ts_mono[title.str()]->Fill( pullGMX_ts_mono );
	title.str("");
	title << "PullGM_Y_" << subdetId << "-" << layerId << "_ts_mono";
	hPullGM_Y_ts_mono[title.str()]->Fill( pullGMY_ts_mono );
	title.str("");
	title << "PullGM_Z_" << subdetId << "-" << layerId << "_ts_mono";
	hPullGM_Z_ts_mono[title.str()]->Fill( pullGMZ_ts_mono );

	//stereo
	LogTrace("TestTrackHits") << "STEREO HIT" ;
	CTTRHp tStereoHit = 
	  theBuilder->build(dynamic_cast<const SiStripMatchedRecHit2D*>(rhit->hit())->stereoHit());
	if (tStereoHit==0) continue;
	vector<PSimHit> assStereoSimHits = hitAssociator->associateHit(*tStereoHit->hit());
	if (assStereoSimHits.size()==0) continue;
	const PSimHit sStereoHit = *(assStereoSimHits.begin());
	const Surface * stereoSurf = &( tStereoHit->det()->surface() );
	if (stereoSurf==0) continue;
	TSOS stereoState = thePropagatorAnyDir->propagate(state,*stereoSurf);
	if (stereoState.isValid()==0) continue;

	LocalVector stereoShitLMom = sStereoHit.momentumAtEntry();
	GlobalVector stereoShitGMom = stereoSurf->toGlobal(stereoShitLMom);
	LocalPoint stereoShitLPos = sStereoHit.localPosition();
	GlobalPoint stereoShitGPos = stereoSurf->toGlobal(stereoShitLPos);

	//LogTrace("TestTrackHits") << "assStereoSimHits.size()=" << assStereoSimHits.size() ;
	//LogTrace("TestTrackHits") << "stereo shit=" << stereoShitGPos ;

	GlobalVector stereoTsosGMom = stereoState.globalMomentum();
	GlobalError  stereoTsosGMEr(stereoState.cartesianError().matrix().Sub<AlgebraicSymMatrix33>(3,3));
	GlobalPoint  stereoTsosGPos = stereoState.globalPosition();
	GlobalError  stereoTsosGPEr = stereoState.cartesianError().position();

	GlobalPoint stereoRhitGPos = tStereoHit->globalPosition();
	GlobalError stereoRhitGPEr = tStereoHit->globalPositionError();

	double pullGPX_rs_stereo = (stereoRhitGPos.x()-stereoShitGPos.x())/sqrt(stereoRhitGPEr.cxx());
	double pullGPY_rs_stereo = (stereoRhitGPos.y()-stereoShitGPos.y())/sqrt(stereoRhitGPEr.cyy());
	double pullGPZ_rs_stereo = (stereoRhitGPos.z()-stereoShitGPos.z())/sqrt(stereoRhitGPEr.czz());
	// 	double pullGPX_rs_stereo = (stereoRhitGPos.x()-stereoShitGPos.x())/*/sqrt(stereoRhitGPEr.cxx())*/;
	// 	double pullGPY_rs_stereo = (stereoRhitGPos.y()-stereoShitGPos.y())/*/sqrt(stereoRhitGPEr.cyy())*/;
	// 	double pullGPZ_rs_stereo = (stereoRhitGPos.z()-stereoShitGPos.z())/*/sqrt(stereoRhitGPEr.czz())*/;

	title.str("");
	title << "PullGP_X_" << subdetId << "-" << layerId << "_rs_stereo";
	hPullGP_X_rs_stereo[title.str()]->Fill( pullGPX_rs_stereo );
	title.str("");
	title << "PullGP_Y_" << subdetId << "-" << layerId << "_rs_stereo";
	hPullGP_Y_rs_stereo[title.str()]->Fill( pullGPY_rs_stereo );
	title.str("");
	title << "PullGP_Z_" << subdetId << "-" << layerId << "_rs_stereo";
	hPullGP_Z_rs_stereo[title.str()]->Fill( pullGPZ_rs_stereo );

	double pullGPX_tr_stereo = (stereoTsosGPos.x()-stereoRhitGPos.x())/sqrt(stereoTsosGPEr.cxx()+stereoRhitGPEr.cxx());
	double pullGPY_tr_stereo = (stereoTsosGPos.y()-stereoRhitGPos.y())/sqrt(stereoTsosGPEr.cyy()+stereoRhitGPEr.cyy());
	double pullGPZ_tr_stereo = (stereoTsosGPos.z()-stereoRhitGPos.z())/sqrt(stereoTsosGPEr.czz()+stereoRhitGPEr.czz());
	//double pullGPX_tr_stereo = (stereoTsosGPos.x()-stereoRhitGPos.x());
	//double pullGPY_tr_stereo = (stereoTsosGPos.y()-stereoRhitGPos.y());
	//double pullGPZ_tr_stereo = (stereoTsosGPos.z()-stereoRhitGPos.z());

	title.str("");
	title << "PullGP_X_" << subdetId << "-" << layerId << "_tr_stereo";
	hPullGP_X_tr_stereo[title.str()]->Fill( pullGPX_tr_stereo );
	title.str("");
	title << "PullGP_Y_" << subdetId << "-" << layerId << "_tr_stereo";
	hPullGP_Y_tr_stereo[title.str()]->Fill( pullGPY_tr_stereo );
	title.str("");
	title << "PullGP_Z_" << subdetId << "-" << layerId << "_tr_stereo";
	hPullGP_Z_tr_stereo[title.str()]->Fill( pullGPZ_tr_stereo );

	double pullGPX_ts_stereo = (stereoTsosGPos.x()-stereoShitGPos.x())/sqrt(stereoTsosGPEr.cxx());
	double pullGPY_ts_stereo = (stereoTsosGPos.y()-stereoShitGPos.y())/sqrt(stereoTsosGPEr.cyy());
	double pullGPZ_ts_stereo = (stereoTsosGPos.z()-stereoShitGPos.z())/sqrt(stereoTsosGPEr.czz());
	//double pullGPX_ts_stereo = (stereoTsosGPos.x()-stereoShitGPos.x());
	//double pullGPY_ts_stereo = (stereoTsosGPos.y()-stereoShitGPos.y());
	//double pullGPZ_ts_stereo = (stereoTsosGPos.z()-stereoShitGPos.z());

	title.str("");
	title << "PullGP_X_" << subdetId << "-" << layerId << "_ts_stereo";
	hPullGP_X_ts_stereo[title.str()]->Fill( pullGPX_ts_stereo );
	title.str("");
	title << "PullGP_Y_" << subdetId << "-" << layerId << "_ts_stereo";
	hPullGP_Y_ts_stereo[title.str()]->Fill( pullGPY_ts_stereo );
	title.str("");
	title << "PullGP_Z_" << subdetId << "-" << layerId << "_ts_stereo";
	hPullGP_Z_ts_stereo[title.str()]->Fill( pullGPZ_ts_stereo );

	double pullGMX_ts_stereo = (stereoTsosGMom.x()-stereoShitGMom.x())/sqrt(stereoTsosGMEr.cxx());
	double pullGMY_ts_stereo = (stereoTsosGMom.y()-stereoShitGMom.y())/sqrt(stereoTsosGMEr.cyy());
	double pullGMZ_ts_stereo = (stereoTsosGMom.z()-stereoShitGMom.z())/sqrt(stereoTsosGMEr.czz());
	//double pullGMX_ts_stereo = (stereoTsosGMom.x()-stereoShitGMom.x());
	//double pullGMY_ts_stereo = (stereoTsosGMom.y()-stereoShitGMom.y());
	//double pullGMZ_ts_stereo = (stereoTsosGMom.z()-stereoShitGMom.z());

	title.str("");
	title << "PullGM_X_" << subdetId << "-" << layerId << "_ts_stereo";
	hPullGM_X_ts_stereo[title.str()]->Fill( pullGMX_ts_stereo );
	title.str("");
	title << "PullGM_Y_" << subdetId << "-" << layerId << "_ts_stereo";
	hPullGM_Y_ts_stereo[title.str()]->Fill( pullGMY_ts_stereo );
	title.str("");
	title << "PullGM_Z_" << subdetId << "-" << layerId << "_ts_stereo";
	hPullGM_Z_ts_stereo[title.str()]->Fill( pullGMZ_ts_stereo );
      }
    }
    LogTrace("TestTrackHits") << "traj chi2="  << tchi2 ;
    LogTrace("TestTrackHits") << "track chi2=" << track->chi2() ;
    i++;
  }
  delete hitAssociator;
  LogTrace("TestTrackHits") << "end of event" ;
}

void TestTrackHits::endJob() {
  //file->Write();
  TDirectory * chi2d = file->mkdir("Chi2Increment");

  TDirectory * gp_ts = file->mkdir("GP_TSOS-SimHit");
  TDirectory * gm_ts = file->mkdir("GM_TSOS-SimHit");
  TDirectory * gp_tr = file->mkdir("GP_TSOS-RecHit");
  TDirectory * gp_rs = file->mkdir("GP_RecHit-SimHit");

  TDirectory * gp_tsx = gp_ts->mkdir("X");
  TDirectory * gp_tsy = gp_ts->mkdir("Y");
  TDirectory * gp_tsz = gp_ts->mkdir("Z");
  TDirectory * gm_tsx = gm_ts->mkdir("X");
  TDirectory * gm_tsy = gm_ts->mkdir("Y");
  TDirectory * gm_tsz = gm_ts->mkdir("Z");
  TDirectory * gp_trx = gp_tr->mkdir("X");
  TDirectory * gp_try = gp_tr->mkdir("Y");
  TDirectory * gp_trz = gp_tr->mkdir("Z");
  TDirectory * gp_rsx = gp_rs->mkdir("X");
  TDirectory * gp_rsy = gp_rs->mkdir("Y");
  TDirectory * gp_rsz = gp_rs->mkdir("Z");

  TDirectory * gp_tsx_mono = gp_ts->mkdir("MONOX");
  TDirectory * gp_tsy_mono = gp_ts->mkdir("MONOY");
  TDirectory * gp_tsz_mono = gp_ts->mkdir("MONOZ");
  TDirectory * gm_tsx_mono = gm_ts->mkdir("MONOX");
  TDirectory * gm_tsy_mono = gm_ts->mkdir("MONOY");
  TDirectory * gm_tsz_mono = gm_ts->mkdir("MONOZ");
  TDirectory * gp_trx_mono = gp_tr->mkdir("MONOX");
  TDirectory * gp_try_mono = gp_tr->mkdir("MONOY");
  TDirectory * gp_trz_mono = gp_tr->mkdir("MONOZ");
  TDirectory * gp_rsx_mono = gp_rs->mkdir("MONOX");
  TDirectory * gp_rsy_mono = gp_rs->mkdir("MONOY");
  TDirectory * gp_rsz_mono = gp_rs->mkdir("MONOZ");

  TDirectory * gp_tsx_stereo = gp_ts->mkdir("STEREOX");
  TDirectory * gp_tsy_stereo = gp_ts->mkdir("STEREOY");
  TDirectory * gp_tsz_stereo = gp_ts->mkdir("STEREOZ");
  TDirectory * gm_tsx_stereo = gm_ts->mkdir("STEREOX");
  TDirectory * gm_tsy_stereo = gm_ts->mkdir("STEREOY");
  TDirectory * gm_tsz_stereo = gm_ts->mkdir("STEREOZ");
  TDirectory * gp_trx_stereo = gp_tr->mkdir("STEREOX");
  TDirectory * gp_try_stereo = gp_tr->mkdir("STEREOY");
  TDirectory * gp_trz_stereo = gp_tr->mkdir("STEREOZ");
  TDirectory * gp_rsx_stereo = gp_rs->mkdir("STEREOX");
  TDirectory * gp_rsy_stereo = gp_rs->mkdir("STEREOY");
  TDirectory * gp_rsz_stereo = gp_rs->mkdir("STEREOZ");

  chi2d->cd();
  hTotChi2Increment->Write();
  hTotChi2GoodHit->Write();
  hTotChi2BadHit->Write();
  hProcess_vs_Chi2->Write();
  hClsize_vs_Chi2->Write();
  hGoodHit_vs_Chi2->Write();
  for (int i=0; i!=6; i++)
    for (int j=0; j!=9; j++){
      if (i==0 && j>2) break;
      if (i==1 && j>1) break;
      if (i==2 && j>3) break;
      if (i==3 && j>2) break;
      if (i==4 && j>5) break;
      if (i==5 && j>8) break;
      chi2d->cd();
      title.str("");
      title << "Chi2Increment_" << i+1 << "-" << j+1 ;
      hChi2Increment[title.str()]->Write();
      title.str("");
      title << "Chi2GoodHit_" << i+1 << "-" << j+1 ;
      hChi2GoodHit[title.str()]->Write();
      title.str("");
      title << "Chi2BadHit_" << i+1 << "-" << j+1 ;
      hChi2BadHit[title.str()]->Write();

      gp_ts->cd();
      gp_tsx->cd();
      title.str("");
      title << "PullGP_X_" << i+1 << "-" << j+1 << "_ts";
      hPullGP_X_ts[title.str()]->Write();
      gp_tsy->cd();
      title.str("");
      title << "PullGP_Y_" << i+1 << "-" << j+1 << "_ts";
      hPullGP_Y_ts[title.str()]->Write();
      gp_tsz->cd();
      title.str("");
      title << "PullGP_Z_" << i+1 << "-" << j+1 << "_ts";
      hPullGP_Z_ts[title.str()]->Write();

      gm_ts->cd();
      gm_tsx->cd();
      title.str("");
      title << "PullGM_X_" << i+1 << "-" << j+1 << "_ts";
      hPullGM_X_ts[title.str()]->Write();
      gm_tsy->cd();
      title.str("");
      title << "PullGM_Y_" << i+1 << "-" << j+1 << "_ts";
      hPullGM_Y_ts[title.str()]->Write();
      gm_tsz->cd();
      title.str("");
      title << "PullGM_Z_" << i+1 << "-" << j+1 << "_ts";
      hPullGM_Z_ts[title.str()]->Write();

      gp_tr->cd();
      gp_trx->cd();
      title.str("");
      title << "PullGP_X_" << i+1 << "-" << j+1 << "_tr";
      hPullGP_X_tr[title.str()]->Write();
      gp_try->cd();
      title.str("");
      title << "PullGP_Y_" << i+1 << "-" << j+1 << "_tr";
      hPullGP_Y_tr[title.str()]->Write();
      gp_trz->cd();
      title.str("");
      title << "PullGP_Z_" << i+1 << "-" << j+1 << "_tr";
      hPullGP_Z_tr[title.str()]->Write();

      gp_rs->cd();
      gp_rsx->cd();
      title.str("");
      title << "PullGP_X_" << i+1 << "-" << j+1 << "_rs";
      hPullGP_X_rs[title.str()]->Write();
      gp_rsy->cd();
      title.str("");
      title << "PullGP_Y_" << i+1 << "-" << j+1 << "_rs";
      hPullGP_Y_rs[title.str()]->Write();
      gp_rsz->cd();
      title.str("");
      title << "PullGP_Z_" << i+1 << "-" << j+1 << "_rs";
      hPullGP_Z_rs[title.str()]->Write();

      if ( ((i==2||i==4)&&(j==0||j==1)) || (i==3||i==5) ){
	//mono
	gp_ts->cd();
	gp_tsx_mono->cd();
	title.str("");
	title << "PullGP_X_" << i+1 << "-" << j+1 << "_ts_mono";
	hPullGP_X_ts_mono[title.str()]->Write();
	gp_tsy_mono->cd();
	title.str("");
	title << "PullGP_Y_" << i+1 << "-" << j+1 << "_ts_mono";
	hPullGP_Y_ts_mono[title.str()]->Write();
	gp_tsz_mono->cd();
	title.str("");
	title << "PullGP_Z_" << i+1 << "-" << j+1 << "_ts_mono";
	hPullGP_Z_ts_mono[title.str()]->Write();

	gm_ts->cd();
	gm_tsx_mono->cd();
	title.str("");
	title << "PullGM_X_" << i+1 << "-" << j+1 << "_ts_mono";
	hPullGM_X_ts_mono[title.str()]->Write();
	gm_tsy_mono->cd();
	title.str("");
	title << "PullGM_Y_" << i+1 << "-" << j+1 << "_ts_mono";
	hPullGM_Y_ts_mono[title.str()]->Write();
	gm_tsz_mono->cd();
	title.str("");
	title << "PullGM_Z_" << i+1 << "-" << j+1 << "_ts_mono";
	hPullGM_Z_ts_mono[title.str()]->Write();

	gp_tr->cd();
	gp_trx_mono->cd();
	title.str("");
	title << "PullGP_X_" << i+1 << "-" << j+1 << "_tr_mono";
	hPullGP_X_tr_mono[title.str()]->Write();
	gp_try_mono->cd();
	title.str("");
	title << "PullGP_Y_" << i+1 << "-" << j+1 << "_tr_mono";
	hPullGP_Y_tr_mono[title.str()]->Write();
	gp_trz_mono->cd();
	title.str("");
	title << "PullGP_Z_" << i+1 << "-" << j+1 << "_tr_mono";
	hPullGP_Z_tr_mono[title.str()]->Write();

	gp_rs->cd();
	gp_rsx_mono->cd();
	title.str("");
	title << "PullGP_X_" << i+1 << "-" << j+1 << "_rs_mono";
	hPullGP_X_rs_mono[title.str()]->Write();
	gp_rsy_mono->cd();
	title.str("");
	title << "PullGP_Y_" << i+1 << "-" << j+1 << "_rs_mono";
	hPullGP_Y_rs_mono[title.str()]->Write();
	gp_rsz_mono->cd();
	title.str("");
	title << "PullGP_Z_" << i+1 << "-" << j+1 << "_rs_mono";
	hPullGP_Z_rs_mono[title.str()]->Write();

	//stereo
	gp_ts->cd();
	gp_tsx_stereo->cd();
	title.str("");
	title << "PullGP_X_" << i+1 << "-" << j+1 << "_ts_stereo";
	hPullGP_X_ts_stereo[title.str()]->Write();
	gp_tsy_stereo->cd();
	title.str("");
	title << "PullGP_Y_" << i+1 << "-" << j+1 << "_ts_stereo";
	hPullGP_Y_ts_stereo[title.str()]->Write();
	gp_tsz_stereo->cd();
	title.str("");
	title << "PullGP_Z_" << i+1 << "-" << j+1 << "_ts_stereo";
	hPullGP_Z_ts_stereo[title.str()]->Write();

	gm_ts->cd();
	gm_tsx_stereo->cd();
	title.str("");
	title << "PullGM_X_" << i+1 << "-" << j+1 << "_ts_stereo";
	hPullGM_X_ts_stereo[title.str()]->Write();
	gm_tsy_stereo->cd();
	title.str("");
	title << "PullGM_Y_" << i+1 << "-" << j+1 << "_ts_stereo";
	hPullGM_Y_ts_stereo[title.str()]->Write();
	gm_tsz_stereo->cd();
	title.str("");
	title << "PullGM_Z_" << i+1 << "-" << j+1 << "_ts_stereo";
	hPullGM_Z_ts_stereo[title.str()]->Write();

	gp_tr->cd();
	gp_trx_stereo->cd();
	title.str("");
	title << "PullGP_X_" << i+1 << "-" << j+1 << "_tr_stereo";
	hPullGP_X_tr_stereo[title.str()]->Write();
	gp_try_stereo->cd();
	title.str("");
	title << "PullGP_Y_" << i+1 << "-" << j+1 << "_tr_stereo";
	hPullGP_Y_tr_stereo[title.str()]->Write();
	gp_trz_stereo->cd();
	title.str("");
	title << "PullGP_Z_" << i+1 << "-" << j+1 << "_tr_stereo";
	hPullGP_Z_tr_stereo[title.str()]->Write();

	gp_rs->cd();
	gp_rsx_stereo->cd();
	title.str("");
	title << "PullGP_X_" << i+1 << "-" << j+1 << "_rs_stereo";
	hPullGP_X_rs_stereo[title.str()]->Write();
	gp_rsy_stereo->cd();
	title.str("");
	title << "PullGP_Y_" << i+1 << "-" << j+1 << "_rs_stereo";
	hPullGP_Y_rs_stereo[title.str()]->Write();
	gp_rsz_stereo->cd();
	title.str("");
	title << "PullGP_Z_" << i+1 << "-" << j+1 << "_rs_stereo";
	hPullGP_Z_rs_stereo[title.str()]->Write();
      }
    }

  file->Close();
}


//needed by to do the residual for matched hits
//taken from SiStripTrackingRecHitsValid.cc
std::pair<LocalPoint,LocalVector> 
TestTrackHits::projectHit( const PSimHit& hit, const StripGeomDetUnit* stripDet, const BoundPlane& plane) 
{ 
  const StripTopology& topol = stripDet->specificTopology();
  GlobalPoint globalpos= stripDet->surface().toGlobal(hit.localPosition());
  LocalPoint localHit = plane.toLocal(globalpos);
  //track direction
  LocalVector locdir=hit.localDirection();
  //rotate track in new frame
   
  GlobalVector globaldir= stripDet->surface().toGlobal(locdir);
  LocalVector dir=plane.toLocal(globaldir);
  float scale = -localHit.z() / dir.z();
   
  LocalPoint projectedPos = localHit + scale*dir;
   
  float selfAngle = topol.stripAngle( topol.strip( hit.localPosition()));
   
  LocalVector stripDir( sin(selfAngle), cos(selfAngle), 0); // vector along strip in hit frame
   
  LocalVector localStripDir( plane.toLocal(stripDet->surface().toGlobal( stripDir)));
   
  return std::pair<LocalPoint,LocalVector>( projectedPos, localStripDir);
}


#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_ANOTHER_FWK_MODULE(TestTrackHits);
