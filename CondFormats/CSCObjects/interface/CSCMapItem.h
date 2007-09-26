#ifndef CSCMapItem_h
#define CSCMapItem_h

#include <string>

class CSCMapItem{
 public:
  CSCMapItem();
  ~CSCMapItem();

  struct MapItem{
    std::string chamberLabel;
    int chamberId;
    int endcap;
    int station;
    int ring;
    int chamber;
    int cscIndex;
    int layerIndex;
    int stripIndex;
    int anodeIndex;
    int strips;
    int anodes;
    std::string crateLabel;
    int crateid;
    int sector;
    int trig_sector;
    int dmb;
    int cscid;
    int ddu;
    int ddu_input;
    int slink;
  };
};

#endif
