#ifndef structures
#define structures 

struct vec {
  float x;
  float y;
  float z;
};

struct packet {
  vec accel;
  vec rot_accel;
  //vec position; // TODO: Change to gps position?
  vec angle;
  float temperature;
  float pressure;
  float altitude;
  unsigned long time;
};

struct SensorEvents {
  bool altitude = false;
  bool temperature = false;
  bool angle = false;
  bool throttle = false;
};

#endif