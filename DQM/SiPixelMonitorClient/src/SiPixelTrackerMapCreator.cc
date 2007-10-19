/*! \file SiPixelTrackerMapCreator.cc
 *
 *  \brief This class represents ...
 *  
 *  (Documentation under development)
 *  
 */
#include "DQM/SiPixelMonitorClient/interface/SiPixelTrackerMapCreator.h"
#include "DQM/SiPixelMonitorClient/interface/SiPixelUtility.h"
#include "DQM/SiPixelMonitorClient/interface/SiPixelInformationExtractor.h"
#include "DQM/SiPixelMonitorClient/interface/ANSIColors.h"
#include "DQMServices/Core/interface/QTestStatus.h"
#include "DQMServices/Core/interface/DaqMonitorBEInterface.h"
#include <qstring.h>
#include <qregexp.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "TText.h"
using namespace std;

//==============================================================================
// -- Constructor
// 
SiPixelTrackerMapCreator::SiPixelTrackerMapCreator(string themEName, 
                                                   string theTKType) 
{
  cout << ACYellow << ACBold 
       << "[SiPixelTrackerMapCreator::SiPixelTrackerMapCreator()]" 
       << ACPlain << " ctor" << endl ;
       
  mEName = themEName ;
  TKType = theTKType ;

  stringstream title ;
  title.str("") ; 
  title << themEName ;
	
  trackerMap      = new SiPixelTrackerMap(title.str());
  infoExtractor_  = new SiPixelInformationExtractor();
}

//==============================================================================
// -- Destructor
//
SiPixelTrackerMapCreator::~SiPixelTrackerMapCreator() 
{
  if (trackerMap)     delete trackerMap;
  if (infoExtractor_) delete infoExtractor_;
  cout << ACYellow << ACBold 
       << "[SiPixelTrackerMapCreator::~SiPixelTrackerMapCreator()]" 
       << ACPlain << " dtor" << endl ;
}

//------------------------------------------------------------------------------
/*! \brief (Documentation under construction).
 *  
 */
void SiPixelTrackerMapCreator::create(DaqMonitorBEInterface * bei) 
{
//   cout << ACYellow << ACBold
//        << "[SiPixelTrackerMapCreator::create()] "
//        << ACPlain 
//        << " Creating tracker map for ME: " 
//        << mEName 
//        << endl;

  vector<MonitorElement*> mEList ;
  map<string, int>        mEHash ;
  
  infoExtractor_->selectMEList(bei, mEName, mEList) ;
  infoExtractor_->getMEList(   bei,         mEHash) ;
  
  int nImages = mEHash.size() ;
  
  for(vector<MonitorElement*>::iterator it=mEList.begin(); it!=mEList.end(); it++)
  {
    paintTkMap(*it);
  }

  trackerMap->print(true, TKType);  

  ofstream innerFrame ;

  innerFrame.open( "rightEmbedded.html", ios::out );
  
  if( !innerFrame )
  {
   cout << ACRed << ACBold
   	<< "[SiPixelTrackerMapCreator::create()] "
   	<< ACCyan << ACBold
   	<< "Could not open rightEmbedded.html"
   	<< ACPlain
   	<< endl ;
   return ;
  }
  
  innerFrame << "<html>					    				   " << "\n"
             << "<!--									   " << "\n"
             << " Author: D. Menasce							   " << "\n"
             << " Test widget for the Pixel Tracker Map 				   " << "\n"
             << "-->									   " << "\n"
             << "									   " << "\n"
             << "<meta http-equiv='pragma'						   " << "\n"
             << "      content   ='no-cache'>						   " << "\n"
             << "									   " << "\n"
             << "<head> 								   " << "\n"
             << " <link   rel  = 'stylesheet'						   " << "\n"
             << "	  type = 'text/css'						   " << "\n"
             << "	  href = 'css_files/wz_dragdrop.css>'				   " << "\n"
             << " <link   rel  = 'stylesheet'						   " << "\n"
             << "	  type = 'text/css'						   " << "\n"
             << "	  href = 'css_files/magnifier.css'>				   " << "\n"
             << " <script type = 'text/javascript'					   " << "\n"
             << "	  src  = 'js_files/magnifier.js'>				   " << "\n"
             << " </script>								   " << "\n"
             << " <script type = 'text/javascript'					   " << "\n"
             << "	  src  = 'js_files/wz_dragdrop.js'>				   " << "\n"
             << " </script>								   " << "\n"
             << " <script type = 'text/javascript'					   " << "\n"
             << "	  src  = 'js_files/DMLibrary.js'>				   " << "\n"
             << " </script>								   " << "\n"
             << "									   " << "\n"
             << "</head>								   " << "\n"
             << "									   " << "\n"
             << "<body bgcolor='#414141'>						   " << "\n"
             << "									   " << "\n"
             << " <center>								   " << "\n"
             << "									   " << endl ;
  for( int img=1; img<=nImages; img++)
  {
    stringstream sId, sNm ;
    sId.str(""); sId << "binding"   << img ;
    sNm.str(""); sNm << "baseImage" << img ;
    string sid = sId.str() ;
    string snm = sNm.str() ;
    innerFrame << "  <div   id	        = 'binding'					      		   " << "\n" 
               << "         name        = '" << sid << "'> 				      		   " << "\n" 
               << "   <img  id  	= '" << snm << "'  				      		   " << "\n" 
               << "	    name	= '" << snm << "'  				      		   " << "\n" 
               << "	    src 	= 'images/EmptyPlot.png'			      		   " << "\n" 
               << "	    alt 	= 'picture geometry:800x600'			      		   " << "\n" 
               << "	    onload	= 'RightEmbedded.innerLoading(\"" << sid << "\",\"" << snm << "\")'" << "\n"
               << "	    onclick	= 'RightEmbedded.innerTransport(event)'			           " << "\n"
               << "	    onmouseover = 'this.T_SHADOWWIDTH=4;			      		   " << "\n"
               << "			   this.T_OPACITY    =70;			      		   " << "\n"
               << "			   this.T_FONTCOLOR  =\"#000000\";		      		   " << "\n"
               << "			   this.T_WIDTH      =200;			      		   " << "\n"
               << "			   return escape(\"Click to send to pan/zoom area\")' 		   " << "\n"
               << "	    width	= '267' 					      		   " << "\n"
               << "	    height	= '200' />					      		   " << "\n"
               << "  </div>								      		   " << endl ;
  }
  innerFrame << "  									   " << "\n"
             << "</center>								   " << "\n"
             << "									   " << "\n"
             << "<script type = 'text/javascript'					   " << "\n"
             << "	 src  = 'js_files/rightEmbedded.js'>				   " << "\n"
             << "</script>								   " << "\n"
             << "									   " << "\n"
             << "<script type='text/javascript' 					   " << "\n"
             << "	 src ='js_files/wz_tooltip.js'> 				   " << "\n"
 	     << "</script>								   " << "\n"
 	     << "</html>								   " << endl ;
  innerFrame.close() ;    								    
}

//==============================================================================
// -- Draw Monitor Elements
//
void SiPixelTrackerMapCreator::paintTkMap(MonitorElement * mE) 
{
  double sts;
  int    rval, gval, bval, detId;

//  sts   = infoExtractor_->computeStatus(mE) ; 
//  detId = infoExtractor_->getDetId(     mE);
//  SiPixelUtility::getStatusColor(sts,   rval, gval, bval);
//  trackerMap->fillc(             detId, rval, gval, bval);
}

//==============================================================================
// -- get Tracker Map ME 
//
MonitorElement* SiPixelTrackerMapCreator::getTkMapMe(DaqMonitorBEInterface* bei,
                                                     string& me_name, 
					             int ndet) 
{
/*
  string new_name = "TrackerMap_for_" + me_name;
  string path = "Collector/" + new_name;
  MonitorElement*  tkmap_me =0;
  tkmap_me = mui->get(path);
  if (!tkmap_me) 
  {
    string save_dir = mui->pwd();   
    DaqMonitorBEInterface * bei = mui->getBEInterface();
    bei->setCurrentFolder("Collector");
    tkmap_me = bei->book1D(new_name, new_name, ndet, 0.5, ndet+0.5);
    bei->setCurrentFolder(save_dir);
  }
  return tkmap_me;
*/
}
//==============================================================================
// -- Browse through monitorable and get values needed by TrackerMap
//
bool SiPixelTrackerMapCreator::exploreBeiStructure(DaqMonitorBEInterface* bei) 
{
  cout << ACCyan << ACBold
       << "[SiPixelTrackerMapCreator::create()] "
       << ACRed << ACReverse 
       << "List of histograms in"
       << ACPlain
       << " "
       << ACBlue << ACBold
       << bei->pwd()
       << ACPlain << endl ;


  vector<string> histoList = bei->getMEs(); 
     
  for (vector<string>::const_iterator it = histoList.begin(); it != histoList.end(); it++) 
  {
   cout << ACCyan << ACBold
   	<< "[SiPixelTrackerMapCreator::create()] "
   	<< ACRed << ACReverse 
   	<< "Histogram:"
   	<< ACPlain
   	<< " "
   	<< *it
   	<< ACPlain << endl ;
  }

  vector<string> subDirs = bei->getSubdirs();
  
  for (vector<string>::const_iterator it = subDirs.begin(); it != subDirs.end(); it++) 
  {
    bei->cd(*it);
    cout << ACCyan << ACBold
    	 << "[SiPixelTrackerMapCreator::create()] "
    	 << ACRed << ACBold 
    	 << "Moved down to"
    	 << ACPlain
    	 << " "
    	 << ACBlue << ACBold
    	 << bei->pwd()
    	 << ACPlain << endl ;
    exploreBeiStructure(bei);
    bei->goUp();
    cout << ACCyan << ACBold
    	 << "[SiPixelTrackerMapCreator::create()] "
    	 << ACRed << ACBold 
    	 << "Now back to"
    	 << ACPlain
    	 << " "
    	 << ACBlue << ACBold
    	 << bei->pwd()
    	 << ACPlain << endl ;
  }
  return true ;
}
