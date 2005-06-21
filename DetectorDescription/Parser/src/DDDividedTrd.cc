//
// ********************************************************************
// 25.04.04 - M. Case ddd-ize G4ParameterisationTrd*
// ********************************************************************

#include "CLHEP/Units/SystemOfUnits.h"

#include "DetectorDescription/DDParser/interface/DDDividedTrd.h"
#include "DetectorDescription/DDParser/interface/DDXMLElement.h"

#include "DetectorDescription/DDCore/interface/DDLogicalPart.h"
#include "DetectorDescription/DDCore/interface/DDName.h"
#include "DetectorDescription/DDCore/interface/DDPosPart.h"
#include "DetectorDescription/DDCore/interface/DDAxes.h"
#include "DetectorDescription/DDCore/interface/DDSolid.h"
#include "DetectorDescription/DDCore/interface/DDSolidShapes.h"
#include "DetectorDescription/DDCore/interface/DDMaterial.h"

#include "DetectorDescription/DDBase/interface/DDdebug.h"
#include "DetectorDescription/DDBase/interface/DDException.h"
#include "DetectorDescription/DDBase/interface/DDRotationMatrix.h"
#include "DetectorDescription/DDBase/interface/DDTranslation.h"

//------------------------------------------------------------------------
DDDividedTrdX::DDDividedTrdX ( const DDDivision & div )
  :  DDDividedGeometryObject ( div )
{
  checkParametersValidity();
  setType( "DivisionTrdX" );
  DDTrap mtrd = (DDTrap)( div_.parent().solid() );

  if ( divisionType_ == DivWIDTH )
  {
    compNDiv_ = calculateNDiv( 2 * mtrd.x1(), div_.width(), div_.offset() );
  }
  else if( divisionType_ == DivNDIV )
  {
    compWidth_ = calculateWidth( 2*mtrd.x1(), div_.nReplicas(), div_.offset() );
  }

  DCOUT_V ('P', " DDDividedTrdX - ## divisions " << compNDiv_ << " = " << div_.nReplicas() << "\n Offset " << div_.offset() << "\n Width " << compWidth_ << " = " << div_.width());
}

//------------------------------------------------------------------------
DDDividedTrdX::~DDDividedTrdX()
{
}

//------------------------------------------------------------------------
double DDDividedTrdX::getMaxParameter() const
{
  DDTrap mtrd = (DDTrap)(div_.parent().solid());
  return 2 * mtrd.x1();
}

//------------------------------------------------------------------------
DDTranslation DDDividedTrdX::makeDDTranslation ( const int copyNo ) const
{
  DDTrap mtrd = (DDTrap)(div_.parent().solid());
  double mdx = mtrd.x1();


  //----- translation 
  double posi = -mdx + div_.offset() + (copyNo+0.5)*compWidth_;

  DCOUT_V ('P', " DDDividedTrdX: " << copyNo << "\n Position: x=" << posi << "  Axis= " << DDAxesNames::name(div_.axis()) << "\n");

  if( div_.axis() == x )
  {
    return DDTranslation(posi, 0.0, 0.0);
  }
  else
  { 
    string s = "ERROR - DDDividedTrdX::makeDDTranslation()";
    s += "\n        Axis is along ";
    s += DDAxesNames::name(div_.axis());
    s += " !\n" ;
    s += "DDDividedTrdX::makeDDTranslation()";
    s += " IllegalConstruct: Only axes along x are allowed !";
    throw DDException(s);
  }
  
  return DDTranslation();
}

//------------------------------------------------------------------------
DDRotation DDDividedTrdX::makeDDRotation ( const int copyNo ) const
{
  return DDRotation();
}

//--------------------------------------------------------------------------
DDLogicalPart DDDividedTrdX::makeDDLogicalPart( const int copyNo ) const
{
  DDTrap mtrd = (DDTrap)(div_.parent().solid());
  DDMaterial usemat = div_.parent().material();

  double pDy1 = mtrd.y1(); //GetYHalfLength1();
  double pDy2 = mtrd.y2(); //->GetYHalfLength2();
  double pDz = mtrd.halfZ(); //->GetZHalfLength();
  double pDx = compWidth_/2.;
 
  //trd.SetAllParameters ( pDx, pDx, pDy1, pDy2, pDz );

  DDName solname(div_.parent().ddname().name() + "_DIVCHILD" 
		 , div_.parent().ddname().ns());
  DDSolid dsol(solname);
  DDLogicalPart ddlp(solname);
  if (!dsol.isDefined().second)
    {
      dsol = DDSolidFactory::trap(solname
				  , pDz
				  , 0.*deg
				  , 0.*deg
				  , pDy1
				  , pDx
				  , pDx
				  , 0.*deg
				  , pDy2
				  , pDx
				  , pDx
				  , 0.*deg);
      ddlp = DDLogicalPart(solname, usemat, dsol);
    }
  DCOUT_V ('P', "DDDividedTrdX::makeDDLogicalPart lp = " << ddlp);
  return ddlp;
}

//--------------------------------------------------------------------------
void DDDividedTrdX::checkParametersValidity()
{
  DDDividedGeometryObject::checkParametersValidity();
  
  DDTrap mtrd = (DDTrap)(div_.parent().solid());

  double mpDx1 = mtrd.x1(); //->GetXHalfLength1();
  double mpDx2 = mtrd.x2(); //->GetXHalfLength2();
  double mpDx3 = mtrd.x3(); 
  double mpDx4 = mtrd.x4();
  double mpTheta = mtrd.theta();
  double mpPhi = mtrd.phi();
  double mpAlpha1 = mtrd.alpha1();  
  double mpAlpha2 = mtrd.alpha2();
//    double mpDy1 = mtrd.y1();
//    double mpDy2 = mtrd.y2();

  if ( abs(mpDx1 - mpDx2) > tolerance()  || abs(mpDx3 - mpDx4) > tolerance()
       || abs(mpDx1 - mpDx4) > tolerance())
    {
      string s = "ERROR - DDDividedTrdX::checkParametersValidity()";
      s+= "\n        Making a division of a TRD along axis X,";
      s+= "\n        while the X half lengths are not equal,";
      s+= "\n        is not (yet) supported. It will result";
      s+= "\n        in non-equal division solids.";
      throw DDException(s);
    }
//    if (abs(mpDy1 - mpDy2) > tolerance())
//      {
//        string s = "ERROR - DDDividedTrdX::checkParametersValidity()";
//        s+= "\n        Making a division of a TRD along axis X,";
//        s+= "\n        while the Y half lengths are not equal,";
//        s+= "\n        is not (yet) supported. It will result";
//        s+= "\n        in non-equal division solids.";
//        throw DDException(s);
//      }
  // mec:  we only have traps, not trds in DDD, so I added this check
  // to make sure it is only a trd (I think! :-))
  if (mpAlpha1 != 0.*deg || mpAlpha2 != 0.*deg || mpTheta != 0.*deg || mpPhi != 0.*deg)
    {
      string s = "ERROR - DDDividedTrdX::checkParametersValidity()";
      s+= "\n        Making a division of a TRD along axis X,";
      s+= "\n        while the theta, phi and aplhpa2 are not zero,";
      s+= "\n        is not (yet) supported. It will result";
      s+= "\n        in non-equal division solids.";
      throw DDException(s);
    }

}

//--------------------------------------------------------------------------
DDDividedTrdY::DDDividedTrdY ( const DDDivision & div )
  : DDDividedGeometryObject( div )
{
  checkParametersValidity();
  setType( "DivisionTrdY" );
  DDTrap mtrd = (DDTrap)(div_.parent().solid());

  if( divisionType_ == DivWIDTH )
    {
      compNDiv_ = calculateNDiv( 2 * mtrd.y1(), div_.width(), div_.offset() );
    }
  else if( divisionType_ == DivNDIV )
    {
      compWidth_ = calculateWidth( 2 * mtrd.y1(), div_.nReplicas(), div_.offset() );
    }

  DCOUT_V ('P', " DDDividedTrdY no divisions " << compNDiv_ << " = " << div_.nReplicas() << "\n Offset " << div_.offset() << "\n width " << compWidth_ << " = " << div_.width() << endl);
  
}

//------------------------------------------------------------------------
DDDividedTrdY::~DDDividedTrdY()
{
}

//------------------------------------------------------------------------
double DDDividedTrdY::getMaxParameter() const
{
  DDTrap mtrd = (DDTrap)(div_.parent().solid());
  return 2 * mtrd.y1(); 
}

//--------------------------------------------------------------------------
DDTranslation DDDividedTrdY::makeDDTranslation( const int copyNo ) const
{
  DDTrap mtrd = (DDTrap)(div_.parent().solid() );
  double mdy = mtrd.y1();

  //----- translation 
  double posi = -mdy + div_.offset() + (copyNo+0.5)*compWidth_;

  DCOUT_V ('P', " DDDividedTrdY: " << copyNo << "\n Position: y=" << posi << "  Axis= " << DDAxesNames::name(div_.axis()) << "\n");

  if( div_.axis() == y )
  {
    return DDTranslation(0.0, posi, 0.0);
  }
  else
  { 
    string s = "ERROR - DDDividedTrdY::makeDDTranslation()";
    s += "\n        Axis is along ";
    s += DDAxesNames::name(div_.axis());
    s += " !\n" ;
    s += "DDDividedTrdY::makeDDTranslation()";
    s += " IllegalConstruct: Only axes along y are allowed !";
    throw DDException(s);
  }
  return DDTranslation();
}

//------------------------------------------------------------------------
DDRotation DDDividedTrdY::makeDDRotation ( const int copyNo ) const
{
  return DDRotation();
}

//--------------------------------------------------------------------------
DDLogicalPart DDDividedTrdY::makeDDLogicalPart( const int copyNo ) const
{
  //---- The division along Y of a Trd will result a Trd, only 
  //--- if Y at -Z and +Z are equal, else use the G4Trap version
  DDTrap mtrd = (DDTrap)(div_.parent().solid());
  DDMaterial usemat = div_.parent().material();
  
  double pDx1 = mtrd.x1(); //->GetXHalfLength1() at Y+;
  double pDx2 = mtrd.x2(); //->GetXHalfLength2() at Y+;
  double pDx3 = mtrd.x3(); //->GetXHalfLength1() at Y-;
  double pDx4 = mtrd.x4(); //->GetXHalfLength2() at Y-;
  double pDz = mtrd.halfZ(); //->GetZHalfLength();
  double pDy = compWidth_/2.;
 
  //trd.SetAllParameters ( pDx1, pDx2, pDy, pDy, pDz );
  DDName solname(div_.name() );
  DDSolid  dsol(solname);
  DDLogicalPart ddlp(solname);
  if (!dsol.isDefined().second)
    {
      dsol = DDSolidFactory::trap(solname
				  , pDz
				  , 0.*deg
				  , 0.*deg
				  , pDy
				  , pDx1
				  , pDx2
				  , 0*deg
				  , pDy
				  , pDx3
				  , pDx4
				  , 0.*deg);
      DDLogicalPart ddlp(solname,  usemat, dsol);
    }
  DCOUT_V ('P', "DDDividedTrdY::makeDDLogicalPart lp = " << ddlp);
  return ddlp;
}

//--------------------------------------------------------------------------
void DDDividedTrdY::checkParametersValidity()
{
  DDDividedGeometryObject::checkParametersValidity();

  DDTrap mtrd = (DDTrap)(div_.parent().solid());

  double mpDy1 = mtrd.y1(); //->GetYHalfLength1();
  double mpDy2 = mtrd.y2(); //->GetYHalfLength2();
  double mpTheta = mtrd.theta();
  double mpPhi = mtrd.phi();
  double mpAlpha1 = mtrd.alpha1();
  double mpAlpha2 = mtrd.alpha2();

  if( abs(mpDy1 - mpDy2) > tolerance() )
  {
    string s= "ERROR - DDDividedTrdY::checkParametersValidity()";
    s += "\n        Making a division of a TRD along axis Y while";
    s += "\n        the Y half lengths are not equal is not (yet)";
    s += "\n        supported. It will result in non-equal";
    s += "\n        division solids.";
    throw DDException(s);
  }
  // mec:  we only have traps, not trds in DDD, so I added this check
  // to make sure it is only a trd (I think! :-))
  if (mpAlpha1 != 0.*deg || mpAlpha2 != 0.*deg || mpTheta != 0.*deg || mpPhi != 0.*deg)
    {
      string s = "ERROR - DDDividedTrdY::checkParametersValidity()";
      s+= "\n        Making a division of a TRD along axis X,";
      s+= "\n        while the theta, phi and aplhpa2 are not zero,";
      s+= "\n        is not (yet) supported. It will result";
      s+= "\n        in non-equal division solids.";
      throw DDException(s);
    }
}

//--------------------------------------------------------------------------
DDDividedTrdZ::DDDividedTrdZ( const DDDivision & div )
  : DDDividedGeometryObject ( div )
{ 
  checkParametersValidity();
  setType( "DivTrdZ" );
  DDTrap mtrd = (DDTrap)(div_.parent().solid());

  if ( divisionType_ == DivWIDTH )
    {
      compNDiv_ = calculateNDiv( 2*mtrd.halfZ(), div_.width(), div_.offset() );
    }
  else if( divisionType_ == DivNDIV )
  {
    compWidth_ = calculateWidth( 2*mtrd.halfZ(), div_.nReplicas(), div_.offset() );
  }
  DCOUT_V ('P', " DDDividedTrdY no divisions " << compNDiv_ << " = " << div_.nReplicas() << "\n Offset " << div_.offset() << "\n width " << compWidth_ << " = " << div_.width() << endl);
}

//------------------------------------------------------------------------
DDDividedTrdZ::~DDDividedTrdZ()
{
}

//------------------------------------------------------------------------
double DDDividedTrdZ::getMaxParameter() const
{
  DDTrap mtrd = (DDTrap)(div_.parent().solid());
  return 2 * mtrd.halfZ();
}

//--------------------------------------------------------------------------
DDTranslation DDDividedTrdZ::makeDDTranslation ( const int copyNo ) const
{
  DDTrap mtrd = (DDTrap)(div_.parent().solid() );
  double mdz = mtrd.halfZ();

  //----- translation 
  double posi = -mdz + div_.offset() + (copyNo+0.5)*compWidth_;

  DCOUT_V ('P', " DDDividedTrdZ: " << copyNo << "\n Position: z=" << posi << "  Axis= " << DDAxesNames::name(div_.axis()) << "\n");

  if( div_.axis() == z )
  {
    return DDTranslation(0.0, 0.0, posi);
  }
  else
  { 
    string s = "ERROR - DDDividedTrdZ::makeDDTranslation()";
    s += "\n        Axis is along ";
    s += DDAxesNames::name(div_.axis());
    s += " !\n" ;
    s += "DDDividedTrdY::makeDDTranslation()";
    s += " IllegalConstruct: Only axes along z are allowed !";
    throw DDException(s);

  }
  return DDTranslation();
}

//------------------------------------------------------------------------
DDRotation DDDividedTrdZ::makeDDRotation ( const int copyNo ) const
{
  return DDRotation();
}

//--------------------------------------------------------------------------
DDLogicalPart DDDividedTrdZ::makeDDLogicalPart ( const int copyNo ) const
{
  //---- The division along Z of a Trd will result a Trd
  DDTrap mtrd = (DDTrap)(div_.parent().solid());
  DDMaterial usemat = div_.parent().material();

  double pDx1 = mtrd.x1(); //->GetXHalfLength1();
  //(mtrd->GetXHalfLength2() - mtrd->GetXHalfLength1() );
  double DDx = (mtrd.x2() - mtrd.x1() );
  double pDy1 = mtrd.y1(); // ->GetYHalfLength1();
  //(mtrd->GetYHalfLength2() - mtrd->GetYHalfLength1() );
  double DDy = (mtrd.y2() - mtrd.y1() );
  double pDz = compWidth_/2.;
  double zLength = 2*mtrd.halfZ(); //->GetZHalfLength();
 
//    trd.SetAllParameters ( pDx1+DDx*(div_.offset()+copyNo*compWidth_)/zLength,
//                           pDx1+DDx*(div_.offset()+(copyNo+1)*compWidth_)/zLength, 
//                           pDy1+DDy*(div_.offset()+copyNo*compWidth_)/zLength,
//                           pDy1+DDy*(div_.offset()+(copyNo+1)*compWidth_)/zLength, pDz );

  DDName solname(div_.parent().ddname().name() + "_DIVCHILD" 
		 + DDXMLElement::itostr(copyNo)
		 , div_.parent().ddname().ns());
  DDSolid  dsol = 
    DDSolidFactory::trap(solname
			 , pDz
			 , 0.*deg
			 , 0.*deg
			 , pDy1+DDy*(div_.offset()+copyNo*compWidth_)/zLength
			 , pDx1+DDx*(div_.offset()+copyNo*compWidth_)/zLength
			 , pDx1+DDx*(div_.offset()+copyNo*compWidth_)/zLength
			 , 0.*deg
			 , pDy1+DDy*(div_.offset()+(copyNo+1)*compWidth_)/zLength
			 , pDx1+DDx*(div_.offset()+(copyNo+1)*compWidth_)/zLength
			 , pDx1+DDx*(div_.offset()+(copyNo+1)*compWidth_)/zLength
			 , 0*deg
			 );

  DDLogicalPart ddlp(solname, usemat, dsol);
  DCOUT_V ('P', "DDDividedTrdZ::makeDDLogicalPart lp = " << ddlp);
  return ddlp;
}

//--------------------------------------------------------------------------
void DDDividedTrdZ::checkParametersValidity()
{
  DDDividedGeometryObject::checkParametersValidity();

  DDTrap mtrd = (DDTrap)(div_.parent().solid());

  double mpTheta = mtrd.theta();
  double mpPhi = mtrd.phi();
  double mpAlpha1 = mtrd.alpha1();
  double mpAlpha2 = mtrd.alpha2();

  // mec:  we only have traps, not trds in DDD, so I added this check
  // to make sure it is only a trd (I think! :-))
  if (mpAlpha1 != 0.*deg || mpAlpha2 != 0.*deg || mpTheta != 0.*deg || mpPhi != 0.*deg)
    {
      string s = "ERROR - DDDividedTrdZ::checkParametersValidity()";
      s+= "\n        Making a division of a TRD along axis X,";
      s+= "\n        while the theta, phi and aplhpa2 are not zero,";
      s+= "\n        is not (yet) supported. It will result";
      s+= "\n        in non-equal division solids.";
      throw DDException(s);
    }
}
