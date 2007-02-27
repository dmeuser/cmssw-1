//-------------------------------------------------
//
//   Class L1MuDTChambPhContainer
//
//   Description: input data for PHTF trigger
//
//
//   Author List: Jorge Troconiz  UAM Madrid
//
//
//--------------------------------------------------
#ifndef L1MuDTChambPhContainer_H
#define L1MuDTChambPhContainer_H

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhDigi.h"

//----------------------
// Base Class Headers --
//----------------------
#include <vector>

//---------------
// C++ Headers --
//---------------

//              ---------------------
//              -- Class Interface --
//              ---------------------


class L1MuDTChambPhContainer {

 public:

  typedef std::vector<L1MuDTChambPhDigi>  Phi_Container;
  typedef Phi_Container::const_iterator   Phi_iterator;

  //  Constructors
  L1MuDTChambPhContainer();

  //  Destructor
  ~L1MuDTChambPhContainer();

  void setContainer(Phi_Container inputSegments);

  Phi_Container* getContainer() const;

  L1MuDTChambPhDigi* chPhiSegm1(int wheel, int stat, int sect, int bx) const;

  L1MuDTChambPhDigi* chPhiSegm2(int wheel, int stat, int sect, int bx) const;

 private:

  Phi_Container phiSegments; 

};

#endif
