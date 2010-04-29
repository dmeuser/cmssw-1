#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/GeomPropagators/interface/AnalyticalPropagator.h"
#include "DataFormats/GeometrySurface/interface/Plane.h"
#include "DataFormats/GeometrySurface/interface/Cylinder.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"

#include "Geometry/EcalAlgo/interface/EcalBarrelGeometry.h"
#include "Geometry/EcalAlgo/interface/EcalEndcapGeometry.h"

#include "Calibration/IsolatedParticles/interface/CaloPropagateTrack.h"

#include <iostream>

namespace spr{

  std::vector<spr::propagatedTrackID> propagateCALO(edm::Handle<reco::TrackCollection>& trkCollection, const CaloGeometry* geo, const MagneticField* bField, std::string & theTrackQuality, bool debug) {

    std::vector<spr::propagatedTrackID> vdets;
    const EcalBarrelGeometry *barrelGeom = (dynamic_cast< const EcalBarrelGeometry *> (geo->getSubdetectorGeometry(DetId::Ecal,EcalBarrel)));
    const EcalEndcapGeometry *endcapGeom = (dynamic_cast< const EcalEndcapGeometry *> (geo->getSubdetectorGeometry(DetId::Ecal,EcalEndcap)));
    const CaloSubdetectorGeometry* gHB = geo->getSubdetectorGeometry(DetId::Hcal,HcalBarrel);
    reco::TrackBase::TrackQuality trackQuality_=reco::TrackBase::qualityByName(theTrackQuality);

    unsigned indx;
    reco::TrackCollection::const_iterator trkItr;
    for (trkItr = trkCollection->begin(),indx=0; trkItr != trkCollection->end(); ++trkItr,indx++) {
      const reco::Track* pTrack = &(*trkItr);
      propagatedTrackID vdet;
      vdet.trkItr = trkItr;
      vdet.ok     = (pTrack->quality(trackQuality_));
      vdet.detIdECAL = DetId(0);
      vdet.detIdHCAL = DetId(0);
      if (debug) std::cout << "Propagate track " << indx << " p " << trkItr->p() << " eta " << trkItr->eta() << " phi " << trkItr->phi() << " Flag " << vdet.ok << std::endl;

      std::pair<math::XYZPoint,bool> info = spr::propagateECAL (pTrack, bField, debug);
      vdet.okECAL = info.second;
      if (vdet.okECAL) {
	const GlobalPoint point(info.first.x(),info.first.y(),info.first.z());
	if (std::abs(point.eta())<1.479) {
	  vdet.detIdECAL = barrelGeom->getClosestCell(point);
	} else {
	  vdet.detIdECAL = endcapGeom->getClosestCell(point);
	}
      }
      info = spr::propagateHCAL (pTrack, bField, debug);
      vdet.okHCAL = info.second;
      if (vdet.okHCAL) {
	const GlobalPoint point(info.first.x(),info.first.y(),info.first.z());
	vdet.detIdHCAL = gHB->getClosestCell(point);
      }

      vdets.push_back(vdet);
    }
    
    if (debug) {
      std::cout << "propagateCALO:: for " << vdets.size() << " tracks" << std::endl;
      for (unsigned int i=0; i<vdets.size(); ++i) {
	std::cout << "Track [" << i << "] Flag: " << vdets[i].ok << " ECAL (" << vdets[i].okECAL << ") ";
	if (vdets[i].detIdECAL.subdetId() == EcalBarrel) {
	  std::cout << (EBDetId)(vdets[i].detIdECAL);
	} else {
	  std::cout << (EEDetId)(vdets[i].detIdHCAL); 
	}
	std::cout << " HCAL (" << vdets[i].okHCAL << ") " << (HcalDetId)(vdets[i].detIdHCAL) << std::endl;
      }
    }
    return vdets;
  }

  propagatedTrack propagateTrackToECAL(const reco::Track *track, const MagneticField* bfield, bool debug) {
    GlobalPoint  vertex (track->vx(), track->vy(), track->vz());
    GlobalVector momentum (track->px(), track->py(), track->pz());
    int charge (track->charge());
    return spr::propagateCalo (vertex, momentum, charge, bfield, 319.2, 129.4, 1.479, debug);
  }

  std::pair<math::XYZPoint,bool> propagateECAL(const reco::Track *track, const MagneticField* bfield, bool debug) {    
    GlobalPoint  vertex (track->vx(), track->vy(), track->vz());
    GlobalVector momentum (track->px(), track->py(), track->pz());
    int charge (track->charge());
    return spr::propagateECAL (vertex, momentum, charge, bfield, debug);
  }

  std::pair<math::XYZPoint,bool> propagateECAL(const GlobalPoint& vertex, const GlobalVector& momentum, int charge, const MagneticField* bfield, bool debug) {
    spr::propagatedTrack track = spr::propagateCalo (vertex, momentum, charge, bfield, 319.2, 129.4, 1.479, debug);
    return std::pair<math::XYZPoint,bool>(track.point,track.ok);
  }

  propagatedTrack propagateTrackToHCAL(const reco::Track *track, const MagneticField* bfield, bool debug) {
    GlobalPoint  vertex (track->vx(), track->vy(), track->vz());
    GlobalVector momentum (track->px(), track->py(), track->pz());
    int charge (track->charge());
    return spr::propagateCalo (vertex, momentum, charge, bfield, 402.7, 180.7, 1.392, debug);
  }

  std::pair<math::XYZPoint,bool> propagateHCAL(const reco::Track *track, const MagneticField* bfield, bool debug) {
    GlobalPoint  vertex (track->vx(), track->vy(), track->vz());
    GlobalVector momentum (track->px(), track->py(), track->pz());
    int charge (track->charge());
    return spr::propagateHCAL (vertex, momentum, charge, bfield, debug);
  }

  std::pair<math::XYZPoint,bool> propagateHCAL(const GlobalPoint& vertex, const GlobalVector& momentum, int charge, const MagneticField* bfield, bool debug) {
    spr::propagatedTrack track = spr::propagateCalo (vertex, momentum, charge, bfield, 402.7, 180.7, 1.392, debug);
    return std::pair<math::XYZPoint,bool>(track.point,track.ok);
  }

  std::pair<math::XYZPoint,bool> propagateTracker(const reco::Track *track, const MagneticField* bfield, bool debug) {
    GlobalPoint  vertex (track->vx(), track->vy(), track->vz());
    GlobalVector momentum (track->px(), track->py(), track->pz());
    int charge (track->charge());
    spr::propagatedTrack track1 = spr::propagateCalo (vertex, momentum, charge, bfield, 290.0, 109.0, 1.705, debug);
    return std::pair<math::XYZPoint,bool>(track1.point,track1.ok);
  }

  std::pair<math::XYZPoint,double> propagateTrackerEnd(const reco::Track *track, const MagneticField* bField, bool debug) {

    GlobalPoint  vertex (track->vx(), track->vy(), track->vz());
    GlobalVector momentum (track->px(), track->py(), track->pz());
    int charge (track->charge());
    float radius = track->outerPosition().Rho();
    float zdist  = track->outerPosition().Z();
    if (debug) std::cout << "propagateTrackerEnd:: Vertex " << vertex << " Momentum " << momentum << " Charge " << charge << " Radius " << radius << " Z " << zdist << std::endl;

    FreeTrajectoryState fts (vertex, momentum, charge, bField);
    Plane::PlanePointer endcap = Plane::build(Plane::PositionType (0, 0, zdist), Plane::RotationType());
    Cylinder::CylinderPointer barrel = Cylinder::build(Cylinder::PositionType (0, 0, 0), Cylinder::RotationType (), radius);

    AnalyticalPropagator myAP (bField, alongMomentum, 2*M_PI);

    TrajectoryStateOnSurface tsose = myAP.propagate(fts, *endcap);
    TrajectoryStateOnSurface tsosb = myAP.propagate(fts, *barrel);

    math::XYZPoint point(-999.,-999.,-999.);
    bool ok=false;
    GlobalVector direction(0,0,1);
    if (tsosb.isValid() && std::abs(zdist) < 110) {
      point.SetXYZ(tsosb.globalPosition().x(), tsosb.globalPosition().y(), tsosb.globalPosition().z());
      direction = tsosb.globalDirection();
      ok = true;
    } else if (tsose.isValid()) {
      point.SetXYZ(tsose.globalPosition().x(), tsose.globalPosition().y(), tsose.globalPosition().z());
      direction = tsose.globalDirection();
      ok = true;
    }

    double length = -1;
    if (ok) {
      math::XYZPoint vDiff(point.x()-vertex.x(), point.y()-vertex.y(), point.z()-vertex.z());
      double dphi  = direction.phi()-momentum.phi();
      double rdist = std::sqrt(vDiff.x()*vDiff.x()+vDiff.y()*vDiff.y());
      double rat   = 0.5*dphi/std::sin(0.5*dphi);
      double dZ    = vDiff.z();
      double dS    = rdist*rat; //dZ*momentum.z()/momentum.perp();
      length       = std::sqrt(dS*dS+dZ*dZ);
      if (debug) 
	std::cout << "propagateTracker:: Barrel " << tsosb.isValid() << " Endcap " << tsose.isValid() << " OverAll " << ok << " Point " << point << " RDist " << rdist << " dS " << dS << " dS/pt " << rdist*rat/momentum.perp() << " zdist " << dZ << " dz/pz " << dZ/momentum.z() << " Length " << length << std::endl;
    }

    return std::pair<math::XYZPoint,double>(point,length);
  }

  spr::propagatedTrack propagateCalo(const GlobalPoint& tpVertex, const GlobalVector& tpMomentum, int tpCharge, const MagneticField* bField, float zdist, float radius, float corner, bool debug) {
    
    spr::propagatedTrack track;
    if (debug) std::cout << "propagateCalo:: Vertex " << tpVertex << " Momentum " << tpMomentum << " Charge " << tpCharge << " Radius " << radius << " Z " << zdist << " Corner " << corner << std::endl;
    FreeTrajectoryState fts (tpVertex, tpMomentum, tpCharge, bField);
    
    Plane::PlanePointer lendcap = Plane::build(Plane::PositionType (0, 0, -zdist), Plane::RotationType());
    Plane::PlanePointer rendcap = Plane::build(Plane::PositionType (0, 0,  zdist), Plane::RotationType());
    
    Cylinder::CylinderPointer barrel = Cylinder::build(Cylinder::PositionType (0, 0, 0), Cylinder::RotationType (), radius);
  
    AnalyticalPropagator myAP (bField, alongMomentum, 2*M_PI);

    TrajectoryStateOnSurface tsose;
    if (tpMomentum.eta() < 0) {
      tsose = myAP.propagate(fts, *lendcap);
    } else {
      tsose = myAP.propagate(fts, *rendcap);
    }

    TrajectoryStateOnSurface tsosb = myAP.propagate(fts, *barrel);

    track.ok=true;
    if (tsose.isValid() && tsosb.isValid()) {
      float absEta = std::abs(tsosb.globalPosition().eta());
      if (absEta < corner) {
	track.point.SetXYZ(tsosb.globalPosition().x(), tsosb.globalPosition().y(), tsosb.globalPosition().z());
	track.direction = tsosb.globalDirection();
      } else {
	track.point.SetXYZ(tsose.globalPosition().x(), tsose.globalPosition().y(), tsose.globalPosition().z());
	track.direction = tsose.globalDirection();
      }
    } else if (tsose.isValid()) {
      track.point.SetXYZ(tsose.globalPosition().x(), tsose.globalPosition().y(), tsose.globalPosition().z());
      track.direction = tsose.globalDirection();
    } else if (tsosb.isValid()) {
      track.point.SetXYZ(tsosb.globalPosition().x(), tsosb.globalPosition().y(), tsosb.globalPosition().z());
      track.direction = tsosb.globalDirection();
    } else {
      track.point.SetXYZ(-999., -999., -999.);
      track.direction = GlobalVector(0,0,1);
      track.ok = false;
    }
    if (debug) {
      std::cout << "propagateCalo:: Barrel " << tsosb.isValid() << " Endcap " << tsose.isValid() << " OverAll " << track.ok << " Point " << track.point << " Direction " << track.direction << std::endl;
      if (track.ok) {
	math::XYZPoint vDiff(track.point.x()-tpVertex.x(), track.point.y()-tpVertex.y(), track.point.z()-tpVertex.z());
	double dphi = track.direction.phi()-tpMomentum.phi();
	double rdist = std::sqrt(vDiff.x()*vDiff.x()+vDiff.y()*vDiff.y());
	double pt    = tpMomentum.perp();
	double rat   = 0.5*dphi/std::sin(0.5*dphi);
	std::cout << "RDist " << rdist << " pt " << pt << " r/pt " << rdist*rat/pt << " zdist " << vDiff.z() << " pz " << tpMomentum.z() << " z/pz " << vDiff.z()/tpMomentum.z() << std::endl;
      }
    }
    return track;
  }

}
