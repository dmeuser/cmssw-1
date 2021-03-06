#ifndef Utils_H
#define Utils_H

#include <sstream>
#include <iostream>
#include "TH1D.h"

using namespace std;
struct t_label{ stringstream text;  TH1D * h;};
struct t_data{double pt1,pt2,pt1_gen,pt2_gen,px_pt1,px_pt2,py_pt1,py_pt2,pz_pt1,pz_pt2,p_pt1,p_pt2,eta1,phi1,eta2,phi2,mass,r1,r2,rp1,rp2;};
struct t_dataW{
  double mu_pt,mu_pt_gen,nu_pt,mu_pz,mu_eta,mu_phi,mu_charge,nu_phi,WmassT,r,rp;
};
struct t_do{
  bool dosigma,dodelta,dophi,doeta,docharge;
};

class Tutils{
 public:
  void draw_label(t_label l[], double x1, double y1,double dx, double dy,int n);
  double modpt(double pt, double pt_gen, double r,double rp, const double * sigma,int mode,t_do& do_opt,double phi,double eta,int charge);
  double modpt(double pt, double pt_gen, double r,double rp, const double * sigma, int mode);
  double computeMass(t_data& data, double r1, double r2,double rp1, double rp2, const double sigma[],int mode,t_do& do_opt);
  double computeMass(t_data& data, const double sigma[],int mode,t_do& do_opt);
  
  double likelihood(TH1D * hZmassVar, TH1D * hZmassModel);
  void setTDRStyle();
};

#endif
