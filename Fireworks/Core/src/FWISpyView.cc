// -*- C++ -*-
//
// Package:     cmsShow36
// Class  :     FWISpyView
// 
// Implementation:
//     [Notes on implementation]
//
// Original Author:  Alja Mrak-Tadel 
//         Created:  Wed Apr  7 14:40:31 CEST 2010
// $Id: FWISpyView.cc,v 1.1 2010/04/12 10:09:17 amraktad Exp $
//

// system include files

// user include files
#include "Fireworks/Core/interface/FWISpyView.h"


//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
FWISpyView::FWISpyView(TEveWindowSlot* slot, FWViewType::EType typeId):
   FW3DViewBase(slot, typeId)
{
}

// FWISpyView::FWISpyView(const FWISpyView& rhs)
// {
//    // do actual copying here;
// }

FWISpyView::~FWISpyView()
{
}

//
// assignment operators
//
// const FWISpyView& FWISpyView::operator=(const FWISpyView& rhs)
// {
//   //An exception safe implementation is
//   FWISpyView temp(rhs);
//   swap(rhs);
//
//   return *this;
// }

//
// member functions
//

//
// const member functions
//

//
// static member functions
//
