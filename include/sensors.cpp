#include "sensors/mpu.cpp"
#include "sensors/bmp.cpp"

#include "detail/structures.h" 
#include "HardwareSerial.h"

class Sensors {
  public:
    MPU* mpu;
    BMP* bmp;

    Sensors() {
      mpu = new MPU();
      bmp = new BMP();
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