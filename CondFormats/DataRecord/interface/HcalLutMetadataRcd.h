#ifndef DataRecord_HcalLutMetadataRcd_h
#define DataRecord_HcalLutMetadataRcd_h
// -*- C++ -*-
//
// Package:     DataRecord
// Class  :     HcalLutMetadataRcd
// 
/**\class HcalLutMetadataRcd HcalLutMetadataRcd.h CondFormats/DataRecord/interface/HcalLutMetadataRcd.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      
// Created:     Sat Mar  1 15:49:28 CET 2008
// $Id: HcalLutMetadataRcd.h,v 1.1 2009/09/18 14:25:15 kukartse Exp $
//

#include "FWCore/Framework/interface/DependentRecordImplementation.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

class HcalLutMetadataRcd : public edm::eventsetup::DependentRecordImplementation<HcalLutMetadataRcd, boost::mpl::vector<IdealGeometryRecord> > {};

#endif
