#include "Geometry/HcalTowerAlgo/interface/HcalDDDGeometryLoader.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/IdealObliquePrism.h"
#include "Geometry/CaloGeometry/interface/IdealZPrism.h"
#include "Geometry/CaloEventSetup/interface/CaloGeometryLoader.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include<string>

//#define DebugLog

HcalDDDGeometryLoader::HcalDDDGeometryLoader(const DDCompactView & cpv) {
  std::string name = "HcalHits";
  numberingFromDDD = new HcalNumberingFromDDD(name, cpv);
}

HcalDDDGeometryLoader::~HcalDDDGeometryLoader() {
  delete numberingFromDDD;
}


HcalDDDGeometryLoader::ReturnType 
HcalDDDGeometryLoader::load(DetId::Detector det, int subdet) {

  HcalSubdetector  hsub        = static_cast<HcalSubdetector>(subdet);
  HcalDDDGeometry* gDDD ( new HcalDDDGeometry );
  ReturnType geom ( gDDD );

  if ( geom->cornersMgr() == 0 ) {
     const unsigned int count (numberingFromDDD->numberOfCells(HcalBarrel ) +
			       numberingFromDDD->numberOfCells(HcalEndcap ) +
			       numberingFromDDD->numberOfCells(HcalForward) +
			       numberingFromDDD->numberOfCells(HcalOuter  ) );
     geom->allocateCorners( count ) ;
  }

  //  if( geom->cornersMgr() == 0 )  geom->allocateCorners( 2592 ) ;

  if ( geom->parMgr()     == 0 ) geom->allocatePar( 500, 3 ) ;

  fill (hsub, gDDD, geom );
  return geom ;
}

HcalDDDGeometryLoader::ReturnType 
HcalDDDGeometryLoader::load() {

   HcalDDDGeometry* gDDD ( new HcalDDDGeometry );
   ReturnType geom ( gDDD );

   if( geom->cornersMgr() == 0 ) {
      const unsigned int count (numberingFromDDD->numberOfCells(HcalBarrel ) +
				numberingFromDDD->numberOfCells(HcalEndcap ) +
				numberingFromDDD->numberOfCells(HcalForward) +
				numberingFromDDD->numberOfCells(HcalOuter  ) );
     geom->allocateCorners( count ) ;
   }
   if( geom->parMgr()     == 0 ) geom->allocatePar( 500, 3 ) ;

   fill(HcalBarrel,  gDDD, geom); 
   fill(HcalEndcap,  gDDD, geom); 
   fill(HcalForward, gDDD, geom); 
   fill(HcalOuter,   gDDD, geom);
   return geom ;
}

void HcalDDDGeometryLoader::fill(HcalSubdetector          subdet, 
				 HcalDDDGeometry*         geometryDDD,
				 CaloSubdetectorGeometry* geom           ) {

  // start by making the new HcalDetIds
  std::vector<HcalCellType::HcalCellType> hcalCells = numberingFromDDD->HcalCellTypes(subdet);
  geometryDDD->insertCell(hcalCells);
#ifdef DebugLog
  LogDebug("HCalGeom") << "HcalDDDGeometryLoader::fill gets " 
		       << hcalCells.size() << " cells for subdetector " 
		       << subdet;
#endif			 
  // Make the new HcalDetIds and the cells

  double deg = M_PI/180.;
  std::vector<HcalDetId> hcalIds;
  for (unsigned int i=0; i<hcalCells.size(); i++) {
    int etaRing  = hcalCells[i].etaBin();
    int depthBin = hcalCells[i].depthSegment();
    int phiInc   = 4/hcalCells[i].nPhiModule();
    unsigned int iphi = 1;
    if (hcalCells[i].unitPhi() == 4) iphi = 3;
    double  dphi = (hcalCells[i].phiBinWidth())*deg;
    double   phi =-(hcalCells[i].phiOffset())*deg + 0.5*dphi;
#ifdef DebugLog
    LogDebug("HCalGeom") << "HcalDDDGeometryLoader: Subdet " << subdet
			 << " eta " << etaRing << " depth " << depthBin
			 << " modules " << hcalCells[i].nPhiModule() << " "
			 << phiInc << " phi " << phi/deg << " " << dphi/deg;
#endif
    for (int k = 0; k < hcalCells[i].nPhiBins(); k++) {
#ifdef DebugLog
      LogDebug("HCalGeom") << "HcalDDDGeometryLoader::fill Cell " << i
			   << " eta " << etaRing << " phi " << iphi << "("
			   << phi/deg << ", " << dphi/deg << ") depth "
			   << depthBin;
#endif
      HcalDetId id(subdet, etaRing, iphi, depthBin);
      hcalIds.push_back(id);
      geom->addCell( id, makeCell(id,hcalCells[i],phi,dphi,geom) );
      if (hcalCells[i].nHalves() > 1) {
#ifdef DebugLog
	LogDebug("HCalGeom") << "HcalDDDGeometryLoader::fill Cell " << i
			     << " eta " << -etaRing << " phi " << iphi << " ("
			     << phi/deg << ", " << dphi/deg << ") depth "
			     << depthBin;
#endif
	HcalDetId id(subdet, -etaRing, iphi, depthBin);
	hcalIds.push_back(id);
	geom->addCell( id, makeCell(id,hcalCells[i],phi,dphi,geom) );
      }
      iphi += phiInc;
      phi  += dphi;
    }
  }
  
  edm::LogInfo("HCalGeom") << "Number of HCAL DetIds made for " << subdet
			   << " is " << hcalIds.size();
}

CaloCellGeometry* 
HcalDDDGeometryLoader::makeCell( const HcalDetId& detId,
				 HcalCellType::HcalCellType hcalCell,
				 double phi, 
				 double dphi,
				 CaloSubdetectorGeometry* geom) const {

  // the two eta boundaries of the cell
  double          eta1   = hcalCell.etaMin();
  double          eta2   = hcalCell.etaMax();
  HcalSubdetector subdet = detId.subdet();
  double          eta    = 0.5*(eta1+eta2) * detId.zside();
  double          deta   = (eta2-eta1);
  double          theta  = 2.0*atan(exp(-eta));

  // barrel vs forward
  bool rzType   = hcalCell.depthType();
  bool isBarrel = (subdet == HcalBarrel || subdet == HcalOuter);

  double          z, r, thickness;

  if (rzType) {
    r          = hcalCell.depthMin();
    if (isBarrel) {
      z         = r * sinh(eta); // sinh(eta) == 1/tan(theta)
      thickness = (hcalCell.depthMax() - r) * cosh(eta); // cosh(eta) == 1/sin(theta)
    } else {
      z         = r * sinh(eta2);
      thickness = 2. * hcalCell.halfSize();
    }
#ifdef DebugLog
    LogDebug("HCalGeom") << "HcalDDDGeometryLoader::makeCell SubDet " << subdet
			 << " eta = " << eta << " theta = " << theta
			 << " r = " << r << " thickness = " << thickness;
#endif
  } else {
    z          = hcalCell.depthMin();
    thickness  = hcalCell.depthMax() - z;
    z         *= detId.zside(); // get the sign right.
    r          = z * tan(theta);
    thickness /= std::abs(cos(theta));
#ifdef DebugLog
    LogDebug("HCalGeom") << "HcalDDDGeometryLoader::makeCell SubDet " << subdet
			 << " eta = " << eta << " theta = " << theta
			 << " z = " << z << " r = " << r << " thickness = "
			 << thickness;
#endif
  }

  double x = r * cos(phi);
  double y = r * sin(phi);
  GlobalPoint point(x,y,z);

#ifdef DebugLog
  LogDebug("HCalGeom") << "HcalDDDGeometryLoader::makeCell for " << detId
		       << " Point " << point << " deta = " << deta 
		       << " dphi = " << dphi << " thickness = " << thickness
		       << " isBarrel = " << isBarrel << " " << rzType;
#endif
  
  if (subdet==HcalForward) {
    std::vector<float> hf ;
    hf.reserve(3) ;
    hf.push_back(deta/2.) ;
    hf.push_back(dphi/2.) ;
    hf.push_back(thickness/2.) ;
    return new calogeom::IdealZPrism(point, 
				     geom->cornersMgr(),
				     CaloCellGeometry::getParmPtr(hf, 
								  geom->parMgr(), 
								  geom->parVecVec() ) );
  } else  { 
    const double sign ( isBarrel ? 1 : -1 ) ;
    std::vector<float> hh ;
    hh.reserve(3) ;
    hh.push_back(deta/2.) ;
    hh.push_back(dphi/2.) ;
    hh.push_back(sign*thickness/2.) ;
    return new calogeom::IdealObliquePrism(point,
					   geom->cornersMgr(),
					   CaloCellGeometry::getParmPtr(hh, 
									geom->parMgr(), 
									geom->parVecVec() ) );
  }
}
