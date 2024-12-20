#include "bt.cpp"
#include "sensors.cpp"

bool STARTED = false;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(10);

  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  SerialBT.register_callback(BT::Bt_Status);

  MPU::init();
}

void loop() {
  if (SerialBT.available()) { // Try to condense this
    if (SerialBT.read() == 83 && STARTED == false) {
      STARTED = true;
      Serial.println("Recived start code from ground station.");
    }
    if (STARTED) BT::Bt_Data_Received(SerialBT.read());
  }
  if (!STARTED) return;

  // Internal refresh ~50ms
  // Radio/BT refresh ~500ms - send packets of information from the past 500ms

  if (MPU::connected) BT::Bt_Send_Data(1, MPU::getGyroData());
  if (BMP::connected) BT::Bt_Send_Data(1, BMP::getData());

  delay(100);
}
