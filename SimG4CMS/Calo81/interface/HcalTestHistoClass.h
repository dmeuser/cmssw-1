#ifndef SimG4CMS_HcalTestHistoClass_H
#define SimG4CMS_HcalTestHistoClass_H
///////////////////////////////////////////////////////////////////////////////
// File: HcalTestHistoClass.h
// Histogram handling class for analysis in HcalTest
///////////////////////////////////////////////////////////////////////////////

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "SimG4CMS/Calo81/interface/CaloHit.h"

#include <boost/cstdint.hpp>
#include <string>
#include <vector>
#include <memory>

class HcalTestHistoClass {

public: 

  HcalTestHistoClass(int i)     {}
  explicit HcalTestHistoClass() {}
  virtual ~HcalTestHistoClass() {}

  void setCounters();
  void fillLayers (double el[], double ho, double hbhe, double muxy[]);
  void fillHits   (std::vector<CaloHit>);
  void fillQie    (int id, double esimtot, double eqietot, int nGroup,
		   std::vector<double> longs,  std::vector<double> longq,
		   int nTower, std::vector<double> latphi, 
		   std::vector<double> latfs, std::vector<double> latfq);

  struct Layer {
    Layer() {}
    float e;
    float muDist;
  };

  struct Hit {
    Hit() {}
    int   layer;
    int   id;
    float eta;
    float phi;
    float e;
    float t;
    float jitter;
  };

  struct QIE {
    QIE() {}
    float sim;
    float qie;
    int   id;
    std::vector<float> lats, latq;
    std::vector<float> lngs, lngq;
    std::vector<int>   tow;
  };

private:

  const static int   nLayersMAX = 20;
  int                nLayers;
  std::vector<Layer> layers;
  float              eHO, eHBHE;
 
  int                nHits; 
  std::vector<Hit>   hits;

  int                nQIE, nTowerQIE, nGroupQIE;
  std::vector<QIE>   qie;

};

#endif
