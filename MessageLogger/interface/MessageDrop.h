#ifndef MessageLogger_MessageDrop_h
#define MessageLogger_MessageDrop_h

// -*- C++ -*-
//
// Package:     MessageLogger
// Class  :     MessageDrop
//
/**\class MessageDrop MessageDrop.h 

 Description: <one line class summary>

 Usage:
    <usage>

*/

//
// Original Author:  M. Fischler and Jim Kowalkowski
//         Created:  Tues Feb 14 16:38:19 CST 2006
// $Id$
//

// Framework include files

#include "FWCore/Utilities/interface/EDMException.h"	// change log 4


// system include files

#include <string>

// Change log
//
//  1  mf 5/12/06	initialize debugEnabled to true, to avoid unitialized
//			data detection in memory checks (and to be safe in
//			getting enabled output independant of timings) 
//
//  4  mf 2/22/07	static ex_p to have a way to convey exceptions to throw
//			(this is needed when configuring could lead to an 
//			exception, for example)
//
//  5  mf 2/22/07	jobreport_name to have a way to convey content
//			of jobreport option from cmsRun to MessageLogger class
//
//  6  mf 6/18/07	jobMode to have a way to convey choice of hardwired
//			MessageLogger defaults
//
//  7  mf 6/20/08	MessageLoggerScribeIsRunning to let the scribe convey
//			that it is active.
//
//  8  cdj 2/08/10      Make debugEnabled, infoEnabled, warningEnabled statics
//                      to avoid overhead of thread specific singleton access
//                      when deciding to keep or throw away messages
//
//  9  mf 9/23/10	Support for situations where no thresholds are low
//                      enough to react to LogDebug (or info, or warning)
//
// 10  mf, crj 11/2/10	(see MessageServer/src/MessageLogger.cc change 17)
//                      Support for reducing the string operations done when
//			moving from one module to the next.
//
// 11  mf 11/29/10	Snapshot method to preare for invalidation of the   
//			pointers used to hold module context.  Supports 
//			surviving throws that cause objects to go out of scope.
//
// 12  fwyzard 7/6/11   Add support for discarding LogError-level messages
//                      on a per-module basis (needed at HLT)

// user include files

namespace edm {

namespace messagedrop {
  class StringProducer;
  class StringProducerWithPhase;
  class StringProducerPath; 
  class StringProducerSinglet;  
}

struct MessageDrop {
private:
  MessageDrop();					// change log 10:
  							// moved to cc file  
public:
  ~MessageDrop();					// change log 10
  static MessageDrop * instance ();
  std::string moduleName;
  std::string moduleContext();
  void setModuleWithPhase(std::string const & name,
  			  std::string const & label,
			  const void * moduleID,
			  const char* phase);  
  void setPath(const char* type, std::string const & pathname);
  void setSinglet(const char * sing);
  void snapshot();
  std::string runEvent;
  std::string jobreport_name;				// change log 5
  std::string jobMode;					// change log 6
  static bool debugEnabled;                             // change log 8
  static bool infoEnabled;                              // change log 8
  static bool warningEnabled;                           // change log 8
  static bool errorEnabled;                             // change log 8, 12
  static unsigned char messageLoggerScribeIsRunning;	// change log 7
  static edm::Exception * ex_p;				// change log 4
  static bool debugAlwaysSuppressed;			// change log 9
  static bool infoAlwaysSuppressed;			// change log 9
  static bool warningAlwaysSuppressed;			// change log 9
private:
  messagedrop::StringProducerWithPhase * spWithPhase;
  messagedrop::StringProducerPath      * spPath;
  messagedrop::StringProducerSinglet   * spSinglet;
  messagedrop::StringProducer const    * moduleNameProducer;
};

static const unsigned char  MLSCRIBE_RUNNING_INDICATOR = 29; // change log 7

} // end of namespace edm


#endif // MessageLogger_MessageDrop_h
