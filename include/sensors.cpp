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

    void refreshData(unsigned long dt) {
      mpu->refreshData(dt);
      bmp->refreshData();
    }

    packet getPacket() {
      return packet {
        mpu->accel,
        mpu->omega,
        mpu->angle,
        bmp->temperature,
        bmp->pressure,
        bmp->altitude,
        millis(),
      };
    }
};