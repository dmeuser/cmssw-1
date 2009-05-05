#ifndef CSCBadChambers_h
#define CSCBadChambers_h

#include "DataFormats/MuonDetId/interface/CSCDetId.h"

#include <vector>

class CSCBadChambers{
 public:
  CSCBadChambers() {};
  ~CSCBadChambers(){};

  /// How many bad chambers are there>
  int numberOfChambers() const { return numberOfBadChambers; }

  /// Return the container of bad chambers
  std::vector<int> container() const { return chambers; }

  /// Is the gven chamber flagged as bad?
  bool isInBadChamber( const CSCDetId& id ) const;

 private:
  int numberOfBadChambers;
  std::vector<int> chambers;
};

#endif
