#include "FWCore/PluginManager/interface/ModuleDef.h"
#include <IORawData/CSCTFCommissioning/src/CSCTFFileReader.h>
#include <IORawData/DaqSource/interface/DaqReaderPluginFactory.h>
DEFINE_SEAL_MODULE();
DEFINE_EDM_PLUGIN (DaqReaderPluginFactory, CSCTFFileReader, "CSCTFFileReader");
