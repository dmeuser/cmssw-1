/// ////////////////////////////////////////
/// Stacked Tracker Simulations          ///
/// Written by:                          ///
/// Kristofer Henriksson                 ///
/// 2009                                 ///
///                                      ///
/// Changed by:                          ///
/// Nicola Pozzobon                      ///
/// UNIPD                                ///
/// 2010, May                            ///
///                                      ///
/// Added features:                      ///
/// Possibility to have a flag telling   ///
/// if the Stub is compatible with a     ///
/// higher Pt threshold than the default ///
/// one. Just performs the cross check   ///
/// twice.                               ///
/// ////////////////////////////////////////

#ifndef HIT_MATCHING_ALGORITHM_pixelray_H
#define HIT_MATCHING_ALGORITHM_pixelray_H

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/ESProducer.h"

#include "SLHCUpgradeSimulations/L1Trigger/interface/HitMatchingAlgorithm.h"
#include "SLHCUpgradeSimulations/L1Trigger/interface/HitMatchingAlgorithmRecord.h"

#include "SLHCUpgradeSimulations/L1Trigger/interface/HitMatchingAlgorithm_pixelray_helper.h"

#include "SLHCUpgradeSimulations/Utilities/interface/constants.h"
#include "SLHCUpgradeSimulations/Utilities/interface/classInfo.h"

#include <boost/shared_ptr.hpp>

#include <memory>
#include <string>

#include <map>

namespace cmsUpgrades{

// Define the algorithm

template <typename T>
class HitMatchingAlgorithm_pixelray : public HitMatchingAlgorithm<T>
{
public:
    HitMatchingAlgorithm_pixelray(const cmsUpgrades::StackedTrackerGeometry *i,
                                  double aCompatibilityScalingFactor, double aCompatibilityScalingFactorTight, double aIPWidth) :
        cmsUpgrades::HitMatchingAlgorithm<T>(i),
        mClassInfo( new cmsUpgrades::classInfo(__PRETTY_FUNCTION__) ),
        mCompatibilityScalingFactor(aCompatibilityScalingFactor),
        mCompatibilityScalingFactorTight(aCompatibilityScalingFactorTight),
        mIPWidth(aIPWidth)
    {
    }
        
    ~HitMatchingAlgorithm_pixelray()
    {
    }

    std::pair<bool,bool> CheckTwoMemberHitsForCompatibility(const cmsUpgrades::LocalStub<T> & aLocalStub) const
    {
      std::pair<bool,bool> a(false,false);

        std::pair< std::pair<double,double> , bool> * rayEndpoints;
        
        // Just call the helper function to do all the work
        rayEndpoints = getPixelRayEndpoints(aLocalStub,
            HitMatchingAlgorithm<T>::theStackedTracker, mCompatibilityScalingFactor, mCompatibilityScalingFactorTight);

            
        if (rayEndpoints) {
            // Establish the valid window
            double positiveZBoundary = mIPWidth/2;
            double negativeZBoundary = -mIPWidth/2;
            
            if (( (rayEndpoints->first).second > negativeZBoundary) &&
                ( (rayEndpoints->first).first < positiveZBoundary))
            {
                a.first = true;
                a.second = rayEndpoints->second;

                delete rayEndpoints;
                //return true;
            }
        }
        
        else delete rayEndpoints;
        
        //return false;
        return a;
    }
    
    std::string AlgorithmName() const
    { 
        return mClassInfo->FunctionName() + "<" +
               mClassInfo->TemplateTypes().begin()->second + ">";
    }
    
private:
    const cmsUpgrades::classInfo *mClassInfo;
    double mCompatibilityScalingFactor;  
    double mCompatibilityScalingFactorTight;
    double mIPWidth;

};

}

// Declare the algorithm to the framework

template <typename T>
class  ES_HitMatchingAlgorithm_pixelray : public edm::ESProducer
{
public:
    ES_HitMatchingAlgorithm_pixelray(const edm::ParameterSet & p) :
        mPtThreshold(p.getParameter<double>("minPtThreshold")),
        mPtThresholdTight(p.getParameter<double>("minPtThresholdTight")),
        mIPWidth(p.getParameter<double>("ipWidth"))
    {
        setWhatProduced(this);
    }
    
    virtual ~ES_HitMatchingAlgorithm_pixelray()
    {
    }
    
    boost::shared_ptr< cmsUpgrades::HitMatchingAlgorithm<T> >
    produce(const cmsUpgrades::HitMatchingAlgorithmRecord & record)
    { 
        edm::ESHandle<MagneticField> magnet;
        record.getRecord<IdealMagneticFieldRecord>().get(magnet);
        double mMagneticFieldStrength = magnet->inTesla(GlobalPoint(0,0,0)).z();
        double mCompatibilityScalingFactor = (100.0 * 2.0e+9 * mPtThreshold) /
            (cmsUpgrades::KGMS_C * mMagneticFieldStrength);
        // Invert so we use multiplication instead of division in the comparison
        mCompatibilityScalingFactor = 1.0 / mCompatibilityScalingFactor;

        double mCompatibilityScalingFactorTight = (100.0 * 2.0e+9 * mPtThresholdTight) /
            (cmsUpgrades::KGMS_C * mMagneticFieldStrength);
        // Invert so we use multiplication instead of division in the comparison
        mCompatibilityScalingFactorTight = 1.0 / mCompatibilityScalingFactorTight;
        
        edm::ESHandle<cmsUpgrades::StackedTrackerGeometry> StackedTrackerGeomHandle;
        record.getRecord<cmsUpgrades::StackedTrackerGeometryRecord>().get(StackedTrackerGeomHandle);
  
        cmsUpgrades::HitMatchingAlgorithm<T>* HitMatchingAlgo =
            new cmsUpgrades::HitMatchingAlgorithm_pixelray<T>(&(*StackedTrackerGeomHandle),
                                                              mCompatibilityScalingFactor,mCompatibilityScalingFactorTight,
                                                              mIPWidth);

        _theAlgo = boost::shared_ptr< cmsUpgrades::HitMatchingAlgorithm<T> >(HitMatchingAlgo);

        return _theAlgo;
    } 
    
private:
    boost::shared_ptr< cmsUpgrades::HitMatchingAlgorithm<T> > _theAlgo;
    double mPtThreshold;
    double mPtThresholdTight;
    double mIPWidth;
};

#endif


