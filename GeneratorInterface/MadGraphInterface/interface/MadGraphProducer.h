#ifndef MadGraph_Producer_h
#define MadGraph_Producer_h
/** \class MadGraphProducer
 *
 * A modified version of the PythiaProducer that generates 
 * events by reading in a file produced with MadGraph/MadEvent
 * and shower them with Pythia. Includes Matrix Element - Parton Shower matching
 * 
 * July 2006
 * Maria Hansen, University of Bristol
 * Hector Naves : added the MCDB Interface (25/10/06)
 * Dorian Kcira : change to new Les Houches format.
 * Dorian Kcira : add automatic end of event processing when hitting
 *                the end of the lhe file (29/01/07)
 * Dorian Kcira : added ME-PS matching (22/05/2007)
 * Carsten Hof  : add saving of flavours and fractional momenta of ingoing partons in pdfinfo (06/02/2008)
 ***************************************/
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <map>
#include <string>
#include "HepMC/GenEvent.h"
#include "HepMC/PdfInfo.h"

namespace CLHEP {
class HepRandomEngine;
}

#include "GeneratorInterface/CommonInterface/interface/TauolaInterface.h"

namespace edm
{
  class MadGraphProducer : public EDFilter {
  public:
    /// Constructor
    MadGraphProducer(const ParameterSet & );
    /// Destructor
    virtual ~MadGraphProducer();
  private:
    /// Interface to the PYGIVE pythia routine, with add'l protections
    bool call_pygive(const std::string& iParm );
    virtual bool beginRun(Run& run, const EventSetup& es);
    virtual bool endRun(Run& run, const EventSetup& es);
    virtual bool filter(Event & e, const EventSetup& es);
    void init();
    void clear();
  private:
    HepMC::GenEvent  *evt;
    HepMC::PdfInfo* pdf_info;
    /// Pythia PYLIST Verbosity flag
    unsigned int pythiaPylistVerbosity_;
    /// HepMC verbosity flag
    bool pythiaHepMCVerbosity_;
    /// Events to print if verbosity
    unsigned int maxEventsToPrint_;    
    unsigned int firstEvent_;
    // Matching & MEMAIN
    double MEMAIN_etaclmax;
    double MEMAIN_qcut;
    unsigned int MEMAIN_iexcfile; // 1 = perform exclusive matching
    bool produceEventTreeFile_;
    // Flag to allow reading a simple LHE file using the minimal Les Houches functionality (ignoring reading parameters from the header)
    bool minimalLH_;
    bool initialized_;
    unsigned int eventNumber_;
    CLHEP::HepRandomEngine* fRandomEngine;
    ParameterSet pset_;

    // external generators (tauola,...)
    bool useExternalGenerators_ ;
    bool useTauola_ ;
    bool useTauolaPolarization_ ;
    TauolaInterface tauola_ ;
  };
} 
#endif
