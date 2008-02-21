#ifndef QTestHandle_H
#define QTestHandle_H

/** \class QTestHandle
 * *
 *  Handles quality tests (configuring, attaching to ME's, 
 *
 *  $Date: 2007/12/17 18:32:13 $
 *  $Revision: 1.5 $
 *  \author Ilaria Segoni
 */
  
#include<string>
#include<vector>
#include<map>

class DQMStore;
class QTestConfigurationParser;
class QTestConfigure;
class QTestStatusChecker;

class QTestHandle{
public:
  ///Creator
  QTestHandle();
  ///Destructor
  ~QTestHandle();
  ///Parses Config File and configures the quality tests
  bool configureTests(const std::string &configFile, DQMStore *bei);
  ///Attaches the quality tests to the MonitorElement
  void attachTests(DQMStore * bei);
  ///Checks global status of Quality Tests
  std::pair<std::string,std::string> checkGlobalQTStatus(DQMStore *bei) const;
  ///Checks alarms for single MonitorElements
  std::map< std::string, std::vector<std::string> > checkDetailedQTStatus(DQMStore *bei) const;
  
private:
  QTestConfigurationParser * qtParser;
  QTestConfigure * qtConfigurer;
  QTestStatusChecker * qtChecker;
  bool testsConfigured;
};


#endif
