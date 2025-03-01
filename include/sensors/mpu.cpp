#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <ArduinoJson.h>

#include "detail/structures.cpp"

Adafruit_MPU6050 mpu6050;
sensors_event_t a, g, temp;

// Offsets for MPU6050
vec gyroOffset = { -0.068186, 0.031862, 0.006056 };
vec accelOffset = { 0.660096, 0.080777, 10.959007 - 9.81 };

class MPU {
  public:
    bool connected = false;

    vec position = { 0, 0, 0 };
    vec accel = { 0, 0, 0 };
    vec angle = { 0, 0, 0 };
    vec rot_accel = { 0, 0, 0 };

    HardwareSerial& _Serial;

    MPU(HardwareSerial& serial) :
      _Serial(serial)
    {
      // Initialize MPU
      if (mpu6050.begin()) {
        _Serial.println("MPU6050 Found!");
        connected = true;
      } else _Serial.println("Failed to find MPU6050 chip");
    }
    
    void refreshData() {
      mpu6050.getEvent(&a, &g, &temp);

      accel.x = a.acceleration.x - accelOffset.x;
      accel.y = a.acceleration.y - accelOffset.y;
      accel.z = a.acceleration.z - accelOffset.z;
      rot_accel.x = g.gyro.x - gyroOffset.x;
      rot_accel.y = g.gyro.y - gyroOffset.y;
      rot_accel.z = g.gyro.z - gyroOffset.z;
    }

    String getGyroData() {
      
      //https://wiki.dfrobot.com/How_to_Use_a_Three-Axis_Accelerometer_for_Tilt_Sensing
      //https://forum.arduino.cc/t/guide-to-gyro-and-accelerometer-with-arduino-including-kalman-filtering/57971
      String gyro_data;
      JsonDocument doc; // allocates in the heap
      doc["x"] = rot_accel.x;
      doc["y"] = rot_accel.y;
      doc["z"] = rot_accel.z;
      doc["time"] = millis();
      serializeJson(doc, gyro_data);

      return gyro_data;
    }
};