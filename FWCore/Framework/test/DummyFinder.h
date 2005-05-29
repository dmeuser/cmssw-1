#ifndef EVENTSETUP_DUMMYFINDER_H
#define EVENTSETUP_DUMMYFINDER_H
// -*- C++ -*-
//
// Package:     CoreFramework
// Class  :     DummyFinder
// 
/**\class DummyFinder DummyFinder.h Core/CoreFramework/interface/DummyFinder.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Chris Jones
// Created:     Sat Apr 16 18:47:04 EDT 2005
// $Id: DummyFinder.h,v 1.3 2005/05/26 20:50:51 chrjones Exp $
//

// system include files

// user include files
#include "FWCore/CoreFramework/interface/EventSetupRecordIntervalFinder.h"
#include "FWCore/CoreFramework/test/DummyRecord.h"

// forward declarations

class DummyFinder : public edm::eventsetup::EventSetupRecordIntervalFinder {
public:
   DummyFinder() {
      this->findingRecord<DummyRecord>();
   }
   
   void setInterval( const edm::ValidityInterval& iInterval ) {
      interval_ = iInterval;
   }
protected:
   virtual void setIntervalFor( const edm::eventsetup::EventSetupRecordKey&,
                                const edm::Timestamp& iTime, 
                                edm::ValidityInterval& iInterval) {
      if( interval_.validFor( iTime ) ) {
         iInterval = interval_;
      } else {
         if( interval_.last() == edm::Timestamp::invalidTimestamp() &&
             interval_.first() != edm::Timestamp::invalidTimestamp() &&
             interval_.first() <= iTime ) {
            iInterval = interval_;
         }else {
            iInterval = edm::ValidityInterval();
         }
      }
   }
private:
   edm::ValidityInterval interval_;   
};


#endif /* EVENTSETUP_DUMMYFINDER_H */
