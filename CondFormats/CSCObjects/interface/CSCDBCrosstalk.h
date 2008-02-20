#ifndef CSCObjects_CSCDBCrosstalk_h
#define CSCObjects_CSCDBCrosstalk_h

#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include <vector>

class CSCDBCrosstalk
{
 public:
  CSCDBCrosstalk() {}
  ~CSCDBCrosstalk() {}
  
  struct Item{
    short int xtalk_slope_right;
    short int xtalk_intercept_right;
    short int xtalk_slope_left;
    short int xtalk_intercept_left;
  };

  // accessor to appropriate element
  const Item & item(const CSCDetId & cscId, int strip) const;

  typedef std::vector<Item> CrosstalkContainer;

  CrosstalkContainer crosstalk;
};

#endif

