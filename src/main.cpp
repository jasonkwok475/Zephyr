#include "bt.cpp"

#include "sensors.cpp"
#include "detail/config.cpp"
#include "packetHandler.cpp"

using namespace std;

bool STARTED = false;
int count_since_last_updated = 0;

Sensors* sensors;

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  SerialBT.begin(NAME); 
  Serial.println("The device started, now you can pair it with bluetooth!");

  SerialBT.register_callback(BT::Bt_Status);

  sensors = new Sensors(&Serial); // TODO: Use stream
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
    Serial.print("started!");
    //BT::Bt_Data_Received(SerialBT.read());
    analogWrite(17, 100);
    Serial.print(count_since_last_updated);
    if (count_since_last_updated == UPDATE_RATE) {
      count_since_last_updated = 0;
      BT::Bt_Send_Packets(packetHandler::packets);
    } else {
      count_since_last_updated++;
      packetHandler::addPacket(sensors->getPacket(), count_since_last_updated);
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
