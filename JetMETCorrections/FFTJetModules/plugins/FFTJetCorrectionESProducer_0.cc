#include "JetMETCorrections/FFTJetModules/plugins/FFTJetCorrectionESProducer.h"

//
// define this as a plug-in
//
typedef FFTJetCorrectionESProducer<fftcorrtypes::BasicJet> FFTBasicJetCorrectionESProducer;
typedef FFTJetCorrectionESProducer<fftcorrtypes::GenJet>   FFTGenJetCorrectionESProducer;
typedef FFTJetCorrectionESProducer<fftcorrtypes::CaloJet>  FFTCaloJetCorrectionESProducer;
typedef FFTJetCorrectionESProducer<fftcorrtypes::PFJet>    FFTPFJetCorrectionESProducer;
typedef FFTJetCorrectionESProducer<fftcorrtypes::TrackJet> FFTTrackJetCorrectionESProducer;
typedef FFTJetCorrectionESProducer<fftcorrtypes::JPTJet>   FFTJPTJetCorrectionESProducer;
typedef FFTJetCorrectionESProducer<fftcorrtypes::PFCHS0>   FFTPFCHS0CorrectionESProducer;
typedef FFTJetCorrectionESProducer<fftcorrtypes::PFCHS1>   FFTPFCHS1CorrectionESProducer;
typedef FFTJetCorrectionESProducer<fftcorrtypes::PFCHS2>   FFTPFCHS2CorrectionESProducer;

DEFINE_FWK_EVENTSETUP_MODULE(FFTBasicJetCorrectionESProducer);
DEFINE_FWK_EVENTSETUP_MODULE(FFTGenJetCorrectionESProducer);
DEFINE_FWK_EVENTSETUP_MODULE(FFTCaloJetCorrectionESProducer);
DEFINE_FWK_EVENTSETUP_MODULE(FFTPFJetCorrectionESProducer);
DEFINE_FWK_EVENTSETUP_MODULE(FFTTrackJetCorrectionESProducer);
DEFINE_FWK_EVENTSETUP_MODULE(FFTJPTJetCorrectionESProducer);
DEFINE_FWK_EVENTSETUP_MODULE(FFTPFCHS0CorrectionESProducer);
DEFINE_FWK_EVENTSETUP_MODULE(FFTPFCHS1CorrectionESProducer);
DEFINE_FWK_EVENTSETUP_MODULE(FFTPFCHS2CorrectionESProducer);
