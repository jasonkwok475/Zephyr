#include "sensors/mpu.cpp"
#include "sensors/bmp.cpp"

#include "detail/structures.cpp" // TODO: Change to header file?
#include "HardwareSerial.h"

class Sensors {
  public:
    HardwareSerial& _Serial;
    MPU* mpu;
    BMP* bmp;

    Sensors(HardwareSerial& serial) :
      _Serial(serial)
    {
      mpu = new MPU(_Serial);
      bmp = new BMP(_Serial);
    }

    packet getPacket() {
      mpu->refreshData();
      bmp->refreshData();

      return packet {
        mpu->accel,
        mpu->rot_accel,
        mpu->position,
        mpu->angle,
        bmp->temperature,
        bmp->pressure,
        bmp->altitude,
        millis(),
      };
    }
};