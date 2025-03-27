#include "bt.cpp"

#include "sensors.cpp"
#include "detail/config.cpp"
#include "packetHandler.cpp"
#include "detail/structures.h"
#include "pid.cpp"

using namespace std;

bool STARTED = true; // !TEMP
int count_since_last_updated = 0;
unsigned long _lasttime;

Sensors* sensors;
SensorEvents sens_events; // Whether Nova wants to be subscribed to certain events
PID roll_PID(1, 1, 1, 0, 4095); // kp, ki, kd

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  SerialBT.begin(NAME); 
  Serial.println("The device started, now you can pair it with bluetooth!");

  SerialBT.register_callback(BT::Bt_Status);

  analogWriteResolution(PWM_RES);
  analogWriteFrequency(PWM_FREQ);

  sensors = new Sensors();
  delay(1000);
  _lasttime = millis();
}

void loop() {
  if (SerialBT.available()) { // Try to condense this
    if (SerialBT.read() == START_CODE && STARTED == false) {
      STARTED = true;
      Serial.println("Recived start code from ground station.");
      _lasttime = millis();
    }
  }

  
  if (STARTED) { // if (!STARTED) return;
    unsigned long dt = millis() - _lasttime;
    sensors->refreshData(dt);

    double roll_thrust = roll_PID.compute(sensors->mpu->angle.x,0.00,millis());
    char buf[500];
    sprintf(buf, "PID: %lf, x: %f", roll_thrust, sensors->mpu->angle.x);
    Serial.println(buf);
    analogWrite(MOTOR, roll_thrust);

    // array of uint_8? for packet

    //BT::Bt_Data_Received(SerialBT.read());
    // if (count_since_last_updated == UPDATE_RATE) {
    //   count_since_last_updated = 0;
    //   packet p = sensors->getPacket();
    //   BT::Bt_Send_Packets(p, sizeof(p));
    // } else {
    //   packet p = sensors->getPacket();
    //   packetHandler::addPacket(p, count_since_last_updated);
    //   count_since_last_updated++;
    // }

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
// int vol = 0;
// for (vol = 0; vol <= 4095; vol += 1) { // goes from 0 degrees to 180 degrees
//   // in steps of 1 degree
//   analogWrite(MOTOR, vol);              // tell servo to go to position in variable 'pos'
//   delay(1);                       // waits 15ms for the servo to reach the position
// }
// for (vol = 4095; vol >= 0; vol -= 1) { // goes from 180 degrees to 0 degrees
//   analogWrite(MOTOR, vol);              // tell servo to go to position in variable 'pos'
//   delay(1);                       // waits 15ms for the servo to reach the position
// }
   }
  //if (!STARTED) return;

  // Internal refresh ~50ms
  // Radio/BT refresh ~500ms - send packets of information from the past 500ms


  //delay(REFRESH_RATE);
}