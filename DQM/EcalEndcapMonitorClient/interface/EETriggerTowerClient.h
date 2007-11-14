#ifndef EETriggerTowerClient_H
#define EETriggerTowerClient_H

/*
 * \file EETriggerTowerClient.h
 *
 * $Date: 2007/11/13 13:20:51 $
 * $Revision: 1.9 $
 * \author G. Della Ricca
 * \author F. Cossutti
 *
*/

#include <vector>
#include <string>

#include "TROOT.h"
#include "TProfile2D.h"
#include "TH1F.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQM/EcalEndcapMonitorClient/interface/EEClient.h"

class MonitorElement;
class MonitorUserInterface;
class DaqMonitorBEInterface;
class EcalCondDBInterface;
class RunIOV;
class MonRunIOV;

class EETriggerTowerClient : public EEClient {

friend class EESummaryClient;

public:

/// Constructor
EETriggerTowerClient(const edm::ParameterSet& ps);

/// Destructor
virtual ~EETriggerTowerClient();

/// Subscribe/Unsubscribe to Monitoring Elements
void subscribe(void);
void subscribeNew(void);
void unsubscribe(void);

/// softReset
void softReset(void);

/// Analyze
void analyze(void);

/// BeginJob
void beginJob(MonitorUserInterface* mui);

/// EndJob
void endJob(void);

/// BeginRun
void beginRun(void);

/// EndRun
void endRun(void);

/// Setup
void setup(void);

/// Cleanup
void cleanup(void);

/// HtmlOutput
void htmlOutput(int run, string htmlDir, string htmlName);

/// WriteDB
bool writeDb(EcalCondDBInterface* econn, RunIOV* runiov, MonRunIOV* moniov);

/// Get Functions
inline int getEvtPerJob() { return ievt_; }
inline int getEvtPerRun() { return jevt_; }

private:

 void subscribe(const char* nameext, const char* folder, bool emulated);
 void subscribeNew(const char* nameext, const char* folder, bool emulated);
 void unsubscribe(const char* nameext, const char* folder, bool emulated);
 void analyze(const char* nameext, const char* folder, bool emulated);

int ievt_;
int jevt_;

bool cloneME_;

bool verbose_;

bool enableMonitorDaemon_;

string prefixME_;

vector<int> superModules_;

MonitorUserInterface* mui_;
DaqMonitorBEInterface* dbe_;

MonitorElement* meh01_[18];
MonitorElement* meh02_[18];
MonitorElement* mei01_[18];
MonitorElement* mei02_[18];
MonitorElement* mej01_[18];
MonitorElement* mej02_[18];

MonitorElement* mel01_[18];
MonitorElement* mem01_[18];
MonitorElement* men01_[18];

TH3F* h01_[18];
TH3F* h02_[18];
TH3F* i01_[18];
TH3F* i02_[18];
TH3F* j01_[18];
TH3F* j02_[18];

TH2F* l01_[18];
TH3F* m01_[18];
TH3F* n01_[18];

//MonitorElement* mek01_[18][34];
//MonitorElement* mek02_[18][34];

//TH1F* k01_[18][34];
//TH1F* k02_[18][34];

MonitorElement* me_h01_[18];
MonitorElement* me_h02_[18];
MonitorElement* me_i01_[18][2];
MonitorElement* me_i02_[18][2];
MonitorElement* me_j01_[18][6];
MonitorElement* me_j02_[18][6];

};

#endif
