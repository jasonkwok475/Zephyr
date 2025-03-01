#include <Adafruit_BMP085.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_BMP085 bmp180;

class BMP {
  public:
    bool connected = false;

    float temperature;
    float pressure;
    float altitude;

    HardwareSerial& _Serial;

    BMP(HardwareSerial& serial) :
      _Serial(serial)
    {
      // Initialize BMP
      if (bmp180.begin()) {
        _Serial.println("BMP180 Found!");
        connected = true;
      } else _Serial.println("Failed to find BMP180 chip");
    }

    void refreshData() {
      temperature = bmp180.readTemperature();
      pressure = bmp180.readPressure();
      altitude = bmp180.readAltitude();
    }
};