#include "bt.cpp"

#include "sensors.cpp"
#include "detail/config.cpp"
#include "packetHandler.cpp"
#include "detail/structures.h"

using namespace std;

bool STARTED = false;
int count_since_last_updated = 0;

Sensors* sensors;
SensorEvents sens_events; // Whether Nova wants to be subscribed to certain events

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  SerialBT.begin(NAME); 
  Serial.println("The device started, now you can pair it with bluetooth!");

  SerialBT.register_callback(BT::Bt_Status);

  sensors = new Sensors();
  delay(1000);
}

void loop() {
  if (SerialBT.available()) { // Try to condense this
    if (SerialBT.read() == 83 && STARTED == false) {
      STARTED = true;
      Serial.println("Recived start code from ground station.");
    }
  }

  
  if (STARTED) {
    //BT::Bt_Data_Received(SerialBT.read());
    analogWrite(17, 100);
    if (count_since_last_updated == UPDATE_RATE) {
      count_since_last_updated = 0;
      packet p = sensors->getPacket();
      BT::Bt_Send_Packets(p, sizeof(p));
    } else {
      packet p = sensors->getPacket();
      packetHandler::addPacket(p, count_since_last_updated);
      count_since_last_updated++;
    }

// char accelbuffer[500];
// sprintf(accelbuffer, "Accel x:%f, y:%f, z:%f", MPU::accel.x, MPU::accel.y, MPU::accel.z);
// Serial.println(accelbuffer);
// char rotbuffer[500];
// sprintf(rotbuffer, "Accel Rot x:%f, y:%f, z:%f", MPU::offset_rot.x, MPU::offset_rot.y, MPU::offset_rot.z);
// Serial.println(rotbuffer);

// if (MPU::connected) BT::Bt_Send_Data(1, MPU::getGyroData());
// if (BMP::connected) BT::Bt_Send_Data(1, BMP::getData());

// Display current acceleration
// analogWrite(17, abs(MPU::accel.x)*255/20);
// analogWrite(16, abs(MPU::accel.y)*255/20);
// analogWrite(4, abs(MPU::accel.z)*255/20);
  }
  //if (!STARTED) return;

  // Internal refresh ~50ms
  // Radio/BT refresh ~500ms - send packets of information from the past 500ms


  delay(REFRESH_RATE);
}
