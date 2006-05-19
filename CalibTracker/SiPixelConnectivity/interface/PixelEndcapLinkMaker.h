#ifndef PixelEndcapLinkMaker_H
#define PixelEndcapLinkMaker_H

#include <boost/cstdint.hpp>
#include <vector>

#include "CalibTracker/SiPixelConnectivity/interface/TRange.h"

class PixelModuleName;
class PixelEndcapName;
class PixelFEDCabling;
class PixelFEDLink;

class PixelEndcapLinkMaker {
public:
  typedef std::vector<PixelModuleName* > Names;
  typedef std::vector<uint32_t> DetUnits;
  typedef std::vector<PixelFEDLink *> Links;
  typedef TRange<int> Range;

  /// ctor from owner
  PixelEndcapLinkMaker(const PixelFEDCabling * o) : theOwner(o) { }

  /// construct links
  /// Each Endcap module triggers one or two link Items.
  /// They are sorted according to Order().
  /// The ROCs corresponding to items are created. The link is
  /// form from link items and ROCS.
  Links links( const Names & n, const DetUnits & u) const;

private:
  const PixelFEDCabling * theOwner;

  /// plaquette types. The order matters (first narrow modules than wider)
  enum Plaquette { v1x2, v1x5, v2x3, v2x4, v2x5 } ;

  /// link item.
  /// defined by DetUnit with name (representing Pixel Detector module)
  /// and Ids of associated ROCs. The Id is the ROC number in module according
  /// to PixelDatabase.
  /// The Item represents one module (DetUnit+name) with the Plaquette type
  /// and associated ROCs Ids given explicitly (Id is the ROC number in module)
  struct Item { const PixelEndcapName * name;
                uint32_t unit;
                Range rocIds;
                Plaquette type; };

  /// define order of link components.
  /// Highest priority to Endcap id (forward or backward endcpa)
  /// Second priority to disk id.
  /// Next blade
  /// Next panned
  /// To compare order of modules in one pannel plaquette id is used.
  /// The order of plaquettes define order of ROCs in link
  struct Order { bool operator() (const Item&, const Item&) const; } ;

private:

};

#endif


