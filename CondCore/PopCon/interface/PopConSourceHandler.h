#ifndef  PopConSourceHandler_H
#define  PopConSourceHandler_H


#include "CondCore/DBCommon/interface/Time.h"
#include "CondCore/DBOutputService/interface/TagInfo.h"
#include "CondCore/DBOutputService/interface/LogDBEntry.h"

#include <boost/bind.hpp>
#include <algorithm>
#include <vector>
#include <string>

namespace popcon {

  /** Online DB source handler, aims at returning the vector of data to be 
   * transferred to the online database
   * Subdetector developers inherit over this class with template parameter of 
   * payload class; 
   * need just to implement the getNewObjects method that loads the calibs,
   * the sourceId methods that return a text identifier of the source,
   * and provide a constructor that accept a ParameterSet
   */
  template <class T>
    class PopConSourceHandler{
    public: 
    typedef T value_type;
    typedef PopConSourceHandler<T> self;
    typedef std::vector<std::pair<T*, cond::Time_t> > Container;
    typedef cond::Time_t Time_t;


    PopConSourceHandler(){}
    
    virtual ~PopConSourceHandler(){
    }
    

    cond::TagInfo const & tagInfo() const { return  *m_tagInfo; }

    // return last successful log entry for the tag in question
    cond::LogDBEntry const & logDBEntry() const { return *m_logDBEntry; }


    void initialize (cond::TagInfo const & tagInfo, cond::LogDBEntry const & logDBEntry)  {
      m_tagInfo = &tagInfo;
      m_logDBEntry = &logDBEntry;
    }

    // this is the only mandatory interface
    Container const & operator()(cond::TagInfo const & tagInfo, 
				 cond::LogDBEntry const & logDBEntry) const {
      const_cast<self*>(this)->initialize(tagInfo, logDBEntry);
      return const_cast<self*>(this)->returnData();
    }
    
    Container const &  returnData() {
      getNewObjects();
      sort();
      return m_to_transfer;
    }
    
    //Implement to fill m_to_transfer vector
    //use getOfflineInfo to get the contents of offline DB
    virtual void getNewObjects()=0;

    // return a string identifing the source
    virtual std::string id() const=0;

    void sort() {
      std::sort(m_to_transfer.begin(),m_to_transfer.end(),
		boost::bind(std::less<cond::Time_t>(),
			    boost::bind(&Container::value_type::second,_1),
			    boost::bind(&Container::value_type::second,_2)
			    )
		);
    }
    
  private:
    
    cond::TagInfo const * m_tagInfo;
    
    cond::LogDBEntry const * m_logDBEntry;
    
  protected:
    
    //vector of payload objects and iovinfo to be transferred
    //class looses ownership of payload object
    Container m_to_transfer;
  };
}
#endif
