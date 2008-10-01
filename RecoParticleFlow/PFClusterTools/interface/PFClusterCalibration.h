#ifndef PFCLUSTERCALIBRATION_H_
#define PFCLUSTERCALIBRATION_H_

#include "RecoParticleFlow/PFClusterTools/interface/DetectorElementType.h"
#include "RecoParticleFlow/PFClusterTools/interface/Calibratable.h"
#include "RecoParticleFlow/PFClusterTools/interface/CalibrationResultWrapper.h"

#include <TF1.h>
#include <vector>
#include <string>
#include <map>
#include <TTree.h>
#include <ostream>

namespace pftools {

/*
 * \class PFClusterCalibration
 * \brief Calibrated calorimeter cluster energy for hadronic PFCandidates.
 * \author Jamie Ballin, Imperial College London
 * \date September 2008
 * 
 * The ECAL and HCAL have been calibrated to 50 GeV electrons. Therefore, a calibration is required 
 * to evaluate the correct hadronic response. This class will calibrate clusters belonging to a PFCandidate.
 * (Users should access these clusters from the blocks in the PFCandidate).
 * 
 * For a given particle to calibrate, this object selects from one of three classes:
 * 	i) ECAL only where E_hcal < hcalCut
 * 	ii) HCAL only where E_ecal < ecalCut
 * 	iii) ECAL and HCAL where both are greater than the cuts
 * 	iv) do nothing when all tests fail
 * 
 * In each case, a linear calibration is evaluated, for barrel and endcap (call setBarrelBoundary(double eta)
 * to set this limit).
 * 
 * Sensible default values are set for all members, but in order to get usable results, you must supply the
 * latest function parameters and corrections (seperately available) - see setCorrections() 
 * and setEvolutionParameters() documentation below.
 */
class PFClusterCalibration {
public:
	
	/* Constructor with sensible defaults */
	PFClusterCalibration();

	virtual ~PFClusterCalibration() {
	}

	/* Returns the calibrated ecalEnergy */
	double getCalibratedEcalEnergy(const double& ecalE, const double& hcalE,
			const double& eta, const double& phi) const;
	
	/* Returns the calibrated hcalEnergy */
	double getCalibratedHcalEnergy(const double& ecalE, const double& hcalE,
			const double& eta, const double& phi) const;

	/* Returns the calibrated particle energy with the correction
	 * Note: for, say, ecalOnly particles:
	 * energy = correction_function([calibrated ecalEnergy + hcalEnergy(v small)])
	 * ditto hcalOnly
	 */
	double getCalibratedEnergy(const double& ecalE, const double& hcalE,
			const double& eta, const double& phi) const;
	
	void getCalibratedEnergyEmbedAInHcal(double& ecalE,
			double& hcalE, const double& eta, const double& phi) const;

	/* TESTING purposes only! */
	void calibrate(Calibratable& c);

	/* TESTING purposes only! */
	void getCalibrationResultWrapper(const Calibratable& c,
			CalibrationResultWrapper& crw);

	/* TESTING purposes only! */
	void calibrateTree(TTree* tree);

	/* Sets the 'a' term in the abc calibration and a final linear correction.
	 * You get these values from the (seperately available) option file. */
	void setCorrections(const double& lowEP0, const double& lowEP1,
			const double& globalP0, const double& globalP1);

	/* getCalibratedEnergy() returns max(0, calibrated energy) if this is true. */
	void setAllowNegativeEnergy(const bool& allowIt) {
		allowNegativeEnergy_ = allowIt;
	}

	/* Whether to apply a final correction function in getCalibratedEnergy()
	 * Highly recommended ('a' term of abc calibration will be neglected otherwise. */
	void setDoCorrection(const int& doCorrection) {
		doCorrection_ = doCorrection;
	}

	/* Threshold for ecalOnly and hcalOnly evaluation. */
	void setEcalHcalEnergyCuts(const double& ecalCut, const double& hcalCut) {
		ecalOnlyDiv_ = ecalCut;
		hcalOnlyDiv_ = hcalCut;
	}

	/* Hard cut between barrel and endcap. */
	void setBarrelBoundary(const double& eta) {
		barrelEndcapEtaDiv_ = eta;
	}

	/* Sets the function parameters - very important! */
	void setEvolutionParameters(const std::string& sector,
			std::vector<double> params);

	/* Elements in this vector refer to the different calibration functions
	 * available. For each one of these, you should call setEvolutionParameters()
	 * with the appropriate vector<double> acquired from an options file.
	 */
	std::vector<std::string>* getKnownSectorNames() {
		return &names_;
	}
	
	/* Dumps the member values to the stream */
	friend std::ostream& pftools::operator<<(std::ostream& s, const PFClusterCalibration& cc);

private:

	//where to select either barrel or endcap
	double barrelEndcapEtaDiv_;

	//at what energy to split between ecalOnly, hcalOnly, ecalHcal
	double ecalOnlyDiv_;
	double hcalOnlyDiv_;

	int doCorrection_;
	int allowNegativeEnergy_;

	double correctionLowLimit_;
	double globalP0_;
	double globalP1_;
	double lowEP0_;
	double lowEP1_;

	//Function used to correct final total energies
	TF1 correction_;

	std::map<std::string, TF1> namesAndFunctions_;
	std::vector<std::string> names_;

};
}

#endif /* PFCLUSTERCALIBRATION_H_ */
