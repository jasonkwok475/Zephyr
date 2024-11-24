#include <Adafruit_MPU6050.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <ArduinoJson.h>

Adafruit_MPU6050 mpu;
Adafruit_BMP085 bmp;

namespace MPU {
  bool connected = false;

  void init() {
    // Initialize MPU
    if (!mpu.begin()) return Serial.println("Failed to find MPU6050 chip");

    Serial.println("MPU6050 Found!");
    connected = true;
  }

  String getGyroData() {
    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    
    //https://wiki.dfrobot.com/How_to_Use_a_Three-Axis_Accelerometer_for_Tilt_Sensing
    //https://forum.arduino.cc/t/guide-to-gyro-and-accelerometer-with-arduino-including-kalman-filtering/57971
    String gyro_data;
    JsonDocument doc; // allocates in the heap
    doc["x"] = g.gyro.x;
    doc["y"] = g.gyro.y;
    doc["z"] = g.gyro.z;
    doc["time"] = millis();
    serializeJson(doc, gyro_data);

    return gyro_data;
  }
}

namespace BMP {
  bool connected = false;

  void init() {
    // Initialize BMP
    if (!bmp.begin()) return Serial.println("Failed to find BMP180 chip");
    
    Serial.println("BMP180 Found!");
    connected = true;
  }
}