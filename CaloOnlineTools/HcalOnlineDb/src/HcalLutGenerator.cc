#include "CaloOnlineTools/HcalOnlineDb/interface/HcalLutGenerator.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/CaloTopology/interface/HcalTopology.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "CalibFormats/HcalObjects/interface/HcalTPGRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalTPGCoder.h"
#include "CaloOnlineTools/HcalOnlineDb/interface/HcalLutManager.h"
#include "CalibFormats/CaloTPG/interface/CaloTPGRecord.h"
//#include "CalibCalorimetry/CaloTPG/src/CaloTPGTranscoderULUT.h"
 #include "CondFormats/HcalObjects/interface/HcalElectronicsMap.h"

#include "CaloOnlineTools/HcalOnlineDb/interface/LMap.h"


#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

HcalLutGenerator::HcalLutGenerator(const edm::ParameterSet& iConfig)
{
  cout << " --> HcalLutGenerator::HcalLutGenerator()" << endl;
  _tag = iConfig.getParameter<string>("tag");
  _lin_file = iConfig.getParameter<string>("HO_master_file");
}

HcalLutGenerator::~HcalLutGenerator()
{
}


void HcalLutGenerator::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  
  cout << " --> HcalLutGenerator::analyze()" << endl;
  
  //
  //_____ get the coders from Event Setup _______________________________
  //
  edm::ESHandle<HcalTPGCoder> inputCoder;
  iSetup.get<HcalTPGRecord>().get(inputCoder);
  HcalTopology theTopo;
  HcalDetId did;
  //
  edm::ESHandle<CaloTPGTranscoder> outTranscoder;
  iSetup.get<CaloTPGRecord>().get(outTranscoder);
  outTranscoder->setup(iSetup,CaloTPGTranscoder::HcalTPG);
  edm::ESHandle<CaloTPGTranscoderULUT> transcoder;
  transcoder.swap(outTranscoder);

  //
  //_____ get EMAP from Event Setup _____________________________________
  //
  edm::ESHandle<HcalElectronicsMap> hEmap;
  iSetup.get<HcalElectronicsMapRcd>().get(hEmap);
  std::vector<HcalGenericDetId> vEmap = hEmap->allPrecisionId();
  cout << "EMAP from Event Setup has " << vEmap.size() << " entries" << endl;

  //
  //_____ get Channel Quality conditions from Event Setup (example)______
  //
  edm::ESHandle<HcalChannelQuality> hCQ;
  iSetup.get<HcalChannelQualityRcd>().get(hCQ);
  const HcalChannelQuality * _cq = &(*hCQ);
  //
  // Here's how one gets channel status from the Channel Quality condition.
  // One can use their own loop over channels or get a vactor of all
  // channels from the conditions object
  //
  //_____ get list of all channels
  //
  std::vector<DetId> _channels = _cq->getAllChannels();
  cout << "Channel Quality available for " << _channels.size() << " channels" << endl;
  //
  //_____ loop over channels
  //
  for (std::vector<DetId>::const_iterator _ch = _channels.begin();
       _ch != _channels.end();
       _ch++){
    //
    //_____ select only HBEF logical channels
    //
    HcalGenericDetId _gid( *_ch );
    if ( !(_gid.null()) &&
	 (_gid.genericSubdet()==HcalGenericDetId::HcalGenBarrel ||
	  _gid.genericSubdet()==HcalGenericDetId::HcalGenEndcap ||
	  _gid.genericSubdet()==HcalGenericDetId::HcalGenForward ||
	  _gid.genericSubdet()==HcalGenericDetId::HcalGenOuter
	  )
	 ){
      const HcalChannelStatus * _cs = _cq->getValues( *_ch );

      // get the full 32-bit channel status word
      uint32_t status_word = _cs->getValue();
      
      // get the 15th bit (which is supposed to mean hot channel)
      bool is_hot = _cs->isBitSet(15);
      cout << "HCAL channel ID: " << _ch->rawId()
	   << ", status word: " << status_word
	   << ", hot flag: " << is_hot << endl;
    }
  }
  //_____end of Channel Quality example_____________________

  //
  //_____ generate LUTs _________________________________________________
  //
  //HcalLutManager * manager = new HcalLutManager(); // old ways
  HcalLutManager * manager = new HcalLutManager(&(*hEmap));
  bool split_by_crate = true;
  cout << " tag name: " << _tag << endl;
  cout << " HO master file: " << _lin_file << endl;

  // default
  manager -> createLutXmlFiles_HBEFFromCoder_HOFromAscii( _tag, *inputCoder, *transcoder, _lin_file, split_by_crate );

  // with ZDC (experimental)
  //manager -> createLutXmlFiles_HBEFFromCoder_HOFromAscii_ZDC( _tag, *inputCoder, *transcoder, _lin_file, split_by_crate );
  delete manager;

  transcoder->releaseSetup();
   
}


void HcalLutGenerator::endJob() {

}
