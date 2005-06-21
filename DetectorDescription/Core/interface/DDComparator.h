#ifndef DDComparator_h
#define DDComparator_h

#include "DetectorDescription/DDCore/interface/DDExpandedView.h"
#include "DetectorDescription/DDCore/interface/DDPartSelection.h"
#include "DetectorDescription/DDCore/interface/DDLogicalPart.h"


//! compares a given geometrical-history whether it corresponds to the given part-selector
/**
  This is a function-object.
*/
class DDCompareEqual // : public binary_function<DDGeoHistory,DDPartSelection,bool> 
{
public:
  DDCompareEqual(const DDGeoHistory & h, const DDPartSelection & s)
    : hist_(h), 
    partsel_(s), 
    hMax_(h.size()), 
    hIndex_(0), 
    sMax_(s.size()), 
    sIndex_(0), 
    hLp_(), 
    sLp_(), 
    sCopyno_(0), 
    absResult_(true) 
    { 
      // it makes only sense to compare if both vectors have at least one entry each.
      if (hMax_>0 && sMax_>0 ) {
	hLp_ = hist_[0].logicalPart();
	sLp_ = partsel_[0].lp_;
      }
      else  {
	absResult_=false;
      }
      //cout << endl << endl << "COMPARATOR CREATED" << endl << endl;
      //DCOUT('U', "Comparator():\n  hist=" << h << "\n  PartSel=" << s);
    }

  bool operator() (const DDGeoHistory &, const DDPartSelection &);
  bool operator() ();

protected:
  inline bool nextAnylogp();
  inline bool nextAnyposp();
  inline bool nextChildlogp();
  inline bool nextChildposp();
  
private:
  DDCompareEqual();  
  const DDGeoHistory & hist_;
  const DDPartSelection & partsel_;
  DDGeoHistory::size_type hMax_, hIndex_;
  DDPartSelection::size_type sMax_, sIndex_;
  DDLogicalPart hLp_, sLp_;
  /*
  lpredir_type * hLp_;
  lpredir_type * sLp_;
  */
  int sCopyno_;
  bool absResult_;
};


#endif
