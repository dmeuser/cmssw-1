#ifndef EETimingTask_H
#define EETimingTask_H

/*
 * \file EETimingTask.h
 *
 * $Date: 2007/04/05 14:54:03 $
 * $Revision: 1.3 $
 * \author G. Della Ricca
 *
*/

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DQMServices/Core/interface/MonitorElement.h"

class EETimingTask: public edm::EDAnalyzer{

public:

/// Constructor
EETimingTask(const edm::ParameterSet& ps);

/// Destructor
virtual ~EETimingTask();

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

edm::InputTag EcalUncalibratedRecHitCollection_;

MonitorElement* meTimeMap_[18];

bool init_;

};

#endif
