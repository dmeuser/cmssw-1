/************************************************************************
 * XDAQ layer for HCAL HLX card                                          *
 * Copyright (C) 2007, Princeton University                              *
 * All rights reserved.                                                  *
 * Author(s): J. Jones                 			                 *
 ************************************************************************/

#include "HLXReadoutSupervisor.hh"
#include <log4cplus/consoleappender.h>
#include <iomanip>

// Use the HCAL cfgDocument framework so we can parse the HLX setup
#include "hcal/cfg/CfgDocument.hh"

using namespace log4cplus;
using namespace log4cplus::spi;
using namespace log4cplus::helpers;
using namespace std;
using namespace HCAL_HLX;

#define XDAQ_PAGE_HEADER "HF HLX DAQ Supervisor"

// Standard instansiator for lumiHCALLUMIDAQ
XDAQ_INSTANTIATOR_IMPL(lumiHCALLUMIDAQ)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

lumiHCALLUMIDAQ::lumiHCALLUMIDAQ(xdaq::ApplicationStub * s) throw (xdaq::exception::Exception):
  hcal::Application(s),
  m_interfacePort(0),
  m_interfaceAddress(""),
  m_numOrbits(0),
  m_numBunches(0),
  m_shortSectionLength(0),
  m_longSectionLength(0),
  m_numHLXs(0),
  m_tcpShortPort(0),
  m_tcpLongPort(0),
  m_oracleDBUserName(""),
  m_oracleDBPassword(""),
  m_oracleDBLocation(""),
  m_oracleDBSchema(""),
  m_debugData(0),
  m_fakeHLXMode(false),
  m_useShortTCP(false),
  m_useLongTCP(false),
  m_useOracle(false),
  m_useGIF(false),
  m_useDIP(false),
  m_useSectionList(false) {
  
  try {

    // HCAL base class setup
    setConfigurationStyle(cfg_DOC);

    // Just initialise configFile_ to a blank string
    mNibbleCollector=0;
    mSectionCollectorShort=0;
    mSectionCollectorLong=0;
    mOracleDistributor=0;
    mGIFDistributor=0;
    mDIPDistributor=0;
    mShortSectionListDistributor=0;
    mLongSectionListDistributor=0;
    mShortSectionTCPDistributor=0;
    mLongSectionTCPDistributor=0;
    mFakeHLX = 0;
    mShortTestDistributor=0;
    mLongTestDistributor=0;
    mTestSectionCollector=0;

    // Cache the host name
    char hostnamea[HOST_NAME_MAX];
    gethostname(hostnamea,HOST_NAME_MAX);
    m_hostName = hostnamea;

    // Web configuration page
    xgi::bind(this,&lumiHCALLUMIDAQ::WebConfig,"WebConfig");

  } catch(ICException &aExc) {
    XCEPT_RAISE(xdaq::exception::Exception,aExc.what());
  } catch(xgi::exception::Exception &e) {
    string errMsg = "Exception caught in ";
    errMsg += __PRETTY_FUNCTION__;
    XCEPT_RETHROW(xdaq::exception::Exception, errMsg, e);
  } catch(...) {
    XCEPT_RAISE(xdaq::exception::Exception, "Unknown exception caught in constructor");
  }
  
}


void lumiHCALLUMIDAQ::StartReadout(uint32_t runNumber) throw (xdaq::exception::Exception) {
  using namespace std;
  std::string exceptionMsg;
 
  // A little debug
  LOG4CPLUS_INFO(getApplicationLogger(),"In StartReadout()");

  // Make sure readout is stopped and clean first
  StopReadout();

  // A little debug
  LOG4CPLUS_DEBUG(getApplicationLogger(),"Constructing nibble collector");

  try {

    // Create the nibble collector
    mNibbleCollector = new HCAL_HLX::NibbleCollector(m_numHLXs,
						     m_numBunches,
						     m_numOrbits,
						     m_interfacePort,
						     m_interfaceAddress);
    if ( mNibbleCollector == 0 ) {
      exceptionMsg = "NibbleCollector could not be constructed";
      goto failure_exception;
    }
    
    LOG4CPLUS_DEBUG(getApplicationLogger(),"Constructing section collectors");
    
    // Create the section collectors
    mSectionCollectorShort = new HCAL_HLX::SectionCollector(m_numBunches,
							    m_shortSectionLength,
							    m_numOrbits,
							    m_numHLXs,
							    runNumber);
    if ( mSectionCollectorShort == 0 ) {
      exceptionMsg = "Short section collector could not be constructed";
      goto failure_exception;
    }
    
    mSectionCollectorLong = new HCAL_HLX::SectionCollector(m_numBunches,
							   m_longSectionLength,
							   m_numOrbits,
							   m_numHLXs,
							   runNumber);
    if ( mSectionCollectorLong == 0 ) {
      exceptionMsg = "Long section collector could not be constructed";
      goto failure_exception;
    }
    
    // Connect things and set next run number (just in case we see OC0)
    mSectionCollectorShort->SetNextRunNumber(runNumber);
    mSectionCollectorLong->SetNextRunNumber(runNumber);
    
    LOG4CPLUS_DEBUG(getApplicationLogger(), "Attaching section collectors");
    
    // Attach to the nibble collector
    mNibbleCollector->AttachSectionCollector(mSectionCollectorShort);
    mNibbleCollector->AttachSectionCollector(mSectionCollectorLong);
    
    // Create the distributors
    if ( m_useSectionList ) {
      LOG4CPLUS_DEBUG(getApplicationLogger(), "Constructing section list distributors");

      mShortSectionListDistributor = new HCAL_HLX::SectionListDistributor("/tmp/shortSectionList.log");
      if ( mShortSectionListDistributor == 0 ) {
	exceptionMsg = "Short section list distributor could not be constructed";
	goto failure_exception;
      }
      mSectionCollectorShort->AttachDistributor(mShortSectionListDistributor);

      mLongSectionListDistributor = new HCAL_HLX::SectionListDistributor("/tmp/longSectionList.log");
      if ( mLongSectionListDistributor == 0 ) {
	exceptionMsg = "Long section list distributor could not be constructed";
	goto failure_exception;
      }
      mSectionCollectorLong->AttachDistributor(mLongSectionListDistributor);
    }

    if ( m_useDIP ) {
      LOG4CPLUS_DEBUG(getApplicationLogger(), "Constructing DIP distributor");
      mDIPDistributor = new HCAL_HLX::DIPDistributor;
      if ( mLongSectionListDistributor == 0 ) {
	exceptionMsg = "DIP distributor could not be constructed";
	goto failure_exception;
      }
      mSectionCollectorShort->AttachDistributor(mDIPDistributor);
    }

    if ( m_useShortTCP ) {
      LOG4CPLUS_DEBUG(getApplicationLogger(), "Constructing short TCP distributor");
      mShortSectionTCPDistributor = new HCAL_HLX::TCPDistributor(m_hostName,
								 m_tcpShortPort);
      if ( mShortSectionTCPDistributor == 0 ) {
	exceptionMsg = "Short TCP distributor could not be constructed";
	goto failure_exception;
      }
      mSectionCollectorShort->AttachDistributor(mShortSectionTCPDistributor);
    }
    
    if ( m_useLongTCP ) {
      LOG4CPLUS_DEBUG(getApplicationLogger(), "Constructing long TCP distributor");
      mLongSectionTCPDistributor = new HCAL_HLX::TCPDistributor(m_hostName,
								m_tcpLongPort);
      if ( mLongSectionTCPDistributor == 0 ) {
	exceptionMsg = "Long TCP distributor could not be constructed";
	goto failure_exception;
      }
      mSectionCollectorLong->AttachDistributor(mLongSectionTCPDistributor);
    }
    
    if ( m_useOracle ) {
      LOG4CPLUS_DEBUG(getApplicationLogger(), "Constructing section list distributors");
      mOracleDistributor = new HCAL_HLX::OracleDistributor(m_oracleDBUserName,
							   m_oracleDBPassword,
							   m_oracleDBLocation,
							   m_oracleDBSchema);
      if ( mOracleDistributor == 0 ) {
	exceptionMsg = "Oracle distributor could not be constructed";
	goto failure_exception;
      }
      mSectionCollectorLong->AttachDistributor(mOracleDistributor);
    }
    
    if ( m_useGIF ) {
      LOG4CPLUS_DEBUG(getApplicationLogger(), "Constructing section list distributors");
      mGIFDistributor = new HCAL_HLX::GIFDistributor;
      if ( mGIFDistributor == 0 ) {
	exceptionMsg = "GIF distributor could not be constructed";
	goto failure_exception;
      }
      mSectionCollectorLong->AttachDistributor(mGIFDistributor);
    }
    
    // Automatically turn on test distributors
    // and section collectors if the HLXs are in debug
    // or the fake generator is switched on
    if ( m_debugData ) {
      LOG4CPLUS_DEBUG(getApplicationLogger(), "Constructing test distributors and section collectors");
      mTestSectionCollector = new TestSectionCollector(m_numHLXs,
						       m_numOrbits);
      if ( mTestSectionCollector == 0 ) {
	exceptionMsg = "Test section collector could not be constructed";
	goto failure_exception;
      }
      mNibbleCollector->AttachSectionCollector(mTestSectionCollector);
      mShortTestDistributor = new TestDistributor;
      if ( mShortTestDistributor == 0 ) {
	exceptionMsg = "Short section test distributor could not be constructed";
	goto failure_exception;
      }
      mSectionCollectorShort->AttachDistributor(mShortTestDistributor);
      mLongTestDistributor = new TestDistributor;
      if ( mLongTestDistributor == 0 ) {
	exceptionMsg = "Long section test distributor could not be constructed";
	goto failure_exception;
      }
      mSectionCollectorLong->AttachDistributor(mLongTestDistributor);
    }
    
    LOG4CPLUS_INFO(getApplicationLogger(),"Starting nibble collector");
    
    // LAST THING - Start the threads for the nibble collector
    mNibbleCollector->Start();
    
  } catch(ICException & aExc) {
    exceptionMsg = aExc.what();
    goto failure_exception;
  }

  // return here
  return;

 failure_exception:
  // Clean up partially-constructed readout
  // TODO - make sure this can't throw an exception or it may not clean up properly
  StopReadout();

  // Raise a XDAQ exception with the error that caused startup to fail
  XCEPT_RAISE(xdaq::exception::Exception, exceptionMsg);
  
}


void lumiHCALLUMIDAQ::StopReadout() throw (xdaq::exception::Exception) {
  LOG4CPLUS_INFO(getApplicationLogger(),"In StopReadout()");

  // FIRST THING - Stop the nibble collector threads
  if ( mNibbleCollector ) {
    LOG4CPLUS_DEBUG(getApplicationLogger(),"Stopping NibbleCollector");
    mNibbleCollector->Stop();
    LOG4CPLUS_DEBUG(getApplicationLogger(),"NibbleCollector stopped");
    delete mNibbleCollector; mNibbleCollector = 0;
  }

  if ( mSectionCollectorShort ) {
    LOG4CPLUS_DEBUG(getApplicationLogger(),"Deleting short section collector");
    delete mSectionCollectorShort; mSectionCollectorShort = 0;
  }
  if ( mSectionCollectorLong ) {
    LOG4CPLUS_DEBUG(getApplicationLogger(),"Deleting long section collector");
    delete mSectionCollectorLong; mSectionCollectorLong = 0;
  }

  if ( mShortSectionListDistributor ) {
    LOG4CPLUS_DEBUG(getApplicationLogger(),"Deleting short section list distributor");
    delete mShortSectionListDistributor; mShortSectionListDistributor = 0;
  }
  if ( mLongSectionListDistributor ) {
    LOG4CPLUS_DEBUG(getApplicationLogger(),"Deleting long section list distributor");
    delete mLongSectionListDistributor; mLongSectionListDistributor = 0;
  }
  if ( mDIPDistributor ) {
    LOG4CPLUS_DEBUG(getApplicationLogger(),"Deleting DIP distributor");
    delete mDIPDistributor; mDIPDistributor = 0;
  }
  if ( mGIFDistributor ) {
    LOG4CPLUS_DEBUG(getApplicationLogger(),"Deleting GIF distributor");
    delete mGIFDistributor; mGIFDistributor = 0;
  }
  if ( mOracleDistributor ) {
    LOG4CPLUS_DEBUG(getApplicationLogger(),"Deleting oracle distributor");
    delete mOracleDistributor; mOracleDistributor = 0;
  }
  if ( mShortSectionTCPDistributor ) {
    LOG4CPLUS_DEBUG(getApplicationLogger(),"Deleting short TCP distributor");
    delete mShortSectionTCPDistributor; mShortSectionTCPDistributor = 0;
  }
  if ( mLongSectionTCPDistributor ) {
    LOG4CPLUS_DEBUG(getApplicationLogger(),"Deleting long TCP distributor");
    delete mLongSectionTCPDistributor; mLongSectionTCPDistributor = 0;
  }

  // Debug controls
  if ( mTestSectionCollector ) {
    LOG4CPLUS_DEBUG(getApplicationLogger(),"Deleting test section collector");
    delete mTestSectionCollector; mTestSectionCollector = 0;
  }
  if ( mShortTestDistributor ) {
    LOG4CPLUS_DEBUG(getApplicationLogger(),"Deleting short test distributor");
    delete mShortTestDistributor; mShortTestDistributor = 0;
  }
  if ( mLongTestDistributor ) {
    LOG4CPLUS_DEBUG(getApplicationLogger(),"Deleting long test distributor");
    delete mLongTestDistributor; mLongTestDistributor = 0;
  }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

lumiHCALLUMIDAQ::~lumiHCALLUMIDAQ() {
  try {
    // Just make sure the readout is stopped
    StopReadout();
    StopFakeHLX();

    //mWorkerThreadContinue=false;
    //pthread_join(mThreadId,NULL);
    //mThreadId=0;
    //delete mNibbleCollector;
    //mNibbleCollector=0;
  } catch(ICException &aExc) {
    XCEPT_RAISE(xdaq::exception::Exception,aExc.what());
  } catch(xgi::exception::Exception &e) {
    string errMsg = "Exception caught in ";
    errMsg += __PRETTY_FUNCTION__;
    XCEPT_RETHROW(xdaq::exception::Exception, errMsg, e);
  } catch(...) {
    XCEPT_RAISE(xdaq::exception::Exception, "Unknown exception caught in constructor");
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*   void lumiHCALLUMIDAQ::TransmitterControl(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception) {
    try {
      // Extract the board ID from the cgi input
      i32 boardID = this->ExtractBoardID(in);

      mLastPage = "TransmitterControl";
      std::string stringTemp;
      std::stringstream sstreamTemp;
      std::string link = "/";
      link += getApplicationDescriptor()->getURN().c_str();
      link += "/";
      mLastPage += "?board=";
      sstreamTemp << dec << boardID << endl;
      sstreamTemp >> stringTemp;
      mLastPage += stringTemp;
      link += mLastPage;

      std::string cmdLink = "/";
      cmdLink += getApplicationDescriptor()->getURN().c_str();
      cmdLink += "/TransmitterControl";

      // Update the status of the whole system
      mHardwareManager->UpdateSystemStatus();

      // Construct the menus, FSM controls, etc...
      this->WebConstructPageHeader(out,"Transmitter Options");

      // *out << "<META HTTP-EQUIV=\"Refresh\" CONTENT=\"30; URL=" << link << "\">";

      *out << cgicc::form().set("method","GET").set("action", cmdLink) << std::endl;
      *out << "<font style=\"font-family:arial\">Board(s) selected:</font>" << std::endl;
      *out << cgicc::select().set("name","board");
      if ( boardID == -1 ) {
	*out << cgicc::option().set("selected") << "All" << cgicc::option() << endl;
      } else {
	*out << cgicc::option() << "All" << cgicc::option() << endl;
      }
      for ( i32 i = 0 ; i != static_cast<i32>(mHardwareManager->GetNumBoards()) ; i++ ) {
	sstreamTemp << dec << i << endl;
	sstreamTemp >> stringTemp;
	if ( i == boardID ) {
	  *out << cgicc::option().set("value",stringTemp).set("selected") << i+1 << " (UID: " << static_cast<u16>(mHardwareManager->GetUID(i)) << ")" << cgicc::option() << endl;
	} else {
	  *out << cgicc::option().set("value",stringTemp) << i+1 << " (UID: " << static_cast<u16>(mHardwareManager->GetUID(i)) << ")" << cgicc::option() << endl;
	}
      }
      *out << cgicc::select();
      *out << cgicc::input().set("type","submit").set("value","Update") << std::endl;
      *out << cgicc::form() << "<br/>" << std::endl;

      SC_TRANSMITTER_CONFIG localCfg;
      const SC_TRANSMITTER_CONFIG *currentLocalCfg;
      if ( boardID == -1 ) {
	// Get the global config
	const SC_TRANSMITTER_CONFIG *theConfiguration = mHardwareManager->GetTransmitterConfig();
	memcpy(&localCfg,theConfiguration,sizeof(SC_TRANSMITTER_CONFIG));
	currentLocalCfg = (mHardwareManager->GetCurrentTransmitterConfig());
      } else {
	// Get the individual config
	const SC_TRANSMITTER_CONFIG *theConfiguration = mHardwareManager->GetTransmitterConfig(boardID);
	memcpy(&localCfg,theConfiguration,sizeof(SC_TRANSMITTER_CONFIG));
	currentLocalCfg = (mHardwareManager->GetCurrentTransmitterConfig(boardID));
      }

      *out << "<table border=0 cellpadding=0 cellspacing=0 width=975><tr><td width=675>" << std::endl;



////////////////// software trigger and reset //////////////////////////////////////////////
        sstreamTemp << dec << boardID << endl;
        sstreamTemp >> stringTemp;

        std::string softwareTriggerLink = "/";
        softwareTriggerLink += getApplicationDescriptor()->getURN().c_str();
        softwareTriggerLink += "/ParameterSet?setting=SoftwareTransmitterTrigger";
        if ( boardID != -1 ) {
	  softwareTriggerLink += "?board=";
  	  softwareTriggerLink += stringTemp;
        }

        std::string softwareResetLink = "/";
        softwareResetLink += getApplicationDescriptor()->getURN().c_str();
        softwareResetLink += "/ParameterSet?setting=SoftwareTransmitterReset";
        if ( boardID != -1 ) {
	  softwareResetLink += "?board=";
	  softwareResetLink += stringTemp;
        }
      
        *out << "<table border=0 cellpadding=0 cellspacing=1 width=675 style=\"font-family:arial;font-size:12pt;color:#0000AA\"><tr>" << endl;
        if (currentLocalCfg->bTriggerSource[0] != true) {
	  *out << "<td width=200><font style=\"font-family:arial;color:#AAAAAA\">Software Triggers Disabled</a></td>" << endl;	
        } else {
	  *out << "<td width=200><a href=\"" << softwareTriggerLink << "\" style=\"font-family:arial\">Trigger Transmission</a></td>" << endl;
        }
        if (currentLocalCfg->bResetSource[0] != true) {
	  *out << "<td><font style=\"font-family:arial;color:#AAAAAA\">Software Resets Disabled</a></td>" << endl;	
        } else {
	  *out << "<td><a href=\"" << softwareResetLink << "\" style=\"font-family:arial\">Reset Transmission</a></td>" << endl;
        }
        *out << "</tr></table><p>"<<endl;
      
      std::string titleString = "Transmitter Enables";
      std::string settingString = "SetTransmitterEnables";
      std::string optionValues[10]; optionValues[0] = "1"; optionValues[1] = "2"; optionValues[2] = "3"; optionValues[3] = "4";
      std::string optionStrings[10]; optionStrings[0] = "1"; optionStrings[1] = "2"; optionStrings[2] = "3"; optionStrings[3] = "4";
      u32 defaultValue;
      this->WebShowHighlightOption(out,
				   &titleString,
				   &settingString,
				   4,
				   localCfg.bChannelEnables,
				   optionValues,
				   optionStrings,
				   boardID);
      *out << "<p>" << endl;
      
      titleString = "Trigger Source";
      settingString = "SetTransmitterTriggerSource";
      optionValues[0] = "Software"; optionValues[1] = "RCT"; optionValues[2] = "TTCL1RST"; optionValues[3] = "TTCBC0";
      optionStrings[0] = "Software"; optionStrings[1] = "RCT BC0"; optionStrings[2] = "TTC L1Reset"; optionStrings[3] = "TTC BC0";
      this->WebShowHighlightOption(out,
				   &titleString,
				   &settingString,
				   4,
				   localCfg.bTriggerSource,
				   optionValues,
				   optionStrings,
				   boardID);
      *out << "<p>" << endl;
      
      titleString = "Reset Source";
      settingString = "SetTransmitterResetSource";
      optionValues[0] = "Software"; optionValues[1] = "TTCL1RST"; optionValues[2] = "TTCBC0";
      optionStrings[0] = "Software"; optionStrings[1] = "TTC L1Reset"; optionStrings[2] = "TTC BC0";
      this->WebShowHighlightOption(out,
				   &titleString,
				   &settingString,
				   3,
				   localCfg.bResetSource,
				   optionValues,
				   optionStrings,
				   boardID);
      *out << "<p>" << endl;
      
      titleString = "Trigger Mode";
      settingString = "SetTransmitterTriggerMode";
      optionValues[0] = "Multi"; optionValues[1] = "Loop";
      optionStrings[0] = "Multi"; optionStrings[1] = "Loop"; 
      switch ( localCfg.mTriggerMode ) {
      case HLXInterface::MULTI:
	defaultValue = 0;
	break;
      case HLXInterface::LOOP:
	defaultValue = 1;
	break;
      }
      this->WebShowHighlightOption(out,
				   &titleString,
				   &settingString,
				   2,
				   defaultValue,
				   optionValues,
				   optionStrings,
				   boardID);
      *out << "<p>" << endl;
      
      titleString = "Transmitter Data Mode";
      settingString = "SetTransmitterDataMode";
      optionValues[0] = "RCT"; optionValues[1] = "A5"; optionValues[2] = "Counter"; optionValues[3] = "LFSR"; optionValues[4] = "FIXED"; optionValues[5] = "RAM";
      optionStrings[0] = "RCT data"; optionStrings[1] = "A5"; optionStrings[2] = "Counter"; optionStrings[3] = "LFSR"; optionStrings[4] = "Software"; optionStrings[5] = "Ram";
      switch ( localCfg.mDataMode ) {
      case HLXInterface::RCT:
      default:
	defaultValue = 0;
	break;
      case HLXInterface::A5:
	defaultValue = 1;
	break;
      case HLXInterface::COUNTER:
	defaultValue = 2;
	break;
      case HLXInterface::LFSR:
	defaultValue = 3;
	break;
      case HLXInterface::FIXED:
	defaultValue = 4;
	break;
      case HLXInterface::RAM:
	defaultValue = 5;
	break;
      }
      this->WebShowHighlightOption(out,
				   &titleString,
				   &settingString,
				   6,
				   defaultValue,
				   optionValues,
				   optionStrings,
				   boardID);
      
      std::stringstream temp;

      if (localCfg.mDataMode == HLXInterface::FIXED){
	*out << "<table border=0 cellpadding=0 cellspacing=0 width=675><tr><td bgcolor=\"#DDFFDD\">" << endl;

	//titleString = "Software Data Source";
	//settingString = "SetSWDataSource";
	
	  titleString = "Data Pattern";
	  settingString = "SetSWPatternData";
  	  temp << hex << localCfg.mFixedPatternData[0] << endl; temp >> optionValues[0];
	  temp << hex << localCfg.mFixedPatternData[1] << endl; temp >> optionValues[1];
	  temp << hex << localCfg.mFixedPatternData[2] << endl; temp >> optionValues[2];
	  temp << hex << localCfg.mFixedPatternData[3] << endl; temp >> optionValues[3];
	  this->WebShowFourChannelHexOption(out,
					  &titleString,
					  &settingString,
					  optionValues,
					  boardID);

	if (boardID==-1){	
	  *out << "</td></tr><tr><td bgcolor=\"#DDFFDD\" style=\"font-family:arial;font-size:12pt;color:#0000AA\" align=\"center\">OR</td></tr><tr><td bgcolor=\"#DDFFDD\">" << std::endl;
	  string directory = 	"fixed_pattern_data_files";
	  string extension = 	"dat";
	  string setting   = 	"LoadFixedPatternDataFile";
 	  this->WebShowLocalDataFiles(out, &setting, &directory, &extension);
	}else{	
  	  *out << "</td></tr><tr><td bgcolor=\"#DDFFDD\" style=\"font-family:arial;font-size:12pt;color:#0000AA\" align=\"center\">OR</td></tr><tr><td bgcolor=\"#DDFFDD\">" << std::endl;
	  this->WebShowPhysicalDataOption(out, localCfg.mFixedPatternData,boardID);
	}
	*out << "</td></tr></table>" << std::endl;


      } else if (localCfg.mDataMode == HLXInterface::RAM){
 	    if (!mHardwareManager->GetConfig()->bUseBRAMForTransmitter){
		std::string BRAMuseLink = "/";
		BRAMuseLink += getApplicationDescriptor()->getURN().c_str();
		BRAMuseLink += "/ParameterSet?setting=useBRAMForTransmitter";
		BRAMuseLink += "&value=true";
		*out << "<table border=0 cellpadding=0 cellspacing=0 width=675 style=\"font-family:arial\">" << std::endl;
		*out << "<tr><td bgcolor=\"#DDFFDD\">Block RAM configured for use by RCT Capture.</td></tr>" << std::endl;
		*out << "<tr><td bgcolor=\"#DDFFDD\"><a href=\""<<BRAMuseLink<<"\">Configure Block RAM for Transmitter.<a></td></tr>" << std::endl;
		*out << "</table>" << std::endl;
	    }else{
		*out << "<table border=0 cellpadding=0 cellspacing=0 width=675 ><tr><td bgcolor=\"#DDFFDD\">" << endl;
		string directory = 	"blk_ram_data_files";
		string extension = 	"dat";
		string setting   = 	"LoadBRAMDataFile";
	 	this->WebShowLocalDataFiles(out, &setting, &directory, &extension);
		*out << "</td></tr></table>" << std::endl;
	    }	
      } else if (localCfg.mDataMode == HLXInterface::LFSR){
	*out << "<table border=0 cellpadding=0 cellspacing=0 width=675 ><tr><td bgcolor=\"#DDFFDD\">" << endl;
	
	titleString = "LFSR seed";
	settingString = "SetSWPatternData";

	temp << hex << localCfg.mFixedPatternData[0] << endl; temp >> optionValues[0];
	temp << hex << localCfg.mFixedPatternData[1] << endl; temp >> optionValues[1];
	temp << hex << localCfg.mFixedPatternData[2] << endl; temp >> optionValues[2];
	temp << hex << localCfg.mFixedPatternData[3] << endl; temp >> optionValues[3];
	this->WebShowFourChannelHexOption(out,
					  &titleString,
					  &settingString,
					  optionValues,
					  boardID);
	
	*out << "</td></tr></table>" << std::endl;
	
      } else if (localCfg.mDataMode == HLXInterface::RCT){
	*out << "<table border=0 cellpadding=0 cellspacing=0 width=675><tr><td bgcolor=\"#DDFFDD\">" << endl;
	
	titleString = "Transmitter Mask";
	settingString = "SetTransmitterChannelMask";
	temp << hex << localCfg.mChannelMask[0] << endl; temp >> optionValues[0];
	temp << hex << localCfg.mChannelMask[1] << endl; temp >> optionValues[1];
	temp << hex << localCfg.mChannelMask[2] << endl; temp >> optionValues[2];
	temp << hex << localCfg.mChannelMask[3] << endl; temp >> optionValues[3];
	this->WebShowFourChannelHexOption(out,
					  &titleString,
					  &settingString,
					  optionValues,
					  boardID);
	
	*out << "</td></tr></table>" << std::endl;
      }
      
      *out << "<p>" << endl;
      
      titleString = "Trigger Delay";
      settingString = "SetTransmitterDelayPeriod";
      temp << dec << localCfg.mDelayPeriod << endl;
      temp >> optionValues[0];
      this->WebShowTextOption(out,
			      &titleString,
			      &settingString,
			      &optionValues[0],
			      boardID);
      *out << "<p>" << endl;
      
      titleString = "Running Period";
      settingString = "SetTransmitterRunningPeriod";
      temp << dec << localCfg.mRunningPeriod+1 << endl;
      temp >> optionValues[0];
      this->WebShowTextOption(out,
			      &titleString,
			      &settingString,
			      &optionValues[0],
			      boardID);
      *out << "<p>" << endl;
      
      titleString = "Loop Period";
      settingString = "SetTransmitterLoopPeriod";
      temp << dec << localCfg.mLoopPeriod << endl;
      temp >> optionValues[0];
      this->WebShowTextOption(out,
			      &titleString,
			      &settingString,
			      &optionValues[0],
			      boardID);
      *out << "<p>" << endl;
        
      // Now we do the orbit structure

      settingString = "TransmitterDataPattern";
      titleString = "Transmitter Data Pattern";
      std::string ModeOptions[4]; ModeOptions[0] = "DATA"; ModeOptions[1] = "COMMA"; ModeOptions[2] = "CRC"; ModeOptions[3] = "STATUS";
      std::string currentMode[8192] ;
      std::string currentLength[8192];
      std::stringstream Temp;
      u32 numOptions=4;
      u32 numTerms=localCfg.mNumBRAMDataEntries;


     for ( u32 i = 0 ; i != localCfg.mNumBRAMDataEntries ; i++ ) {
	switch (localCfg.mBRAMData[i].mode) {
	case HLXInterface::DATA:
	  currentMode[i]="DATA"; break;
	case HLXInterface::COMMA:
	  currentMode[i]="COMMA"; break;
	case HLXInterface::CRC:
	  currentMode[i]="CRC"; break;
	case HLXInterface::STATUS:
	  currentMode[i]="STATUS"; break;
	}

	Temp << localCfg.mBRAMData[i].length<<endl;
	Temp >> currentLength[i];
      }

       this->WebShowModeLengthOption(out,
					&titleString,
					&settingString,
					numTerms,
					&currentMode[0],
					&ModeOptions[0],
					numOptions,
					&currentLength[0],
					boardID);


        *out << "</td><td>&nbsp;</td></tr></table><br/>" << std::endl;


 
// now the bar chart...
	int color_r=0;
	int color_g=0;
	int color_b=0;
	int temp_length=0;

      *out << "<br><table border=0 cellpadding=2 cellspacing=0 width=905><tr><td rowspan=100>" << std::endl;
        *out << "<table border=0 cellpadding=0 cellspacing=0 width=100 style=\"font-family:arial;font-size:10pt;color:#000000\">" << std::endl;
        *out << "<tr><td style=\"text-align:center; background-color: rgb(255,0,0)\">DATA</td></tr>";
        *out << "<tr><td style=\"text-align:center; background-color: rgb(0,255,0)\">COMMA</td></tr>";
        *out << "<tr><td style=\"text-align:center; background-color: rgb(0,0,255)\">CRC</td></tr>";
        *out << "<tr><td style=\"text-align:center; background-color: rgb(255,0,255)\">STATUS</td></tr>";
        *out << "</table>" << std::endl;

      *out << "<td><table border=0 cellpadding=0 cellspacing=0 width=800 style=\"font-family:arial;font-size:10pt;color:#000000\"><tr height=50>" << endl;

         for (int i=0; i<numTerms; i++){

	  if (atoi(currentLength[i].c_str())>0){
 
	  switch (localCfg.mBRAMData[i].mode) {
	  case HLXInterface::DATA:
	    color_r=255; color_g=0; color_b=0; break;
	  case HLXInterface::COMMA:
	    color_r=0; color_g=255; color_b=0; break;
	  case HLXInterface::CRC:
	    color_r=0; color_g=0; color_b=255; break;
	  case HLXInterface::STATUS:
	    color_r=255; color_g=0; color_b=255; break;
	  }
	  
	  if (atoi(currentLength[i].c_str())<10){
 	    if (temp_length + 20*atoi(currentLength[i].c_str()) > 800) { 
	      *out << "</td></tr></table></td><td>&nbsp;</td></tr><tr><td>"<<endl;
	      *out << "<table border=0 cellpadding=0 cellspacing=0 width=800 style=\"font-family:arial;font-size:10pt;color:#000000\"><tr height=50>" << endl;
 	      temp_length = 0;
	    }
  	    *out << "<td style=\"text-align:center; background-color: rgb(" << color_r <<", "<< color_g <<","<< color_b <<")\" width="<< (20*atoi(currentLength[i].c_str()))  << ">";
	    *out <<  currentLength[i] << "</td>" << endl;
	    temp_length += 20*atoi(currentLength[i].c_str());

	  }else{
	    if (temp_length > 600) { 
	      *out << "</td></tr></table></td><td>&nbsp;</td></tr><tr><td>"<<endl;
	      *out << "<table border=0 cellpadding=0 cellspacing=0 width=800 style=\"font-family:arial;font-size:10pt;color:#000000\"><tr height=50>" << endl;
 	      temp_length = 0;
	    }

   	    *out << "<td style=\"text-align:center; background-color: rgb(" << color_r <<","<< color_g <<","<< color_b <<")\" width=100>";
	    *out <<  currentLength[i] << "</td>" << endl;
 	    temp_length += 200;
	    for (int j=0; j<25; j++){
  	      *out << "<td style=\"background-color: rgb(" << int(color_r+(255-color_r)*j/25) <<","<< int(color_g+(255-color_g)*j/25) <<","<< int(color_b+(255-color_b)*j/25) <<")\" width=2\"></td>";
	    }
	    for (int j=24; j>=0; j--){
  	      *out << "<td style=\"background-color: rgb(" << int(color_r+(255-color_r)*j/25) <<","<< int(color_g+(255-color_g)*j/25) <<","<< int(color_b+(255-color_b)*j/25) <<")\" width=2\"></td>";
	    }	
	
	  }
	}	

	}
      *out <<endl<< "<td>&nbsp;</td></tr></table>"<<endl<<"</td></tr></table>" << endl;

	
     
      // Always put the page footer LAST
      xgi::Utils::getPageFooter(*out);
      
    } catch(ICException &aExc) {
      XCEPT_RAISE(xgi::exception::Exception,aExc.what());
    } catch(xgi::exception::Exception &e) {
      string errMsg = "Exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RETHROW(xgi::exception::Exception, errMsg, e);
    } catch(...) {
      string errMsg = "Unknown exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RAISE(xgi::exception::Exception, errMsg);
    }

  }*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*  void lumiHCALLUMIDAQ::RCTCapture(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception) {
    try {      
      // Extract the board ID from the cgi input
      //i32 boardIDtemp = this->ExtractBoardID(in);
      // if ( boardIDtemp == -1 ) boardIDtemp = 0;
      //u32 boardID = static_cast<u32>(boardIDtemp);

      i32 boardID = this->ExtractBoardID(in);

      mLastPage = "RCTCapture";
      std::string stringTemp;
      std::stringstream sstreamTemp;
      std::string link = "/";
      link += getApplicationDescriptor()->getURN().c_str();
      link += "/";
      mLastPage += "?board=";
      sstreamTemp << dec << boardID << endl;
      sstreamTemp >> stringTemp;
      mLastPage += stringTemp;
      link += mLastPage;

      // Update the status of the whole system
      mHardwareManager->UpdateSystemStatus();

      // Construct the menus, FSM controls, etc...
      this->WebConstructPageHeader(out,"RCT Capture");

      std::string cmdLink = "/";
      cmdLink += getApplicationDescriptor()->getURN().c_str();
      cmdLink += "/RCTCapture";

    if ( mHardwareManager->GetConfig()->bUseBRAMForTransmitter){

      std::string BRAMuseLink = "/";
      BRAMuseLink += getApplicationDescriptor()->getURN().c_str();
      BRAMuseLink += "/ParameterSet?setting=useBRAMForTransmitter";
      BRAMuseLink += "&value=false";

      *out << "<table border=0 cellpadding=0 cellspacing=0 width=675 style=\"font-family:arial\">" << std::endl;
      *out << "<tr><td>Block RAM configured for use by Transmitter.</td></tr>" << std::endl;
      *out << "<tr><td><a href=\""<<BRAMuseLink<<"\">Configure Block RAM for RCT Capture.<a></td></tr>" << std::endl;
      *out << "</table>" << std::endl;


    }else{

      *out << cgicc::form().set("method","GET").set("action", cmdLink) << std::endl;
      *out << "<font style=\"font-family:arial\">Board(s) selected:</font>" << std::endl;
      *out << cgicc::select().set("name","board");
      if ( boardID == -1 ) {
	*out << cgicc::option().set("selected") << "All" << cgicc::option() << endl;
      } else {
	*out << cgicc::option() << "All" << cgicc::option() << endl;
      }
      for ( u32 i = 0 ; i != mHardwareManager->GetNumBoards() ; i++ ) {
	sstreamTemp << dec << i << endl;
	sstreamTemp >> stringTemp;
	if ( i == boardID ) {
	  *out << cgicc::option().set("value",stringTemp).set("selected") << i+1 << " (UID: " << static_cast<u16>(mHardwareManager->GetUID(i)) << ")" << cgicc::option() << endl;
	} else {
	  *out << cgicc::option().set("value",stringTemp) << i+1 << " (UID: " << static_cast<u16>(mHardwareManager->GetUID(i)) << ")" << cgicc::option() << endl;
	}
      }
      *out << cgicc::select();
      *out << cgicc::input().set("type","submit").set("value","Update") << std::endl;
      *out << cgicc::form() << "<br/>" << std::endl;

      const SC_CAPTURE_CONFIG *localCfg;
      const SC_CAPTURE_CONFIG *currentLocalCfg;
      if ( boardID == -1 ) {
	// Get the global config
        localCfg = (mHardwareManager->GetCaptureConfig());
        currentLocalCfg = (mHardwareManager->GetCurrentCaptureConfig());
      } else {
	// Get the individual config
        localCfg = (mHardwareManager->GetCaptureConfig(boardID));
        currentLocalCfg = (mHardwareManager->GetCurrentCaptureConfig(boardID));
      }

      *out << "<table border=0 cellpadding=0 cellspacing=0 width=975><tr><td width=675>" << std::endl;

/////////////////////////// software trigger and reset //////////////////////////////////////////

        sstreamTemp << dec << boardID << endl;
        sstreamTemp >> stringTemp;

        std::string softwareTriggerLink = "/";
        softwareTriggerLink += getApplicationDescriptor()->getURN().c_str();
        softwareTriggerLink += "/ParameterSet?setting=SoftwareCaptureTrigger";
        if ( boardID != -1 ) {
  	  softwareTriggerLink += "?board=";
  	  softwareTriggerLink += stringTemp;
        }

        std::string softwareResetLink = "/";
        softwareResetLink += getApplicationDescriptor()->getURN().c_str();
        softwareResetLink += "/ParameterSet?setting=SoftwareCaptureReset";
        if ( boardID != -1 ) {
	  softwareResetLink += "?board=";
    	  softwareResetLink += stringTemp;
        }


        *out << "<table border=0 cellpadding=0 cellspacing=1 width=675 style=\"font-family:arial;font-size:12pt;color:#0000AA\"><tr>" << endl;
        if (currentLocalCfg->bCaptureTriggerSources[0] != true) {
	  *out << "<td width=200><font style=\"font-family:arial;color:#AAAAAA\">Software Trigger Disabled</a></td>" << endl;	
        } else {
  	  *out << "<td width=200><a href=\"" << softwareTriggerLink << "\" style=\"font-family:arial\">Trigger Capture</a></td>" << endl;
        }
        if (currentLocalCfg->bCaptureResetSources[0] != true) {
	  *out << "<td><font style=\"font-family:arial;color:#AAAAAA\">Software Reset Disabled</a></td>" << endl;	
        } else {
          *out << "<td><a href=\"" << softwareResetLink << "\" style=\"font-family:arial\">Capture Reset</a></td>" << endl;
        }
        *out << "</tr></table><p>"<<endl;


      std::string titleString = "Trigger Source";
      std::string settingString = "SetCaptureTriggerSource";
      std::string optionValues[10]; optionValues[0] = "Software"; optionValues[1] = "RCTBC0"; optionValues[2] = "TTCResync"; optionValues[3] = "TTCBC0"; optionValues[4] = "Pattern";
      std::string optionStrings[10]; optionStrings[0] = "Software"; optionStrings[1] = "RCT BC0"; optionStrings[2] = "TTC Resync"; optionStrings[3] = "TTC BC0"; optionStrings[4] = "Pattern";
      this->WebShowHighlightOption(out,&titleString,&settingString,5,localCfg->bCaptureTriggerSources,optionValues,optionStrings,boardID);

      std::stringstream temp;

      if (localCfg->bCaptureTriggerSources[4]==true){  
	*out << "<table border=0 cellpadding=0 cellspacing=0 width=675 height=1><tr><td width=576></td><td width=99 bgcolor=\"#DDFFDD\" style=\"font-size:1pt\">&nbsp;</td></tr></table>" << endl;
	*out << "<table border=0 cellpadding=0 cellspacing=0 width=675><tr><td bgcolor=\"#DDFFDD\">" << endl;
	
	titleString = "Capture Mask";
	settingString = "SetCaptureMask";
	temp << hex << localCfg->mCaptureMask[0] << endl; temp >> optionValues[0];
	temp << hex << localCfg->mCaptureMask[1] << endl; temp >> optionValues[1];
	temp << hex << localCfg->mCaptureMask[2] << endl; temp >> optionValues[2];
	temp << hex << localCfg->mCaptureMask[3] << endl; temp >> optionValues[3];
	this->WebShowFourChannelHexOption(out,&titleString,&settingString,optionValues,boardID);
	
	*out << "<p>" << endl;
	
	titleString = "Capture Pattern";
	settingString = "SetCapturePattern";
	temp << hex << localCfg->mCapturePattern[0] << endl; temp >> optionValues[0];
	temp << hex << localCfg->mCapturePattern[1] << endl; temp >> optionValues[1];
	temp << hex << localCfg->mCapturePattern[2] << endl; temp >> optionValues[2];
	temp << hex << localCfg->mCapturePattern[3] << endl; temp >> optionValues[3];
	this->WebShowFourChannelHexOption(out,&titleString,&settingString,optionValues,boardID);
	
	*out << "</td></tr></table>" << std::endl;
      }

      *out <<"<p>" << endl;
      
      titleString = "Reset Source";
      settingString = "SetCaptureResetSource";
      optionValues[0] = "Software"; optionValues[1] = "TTCResync"; optionValues[2] = "TTCBC0";
      optionStrings[0] = "Software"; optionStrings[1] = "TTC Resync"; optionStrings[2] = "TTC BC0";
      this->WebShowHighlightOption(out,&titleString,&settingString,3,localCfg->bCaptureResetSources,optionValues,optionStrings,boardID);
      *out << "<p>" << endl;

      titleString = "Delay Period";
      settingString = "SetCaptureDelayPeriod";
      temp << dec << localCfg->mCaptureDelayPeriod << endl;
      temp >> optionValues[0];
      this->WebShowTextOption(out,&titleString,&settingString,&optionValues[0],boardID);
      *out << "<p>" << endl;

      *out << cgicc::form().set("method","GET").set("action", cmdLink) << std::endl;
      *out << "<font style=\"font-family:arial\"></font>" << std::endl;
     *out << cgicc::select().set("name","SetCaptureDepth");
      for ( int i = 6 ; i < 13 ; i++ ) {
	sstreamTemp << dec << (0x2<<i) << endl;
	sstreamTemp >> stringTemp;
	if ( (0x2<<i) == localCfg->mCaptureDepth ) {
	  *out << cgicc::option().set("value",stringTemp).set("selected") << stringTemp << cgicc::option() << endl;
	} else {
	  *out << cgicc::option().set("value",stringTemp) << stringTemp << cgicc::option() << endl;
	}
      }
      *out << cgicc::select();
      *out << cgicc::input().set("type","submit").set("value","Update") << std::endl;
      *out << cgicc::form() << "<p/>" << std::endl;


      *out<<"</td><td align=CENTER>"<<endl;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
if ( boardID == -1 ) {
*out << "&nbsp;" << endl;
} else {
// *out << "<table border=1 cellpadding=0 cellspacing=0 width=300 style=\"font-family:arial;font-size:10pt\">"
//<< "<tr>"
//<< "<td width=70 align=CENTER>Has Fired</td>"
		//<< "<td width=90 align=CENTER>Fifo Full</td>"
		//<< "<td width=90 align=CENTER>Fifo Empty</td>"
		//<< "<td width=80></td>"
		//<<endl;

        cmdLink = "/";
        cmdLink += getApplicationDescriptor()->getURN().c_str();
        cmdLink += "/ReadRCTData";

	*out << "Has Fired = " << (*mHardwareManager)[boardID].bCaptureHasFired << "<br>" << endl;
	*out << "Capture Fifo Empty = " << (*mHardwareManager)[boardID].bCaptureFIFOEmpty << "<br>" << endl;
	*out << "Capture Fifo Full = " << (*mHardwareManager)[boardID].bCaptureFIFOFull << "<br>" << endl;
	*out << "Needs Reset = " << (*mHardwareManager)[boardID].bCaptureNeedsReset << "<br>" << endl;
	
	*out <<"<a style=\"text-decoration:none\" href=\"" << cmdLink << "?board="<<boardID<< "\" target=\"_blank\">Read</a></td>" << endl;
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


       *out << "</td></tr></table>" << endl;

    } //if bUseBRAMforTransmitter

      // Always put the page footer LAST
      xgi::Utils::getPageFooter(*out);

    } catch(ICException &aExc) {
      XCEPT_RAISE(xgi::exception::Exception,aExc.what());
    } catch(xgi::exception::Exception &e) {
      string errMsg = "Exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RETHROW(xgi::exception::Exception, errMsg, e);
    } catch(...) {
      string errMsg = "Unknown exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RAISE(xgi::exception::Exception, errMsg);
    }
    }*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* void lumiHCALLUMIDAQ::ReadRCTData(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception) {
     try {
      i32 boardID = this->ExtractBoardID(in);

     // Construct the menus, FSM controls, etc...
     //this->WebConstructPageHeader(out, "Read RCT Data");     

    *out << "<html><body>" << endl;
 

     u32 lDataA[4096] = {0};
     u32 lDataB[4096] = {0};
    
      *out << "<font face=\"arial\">boardID="<< boardID << "</font><p>" << endl;
      cout << "----------------------------> boardID="<< boardID << endl;
   
       mHardwareManager->GetRCTCaptureData(boardID , lDataA, lDataB);
       
	cout << "Got RCT data" <<endl;

       *out << "<table border=0 cellspacing=0 cellpadding=0 style=\"font-family:arial;font-size:10pt\">" << endl;
       *out<<"<tr><td width=150></td><td width=150 align=CENTER>SCSI:0</td><td width=150 align=CENTER>SCSI:1</td></tr>"<<endl;  
       
       for (GCT_SC::u16 i=0; i< (mHardwareManager->GetCaptureConfig(boardID)->mCaptureDepth); i++){
	 *out<<"<tr><td align=CENTER>"<<dec<<i<<"</td><td align=CENTER>"<<hex<<lDataA[i]<<"</td><td align=CENTER>"<<hex<<lDataB[i]<<"</td></tr>"<<endl;  
       }
       *out << "</table>"<<endl;
       
     
     // Always put the page footer LAST
     //xgi::Utils::getPageFooter(*out);
      *out << "</body></html>" << endl;
 
	
   } catch(ICException &aExc) {
     XCEPT_RAISE(xgi::exception::Exception,aExc.what());
   } catch(xgi::exception::Exception &e) {
     string errMsg = "Exception caught in ";
     errMsg += __PRETTY_FUNCTION__;
     XCEPT_RETHROW(xgi::exception::Exception, errMsg, e);
   } catch(...) {
     string errMsg = "Unknown exception caught in ";
     errMsg += __PRETTY_FUNCTION__;
     XCEPT_RAISE(xgi::exception::Exception, errMsg);
     }
     }*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*  void lumiHCALLUMIDAQ::GlobalConfig(xgi::Input * in, xgi::Output * out) throw (xgi::exception::Exception) {
    try {      

      mLastPage = "GlobalConfig";
      std::string linkContainer1 = "<a href=\"/";
      std::string linkContainer2 = getApplicationDescriptor()->getURN().c_str();
      std::string linkContainer3 = "\">";
      std::string linkContainer4 = "</a>";

      std::string cfgLink1 = "TTCCounter";
      std::string cfgString1 = "All-Channel Counter Pattern (TTC BC0)";
      std::string cfgLink2 = "TTCA5";
      std::string cfgString2 = "All-Channel A5 Pattern (TTC BC0)";
      std::string cfgLink3 = "TTCLFSR";
      std::string cfgString3 = "All-Channel LFSR Pattern (TTC BC0)";
      std::string cfgLink4 = "TTCID";
      std::string cfgString4 = "All-Channel ID Pattern (TTC BC0)";

      // Update the hardware status
      mHardwareManager->UpdateSystemStatus();

      // Construct the menus, FSM controls, etc...
      this->WebConstructPageHeader(out,"Global Configuration");

      std::string cmdLink = "/ParameterSet?setting=ConfigMacro&value=";

      // Show the links
      *out << linkContainer1 << linkContainer2 << cmdLink << cfgLink1 << linkContainer3 << cfgString1 << linkContainer4 << "<br/>" << endl;
      *out << linkContainer1 << linkContainer2 << cmdLink << cfgLink2 << linkContainer3 << cfgString2 << linkContainer4 << "<br/>" << endl;
      *out << linkContainer1 << linkContainer2 << cmdLink << cfgLink3 << linkContainer3 << cfgString3 << linkContainer4 << "<br/>" << endl;
      *out << linkContainer1 << linkContainer2 << cmdLink << cfgLink4 << linkContainer3 << cfgString4 << linkContainer4 << "<br/>" << endl;
     
      // Always put the page footer LAST
      xgi::Utils::getPageFooter(*out);    

    } catch(ICException &aExc) {
      XCEPT_RAISE(xgi::exception::Exception,aExc.what());
    } catch(xgi::exception::Exception &e) {
      string errMsg = "Exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RETHROW(xgi::exception::Exception, errMsg, e);
    } catch(...) {
      string errMsg = "Unknown exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RAISE(xgi::exception::Exception, errMsg);
    }
    
    }*/

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  /*void lumiHCALLUMIDAQ::IndividualStatus(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception) {    
    try {      
      // Extract the board ID from the cgi input
      i32 boardIDtemp = this->ExtractBoardID(in);
      if ( boardIDtemp == -1 ) boardIDtemp = 0;
      u32 boardID = static_cast<u32>(boardIDtemp);

      mLastPage = "IndividualStatus";
      std::string stringTemp;
      std::stringstream sstreamTemp;
      std::string link = "/";
      link += getApplicationDescriptor()->getURN().c_str();
      link += "/";
      mLastPage += "?board=";
      sstreamTemp << dec << boardID << endl;
      sstreamTemp >> stringTemp;
      mLastPage += stringTemp;
      link += mLastPage;

      // Update the status of the whole system
      mHardwareManager->UpdateSystemStatus();

      // Construct the menus, FSM controls, etc...
      this->WebConstructPageHeader(out,"Individual Status");

      std::string cmdLink = "/";
      cmdLink += getApplicationDescriptor()->getURN().c_str();
      cmdLink += "/IndividualStatus";

      *out << "<META HTTP-EQUIV=\"Refresh\" CONTENT=\"30; URL=" << link << "\">";

      *out << cgicc::form().set("method","GET").set("action", cmdLink) << std::endl;
      *out << "<font style=\"font-family:arial\">Board selected:</font>" << std::endl;
      *out << cgicc::select().set("name","board");
      for ( u32 i = 0 ; i != mHardwareManager->GetNumBoards() ; i++ ) {
	sstreamTemp << dec << i << endl;
	sstreamTemp >> stringTemp;
	if ( i == boardID ) {
	  *out << cgicc::option().set("value",stringTemp).set("selected") << i+1 << " (UID: " << static_cast<u16>(mHardwareManager->GetUID(i)) << ")" << cgicc::option() << endl;
	} else {
	  *out << cgicc::option().set("value",stringTemp) << i+1 << " (UID: " << static_cast<u16>(mHardwareManager->GetUID(i)) << ")" << cgicc::option() << endl;
	}
      }
      *out << cgicc::select();
      *out << cgicc::input().set("type","submit").set("value","Update") << std::endl;
      *out << cgicc::form() << "<br/>" << std::endl;

      // Contruct the individual status table
      *out << "<table style=\"font-family:arial\" border=1 cellpadding=0 cellspacing=0 width=100%><tr><td style=\"font-size:20pt\" colspan=2>General Information</td>" << endl;
      *out << "</tr><tr><td>TTC Master Mode A</td><td width=100>" << hex << (*mHardwareManager)[boardID].mTTCMasterModeA << "</td>" << dec << endl;
      *out << "</tr><tr><td>TTC Master Mode B</td><td width=100>" << hex << (*mHardwareManager)[boardID].mTTCMasterModeB << "</td>" << dec << endl;
      *out << "</tr><tr><td>TTC Chip ID</td><td width=100>" << hex << (*mHardwareManager)[boardID].mTTCChipID << "</td>" << dec << endl;
      *out << "</tr><tr><td>TTC I2C Address</td><td width=100>" << hex << (*mHardwareManager)[boardID].mTTCI2CAddress << "</td>" << dec << endl;
      *out << "</tr><tr><td>TTC Broadcast Decode Shift Register</td><td width=100>" << hex << (*mHardwareManager)[boardID].mTTCDecodeDebug << "</td>" << dec << endl;
      *out << "</tr><tr><td>RCT BC0 Phase Offset</td><td width=100>" << (*mHardwareManager)[boardID].mRCTBC0Phase << "</td>" << endl;
      *out << "</tr><tr><td>TTC BC0 Phase Offset</td><td width=100>" << (*mHardwareManager)[boardID].mTTCBC0Phase << "</td>" << endl;
      *out << "</tr><tr><td>RCT BC0 Period</td><td width=100>" << (*mHardwareManager)[boardID].mRCTBC0Period << "</td>" << endl;
      *out << "</tr><tr><td>TTC BC0 Period</td><td width=100>" << (*mHardwareManager)[boardID].mTTCBC0Period << "</td>" << endl;
      *out << "</tr><tr><td>TTC RESYNC Period</td><td width=100>" << (*mHardwareManager)[boardID].mTTCResyncPeriod << "</td>" << endl;

      *out << "</tr><tr><td>TTC ID</td><td width=100>" << (((*mHardwareManager)[boardID].mTTCChipID>>8)&0xff) << "</td>" << endl;
      *out << "</tr><tr><td>Logical ID</td><td width=100>" << (*mHardwareManager)[boardID].mLogicalID << "</td>" << endl;

      *out << "</tr><tr><td>Board Routing Mode</td><td width=100>";
      switch ( (*mHardwareManager)[boardID].mRoutingMode ) {
      case HLXInterface::ELECTRON:
	*out << "e/mu";
     	break;
      case HLXInterface::RC_234_AND_SISTER:
	*out << "RC 2,3,4 & sister RCT crate";
	break;
      case HLXInterface::HF_AND_RC_56:
	*out << "HF & RC 5,6";
	break;
      case HLXInterface::ETA0_RC_012:
	*out << "RC 0,1,2 & duplicated RC 0,1";
	break;
      }

     *out << "</td>" << endl;

      *out << "</tr></table>" << "<br/>" << std::endl;

      // Contruct the board information table
      *out << "<table style=\"font-family:arial\" border=1 cellpadding=0 cellspacing=0 width=100%>" << endl;      
      *out << "<tr><td style=\"font-size:20pt\" colspan=2>Board Status</td>" << endl;

      // Actual temperature
      *out << "</tr><tr><td>Power temperature</td>" << endl;      
      *out << "<td width=100 style=\"background:#ffffff\">" << static_cast<u16>((*mHardwareManager)[boardID].mPowerTemperature) << "</td>" << endl;

      *out << "</tr><tr><td>ECL temperature</td>" << endl;      
      *out << "<td style=\"background:#ffffff\">" << static_cast<u16>((*mHardwareManager)[boardID].mECLTemperature) << "</td>" << endl;
	
      *out << "</tr><tr><td>Optical link temperature</td>" << endl;      
      *out << "<td style=\"background:#ffffff\">" << static_cast<u16>((*mHardwareManager)[boardID].mOptoTemperature) << "</td>" << endl;

      *out << "</tr><tr><td>Board temperature</td>" << endl;      
      *out << "<td style=\"background:#ffffff\">" << static_cast<u16>((*mHardwareManager)[boardID].mBoardTemperature) << "</td>" << endl;

      *out << "</tr><tr><td style=\"font-size:20pt\" colspan=2>Board Controls</td>" << endl;      

      // Optical module control
      for ( u32 i = 0 ; i != 4 ; i++ ) {
	*out << "</tr><tr><td>Opto Tx " << i+1 << "</td>" << endl;      
	*out << "<td style=\"background:#" <<
	  ((*mHardwareManager)[boardID].bOptoTxEn[i]?((*mHardwareManager)[boardID].bOptoTxFault[i]?"FF0000":"00FF00"):"FFAA00")
	     << "\"></td>" << endl;
      }

      // Serdes control
      for ( u32 i = 0 ; i != 4 ; i++ ) {
	*out << "</tr><tr><td>Transmitter FSM " << i+1 << "</td>" << endl;      
	*out << "<td style=\"background:#" <<
	  ((*mHardwareManager)[boardID].bTransmitterEn[i]?"00FF00":"FFAA00")
	     << "\"></td>" << endl;
      }

      // Clock system
      *out << "</tr><tr><td>Serdes DCM</td>" << endl;      
      *out << "<td style=\"background:#" <<
	((*mHardwareManager)[boardID].bSerdesDCMEnabled?((*mHardwareManager)[boardID].bSerdesDCMLocked?"00FF00":"FF0000"):"FFAA00")
	   << "\"></td>" << endl;

      *out << "</tr><tr><td>TTC DCM</td>" << endl;      
      *out << "<td style=\"background:#" <<
	((*mHardwareManager)[boardID].bTTCDCMEnabled?((*mHardwareManager)[boardID].bTTCDCMLocked?"00FF00":"FF0000"):"FFAA00")
	   << "\"></td>" << endl;

      *out << "</tr><tr><td>TTCrx enabled</td>" << endl;      
      *out << "<td style=\"background:#" << ((*mHardwareManager)[boardID].bTTCrxEnabled?"00FF00":"FFAA00")
	   << "\"></td>" << endl;

      *out << "</tr><tr><td>QPLL</td>" << endl;      
      *out << "<td style=\"background:#" <<
	((*mHardwareManager)[boardID].bQPLLEnabled?(((*mHardwareManager)[boardID].bQPLLLocked && !(*mHardwareManager)[boardID].bQPLLError)?"00FF00":"FF0000"):"FFAA00")
	   << "\"></td>" << endl;
      
      *out << "</tr><tr><td style=\"font-size:20pt\" colspan=\"" << mHardwareManager->GetNumBoards()+1 << "\">Transmitter Status</td>" << endl;      
      
      *out << "</tr><tr><td>RCT BC0 Period Stability</td>" << endl;      
      *out << "<td style=\"background:#" << ((*mHardwareManager)[boardID].bRCTPeriodChange?"FF0000":"00FF00")
	   << "\"></td>" << endl;

      *out << "</tr><tr><td>TTC BC0 Period Stability</td>" << endl;      
      *out << "<td style=\"background:#" << ((*mHardwareManager)[boardID].bTTCBC0PeriodChange?"FF0000":"00FF00")
	   << "\"></td>" << endl;

      *out << "</tr><tr><td>TTC RESYNC Period Stability</td>" << endl;      
      *out << "<td style=\"background:#" << ((*mHardwareManager)[boardID].bTTCRESYNCPeriodChange?"FF0000":"00FF00")
	   << "\"></td>" << endl;

      *out << "</tr><tr><td>RCT BC0 Trigger</td>" << endl;      
      *out << "<td style=\"background:#" << ((*mHardwareManager)[boardID].bRCTTrigger?"00FF00":"FFFFFF")
	   << "\"></td>" << endl;

      *out << "</tr><tr><td>TTC BC0 Trigger</td>" << endl;      
      *out << "<td style=\"background:#" << ((*mHardwareManager)[boardID].bTTCBC0Trigger?"00FF00":"FFFFFF")
	   << "\"></td>" << endl;

      *out << "</tr><tr><td>TTC RESYNC Trigger</td>" << endl;      
      *out << "<td style=\"background:#" << ((*mHardwareManager)[boardID].bTTCRESYNCTrigger?"00FF00":"FFFFFF")
	   << "\"></td>" << endl;

      *out << "</tr><tr><td>Transmitter FSM Trigger</td>" << endl;      
      *out << "<td style=\"background:#" << ((*mHardwareManager)[boardID].bFSMTrigger?"00FF00":"FFFFFF")
	   << "\"></td>" << endl;

      *out << "</tr><tr><td>Transmitter FSM Running</td>" << endl;      
      *out << "<td style=\"background:#" << ((*mHardwareManager)[boardID].bFSMRunning?"00FF00":"FFFFFF")
	   << "\"></td>" << endl;

      *out << "</tr><tr><td>RCT BC0 Phase Valid</td>" << endl;      
      *out << "<td style=\"background:#" << ((*mHardwareManager)[boardID].bPhaseRCTBC0Valid?"00FF00":"FF0000")
	   << "\"></td>" << endl;

      *out << "</tr><tr><td>RCT BC0 Phase Error</td>" << endl;      
      *out << "<td style=\"background:#" << ((*mHardwareManager)[boardID].bPhaseRCTBC0Error?"FF0000":"00FF00")
	   << "\"></td>" << endl;

      *out << "</tr><tr><td>RCT BC0 Phase Error (Latched)</td>" << endl;      
      *out << "<td style=\"background:#" << ((*mHardwareManager)[boardID].bPhaseRCTBC0ErrorLatched?"FF0000":"00FF00")
	   << "\"></td>" << endl;

      *out << "</tr><tr><td>TTC BC0 Phase Valid</td>" << endl;      
      *out << "<td style=\"background:#" << ((*mHardwareManager)[boardID].bPhaseTTCBC0Valid?"00FF00":"FF0000")
	   << "\"></td>" << endl;

      *out << "</tr><tr><td>TTC BC0 Phase Error</td>" << endl;      
      *out << "<td style=\"background:#" << ((*mHardwareManager)[boardID].bPhaseTTCBC0Error?"FF0000":"00FF00")
	   << "\"></td>" << endl;

      *out << "</tr><tr><td>TTC BC0 Phase Error (Latched)</td>" << endl;      
      *out << "<td style=\"background:#" << ((*mHardwareManager)[boardID].bPhaseTTCBC0ErrorLatched?"FF0000":"00FF00")
	   << "\"></td>" << endl;

      // Close the table
      *out << "</tr></table>" << std::endl;
      
      // Always put the page footer LAST
      xgi::Utils::getPageFooter(*out);    

    } catch(ICException &aExc) {
      XCEPT_RAISE(xgi::exception::Exception,aExc.what());
    } catch(xgi::exception::Exception &e) {
      string errMsg = "Exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RETHROW(xgi::exception::Exception, errMsg, e);
    } catch(...) {
      string errMsg = "Unknown exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RAISE(xgi::exception::Exception, errMsg);
    }
    
    }*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//void lumiHCALLUMIDAQ::webPeek(xgi::Input * in, xgi::Output * out) throw (hcal::exception::Exception) {}
//void lumiHCALLUMIDAQ::webCfg(xgi::Input * in, xgi::Output * out) throw (hcal::exception::Exception) {}

// Initialise (configure)
void lumiHCALLUMIDAQ::init() throw (hcal::exception::Exception) {
  LOG4CPLUS_DEBUG(getApplicationLogger(), toolbox::toString("lumiHCALLUMIDAQ::init()"));
  mStatusString = "INIT";

  // Pull the configuration document from the base class
  hcal::cfg::CfgDocument* cfg = getConfigurationDoc();

  // The context for global HLX parameters (some of which are shared by the DAQ
  hcal::cfg::Context hardware_cxt("HLX");
  hcal::cfg::Context daq_cxt("HCALLUMIDAQ");

  /////////////////
  // First we grab the DAQ configuration, some of which can override the HLX setup
  m_shortSectionLength = cfg->get("ShortSectionNumNibbles",daq_cxt,4);
  m_longSectionLength = cfg->get("LongSectionNumNibbles",daq_cxt,256);
  m_numHLXs = cfg->get("NumHLXs",daq_cxt,36);

  // Fake mode - overrides normal operation and settings
  m_fakeHLXMode = cfg->get("FakeHLXMode",daq_cxt,false);

  // Distributor config.
  m_useShortTCP = cfg->get("UseShortTCP",daq_cxt,false);
  m_useLongTCP = cfg->get("UseLongTCP",daq_cxt,false);
  m_useOracle = cfg->get("UseOracle",daq_cxt,false);
  m_useGIF = cfg->get("UseGIF",daq_cxt,false);
  m_useDIP = cfg->get("UseDIP",daq_cxt,false);
  m_useSectionList = cfg->get("UseSectionList",daq_cxt,false);

  // TCP settings
  m_tcpShortPort = cfg->get("ShortTCPPort",daq_cxt,51001);
  m_tcpLongPort = cfg->get("LongTCPPort",daq_cxt,51002);
  
  // Oracle settings
  m_oracleDBUserName = cfg->get("OracleDBUserName",daq_cxt,"*undefined*");
  m_oracleDBPassword = cfg->get("OracleDBPassword",daq_cxt,"*undefined*");
  m_oracleDBLocation = cfg->get("OracleDBLocation",daq_cxt,"*undefined*");
  m_oracleDBSchema = cfg->get("OracleDBSchema",daq_cxt,"*undefined*");

  //////////////////////
  // Now the HLX settings

  // If we're in fake HLX mode we ignore these
  m_interfaceAddress = "192.168.1.100";
  
  if ( m_fakeHLXMode ) {
    
    // Overwrite default settings to match fake loopback module
    m_interfacePort = 21308;
    m_numOrbits = 4096;
    m_numBunches = 3564;
    m_numHLXs = 36;
    m_debugData = true;
 
  } else {
 
    // Destination port (readout PC)
    m_interfacePort = cfg->get("DestPort",hardware_cxt,21306);
    // Number of orbits
    m_numOrbits = cfg->get("NumOrbits",hardware_cxt,4095);
    m_numOrbits++;
    // Orbit period
    m_numBunches = cfg->get("OrbitPeriod",hardware_cxt,3563);
    m_numBunches++;

    // Check for debug data
    m_debugData = cfg->get("DebugData",hardware_cxt,0);
    
    // Check to see if we're in broadcast or unicast mode
    if ( cfg->get("DestMacAddrHigh",hardware_cxt,0xFFFF) == 0xFFFF
	 && cfg->get("DestMacAddrMed",hardware_cxt,0xFFFF) == 0xFFFF
	 && cfg->get("DestMacAddrLow",hardware_cxt,0xFFFF) == 0xFFFF ) {
      m_interfaceAddress = "BROADCAST";
    }

  }
  
}

// Start DAQ
void lumiHCALLUMIDAQ::enable() throw (hcal::exception::Exception) {
  LOG4CPLUS_DEBUG(getApplicationLogger(), toolbox::toString("lumiHCALLUMIDAQ::enable()"));
  mStatusString = "ENABLED";

  // Start the main DAQ
  StartReadout(getRunNumber());

  // Conditionally start the fake data generator
  if ( m_fakeHLXMode ) {
    StartFakeHLX();
  }

}

void lumiHCALLUMIDAQ::StartFakeHLX() throw (xdaq::exception::Exception) {
  LOG4CPLUS_INFO(getApplicationLogger(), "In StartFakeHLX");
  
  // Make sure it's stopped first
  StopFakeHLX();

  mFakeHLX = new FakeHLX(m_numHLXs,
			 m_numBunches,
			 m_numOrbits,
			 m_shortSectionLength,
			 m_interfacePort,
			 true); // using resync every 60s on average
  if ( mFakeHLX == 0 ) {
    // Raise a XDAQ exception with the error that caused startup to fail
    std::string msg = "Cannot create fake HLX module";
    XCEPT_RAISE(xdaq::exception::Exception, msg);
  }

}

void lumiHCALLUMIDAQ::StopFakeHLX() throw (xdaq::exception::Exception) {
  LOG4CPLUS_INFO(getApplicationLogger(), "In StopFakeHLX");
  
  if ( mFakeHLX ) {
    delete mFakeHLX; mFakeHLX = 0;
  }

}

// Stop DAQ
void lumiHCALLUMIDAQ::disable() throw (hcal::exception::Exception) {
  LOG4CPLUS_DEBUG(getApplicationLogger(), toolbox::toString("lumiHCALLUMIDAQ::disable()"));
  mStatusString = "DISABLED";
  StopReadout();
  StopFakeHLX();
}


// not used by lumi DAQ (just ignore)
void lumiHCALLUMIDAQ::pause() throw (hcal::exception::Exception) {
  LOG4CPLUS_DEBUG(getApplicationLogger(), toolbox::toString("lumiHCALLUMIDAQ::pause()"));
  //mStatusString = "PAUSED";
}
// not used by lumi DAQ (just ignore)
void lumiHCALLUMIDAQ::resume() throw (hcal::exception::Exception) {
  LOG4CPLUS_DEBUG(getApplicationLogger(), toolbox::toString("lumiHCALLUMIDAQ::resume()"));
  //mStatusString = "RESUMED";
}

// Not used by lumi daq (programs HTR firmwares)
void lumiHCALLUMIDAQ::coldInit() throw (hcal::exception::Exception) {
  LOG4CPLUS_DEBUG(getApplicationLogger(), toolbox::toString("lumiHCALLUMIDAQ::coldInit()"));
  //mStatusString = "COLDINIT";
}

void lumiHCALLUMIDAQ::reset() throw (hcal::exception::Exception) {
  LOG4CPLUS_DEBUG(getApplicationLogger(), toolbox::toString("lumiHCALLUMIDAQ::reset()"));
  mStatusString = "RESET";
}

void lumiHCALLUMIDAQ::WebConfig(xgi::Input * in, xgi::Output * out) throw (hcal::exception::Exception) {
  try {

    mLastPage = "Default";
    
    // Construct the menus, FSM controls, etc...
    this->WebConstructPageHeader(out,"Configuration");     
    
    *out << "<div style=\"font-family:arial\">HLX interface port: " << m_interfacePort << "<br/>" << endl;
    *out << "<div style=\"font-family:arial\">Orbits per nibble: " << m_numOrbits << "<br/>" << endl;
    *out << "<div style=\"font-family:arial\">Bunches per orbit: " << m_numBunches << "<br/>" << endl;
    *out << "<div style=\"font-family:arial\">Number of HLXs: " << m_numHLXs << "<br/>" << endl;
    *out << "<div style=\"font-family:arial\">Hostname: " << m_hostName << "<br/>" << endl;
    *out << "<div style=\"font-family:arial\">Num nibbles per short section: " << m_shortSectionLength << "<br/>" << endl;
    *out << "<div style=\"font-family:arial\">Num nibbles per long section: " << m_longSectionLength << "<br/>" << endl;
    *out << "<div style=\"font-family:arial\">Fake HLX mode: " << m_fakeHLXMode << "<br/>" << endl;
    *out << "<div style=\"font-family:arial\">Use short TCP: " << m_useShortTCP << "<br/>" << endl;
    *out << "<div style=\"font-family:arial\">Use long TCP: " << m_useLongTCP << "<br/>" << endl;
    *out << "<div style=\"font-family:arial\">Use section list: " << m_useSectionList << "<br/>" << endl;
    *out << "<div style=\"font-family:arial\">Use oracle: " << m_useOracle << "<br/>" << endl;
    *out << "<div style=\"font-family:arial\">Use DIP: " << m_useDIP << "<br/>" << endl;
    *out << "<div style=\"font-family:arial\">Use GIF: " << m_useGIF << "<br/>" << endl;
    
    *out << "<div style=\"font-family:arial\">Short TCP port: " << m_tcpShortPort << "<br/>" << endl;
    *out << "<div style=\"font-family:arial\">Long TCP port: " << m_tcpLongPort << "<br/>" << endl;
    
    *out << "<div style=\"font-family:arial\">Oracle user name: " << m_oracleDBUserName << "<br/>" << endl;
    *out << "<div style=\"font-family:arial\">Oracle password: " << m_oracleDBPassword << "<br/>" << endl;
    *out << "<div style=\"font-family:arial\">Oracle location: " << m_oracleDBLocation << "<br/>" << endl;
    *out << "<div style=\"font-family:arial\">Oracle schema: " << m_oracleDBSchema << "<br/>" << endl;
    
    *out << "<div style=\"font-family:arial\">Interface address: " << m_interfaceAddress << "<br/>" << endl;
    
    // Always put the page footer LAST
    xgi::Utils::getPageFooter(*out);    
    
  } catch(xgi::exception::Exception &e) {
    string errMsg = "Exception caught in ";
    errMsg += __PRETTY_FUNCTION__;
    XCEPT_RETHROW(hcal::exception::Exception, errMsg, e);
  } catch(...) {
    string errMsg = "Unknown exception caught in ";
    errMsg += __PRETTY_FUNCTION__;
    XCEPT_RAISE(hcal::exception::Exception, errMsg);
  }
  
}

void lumiHCALLUMIDAQ::webMenu(xgi::Input * in, xgi::Output * out ) throw (hcal::exception::Exception) {    
  try {      
    mLastPage = "Default";
    
    // Construct the menus, FSM controls, etc...
    this->WebConstructPageHeader(out,"Status");     
    
    std::string link = "/";
    link += getApplicationDescriptor()->getURN().c_str();
    link += "/";
    link += mLastPage;
    *out << "<META HTTP-EQUIV=\"Refresh\" CONTENT=\"10; URL=" << link << "\">";
    
    if ( mSectionCollectorShort ) {
      *out << "<div style=\"font-family:arial\">Current run number (short): " << mSectionCollectorShort->GetCurrentRunNumber() << "<br/>" << endl;
    }
    
    if ( mSectionCollectorLong ) {
      *out << "<div style=\"font-family:arial\">Current run number (long): " << mSectionCollectorLong->GetCurrentRunNumber() << "<br/>" << endl;
    }
    
/*
      *out << "<div style=\"font-family:arial\">HLX interface port: " << m_interfacePort << "<br/>" << endl;
      *out << "<div style=\"font-family:arial\">Orbits per nibble: " << m_numOrbits << "<br/>" << endl;
      *out << "<div style=\"font-family:arial\">Bunches per orbit: " << m_numBunches << "<br/>" << endl;
      *out << "<div style=\"font-family:arial\">Number of HLXs: " << m_numHLXs << "<br/>" << endl;
      *out << "<div style=\"font-family:arial\">Hostname: " << m_hostName << "<br/>" << endl;
      *out << "<div style=\"font-family:arial\">Num nibbles per short section: " << m_shortSectionLength << "<br/>" << endl;
      *out << "<div style=\"font-family:arial\">Num nibbles per long section: " << m_longSectionLength << "<br/>" << endl;
      *out << "<div style=\"font-family:arial\">Fake HLX mode: " << m_fakeHLXMode << "<br/>" << endl;
      *out << "<div style=\"font-family:arial\">Use short TCP: " << m_useShortTCP << "<br/>" << endl;
      *out << "<div style=\"font-family:arial\">Use long TCP: " << m_useLongTCP << "<br/>" << endl;
      *out << "<div style=\"font-family:arial\">Use section list: " << m_useSectionList << "<br/>" << endl;
      *out << "<div style=\"font-family:arial\">Use oracle: " << m_useOracle << "<br/>" << endl;
      *out << "<div style=\"font-family:arial\">Use DIP: " << m_useDIP << "<br/>" << endl;
      *out << "<div style=\"font-family:arial\">Use GIF: " << m_useGIF << "<br/>" << endl;

      *out << "<div style=\"font-family:arial\">Short TCP port: " << m_tcpShortPort << "<br/>" << endl;
      *out << "<div style=\"font-family:arial\">Long TCP port: " << m_tcpLongPort << "<br/>" << endl;

      *out << "<div style=\"font-family:arial\">Oracle user name: " << m_oracleDBUserName << "<br/>" << endl;
      *out << "<div style=\"font-family:arial\">Oracle password: " << m_oracleDBPassword << "<br/>" << endl;
      *out << "<div style=\"font-family:arial\">Oracle location: " << m_oracleDBLocation << "<br/>" << endl;
      *out << "<div style=\"font-family:arial\">Oracle schema: " << m_oracleDBSchema << "<br/>" << endl;

      *out << "<div style=\"font-family:arial\">Interface address: " << m_interfaceAddress << "<br/>" << endl;
*/

      *out << "<table style=\"font-family:arial\" border=0 cellpadding=0 cellspacing=0 width=100%><tr><td style=\"font-size:20pt\" colspan=2>Global Information</td>" << endl;
      *out << "</tr><tr><td valign=top width=400><table border=0 cellpadding=0 cellspacing=0 width=100% height=100%>" << endl;

      if ( mNibbleCollector ) {
	*out << "<tr><td style=\"font-weight:bold\" colspan=2>Nibble Collector</td></tr>" << endl;
	*out << "<tr><td width=300>Packet count</td><td width=100>" << mNibbleCollector->GetNumGoodPackets() << "</td></tr>" << endl;
	*out << "<tr><td width=300>Bad packet count</td><td width=100>" << mNibbleCollector->GetNumBadPackets() << "</td></tr>" << endl;
	*out << "<tr><td width=300>Lost packet count</td><td width=100>" << mNibbleCollector->GetNumLostPackets() << "</td></tr>" << endl;
	*out << "<tr><td width=300>Total data volume</td><td width=100>" << mNibbleCollector->GetTotalDataVolume() << "</td></tr>" << endl;
	*out << "<tr><td width=300>Good ET sum nibble count</td><td width=100>" << mNibbleCollector->GetNumGoodETSumNibbles() << "</td></tr>" << endl;
	*out << "<tr><td width=300>Bad ET sum nibble count</td><td width=100>" << mNibbleCollector->GetNumBadETSumNibbles() << "</td></tr>" << endl;
	*out << "<tr><td width=300>Good LHC nibble count</td><td width=100>" << mNibbleCollector->GetNumGoodLHCNibbles() << "</td></tr>" << endl;
	*out << "<tr><td width=300>Bad LHC nibble count</td><td width=100>" << mNibbleCollector->GetNumBadLHCNibbles() << "</td></tr>" << endl;
	*out << "<tr><td width=300>Good occupancy nibble count</td><td width=100>" << mNibbleCollector->GetNumGoodOccupancyNibbles() << "</td></tr>" << endl;
	*out << "<tr><td width=300>Bad occupancy nibble count</td><td width=100>" << mNibbleCollector->GetNumBadOccupancyNibbles() << "</td></tr>" << endl;
	*out << "<tr><td width=300>Last error</td><td width=100>" << mNibbleCollector->GetLastError() << "</td></tr>" << endl;
      }

      if ( mTestSectionCollector ) {
	*out << "<tr><td style=\"font-weight:bold\" colspan=2>Test Section Collector</td></tr>" << endl;
	*out << "<tr><td width=300>Good nibble count</td><td width=100>" << mTestSectionCollector->GetNumGoodNibbles() << "</td></tr>" << endl;
	*out << "<tr><td width=300>Bad nibble count</td><td width=100>" << mTestSectionCollector->GetNumBadNibbles() << "</td></tr>" << endl;
      }

      if ( mShortTestDistributor ) {
	*out << "<tr><td style=\"font-weight:bold\" colspan=2>Short section test distributor</td></tr>" << endl;
	*out << "<tr><td width=300>Good section count</td><td width=100>" << mShortTestDistributor->GetNumGoodSections() << "</td></tr>" << endl;
	*out << "<tr><td width=300>Bad section count</td><td width=100>" << mShortTestDistributor->GetNumBadSections() << "</td></tr>" << endl;
      }

      if ( mLongTestDistributor ) {
	*out << "<tr><td style=\"font-weight:bold\" colspan=2>Long section test distributor</td></tr>" << endl;
	*out << "<tr><td width=300>Good section count</td><td width=100>" << mLongTestDistributor->GetNumGoodSections() << "</td></tr>" << endl;
	*out << "<tr><td width=300>Bad section count</td><td width=100>" << mLongTestDistributor->GetNumBadSections() << "</td></tr>" << endl;
      }

      if ( mSectionCollectorShort ) {
	*out << "<tr><td style=\"font-weight:bold\" colspan=2>Short Section Collector ("
	     << m_shortSectionLength << " nibbles)</td></tr>" << endl;
	*out << "<tr><td width=300>Complete section count</td><td width=100>" << mSectionCollectorShort->GetNumCompleteLumiSections() << "</td></tr>" << endl;
        *out << "<tr><td width=300>Incomplete section count</td><td width=100>" << mSectionCollectorShort->GetNumIncompleteLumiSections() << "</td></tr>" << endl;
        *out << "<tr><td width=300>Resync count</td><td width=100>" << mSectionCollectorShort->GetResyncCount() << "</td></tr>" << endl;
        *out << "<tr><td width=300>Last error</td><td width=100>" << mSectionCollectorShort->GetLastError() << "</td></tr>" << endl;
      }
      
      if ( mSectionCollectorLong ) {
	*out << "<tr><td style=\"font-weight:bold\" colspan=2>Long Section Collector ("
	     << m_longSectionLength << " nibbles)</td></tr>" << endl;
	*out << "<tr><td width=300>Complete section count</td><td width=100>" << mSectionCollectorLong->GetNumCompleteLumiSections() << "</td></tr>" << endl;
        *out << "<tr><td width=300>Incomplete section count</td><td width=100>" << mSectionCollectorLong->GetNumIncompleteLumiSections() << "</td></tr>" << endl;
        *out << "<tr><td width=300>Resync count</td><td width=100>" << mSectionCollectorLong->GetResyncCount() << "</td></tr>" << endl;
	*out << "<tr><td width=300>Last error</td><td width=100>" << mSectionCollectorLong->GetLastError() << "</td></tr>" << endl;
      }
      
      if ( mDIPDistributor ) {
	*out << "<tr><td style=\"font-weight:bold\" colspan=2>DIP Distributor</td></tr>" << endl;
	*out << "<tr><td width=300>Error count</td><td width=100>" << mDIPDistributor->GetErrorCount() << "</td></tr>" << endl;
	*out << "<tr><td width=300>Last error</td><td width=100>" << mDIPDistributor->GetLastError() << "</td></tr>" << endl;
      }

      if ( mShortSectionListDistributor ) {
	*out << "<tr><td style=\"font-weight:bold\" colspan=2>Short Section List Distributor</td></tr>" << endl;
	*out << "<tr><td width=300>Error count</td><td width=100>" << mShortSectionListDistributor->GetErrorCount() << "</td></tr>" << endl;
	*out << "<tr><td width=300>Last error</td><td width=100>" << mShortSectionListDistributor->GetLastError() << "</td></tr>" << endl;
      }

      if ( mLongSectionListDistributor ) {
	*out << "<tr><td style=\"font-weight:bold\" colspan=2>Long Section List Distributor</td></tr>" << endl;
	*out << "<tr><td width=300>Error count</td><td width=100>" << mLongSectionListDistributor->GetErrorCount() << "</td></tr>" << endl;
	*out << "<tr><td width=300>Last error</td><td width=100>" << mLongSectionListDistributor->GetLastError() << "</td></tr>" << endl;
      }

      if ( mShortSectionTCPDistributor ) {
	*out << "<tr><td style=\"font-weight:bold\" colspan=2>Short Section TCP Distributor</td></tr>" << endl;
	*out << "<tr><td width=300>Error count</td><td width=100>" << mShortSectionTCPDistributor->GetErrorCount() << "</td></tr>" << endl;
	*out << "<tr><td width=300>Last error</td><td width=100>" << mShortSectionTCPDistributor->GetLastError() << "</td></tr>" << endl;
      }

      if ( mLongSectionTCPDistributor ) {
	*out << "<tr><td style=\"font-weight:bold\" colspan=2>Long Section TCP Distributor</td></tr>" << endl;
	*out << "<tr><td width=300>Error count</td><td width=100>" << mLongSectionTCPDistributor->GetErrorCount() << "</td></tr>" << endl;
	*out << "<tr><td width=300>Last error</td><td width=100>" << mLongSectionTCPDistributor->GetLastError() << "</td></tr>" << endl;
      }

      if ( mGIFDistributor ) {
	*out << "<tr><td style=\"font-weight:bold\" colspan=2>GIF Distributor</td></tr>" << endl;
	*out << "<tr><td width=300>Error count</td><td width=100>" << mGIFDistributor->GetErrorCount() << "</td></tr>" << endl;
	*out << "<tr><td width=300>Last error</td><td width=100>" << mGIFDistributor->GetLastError() << "</td></tr>" << endl;
      }

      if ( mOracleDistributor ) {
	*out << "<tr><td style=\"font-weight:bold\" colspan=2>Oracle Distributor</td></tr>" << endl;
	*out << "<tr><td width=300>Error count</td><td width=100>" << mOracleDistributor->GetErrorCount() << "</td></tr>" << endl;
	*out << "<tr><td width=300>Last error</td><td width=100>" << mOracleDistributor->GetLastError() << "</td></tr>" << endl;
      }

      *out << "</table></td><td valign=top align=center width=*>" << endl;
      *out << "<img src=\"http://cmsmon.cern.ch/lumi/pic/et.png\"/></td>" << endl;
      *out << "</tr></table>" << endl;

      *out << "</div><br/>" << endl;
            /*
      u16 temp16[3];
      mHLX->GetFirmwareVersion(temp16[0]);
      *out << "Firmware version: " << temp16[0] << "<br/>" << endl;
      mHLX->GetHLXID(temp16[0]);
      *out << "Board ID: " << temp16[0] << "<br/>" << endl;
      mHLX->GetSourceMAC(temp16[0],temp16[1],temp16[2]);
      *out << "Source MAC: 0x" << hex << temp16[0] << temp16[1] << temp16[2] << "<br/>" << endl;
      mHLX->GetDestinationMAC(temp16[0],temp16[1],temp16[2]);
      *out << "Destination MAC: 0x" << hex << temp16[0] << temp16[1] << temp16[2] << "<br/>" << endl;
      */
      /*
      u16 temps[6];
      mHardwareManager->GetFirmwareBuildDate(temps[0],temps[1],temps[2],temps[3],temps[4],temps[5]);
      //out->setfill('0');
      *out << "Firmware built on "
	   << setfill('0')
	   << setw(2) << temps[2] << "/"
	   << setw(2) << temps[1] << "/"
	   << setw(2) << temps[0] << " at "
	   << setw(2) << temps[3] << ":"
	   << setw(2) << temps[4] << ":"
	   << setw(2) << temps[5];
      i32 firmwareOffset = mHardwareManager->VerifyFirmwareMatch();
      if ( firmwareOffset == 0 ) {
	*out << " (VALID)" << endl;
      } else if ( firmwareOffset > 0 ) {
	*out << " (NEWER THAN BITFILE!)";
      } else {
	*out << " (OLDER THAN BITFILE!)";
      }
      *out << "</div><br/>" << endl;
      */

      // Contruct the global status table
      /*
      *out << "<table style=\"font-family:arial\" border=1 cellpadding=0 cellspacing=0 width=100%><tr><td style=\"font-size:20pt\" colspan=2>Global Information</td>" << endl;
      bool bValidReg; u16 tempReg;
      bValidReg = mHardwareManager->GetRCTBC0Phase(tempReg);
      *out << "</tr><tr><td>RCT BC0 Phase Offset</td><td style=\"font-weight:bold;color:#"
	   << (bValidReg?"00aa00":"ff0000")  << "\" width=100>" << tempReg << "</td>" << endl;

      bValidReg = mHardwareManager->GetTTCBC0Phase(tempReg);      
      *out << "</tr><tr><td>TTC BC0 Phase Offset</td><td style=\"font-weight:bold;color:#"
	   << (bValidReg?"00aa00":"ff0000")  << "\" width=100>" << tempReg << "</td>" << endl;

      bValidReg = mHardwareManager->GetRCTBC0Period(tempReg);      
      *out << "</tr><tr><td>RCT BC0 Period</td><td style=\"font-weight:bold;color:#"
	   << (bValidReg?"00aa00":"ff0000")  << "\" width=100>" << tempReg << "</td>" << endl;

      bValidReg = mHardwareManager->GetTTCBC0Period(tempReg);
      *out << "</tr><tr><td>TTC BC0 Period</td><td style=\"font-weight:bold;color:#"
	   << (bValidReg?"00aa00":"ff0000")  << "\" width=100>" << tempReg << "</td>" << endl;

      bValidReg = mHardwareManager->GetTTCResyncPeriod(tempReg);
      *out << "</tr><tr><td>TTC RESYNC Period</td><td style=\"font-weight:bold;color:#"
	   << (bValidReg?"00aa00":"ff0000")  << "\" width=100>" << tempReg << "</td>" << endl;

      *out << "</tr></table>" << "<br/>" << std::endl;

      // Contruct the board information table
      *out << "<table style=\"font-family:arial\" border=1 cellpadding=0 cellspacing=0 width=100%><tr><td></td>" << endl;      
      for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	*out << "<td width=10>" << j+1 << "</td>" << endl;
      }     
      *out << "</tr><tr><td style=\"font-size:20pt\" colspan=\"" << mHardwareManager->GetNumBoards()+1 << "\">Board Status</td>" << endl;

      // Actual temperature
      *out << "</tr><tr><td>Power temperature</td>" << endl;      
      for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	*out << "<td style=\"background:#ffffff\">" << static_cast<u16>((*mHardwareManager)[j].mPowerTemperature) << "</td>" << endl;
      }

      *out << "</tr><tr><td>ECL temperature</td>" << endl;      
      for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	*out << "<td style=\"background:#ffffff\">" << static_cast<u16>((*mHardwareManager)[j].mECLTemperature) << "</td>" << endl;
      }

      *out << "</tr><tr><td>Optical link temperature</td>" << endl;      
      for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	*out << "<td style=\"background:#ffffff\">" << static_cast<u16>((*mHardwareManager)[j].mOptoTemperature) << "</td>" << endl;
      }

      *out << "</tr><tr><td>Board temperature</td>" << endl;      
      for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	*out << "<td style=\"background:#ffffff\">" << static_cast<u16>((*mHardwareManager)[j].mBoardTemperature) << "</td>" << endl;
      }

      *out << "</tr><tr><td style=\"font-size:20pt\" colspan=\"" << mHardwareManager->GetNumBoards()+1 << "\">Board Controls</td>" << endl;      

      // Optical module control
      for ( u32 i = 0 ; i != 4 ; i++ ) {
	*out << "</tr><tr><td>Opto Tx " << i+1 << "</td>" << endl;      
	for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	  *out << "<td style=\"background:#" <<
	    ((*mHardwareManager)[j].bOptoTxEn[i]?((*mHardwareManager)[j].bOptoTxFault[i]?"FF0000":"00FF00"):"FFAA00")
	       << "\"></td>" << endl;
	}
      }

      // Serdes control
      for ( u32 i = 0 ; i != 4 ; i++ ) {
	*out << "</tr><tr><td>Transmitter FSM " << i+1 << "</td>" << endl;      
	for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	  *out << "<td style=\"background:#" <<
	    ((*mHardwareManager)[j].bTransmitterEn[i]?"00FF00":"FFAA00")
	       << "\"></td>" << endl;
	}
      }

      // Clock system
      *out << "</tr><tr><td>Serdes DCM</td>" << endl;      
      for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	*out << "<td style=\"background:#" <<
	  ((*mHardwareManager)[j].bSerdesDCMEnabled?((*mHardwareManager)[j].bSerdesDCMLocked?"00FF00":"FF0000"):"FFAA00")
	     << "\"></td>" << endl;
      }

      *out << "</tr><tr><td>TTC DCM</td>" << endl;      
      for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	*out << "<td style=\"background:#" <<
	  ((*mHardwareManager)[j].bTTCDCMEnabled?((*mHardwareManager)[j].bTTCDCMLocked?"00FF00":"FF0000"):"FFAA00")
	     << "\"></td>" << endl;
      }

      *out << "</tr><tr><td>TTCrx enabled</td>" << endl;      
      for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	*out << "<td style=\"background:#" << ((*mHardwareManager)[j].bTTCrxEnabled?"00FF00":"FFAA00")
	     << "\"></td>" << endl;
      }

      *out << "</tr><tr><td>QPLL</td>" << endl;      
      for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	*out << "<td style=\"background:#" <<
	  ((*mHardwareManager)[j].bQPLLEnabled?(((*mHardwareManager)[j].bQPLLLocked && !(*mHardwareManager)[j].bQPLLError)?"00FF00":"FF0000"):"FFAA00")
	     << "\"></td>" << endl;
      }

      *out << "</tr><tr><td style=\"font-size:20pt\" colspan=\"" << mHardwareManager->GetNumBoards()+1 << "\">Transmitter Status</td>" << endl;      
      
      *out << "</tr><tr><td>RCT BC0 Period Stability</td>" << endl;      
      for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	*out << "<td style=\"background:#" << ((*mHardwareManager)[j].bRCTPeriodChange?"FF0000":"00FF00")
	     << "\"></td>" << endl;
      } 

      *out << "</tr><tr><td>TTC BC0 Period Stability</td>" << endl;      
      for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	*out << "<td style=\"background:#" << ((*mHardwareManager)[j].bTTCBC0PeriodChange?"FF0000":"00FF00")
	     << "\"></td>" << endl;
      } 

      *out << "</tr><tr><td>TTC RESYNC Period Stability</td>" << endl;      
      for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	*out << "<td style=\"background:#" << ((*mHardwareManager)[j].bTTCRESYNCPeriodChange?"FF0000":"00FF00")
	     << "\"></td>" << endl;
      } 

      *out << "</tr><tr><td>RCT BC0 Trigger</td>" << endl;      
      for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	*out << "<td style=\"background:#" << ((*mHardwareManager)[j].bRCTTrigger?"00FF00":"FFFFFF")
	     << "\"></td>" << endl;
      } 

      *out << "</tr><tr><td>TTC BC0 Trigger</td>" << endl;      
      for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	*out << "<td style=\"background:#" << ((*mHardwareManager)[j].bTTCBC0Trigger?"00FF00":"FFFFFF")
	     << "\"></td>" << endl;
      } 

      *out << "</tr><tr><td>TTC RESYNC Trigger</td>" << endl;      
      for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	*out << "<td style=\"background:#" << ((*mHardwareManager)[j].bTTCRESYNCTrigger?"00FF00":"FFFFFF")
	     << "\"></td>" << endl;
      } 

      *out << "</tr><tr><td>Transmitter FSM Trigger</td>" << endl;      
      for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	*out << "<td style=\"background:#" << ((*mHardwareManager)[j].bFSMTrigger?"00FF00":"FFFFFF")
	     << "\"></td>" << endl;
      }

      *out << "</tr><tr><td>Transmitter FSM Running</td>" << endl;      
      for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	*out << "<td style=\"background:#" << ((*mHardwareManager)[j].bFSMRunning?"00FF00":"FFFFFF")
	     << "\"></td>" << endl;
      } 

      *out << "</tr><tr><td>RCT BC0 Phase Valid</td>" << endl;      
      for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	*out << "<td style=\"background:#" << ((*mHardwareManager)[j].bPhaseRCTBC0Valid?"00FF00":"FF0000")
	     << "\"></td>" << endl;
      }

      *out << "</tr><tr><td>RCT BC0 Phase Error</td>" << endl;      
      for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	*out << "<td style=\"background:#" << ((*mHardwareManager)[j].bPhaseRCTBC0Error?"FF0000":"00FF00")
	     << "\"></td>" << endl;
      } 

      *out << "</tr><tr><td>RCT BC0 Phase Error (Latched)</td>" << endl;      
      for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	*out << "<td style=\"background:#" << ((*mHardwareManager)[j].bPhaseRCTBC0ErrorLatched?"FF0000":"00FF00")
	     << "\"></td>" << endl;
      } 

      *out << "</tr><tr><td>TTC BC0 Phase Valid</td>" << endl;      
      for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	*out << "<td style=\"background:#" << ((*mHardwareManager)[j].bPhaseTTCBC0Valid?"00FF00":"FF0000")
	     << "\"></td>" << endl;
      } 

      *out << "</tr><tr><td>TTC BC0 Phase Error</td>" << endl;      
      for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	*out << "<td style=\"background:#" << ((*mHardwareManager)[j].bPhaseTTCBC0Error?"FF0000":"00FF00")
	     << "\"></td>" << endl;
      } 

      *out << "</tr><tr><td>TTC BC0 Phase Error (Latched)</td>" << endl;      
      for (u32 j=0 ; j != mHardwareManager->GetNumBoards() ; j++ ) {
	*out << "<td style=\"background:#" << ((*mHardwareManager)[j].bPhaseTTCBC0ErrorLatched?"FF0000":"00FF00")
	     << "\"></td>" << endl;
      } 

      // Close the table
      *out << "</tr></table>" << std::endl;*/
      
      // Always put the page footer LAST
      xgi::Utils::getPageFooter(*out);    

    } catch(ICException &aExc) {
      XCEPT_RAISE(xgi::exception::Exception,aExc.what());
    } catch(xgi::exception::Exception &e) {
      string errMsg = "Exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RETHROW(hcal::exception::Exception, errMsg, e);
    } catch(...) {
      string errMsg = "Unknown exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RAISE(hcal::exception::Exception, errMsg);
    }
    
  }

 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
xoap::MessageReference lumiHCALLUMIDAQ::ChangeState (xoap::MessageReference msg) throw (xoap::exception::Exception) {
  try {
    xoap::SOAPPart part = msg->getSOAPPart();
    xoap::SOAPEnvelope env = part.getEnvelope();
    xoap::SOAPBody body = env.getBody();
    DOMNode* node = body.getDOMNode();
    DOMNodeList* bodyList = node->getChildNodes();
    string StateReply = currentState_;

    for (unsigned int i = 0; i < bodyList->getLength(); i++) {
      DOMNode* command = bodyList->item(i);
    
      if (command->getNodeType() == DOMNode::ELEMENT_NODE) { 
	std::string commandName = xoap::XMLCh2String (command->getLocalName());

	//cout << "INFO: change state: "<<commandName<<endl;

	try {
	  toolbox::Event::Reference event(new toolbox::Event(commandName, this));
	  fsm_.fireEvent(event);
	  currentState_ = fsm_.getStateName(fsm_.getCurrentState());
	} catch (toolbox::fsm::exception::Exception & e) {
	  XCEPT_RETHROW(xoap::exception::Exception, "invalid command", e);
	}

      xoap::MessageReference reply = xoap::createMessage();
      xoap::SOAPEnvelope envelope = reply->getSOAPPart().getEnvelope();
      xoap::SOAPBody replybody = envelope.getBody();
      xoap::SOAPName responseName = envelope.createName( commandName +"Response", "xdaq", XDAQ_NS_URI);
      xoap::SOAPBodyElement responseElement = replybody.addBodyElement(responseName); 
      xoap::SOAPName stateName = envelope.createName("state", "xdaq", XDAQ_NS_URI); 
      xoap::SOAPElement stateElement = responseElement.addChildElement(stateName);
      xoap::SOAPName attributeName = envelope.createName("stateName", "xdaq", XDAQ_NS_URI);
      StateReply = currentState_;
      stateElement.addAttribute(attributeName, StateReply);

      return reply;
      }
    }

    xoap::MessageReference reply = xoap::createMessage();   //null reply
    return reply;                                           //null reply

  } catch(ICException &aExc) {
      XCEPT_RAISE(xgi::exception::Exception,aExc.what());
  } catch(...) {
    XCEPT_RAISE(xdaq::exception::Exception, "Unknown exception caught in changeState");
  }
}
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
  // Trigger FSM transitions from state to state within the software
  void lumiHCALLUMIDAQ::TriggerFSMTransition(conststd::string &in) throw (xdaq::exception::Exception) {
    try {
      toolbox::Event::Reference e(new toolbox::Event(in,this));
      fsm_.fireEvent(e);
    } catch (toolbox::fsm::exception::Exception &e) {
      string errMsg = "Exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RETHROW(xdaq::exception::Exception, errMsg, e);
    } catch(ICException &aExc) {
      XCEPT_RAISE(xgi::exception::Exception,aExc.what());
    } catch(xgi::exception::Exception &e) {
      string errMsg = "Exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RETHROW(xgi::exception::Exception, errMsg, e);
    } catch(...) {
      string errMsg = "Unknown exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RAISE(xgi::exception::Exception, errMsg);
    }
  }
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
xoap::MessageReference lumiHCALLUMIDAQ::HLXControl(xoap::MessageReference msg) throw (xoap::exception::Exception) {
  try {
    xoap::SOAPPart soap = msg->getSOAPPart();
    xoap::SOAPEnvelope envelope = soap.getEnvelope();
    xoap::SOAPBody body = envelope.getBody();

    std::vector<xoap::SOAPElement> elems = body.getChildElements();
    string response[100][3];
    string setting, val;
    int imax=0;

    for (size_t i = 0; i < elems.size(); ++i) {      
      xoap::SOAPName cmdname = elems[i].getElementName();
      setting = cmdname.getLocalName();
      xoap::SOAPName valuename("Value","xdaq",XDAQ_NS_URI);
      string temp = elems[i].getAttributeValue(valuename);
      val = (temp.c_str());
      //cout << "INFO: SOAP command = " << setting << ", param = " << val << endl;

      
      // TODO: add this
      //this->SetHLXParameter(setting, val, 0, false);


     response[i][2]="SetGlobalParameter called";
      response[i][0]=setting;
      response[i][1]=val;
      imax++;
    }

    xoap::MessageReference reply = xoap::createMessage();
    xoap::SOAPEnvelope renvelope = reply->getSOAPPart().getEnvelope();
    xoap::SOAPBody replybody = renvelope.getBody();

    for(int i=0; i<imax; i++){
      xoap::SOAPName responseName = renvelope.createName( response[i][0] +"Response", "xdaq", XDAQ_NS_URI);
      xoap::SOAPBodyElement responseElement = replybody.addBodyElement(responseName); 
      xoap::SOAPName stateName = renvelope.createName("Result", "xdaq", XDAQ_NS_URI); 
      xoap::SOAPElement stateElement = responseElement.addChildElement(stateName);
      xoap::SOAPName attributeName = renvelope.createName("Value", "xdaq", XDAQ_NS_URI);
      stateElement.addAttribute(attributeName, response[i][2]);
    }

     return reply;
  
  } catch(ICException::exception &e) {
    XCEPT_RAISE(xdaq::exception::Exception, e.what()); 
  } catch(...) {
    XCEPT_RAISE(xdaq::exception::Exception, "Unknown exception caught in ApveControl");
  }
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  void lumiHCALLUMIDAQ::actionPerformed(xdata::Event &e) throw (xoap::exception::Exception) {
    try {

     if (e.type() == "ItemChangedEvent") {  

       std::string item = dynamic_cast<xdata::ItemChangedEvent&>(e).itemName();
       if ( item == "configFile") {
	 LOG4CPLUS_DEBUG(getApplicationLogger(), toolbox::toString("lumiHCALLUMIDAQ::actionPerformed(): (change) item=\"%s\" : %s",item.c_str(), mConfigFile_.toString().c_str()));
	 std::string tempStr = mConfigFile_;
	 //mHLX->SetConfig(tempStr.c_str());
       }

     }
    
    } catch(ICException & aExc) {
      string errMsg = "ICException caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RAISE(xoap::exception::Exception,errMsg);
    } catch(xdaq::exception::Exception &e) {
      string errMsg = "Exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RETHROW(xoap::exception::Exception, errMsg, e);
    } catch(...) {
      string errMsg = "Unknown exception caught in ";
      errMsg += __PRETTY_FUNCTION__;
      XCEPT_RAISE(xoap::exception::Exception, errMsg);
    }
  }
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//}  //Namespace = HCAL_HLX
