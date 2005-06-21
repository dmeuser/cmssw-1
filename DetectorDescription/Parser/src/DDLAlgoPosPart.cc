/***************************************************************************
                          DDLAlgoPosPart.cc  -  description
                             -------------------
    begin                : Wed Apr 17 2002
    email                : case@ucdhep.ucdavis.edu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *           DDDParser sub-component of DDD                                *
 *                                                                         *
 ***************************************************************************/

namespace std{} using namespace std;

// Parser parts
#include "DetectorDescription/DDParser/interface/DDLAlgoPosPart.h"
#include "DetectorDescription/DDParser/interface/DDLElementRegistry.h"
#include "DetectorDescription/DDParser/interface/DDXMLElement.h"

// DDCore dependencies
#include "DetectorDescription/DDCore/interface/DDalgoPosPart.h"
#include "DetectorDescription/DDCore/interface/DDName.h"
#include "DetectorDescription/DDCore/interface/DDLogicalPart.h"
#include "DetectorDescription/DDCore/interface/DDAlgo.h"
#include "DetectorDescription/DDBase/interface/DDAlgoPar.h"
#include "DetectorDescription/DDBase/interface/DDdebug.h"
#include "DetectorDescription/DDBase/interface/DDException.h"

#include "DetectorDescription/DDExprAlgo/interface/ExprEvalSingleton.h"

#include <string>
#include <iostream>



// Default constructor
DDLAlgoPosPart::DDLAlgoPosPart()
{
}

// Default desctructor
DDLAlgoPosPart::~DDLAlgoPosPart()
{
}

// Upon encountering the end tag of the AlgoPosPart we should have in the meantime
// hit two rLogicalPart calls and one of Rotation or rRotation and a Translation.
// So, retrieve them and make the call to DDCore.
void DDLAlgoPosPart::processElement (const string& type, const string& nmspace)
{
  DCOUT_V('P', "DDLAlgoPosPart::processElement started");
  
  // get all internal elements.
  DDXMLElement* myParent  = DDLElementRegistry::getElement("rParent");
  DDXMLElement* myChild   = DDLElementRegistry::getElement("rChild");
  DDXMLElement* myParS    = DDLElementRegistry::getElement("ParS");
  DDXMLElement* myParE    = DDLElementRegistry::getElement("ParE");
  
  
  ExprEvalInterface & ev = ExprEvalSingleton::instance();
  
  DDXMLAttribute atts = getAttributeSet();

  // these were doubles
  int st = static_cast<int> ((atts.find("start") == atts.end() ? 0.0 : ev.eval(nmspace, atts.find("start")->second)));
  int ic = static_cast<int> ((atts.find("incr") == atts.end() ? 0.0 : ev.eval(nmspace, atts.find("incr")->second)));
  int ed = static_cast<int> ((atts.find("end") == atts.end() ? 0.0 : ev.eval(nmspace, atts.find("end")->second)));
  

  // get actual DDLogicalPart objects.
  DDLogicalPart parent(DDName(myParent->getDDName(nmspace)));
  DDLogicalPart self(DDName(myChild->getDDName(nmspace)));

  // get the algorithm
  DDAlgo algo( getDDName(nmspace, "algo" ));
  if (!(algo.isDefined().second)) 
    {
      string  msg = string("\n\tDDLParser, algo requested is not defined.  Either AlgoInit() or check algo spelling.\n ")
	+ "\n\t\talgo=" + string(getDDName(nmspace, "algo" ))
	+ "\n\t\tparent=" + string(myParent->getDDName(nmspace))
	+ "\n\t\tself=" + string(myChild->getDDName(nmspace));
      throwError(msg);
    }

  // set the parameters for the algorithm

  // First for ParE type
  parE_type parE;
  for (size_t i = 0; i < myParE->size(); i++)
    {
      atts = myParE->getAttributeSet(i);
      // find vname in ParE.
      parE_type::iterator existingName=parE.find(atts.find("name")->second);
      
      // if found, get vector, then add this value to it.
      // if not found, add this var, then add a value to it.
      if (existingName != parE.end())
	existingName->second.push_back(ev.eval(nmspace,atts.find("value")->second));
      //	tvect = existingName->second;
      else
	{
	  vector<double> tvect;
	  tvect.push_back(ev.eval(nmspace,atts.find("value")->second));
	  parE[atts.find("name")->second] = tvect;
	}
    }

  // Now for ParS type
  parS_type parS;

  for (size_t i = 0; i < myParS->size(); i++)
    {
      atts = myParS->getAttributeSet(i);

      // find vname in ParS.
      parS_type::iterator existingName=parS.find(atts.find("name")->second);
      
      // if found, get vector, then add this value to it.
      // if not found, add this var, then add a value to it.

      if (existingName != parS.end())
	existingName->second.push_back(atts.find("value")->second);
      else
	{
	  vector<string> tvect;
	  tvect.push_back(atts.find("value")->second);
	  parS[atts.find("name")->second] = tvect;
	}
    }

  try {
    algo.setParameters(st,ed,ic,parS,parE);
  }
  catch (DDException& e)
    {
      string msg(e.what());
      msg += string("\n\tDDLParser, algo.setParameters failed ")
	+ "\n\t\talgo=" +  string(getDDName(nmspace, "algo" ))
	+ "\n\t\tparent=" + string(myParent->getDDName(nmspace))
	+ "\n\t\tself=" + string(myChild->getDDName(nmspace));
      throwError(msg);

    }
  catch (...) {
    string msg = "Unknown error (...) caught in DDLAlgoPosPart.";
    throwError(msg);
  }

  try
    {  
      DDalgoPosPart(self, parent, algo);
    }
  catch (DDException& e)
    {
      string msg(e.what());
      msg += string("\n\tDDLParser, DDalgoPosPart failed")
	+ "\n\t\talgo=" + string(getDDName(nmspace, "algo"))
	+ "\n\t\tparent=" + string(myParent->getDDName(nmspace))
	+ "\n\t\tself=" + string(myChild->getDDName(nmspace));
      throwError(msg);
    }
  catch (...) {
    cout << "something really bad happened in DDalgoPosPart call." << endl;
  }

  // clear all "children" and attributes
  myChild->clear();
  myParent->clear();
  myParS->clear();
  myParE->clear();
  // after an AlgoPosPart, we are sure it can be cleared.
  clear();

  DCOUT_V('P', "DDLAlgoPosPart::processElement completed");
}

