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
  vec position;
  vec angle;
  float temperature;
  float pressure;
  float altitude;
  unsigned long time;
};

#endif