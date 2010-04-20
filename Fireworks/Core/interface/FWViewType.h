#ifndef Fireworks_Core_FWViewType_h
#define Fireworks_Core_FWViewType_h
// -*- C++ -*-
//
// Package:     Core
// Class  :     FWViewType
// 
/**\class FWViewType FWViewType.h Fireworks/Core/interface/FWViewType.h

 Description: [one line class summary]

 Usage:
    <usage>

*/
//
// Original Author:  Alja Mrak-Tadel
//         Created:  Mon Mar 22 12:08:40 CET 2010
// $Id: FWViewType.h,v 1.6 2010/04/16 11:28:03 amraktad Exp $
//
#include <string>

class  FWViewType
{
public:
   enum EType { k3D, kRhoPhi, kRhoZ, kISpy, kLego, kGlimpse, kSize };

   enum EBit
   {
      k3DBit       = 1 << k3D,
      kRhoPhiBit   = 1 << kRhoPhi,
      kRhoZBit     = 1 << kRhoZ,
      kISpyBit     = 1 << kISpy,
      kLegoBit     = 1 << kLego,
      kGlimpseBit  = 1 << kGlimpse
   };

   // shorcuts
   static const int kAllRPZBits;
   static const int kAll3DBits;

   static  std::string  k3DName;
   static  std::string  kRhoPhiName;
   static  std::string  kRhoZName;
   static  std::string  kISpyName;
   static  std::string  kLegoName;
   static  std::string  kGlimpseName;
   
   const static std::string& idToName(int);

   FWViewType(EType);
   virtual ~FWViewType();

   const  std::string& name() const;
   EType id() const;

private: 
   EType m_id;
};

#endif
