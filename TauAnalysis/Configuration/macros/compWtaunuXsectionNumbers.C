
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <iomanip>

#include <TMath.h>

double square(double x)
{
  return x*x;
}

void compWtaunuXsectionNumbers()
{
  //-------------------------------------------------------------------------------
  // define number of observed events
  //-------------------------------------------------------------------------------

  double numEventsObserved = 793;

  std::cout << "Nr Events observed in Data = " << numEventsObserved << std::endl;

  //-------------------------------------------------------------------------------
  // Estimate the number of QCD events as an average of ABCD and template methods
  //-------------------------------------------------------------------------------

  double numEventsExpected_qcd_template = 160; //Nr of bg events predicted by the template fit
  double errEventsExpected_qcd_template =  30; //Error of bg events predicted by the template fit
  // CV: subtract statistical component of uncertainty on QCD background estimate
  //     returned by RooFit/template fit
  errEventsExpected_qcd_template = TMath::Sqrt(square(errEventsExpected_qcd_template) - numEventsExpected_qcd_template);
  cout<<"err events expected QCD template = "<<errEventsExpected_qcd_template<<endl;  
  double numEventsExpected_qcd_ABCD     = 203; //ABCD method
  double errEventsExpected_qcd_ABCD     =  26;

  double weight_qcd_template            = 1./square(errEventsExpected_qcd_template);
  double weight_qcd_ABCD                = 1./square(errEventsExpected_qcd_ABCD);

  double numEventsExpected_qcd          = 
    (weight_qcd_template*numEventsExpected_qcd_template + weight_qcd_ABCD*numEventsExpected_qcd_ABCD)/
    (weight_qcd_template + weight_qcd_ABCD);
  double errEventsExpected_qcd          =  
    TMath::Sqrt(1./(square(1./errEventsExpected_qcd_template) + square(1./errEventsExpected_qcd_ABCD)));

  //----------------------------------------------------------------------------
  // assign 40% uncertainty on EWK backgrounds
  //----------------------------------------------------------------------------

  double numEventsExpected_Wenu    = 50.1; //update to NNLO
  double errEventsExpected_Wenu    = 0.24*numEventsExpected_Wenu;
  double numEventsExpected_Wmunu   =  4.9; //update to NNLO
  double errEventsExpected_Wmunu   = 0.35*numEventsExpected_Wmunu; //0.75 uncertainty for muons faking taus and 0.2 uncertainty for jets faking taus
  double numEventsExpected_Ztautau = 54.2; //update to average of D6T and Z2T
  //  double errEventsExpectedTauJetEn_Ztautau =
  double errEventsExpected_Ztautau = 0.3*numEventsExpected_Ztautau; // tau + jet energy scale uncertainty + trigger uncertainty


  double numEventsExpected_ewk     = numEventsExpected_Wenu + numEventsExpected_Wmunu + numEventsExpected_Ztautau;
  double errEventsExpected_ewk     = TMath::Sqrt(square(errEventsExpected_Wenu) + square(errEventsExpected_Wmunu) + square(errEventsExpected_Ztautau));

  //----------------------------------------------------------------------------
  // Sum Bgr = ewk + qcd
  //---------------------------------------------------------------------------

  double numEventsExpected_Bgr     = numEventsExpected_qcd + numEventsExpected_ewk;
  double errEventsExpected_Bgr     = TMath::Sqrt(square(errEventsExpected_qcd) + square(errEventsExpected_ewk));

  //-----------------------------------------------------------------------------
  // relative uncertainties
  //------------------------------------------------------------------------------

  double errEventsExpectedRel_ewk = errEventsExpected_ewk/numEventsExpected_ewk;
  double errEventsExpectedRel_qcd = errEventsExpected_qcd/numEventsExpected_qcd;
  double errEventsExpectedRel_Bgr = errEventsExpected_Bgr/numEventsExpected_Bgr;
  
  std::cout << "Nr QCD = " << numEventsExpected_qcd 
	    << " +/- " << errEventsExpected_qcd << " (" << errEventsExpectedRel_qcd << "%)" << std::endl;
  std::cout << "Nr EWK = " << numEventsExpected_ewk 
	    << " +/- " << errEventsExpected_ewk << " (" << errEventsExpectedRel_ewk << "%)" << std::endl;
  std::cout << "Nr Bgr = "<< numEventsExpected_Bgr 
	    << " +/- " << errEventsExpected_Bgr << " (" << errEventsExpectedRel_Bgr << "%)" << std::endl;
	
 //----------------------------------------------------------------------------
 // Number of signal events
 //---------------------------------------------------------------------------

  double numSignal = numEventsObserved-numEventsExpected_Bgr;
  double errSignal = errEventsExpected_Bgr;
  double errSignalRel = errSignal/numSignal;

  std::cout << "Nr Sig = " << numSignal 
	    << " +/- " << errSignal << " (" << errSignalRel << "%)" << std::endl;
  
  //-------------------------------------------------------------------------------
  // define signal efficiency and acceptance
  //-------------------------------------------------------------------------------

  double intLumiData       = 32.35; // pb^-1
  
  double mcAcceptance_Sig  = 0.0790;
  double mcEfficiency_Sig  = 0.0334;
  double brToHadTau_Sig    = 0.648;
  
  //-------------------------------------------------------------------------------
  // define systematic uncertainties
  //
  // NOTE: all uncertainties are **relative** to central value
  //-------------------------------------------------------------------------------

  double errTriggerEff     = 0.15;

  double errTauIdEff       = 0.07;

  double errTauEnScaleUp   = 0.160; //powheg
  double errTauEnScaleDown = 0.148;
  double errJetEnScaleUp   = 0.101;
  double errJetEnScaleDown = 0.100;

  double errIntLumiData    = 0.04;
  
  double errAcc            = 0.02;

  //-------------------------------------------------------------------------------
  // compute W --> tau nu cross-section "central value" 
  //-------------------------------------------------------------------------------

  // CV: convert luminosity from pb^-1 to nb^-1 
  //    (--> cross-section from pb to nb)
  double denominator = (intLumiData*1.e+3*brToHadTau_Sig*mcAcceptance_Sig*mcEfficiency_Sig);
  cout<<"Signal efficiency tot = "<<brToHadTau_Sig*mcAcceptance_Sig*mcEfficiency_Sig<<endl;
 
  double Xsection = numSignal/denominator;
  std::cout << "measured Xsection = " << Xsection << " nb" << std::endl;

  double expXsection = 10.44; // nb; computed with FEWZ @ NNLO using MSTW 2008 PDF
                              // ( cf. https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections )
  std::cout << "Theory prediction = " << expXsection << " nb" << std::endl;
  
  //-------------------------------------------------------------------------------
  // compute error estimation on cross section
  //-------------------------------------------------------------------------------
   
   double errSysJetpTauJetUp2    = square(errTauEnScaleUp) + square(errJetEnScaleDown);	
   double errSysJetpTauJetDown2  = square(errTauEnScaleDown) + square(errJetEnScaleUp);
   std::cout << "Sys. error from tau+jet energy scale = " 
	     << " + " << TMath::Sqrt(errSysJetpTauJetUp2) << " - " << TMath::Sqrt(errSysJetpTauJetDown2) << std::endl;

   double errEffAccUp2           = errSysJetpTauJetUp2   + square(errTriggerEff) + square(errAcc) + square(errTauIdEff);
   double errEffAccDown2         = errSysJetpTauJetDown2 + square(errTriggerEff) + square(errAcc) + square(errTauIdEff);

   double sysUncertaintyTotUp2   = errEffAccUp2   + square(errSignalRel);
   double sysUncertaintyTotDown2 = errEffAccDown2 + square(errSignalRel);
   cout<< "Sys. error total = "<<"+"<<TMath::Sqrt(sysUncertaintyTotUp2)<<"-"<<TMath::Sqrt(sysUncertaintyTotDown2)<<endl;
   
   //------------------------------------------------------------------------------
   // summarize error estimates
   //----------------------------------------------------------------------------

   double statUncertainty = TMath::Sqrt(numEventsObserved)/denominator; 
   std::cout << "statUncertainty = " 
	     << statUncertainty << " nb" << std::endl;

   std::cout << "sysUncertainty = " 
	     << " + " << TMath::Sqrt(sysUncertaintyTotUp2)*Xsection 
	     << " - " << TMath::Sqrt(sysUncertaintyTotDown2)*Xsection << " nb" << std::endl;

   std::cout << "intLumiDataUncertainty = " << errIntLumiData*Xsection << " nb" << std::endl;

   std::cout << "Data/Theory = " << Xsection/expXsection 
	     << " + " << TMath::Sqrt(0.//square(statUncertainty) 
				    + (sysUncertaintyTotUp2   + square(errIntLumiData))*square(Xsection))/expXsection
	     << " - " << TMath::Sqrt(0.//square(statUncertainty) 
				    + (sysUncertaintyTotDown2 + square(errIntLumiData))*square(Xsection))/expXsection 
	     << std::endl;
}
