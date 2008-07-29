#ifndef CSCTriggerPrimitives_CSCCathodeLCTProcessor_h
#define CSCTriggerPrimitives_CSCCathodeLCTProcessor_h

/** \class CSCCathodeLCTProcessor
 *
 * This class simulates the functionality of the cathode LCT card. It is run by
 * the MotherBoard and returns up to two CathodeLCTs.  It can be run either in
 * a test mode, where it is passed an array of comparator times and comparator
 * values, or in normal mode where it determines the time and comparator
 * information from the comparator digis.
 *
 * The CathodeLCTs come in distrip and halfstrip flavors; they are sorted
 * (from best to worst) as follows: 6/6H, 5/6H, 6/6D, 4/6H, 5/6D, 4/6D.
 *
 * \date May 2001  Removed the card boundaries.  Changed the Pretrigger to
 * emulate the hardware electronic logic.  Also changed the keylayer to be
 * the 4th layer in a chamber instead of the 3rd layer from the interaction
 * region. The code is a more realistic simulation of hardware LCT logic now.
 * -Jason Mumford.
 *
 * \author Benn Tannenbaum  UCLA 13 July 1999 benn@physics.ucla.edu
 * Numerous later improvements by Jason Mumford and Slava Valuev (see cvs
 * in ORCA).
 * Porting from ORCA by S. Valuev (Slava.Valuev@cern.ch), May 2006.
 *
 * $Date: 2008/07/06 05:17:00 $
 * $Revision: 1.16 $
 *
 */

#include <vector>
#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include <DataFormats/CSCDigi/interface/CSCComparatorDigiCollection.h>
#include <DataFormats/CSCDigi/interface/CSCCLCTDigi.h>
#include <CondFormats/CSCObjects/interface/CSCL1TPParameters.h>
#include <L1Trigger/CSCCommonTrigger/interface/CSCConstants.h>

class CSCCathodeLCTProcessor
{
 public:
  /** Normal constructor. */
  CSCCathodeLCTProcessor(unsigned endcap, unsigned station, unsigned sector,
			 unsigned subsector, unsigned chamber,
			 const edm::ParameterSet& conf,
			 const edm::ParameterSet& comm);

  /** Default constructor. Used for testing. */
  CSCCathodeLCTProcessor();

  /** Sets configuration parameters obtained via EventSetup mechanism. */
  void setConfigParameters(const CSCL1TPParameters* conf);

  /** Clears the LCT containers. */
  void clear();

  /** Runs the LCT processor code. Called in normal running -- gets info from
      a collection of comparator digis. */
  std::vector<CSCCLCTDigi> run(const CSCComparatorDigiCollection* compdc);

  /** Called in test mode and by the run(compdc) function; does the actual LCT
      finding. */
  void run(int triad[CSCConstants::NUM_LAYERS][CSCConstants::MAX_NUM_STRIPS],
	   int time[CSCConstants::NUM_LAYERS][CSCConstants::MAX_NUM_STRIPS],
	   int digiNum[CSCConstants::NUM_LAYERS][CSCConstants::MAX_NUM_STRIPS]);
 
  /** Access routine to comparator digis. */
  bool getDigis(const CSCComparatorDigiCollection* compdc);

  /** Best LCT in this chamber, as found by the processor. */
  CSCCLCTDigi bestCLCT;

  /** Second best LCT in this chamber, as found by the processor. */
  CSCCLCTDigi secondCLCT;

  /** Returns vector of found CLCTs, if any. */
  std::vector<CSCCLCTDigi> getCLCTs();

  static void distripStagger(int stag_triad[CSCConstants::MAX_NUM_STRIPS],
			     int stag_time[CSCConstants::MAX_NUM_STRIPS],
			     int stag_digi[CSCConstants::MAX_NUM_STRIPS],
			     int i_distrip, bool debug = false);

  /** Pre-defined patterns. */
  enum {NUM_PATTERN_STRIPS = 26};
  static const int pre_hit_pattern[2][NUM_PATTERN_STRIPS];
  static const int pattern[CSCConstants::NUM_CLCT_PATTERNS_PRE_TMB07][NUM_PATTERN_STRIPS+1];

  bool isTMB07;
  enum {NUM_PATTERN_HALFSTRIPS = 42};
  static const int pattern2007_offset[NUM_PATTERN_HALFSTRIPS];
  static const int pattern2007[CSCConstants::NUM_CLCT_PATTERNS][NUM_PATTERN_HALFSTRIPS+1];
  unsigned int best_pid[CSCConstants::NUM_HALF_STRIPS];
  unsigned int nhits[CSCConstants::NUM_HALF_STRIPS];

  std::vector<CSCCLCTDigi> findLCTs2007(const int halfstrip[CSCConstants::NUM_LAYERS][CSCConstants::NUM_HALF_STRIPS]);
  void ptnFinding2007(
	const unsigned int pulse[CSCConstants::NUM_LAYERS][CSCConstants::NUM_HALF_STRIPS],
	const int nStrips, const unsigned int bx_time);
  void markBusyKeys(const int best_hstrip, const int best_pid,
		    int quality[CSCConstants::NUM_HALF_STRIPS]);

  /** Number of di-strips/half-strips per CFEB. **/
  static const int cfeb_strips[2];

  /** Maximum number of cathode front-end boards (move to CSCConstants?). */
  enum {MAX_CFEBS = 5};

  // we use these next ones to address the various bits inside the array that's
  // used to make the cathode LCTs.
  enum CLCT_INDICES {CLCT_PATTERN, CLCT_BEND, CLCT_STRIP, CLCT_BX,
		     CLCT_STRIP_TYPE, CLCT_QUALITY, CLCT_CFEB};

 private:
  /** Verbosity level: 0: no print (default).
   *                   1: print only CLCTs found.
   *                   2: info at every step of the algorithm.
   *                   3: add special-purpose prints. */
  int infoV;

  /** Chamber id (trigger-type labels). */
  const unsigned theEndcap;
  const unsigned theStation;
  const unsigned theSector;
  const unsigned theSubsector;
  const unsigned theTrigChamber;

  int numStrips;
  int stagger[CSCConstants::NUM_LAYERS];

  std::vector<CSCComparatorDigi> digiV[CSCConstants::NUM_LAYERS];

  /** Flag for MTCC data. */
  bool isMTCC; 

  /** Configuration parameters. */
  unsigned int bx_width, drift_delay, hs_thresh, ds_thresh, nph_pattern;
  unsigned int fifo_tbins, fifo_pretrig; // only for test beam mode.
  unsigned int hit_thresh, pid_thresh;   // new TMB-07 parameters
  unsigned int sep_src, sep_vme;

  /** Default values of configuration parameters. */
  static const unsigned int def_bx_width,   def_drift_delay;
  static const unsigned int def_hs_thresh,  def_ds_thresh;
  static const unsigned int def_nph_pattern;
  static const unsigned int def_fifo_tbins, def_fifo_pretrig;
  static const unsigned int def_hit_thresh, def_pid_thresh;
  static const unsigned int def_sep_src,    def_sep_vme;

  /** Set default values for configuration parameters. */
  void setDefaultConfigParameters();

  /** Make sure that the parameter values are within the allowed range. */
  void checkConfigParameters();

  //----------------------- Default functions ---------------------------------
  std::vector<CSCCLCTDigi> findLCTs(const int strip[CSCConstants::NUM_LAYERS][CSCConstants::NUM_HALF_STRIPS],
				    int width, int numStrips);
  bool preTrigger(const int strip[CSCConstants::NUM_LAYERS][CSCConstants::NUM_HALF_STRIPS],
		  const int stripType, const int nStrips, int& first_bx);
  void getKeyStripData(const int strip[CSCConstants::NUM_LAYERS][CSCConstants::NUM_HALF_STRIPS],
		       int keystrip_data[CSCConstants::NUM_HALF_STRIPS][7],
		       int nStrips, int first_bx, int& best_strip,
		       int stripType);
  void getPattern(int pattern_num, int strip_value[NUM_PATTERN_STRIPS],
		  int bx_time, int &quality, int &bend);
  bool hitIsGood(int hitTime, int BX);

  //----------------------- Test Beam Fcns below ----------------------------
  std::vector<CSCCLCTDigi> findLCTs(const int halfstrip[CSCConstants::NUM_LAYERS][CSCConstants::NUM_HALF_STRIPS],
				    const int distrip[CSCConstants::NUM_LAYERS][CSCConstants::NUM_HALF_STRIPS]);
  bool preTrigger(const int strip[CSCConstants::NUM_LAYERS][CSCConstants::NUM_HALF_STRIPS],
		  unsigned int pulse[CSCConstants::NUM_LAYERS][CSCConstants::NUM_HALF_STRIPS], 
		  const int stripType, const int nStrips,
		  const int start_bx, int& first_bx);
  bool preTrigLookUp(const unsigned int pulse[CSCConstants::NUM_LAYERS][CSCConstants::NUM_HALF_STRIPS],
		     const int stripType, const int nStrips,
		     const unsigned int bx_time);
  void latchLCTs(const unsigned int pulse[CSCConstants::NUM_LAYERS][CSCConstants::NUM_HALF_STRIPS],
		 int keyStrip[MAX_CFEBS], unsigned int nhits[MAX_CFEBS],
		 const int stripType, const int nStrips, const int bx_time);
  void priorityEncode(const int h_keyStrip[MAX_CFEBS],
		      const unsigned int h_nhits[MAX_CFEBS],
		      const int d_keyStrip[MAX_CFEBS],
		      const unsigned int d_nhits[MAX_CFEBS],
		      int keystrip_data[2][7]);
  void getKeyStripData(const unsigned int h_pulse[CSCConstants::NUM_LAYERS][CSCConstants::NUM_HALF_STRIPS],
		       const unsigned int d_pulse[CSCConstants::NUM_LAYERS][CSCConstants::NUM_HALF_STRIPS],
		       int keystrip_data[2][7], const int first_bx);
  void getPattern(unsigned int pattern_num,
		  const int strip_value[NUM_PATTERN_STRIPS],
		  unsigned int& quality, unsigned int& bend);
  //-------------------------------------------------------------------------

  /** Dump CLCT configuration parameters. */
  void dumpConfigParams() const;

  /** Dump digis on half-strips and di-strips. */
  void dumpDigis(const int strip[CSCConstants::NUM_LAYERS][CSCConstants::NUM_HALF_STRIPS],
		 const int stripType, const int nStrips) const;

  void testDistripStagger();
  void testLCTs();
  void printPatterns();
  void testPatterns();
  int findNumLayersHit(int stripsHit[CSCConstants::NUM_LAYERS][CSCConstants::NUM_HALF_STRIPS]);
};

#endif
