#include "CondCore/DBCommon/interface/ConnectionHandler.h"
#include "CondCore/DBCommon/interface/CoralTransaction.h"
#include "CondCore/DBCommon/interface/PoolTransaction.h"
#include "CondCore/DBCommon/interface/AuthenticationMethod.h"
#include "CondCore/DBCommon/interface/Connection.h"
#include "CondCore/DBCommon/interface/SessionConfiguration.h"
#include "CondCore/DBCommon/interface/FipProtocolParser.h"
#include "CondCore/DBCommon/interface/MessageLevel.h"
#include "CondCore/DBCommon/interface/DBSession.h"
#include "CondCore/DBCommon/interface/Exception.h"
#include "CondCore/MetaDataService/interface/MetaData.h"

#include "CondCore/IOVService/interface/IOVService.h"
#include "CondCore/IOVService/interface/IOVEditor.h"
#include "CondCore/IOVService/interface/IOVProxy.h"


#include "SealBase/SharedLibrary.h"
#include "SealBase/SharedLibraryError.h"


#include "CondCore/DBCommon/interface/Logger.h"
#include "CondCore/DBCommon/interface/LogDBEntry.h"
#include "CondCore/DBCommon/interface/UserLogInfo.h"
#include "CondCore/DBCommon/interface/TagInfo.h"


#include "CondCore/DBCommon/interface/ObjectRelationalMappingUtility.h"
#include "CondCore/IOVService/interface/IOVNames.h"

#include "FWCore/PluginManager/interface/PluginManager.h"
#include "FWCore/PluginManager/interface/standard.h"

#include <boost/program_options.hpp>
#include <iterator>
#include <limits>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstdlib>
#include<sstream>


int main( int argc, char** argv ){
  
  boost::program_options::options_description desc("options");
  boost::program_options::options_description visible("Usage: cmscond_duplicate_iov [options] \n");
  visible.add_options()
    ("connection,c",boost::program_options::value<std::string>(),"connection string(required)")
    ("dictionary,D",boost::program_options::value<std::string>(),"data dictionary(required if no plugin available)")
    ("tag,t",boost::program_options::value<std::string>(),"tag (required)")
    ("fromTime,f",boost::program_options::value<cond::Time_t>(),"a valid time of payload to append (required)")
    ("sinceTime,",boost::program_options::value<cond::Time_t>(),"since time of new iov(required)")
    ("authPath,P",boost::program_options::value<std::string>(),"path to authentication xml(default .)")
    ("configFile,f",boost::program_options::value<std::string>(),"configuration file(optional)")
    ("blobStreamer,B",boost::program_options::value<std::string>(),"BlobStreamerName(default to COND/Services/TBufferBlobStreamingService)")
    ("logDB,l",boost::program_options::value<std::string>(),"logDB(optional")
    ("debug","switch on debug mode")
    ("help,h", "help message")
    ;
  desc.add(visible);
  std::string destConnect;
  std::string dictionary;
  std::string destTag;
  std::string logConnect;

  cond::Time_t from = std::numeric_limits<cond::Time_t>::max();
  cond::Time_t since = std::numeric_limits<cond::Time_t>::max();

  std::string authPath(".");
  std::string configuration_filename;
  bool debug=false;
  std::string blobStreamerName("COND/Services/TBufferBlobStreamingService");
  boost::program_options::variables_map vm;
  try{
    boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(desc).run(), vm);
    if (vm.count("help")) {
      std::cout << visible <<std::endl;;
      return 0;
    }
    if( vm.count("configFile") ){
      configuration_filename=vm["configFile"].as<std::string>();
      if (! configuration_filename.empty()){
	std::fstream configuration_file;
	configuration_file.open(configuration_filename.c_str(), std::fstream::in);
	boost::program_options::store(boost::program_options::parse_config_file(configuration_file,desc), vm);
	configuration_file.close();
      }
    }
    if(!vm.count("connection")){
      std::cerr <<"[Error] no connection[c] option given \n";
      std::cerr<<" please do "<<argv[0]<<" --help \n";
      return 1;
    }else{
      destConnect=vm["connection"].as<std::string>();
    }
    if(vm.count("dictionary")){
      dictionary=vm["dictionary"].as<std::string>();
    }
    if(!vm.count("tag")){
      std::cerr <<"[Error] no tag[t] option given \n";
      std::cerr<<" please do "<<argv[0]<<" --help \n";
      return 1;
    }else
      destTag = vm["tag"].as<std::string>();

    if(!vm.count("fromTime")){
      std::cerr <<"[Error] no fromTime[f] option given \n";
      std::cerr<<" please do "<<argv[0]<<" --help \n";
      return 1;
    }else
      from = vm["fromTime"].as<cond::Time_t>();
 
    if(!vm.count("sinceTime")){
      std::cerr <<"[Error] no sinceTime[f] option given \n";
      std::cerr<<" please do "<<argv[0]<<" --help \n";
      return 1;
    }else
      since = vm["sinceTime"].as<cond::Time_t>();
    
   if(vm.count("logDB"))
      logConnect = vm["logDB"].as<std::string>();



    if( vm.count("authPath") ){
      authPath=vm["authPath"].as<std::string>();
    }
    if(vm.count("debug")){
      debug=true;
    }
    if(vm.count("BlobStreamerName")){
      blobStreamerName=vm["blobStreamerName"].as<std::string>();
    }
    boost::program_options::notify(vm);
  }catch(const boost::program_options::error& er) {
    std::cerr << er.what()<<std::endl;
    return 1;
  }
  std::string dictlibrary =  dictionary.empty() ? "" : seal::SharedLibrary::libname( dictionary );
  if(debug){
    std::cout<<"connection:\t"<<destConnect<<'\n';
    std::cout<<"logDb:\t"<<logConnect<<'\n';
    std::cout<<"dictionary:\t"<<dictlibrary<<'\n';
    std::cout<<"tag:\t"<<destTag<<'\n';
    std::cout<<"fromTime:\t"<<from<<'\n';
    std::cout<<"sinceTime:\t"<<since<<'\n';
    std::cout<<"authPath:\t"<<authPath<<'\n';
    std::cout<<"use Blob streamer"<<blobStreamerName<<'\n';
    std::cout<<"configFile:\t"<<configuration_filename<<std::endl;
  }
  //
  edmplugin::PluginManager::Config config;
  edmplugin::PluginManager::configure(edmplugin::standard::config());

  if (!dictlibrary.empty())
  try {
    seal::SharedLibrary::load( dictlibrary );
  }catch ( seal::SharedLibraryError *error) {
    throw std::runtime_error( error->explainSelf().c_str() );
  }

  cond::DBSession session;

  if(!debug){
    session.configuration().setMessageLevel(cond::Error);
  }else{
    session.configuration().setMessageLevel(cond::Debug);
  }

  session.configuration().setAuthenticationMethod(cond::XML);
  session.configuration().setBlobStreamer(blobStreamerName);

  std::string pathval("CORAL_AUTH_PATH=");
  pathval+=authPath;
  ::putenv(const_cast<char*>(pathval.c_str()));

  cond::ConnectionHandler& conHandler=cond::ConnectionHandler::Instance();
  conHandler.registerConnection("destdb",destConnect,-1);
  if (!logConnect.empty()) 
    conHandler.registerConnection("logdb",logConnect,-1);


  try{
    session.open();
    
    conHandler.connect(&session);
    std::string iovtoken;
    cond::TimeType iovtype;
    if( destConnect.find("sqlite_fip:") != std::string::npos ){
      cond::FipProtocolParser p;
      destConnect=p.getRealConnect(destConnect);
    }
    
    
    // find tag
    {
      cond::CoralTransaction& coralDB=conHandler.getConnection("destdb")->coralTransaction();
      coralDB.start(true);
      cond::MetaData  metadata(coralDB);
      if( !metadata.hasTag(destTag) ){
	throw std::runtime_error(std::string("tag ")+destTag+std::string(" not found") );
      }
      //sourceiovtoken=sourceMetadata->getToken(inputTag);
      cond::MetaDataEntry entry;
      metadata.getEntryByTag(destTag,entry);
      iovtoken=entry.iovtoken;
      iovtype=entry.timetype;
      std::string const & timetypestr = cond::timeTypeSpecs[iovtype].name;
       
      coralDB.commit();
      if(debug){
	std::cout<<"iov token "<< iovtoken<<std::endl;
	std::cout<<"iov type "<<  timetypestr<<std::endl;
      }
    }
    

    cond::PoolTransaction& destdb=conHandler.getConnection("destdb")->poolTransaction();
    cond::IOVService iovmanager(destdb);
    std::string payload;
    int size=0;
    {
      // to be streamlined
      cond::IOVProxy iov(destdb,iovtoken,false);
      size = iov.size();
      payload = iovmanager.payloadToken(iovtoken,from);
      if (payload.empty()) {
	std::cerr <<"[Error] no payload found for time " << from << std::endl;
	return 1;
      }
      if ( (iov.end()-1)->token()==payload) {
	std::cerr <<"[Warning] payload for time " << from
		  <<" equal to last inserted payload, no new IOV will be created" <<  std::endl;
	return 0;
      }
      if (payload == iovmanager.payloadToken(iovtoken,since)) {
	std::cerr <<"[Warning] payload for time " << from 
		  <<" equal to payload valid at time "<< since
		  <<", no new IOV will be created" <<  std::endl;
	return 0;
      }
      
    }
      

    // setup logDB
    std::auto_ptr<cond::Logger> logdb;
    if (!logConnect.empty()) {
      logdb.reset(new cond::Logger(conHandler.getConnection("logdb")));
      //logdb->getWriteLock();
      logdb->createLogDBIfNonExist();
      //logdb->releaseWriteLock();
    }
    cond::UserLogInfo a;
    a.provenance=sourceConnect+"/"+destTag;
    a.usertext="duplicateIOV V1.0;";
    {
      std::ostringstream ss; 
      ss << "from="<< from
	 <<", since="<< since <<";";
      a.usertext +=ss.str();
    }


    //append it
    std::auto_ptr<cond::IOVEditor> editor(iovmanager.newIOVEditor());
    destdb.start(false);
    editor.append(since,payload);
    destdb.commit();
 

    if (!logConnect.empty()){
      logdb->getWriteLock();
      logdb->logOperationNow(a,destConnect,payload,destTag,timetypestr,size);
      logdb->releaseWriteLock();
    }

  }catch(const cond::Exception& er){
    std::cout<<"error "<<er.what()<<std::endl;
  }catch(const std::exception& er){
    std::cout<<"std error "<<er.what()<<std::endl;
  }
  return 0;
}
