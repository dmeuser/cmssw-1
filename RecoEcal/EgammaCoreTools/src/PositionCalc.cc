#include "RecoEcal/EgammaCoreTools/interface/PositionCalc.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/TruncatedPyramid.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "Geometry/EcalPreshowerAlgo/interface/EcalPreshowerGeometry.h"

PositionCalc::PositionCalc(std::map<std::string,double> providedParameters)
{
  param_LogWeighted_ = providedParameters.find("LogWeighted")->second;
  param_T0_barl_ = providedParameters.find("T0_barl")->second; 
  param_T0_endc_ = providedParameters.find("T0_endc")->second; 
  param_T0_endcPresh_ = providedParameters.find("T0_endcPresh")->second; 
  param_W0_ = providedParameters.find("W0")->second;
  param_X0_ = providedParameters.find("X0")->second;

  //storedRecHitsMap_ = passedRecHitsMap;
  //storedSubdetectorGeometry_ = passedGeometry;
}

const PositionCalc& PositionCalc::operator=(const PositionCalc& rhs) {
  param_LogWeighted_ = rhs.param_LogWeighted_;
  param_T0_barl_ = rhs.param_T0_barl_;
  param_T0_endc_ = rhs.param_T0_endc_;
  param_T0_endcPresh_ = rhs.param_T0_endcPresh_;
  param_W0_ = rhs.param_W0_;
  param_X0_ = rhs.param_X0_;
  return *this;
}

math::XYZPoint PositionCalc::Calculate_Location(std::vector<DetId> passedDetIds,
                                                EcalRecHitCollection const * storedRecHitsMap_,
                                                const CaloSubdetectorGeometry * storedSubdetectorGeometry_,
						const CaloSubdetectorGeometry * storedESGeometry_)
{

  // Throw an error if the cluster was not initialized properly

  if(storedRecHitsMap_ == NULL || storedSubdetectorGeometry_ == NULL)
    throw(std::runtime_error("\n\nPositionCalc::Calculate_Location called uninitialized or wrong initialization.\n\n"));

  std::vector<DetId> validDetIds;

  // Check that DetIds are nonzero
  std::vector<DetId>::iterator n;
  for (n = passedDetIds.begin(); n != passedDetIds.end(); n++) {
    if (((*n) != DetId(0)) 
	&& (storedRecHitsMap_->find(*n) != storedRecHitsMap_->end()))
      validDetIds.push_back(*n);
  }

  passedDetIds.clear();
  passedDetIds = validDetIds;

  // Figure out what the central crystal is and also calculate the 
  // total energy

  double eTot = 0;

  DetId maxId_ = (*(passedDetIds.begin()));
  EcalRecHitCollection::const_iterator itm = storedRecHitsMap_->find(maxId_);

  double eMax = itm->energy();

  DetId id_;
  double e_i = 0;

  std::vector<DetId>::iterator i;
  for (i = passedDetIds.begin(); i !=  passedDetIds.end(); i++) {
    id_ = (*i);
    EcalRecHitCollection::const_iterator itt = storedRecHitsMap_->find(id_);

    e_i = itt->energy();
    if (e_i > eMax) {
      eMax = e_i;
      maxId_ = id_;
    }
    
    eTot += e_i;
  }
  
  //Select the correct value of the T0 parameter depending on subdetector
  float T0;
  const CaloCellGeometry* center_cell = storedSubdetectorGeometry_->getGeometry(maxId_);
  GlobalPoint p = center_cell->getPosition();
  if (fabs(p.eta())<1.479) {
    //barrel
    T0 = param_T0_barl_;
  } else {
    DetId preshDet;
    if (storedESGeometry_) {
      preshDet = (dynamic_cast<const EcalPreshowerGeometry*>(storedESGeometry_))->getClosestCell(p);
    }
    if (preshDet.null()) {
      //endcap, not behind preshower
      T0 = param_T0_endc_;
    } else {
      //endcap, behind preshower
      T0 = param_T0_endcPresh_;
    }
  }

  // Calculate shower depth
  float depth = 0.;
  if(eTot<=0.) {
    edm::LogError("NegativeClusterEnergy") << "cluster with negative energy: " << eTot
                                      << " setting depth to 0.";
  } else {
    depth = param_X0_ * (T0 + log(eTot));
  }

  // Get position of center cell from shower depth
  GlobalPoint center_pos = 
    (dynamic_cast<const TruncatedPyramid*>(center_cell))->getPosition(depth);
  

  // Loop over hits and get weights
  double weight = 0;
  double total_weight = 0;

  double center_phi = center_pos.phi();
  double center_theta = center_pos.theta();

  double delta_theta = 0;
  double delta_phi = 0;

  double dphi = 0;

  std::vector<DetId>::iterator j;
  for (j = passedDetIds.begin(); j != passedDetIds.end(); j++) {
    id_ = (*j);
    EcalRecHitCollection::const_iterator itj = storedRecHitsMap_->find(id_);
    double e_j = itj->energy();

    if (param_LogWeighted_) {
       if(eTot<=0.) {
         weight = 0.;
       } else {
         weight = std::max(0., param_W0_ + log( fabs(e_j)/eTot) );
       }
    } else {
      weight = e_j/eTot;
    }
    
    total_weight += weight;
  
    const CaloCellGeometry* jth_cell = 
      storedSubdetectorGeometry_->getGeometry(id_);
    GlobalPoint jth_pos = 
      dynamic_cast<const TruncatedPyramid*>(jth_cell)->getPosition(depth);

    delta_theta += weight * (jth_pos.theta() - center_theta);
    dphi = (jth_pos.phi() - center_phi);

    // Check the 2*pi problem for delta_phi
    if (dphi > M_PI)
      dphi -= 2.*M_PI;
    if (dphi < -M_PI)
      dphi += 2.*M_PI;

    delta_phi += dphi*weight;    
  }

  delta_theta /= total_weight;
  delta_phi /= total_weight;
  
  double cluster_theta = center_theta + delta_theta;
  double cluster_phi = center_phi + delta_phi;

  // Check the 2*pi problem for cluster_phi
  if (cluster_phi > M_PI)
    cluster_phi -= 2.*M_PI;
  if (cluster_phi < -M_PI)
    cluster_phi += 2.*M_PI;

  double radius = sqrt(center_pos.x()*center_pos.x()
		       + center_pos.y()*center_pos.y()
		       + center_pos.z()*center_pos.z());

  double xpos = radius*cos(cluster_phi)*sin(cluster_theta);
  double ypos = radius*sin(cluster_phi)*sin(cluster_theta);
  double zpos = radius*cos(cluster_theta);

  return math::XYZPoint(xpos, ypos, zpos);
  
}

std::map<std::string,double> PositionCalc::Calculate_Covariances(math::XYZPoint passedPoint,
                                                                 std::vector<DetId> passedDetIds,
                                                                 const EcalRecHitCollection* hits,
                                                                 const CaloSubdetectorGeometry * geometry)
{

  std::vector<DetId> validDetIds;

  // Check that DetIds are nonzero
  std::vector<DetId>::iterator m;
  for (m = passedDetIds.begin(); m != passedDetIds.end(); m++) {
    if (((*m) != DetId(0))
	&& (hits->find(*m) != hits->end()))
      validDetIds.push_back(*m);
  }

  passedDetIds.clear();
  passedDetIds = validDetIds;

  // Check to see that PositionCalc was initialized.  Throw an error if not.

  if(hits == NULL ||geometry == NULL)
    throw(std::runtime_error("\n\nPositionCalc::Calculate_Covariance called uninitialized or wrong initialization.\n\n"));

  // Init cov variable
  double covEtaEta=0, covEtaPhi=0, covPhiPhi=0;
  double eTot = 0.;

  // Sum total energy for weighting
  std::vector<DetId>::iterator n;

  for (n = passedDetIds.begin(); n != passedDetIds.end(); n++) {
    EcalRecHitCollection::const_iterator itt = hits->find(*n);
    eTot += itt->energy();
  }

  // Main loop to calculate covariances
    
  if (eTot > 0.) {  

    // Find eta, phi of passedPoint 
    double pX = passedPoint.x();
    double pY = passedPoint.y();

    double pEta = passedPoint.Eta();
    double pPhi = atan2(pY,pX);

    // Init variables for kth cell
    double kX = 0., kY = 0., kZ = 0., kEta = 0., kPhi = 0., weight = 0., wTot = 0., w_min = 0.;

    std::vector<DetId>::iterator k;
    for (k = passedDetIds.begin(); k != passedDetIds.end(); k++) {

      // Find out what the physical location of the kth cell is
      DetId id_ = *k;
      const CaloCellGeometry *this_cell = geometry->getGeometry(id_);
      GlobalPoint posi = this_cell->getPosition();
      math::XYZPoint posi2(posi.x(),posi.y(),posi.z());

      kX = posi.x();
      kY = posi.y();
      kZ = posi.z();

      kEta = posi2.Eta();
      kPhi = atan2(kY,kX);
      EcalRecHitCollection::const_iterator itk = hits->find(*k);
      double e_k = itk->energy();

      // Do the log weighting

      if (param_LogWeighted_) {
        weight = std::max(w_min, param_W0_ + log( fabs(e_k)/eTot ) );
      } else { // Or else arithmetic weighting
        weight = e_k;
      }

      // Increment covariances
      // Corrected to use proper weighting -TK
      covEtaEta += weight*(kEta - pEta)*(kEta - pEta);
      covEtaPhi += weight*(kEta - pEta)*(kPhi - pPhi);
      covPhiPhi += weight*(kPhi - pPhi)*(kPhi - pPhi);

      wTot += weight;

    }

    covEtaEta /= wTot;
    covEtaPhi /= wTot;
    covPhiPhi /= wTot;

  } else {

    // Warn the user if there was no energy in the cells and return zeroes.

    std::cout << "\nPositionCalc::Calculate_Covariances:  no energy in supplied cells.\n";

    covEtaEta = 0;
    covEtaPhi = 0;
    covPhiPhi = 0;
  }

  // Build a map of the covariances.

  std::map<std::string, double> covMap;

  covMap.insert(std::make_pair("covEtaEta",covEtaEta));
  covMap.insert(std::make_pair("covEtaPhi",covEtaPhi));
  covMap.insert(std::make_pair("covPhiPhi",covPhiPhi));

  return covMap;
}

