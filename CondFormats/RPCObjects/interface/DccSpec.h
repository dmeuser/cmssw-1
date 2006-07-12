#ifndef CondFormatsRPCObjectsDccSpec_H
#define CondFormatsRPCObjectsDccSpec_H

/** \ class DccSpec
 * RPC DCC (==FED) specification for redout decoding
 */

#include <vector>
#include <boost/cstdint.hpp>
#include  "CondFormats/RPCObjects/interface/TriggerBoardSpec.h"

class ChamberLocationSpec;

class DccSpec {
public:
  /// ctor with ID only
  DccSpec(int id = -1);

  /// id of FED
  int id() const { return theId;}

  /// TB attached to channel
  const TriggerBoardSpec * triggerBoard(int channelNumber) const;

  /// attach TB to DCC. The channel is defined by TB
  void add(const TriggerBoardSpec & tb);

  /// debud printaout, call its components with depth dectreased by one
  void print(int depth = 0) const;

private:
  int theId;
  std::vector<TriggerBoardSpec> theTBs;

//  static const int MIN_CHANNEL_NUMBER = 1;
//  static const int NUMBER_OF_CHANNELS = 68;
};

#endif
