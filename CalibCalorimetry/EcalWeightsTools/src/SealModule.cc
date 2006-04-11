#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "TestEcal/Analyzers/src/EcalPedestalAnalyzer.h"
#include "CalibCalorimetry/EcalWeightsTools/interface/EcalWeightsBuilder.h"

DEFINE_SEAL_MODULE();

DEFINE_ANOTHER_FWK_MODULE(EcalWeightsBuilder);
