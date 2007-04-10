#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "L1TriggerConfig/DTTPGConfigProducers/src/DTConfigTrivialProducer.h"
#include "L1TriggerConfig/DTTPGConfigProducers/src/DTConfigTester.h"

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_EVENTSETUP_MODULE(DTConfigTrivialProducer);
DEFINE_ANOTHER_FWK_MODULE(DTConfigTester);
