
//
// F.Ratnikov (UMd), Oct 28, 2005
// $Id: HcalDbXml.cc,v 1.10 2006/07/26 01:09:26 fedor Exp $
//
#include <vector>
#include <string>

#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDetId/interface/HcalTrigTowerDetId.h"
#include "DataFormats/HcalDetId/interface/HcalElectronicsId.h"

#include "CondFormats/HcalObjects/interface/AllObjects.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalDbXml.h"

namespace {
  void dumpProlog (std::ostream& fOutput) {
    fOutput << "<?xml version='1.0' encoding='UTF-8'?>" << std::endl;
    fOutput << "<!DOCTYPE root []>" << std::endl;
    fOutput << "<ROOT>" << std::endl;
  }

  void dumpRun (std::ostream& fOutput, unsigned fRun) {
    fOutput << "<RUN>";
    fOutput << "<RUN_TYPE>" << "HcalDbXml" << "</RUN_TYPE>";
    fOutput << "<RUN_NUMBER>" << fRun << "</RUN_NUMBER>";
    fOutput << "</RUN>" << std::endl;
  }

  void dumpHeader (std::ostream& fOutput, unsigned fRun, const std::string& fTableName, const std::string& fTypeName) {
    fOutput << "  <HEADER>" << std::endl;
    fOutput << "    <TYPE>" << std::endl;
    fOutput << "      <EXTENSION_TABLE_NAME>" << fTableName << "</EXTENSION_TABLE_NAME>" << std::endl;
    fOutput << "      <NAME>" << fTypeName << "</NAME>" << std::endl;
    fOutput << "    </TYPE>" << std::endl;
    dumpRun (fOutput, fRun);
    fOutput << "  </HEADER>" << std::endl;
  }

  void dumpFooter (std::ostream& fOutput) {
    fOutput << "</ROOT>" << std::endl;
  }

  void dumpChannelId (std::ostream& fOutput, DetId fChannel) {
    HcalText2DetIdConverter converter (fChannel);
    fOutput << "      ";
    fOutput << "<CHANNEL> ";
    fOutput << "<EXTENSION_TABLE_NAME>HCAL_CHANNELS</EXTENSION_TABLE_NAME> ";
    fOutput << "<ETA>" << abs (converter.getField (1)) << "</ETA>";
    fOutput << "<PHI>" << converter.getField (2) << "</PHI> ";
    fOutput << "<DEPTH>" << converter.getField (3) << "</DEPTH> ";
    fOutput << "<Z>" << (converter.getField (1) > 0 > 0 ? "1" : "-1") << "</Z> ";
    fOutput << "<DETECTOR_NAME>" << converter.getFlavor () << "</DETECTOR_NAME> ";
    fOutput << "<HCAL_CHANNEL_ID>" << converter.getId().rawId () << "</HCAL_CHANNEL_ID> ";
    fOutput << "</CHANNEL>";
    fOutput << std::endl;
  }

  void dumpData (std::ostream& fOutput, const float* fValues, const HcalPedestalWidth& fErrors) {
    fOutput << "      ";
    fOutput << "<DATA> ";
    fOutput << "<CAPACITOR_0_VALUE>" << fValues [0] << "</CAPACITOR_0_VALUE> ";
    fOutput << "<CAPACITOR_1_VALUE>" << fValues [1] << "</CAPACITOR_1_VALUE> ";
    fOutput << "<CAPACITOR_2_VALUE>" << fValues [2] << "</CAPACITOR_2_VALUE> ";	
    fOutput << "<CAPACITOR_3_VALUE>" << fValues [3] << "</CAPACITOR_3_VALUE> ";
    fOutput << "<SIGMA_0_0>" << fErrors.getSigma (0,0) << "</SIGMA_0_0> ";	
    fOutput << "<SIGMA_1_1>" << fErrors.getSigma (1,1) << "</SIGMA_1_1> ";	
    fOutput << "<SIGMA_2_2>" << fErrors.getSigma (2,2) << "</SIGMA_2_2> ";	
    fOutput << "<SIGMA_3_3>" << fErrors.getSigma (3,3) << "</SIGMA_3_3> ";	
    fOutput << "<SIGMA_0_1>" << fErrors.getSigma (1,0) << "</SIGMA_0_1> ";	
    fOutput << "<SIGMA_0_2>" << fErrors.getSigma (2,0) << "</SIGMA_0_2> ";	
    fOutput << "<SIGMA_0_3>" << fErrors.getSigma (3,0) << "</SIGMA_0_3> ";	
    fOutput << "<SIGMA_1_2>" << fErrors.getSigma (2,1) << "</SIGMA_1_2> ";	
    fOutput << "<SIGMA_1_3>" << fErrors.getSigma (3,1) << "</SIGMA_1_3> ";	
    fOutput << "<SIGMA_2_3>" << fErrors.getSigma (3,2) << "</SIGMA_2_3> ";	
    fOutput << "</DATA> " << std::endl;
  }

  void dumpData (std::ostream& fOutput, const float* fValues, const float* fErrors) {
    fOutput << "      ";
    fOutput << "<DATA> ";
    fOutput << "<CAPACITOR_0_VALUE>" << fValues [0] << "</CAPACITOR_0_VALUE> ";
    fOutput << "<CAPACITOR_1_VALUE>" << fValues [1] << "</CAPACITOR_1_VALUE> ";
    fOutput << "<CAPACITOR_2_VALUE>" << fValues [2] << "</CAPACITOR_2_VALUE> ";	
    fOutput << "<CAPACITOR_3_VALUE>" << fValues [3] << "</CAPACITOR_3_VALUE> ";
    fOutput << "<CAPACITOR_0_ERROR>" << fErrors [0] << "</CAPACITOR_0_ERROR> ";
    fOutput << "<CAPACITOR_1_ERROR>" << fErrors [1] << "</CAPACITOR_1_ERROR> ";
    fOutput << "<CAPACITOR_2_ERROR>" << fErrors [2] << "</CAPACITOR_2_ERROR> ";	
    fOutput << "<CAPACITOR_3_ERROR>" << fErrors [3] << "</CAPACITOR_3_ERROR> ";
    fOutput << "</DATA> " << std::endl;
  }

  void dumpDataset (std::ostream& fOutput, unsigned fVersion = 0, const std::string& fFileName = "", const std::string& fDescription = "") {
    fOutput << "    <DATA_SET>" << std::endl;
    fOutput << "      <VERSION>" << fVersion << "</VERSION>" << std::endl;
    if (!fFileName.empty ()) 
      fOutput << "      <DATA_FILE_NAME>" << fFileName << "</DATA_FILE_NAME>" << std::endl;
    if (!fDescription.empty ())
      fOutput << "      <COMMENT_DESCRIPTION>" << fDescription << "</COMMENT_DESCRIPTION>" << std::endl;
  }

  void endDataset (std::ostream& fOutput) {
    fOutput << "    </DATA_SET>" << std::endl;
  }

  void dumpMapping (std::ostream& fOutput, unsigned fRun, const std::string& fKind, 
		    unsigned long fGMTIOVBegin, unsigned long fGMTIOVEnd, 
		    const std::string& fTag, unsigned fVersion, const std::vector<DetId>& fChannels) {
    const std::string IOV_ID = "IOV_ID";
    const std::string TAG_ID = "TAG_ID";
    fOutput << "<ELEMENTS>" << std::endl;
    // set channels affected
    int i = fChannels.size ();
    while (--i >= 0) {
      fOutput << "<DATA_SET id=\"" << i << "\">" << std::endl;
      dumpRun (fOutput, fRun);
      fOutput << "<KIND_OF_CONDITION><NAME>" << fKind << "</NAME></KIND_OF_CONDITION>" << std::endl;
      dumpChannelId (fOutput, fChannels[i]);
      fOutput << "<VERSION>" << fVersion << "</VERSION>" << std::endl;
      fOutput << "</DATA_SET>" << std::endl;
    }
    // set IOV
    fOutput << "<IOV id=\"" << IOV_ID << "\">";
    fOutput << "<INTERVAL_OF_VALIDITY_BEGIN>" << fGMTIOVBegin << "</INTERVAL_OF_VALIDITY_BEGIN>";
    fOutput << "<INTERVAL_OF_VALIDITY_END>" << fGMTIOVEnd << "</INTERVAL_OF_VALIDITY_END>";
    fOutput << "</IOV>" << std::endl;
    // set TAG
    fOutput << "<TAG id=\"" << TAG_ID << "\" mode=\"create\">";
    fOutput << "<TAG_NAME>" << fTag << "</TAG_NAME>";
    fOutput << "<DETECTOR_NAME>HCAL</DETECTOR_NAME>";
    fOutput << "<COMMENT_DESCRIPTION>Automatically created by HcalDbXml</COMMENT_DESCRIPTION>" << std::endl;
    fOutput << "</TAG>" << std::endl;

    fOutput << "</ELEMENTS>" << std::endl;

    // mapping itself
    fOutput << "<MAPS>" << std::endl;
    fOutput << "<TAG idref=\"" << TAG_ID << "\">" << std::endl;
    fOutput << "<IOV idref=\"" << IOV_ID << "\">" << std::endl;
    i = fChannels.size ();
    while (--i >= 0) {
      fOutput << "<DATA_SET idref=\"" << i << "\"/>" << std::endl;
    }
    fOutput << "</IOV>" << std::endl;
    fOutput << "</TAG>" << std::endl;
    fOutput << "</MAPS>" << std::endl;
  }
}



bool HcalDbXml::dumpObject (std::ostream& fOutput, 
			    unsigned fRun, unsigned long fGMTIOVBegin, unsigned long fGMTIOVEnd, const std::string& fTag, unsigned fVersion, 
			    const HcalPedestals& fObject) {
  float dummyError = 0.0001;
  std::cout << "HcalDbXml::dumpObject-> set default errors: 0.0001, 0.0001, 0.0001, 0.0001" << std::endl;
  HcalPedestalWidths widths;
  std::vector<DetId> channels = fObject.getAllChannels ();
  for (std::vector<DetId>::iterator channel = channels.begin ();
       channel !=  channels.end ();
       channel++) {
    HcalPedestalWidth* item = widths.setWidth (*channel);
    for (int iCapId = 1; iCapId <= 4; iCapId++) {
      item->setSigma (iCapId, iCapId, dummyError*dummyError);
    }
  }
  widths.sort ();
  return dumpObject (fOutput, fRun, fGMTIOVBegin, fGMTIOVEnd, fTag, fVersion, fObject, widths);
}

bool HcalDbXml::dumpObject (std::ostream& fOutput, 
			    unsigned fRun, unsigned long fGMTIOVBegin, unsigned long fGMTIOVEnd, const std::string& fTag, unsigned fVersion, 
			    const HcalPedestals& fObject, const HcalPedestalWidths& fError) {
  const std::string KIND = "HCAL_PEDESTALS_V2";

  dumpProlog (fOutput);
  dumpHeader (fOutput, fRun, KIND, KIND);

  std::vector<DetId> channels = fObject.getAllChannels ();
  for (std::vector<DetId>::iterator channel = channels.begin ();
       channel !=  channels.end ();
       channel++) {
    DetId chId = *channel;
    const float* values = fObject.getValues (chId)->getValues ();
    const HcalPedestalWidth* errors = fError.getValues (chId);
    if (!values) {
      std::cerr << "HcalDbXml::dumpObject-> Can not get data for channel " << HcalText2DetIdConverter(chId).toString () << std::endl;
      continue;
    }
    if (!errors) {
      std::cerr << "HcalDbXml::dumpObject-> Can not get errors for channel " << HcalText2DetIdConverter(chId).toString () <<  ". Use defaults" << std::endl;
      continue;
    }
    dumpDataset (fOutput, fVersion, "", "");
    dumpChannelId (fOutput,chId); 
    dumpData (fOutput, values, *errors);
    endDataset (fOutput);
  }
  dumpMapping (fOutput, fRun, KIND, fGMTIOVBegin, fGMTIOVEnd, fTag, fVersion, channels);

  dumpFooter (fOutput);
  return true;
}

bool HcalDbXml::dumpObject (std::ostream& fOutput, 
			    unsigned fRun, unsigned long fGMTIOVBegin, unsigned long fGMTIOVEnd, const std::string& fTag, unsigned fVersion,
			    const HcalGains& fObject) {
  float dummyErrors [4] = {0., 0., 0., 0.};
  std::cout << "HcalDbXml::dumpObject-> set default errors: 4 x 0.0" << std::endl;
  HcalGainWidths widths;
  std::vector<DetId> channels = fObject.getAllChannels ();
  for (std::vector<DetId>::iterator channel = channels.begin ();
       channel !=  channels.end ();
       channel++) {
    widths.addValue (*channel, dummyErrors);
  }
  widths.sort ();
  return dumpObject (fOutput, fRun, fGMTIOVBegin, fGMTIOVEnd, fTag, fVersion, fObject, widths);
}

bool HcalDbXml::dumpObject (std::ostream& fOutput, 
			    unsigned fRun, unsigned long fGMTIOVBegin, unsigned long fGMTIOVEnd, const std::string& fTag, unsigned fVersion,
			    const HcalGains& fObject, const HcalGainWidths& fError) {
  const std::string KIND = "HCAL Gains";
  const std::string TABLE = "HCAL_GAIN_PEDSTL_CALIBRATIONS";

  dumpProlog (fOutput);
  dumpHeader (fOutput, fRun, TABLE, KIND);

  std::vector<DetId> channels = fObject.getAllChannels ();
  for (std::vector<DetId>::iterator channel = channels.begin ();
       channel !=  channels.end ();
       channel++) {
    DetId chId = *channel;
    const float* values = fObject.getValues (chId)->getValues ();
    const float* errors = fError.getValues (chId)->getValues ();
    if (!values) {
      std::cerr << "HcalDbXml::dumpObject-> Can not get data for channel " << HcalText2DetIdConverter(chId).toString () << std::endl;
      continue;
    }
    if (!errors) {
      std::cerr << "HcalDbXml::dumpObject-> Can not get errors for channel " << HcalText2DetIdConverter(chId).toString () <<  ". Use defaults" << std::endl;
      continue;
    }
    dumpDataset (fOutput, fVersion, "", "");
    dumpChannelId (fOutput,chId); 
    dumpData (fOutput, values, errors);
    endDataset (fOutput);
  }
  dumpMapping (fOutput, fRun, KIND, fGMTIOVBegin, fGMTIOVEnd, fTag, fVersion, channels);

  dumpFooter (fOutput);
  return true;
}
