#include "DPGAnalysis/SiStripTools/interface/RunHistogramManager.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"

BaseHistoParams::BaseHistoParams() { }
BaseHistoParams::~BaseHistoParams() { }

RunHistogramManager::RunHistogramManager():
  _histograms() { }

TH1F** RunHistogramManager::makeTH1F(const char* name, const char* title, const unsigned int nbinx, const double xmin, const double xmax) {

  TH1F** pointer =new TH1F*(0);

  BaseHistoParams* hp = new HistoParams<TH1F>(pointer,"TH1F",name,title,nbinx,xmin,xmax);
  _histograms.push_back(hp);

  LogDebug("TH1Fmade") << "Histogram " << name << " " << title << " pre-booked:" << _histograms.size();

  return pointer;

}

RunHistogramManager::~RunHistogramManager() {

  for(std::vector<BaseHistoParams*>::const_iterator hp=_histograms.begin();hp!=_histograms.end();++hp) {

    delete *hp;

  }
  LogDebug("Destructor") << "All BaseHistoParams destroyed ";

}

TProfile** RunHistogramManager::makeTProfile(const char* name, const char* title, const unsigned int nbinx, const double xmin, const double xmax) {

  TProfile** pointer =new TProfile*(0);

  BaseHistoParams* hp = new HistoParams<TProfile>(pointer,"TProfile",name,title,nbinx,xmin,xmax);
  _histograms.push_back(hp);

  LogDebug("TProfilemade") << "Histogram " << name << " " << title << " pre-booked:" << _histograms.size();

  return pointer;

}

TH2F** RunHistogramManager::makeTH2F(const char* name, const char* title, const unsigned int nbinx, const double xmin, const double xmax, const unsigned int nbiny, const double ymin, const double ymax ) {

  TH2F** pointer  = new TH2F*(0);

  BaseHistoParams* hp = new HistoParams<TH2F>(pointer,"TH2F",name,title,nbinx,xmin,xmax,nbiny,ymin,ymax);
  _histograms.push_back(hp);

  LogDebug("TH2Fmade") << "Histogram " << name << " " << title << " pre-booked :" << _histograms.size();

  return pointer;
}

void  RunHistogramManager::beginRun(const edm::Run& iRun) {
  
  // create/go to the run subdirectory
  
  edm::Service<TFileService> tfserv;
  char dirname[300];
  sprintf(dirname,"run_%d",iRun.run());
  TFileDirectory subrun = tfserv->mkdir(dirname);
  
  // loop on the histograms and update the pointer references
  
  for(unsigned int ih=0;ih<_histograms.size();++ih) {
    
    _histograms[ih]->beginRun(iRun,subrun);
    
  }
}

