#include "SimG4CMS/Calo81/interface/ECalSD.h"
#include "SimG4CMS/Calo81/interface/HCalSD.h"
#include "SimG4CMS/Calo81/interface/CaloTrkProcessing.h"
#include "SimG4CMS/Calo81/interface/HcalTestAnalysis.h"
#include "SimG4Core/SensitiveDetector/interface/SensitiveDetectorPluginFactory.h"
#include "SimG4Core/Watcher/interface/SimWatcherFactory.h"
#include "PluginManager/ModuleDef.h"
  
DEFINE_SEAL_MODULE ();
typedef ECalSD EcalSensitiveDetector;
DEFINE_SENSITIVEDETECTOR(EcalSensitiveDetector);
typedef HCalSD HcalSensitiveDetector;
DEFINE_SENSITIVEDETECTOR(HcalSensitiveDetector);
DEFINE_SENSITIVEDETECTOR(CaloTrkProcessing);

DEFINE_SIMWATCHER (HcalTestAnalysis);
