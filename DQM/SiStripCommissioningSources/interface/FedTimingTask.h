#ifndef DQM_SiStripCommissioningSources_FedTimingTask_h
#define DQM_SiStripCommissioningSources_FedTimingTask_h

#include "DQM/SiStripCommissioningSources/interface/CommissioningTask.h"

/**
   @class FedTimingTask
*/
class FedTimingTask : public CommissioningTask {

 public:
  
  FedTimingTask( DaqMonitorBEInterface*, const FedChannelConnection& );
  virtual ~FedTimingTask();
  
 private:

  virtual void book( const FedChannelConnection& );
  virtual void fill( const SiStripEventSummary&,
		     const edm::DetSet<SiStripRawDigi>& );
  virtual void update();
  
  HistoSet timing_;

  uint16_t nBins_;

};

#endif // DQM_SiStripCommissioningSources_FedTimingTask_h

