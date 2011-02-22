#ifndef EcalSimAlgos_EBHitResponse_h
#define EcalSimAlgos_EBHitResponse_h

#include "SimCalorimetry/CaloSimAlgos/interface/CaloHitRespoNew.h"
#include "CondFormats/EcalObjects/interface/EcalIntercalibConstantsMC.h"

/**

 \class EBHitResponse

 \brief Creates electronics signals from EB hits , including APD

*/

class APDSimParameters ;

class EBHitResponse : public CaloHitRespoNew
{
   public:

      typedef CaloHitRespoNew CaloHitResponse ;

      typedef std::vector<double> VecD ;

      enum { kNOffsets = 2000 } ;

      EBHitResponse( const CaloVSimParameterMap* parameterMap , 
		     const CaloVShape*           shape        ,
		     bool                        apdOnly      ,
		     const APDSimParameters*     apdPars      , 
		     const CaloVShape*           apdShape       ) ;

      virtual ~EBHitResponse() ;

      virtual bool keepBlank() const { return false ; }

      void setIntercal( const EcalIntercalibConstantsMC* ical ) ;

      const VecD& offsets() const { return m_timeOffVec ; }

   protected:

      virtual void putAnalogSignal( const PCaloHit & inputHit ) ;

   private:

      const double nonlFunc( double enr ) const {
	 return ( pene > enr ? nonlFunc1( enr ) : 
		  pfac*atan( enr - pene ) + poff ) ; }

      const double nonlFunc1( double enr ) const {
	 const double enr2 ( enr*enr ) ;
	 const double enr3 ( enr2*enr ) ;
	 return ( pcub*enr3 + pqua*enr2 + plin*enr + pcon ) ; }

      const APDSimParameters* apdParameters() const ;
      const CaloVShape*       apdShape()      const ;

      double apdSignalAmplitude( const PCaloHit& hit ) const ;

      void findIntercalibConstant( const DetId& detId, 
				   double&      icalconst ) const ;

      const bool                       m_apdOnly  ;
      const APDSimParameters*          m_apdPars  ;
      const CaloVShape*                m_apdShape ;
      const EcalIntercalibConstantsMC* m_intercal ;

      std::vector<double> m_timeOffVec ;

      const double pcub, pqua, plin, pcon, pene, pasy, poff, pfac ;
};
#endif


