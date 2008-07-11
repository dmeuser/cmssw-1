#include "EventFilter/CSCRawToDigi/interface/CSCTMBHeader2007.h"
#include "EventFilter/CSCRawToDigi/interface/CSCDMBHeader.h"
#include "FWCore/Utilities/interface/Exception.h"

CSCTMBHeader2007::CSCTMBHeader2007() 
{
    nHeaderFrames = 43;
    bzero(this, sizeInWords()*2);
    e0bline = 0x6E0B;
    b0cline = 0x6B0C;
    nTBins = 7;
    nCFEBs = 5;
}


CSCTMBHeader2007::CSCTMBHeader2007(const unsigned short * buf)
{
  memcpy(data(), buf, sizeInWords()*2);
}

  
void CSCTMBHeader2007::setEventInformation(const CSCDMBHeader & dmbHeader) 
{
    cscID = dmbHeader.dmbID();
    l1aNumber = dmbHeader.l1a();
    bxnCount = dmbHeader.bxn();
}

 ///returns CLCT digis
std::vector<CSCCLCTDigi> CSCTMBHeader2007::CLCTDigis(uint32_t idlayer) 
{
    std::vector<CSCCLCTDigi> result;
    int strip   = clct0_key;
    int cfeb    = (clct0_cfeb_low)|(clct0_cfeb_high<<1);
    int pattern = clct0_shape;
    int bend    = clct0_bend;
    //offlineStripNumbering(strip, cfeb, pattern, bend);
    CSCCLCTDigi digi0(clct0_valid, clct0_quality,
                      pattern, 1, bend, strip, cfeb, clct0_bxn, 1);
    digi0.setFullBX(bxnPreTrigger);

    strip = clct1_key;
    cfeb = (clct1_cfeb_low)|(clct1_cfeb_high<<1);
    pattern = clct1_shape;
    bend    = clct1_bend;
    //offlineStripNumbering(strip, cfeb, pattern, bend);
    CSCCLCTDigi digi1(clct1_valid, clct1_quality,
                      pattern, 1, bend, strip, cfeb, clct1_bxn, 2);
    digi1.setFullBX(bxnPreTrigger);

    //if (digi0.isValid() && digi1.isValid()) swapCLCTs(digi0, digi1);

    result.push_back(digi0);
    result.push_back(digi1);


    return result;
}

 ///returns CorrelatedLCT digis
std::vector<CSCCorrelatedLCTDigi> 
CSCTMBHeader2007::CorrelatedLCTDigis(uint32_t idlayer) const 
{
    std::vector<CSCCorrelatedLCTDigi> result;
    /// for the zeroth MPC word:
    int strip = MPC_Muon0_halfstrip_clct_pattern;//this goes from 0-159
    //offlineHalfStripNumbering(strip);
    CSCCorrelatedLCTDigi digi(1, MPC_Muon0_vpf_, MPC_Muon0_quality_,
                              MPC_Muon0_wire_, strip, MPC_Muon0_clct_pattern_,
                              MPC_Muon0_bend_, MPC_Muon0_bx_, 0,
                              MPC_Muon0_bc0_, MPC_Muon0_SyncErr_,
                              MPC_Muon0_cscid_low | (MPC_Muon0_cscid_bit4<<3));
    result.push_back(digi);
    /// for the first MPC word:
    strip = MPC_Muon1_halfstrip_clct_pattern;//this goes from 0-159
    //offlineHalfStripNumbering(strip);
    digi = CSCCorrelatedLCTDigi(2, MPC_Muon1_vpf_, MPC_Muon1_quality_,
                                MPC_Muon1_wire_, strip, MPC_Muon1_clct_pattern_,
                                MPC_Muon1_bend_, MPC_Muon1_bx_, 0,
                                MPC_Muon1_bc0_, MPC_Muon1_SyncErr_,
                                MPC_Muon1_cscid_low | (MPC_Muon1_cscid_bit4<<3));
    result.push_back(digi);
    return result;
}

void
CSCTMBHeader2007::addALCT0(const CSCALCTDigi & digi)
{
  throw cms::Exception("In CSC TMBHeaderFormat 2007, ALCTs belong in ALCT header");
}


void
CSCTMBHeader2007::addALCT1(const CSCALCTDigi & digi)
{
  throw cms::Exception("In CSC TMBHeaderFormat 2007, ALCTs belong in ALCT header");
}

void
CSCTMBHeader2007::addCLCT0(const CSCCLCTDigi & digi)
{
  int strip = digi.getStrip();
  int cfeb = digi.getCFEB();
  int bend = digi.getBend();
  int pattern = digi.getPattern();
  //hardwareStripNumbering(strip, cfeb, pattern, bend);
  clct0_valid = digi.isValid();
  clct0_quality = digi.getQuality();
  clct0_shape = pattern;
  clct0_bend = bend;
  clct0_key = strip;
  clct0_cfeb_low = (cfeb & 0x1);
  clct0_cfeb_high = (cfeb>>1);
  clct0_bxn = digi.getBX();
  bxnPreTrigger = digi.getFullBX();
}

void
CSCTMBHeader2007::addCLCT1(const CSCCLCTDigi & digi)
{
  int strip = digi.getStrip();
  int cfeb = digi.getCFEB();
  int bend = digi.getBend();
  int pattern = digi.getPattern();
  //hardwareStripNumbering(strip, cfeb, pattern, bend);
  clct1_valid = digi.isValid();
  clct1_quality = digi.getQuality();
  clct1_shape = pattern;
  clct1_bend = bend;
  clct1_key = strip;
  clct1_cfeb_low = (cfeb & 0x1);
  clct1_cfeb_high = (cfeb>>1);
  clct1_bxn = digi.getBX();
  bxnPreTrigger = digi.getFullBX();
}

void
CSCTMBHeader2007::addCorrelatedLCT0(const CSCCorrelatedLCTDigi & digi)
{
  int halfStrip = digi.getStrip();
  //hardwareHalfStripNumbering(halfStrip);

  MPC_Muon0_vpf_ = digi.isValid();
  MPC_Muon0_wire_ = digi.getKeyWG();
  MPC_Muon0_clct_pattern_ = digi.getPattern();
  MPC_Muon0_quality_ = digi.getQuality();
  MPC_Muon0_halfstrip_clct_pattern = halfStrip;
  MPC_Muon0_bend_ = digi.getBend();
  MPC_Muon0_SyncErr_ = digi.getSyncErr();
  MPC_Muon0_bx_ = digi.getBX();
  MPC_Muon0_bc0_ = digi.getBX0();
  MPC_Muon0_cscid_low = digi.getCSCID();
}

void
CSCTMBHeader2007::addCorrelatedLCT1(const CSCCorrelatedLCTDigi & digi)
{
  int halfStrip = digi.getStrip();
  //hardwareHalfStripNumbering(halfStrip);

  MPC_Muon1_vpf_ = digi.isValid();
  MPC_Muon1_wire_ = digi.getKeyWG();
  MPC_Muon1_clct_pattern_ = digi.getPattern();
  MPC_Muon1_quality_ = digi.getQuality();
  MPC_Muon1_halfstrip_clct_pattern = halfStrip;
  MPC_Muon1_bend_ = digi.getBend();
  MPC_Muon1_SyncErr_ = digi.getSyncErr();
  MPC_Muon1_bx_ = digi.getBX();
  MPC_Muon1_bc0_ = digi.getBX0();
  MPC_Muon1_cscid_low = digi.getCSCID();
}


void CSCTMBHeader2007::print(std::ostream & os) const
{
  os << "...............TMB Header.................." << "\n";
  os << std::hex << "BOC LINE " << b0cline << " EOB " << e0bline << "\n";
  os << std::dec << "fifoMode = " << fifoMode
     << ", nTBins = " << nTBins << "\n";
//  os << "dumpCFEBs = " << dumpCFEBs << ", nHeaderFrames = "
//     << nHeaderFrames << "\n";
  os << "boardID = " << boardID << ", cscID = " << cscID << "\n";
  os << "l1aNumber = " << l1aNumber << ", bxnCount = " << bxnCount << "\n";
//  os << "preTrigTBins = " << preTrigTBins << ", nCFEBs = "<< nCFEBs<< " ";
  os << "trigSourceVect = " << trigSourceVect
     << ", activeCFEBs = " << activeCFEBs <<"\n";
  os << "bxnPreTrigger = " << bxnPreTrigger << "\n";
  os << "tmbMatch = " << tmbMatch << " alctOnly = " << alctOnly
     << " clctOnly = " << clctOnly << "\n";
//     << " alctMatchTime = " << alctMatchTime << " ";
//  os << "hs_thresh = " << hs_thresh << ", ds_thresh = " << ds_thresh
//     << " ";
  os << "clct0_key = " << clct0_key << " clct0_shape = " << clct0_shape
     << " clct0_quality = " << clct0_quality << "\n";
//  os << "r_buf_nbusy = " << r_buf_nbusy << " ";

  os << "..................CLCT....................." << "\n";

}

