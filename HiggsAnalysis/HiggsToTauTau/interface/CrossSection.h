#ifndef CrossSection_h
#define CrossSection_h

#include <map>
#include <string>
#include <utility>
#include <iostream>

/**
   \class   CrossSection CrossSection.h "HiggsAnalysis/HiggsToTauTau/interface/CrossSection.h"

   \brief   Class to return pre-calculated values for cross sections and branching ratios from the LHC cross section goup

   This is a class to return the precalculated and blessed cross sections for SM Higgs boson 
   production and branching ratios to tau leptons from LHC Cros Section Group. You can find 
   the cross section for 7 TeV and 8 TeV on these TWiki pages: 

   https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageAt7TeV
   https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageAt8TeV

   The range from Higgs masses between 90 GeV and 1000 GeV for ggH and qqH and from 90 GeV 
   to 300 GeV for WH, ZH, ttH. For those mass points, which are not tabluated on the TWikis, 
   linear interpolation is applied. 
*/

class CrossSection {

 public:
  /// default constructor
  CrossSection(float ecms) : ecms_(ecms){};
  /// default destructor
  ~CrossSection(){};

  /// get cross section; values which are not mapped are interpolated
  /// valules which are out of range return 0
  float evaluate(const char* process, float mass);

 private:
  /// load ggH cross sections
  void ggH();
  /// load qqH cross sections
  void qqH();
  /// load WH cross sections
  void WH();
  /// load ZH cross sections
  void ZH();
  /// load ttH cross sections
  void ttH();
  /// load BR
  void BR();
  /// approximate scale factors for 7 TeV to 8 Tev
  void seven2xxxTeV();
  /// get actual value from map
  float linear(float mass, std::map<float, float>& map);
  /// this is the SM background sample for which only the scale to 
  /// different ecms will be provided
  bool background(std::string proc){
    return (proc == std::string("ttbar")||
	    proc == std::string("top-t")||
	    proc == std::string("top-s")||
	    proc == std::string("W-top")||
	    proc == std::string("W"    )||
	    proc == std::string("Z"    )||
	    proc == std::string("QCD"  )||
	    proc == std::string("VV"   ));
  }

 private:
  /// center of mass energy for the evaluation of the cross sections
  float ecms_;
  /// cross section map for desired process
  std::map<float, float> xsec_;
  /// for the time being these are the scale factors to go from 7 TeV to 8 TeV
  std::map<float, float> scale_;
  /// these are the scale factors for background processes
  std::map<std::string, float> scaleBG_;
};

inline void
CrossSection::seven2xxxTeV()
{
  bool scaled=false;
  if(ecms_ == 8){
    scaled=true;
    /// provide BG scales		     
    scaleBG_[std::string("ttbar")] = 1.42982; 
    scaleBG_[std::string("W"    )] = 1.15786; 
    scaleBG_[std::string("Z"    )] = 1.14951;
    scaleBG_[std::string("W-top")] = 1.42024;
    scaleBG_[std::string("top-t")] = 1.32469;
    scaleBG_[std::string("top-s")] = 1.22067;
    scaleBG_[std::string("WW"   )] = 1.21510; 
    scaleBG_[std::string("WZ"   )] = 1.23344;
    scaleBG_[std::string("ZZ"   )] = 1.21944;
    /// provide ggH signal scales
    scale_[  90] = 1.27394;
    scale_[ 120] = 1.27742;
    scale_[ 125] = 1.28090;
    scale_[ 130] = 1.28437;
    scale_[ 135] = 1.28775;
    scale_[ 140] = 1.29112;
    scale_[ 145] = 1.29439;
    scale_[ 150] = 1.29766;
    scale_[ 160] = 1.30403;
    scale_[ 170] = 1.31031;
    scale_[ 180] = 1.31641;
    scale_[ 190] = 1.32237;
    scale_[ 200] = 1.32822;
    scale_[ 250] = 1.35530;
    scale_[ 300] = 1.38238;
    scale_[ 350] = 1.40661;
    scale_[ 400] = 1.43085;
    scale_[ 450] = 1.45471;
    scale_[ 500] = 1.47857;
    scale_[ 550] = 1.50211;
    scale_[ 600] = 1.52566;
    scale_[1000] = 1.52566;
  }
  if(ecms_ == 14){
    scaled=true;
    /// provide BG scales		     
    scaleBG_[std::string("ttbar")] = 5.59001; 
    scaleBG_[std::string("W"    )] = 2.09545; 
    scaleBG_[std::string("Z"    )] = 2.02904;
    scaleBG_[std::string("W-top")] = 5.32411;
    scaleBG_[std::string("top-t")] = 3.88168;
    scaleBG_[std::string("top-s")] = 2.65179;
    scaleBG_[std::string("WW"   )] = 2.62549; 
    scaleBG_[std::string("WZ"   )] = 2.79381;
    scaleBG_[std::string("ZZ"   )] = 2.64949;
    /// provide ggH signal scales
    scale_[  90] = 3.27581;
    scale_[ 115] = 3.27581;
    scale_[ 120] = 3.31737;
    scale_[ 125] = 3.35893;
    scale_[ 130] = 3.40049;
    scale_[ 135] = 3.44132;
    scale_[ 140] = 3.48215;
    scale_[ 145] = 3.52248;
    scale_[ 150] = 3.56281;
    scale_[ 160] = 3.64248;
    scale_[ 170] = 3.72139;
    scale_[ 180] = 3.79989;
    scale_[ 190] = 3.87800;
    scale_[ 200] = 3.95563;
    scale_[ 250] = 4.33932;
    scale_[ 300] = 4.72300;
    scale_[ 350] = 5.10642;
    scale_[ 400] = 5.48983;
    scale_[ 450] = 5.91051;
    scale_[ 500] = 6.33118;
    scale_[ 550] = 6.78806;
    scale_[ 600] = 7.24494;
    scale_[1000] = 7.24494;
  }
  if(!scaled){
    std::cerr 
      << "Warning: the ecms that you chose is not implemented: " << ecms_ << std::endl
      << "Available values of ecms are: 8(TeV), 14(TeV)" << std::endl;
    exit(0);
  }
}

inline void
CrossSection::ggH()
{
  xsec_[  90.0] = 29.47; 	
  xsec_[  95.0] = 26.58; 	
  xsec_[ 100.0] = 24.02; 	
  xsec_[ 105.0] = 21.78; 	
  xsec_[ 110.0] = 19.84; 	
  xsec_[ 110.5] = 19.65; 	
  xsec_[ 111.0] = 19.48; 	
  xsec_[ 111.5] = 19.30; 	
  xsec_[ 112.0] = 19.13; 	
  xsec_[ 112.5] = 18.95; 	
  xsec_[ 113.0] = 18.79; 	
  xsec_[ 113.5] = 18.62; 	
  xsec_[ 114.0] = 18.45; 	
  xsec_[ 114.5] = 18.29; 	
  xsec_[ 115.0] = 18.13; 	
  xsec_[ 115.5] = 17.97; 	
  xsec_[ 116.0] = 17.82; 	
  xsec_[ 116.5] = 17.66; 	
  xsec_[ 117.0] = 17.51; 	
  xsec_[ 117.5] = 17.36; 	
  xsec_[ 118.0] = 17.21; 	
  xsec_[ 118.5] = 17.06; 	
  xsec_[ 119.0] = 16.92; 	
  xsec_[ 119.5] = 16.78; 	
  xsec_[ 120.0] = 16.63; 	
  xsec_[ 120.5] = 16.49; 	
  xsec_[ 121.0] = 16.36; 	
  xsec_[ 121.5] = 16.22; 	
  xsec_[ 122.0] = 16.08; 	
  xsec_[ 122.5] = 15.94; 	
  xsec_[ 123.0] = 15.82; 	
  xsec_[ 123.5] = 15.69; 	
  xsec_[ 124.0] = 15.56; 	
  xsec_[ 124.5] = 15.43; 	
  xsec_[ 125.0] = 15.31; 	
  xsec_[ 125.5] = 15.18; 	
  xsec_[ 126.0] = 15.06; 	
  xsec_[ 126.5] = 14.94; 	
  xsec_[ 127.0] = 14.82; 	
  xsec_[ 127.5] = 14.70; 	
  xsec_[ 128.0] = 14.58; 	
  xsec_[ 128.5] = 14.46; 	
  xsec_[ 129.0] = 14.35; 	
  xsec_[ 129.5] = 14.23; 	
  xsec_[ 130.0] = 14.12; 	
  xsec_[ 130.5] = 14.01; 	
  xsec_[ 131.0] = 13.90; 	
  xsec_[ 131.5] = 13.80; 	
  xsec_[ 132.0] = 13.69; 	
  xsec_[ 132.5] = 13.58; 	
  xsec_[ 133.0] = 13.48; 	
  xsec_[ 133.5] = 13.37; 	
  xsec_[ 134.0] = 13.28; 	
  xsec_[ 134.5] = 13.18; 	
  xsec_[ 135.0] = 13.08; 	
  xsec_[ 135.5] = 12.98; 	
  xsec_[ 136.0] = 12.88; 	
  xsec_[ 136.5] = 12.78; 	
  xsec_[ 137.0] = 12.68; 	
  xsec_[ 137.5] = 12.60; 	
  xsec_[ 138.0] = 12.50; 	
  xsec_[ 138.5] = 12.40; 	
  xsec_[ 139.0] = 12.31; 	
  xsec_[ 139.5] = 12.22; 	
  xsec_[ 140.0] = 12.13; 	
  xsec_[ 141.0] = 11.95; 	
  xsec_[ 142.0] = 11.78; 	
  xsec_[ 143.0] = 11.60; 	
  xsec_[ 144.0] = 11.44; 	
  xsec_[ 145.0] = 11.27; 	
  xsec_[ 146.0] = 11.12; 	
  xsec_[ 147.0] = 10.96; 	
  xsec_[ 148.0] = 10.80; 	
  xsec_[ 149.0] = 10.65; 	
  xsec_[ 150.0] = 10.50; 	
  xsec_[ 151.0] = 10.36; 	
  xsec_[ 152.0] = 10.21; 	
  xsec_[ 153.0] = 10.07; 	
  xsec_[ 154.0] = 9.934; 	
  xsec_[ 155.0] = 9.795; 	
  xsec_[ 156.0] = 9.655; 	
  xsec_[ 157.0] = 9.514; 	
  xsec_[ 158.0] = 9.367; 	
  xsec_[ 159.0] = 9.225; 	
  xsec_[ 160.0] = 9.080; 	
  xsec_[ 162.0] = 8.770; 	
  xsec_[ 164.0] = 8.465; 	
  xsec_[ 166.0] = 8.190; 	
  xsec_[ 168.0] = 7.952; 	
  xsec_[ 170.0] = 7.729; 	
  xsec_[ 172.0] = 7.515; 	
  xsec_[ 174.0] = 7.310; 	
  xsec_[ 176.0] = 7.112; 	
  xsec_[ 178.0] = 6.923; 	
  xsec_[ 180.0] = 6.739; 	
  xsec_[ 182.0] = 6.553; 	
  xsec_[ 184.0] = 6.379; 	
  xsec_[ 186.0] = 6.212; 	
  xsec_[ 188.0] = 6.050; 	
  xsec_[ 190.0] = 5.896; 	
  xsec_[ 192.0] = 5.751; 	
  xsec_[ 194.0] = 5.616; 	
  xsec_[ 196.0] = 5.487; 	
  xsec_[ 198.0] = 5.364; 	
  xsec_[ 200.0] = 5.249; 	
  xsec_[ 202.0] = 5.136; 	
  xsec_[ 204.0] = 5.027; 	
  xsec_[ 206.0] = 4.924; 	
  xsec_[ 208.0] = 4.822; 	
  xsec_[ 210.0] = 4.723; 	
  xsec_[ 212.0] = 4.630; 	
  xsec_[ 214.0] = 4.539; 	
  xsec_[ 216.0] = 4.454; 	
  xsec_[ 218.0] = 4.369; 	
  xsec_[ 220.0] = 4.288; 	
  xsec_[ 222.0] = 4.207; 	
  xsec_[ 224.0] = 4.128; 	
  xsec_[ 226.0] = 4.053; 	
  xsec_[ 228.0] = 3.980; 	
  xsec_[ 230.0] = 3.908; 	
  xsec_[ 232.0] = 3.839; 	
  xsec_[ 234.0] = 3.771; 	
  xsec_[ 236.0] = 3.707; 	
  xsec_[ 238.0] = 3.643; 	
  xsec_[ 240.0] = 3.581; 	
  xsec_[ 242.0] = 3.523; 	
  xsec_[ 244.0] = 3.468; 	
  xsec_[ 246.0] = 3.414; 	
  xsec_[ 248.0] = 3.362; 	
  xsec_[ 250.0] = 3.312; 	
  xsec_[ 252.0] = 3.261; 	
  xsec_[ 254.0] = 3.212; 	
  xsec_[ 256.0] = 3.164; 	
  xsec_[ 258.0] = 3.118; 	
  xsec_[ 260.0] = 3.072; 	
  xsec_[ 262.0] = 3.028; 	
  xsec_[ 264.0] = 2.984; 	
  xsec_[ 266.0] = 2.944; 	
  xsec_[ 268.0] = 2.903; 	
  xsec_[ 270.0] = 2.864; 	
  xsec_[ 272.0] = 2.828; 	
  xsec_[ 274.0] = 2.793; 	
  xsec_[ 276.0] = 2.760; 	
  xsec_[ 278.0] = 2.728; 	
  xsec_[ 280.0] = 2.696; 	
  xsec_[ 282.0] = 2.664; 	
  xsec_[ 284.0] = 2.633; 	
  xsec_[ 286.0] = 2.603; 	
  xsec_[ 288.0] = 2.574; 	
  xsec_[ 290.0] = 2.546; 	
  xsec_[ 295.0] = 2.480; 	
  xsec_[ 300.0] = 2.422; 	
  xsec_[ 305.0] = 2.369; 	
  xsec_[ 310.0] = 2.322; 	
  xsec_[ 315.0] = 2.281; 	
  xsec_[ 320.0] = 2.247; 	
  xsec_[ 325.0] = 2.221; 	
  xsec_[ 330.0] = 2.204; 	
  xsec_[ 335.0] = 2.195; 	
  xsec_[ 340.0] = 2.198; 	
  xsec_[ 345.0] = 2.225; 	
  xsec_[ 350.0] = 2.306; 	
  xsec_[ 360.0] = 2.361; 	
  xsec_[ 370.0] = 2.341; 	
  xsec_[ 380.0] = 2.266; 	
  xsec_[ 390.0] = 2.158; 	
  xsec_[ 400.0] = 2.032; 	
  xsec_[ 420.0] = 1.756; 	
  xsec_[ 440.0] = 1.482; 	
  xsec_[ 460.0] = 1.237; 	
  xsec_[ 480.0] = 1.026; 	
  xsec_[ 500.0] = 0.8491; 	
  xsec_[ 520.0] = 0.7006; 	
  xsec_[ 540.0] = 0.5782; 	
  xsec_[ 560.0] = 0.4771; 	
  xsec_[ 580.0] = 0.3944; 	
  xsec_[ 600.0] = 0.3267; 	
  xsec_[ 620.0] = 0.2713; 	
  xsec_[ 640.0] = 0.2257; 	
  xsec_[ 660.0] = 0.1883; 	
  xsec_[ 680.0] = 0.1574; 	
  xsec_[ 700.0] = 0.1320; 	
  xsec_[ 720.0] = 0.1109; 	
  xsec_[ 740.0] = 0.09335;
  xsec_[ 760.0] = 0.07883;
  xsec_[ 780.0] = 0.06668;
  xsec_[ 800.0] = 0.05655;
  xsec_[ 820.0] = 0.04806;
  xsec_[ 840.0] = 0.04089;
  xsec_[ 860.0] = 0.03490;
  xsec_[ 880.0] = 0.02982;
  xsec_[ 900.0] = 0.02555;
  xsec_[ 920.0] = 0.02193;
  xsec_[ 940.0] = 0.01885;
  xsec_[ 960.0] = 0.01624;
  xsec_[ 980.0] = 0.01400;
  xsec_[1000.0] = 0.01210;
}

inline void
CrossSection::qqH()
{
  xsec_[  90.0] = 1.710; 	
  xsec_[  95.0] = 1.628; 	
  xsec_[ 100.0] = 1.546; 	
  xsec_[ 105.0] = 1.472; 	
  xsec_[ 110.0] = 1.398; 	
  xsec_[ 110.5] = 1.391; 	
  xsec_[ 111.0] = 1.384; 	
  xsec_[ 111.5] = 1.378; 	
  xsec_[ 112.0] = 1.371; 	
  xsec_[ 112.5] = 1.364; 	
  xsec_[ 113.0] = 1.358; 	
  xsec_[ 113.5] = 1.351; 	
  xsec_[ 114.0] = 1.345; 	
  xsec_[ 114.5] = 1.339; 	
  xsec_[ 115.0] = 1.332; 	
  xsec_[ 115.5] = 1.326; 	
  xsec_[ 116.0] = 1.319; 	
  xsec_[ 116.5] = 1.313; 	
  xsec_[ 117.0] = 1.307; 	
  xsec_[ 117.5] = 1.300; 	
  xsec_[ 118.0] = 1.294; 	
  xsec_[ 118.5] = 1.288; 	
  xsec_[ 119.0] = 1.282; 	
  xsec_[ 119.5] = 1.276; 	
  xsec_[ 120.0] = 1.269; 	
  xsec_[ 120.5] = 1.263; 	
  xsec_[ 121.0] = 1.257; 	
  xsec_[ 121.5] = 1.251; 	
  xsec_[ 122.0] = 1.246; 	
  xsec_[ 122.5] = 1.240; 	
  xsec_[ 123.0] = 1.234; 	
  xsec_[ 123.5] = 1.228; 	
  xsec_[ 124.0] = 1.222; 	
  xsec_[ 124.5] = 1.216; 	
  xsec_[ 125.0] = 1.211; 	
  xsec_[ 125.5] = 1.205; 	
  xsec_[ 126.0] = 1.199; 	
  xsec_[ 126.5] = 1.193; 	
  xsec_[ 127.0] = 1.188; 	
  xsec_[ 127.5] = 1.182; 	
  xsec_[ 128.0] = 1.176; 	
  xsec_[ 128.5] = 1.171; 	
  xsec_[ 129.0] = 1.165; 	
  xsec_[ 129.5] = 1.159; 	
  xsec_[ 130.0] = 1.154; 	
  xsec_[ 130.5] = 1.148; 	
  xsec_[ 131.0] = 1.143; 	
  xsec_[ 131.5] = 1.137; 	
  xsec_[ 132.0] = 1.132; 	
  xsec_[ 132.5] = 1.126; 	
  xsec_[ 133.0] = 1.121; 	
  xsec_[ 133.5] = 1.115; 	
  xsec_[ 134.0] = 1.110; 	
  xsec_[ 134.5] = 1.105; 	
  xsec_[ 135.0] = 1.100; 	
  xsec_[ 135.5] = 1.095; 	
  xsec_[ 136.0] = 1.090; 	
  xsec_[ 136.5] = 1.085; 	
  xsec_[ 137.0] = 1.080; 	
  xsec_[ 137.5] = 1.076; 	
  xsec_[ 138.0] = 1.071; 	
  xsec_[ 138.5] = 1.066; 	
  xsec_[ 139.0] = 1.062; 	
  xsec_[ 139.5] = 1.057; 	
  xsec_[ 140.0] = 1.052; 	
  xsec_[ 141.0] = 1.043; 	
  xsec_[ 142.0] = 1.033; 	
  xsec_[ 143.0] = 1.023; 	
  xsec_[ 144.0] = 1.013; 	
  xsec_[ 145.0] = 1.004; 	
  xsec_[ 146.0] = 0.9951; 	
  xsec_[ 147.0] = 0.9866; 	
  xsec_[ 148.0] = 0.9782; 	
  xsec_[ 149.0] = 0.9699; 	
  xsec_[ 150.0] = 0.9617; 	
  xsec_[ 151.0] = 0.9529; 	
  xsec_[ 152.0] = 0.9441; 	
  xsec_[ 153.0] = 0.9353; 	
  xsec_[ 154.0] = 0.9266; 	
  xsec_[ 155.0] = 0.9180; 	
  xsec_[ 156.0] = 0.9095; 	
  xsec_[ 157.0] = 0.9013; 	
  xsec_[ 158.0] = 0.8934; 	
  xsec_[ 159.0] = 0.8859; 	
  xsec_[ 160.0] = 0.8787; 	
  xsec_[ 162.0] = 0.8676; 	
  xsec_[ 164.0] = 0.8571; 	
  xsec_[ 166.0] = 0.8453; 	
  xsec_[ 168.0] = 0.8316; 	
  xsec_[ 170.0] = 0.8173; 	
  xsec_[ 172.0] = 0.8029; 	
  xsec_[ 174.0] = 0.7885; 	
  xsec_[ 176.0] = 0.7744; 	
  xsec_[ 178.0] = 0.7609; 	
  xsec_[ 180.0] = 0.7480; 	
  xsec_[ 182.0] = 0.7361; 	
  xsec_[ 184.0] = 0.7248; 	
  xsec_[ 186.0] = 0.7139; 	
  xsec_[ 188.0] = 0.7032; 	
  xsec_[ 190.0] = 0.6925; 	
  xsec_[ 192.0] = 0.6812; 	
  xsec_[ 194.0] = 0.6699; 	
  xsec_[ 196.0] = 0.6587; 	
  xsec_[ 198.0] = 0.6478; 	
  xsec_[ 200.0] = 0.6371; 	
  xsec_[ 202.0] = 0.6267; 	
  xsec_[ 204.0] = 0.6164; 	
  xsec_[ 206.0] = 0.6064; 	
  xsec_[ 208.0] = 0.5965; 	
  xsec_[ 210.0] = 0.5869; 	
  xsec_[ 212.0] = 0.5775; 	
  xsec_[ 214.0] = 0.5684; 	
  xsec_[ 216.0] = 0.5594; 	
  xsec_[ 218.0] = 0.5506; 	
  xsec_[ 220.0] = 0.5420; 	
  xsec_[ 222.0] = 0.5335; 	
  xsec_[ 224.0] = 0.5252; 	
  xsec_[ 226.0] = 0.5170; 	
  xsec_[ 228.0] = 0.5089; 	
  xsec_[ 230.0] = 0.5011; 	
  xsec_[ 232.0] = 0.4934; 	
  xsec_[ 234.0] = 0.4859; 	
  xsec_[ 236.0] = 0.4785; 	
  xsec_[ 238.0] = 0.4712; 	
  xsec_[ 240.0] = 0.4641; 	
  xsec_[ 242.0] = 0.4572; 	
  xsec_[ 244.0] = 0.4503; 	
  xsec_[ 246.0] = 0.4436; 	
  xsec_[ 248.0] = 0.4369; 	
  xsec_[ 250.0] = 0.4304; 	
  xsec_[ 252.0] = 0.4239; 	
  xsec_[ 254.0] = 0.4174; 	
  xsec_[ 256.0] = 0.4111; 	
  xsec_[ 258.0] = 0.4049; 	
  xsec_[ 260.0] = 0.3988; 	
  xsec_[ 262.0] = 0.3931; 	
  xsec_[ 264.0] = 0.3875; 	
  xsec_[ 266.0] = 0.3821; 	
  xsec_[ 268.0] = 0.3767; 	
  xsec_[ 270.0] = 0.3715; 	
  xsec_[ 272.0] = 0.3663; 	
  xsec_[ 274.0] = 0.3611; 	
  xsec_[ 276.0] = 0.3560; 	
  xsec_[ 278.0] = 0.3510; 	
  xsec_[ 280.0] = 0.3461; 	
  xsec_[ 282.0] = 0.3413; 	
  xsec_[ 284.0] = 0.3365; 	
  xsec_[ 286.0] = 0.3318; 	
  xsec_[ 288.0] = 0.3271; 	
  xsec_[ 290.0] = 0.3226; 	
  xsec_[ 295.0] = 0.3116; 	
  xsec_[ 300.0] = 0.3011; 	
  xsec_[ 305.0] = 0.2908; 	
  xsec_[ 310.0] = 0.2809; 	
  xsec_[ 315.0] = 0.2716; 	
  xsec_[ 320.0] = 0.2627; 	
  xsec_[ 325.0] = 0.2539; 	
  xsec_[ 330.0] = 0.2453; 	
  xsec_[ 335.0] = 0.2368; 	
  xsec_[ 340.0] = 0.2286; 	
  xsec_[ 345.0] = 0.2206; 	
  xsec_[ 350.0] = 0.2132; 	
  xsec_[ 360.0] = 0.2018; 	
  xsec_[ 370.0] = 0.1910; 	
  xsec_[ 380.0] = 0.1808; 	
  xsec_[ 390.0] = 0.1712; 	
  xsec_[ 400.0] = 0.1620; 	
  xsec_[ 420.0] = 0.1451; 	
  xsec_[ 440.0] = 0.1304; 	
  xsec_[ 460.0] = 0.1171; 	
  xsec_[ 480.0] = 0.1054; 	
  xsec_[ 500.0] = 0.09497; 
  xsec_[ 520.0] = 0.08568; 
  xsec_[ 540.0] = 0.07746; 
  xsec_[ 560.0] = 0.07010; 
  xsec_[ 580.0] = 0.06353; 
  xsec_[ 600.0] = 0.05771; 
  xsec_[ 620.0] = 0.05246; 
  xsec_[ 640.0] = 0.04776; 
  xsec_[ 660.0] = 0.04356; 
  xsec_[ 680.0] = 0.03977; 
  xsec_[ 700.0] = 0.03637; 
  xsec_[ 720.0] = 0.03330; 
  xsec_[ 740.0] = 0.03052; 
  xsec_[ 760.0] = 0.02805; 
  xsec_[ 780.0] = 0.02580; 
  xsec_[ 800.0] = 0.02373; 
  xsec_[ 820.0] = 0.02188; 
  xsec_[ 840.0] = 0.02018; 
  xsec_[ 860.0] = 0.01864; 
  xsec_[ 880.0] = 0.01724; 
  xsec_[ 900.0] = 0.01597; 
  xsec_[ 920.0] = 0.01479; 
  xsec_[ 940.0] = 0.01375; 
  xsec_[ 960.0] = 0.01275; 
  xsec_[ 980.0] = 0.01186; 
  xsec_[1000.0] = 0.01104; 
}

inline void
CrossSection::WH()
{
  xsec_[ 90.0] = 1.640; 	
  xsec_[ 95.0] = 1.392;	
  xsec_[100.0] = 1.186; 	
  xsec_[105.0] = 1.018; 	
  xsec_[110.0] = 0.8754; 	
  xsec_[110.5] = 0.8623; 	
  xsec_[111.0] = 0.8495; 	
  xsec_[111.5] = 0.8368; 	
  xsec_[112.0] = 0.8244; 	
  xsec_[112.5] = 0.8122; 	
  xsec_[113.0] = 0.8003; 	
  xsec_[113.5] = 0.7885; 	
  xsec_[114.0] = 0.7770; 	
  xsec_[114.5] = 0.7657; 	
  xsec_[115.0] = 0.7546; 	
  xsec_[115.5] = 0.7439; 	
  xsec_[116.0] = 0.7333; 	
  xsec_[116.5] = 0.7230; 	
  xsec_[117.0] = 0.7129; 	
  xsec_[117.5] = 0.7030; 	
  xsec_[118.0] = 0.6933; 	
  xsec_[118.5] = 0.6837; 	
  xsec_[119.0] = 0.6744; 	
  xsec_[119.5] = 0.6651; 	
  xsec_[120.0] = 0.6561; 	
  xsec_[120.5] = 0.6472; 	
  xsec_[121.0] = 0.6384; 	
  xsec_[121.5] = 0.6297; 	
  xsec_[122.0] = 0.6212; 	
  xsec_[122.5] = 0.6129; 	
  xsec_[123.0] = 0.6046; 	
  xsec_[123.5] = 0.5965; 	
  xsec_[124.0] = 0.5885; 	
  xsec_[124.5] = 0.5806; 	
  xsec_[125.0] = 0.5729; 	
  xsec_[125.5] = 0.5652; 	
  xsec_[126.0] = 0.5576; 	
  xsec_[126.5] = 0.5501; 	
  xsec_[127.0] = 0.5428; 	
  xsec_[127.5] = 0.5355; 	
  xsec_[128.0] = 0.5284; 	
  xsec_[128.5] = 0.5213; 	
  xsec_[129.0] = 0.5144; 	
  xsec_[129.5] = 0.5075; 	
  xsec_[130.0] = 0.5008; 	
  xsec_[130.5] = 0.4942; 	
  xsec_[131.0] = 0.4877; 	
  xsec_[131.5] = 0.4813; 	
  xsec_[132.0] = 0.4749; 	
  xsec_[132.5] = 0.4687; 	
  xsec_[133.0] = 0.4626; 	
  xsec_[133.5] = 0.4566; 	
  xsec_[134.0] = 0.4506; 	
  xsec_[134.5] = 0.4448; 	
  xsec_[135.0] = 0.4390; 	
  xsec_[135.5] = 0.4333; 	
  xsec_[136.0] = 0.4277; 	
  xsec_[136.5] = 0.4221; 	
  xsec_[137.0] = 0.4167; 	
  xsec_[137.5] = 0.4113; 	
  xsec_[138.0] = 0.4060; 	
  xsec_[138.5] = 0.4008; 	
  xsec_[139.0] = 0.3957; 	
  xsec_[139.5] = 0.3907; 	
  xsec_[140.0] = 0.3857; 	
  xsec_[141.0] = 0.3761; 	
  xsec_[142.0] = 0.3669; 	
  xsec_[143.0] = 0.3579; 	
  xsec_[144.0] = 0.3491; 	
  xsec_[145.0] = 0.3406; 	
  xsec_[146.0] = 0.3321; 	
  xsec_[147.0] = 0.3238; 	
  xsec_[148.0] = 0.3157; 	
  xsec_[149.0] = 0.3078; 	
  xsec_[150.0] = 0.3001; 	
  xsec_[151.0] = 0.2928; 	
  xsec_[152.0] = 0.2856; 	
  xsec_[153.0] = 0.2785; 	
  xsec_[154.0] = 0.2715; 	
  xsec_[155.0] = 0.2646; 	
  xsec_[156.0] = 0.2569; 	
  xsec_[157.0] = 0.2494; 	
  xsec_[158.0] = 0.2422; 	
  xsec_[159.0] = 0.2354; 	
  xsec_[160.0] = 0.2291; 	
  xsec_[162.0] = 0.2209; 	
  xsec_[164.0] = 0.2140; 	
  xsec_[166.0] = 0.2063; 	
  xsec_[168.0] = 0.1973; 	
  xsec_[170.0] = 0.1883; 	
  xsec_[172.0] = 0.1802; 	
  xsec_[174.0] = 0.1726; 	
  xsec_[176.0] = 0.1653; 	
  xsec_[178.0] = 0.1585; 	
  xsec_[180.0] = 0.1521; 	
  xsec_[182.0] = 0.1465; 	
  xsec_[184.0] = 0.1413; 	
  xsec_[186.0] = 0.1360; 	
  xsec_[188.0] = 0.1305; 	
  xsec_[190.0] = 0.1253; 	
  xsec_[192.0] = 0.1205; 	
  xsec_[194.0] = 0.1160; 	
  xsec_[196.0] = 0.1116; 	
  xsec_[198.0] = 0.1073; 	
  xsec_[200.0] = 0.1032; 	
  xsec_[202.0] = 0.09932; 
  xsec_[204.0] = 0.09563; 
  xsec_[206.0] = 0.09212; 
  xsec_[208.0] = 0.08877; 
  xsec_[210.0] = 0.08557; 
  xsec_[212.0] = 0.08248; 
  xsec_[214.0] = 0.07953; 
  xsec_[216.0] = 0.07671; 
  xsec_[218.0] = 0.07400; 
  xsec_[220.0] = 0.07142; 
  xsec_[222.0] = 0.06895; 
  xsec_[224.0] = 0.06658; 
  xsec_[226.0] = 0.06432; 
  xsec_[228.0] = 0.06214; 
  xsec_[230.0] = 0.06006; 
  xsec_[232.0] = 0.05805; 
  xsec_[234.0] = 0.05611; 
  xsec_[236.0] = 0.05425; 
  xsec_[238.0] = 0.05247; 
  xsec_[240.0] = 0.05075; 
  xsec_[242.0] = 0.04909; 
  xsec_[244.0] = 0.04750; 
  xsec_[246.0] = 0.04597; 
  xsec_[248.0] = 0.04450; 
  xsec_[250.0] = 0.04308; 
  xsec_[252.0] = 0.04171; 
  xsec_[254.0] = 0.04040; 
  xsec_[256.0] = 0.03913; 
  xsec_[258.0] = 0.03791; 
  xsec_[260.0] = 0.03674; 
  xsec_[262.0] = 0.03561; 
  xsec_[264.0] = 0.03451; 
  xsec_[266.0] = 0.03346; 
  xsec_[268.0] = 0.03244; 
  xsec_[270.0] = 0.03146; 
  xsec_[272.0] = 0.03050; 
  xsec_[274.0] = 0.02958; 
  xsec_[276.0] = 0.02869; 
  xsec_[278.0] = 0.02783; 
  xsec_[280.0] = 0.02700; 
  xsec_[282.0] = 0.02621; 
  xsec_[284.0] = 0.02545; 
  xsec_[286.0] = 0.02472; 
  xsec_[288.0] = 0.02401; 
  xsec_[290.0] = 0.02333; 
  xsec_[295.0] = 0.02168; 
  xsec_[300.0] = 0.02018; 
}

inline void
CrossSection::ZH()
{
  xsec_[ 90.0] = 0.8597; 	
  xsec_[ 95.0] = 0.7348; 	
  xsec_[100.0] = 0.6313; 	
  xsec_[105.0] = 0.5449; 	
  xsec_[110.0] = 0.4721; 	
  xsec_[110.5] = 0.4655; 	
  xsec_[111.0] = 0.4589; 	
  xsec_[111.5] = 0.4525; 	
  xsec_[112.0] = 0.4462; 	
  xsec_[112.5] = 0.4400; 	
  xsec_[113.0] = 0.4340; 	
  xsec_[113.5] = 0.4280; 	
  xsec_[114.0] = 0.4221; 	
  xsec_[114.5] = 0.4164; 	
  xsec_[115.0] = 0.4107; 	
  xsec_[115.5] = 0.4052; 	
  xsec_[116.0] = 0.3998; 	
  xsec_[116.5] = 0.3945; 	
  xsec_[117.0] = 0.3893; 	
  xsec_[117.5] = 0.3842; 	
  xsec_[118.0] = 0.3791; 	
  xsec_[118.5] = 0.3742; 	
  xsec_[119.0] = 0.3693; 	
  xsec_[119.5] = 0.3645; 	
  xsec_[120.0] = 0.3598; 	
  xsec_[120.5] = 0.3551; 	
  xsec_[121.0] = 0.3505; 	
  xsec_[121.5] = 0.3459; 	
  xsec_[122.0] = 0.3414; 	
  xsec_[122.5] = 0.3370; 	
  xsec_[123.0] = 0.3326; 	
  xsec_[123.5] = 0.3283; 	
  xsec_[124.0] = 0.3241; 	
  xsec_[124.5] = 0.3199; 	
  xsec_[125.0] = 0.3158; 	
  xsec_[125.5] = 0.3117; 	
  xsec_[126.0] = 0.3077; 	
  xsec_[126.5] = 0.3038; 	
  xsec_[127.0] = 0.2999; 	
  xsec_[127.5] = 0.2961; 	
  xsec_[128.0] = 0.2923; 	
  xsec_[128.5] = 0.2886; 	
  xsec_[129.0] = 0.2849; 	
  xsec_[129.5] = 0.2813; 	
  xsec_[130.0] = 0.2778; 	
  xsec_[130.5] = 0.2743; 	
  xsec_[131.0] = 0.2709; 	
  xsec_[131.5] = 0.2675; 	
  xsec_[132.0] = 0.2642; 	
  xsec_[132.5] = 0.2609; 	
  xsec_[133.0] = 0.2577; 	
  xsec_[133.5] = 0.2545; 	
  xsec_[134.0] = 0.2514; 	
  xsec_[134.5] = 0.2483; 	
  xsec_[135.0] = 0.2453; 	
  xsec_[135.5] = 0.2423; 	
  xsec_[136.0] = 0.2393; 	
  xsec_[136.5] = 0.2364; 	
  xsec_[137.0] = 0.2336; 	
  xsec_[137.5] = 0.2307; 	
  xsec_[138.0] = 0.2279; 	
  xsec_[138.5] = 0.2252; 	
  xsec_[139.0] = 0.2225; 	
  xsec_[139.5] = 0.2198; 	
  xsec_[140.0] = 0.2172; 	
  xsec_[141.0] = 0.2121; 	
  xsec_[142.0] = 0.2071; 	
  xsec_[143.0] = 0.2023; 	
  xsec_[144.0] = 0.1976; 	
  xsec_[145.0] = 0.1930; 	
  xsec_[146.0] = 0.1884; 	
  xsec_[147.0] = 0.1840; 	
  xsec_[148.0] = 0.1796; 	
  xsec_[149.0] = 0.1754; 	
  xsec_[150.0] = 0.1713; 	
  xsec_[151.0] = 0.1674; 	
  xsec_[152.0] = 0.1636; 	
  xsec_[153.0] = 0.1599; 	
  xsec_[154.0] = 0.1562; 	
  xsec_[155.0] = 0.1525; 	
  xsec_[156.0] = 0.1484; 	
  xsec_[157.0] = 0.1444; 	
  xsec_[158.0] = 0.1405; 	
  xsec_[159.0] = 0.1368; 	
  xsec_[160.0] = 0.1334; 	
  xsec_[162.0] = 0.1289; 	
  xsec_[164.0] = 0.1252; 	
  xsec_[166.0] = 0.1208; 	
  xsec_[168.0] = 0.1157; 	
  xsec_[170.0] = 0.1106; 	
  xsec_[172.0] = 0.1060; 	
  xsec_[174.0] = 0.1016; 	
  xsec_[176.0] = 0.09732; 
  xsec_[178.0] = 0.09310; 
  xsec_[180.0] = 0.08917; 
  xsec_[182.0] = 0.08589; 
  xsec_[184.0] = 0.08286; 
  xsec_[186.0] = 0.07981; 
  xsec_[188.0] = 0.07668; 
  xsec_[190.0] = 0.07366; 
  xsec_[192.0] = 0.07089; 
  xsec_[194.0] = 0.06826; 
  xsec_[196.0] = 0.06573; 
  xsec_[198.0] = 0.06330; 
  xsec_[200.0] = 0.06096; 
  xsec_[202.0] = 0.05872; 
  xsec_[204.0] = 0.05658; 
  xsec_[206.0] = 0.05453; 
  xsec_[208.0] = 0.05256; 
  xsec_[210.0] = 0.05068; 
  xsec_[212.0] = 0.04887; 
  xsec_[214.0] = 0.04713; 
  xsec_[216.0] = 0.04547; 
  xsec_[218.0] = 0.04388; 
  xsec_[220.0] = 0.04235; 
  xsec_[222.0] = 0.04089; 
  xsec_[224.0] = 0.03949; 
  xsec_[226.0] = 0.03814; 
  xsec_[228.0] = 0.03684; 
  xsec_[230.0] = 0.03560; 
  xsec_[232.0] = 0.03439; 
  xsec_[234.0] = 0.03323; 
  xsec_[236.0] = 0.03210; 
  xsec_[238.0] = 0.03103; 
  xsec_[240.0] = 0.02999; 
  xsec_[242.0] = 0.02900; 
  xsec_[244.0] = 0.02805; 
  xsec_[246.0] = 0.02713; 
  xsec_[248.0] = 0.02625; 
  xsec_[250.0] = 0.02540; 
  xsec_[252.0] = 0.02458; 
  xsec_[254.0] = 0.02379; 
  xsec_[256.0] = 0.02302; 
  xsec_[258.0] = 0.02229; 
  xsec_[260.0] = 0.02158; 
  xsec_[262.0] = 0.02089; 
  xsec_[264.0] = 0.02023; 
  xsec_[266.0] = 0.01960; 
  xsec_[268.0] = 0.01898; 
  xsec_[270.0] = 0.01839; 
  xsec_[272.0] = 0.01782; 
  xsec_[274.0] = 0.01727; 
  xsec_[276.0] = 0.01675; 
  xsec_[278.0] = 0.01624; 
  xsec_[280.0] = 0.01575; 
  xsec_[282.0] = 0.01528; 
  xsec_[284.0] = 0.01482; 
  xsec_[286.0] = 0.01438; 
  xsec_[288.0] = 0.01396; 
  xsec_[290.0] = 0.01355; 
  xsec_[295.0] = 0.01257; 
  xsec_[300.0] = 0.01169; 
}

inline void
CrossSection::ttH()
{
  xsec_[ 90.0] = 0.2162; 	
  xsec_[ 95.0] = 0.1880; 	
  xsec_[100.0] = 0.1638; 	
  xsec_[105.0] = 0.1433; 	
  xsec_[110.0] = 0.1257; 	
  xsec_[110.5] = 0.1241; 	
  xsec_[111.0] = 0.1225; 	
  xsec_[111.5] = 0.1209; 	
  xsec_[112.0] = 0.1194; 	
  xsec_[112.5] = 0.1179; 	
  xsec_[113.0] = 0.1164; 	
  xsec_[113.5] = 0.1149; 	
  xsec_[114.0] = 0.1134; 	
  xsec_[114.5] = 0.1120; 	
  xsec_[115.0] = 0.1106; 	
  xsec_[115.5] = 0.1092; 	
  xsec_[116.0] = 0.1078; 	
  xsec_[116.5] = 0.1065; 	
  xsec_[117.0] = 0.1051; 	
  xsec_[117.5] = 0.1038; 	
  xsec_[118.0] = 0.1025; 	
  xsec_[118.5] = 0.1013; 	
  xsec_[119.0] = 0.1000; 	
  xsec_[119.5] = 0.09878;
  xsec_[120.0] = 0.09756;
  xsec_[120.5] = 0.09636;
  xsec_[121.0] = 0.09518;
  xsec_[121.5] = 0.09402;
  xsec_[122.0] = 0.09287;
  xsec_[122.5] = 0.09174;
  xsec_[123.0] = 0.09063;
  xsec_[123.5] = 0.08954;
  xsec_[124.0] = 0.08846;
  xsec_[124.5] = 0.08739;
  xsec_[125.0] = 0.08634;
  xsec_[125.5] = 0.08530;
  xsec_[126.0] = 0.08428;
  xsec_[126.5] = 0.08327;
  xsec_[127.0] = 0.08227;
  xsec_[127.5] = 0.08129;
  xsec_[128.0] = 0.08032;
  xsec_[128.5] = 0.07937;
  xsec_[129.0] = 0.07842;
  xsec_[129.5] = 0.07750;
  xsec_[130.0] = 0.07658;
  xsec_[130.5] = 0.07568;
  xsec_[131.0] = 0.07479;
  xsec_[131.5] = 0.07391;
  xsec_[132.0] = 0.07304;
  xsec_[132.5] = 0.07219;
  xsec_[133.0] = 0.07135;
  xsec_[133.5] = 0.07052;
  xsec_[134.0] = 0.06970;
  xsec_[134.5] = 0.06890;
  xsec_[135.0] = 0.06810;
  xsec_[135.5] = 0.06731;
  xsec_[136.0] = 0.06654;
  xsec_[136.5] = 0.06577;
  xsec_[137.0] = 0.06502;
  xsec_[137.5] = 0.06428;
  xsec_[138.0] = 0.06355;
  xsec_[138.5] = 0.06282;
  xsec_[139.0] = 0.06211;
  xsec_[139.5] = 0.06141;
  xsec_[140.0] = 0.06072;
  xsec_[141.0] = 0.05937;
  xsec_[142.0] = 0.05807;
  xsec_[143.0] = 0.05680;
  xsec_[144.0] = 0.05556;
  xsec_[145.0] = 0.05435;
  xsec_[146.0] = 0.05316;
  xsec_[147.0] = 0.05200;
  xsec_[148.0] = 0.05087;
  xsec_[149.0] = 0.04976;
  xsec_[150.0] = 0.04869;
  xsec_[151.0] = 0.04765;
  xsec_[152.0] = 0.04663;
  xsec_[153.0] = 0.04564;
  xsec_[154.0] = 0.04468;
  xsec_[155.0] = 0.04374;
  xsec_[156.0] = 0.04283;
  xsec_[157.0] = 0.04194;
  xsec_[158.0] = 0.04108;
  xsec_[159.0] = 0.04024;
  xsec_[160.0] = 0.03942;
  xsec_[162.0] = 0.03783;
  xsec_[164.0] = 0.03632;
  xsec_[166.0] = 0.03488;
  xsec_[168.0] = 0.03350;
  xsec_[170.0] = 0.03219;
  xsec_[172.0] = 0.03094;
  xsec_[174.0] = 0.02975;
  xsec_[176.0] = 0.02862;
  xsec_[178.0] = 0.02755;
  xsec_[180.0] = 0.02652;
  xsec_[182.0] = 0.02553;
  xsec_[184.0] = 0.02459;
  xsec_[186.0] = 0.02370;
  xsec_[188.0] = 0.02286;
  xsec_[190.0] = 0.02206;
  xsec_[192.0] = 0.02128;
  xsec_[194.0] = 0.02052;
  xsec_[196.0] = 0.01981;
  xsec_[198.0] = 0.01913;
  xsec_[200.0] = 0.01849;
  xsec_[202.0] = 0.01787;
  xsec_[204.0] = 0.01727;
  xsec_[206.0] = 0.01670;
  xsec_[208.0] = 0.01615;
  xsec_[210.0] = 0.01562;
  xsec_[212.0] = 0.01512;
  xsec_[214.0] = 0.01463;
  xsec_[216.0] = 0.01417;
  xsec_[218.0] = 0.01373;
  xsec_[220.0] = 0.01330;
  xsec_[222.0] = 0.01289;
  xsec_[224.0] = 0.01251;
  xsec_[226.0] = 0.01213;
  xsec_[228.0] = 0.01178;
  xsec_[230.0] = 0.01143;
  xsec_[232.0] = 0.01110;
  xsec_[234.0] = 0.01077;
  xsec_[236.0] = 0.01046;
  xsec_[238.0] = 0.01016;
  xsec_[240.0] = 0.009873;
  xsec_[242.0] = 0.009597;
  xsec_[244.0] = 0.009331;
  xsec_[246.0] = 0.009076;
  xsec_[248.0] = 0.008830;
  xsec_[250.0] = 0.008593;
  xsec_[252.0] = 0.008363;
  xsec_[254.0] = 0.008142;
  xsec_[256.0] = 0.007928;
  xsec_[258.0] = 0.007722;
  xsec_[260.0] = 0.007524;
  xsec_[262.0] = 0.007333;
  xsec_[264.0] = 0.007149;
  xsec_[266.0] = 0.006972;
  xsec_[268.0] = 0.006801;
  xsec_[270.0] = 0.006636;
  xsec_[272.0] = 0.006476;
  xsec_[274.0] = 0.006322;
  xsec_[276.0] = 0.006173;
  xsec_[278.0] = 0.006028;
  xsec_[280.0] = 0.005889;
  xsec_[282.0] = 0.005754;
  xsec_[284.0] = 0.005623;
  xsec_[286.0] = 0.005497;
  xsec_[288.0] = 0.005374;
  xsec_[290.0] = 0.005256;
  xsec_[295.0] = 0.004975;
  xsec_[300.0] = 0.004719;
}

inline void
CrossSection::BR()
{
  xsec_[  90.0] = 8.41E-02;
  xsec_[  95.0] = 8.41E-02;
  xsec_[ 100.0] = 8.36E-02;
  xsec_[ 105.0] = 8.25E-02;
  xsec_[ 110.0] = 8.02E-02;
  xsec_[ 110.5] = 7.99E-02;
  xsec_[ 111.0] = 7.96E-02;
  xsec_[ 111.5] = 7.93E-02;
  xsec_[ 112.0] = 7.89E-02;
  xsec_[ 112.5] = 7.86E-02;
  xsec_[ 113.0] = 7.82E-02;
  xsec_[ 113.5] = 7.78E-02;
  xsec_[ 114.0] = 7.74E-02;
  xsec_[ 114.5] = 7.69E-02;
  xsec_[ 115.0] = 7.65E-02;
  xsec_[ 115.5] = 7.60E-02;
  xsec_[ 116.0] = 7.55E-02;
  xsec_[ 116.5] = 7.50E-02;
  xsec_[ 117.0] = 7.45E-02;
  xsec_[ 117.5] = 7.40E-02;
  xsec_[ 118.0] = 7.34E-02;
  xsec_[ 118.5] = 7.28E-02;
  xsec_[ 119.0] = 7.22E-02;
  xsec_[ 119.5] = 7.16E-02;
  xsec_[ 120.0] = 7.10E-02;
  xsec_[ 120.5] = 7.03E-02;
  xsec_[ 121.0] = 6.97E-02;
  xsec_[ 121.5] = 6.90E-02;
  xsec_[ 122.0] = 6.83E-02;
  xsec_[ 122.5] = 6.75E-02;
  xsec_[ 123.0] = 6.68E-02;
  xsec_[ 123.5] = 6.60E-02;
  xsec_[ 124.0] = 6.53E-02;
  xsec_[ 124.5] = 6.45E-02;
  xsec_[ 125.0] = 6.37E-02;
  xsec_[ 125.5] = 6.28E-02;
  xsec_[ 126.0] = 6.20E-02;
  xsec_[ 126.5] = 6.12E-02;
  xsec_[ 127.0] = 6.03E-02;
  xsec_[ 127.5] = 5.94E-02;
  xsec_[ 128.0] = 5.85E-02;
  xsec_[ 128.5] = 5.76E-02;
  xsec_[ 129.0] = 5.67E-02;
  xsec_[ 129.5] = 5.58E-02;
  xsec_[ 130.0] = 5.48E-02;
  xsec_[ 130.5] = 5.39E-02;
  xsec_[ 131.0] = 5.29E-02;
  xsec_[ 131.5] = 5.20E-02;
  xsec_[ 132.0] = 5.10E-02;
  xsec_[ 132.5] = 5.01E-02;
  xsec_[ 133.0] = 4.91E-02;
  xsec_[ 133.5] = 4.81E-02;
  xsec_[ 134.0] = 4.71E-02;
  xsec_[ 134.5] = 4.61E-02;
  xsec_[ 135.0] = 4.52E-02;
  xsec_[ 135.5] = 4.42E-02;
  xsec_[ 136.0] = 4.32E-02;
  xsec_[ 136.5] = 4.22E-02;
  xsec_[ 137.0] = 4.12E-02;
  xsec_[ 137.5] = 4.02E-02;
  xsec_[ 138.0] = 3.93E-02;
  xsec_[ 138.5] = 3.83E-02;
  xsec_[ 139.0] = 3.73E-02;
  xsec_[ 139.5] = 3.63E-02;
  xsec_[ 140.0] = 3.54E-02;
  xsec_[ 141.0] = 3.35E-02;
  xsec_[ 142.0] = 3.16E-02;
  xsec_[ 143.0] = 2.97E-02;
  xsec_[ 144.0] = 2.79E-02;
  xsec_[ 145.0] = 2.61E-02;
  xsec_[ 146.0] = 2.44E-02;
  xsec_[ 147.0] = 2.27E-02;
  xsec_[ 148.0] = 2.11E-02;
  xsec_[ 149.0] = 1.94E-02;
  xsec_[ 150.0] = 1.78E-02;
  xsec_[ 151.0] = 1.63E-02;
  xsec_[ 152.0] = 1.48E-02;
  xsec_[ 153.0] = 1.33E-02;
  xsec_[ 154.0] = 1.19E-02;
  xsec_[ 155.0] = 1.05E-02;
  xsec_[ 156.0] = 9.16E-03;
  xsec_[ 157.0] = 7.81E-03;
  xsec_[ 158.0] = 6.48E-03;
  xsec_[ 159.0] = 5.18E-03;
  xsec_[ 160.0] = 3.96E-03;
  xsec_[ 162.0] = 2.27E-03;
  xsec_[ 164.0] = 1.57E-03;
  xsec_[ 166.0] = 1.24E-03;
  xsec_[ 168.0] = 1.05E-03;
  xsec_[ 170.0] = 9.19E-04;
  xsec_[ 172.0] = 8.25E-04;
  xsec_[ 174.0] = 7.51E-04;
  xsec_[ 176.0] = 6.90E-04;
  xsec_[ 178.0] = 6.36E-04;
  xsec_[ 180.0] = 5.87E-04;
  xsec_[ 182.0] = 5.34E-04;
  xsec_[ 184.0] = 4.81E-04;
  xsec_[ 186.0] = 4.37E-04;
  xsec_[ 188.0] = 4.03E-04;
  xsec_[ 190.0] = 3.76E-04;
  xsec_[ 192.0] = 3.53E-04;
  xsec_[ 194.0] = 3.33E-04;
  xsec_[ 196.0] = 3.16E-04;
  xsec_[ 198.0] = 3.01E-04;
  xsec_[ 200.0] = 2.87E-04;
  xsec_[ 202.0] = 2.75E-04;
  xsec_[ 204.0] = 2.63E-04;
  xsec_[ 206.0] = 2.53E-04;
  xsec_[ 208.0] = 2.43E-04;
  xsec_[ 210.0] = 2.34E-04;
  xsec_[ 212.0] = 2.25E-04;
  xsec_[ 214.0] = 2.18E-04;
  xsec_[ 216.0] = 2.10E-04;
  xsec_[ 218.0] = 2.03E-04;
  xsec_[ 220.0] = 1.96E-04;
  xsec_[ 222.0] = 1.90E-04;
  xsec_[ 224.0] = 1.84E-04;
  xsec_[ 226.0] = 1.78E-04;
  xsec_[ 228.0] = 1.73E-04;
  xsec_[ 230.0] = 1.68E-04;
  xsec_[ 232.0] = 1.63E-04;
  xsec_[ 234.0] = 1.58E-04;
  xsec_[ 236.0] = 1.54E-04;
  xsec_[ 238.0] = 1.50E-04;
  xsec_[ 240.0] = 1.45E-04;
  xsec_[ 242.0] = 1.42E-04;
  xsec_[ 244.0] = 1.38E-04;
  xsec_[ 246.0] = 1.34E-04;
  xsec_[ 248.0] = 1.31E-04;
  xsec_[ 250.0] = 1.27E-04;
  xsec_[ 252.0] = 1.24E-04;
  xsec_[ 254.0] = 1.21E-04;
  xsec_[ 256.0] = 1.18E-04;
  xsec_[ 258.0] = 1.15E-04;
  xsec_[ 260.0] = 1.12E-04;
  xsec_[ 262.0] = 1.10E-04;
  xsec_[ 264.0] = 1.07E-04;
  xsec_[ 266.0] = 1.05E-04;
  xsec_[ 268.0] = 1.02E-04;
  xsec_[ 270.0] = 1.00E-04;
  xsec_[ 272.0] = 9.79E-05;
  xsec_[ 274.0] = 9.58E-05;
  xsec_[ 276.0] = 9.37E-05;
  xsec_[ 278.0] = 9.17E-05;
  xsec_[ 280.0] = 8.98E-05;
  xsec_[ 282.0] = 8.79E-05;
  xsec_[ 284.0] = 8.61E-05;
  xsec_[ 286.0] = 8.43E-05;
  xsec_[ 288.0] = 8.26E-05;
  xsec_[ 290.0] = 8.09E-05;
  xsec_[ 295.0] = 7.70E-05;
  xsec_[ 300.0] = 7.34E-05;
  xsec_[ 305.0] = 7.00E-05;
  xsec_[ 310.0] = 6.68E-05;
  xsec_[ 315.0] = 6.39E-05;
  xsec_[ 320.0] = 6.12E-05;
  xsec_[ 325.0] = 5.86E-05;
  xsec_[ 330.0] = 5.63E-05;
  xsec_[ 335.0] = 5.41E-05;
  xsec_[ 340.0] = 5.20E-05;
  xsec_[ 345.0] = 5.02E-05;
  xsec_[ 350.0] = 4.76E-05;
  xsec_[ 360.0] = 4.23E-05;
  xsec_[ 370.0] = 3.78E-05;
  xsec_[ 380.0] = 3.40E-05;
  xsec_[ 390.0] = 3.09E-05;
  xsec_[ 400.0] = 2.84E-05;
  xsec_[ 420.0] = 2.43E-05;
  xsec_[ 440.0] = 2.12E-05;
  xsec_[ 460.0] = 1.88E-05;
  xsec_[ 480.0] = 1.69E-05;
  xsec_[ 500.0] = 1.53E-05;
  xsec_[ 520.0] = 1.40E-05;
  xsec_[ 540.0] = 1.28E-05;
  xsec_[ 560.0] = 1.18E-05;
  xsec_[ 580.0] = 1.10E-05;
  xsec_[ 600.0] = 1.02E-05;
  xsec_[ 610.0] = 9.86E-06;
  xsec_[ 620.0] = 9.53E-06;
  xsec_[ 630.0] = 9.21E-06;
  xsec_[ 640.0] = 8.91E-06;
  xsec_[ 650.0] = 8.63E-06;
  xsec_[ 660.0] = 8.35E-06;
  xsec_[ 670.0] = 8.09E-06;
  xsec_[ 680.0] = 7.84E-06;
  xsec_[ 690.0] = 7.60E-06;
  xsec_[ 700.0] = 7.37E-06;
  xsec_[ 710.0] = 7.15E-06;
  xsec_[ 720.0] = 6.94E-06;
  xsec_[ 730.0] = 6.74E-06;
  xsec_[ 740.0] = 6.55E-06;
  xsec_[ 750.0] = 6.36E-06;
  xsec_[ 760.0] = 6.18E-06;
  xsec_[ 770.0] = 6.00E-06;
  xsec_[ 780.0] = 5.83E-06;
  xsec_[ 790.0] = 5.67E-06;
  xsec_[ 800.0] = 5.52E-06;
  xsec_[ 810.0] = 5.36E-06;
  xsec_[ 820.0] = 5.22E-06;
  xsec_[ 830.0] = 5.07E-06;
  xsec_[ 840.0] = 4.94E-06;
  xsec_[ 850.0] = 4.80E-06;
  xsec_[ 860.0] = 4.67E-06;
  xsec_[ 870.0] = 4.55E-06;
  xsec_[ 880.0] = 4.42E-06;
  xsec_[ 890.0] = 4.31E-06;
  xsec_[ 900.0] = 4.19E-06;
  xsec_[ 910.0] = 4.08E-06;
  xsec_[ 920.0] = 3.97E-06;
  xsec_[ 930.0] = 3.86E-06;
  xsec_[ 940.0] = 3.76E-06;
  xsec_[ 950.0] = 3.66E-06;
  xsec_[ 960.0] = 3.56E-06;
  xsec_[ 970.0] = 3.47E-06;
  xsec_[ 980.0] = 3.38E-06;
  xsec_[ 990.0] = 3.29E-06;
  xsec_[1000.0] = 3.20E-06;
}
#endif
