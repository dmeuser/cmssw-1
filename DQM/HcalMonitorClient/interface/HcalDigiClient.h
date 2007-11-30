#ifndef HcalDigiClient_H
#define HcalDigiClient_H

#include "DQM/HcalMonitorClient/interface/HcalBaseClient.h"


class HcalDigiClient : public HcalBaseClient {

 public:
  
  /// Constructor
  HcalDigiClient(const ParameterSet& ps, DaqMonitorBEInterface* dbe_);
  HcalDigiClient();
  
  /// Destructor
  virtual ~HcalDigiClient();
  
  /// Analyze
  void analyze(void);
  
  /// BeginJob
  void beginJob(void);
  
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
  
  
  ///process report
  void report();
  
  /// WriteDB
  void htmlOutput(int run, string htmlDir, string htmlName);
  void getHistograms();
  void loadHistograms(TFile* f);
  
  void resetAllME();
  void createTests();

private:

  TH2F* gl_occ_geo_[4];
  TH2F* gl_occ_elec_[3];
  TH1F* gl_occ_eta_;
  TH1F* gl_occ_phi_;
  TH2F* gl_err_geo_;
  TH2F* gl_err_elec_[3];

  TH2F* sub_occ_geo_[4][4];
  TH2F* sub_occ_elec_[4][3];
  TH1F* sub_occ_eta_[4];
  TH1F* sub_occ_phi_[4];

  TH2F* sub_err_geo_[4];
  TH2F* sub_err_elec_[4][3];

  TH2F* geoRef_;
  
  TH1F* qie_adc_[4];
  TH1F* num_digi_[4];
  TH1F* qie_capid_[4];
  TH1F* qie_dverr_[4];

};

#endif
