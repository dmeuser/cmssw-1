//
// ********************************************************************
// 25.04.04 - M.Case ported algorithm from G4VDivisionParameterisation.cc. to 
//            DDD version
// ********************************************************************

#include "DDDividedGeometryObject.h"

#include "DetectorDescription/Base/interface/DDRotationMatrix.h"
#include "DetectorDescription/Base/interface/DDTranslation.h"
#include "DetectorDescription/Base/interface/DDException.h"
#include "DetectorDescription/Base/interface/DDdebug.h"

#include "DetectorDescription/Core/interface/DDAxes.h"
#include "DetectorDescription/Core/interface/DDDivision.h"
#include "DetectorDescription/Core/interface/DDLogicalPart.h"
#include "DetectorDescription/Core/interface/DDPosPart.h"
#include "DetectorDescription/Core/interface/DDSolid.h"
#include "DetectorDescription/Core/interface/DDSolidShapes.h"


//--------------------------------------------------------------------------
DDDividedGeometryObject::DDDividedGeometryObject( const DDDivision & div ) 
  : div_(div), ftype_(), compNDiv_(div.nReplicas()), compWidth_(div.width())
    , divisionType_(DivNDIVandWIDTH), theVoluFirstCopyNo_(1)
{

  if (div_.nReplicas() == 0 || div_.width() < tolerance())
    {
      if (div_.width() < tolerance())
	divisionType_ = DivNDIV;
      else 
	divisionType_ = DivWIDTH;
    } 
  DCOUT_V('P', " DDDividedGeometryObject Divisions " << div_ << std::endl);
}

//--------------------------------------------------------------------------
DDDividedGeometryObject::~DDDividedGeometryObject()
{
}

//--------------------------------------------------------------------------
DDRotationMatrix* DDDividedGeometryObject::changeRotMatrix( double rotZ ) const
{
  DDRotationMatrix * rm = new DDRotationMatrix();
  rm->rotateZ( rotZ );
  return rm;
}

//--------------------------------------------------------------------------
int
DDDividedGeometryObject::
calculateNDiv( double motherDim, double width, double offset ) const
{
  DCOUT_V('P', " DDDividedGeometryObject::calculateNDiv: " << ( motherDim - offset ) / width << " Motherdim: " <<  motherDim << ", Offset: " << offset << ", Width: " << width << std::endl);
  return int( ( motherDim - offset ) / width );
}

//--------------------------------------------------------------------------
double
DDDividedGeometryObject::
calculateWidth( double motherDim, int nDiv, double offset ) const
{ 
  DCOUT_V('P', " DDDividedGeometryObject::calculateWidth: " << ( motherDim - offset ) / nDiv << ", Motherdim: " << motherDim << ", Offset: " << offset << ", Number of divisions: " << nDiv << std::endl);

  return ( motherDim - offset ) / nDiv;
}

//--------------------------------------------------------------------------
void DDDividedGeometryObject::checkParametersValidity()
{
  double maxPar = getMaxParameter();
  checkOffset( maxPar );
  checkNDivAndWidth( maxPar );
  if (!div_.parent().isDefined().second) {
    std::string s = "DDDividedGeometryObject::checkParametersValidity() :";
    s+= "\n ERROR - the LogicalPart of the parent must be ";
    s+= "\n         defined before a division can occur.";
    throw DDException(s);
  }

}

//--------------------------------------------------------------------------
void DDDividedGeometryObject::checkOffset( double maxPar )
{
  if( div_.offset() >= maxPar )
  {
    DCOUT_V('P', "DDDividedGeometryObject::checkOffset() Division of LogicalPart " << div_.parent() << " offset=" << div_.offset() << " maxPar=" << maxPar << "\n");
    std::string s = "DDDividedGeometryObject::checkOffset() IllegalConstruct";
    s += "\nERROR - DDDividedGeometryObject::checkOffset()";
    s += "\n        failed.";
    s += "  Too big an offset.";
    throw DDException(s);
  }
}

//--------------------------------------------------------------------------
void DDDividedGeometryObject::checkNDivAndWidth( double maxPar )
{
  if( (divisionType_ == DivNDIVandWIDTH)
      && (div_.offset() + compWidth_*compNDiv_ - maxPar > tolerance() ) )
  {
    std::string s = "ERROR - DDDividedGeometryObject::checkNDivAndWidth()";
    s+= "\n        Division of LogicalPart " + div_.parent();
    s+= " has too big an offset.";
    DCOUT_V('P', "DDDividedGeometryObject::checkNDivAndWidth has computed div_.offset() + compWidth_*compNDiv_ - maxPar =" << div_.offset() + compWidth_*compNDiv_ - maxPar << " and tolerance()=" << tolerance());
    std::cout << compWidth_ << std::endl;
    throw DDException(s);
  }
}

const double DDDividedGeometryObject::tolerance()
{
  // this can come from some global tolerance if you want.
  static const double tol = 1.0/1000.00;
  return tol;
}

void DDDividedGeometryObject::setType( const std::string& s) 
{
  ftype_ = s;
}

const std::string& DDDividedGeometryObject::getType() const
{
  return ftype_;
}

void DDDividedGeometryObject::execute()
{
  DCOUT_V('D', "about to make " <<  compNDiv_ << " divisions." << std::endl);
  for (int i = theVoluFirstCopyNo_; i < compNDiv_+theVoluFirstCopyNo_; ++i)
    {
      DCOUT_V('D',  "Parent Volume: " << div_.parent() << std::endl);
      DCOUT_V('D',  "Child Volume: " << makeDDLogicalPart(i) << std::endl);
      DCOUT_V('D',  "   copyNo:" << i << std::endl);
      DCOUT_V('D',  "   Translation: " << makeDDTranslation(i) << std::endl);
      DCOUT_V('D',  "   rotation=" << makeDDRotation(i) << std::endl);

      DDpos( makeDDLogicalPart(i)
	     , div_.parent()
	     , i
	     , makeDDTranslation(i)
	     , makeDDRotation(i)
	     , &div_
	     );
    }
}

//------------------------------------------------------------------------
double DDDividedGeometryObject::getMaxParameter() const
{
  return 0.0;
}

//------------------------------------------------------------------------
DDRotation DDDividedGeometryObject::makeDDRotation( const int copyNo ) const
{
  return DDRotation();
}

//------------------------------------------------------------------------
DDTranslation DDDividedGeometryObject::makeDDTranslation( const int copyNo ) const
{
  return DDTranslation();
}

//------------------------------------------------------------------------
DDLogicalPart DDDividedGeometryObject::makeDDLogicalPart( const int copyNo ) const
{
  // just return the parent... this is USELESS
  return div_.parent();
}
