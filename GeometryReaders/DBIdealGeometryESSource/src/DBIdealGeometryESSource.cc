#include "GeometryReaders/DBIdealGeometryESSource/interface/DBIdealGeometryESSource.h"

#include "DetectorDescription/Base/interface/DDdebug.h"
#include "DetectorDescription/Base/interface/DDException.h"
// #include "DetectorDescription/Parser/interface/DDLParser.h"
#include "DetectorDescription/DBReader/interface/DDORAReader.h"
// #include "DetectorDescription/Core/interface/DDCompactView.h"
// #include "DetectorDescription/Core/interface/DDSpecifics.h"
// #include "DetectorDescription/Parser/interface/DDLConfiguration.h"
// #include "DetectorDescription/Algorithm/src/AlgoInit.h"
#include "CondCore/DBCommon/interface/ServiceLoader.h"
//#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondCore/MetaDataService/interface/MetaData.h"

#include <memory>

using cond::MetaData;

DBIdealGeometryESSource::DBIdealGeometryESSource(const edm::ParameterSet & pset) 
{
  std::string startNode = pset.getParameter<std::string>("startNode") == ""? std::string("cms:OCMS"):pset.getParameter<std::string>("startNode");
  std::string dbConn = pset.getParameter<std::string>("dbConn");
  std::string dbID = pset.getParameter<std::string>("dbID");
  std::string dbUser = pset.getParameter<std::string>("dbUser");
  std::string dbPass = pset.getParameter<std::string>("dbPass");
 
  cond::ServiceLoader* loader=new cond::ServiceLoader;
  dbUser="CORAL_AUTH_USER="+dbUser;
  dbPass="CORAL_AUTH_PASSWORD="+dbPass;
  ::putenv(dbUser.c_str());
  ::putenv(dbPass.c_str());
  loader->loadAuthenticationService( cond::Env );
  loader->loadMessageService( cond::Error );

std::string aToken;
 try {
  DDORAReader ddorar( startNode, 
              dbID,
		      aToken,
		      pset.getParameter<std::string>("dbUser"), 
		      pset.getParameter<std::string>("dbPass"),
		      dbConn ); 
  if ( ddorar.readDB() ) {
  
//     // MEC: FIX! Specs should NOT come from XML?
//     DDLParser * parser = DDLParser::instance();
//     //   DDLParser::setInstance( new DDLParser );
//     //   DDLParser* parser = DDLParser::instance();     
//     DDLConfiguration dp;
//     int result1 = dp.readConfig( pset.getParameter<std::string>("specsConf") );
//     if ( result1 != 0 ) throw DDException ("DDLConfiguration: readConfig failed!");
//     result1 = parser->parse( dp );
//     if ( result1 != 0 ) throw DDException ("DetectorDescription: Parsing failed!");
  } else {
    std::cout << "WARNING: DBIdealGeometryESSource could not make a geometry.  Should this throw?" << std::endl;
  }
 } catch ( std::exception& ser ) {
   std::cout << ser.what() << std::endl;
 }
  //Tell Producer what we produce
  setWhatProduced(this);
  //Tell Finder what records we find
  findingRecord<IdealGeometryRecord>();
}

DBIdealGeometryESSource::~DBIdealGeometryESSource() {}

const DDCompactView *
DBIdealGeometryESSource::produce(const IdealGeometryRecord &)
{ return new DDCompactView(); }

void DBIdealGeometryESSource::setIntervalFor(const edm::eventsetup::EventSetupRecordKey & ,
					     const edm::IOVSyncValue & iosv,
					     edm::ValidityInterval & oValidity)
{
  // mec: not sure of this next line, to force IOVSyncValue to use
  edm::ValidityInterval infinity(iosv.beginOfTime(), iosv.endOfTime());
  oValidity = infinity;
}


#include "FWCore/Framework/interface/SourceFactory.h"


DEFINE_FWK_EVENTSETUP_SOURCE(DBIdealGeometryESSource)


