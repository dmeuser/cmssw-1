//-------------------------------------------------
//
//   Class: L1MuGMTPhiLUT
//
//   Description: Look-up table for GMT Phi Projection Unit
//
//
//   $Date: 2006/05/15 13:56:02 $
//   $Revision: 1.1 $
//
//   Author :
//   H. Sakulin                CERN EP 
//
//   Migrated to CMSSW:
//   I. Mikulec
//
//--------------------------------------------------

//-----------------------
// This Class's Header --
//-----------------------

#include "L1Trigger/GlobalMuonTrigger/src/L1MuGMTPhiLUT.h"

//---------------
// C++ Headers --
//---------------

#include <iostream>
#include <vector>
#include <cmath>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include "FWCore/MessageLogger/interface/MessageLogger.h"

// --------------------------------
//       class L1MuGMTPhiLUT
//---------------------------------

//----------------
// Constructors --
//----------------
L1MuGMTPhiLUT::L1MuGMTPhiLUT() {
}

//--------------
// Destructor --
//--------------
L1MuGMTPhiLUT::~L1MuGMTPhiLUT() { 
  
}

//--------------
// Operations --
//--------------
 
int L1MuGMTPhiLUT::etabin(float eta, int isys) {
  int i;
  for (i=0; i<(int)NETA;i++)
    if (eta >= etabins[isys][i] && eta < etabins[isys][i+1]) break;
  if (i>=(int)NETA) edm::LogWarning("LUTProblem") << "L1MuGMTPhiLUT::etabin(): could not assign eta bin " << endl;
  return i;
}


//
// project muon phi to calo/vertex
//
float L1MuGMTPhiLUT::dphi(int isys, int isISO, int icharge, int ieta, float pt) {

  if ( (isys==DT || isys==bRPC) && pt < 4.) pt = 4.; // cut off
  if ( (isys==CSC || isys==fRPC) && pt < 3.) pt = 3.;

  float a=fitparams_phi[isISO][isys][ieta][icharge][0];
  float b=fitparams_phi[isISO][isys][ieta][icharge][1];
  float c=fitparams_phi[isISO][isys][ieta][icharge][2];

  float dphi = a + b / pt + c / (pt*pt);

  // dphi is difference between low edge of muon phi bin and real phi in calo/vtx

  // correct for RPC LUTs which were calculated with center of bin phi
  if (isys==bRPC || isys==fRPC) dphi -= 1.25 / 180. * M_PI;
  
  return dphi;
}


//
// static parameters of LUTs
//
float L1MuGMTPhiLUT::etabins[L1MuGMTPhiLUT::NSYS][L1MuGMTPhiLUT::NETA+1] = {
  { 0.00, 0.22, 0.27, 0.58, 0.77, 0.87, 0.92, 1.24, 1.24 }, // DT
  { 0.00, 1.06, 1.26, 1.46, 1.66, 1.86, 2.06, 2.26, 2.46 }, // CSC
  { 0.00, 0.06, 0.25, 0.41, 0.54, 0.70, 0.83, 0.93, 1.04 }, // bRPC
  { 1.04, 1.24, 1.36, 1.48, 1.61, 1.73, 1.85, 1.97, 2.10}   // fRPC
};


float L1MuGMTPhiLUT::fitparams_phi[L1MuGMTPhiLUT::NRP][L1MuGMTPhiLUT::NSYS][L1MuGMTPhiLUT::NETA][2][3]= {
  {
    // projection to HCAL derived from HCAL positions retrieved in ORCA
    // calo phi projection parametrization for DT, projection to HCAL in (for MIP)
    {
      { { -0.001290,  0.504308,  4.549529  }, { -0.023984, -0.970760, -2.263805  }  },
      { { -0.001920,  0.489318,  4.356311  }, { -0.022833, -1.012401, -2.018399  }  },
      { { -0.000194,  0.552174,  4.758974  }, { -0.023465, -1.057848, -2.331967  }  },
      { { -0.002269,  0.604601,  4.030573  }, { -0.023426, -1.007778, -2.420637  }  },
      { { -0.000271,  0.533090,  4.488132  }, { -0.025969, -0.981604, -2.149771  }  },
      { {  0.004051,  0.374228,  4.799831  }, { -0.031902, -0.732712, -2.896422  }  },
      { { -0.004666,  0.583151,  3.559875  }, { -0.011293, -1.095987, -1.514169  }  },
      { {  0.000000,  0.000000,  0.000000  }, {  0.000000,  0.000000,  0.000000  }  }
    },
    // calo phi projection parametrization for CSC, projection to HCAL in (for MIP)
    {
      { { -0.001691,  0.581263,  3.408109  }, { -0.019783, -1.150412, -0.622612  }  },
      { { -0.002138,  0.554844,  2.839203  }, { -0.018853, -0.986130, -1.098399  }  },
      { {  0.002930,  0.300343,  2.637024  }, { -0.021499, -0.705878, -1.024004  }  },
      { {  0.002278,  0.217012,  1.529797  }, { -0.019729, -0.581867, -0.343904  }  },
      { {  0.017130,  0.166188,  0.849492  }, { -0.035404, -0.321889, -0.612119  }  },
      { {  0.024403,  0.133149,  0.520598  }, { -0.043978, -0.156961, -0.719900  }  },
      { {  0.026036,  0.161303,  0.114142  }, { -0.045904, -0.096463, -0.652872  }  },
      { {  0.024208,  0.146788,  0.016299  }, { -0.043546, -0.023951, -0.683871  }  }
    },
    // calo phi projection parametrization for bRPC, projection to HCAL in (for MIP)
    {
      { {  0.015837,  0.665802,  3.613197  }, { -0.015709, -0.464655, -4.528647  }  },
      { {  0.017926,  0.613610,  3.745701  }, { -0.016081, -0.448283, -4.606902  }  },
      { {  0.014551,  0.797055,  3.479120  }, { -0.014036, -0.612790, -4.167947  }  },
      { {  0.018614,  0.637761,  4.156929  }, { -0.017071, -0.504211, -4.651026  }  },
      { {  0.017844,  0.685002,  3.569384  }, { -0.017014, -0.487703, -4.822967  }  },
      { {  0.016250,  0.753998,  3.303425  }, { -0.017553, -0.538919, -4.559156  }  },
      { {  0.019015,  0.653392,  3.993684  }, { -0.018937, -0.525990, -4.335586  }  },
      { {  0.017044,  0.659718,  3.461996  }, { -0.018881, -0.479496, -4.242701  }  }
    },
    // calo phi projection parametrization for fRPC, projection to HCAL in (for MIP)
    {
      { {  0.015301,  0.681718,  2.707917  }, { -0.015104, -0.555634, -3.052359  }  },
      { {  0.018868,  0.511874,  2.826352  }, { -0.015763, -0.549633, -2.301781  }  },
      { {  0.022818,  0.267658,  2.445503  }, { -0.018882, -0.216642, -2.313509  }  },
      { {  0.021048,  0.269100,  1.373464  }, { -0.018502, -0.228849, -1.458785  }  },
      { {  0.019976,  0.269698,  0.896744  }, { -0.021801, -0.227103, -1.032946  }  },
      { {  0.039155,  0.112461,  0.965459  }, { -0.043126, -0.065388, -1.062142  }  },
      { {  0.040538,  0.146610,  0.512473  }, { -0.045719, -0.079835, -0.681237  }  },
      { {  0.040019,  0.120561,  0.511092  }, { -0.044669, -0.031321, -0.745264  }  }
    }
  },
  {
    // calo phi projection parametrization for DT, projection to vertex (for ISO)
    {
      { { -0.019085,  2.530752,  3.112250  }, { -0.023016, -2.590798, -2.725504  }  },
      { { -0.018956,  2.533785,  2.969394  }, { -0.023637, -2.568775, -2.964180  }  },
      { { -0.018869,  2.533260,  2.940186  }, { -0.022805, -2.598547, -2.607942  }  },
      { { -0.019732,  2.527371,  2.611957  }, { -0.023624, -2.510180, -2.870058  }  },
      { { -0.019042,  2.477538,  2.628565  }, { -0.022541, -2.560252, -1.989453  }  },
      { { -0.018623,  2.408255,  2.631045  }, { -0.022602, -2.486282, -2.123144  }  },
      { { -0.019051,  2.424498,  2.422834  }, { -0.015844, -2.610096, -2.047908  }  },
      { {  0.000000,  0.000000,  0.000000  }, {  0.000000,  0.000000,  0.000000  }  }
    },
    // calo phi projection parametrization for CSC, projection to vertex (for ISO)
    {
      { { -0.017973,  2.508663,  1.548810  }, { -0.020354, -2.641325, -0.938284  }  },
      { { -0.018821,  2.346581,  1.584596  }, { -0.018082, -2.470272, -1.086931  }  },
      { { -0.016044,  1.919469,  1.765014  }, { -0.021392, -2.039066, -1.013819  }  },
      { { -0.013416,  1.526508,  1.576988  }, { -0.020571, -1.715829, -0.641736  }  },
      { { -0.011872,  1.200872,  1.248002  }, { -0.019462, -1.444363, -0.276027  }  },
      { { -0.009545,  0.882696,  1.369971  }, { -0.019899, -1.152519, -0.299660  }  },
      { { -0.007845,  0.657952,  1.103447  }, { -0.019512, -0.927346, -0.260993  }  },
      { { -0.006395,  0.478389,  1.283519  }, { -0.018998, -0.796310, -0.126124  }  }
    },
    // calo phi projection parametrization for bRPC, projection to vertex (for ISO)
    {
      { {  0.002479,  2.626429,  2.457669  }, {  0.000336, -2.438207, -3.453805  }  },
      { {  0.002440,  2.639809,  2.115057  }, {  0.000804, -2.486881, -3.044556  }  },
      { {  0.002161,  2.641236,  2.264882  }, {  0.001000, -2.491077, -2.856622  }  },
      { {  0.001901,  2.637712,  2.031976  }, { -0.001031, -2.410168, -3.299684  }  },
      { {  0.002487,  2.595609,  2.105496  }, {  0.000474, -2.422474, -3.296519  }  },
      { {  0.001624,  2.614643,  2.065672  }, { -0.000492, -2.468336, -2.884312  }  },
      { {  0.002451,  2.566535,  2.097821  }, { -0.002069, -2.451065, -2.481968  }  },
      { {  0.002541,  2.517676,  2.129723  }, {  0.000255, -2.467283, -2.342318  }  }
    },
    // calo phi projection parametrization for fRPC, projection to vertex (for ISO)
    {
      { {  0.000191,  2.586133,  1.009632  }, {  0.002492, -2.417437, -1.790964  }  },
      { {  0.000549,  2.282987,  0.992959  }, {  0.003267, -2.192674, -1.343063  }  },
      { {  0.000117,  2.081965,  0.577031  }, {  0.003213, -1.933657, -1.071969  }  },
      { { -0.001817,  1.899919,  0.061467  }, {  0.003783, -1.746777, -0.644316  }  },
      { { -0.001978,  1.613175,  0.304733  }, { -0.000203, -1.567548, -0.433546  }  },
      { { -0.003000,  1.437795,  0.192639  }, { -0.001441, -1.342958, -0.556960  }  },
      { { -0.003725,  1.309459, -0.159897  }, { -0.002232, -1.182838, -0.440219  }  },
      { { -0.005232,  1.179625, -0.153718  }, {  0.000748, -1.088718, -0.151436  }  }
    }
  }
};

