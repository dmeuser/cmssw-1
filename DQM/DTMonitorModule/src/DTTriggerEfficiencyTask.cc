/*
 * \file DTTriggerEfficiencyTask.cc
 * 
 * $Date: 2011/06/14 14:23:54 $
 * $Revision: 1.7 $
 * \author C.Battilana - CIEMAT
 *
*/

#include "DQM/DTMonitorModule/src/DTTriggerEfficiencyTask.h"

// Framework
#include "FWCore/Framework/interface/EventSetup.h"

// DT trigger
#include "DQM/DTMonitorModule/interface/DTTrigGeomUtils.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhContainer.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambThContainer.h"

// Geometry
#include "DataFormats/GeometryVector/interface/Pi.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/DTGeometry/interface/DTLayer.h"
#include "Geometry/DTGeometry/interface/DTSuperLayer.h"
#include "Geometry/DTGeometry/interface/DTTopology.h"

// DT Digi
#include <DataFormats/DTDigi/interface/DTDigi.h>
#include <DataFormats/DTDigi/interface/DTDigiCollection.h>


//Root
#include"TH1.h"
#include"TAxis.h"

#include <sstream>
#include <iostream>
#include <fstream>


using namespace edm;
using namespace std;

DTTriggerEfficiencyTask::DTTriggerEfficiencyTask(const edm::ParameterSet& ps) : trigGeomUtils(0) {
  
  LogTrace ("DTDQM|DTMonitorModule|DTTriggerEfficiencyTask")  << "[DTTriggerEfficiencyTask]: Constructor" << endl;

  parameters = ps;
  dbe = edm::Service<DQMStore>().operator->();

}


DTTriggerEfficiencyTask::~DTTriggerEfficiencyTask() {

  LogTrace ("DTDQM|DTMonitorModule|DTTriggerEfficiencyTask")  << "[DTTriggerEfficiencyTask]: analyzed " << nevents << " events" << endl;

}


void DTTriggerEfficiencyTask::beginJob(){

  LogTrace ("DTDQM|DTMonitorModule|DTTriggerEfficiencyTask") << "[DTTriggerEfficiencyTask]: BeginJob" << endl;

  inputTagDCC = parameters.getUntrackedParameter<edm::InputTag>("inputTagDCC");
  inputTagDDU = parameters.getUntrackedParameter<edm::InputTag>("inputTagDDU");
  inputTagSEG = parameters.getUntrackedParameter<edm::InputTag>("inputTagSEG");
  inputTagGMT = parameters.getUntrackedParameter<edm::InputTag>("inputTagGMT");

  detailedPlots = parameters.getUntrackedParameter<bool>("detailedAnalysis");
  processDCC = parameters.getUntrackedParameter<bool>("processDCC");
  processDDU = parameters.getUntrackedParameter<bool>("processDDU");
  minBXDDU = parameters.getUntrackedParameter<int>("minBXDDU");
  maxBXDDU = parameters.getUntrackedParameter<int>("maxBXDDU");

  nMinHitsPhi = parameters.getUntrackedParameter<int>("nMinHitsPhi");
  phiAccRange = parameters.getUntrackedParameter<double>("phiAccRange");

  if (processDCC) processTags.push_back("DCC");
  if (processDDU) processTags.push_back("DDU");
  
}

void DTTriggerEfficiencyTask::beginRun(const edm::Run& run, const edm::EventSetup& context){

  LogTrace ("DTDQM|DTMonitorModule|DTTriggerEfficiencyTask") << "[DTTriggerEfficiencyTask]: BeginRun" << endl;

  nevents = 0;

  for (int wh=-2;wh<=2;++wh){
    vector<string>::const_iterator tagIt  = processTags.begin();
    vector<string>::const_iterator tagEnd = processTags.end();
    for (; tagIt!=tagEnd; ++tagIt) {
      bookWheelHistos(wh,(*tagIt),"Task");
      if (detailedPlots) {
	for (int stat=1;stat<=4;++stat){
	  for (int sect=1;sect<=12;++sect){
	    bookChamberHistos(DTChamberId(wh,stat,sect),(*tagIt),"Segment");
	  }
	}
      }
    }
  }

  context.get<MuonGeometryRecord>().get(muonGeom);
  trigGeomUtils = new DTTrigGeomUtils(muonGeom);

}

void DTTriggerEfficiencyTask::beginLuminosityBlock(const LuminosityBlock& lumiSeg, const EventSetup& context) {

  LogTrace ("DTDQM|DTMonitorModule|DTTriggerEfficiencyTask") <<"[DTTriggerEfficiencyTask]: Begin of LS transition"<<endl;
  
}


void DTTriggerEfficiencyTask::endJob(){

  LogTrace ("DTDQM|DTMonitorModule|DTTriggerEfficiencyTask")  << "[DTTriggerEfficiencyTask]: analyzed " << nevents << " events" << endl;

}


void DTTriggerEfficiencyTask::analyze(const edm::Event& e, const edm::EventSetup& c){

  nevents++;

  if (!hasRPCTriggers(e)) { return; }

  map<DTChamberId,const L1MuDTChambPhDigi*> phBestDCC;
  map<DTChamberId,const DTLocalTrigger*>    phBestDDU;
  
  // Getting best DCC Stuff
  edm::Handle<L1MuDTChambPhContainer> l1DTTPGPh;
  e.getByLabel(inputTagDCC,l1DTTPGPh);
  vector<L1MuDTChambPhDigi>*  phTrigs = l1DTTPGPh->getContainer();
  
  vector<L1MuDTChambPhDigi>::const_iterator iph  = phTrigs->begin();
  vector<L1MuDTChambPhDigi>::const_iterator iphe = phTrigs->end();
  for(; iph !=iphe ; ++iph) {
    
    int phwheel = iph->whNum();
    int phsec   = iph->scNum() + 1; // DTTF numbering [0:11] -> DT numbering [1:12]
    int phst    = iph->stNum();
    int phcode  = iph->code();

    DTChamberId chId(phwheel,phst,phsec);

    if( phcode < 7 && (phBestDCC.find(chId) == phBestDCC.end() || 
		       phcode>phBestDCC[chId]->code()) ) phBestDCC[chId] = &(*iph);
  }
  
  //Getting Best DDU Stuff
  Handle<DTLocalTriggerCollection> trigsDDU;
  e.getByLabel(inputTagDDU,trigsDDU);
  DTLocalTriggerCollection::DigiRangeIterator detUnitIt;

  for (detUnitIt=trigsDDU->begin();detUnitIt!=trigsDDU->end();++detUnitIt){
      
    const DTChamberId& id = (*detUnitIt).first;
    const DTLocalTriggerCollection::Range& range = (*detUnitIt).second;

    DTLocalTriggerCollection::const_iterator trigIt  = range.first;
    DTLocalTriggerCollection::const_iterator trigEnd = range.second;
    for (; trigIt!= trigEnd;++trigIt){
      int quality = trigIt->quality();
      if(quality>-1 && quality<7 &&
	 (phBestDDU.find(id) == phBestDDU.end() || 
	  quality>phBestDDU[id]->quality()) ) phBestDDU[id] = &(*trigIt);
    }
    
  }

  //Getting Best Segments
  vector<const DTRecSegment4D*> best4DSegments;

  Handle<DTRecSegment4DCollection> segments4D;
  e.getByLabel(inputTagSEG, segments4D);
  DTRecSegment4DCollection::const_iterator track;
  DTRecSegment4DCollection::id_iterator chamberId;

  for (chamberId = segments4D->id_begin(); chamberId != segments4D->id_end(); ++chamberId){
    
    DTRecSegment4DCollection::range  range = segments4D->get(*chamberId);
    const DTRecSegment4D* tmpBest=0;
    int tmpHit = 0;
    int hit = 0;

    for ( track = range.first; track != range.second; ++track){
      if( (*track).hasPhi() ) {
	hit = (*track).phiSegment()->degreesOfFreedom()+2;
	if ( hit>tmpHit ){
	  tmpBest = &(*track);
	  tmpHit = hit;
	  int sec = (*track).chamberId().sector();
	  if (sec==13){
	    sec=4;
	  }
	  else if (sec==14){
	    sec=10;
	  }
	}
      }
    }
    if (tmpBest) {
      best4DSegments.push_back(tmpBest);
    }
  }
    
  // Plot filling
  vector<const DTRecSegment4D*>::const_iterator btrack;
  for ( btrack = best4DSegments.begin(); btrack != best4DSegments.end(); ++btrack ){

    int wheel    = (*btrack)->chamberId().wheel();
    int station  = (*btrack)->chamberId().station();
    int scsector = 0;
    float x, xdir, y, ydir;
    trigGeomUtils->computeSCCoordinates((*btrack),scsector,x,xdir,y,ydir);
    int nHitsPhi = (*btrack)->phiSegment()->degreesOfFreedom()+2;	
    DTChamberId dtChId(wheel,station,scsector);
    uint32_t indexCh = dtChId.rawId(); 
    map<string, MonitorElement*> &innerChME = chamberHistos[indexCh];
    map<string, MonitorElement*> &innerWhME = wheelHistos[wheel];
    
    if (fabs(xdir)<phiAccRange && nHitsPhi>=nMinHitsPhi){

      vector<string>::const_iterator tagIt  = processTags.begin();
      vector<string>::const_iterator tagEnd = processTags.end();

      for (; tagIt!=tagEnd; ++tagIt) { 

	int qual   = (*tagIt) == "DCC" ?
	  phBestDCC.find(dtChId) != phBestDCC.end() ? phBestDCC[dtChId]->code() : -1 :
	  phBestDDU.find(dtChId) != phBestDDU.end() ? phBestDDU[dtChId]->quality() : -1;

	innerWhME.find((*tagIt) + "_TrigEffDenum")->second->Fill(scsector,station);
	if ( qual>=0 && qual<7 ) {
	  innerWhME.find((*tagIt) + "_TrigEffNum")->second->Fill(scsector,station);
	  if ( qual>=4 ) {
	    innerWhME.find((*tagIt) + "_TrigEffCorrNum")->second->Fill(scsector,station);
	  }
	}
	if (detailedPlots) {
	  innerChME.find((*tagIt) + "_TrackPosvsAngle")->second->Fill(xdir,x);
	  if ( qual>=0 && qual<7 ) {
	    innerChME.find((*tagIt) + "_TrackPosvsAngleAnyQual")->second->Fill(xdir,x);
	    if ( qual>=4 ) {
	      innerChME.find((*tagIt) + "_TrackPosvsAngleCorr")->second->Fill(xdir,x);	
	    }
	  }
	}
      }
    }
  }
  
}

bool DTTriggerEfficiencyTask::hasRPCTriggers(const edm::Event& e) {

  edm::Handle<L1MuGMTReadoutCollection> gmtrc; 
  e.getByLabel(inputTagGMT,gmtrc);

  std::vector<L1MuGMTReadoutRecord> gmt_records = gmtrc->getRecords();
  std::vector<L1MuGMTReadoutRecord>::const_iterator igmtrr = gmt_records.begin();
  std::vector<L1MuGMTReadoutRecord>::const_iterator egmtrr = gmt_records.end();
  for(; igmtrr!=egmtrr; igmtrr++) {

    std::vector<L1MuGMTExtendedCand> candsGMT = igmtrr->getGMTCands();
    std::vector<L1MuGMTExtendedCand>::const_iterator candGMTIt   = candsGMT.begin();
    std::vector<L1MuGMTExtendedCand>::const_iterator candGMTEnd  = candsGMT.end();

    for(; candGMTIt!=candGMTEnd; ++candGMTIt){
      if(!candGMTIt->empty()) {
	int quality = candGMTIt->quality();
	if(candGMTIt->bx()==0 && 
	   (quality == 5 || quality == 7)){
	  return true;
	}
      }
    }
  }

  return false;
  
}

void DTTriggerEfficiencyTask::bookChamberHistos(const DTChamberId& dtCh, string histoType, string folder) {
  
  int wh = dtCh.wheel();		
  int sc = dtCh.sector();	
  int st = dtCh.station();
  stringstream wheel; wheel << wh;	
  stringstream station; station << st;	
  stringstream sector; sector << sc;	

  string hwFolder      = topFolder(histoType); 
  string bookingFolder = hwFolder + "Wheel" + wheel.str() + "/Sector" + sector.str() + "/Station" + station.str() + "/" + folder;
  string histoTag      = "_W" + wheel.str() + "_Sec" + sector.str() + "_St" + station.str();

  dbe->setCurrentFolder(bookingFolder);
    
  LogTrace ("DTDQM|DTMonitorModule|DTTriggerEfficiencyTask") 
    << "[DTTriggerEfficiencyTask]: booking histos in " << bookingFolder << endl;
  
  float min, max;
  int nbins;
  trigGeomUtils->phiRange(dtCh,min,max,nbins,20);

  string histoName = histoType + "_TrackPosvsAngle" +  histoTag;
  string histoLabel = "Position vs Angle (phi)";
  (chamberHistos[dtCh.rawId()])[histoType + "_TrackPosvsAngle"] = 
    dbe->book2D(histoName,histoLabel,12,-30.,30.,nbins,min,max);

  histoName = histoType + "_TrackPosvsAngleAnyQual" +  histoTag;
  histoLabel = "Position vs Angle (phi) for any qual triggers";
  (chamberHistos[dtCh.rawId()])[histoType + "_TrackPosvsAngleAnyQual"] = 
    dbe->book2D(histoName,histoLabel,12,-30.,30.,nbins,min,max);

  histoName = histoType + "_TrackPosvsAngleCorr" +  histoTag;
  histoLabel = "Position vs Angle (phi) for correlated triggers";
  (chamberHistos[dtCh.rawId()])[histoType + "_TrackPosvsAngleCorr"] = 
    dbe->book2D(histoName,histoLabel,12,-30.,30.,nbins,min,max);
  
}

void DTTriggerEfficiencyTask::bookWheelHistos(int wheel,string hTag,string folder) {  

  stringstream wh; wh << wheel;
  string basedir;  
  if (hTag.find("Summary") != string::npos ) {
    basedir = topFolder(hTag);   //Book summary histo outside folder directory
  } else {
    basedir = topFolder(hTag) + folder + "/" ;
  }

  dbe->setCurrentFolder(basedir);

  string hTagName = "_W" + wh.str();

  LogTrace("DTDQM|DTMonitorModule|DTTriggerEfficiencyTask") 
    << "[DTTriggerEfficiencyTask]: booking histos in "<< basedir << endl;  

  string hName = hTag + "_TrigEffDenum" + hTagName;
  MonitorElement* me = dbe->book2D(hName.c_str(),hName.c_str(),12,1,13,4,1,5);
  
  me->setBinLabel(1,"MB1",2);
  me->setBinLabel(2,"MB2",2);
  me->setBinLabel(3,"MB3",2);
  me->setBinLabel(4,"MB4",2);
  me->setAxisTitle("Sector",1);
  
  wheelHistos[wheel][hTag + "_TrigEffDenum"] = me;

  hName = hTag + "_TrigEffNum" + hTagName;
  me = dbe->book2D(hName.c_str(),hName.c_str(),12,1,13,4,1,5);
  
  me->setBinLabel(1,"MB1",2);
  me->setBinLabel(2,"MB2",2);
  me->setBinLabel(3,"MB3",2);
  me->setBinLabel(4,"MB4",2);
  me->setAxisTitle("Sector",1);
  
  wheelHistos[wheel][hTag + "_TrigEffNum"] = me;

  hName = hTag + "_TrigEffCorrNum" + hTagName;
  me = dbe->book2D(hName.c_str(),hName.c_str(),12,1,13,4,1,5);
  
  me->setBinLabel(1,"MB1",2);
  me->setBinLabel(2,"MB2",2);
  me->setBinLabel(3,"MB3",2);
  me->setBinLabel(4,"MB4",2);
  me->setAxisTitle("Sector",1);
  
  wheelHistos[wheel][hTag + "_TrigEffCorrNum"] = me;

  return;
  
}

