#include "detail/structures.cpp"
#include "detail/config.cpp"

namespace packetHandler {

  packet packets[UPDATE_RATE];

  void init() {

  }

  void addPacket(packet p, int i) {
    packets[i - 1] = p;
  }
}