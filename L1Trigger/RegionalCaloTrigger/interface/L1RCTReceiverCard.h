#ifndef L1RCTReceiverCard_h
#define L1RCTReceiverCard_h

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include "L1RCTRegion.h"
#include "L1RCTLookupTables.h"

using std::vector;
using std::bitset;
using std::string;
using std::cout;
using std::endl;

class L1RCTReceiverCard {

 public:

  L1RCTReceiverCard(int crateNumber, int cardNumber);
  ~L1RCTReceiverCard();

  //Information needed to identify cards
  int crateNumber() {return crtNo;}
  int cardNumber() {return cardNo;}

  //Takes in a 64 element vector of unsigned shorts.
  //First layer is ecal the second is hcal.
  //goes in order of (for crate 0,card 0)
  // (Region 1)   (Region 0)
  // 29 25 21 17 13 09 05 01
  // 30 26 22 18 14 10 06 02
  // 31 27 23 19 15 11 07 03
  // 32 28 24 20 16 12 08 04 
  //
  // For card 6 of crate 0 it would look like 
  //
  // 13 09 05 01
  // 14 10 06 02
  // 15 11 07 03
  // 16 12 08 04
  // 17 21 25 29
  // 18 22 26 30
  // 19 23 27 31
  // 20 24 28 32
  //
  //In either case it is set up as so
  //8bit non linear ET for first tower + tower char. bit for first tower
  //plus 8bit non linear ET for second tower + tower char. bit for second tower
  //for ECAL tower char. bit is FG
  //for HCAL it is muon bit
  void fillInput(vector<unsigned short> input);
  void fillTauBits();
  void fillRegionSums();
  void fillMuonBits();
    
  L1RCTRegion* getRegion(int i) {
    return &regions.at(i);
  }
  unsigned short getTauBitRegion(int i) {return tauBits.at(i);}
  unsigned short getMuonBitRegion(int i) {return muonBits.at(i);}
  unsigned short getEtIn10BitsRegion(int i) {return etIn10Bits.at(i);}

  vector<unsigned short> towerToRegionMap(int towernum);

  void print();

  void printRaw(){
    regions.at(0).printRaw();
    regions.at(1).printRaw();
  }
  void printEdges(){
    regions.at(0).printEdges();
    regions.at(0).printEdges();
  }

  void randomInput();
  void fileInput(char* filename);

 private:
 
  vector<L1RCTRegion> regions;
  
  unsigned short calcRegionSum(L1RCTRegion region);
  unsigned short calcTauBit(L1RCTRegion region);
  unsigned short calcMuonBit(L1RCTRegion region);

  unsigned short crtNo;
  unsigned short cardNo;
  L1RCTLookupTables lut;
  vector<unsigned short> etIn10Bits;
  vector<unsigned short> muonBits;
  vector<unsigned short> tauBits;

  //No default constructor, no copy constructor,
  //and no assignment operator
  L1RCTReceiverCard();
};
#endif
