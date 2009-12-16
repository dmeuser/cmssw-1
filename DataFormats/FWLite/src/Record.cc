// -*- C++ -*-
//
// Package:     FWLite
// Class  :     Record
// 
// Implementation:
//     [Notes on implementation]
//
// Original Author:  
//         Created:  Thu Dec 10 15:58:33 CST 2009
// $Id$
//

// system include files
#include <cassert>
#include "TTree.h"
#include "Reflex/Type.h"
// user include files
#include "DataFormats/FWLite/interface/Record.h"
#include "DataFormats/Provenance/interface/ESRecordAuxiliary.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/FWLite/interface/format_type_name.h"


//
// constants, enums and typedefs
//
using namespace fwlite;
//
// static data member definitions
//
typedef std::map<IOVSyncValue,unsigned int> StartIOVtoEntryMap;

//
// constructors and destructor
//
Record::Record(const char* iName, TTree* iTree):
m_name(iName), m_tree(iTree), m_entry(-1),
m_start(IOVSyncValue::invalidIOVSyncValue()),
m_end(IOVSyncValue::invalidIOVSyncValue())
{
   //read the start iovs and get them in order
   edm::ESRecordAuxiliary aux;
   edm::ESRecordAuxiliary* pAux=&aux;
   TBranch* auxBranch = m_tree->FindBranch("ESRecordAuxiliary");
   auxBranch->SetAddress(&pAux);
   IOVSyncValue temp;
   for(unsigned int index=0; index < m_tree->GetEntries();++index){
      auxBranch->GetEntry(index);
      if(aux.timestamp() != edm::Timestamp::invalidTimestamp()){
         if(aux.eventID().run() != 0) {
            temp = IOVSyncValue(aux.eventID(),aux.timestamp());
         } else {
            temp = IOVSyncValue(aux.timestamp());
         }
      } else {
         temp=IOVSyncValue(aux.eventID());
         assert(aux.eventID().run()!=0);
      }
      
      m_startIOVtoEntry[temp]=index;
   }
}

// Record::Record(const Record& rhs)
// {
//    // do actual copying here;
// }

Record::~Record()
{
}

//
// assignment operators
//
// const Record& Record::operator=(const Record& rhs)
// {
//   //An exception safe implementation is
//   Record temp(rhs);
//   swap(rhs);
//
//   return *this;
// }

//
// member functions
//
void 
Record::syncTo(const edm::EventID& iEvent, const edm::Timestamp& iTime)
{
   
   IOVSyncValue temp;
   if(iTime != edm::Timestamp::invalidTimestamp()){
      if(iEvent.run() != 0) {
         temp = IOVSyncValue(iEvent,iTime);
      } else {
         temp = IOVSyncValue(iTime);
      }
   } else {
      temp=IOVSyncValue(iEvent);
      assert(iEvent.run()!=0);
   }

   //already synched
   if( (m_start != IOVSyncValue::invalidIOVSyncValue()) && 
       (m_start <=temp ) && 
       ( (m_end == IOVSyncValue::invalidIOVSyncValue()) ||
         (temp <m_end))) {
      return;
   }
   std::pair<StartIOVtoEntryMap::iterator, StartIOVtoEntryMap::iterator> range =
      m_startIOVtoEntry.equal_range(temp);
   if(range.first!=range.second){
      //happens to be the start of the IOV
      m_start = range.first->first;
      m_entry = range.first->second;
   } else {
      if(range.first!=m_startIOVtoEntry.begin()){
         //we have overshot
         --range.first;
         m_start = range.first->first;
         m_entry = range.first->second;
      } else {
         //off the beginning
         m_start=IOVSyncValue::invalidIOVSyncValue();
         m_entry = -1;
      }
   }
   if(range.second==m_startIOVtoEntry.end()){
      m_end = IOVSyncValue::invalidIOVSyncValue();
   } else {
      m_end = range.second->first;
   }
}

//
// const member functions
//
const std::string& 
Record::name() const
{
   return m_name;
}

const IOVSyncValue& 
Record::startSyncValue() const {
   return m_start;
}
const IOVSyncValue& 
Record::endSyncValue() const
{
   return m_end;
}


cms::Exception* 
Record::get(const TypeID& iType, 
            const char* iLabel, 
            const void*& iData) const
{
   cms::Exception* returnValue = 0;
   
   TBranch*& branch = m_branches[std::make_pair(iType,iLabel)];
   if(0==branch){
      using namespace ROOT::Reflex;
      Type t = Type::ByTypeInfo(iType.typeInfo());
      if(t == Type()){
         returnValue = new cms::Exception("UnknownType");
         (*returnValue)<<"The type "
         <<iType.typeInfo().name()<<" was requested from Record "<<name()
         <<" but the type has no known dictionary";
         return returnValue;
      }
      //build branch name
      std::string branchName = fwlite::format_type_to_mangled(t.Name(ROOT::Reflex::SCOPED|ROOT::Reflex::FINAL))+"__"+iLabel;
      branch = m_tree->FindBranch(branchName.c_str());
      
      if(0==branch){
         returnValue = new cms::Exception("NoDataAvailable");
         (*returnValue)<<"The data of type "
                       <<t.Name(ROOT::Reflex::SCOPED|ROOT::Reflex::FINAL)
                       <<" with label '"<<iLabel<<"' for Record "<<name()<<" is not in this file.";
         return returnValue;
      }
   }
   if(m_entry<0) {
      returnValue = new cms::Exception("NoValidIOV");
      (*returnValue) <<" The Record "
         <<name()<<" was asked to get data for a 'time' for which it has no data";
      return returnValue;
   }
   branch->SetAddress(&iData);
   branch->GetEntry(m_entry);
   return returnValue;
}

//
// static member functions
//
