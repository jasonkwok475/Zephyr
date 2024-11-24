#include "BluetoothSerial.h"
#include <ArduinoJson.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

namespace BT {
  bool connected = false;

  // Bt_Status callback function
  void Bt_Status(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {

    if (event == ESP_SPP_SRV_OPEN_EVT) {
      Serial.println("Client Connected");

      // https://forum.arduino.cc/t/send-text-via-bluetooth/910499/6
    }

    else if (event == ESP_SPP_CLOSE_EVT ) {
      Serial.println("Client Disconnected");
      // Do stuff if not connected
    }
  }

  void Bt_Data_Received(uint8_t data) {

  }

  void Bt_Send_Data(int type, String data) {

    // Type 0: Log data
    // Type 1: Gyro
    String formatted;
    JsonDocument doc; // allocates in the heap
    doc["type"] = type;
    doc["data"] = data;
    serializeJson(doc, formatted);
    uint8_t buf[formatted.length()];

    memcpy(buf,formatted.c_str(),formatted.length());
    SerialBT.write(buf,formatted.length());
  }
};
