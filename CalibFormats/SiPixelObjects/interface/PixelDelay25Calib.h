#ifndef PixelDELAY25CALIB_h
#define PixelDELAY25CALIB_h
/**
*   \file CalibFormats/SiPixelObjects/interface/PixelDelay25Calib.h
*   \brief This class manages data and files used in the Delay25 calibration
*
*   A longer explanation will be placed here later
*/
#include <vector>
#include <string>
#include <set>
#include <fstream>
#include "CalibFormats/SiPixelObjects/interface/PixelCalibBase.h"
#include "CalibFormats/SiPixelObjects/interface/PixelConfigBase.h"

namespace pos{
/*!  \ingroup ConfigurationObjects "Configuration Objects"
*    
*  @{
*
*  \class PixelDelay25Calib PixelDelay25Calib.h
*  \brief This class manages data and files used in the Delay25 calibration
*/
  class PixelDelay25Calib : public PixelCalibBase, public PixelConfigBase{

  public:
  
    PixelDelay25Calib(std::string);
    ~PixelDelay25Calib();

    virtual void writeASCII(std::string dir="") const;


    virtual std::string mode() {return mode_;}
    std::set<std::string>& portcardList() {return portcardNames_;}
    bool allPortcards() {return allPortcards_;}
    bool allModules() {return allModules_;}
    int getGridSize() {return gridSize_;}
    int getGridSteps() {return gridSteps_;}
    int getNumberTests() {return numTests_;}
    int getRange() {return range_;}
    int getOrigSDa() {return origSDa_;}
    int getOrigRDa() {return origRDa_;}
    void openFiles(std::string portcardName, std::string moduleName, 
		   std::string path="");
    void writeSettings(std::string portcardName, std::string moduleName);
    void writeFiles(std::string tmp);
    void writeFiles(int currentSDa, int currentRDa, int number);
    void closeFiles();

  private:
    std::string mode_;
    std::set<std::string> portcardNames_;
    bool allPortcards_, allModules_;
    int origSDa_, origRDa_, range_, gridSize_, gridSteps_, numTests_;
    std::ofstream graphout_;
    std::ofstream goodout_;
    std::string graph_, good_;

  };
}
/* @} */
#endif
