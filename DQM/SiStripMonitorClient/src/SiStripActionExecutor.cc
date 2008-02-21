#include "DQM/SiStripMonitorClient/interface/SiStripActionExecutor.h"
#include "DQMServices/Core/interface/DaqMonitorBEInterface.h"
#include "DQM/SiStripMonitorClient/interface/SiStripSummaryCreator.h"
#include "DQM/SiStripMonitorClient/interface/SiStripTrackerMapCreator.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"


#include <iostream>
using namespace std;
//
// -- Constructor
// 
SiStripActionExecutor::SiStripActionExecutor() {
  edm::LogInfo("SiStripActionExecutor") << 
    " Creating SiStripActionExecutor " << "\n" ;
  summaryCreator_= 0;
  tkMapCreator_ = 0; 
}
//
// --  Destructor
// 
SiStripActionExecutor::~SiStripActionExecutor() {
  edm::LogInfo("SiStripActionExecutor") << 
    " Deleting SiStripActionExecutor " << "\n" ;
  if (summaryCreator_) delete   summaryCreator_;
  if (tkMapCreator_) delete   tkMapCreator_;
}
//
// -- Read Configurationn File
//
bool SiStripActionExecutor::readConfiguration() {
  
  if (!summaryCreator_) {
    summaryCreator_ = new SiStripSummaryCreator();
  }
  if (summaryCreator_->readConfiguration()) return true;
  else return false;
}
//
// -- Read Configurationn File
//
bool SiStripActionExecutor::readTkMapConfiguration() {
  
  if (tkMapCreator_) delete tkMapCreator_;
  tkMapCreator_ = new SiStripTrackerMapCreator();
  if (tkMapCreator_->readConfiguration()) return true;
  else return false;
}
//
// -- Read Configurationn File
//
bool SiStripActionExecutor::readConfiguration(int& sum_freq) {
  bool result = false;
  if (readConfiguration()) {
    sum_freq = summaryCreator_->getFrequency();
    if (sum_freq != -1) result = true;
  }
  return result;
}
//
// -- Create and Fill Summary Monitor Elements
//
void SiStripActionExecutor::createSummary(DaqMonitorBEInterface* bei) {
  if (summaryCreator_) {
    bei->cd();
    summaryCreator_->createSummary(bei);
  }
}
//
// -- create tracker map
//
void SiStripActionExecutor::createTkMap(const edm::ParameterSet & tkmapPset, 
           const edm::ESHandle<SiStripFedCabling>& fedcabling, DaqMonitorBEInterface* bei) {
  if (tkMapCreator_) tkMapCreator_->create(tkmapPset, fedcabling, bei);
}
