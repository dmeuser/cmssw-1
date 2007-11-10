#ifndef EEPedestalTask_H
#define EEPedestalTask_H

/*
 * \file EEPedestalTask.h
 *
 * $Date: 2007/08/14 17:44:47 $
 * $Revision: 1.5 $
 * \author G. Della Ricca
 *
*/

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DQMServices/Core/interface/MonitorElement.h"
class DaqMonitorBEInterface;

class EEPedestalTask: public edm::EDAnalyzer{

public:

/// Constructor
EEPedestalTask(const edm::ParameterSet& ps);

/// Destructor
virtual ~EEPedestalTask();

protected:

/// Analyze
void analyze(const edm::Event& e, const edm::EventSetup& c);

/// BeginJob
void beginJob(const edm::EventSetup& c);

/// EndJob
void endJob(void);

/// Setup
void setup(void);

/// Cleanup
void cleanup(void);

private:

int ievt_;

DaqMonitorBEInterface* dbe_;

bool enableCleanup_;

edm::InputTag EcalRawDataCollection_;
edm::InputTag EEDigiCollection_;
edm::InputTag EcalPnDiodeDigiCollection_;

MonitorElement* mePedMapG01_[18];
MonitorElement* mePedMapG06_[18];
MonitorElement* mePedMapG12_[18];

MonitorElement* mePed3SumMapG01_[18];
MonitorElement* mePed3SumMapG06_[18];
MonitorElement* mePed3SumMapG12_[18];

MonitorElement* mePed5SumMapG01_[18];
MonitorElement* mePed5SumMapG06_[18];
MonitorElement* mePed5SumMapG12_[18];

MonitorElement* mePnPedMapG01_[18];
MonitorElement* mePnPedMapG16_[18];

bool init_;

};

#endif
