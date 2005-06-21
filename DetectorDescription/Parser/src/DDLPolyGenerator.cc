/***************************************************************************
                          DDLPolyGenerator.cpp  -  description
                             -------------------
    begin                : Thu Oct 25 2001
    email                : case@ucdhep.ucdavis.edu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *           DDDParser sub-component of DDD                                *
 *                                                                         *
 ***************************************************************************/

namespace std{} using namespace std;

// Parser parts needed by PolyGenerator.
#include "DetectorDescription/DDParser/interface/DDLPolyGenerator.h"
#include "DetectorDescription/DDParser/interface/DDLElementRegistry.h"

// DDCore dependencies
#include "DetectorDescription/DDCore/interface/DDName.h"
#include "DetectorDescription/DDCore/interface/DDSolid.h"
#include "DetectorDescription/DDBase/interface/DDdebug.h"
#include "DetectorDescription/DDBase/interface/DDException.h"

#include "DetectorDescription/DDExprAlgo/interface/ExprEvalSingleton.h"

#include <string>

// Default constructor.
DDLPolyGenerator::DDLPolyGenerator()
{
}

// Default destructor
DDLPolyGenerator::~DDLPolyGenerator()
{
}

void DDLPolyGenerator::preProcessElement (const string& name, const string& nmspace)
{
  DDLElementRegistry::getElement("RZPoint")->clear();
  DDLElementRegistry::getElement("ZSection")->clear();
}

// Upon encountering an end Polycone or Polyhedra tag, process the RZPoints
// element and extract the r and z vectors to feed into DDCore.  Then, clear
// the RZPoints and clear this element.
void DDLPolyGenerator::processElement (const string& name, const string& nmspace)
{
  DCOUT_V('P', "DDLPolyGenerator::processElement started");

  DDXMLElement* myRZPoints = DDLElementRegistry::getElement("RZPoint");
  DDXMLElement* myZSection = DDLElementRegistry::getElement("ZSection");

  ExprEvalInterface & ev = ExprEvalSingleton::instance();
  DDXMLAttribute atts;

  // get z and r
  vector<double> z, r;
  for (size_t i = 0; i < myRZPoints->size(); i++)
    {
      atts = myRZPoints->getAttributeSet(i);
      z.push_back(ev.eval(nmspace, atts.find("z")->second));
      r.push_back(ev.eval(nmspace, atts.find("r")->second));
    }

  // if z is empty, then it better not have been a polycone defined
  // by RZPoints, instead, it must be a ZSection defined polycone.
  try {
   if (z.size() == 0 )
     {
       // get zSection information, note, we already have a z declared above
       // and we will use r for rmin.  In this case, no use "trying" because
       // it better be there!
       vector<double> rMax;

       for (size_t i = 0; i < myZSection->size(); i++)
	 {
	   atts = myZSection->getAttributeSet(i);
	   z.push_back(ev.eval(nmspace, atts.find("z")->second));
	   r.push_back(ev.eval(nmspace, atts.find("rMin")->second));
	   rMax.push_back(ev.eval(nmspace, atts.find("rMax")->second));
	 }
       atts = getAttributeSet();
       if (name == "Polycone") // defined with ZSections 
	 {
	   DDSolid ddpolycone = 
	     DDSolidFactory::polycone(getDDName(nmspace)
		    , ev.eval(nmspace, atts.find("startPhi")->second)
		    , ev.eval(nmspace, atts.find("deltaPhi")->second)
		    , z
		    , r
		    , rMax);
	 }
       else if (name == "Polyhedra")  // defined with ZSections
	 {
	   DDSolid ddpolyhedra = 
	     DDSolidFactory::polyhedra(getDDName(nmspace)
		    , int (ev.eval(nmspace, atts.find("numSide")->second))
		    , ev.eval(nmspace, atts.find("startPhi")->second)
		    , ev.eval(nmspace, atts.find("deltaPhi")->second)
		    , z
		    , r
		    , rMax);
	 }

     }
   else if (name == "Polycone") // defined with RZPoints
     {
       atts = getAttributeSet();
       DDSolid ddpolycone = 
	 DDSolidFactory::polycone(getDDName(nmspace)
		, ev.eval(nmspace, atts.find("startPhi")->second)
		, ev.eval(nmspace, atts.find("deltaPhi")->second)
		, z
		, r);
     }
   else if (name == "Polyhedra") // defined with RZPoints
     {
       atts = getAttributeSet();
       DDSolid ddpolyhedra = 
	 DDSolidFactory::polyhedra(getDDName(nmspace)
		, int (ev.eval(nmspace, atts.find("numSide")->second))
		, ev.eval(nmspace, atts.find("startPhi")->second)
		, ev.eval(nmspace, atts.find("deltaPhi")->second)
		, z
		, r);
     }
   else
     {
       string msg = "\nDDLPolyGenerator::processElement was called with incorrect name of solid: " + name;
       throwError(msg);
     }
  } catch (DDException & e) {
    string msg("\nDDLPolyGenerator failed when calling the DDSolidFactory.");
    throwError(msg);
  }
  DDLSolid::setReference(nmspace);

  // clear out RZPoint element accumulator and ZSections
  myRZPoints->clear();
  myZSection->clear();
  clear();

  DCOUT_V('P', "DDLPolyGenerator::processElement completed");
}
