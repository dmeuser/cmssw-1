/* 
 * \class PFTauEfficiencyAssociatorFromTH3 
 * \created : Mon Sep 21 17:46:35 PDT 2009 
 * \author Evan K. Friis, (UC Davis)
 *
 * \version $Revision: 1.6 $
 *
 * Implements PFTauEfficiencyAssociator to produce a mapping of efficiencies
 * (parameterizied by pt, eta, and jet widht) stored in a ROOT TH3 histograms
 * to reco::PFTaus
 *
 * $Id: PFTauEfficiencyAssociatorFromTH3.cc,v 1.6 2010/02/03 10:04:16 veelken Exp $
 *
 */

#include "RecoTauTag/TauAnalysisTools/interface/PFTauEfficiencyAssociator.h"
#include "TH3F.h"
#include "TFile.h"
#include <memory>

using namespace std;

class PFTauEfficiencyAssociatorFromTH3 : public PFTauEfficiencyAssociator {
   public:

     explicit PFTauEfficiencyAssociatorFromTH3(const ParameterSet& pset);

     ~PFTauEfficiencyAssociatorFromTH3(){}

     /// get the efficiency for the current tau for source iEff
     virtual pat::LookupTableRecord getEfficiency(size_t iEff);

     /// setup the efficiency sources
     //virtual void setupEfficiencySources(const ParameterSet& effSources, const KineVarPtrs& vars);

     // contains the histogram object, and the functors to retrieve the lookup values for x,y and z
     struct Histogram {
        std::string name;
        std::string location;           // location (in root file/database) of TH3
        TH3F* histogram;  
     };

   private:
     std::vector<Histogram> efficiencies_;
     TFile* file_;
};

PFTauEfficiencyAssociatorFromTH3::PFTauEfficiencyAssociatorFromTH3(const ParameterSet& pset):
   PFTauEfficiencyAssociator(pset)
{
   const ParameterSet& effSources = pset.getParameter<ParameterSet>("efficiencySources");

   typedef std::vector<std::string> vstring;
   const vstring& effNames = efficiencySourceNames();

   std::string filename = effSources.getParameter<std::string>("filename");

   // keep the previous gDirectory state
   TDirectory* old_dir = gDirectory;

   file_ = TFile::Open(filename.c_str(), "READ");
   if(!file_)
   {
      throw cms::Exception("PFTauEfficiencyAssociatorFromFile") << "Can't open ROOT file: " << filename;
   }

   for(vstring::const_iterator iSource = effNames.begin(); iSource != effNames.end(); ++iSource)
   {
      // get the associated pset
      const ParameterSet& sourcePSet = effSources.getParameter<ParameterSet>(*iSource);

      // build the contianer used to hold this efficiency source
      std::string name = *iSource;
      std::string loc = sourcePSet.getParameter<std::string>("location");

      // build our histogram object
      Histogram container;
      container.histogram = dynamic_cast<TH3F*>(file_->Get(loc.c_str()));
      container.name = name;
      container.location = loc;
      if( !(container.histogram) )
      {
	throw cms::Exception("InputFileError") << "can't retrieve histogram " << container.name 
					       << " from location: " << container.location << " in file: " << filename;
      }
      // set the user as the object owner (to prevent deletion when the file is closed)
      container.histogram->SetDirectory(0);
      // store this efficiency source
      efficiencies_.push_back(container);
   }

   // Restore the previous directory state.
   file_->Close();
   old_dir->cd();
}

pat::LookupTableRecord
PFTauEfficiencyAssociatorFromTH3::getEfficiency(size_t iEff)
{
   // get the appropriate efficiency source
   const Histogram& effSource = efficiencies_[iEff];

   // Find the bin corresponding to this point in feature space
   Int_t globalBin = effSource.histogram->FindBin( this->x(), this->y(), this->z());

   double efficiency = effSource.histogram->GetBinContent(globalBin);
   double error = effSource.histogram->GetBinError(globalBin);

   pat::LookupTableRecord output(efficiency, error, globalBin);

   return output;
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PFTauEfficiencyAssociatorFromTH3);
