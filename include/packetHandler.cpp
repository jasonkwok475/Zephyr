#include "detail/structures.h"
#include "detail/config.cpp"

namespace packetHandler {

  packet packets[UPDATE_RATE];

  void init() {

  }

  void addPacket(packet p, int i) {
    packets[i] = p;
  }

  int packetSize() {
    return sizeof(packets);
  }
}