#ifndef DTLVStatusHandler_H
#define DTLVStatusHandler_H
/** \class DTLVStatusHandler
 *
 *  Description: Class to copy CCB DCS-status via PopCon
 *
 *
 *  $Date: 2009/03/26 14:11:03 $
 *  $Revision: 1.1 $
 *  \author Paolo Ronchese INFN Padova
 *
 */

//----------------------
// Base Class Headers --
//----------------------
#include "CondCore/PopCon/interface/PopConSourceHandler.h"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CondCore/DBCommon/interface/DbConnection.h"
#include "CondFormats/DTObjects/interface/DTLVStatus.h"
#include <string>


//---------------
// C++ Headers --
//---------------


//              ---------------------
//              -- Class Interface --
//              ---------------------

class DTLVStatusHandler: public popcon::PopConSourceHandler<DTLVStatus> {

 public:

  /** Constructor
   */
  DTLVStatusHandler( const edm::ParameterSet& ps );

  /** Destructor
   */
  virtual ~DTLVStatusHandler();

  /** Operations
   */
  /// 
  void getNewObjects();
  std::string id() const;

 private:

  std::string dataTag;
  std::string onlineConnect;
  std::string onlineAuthentication;
  std::string bufferConnect;
  DTLVStatus* ccbStatus;

  cond::DbConnection connection;
  cond::DbSession omds_session;
  cond::DbSession buff_session;

};


#endif // DTLVStatusHandler_H






