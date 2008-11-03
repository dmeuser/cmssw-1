
/*
 * \class DQMFEDIntegrityClient
 *
 * DQM FED Client
 *
 * $Date: 2008/08/12 10:12:28 $
 * $Revision: 1.8 $
 * \author  M. Marienfeld
 *
*/

#ifndef DQMFEDIntegrityClient_H
#define DQMFEDIntegrityClient_H

#include <string>
#include <vector>

#include <FWCore/Framework/interface/EDAnalyzer.h>
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

//
// class declaration
//

class DQMFEDIntegrityClient : public edm::EDAnalyzer {
public:
  DQMFEDIntegrityClient( const edm::ParameterSet& );
  ~DQMFEDIntegrityClient();

protected:

  void beginJob(const edm::EventSetup& c);
  void beginRun(const edm::Run& r, const edm::EventSetup& c);

  void analyze(const edm::Event& e, const edm::EventSetup& c);

  void endRun(const edm::Run& r, const edm::EventSetup& c);
  void endJob();

private:

  void initialize();
 
  edm::ParameterSet parameters_;

  DQMStore * dbe_;

  // ---------- member data ----------

  int   NBINS;
  float XMIN, XMAX;
  float SummaryContent[7];

  MonitorElement * FedEntries;
  MonitorElement * FedFatal;
  MonitorElement * FedNonFatal;

  MonitorElement * reportSummary;
  MonitorElement * reportSummaryContent[7];
  MonitorElement * reportSummaryMap;

};

#endif
