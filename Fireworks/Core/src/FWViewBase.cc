// -*- C++ -*-
//
// Package:     Core
// Class  :     FWViewBase
// 
// Implementation:
//     <Notes on implementation>
//
// Original Author:  Chris Jones
//         Created:  Thu Feb 21 14:43:19 EST 2008
// $Id: FWViewBase.cc,v 1.6 2008/07/21 00:59:11 jmuelmen Exp $
//

// system include files
#include "TGFileDialog.h"

// user include files
#include "Fireworks/Core/interface/FWViewBase.h"


//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
FWViewBase::FWViewBase(unsigned int iVersion):
FWConfigurableParameterizable(iVersion)
{
}

// FWViewBase::FWViewBase(const FWViewBase& rhs)
// {
//    // do actual copying here;
// }

FWViewBase::~FWViewBase()
{
}

//
// assignment operators
//
// const FWViewBase& FWViewBase::operator=(const FWViewBase& rhs)
// {
//   //An exception safe implementation is
//   FWViewBase temp(rhs);
//   swap(rhs);
//
//   return *this;
// }

//
// member functions
//
void 
FWViewBase::destroy()
{
   beingDestroyed_(this);
}

//
// const member functions
//
void 
FWViewBase::promptForSaveImageTo(TGFrame* iParent) const
{
   static TString dir(".");
   const char *  kImageExportTypes[] = {"Encapsulated PostScript", "*.eps",
      "PDF",                     "*.pdf",
      "GIF",                     "*.gif",
      "JPEG",                    "*.jpg",
      "PNG",                     "*.png",
   0, 0};
   
   TGFileInfo fi;
   fi.fFileTypes = kImageExportTypes;
   fi.fIniDir    = StrDup(dir);
   new TGFileDialog(gClient->GetDefaultRoot(), iParent,
                    kFDSave,&fi);
   dir = fi.fIniDir;
   if (fi.fFilename != 0) {
	std::string name = fi.fFilename;
	std::string ext = kImageExportTypes[fi.fFileTypeIdx + 1] + 1;
	if (name.find(ext) == name.npos)
	     name += ext;
	saveImageTo(name);
   }
}

//
// static member functions
//
