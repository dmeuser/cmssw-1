#ifndef _CSCCROSSTALKDBCONDITIONS_H
#define _CSCCROSSTALKDBCONDITIONS_H

#include <memory>
#include <cmath>
#include "FWCore/Framework/interface/SourceFactory.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetupRecordIntervalFinder.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <DataFormats/MuonDetId/interface/CSCDetId.h>
#include "CondFormats/CSCObjects/interface/CSCDBCrosstalk.h"
#include "CondFormats/DataRecord/interface/CSCDBCrosstalkRcd.h"

class CSCCrosstalkDBConditions: public edm::ESProducer, public edm::EventSetupRecordIntervalFinder  {
 public:
  CSCCrosstalkDBConditions(const edm::ParameterSet&);
  ~CSCCrosstalkDBConditions();
  

  inline static CSCDBCrosstalk * prefillDBCrosstalk();

  typedef const  CSCDBCrosstalk * ReturnType;
  
  ReturnType produceDBCrosstalk(const CSCDBCrosstalkRcd&);
  
 private:
  // ----------member data ---------------------------
  void setIntervalFor(const edm::eventsetup::EventSetupRecordKey &, const edm::IOVSyncValue&, edm::ValidityInterval & );


  CSCDBCrosstalk *cndbCrosstalk ;

};

#include<fstream>
#include<vector>
#include<iostream>

// to workaround plugin library
inline CSCDBCrosstalk *  CSCCrosstalkDBConditions::prefillDBCrosstalk()
{
  const int MAX_SIZE = 217728;
  const int SLOPE_FACTOR=10000000;
  const int INTERCEPT_FACTOR=100000;
  const int MAX_SHORT= 32767;
  CSCDBCrosstalk * cndbcrosstalk = new CSCDBCrosstalk();

  int db_index,new_index;
  float db_slope_right,db_slope_left,db_intercept_right;
  float db_intercept_left, db_chi2_right,db_chi2_left;
  std::vector<int> db_index_id;
  std::vector<float> db_slope_r;
  std::vector<float> db_intercept_r;
  std::vector<float> db_chi2_r;
  std::vector<float> db_slope_l;
  std::vector<float> db_intercept_l;
  std::vector<float> db_chi2_l;
  float new_slope_right,new_slope_left,new_intercept_right;
  float new_intercept_left, new_chi2_right,new_chi2_left;
  std::vector<int> new_cham_id;
  std::vector<int> new_index_id;
  std::vector<int> new_strips;
  std::vector<float> new_slope_r;
  std::vector<float> new_intercept_r;
  std::vector<float> new_chi2_r;
  std::vector<float> new_slope_l;
  std::vector<float> new_intercept_l;
  std::vector<float> new_chi2_l;
    
  int counter;
  int db_nrlines=0;
  int new_nrlines=0;
    
  std::ifstream dbdata; 
  dbdata.open("old_dbxtalk.dat",std::ios::in); 
  if(!dbdata) {
    std::cerr <<"Error: old_dbxtalk.dat -> no such file!"<< std::endl;
    exit(1);
  }
  
  while (!dbdata.eof() ) { 
    dbdata >> db_index >> db_slope_right >> db_intercept_right >> db_chi2_right >> db_slope_left >> db_intercept_left >> db_chi2_left ;
    db_index_id.push_back(db_index);
    db_slope_r.push_back(db_slope_right);
    db_slope_l.push_back(db_slope_left);
    db_intercept_r.push_back(db_intercept_right);
    db_intercept_l.push_back(db_intercept_left);
    db_chi2_r.push_back(db_chi2_right);
    db_chi2_l.push_back(db_chi2_left); 
    db_nrlines++;
  }
  dbdata.close();

  std::ifstream newdata;
  newdata.open("xtalk.dat",std::ios::in); 
  if(!newdata) {
    std::cerr <<"Error: xtalk.dat -> no such file!"<< std::endl;
    exit(1);
  }
  
  while (!newdata.eof() ) { 
    newdata >> new_index >> new_slope_right >> new_intercept_right >> new_chi2_right >> new_slope_left >> new_intercept_left >> new_chi2_left ; 
    new_index_id.push_back(new_index);
    new_slope_r.push_back(new_slope_right);
    new_slope_l.push_back(new_slope_left);
    new_intercept_r.push_back(new_intercept_right);
    new_intercept_l.push_back(new_intercept_left);
    new_chi2_r.push_back(new_chi2_right);
    new_chi2_l.push_back(new_chi2_left);
    new_nrlines++;
  }
  newdata.close();
  
  CSCDBCrosstalk::CrosstalkContainer & itemvector = cndbcrosstalk->crosstalk;
  itemvector.resize(MAX_SIZE);
  for(int i=0; i<MAX_SIZE;++i){
    itemvector[i].xtalk_slope_right=int (db_slope_r[i]*SLOPE_FACTOR+0.5);
    itemvector[i].xtalk_intercept_right= int (db_intercept_r[i]*INTERCEPT_FACTOR+0.5); 
    itemvector[i].xtalk_slope_left= int (db_slope_l[i]*SLOPE_FACTOR+0.5);  
    itemvector[i].xtalk_intercept_left= int (db_intercept_l[i]*INTERCEPT_FACTOR+0.5);  
  }

  for(int i=0; i<MAX_SIZE;++i){
    counter=db_index_id[i];  
    for (unsigned int k=0;k<new_index_id.size()-1;k++){
      if(counter==new_index_id[k]){
	if (int (fabs(new_slope_r[k]*SLOPE_FACTOR+0.5))<MAX_SHORT)         itemvector[counter].xtalk_slope_right= int (new_slope_r[k]*SLOPE_FACTOR+0.5);
	if (int (fabs(new_intercept_r[k]*INTERCEPT_FACTOR+0.5))<MAX_SHORT) itemvector[counter].xtalk_intercept_right= int (new_intercept_r[k]*INTERCEPT_FACTOR+0.5); 
	if (int (fabs(new_slope_l[k]*SLOPE_FACTOR+0.5))<MAX_SHORT)             itemvector[counter].xtalk_slope_left= int (new_slope_l[k]*SLOPE_FACTOR+0.5);  
	if (int (fabs(new_intercept_l[k]*INTERCEPT_FACTOR+0.5))<MAX_SHORT) itemvector[counter].xtalk_intercept_left= int (new_intercept_l[k]*INTERCEPT_FACTOR+0.5);  
	itemvector[i] = itemvector[counter];
	//std::cout<<" counter "<<counter <<" dbindex "<<new_index_id[k]<<" dbslope " <<db_slope_r[k]<<" new slope "<<new_slope_r[k]<<std::endl;
      }  
    }
  }
  
  return cndbcrosstalk;

}
  
#endif

