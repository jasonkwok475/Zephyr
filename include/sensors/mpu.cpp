#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include "detail/structures.h"
#include "detail/Kalman.h"

Adafruit_MPU6050 mpu6050;
sensors_event_t a, g, temp;

Kalman kalmanX;
Kalman kalmanY;

// Offsets for MPU6050
vec gyroOffset = { -0.068186, 0.031862, 0.006056 };
vec accelOffset = { 0.660096, 0.080777, 10.959007 - 9.81 };

class MPU {
  private:
    vec compAngle = { 0, 0, 0 }; // deg, comp. filter
    vec gyroAngle = { 0, 0, 0 }; // deg, gyro
    vec kalAngle = { 0, 0, 0 }; // deg, kalman filter
    double roll = 0; // x angle, -90 < x < 90
    double pitch = 0; // y angle
  public:
    bool connected = false;

    //vec position = { 0, 0, 0 }; // m
    vec accel = { 0, 0, 0 }; // m/s^2
    vec angle = { 0, 0, 0 }; // deg, raw
    vec omega = { 0, 0, 0 }; // deg/s, raw

    MPU() {
      // Initialize MPU
      if (mpu6050.begin()) {
        //_Serial.println("MPU6050 Found!");
        connected = true;
        refreshData();

        calculateRawAngles();
        kalmanX.setAngle(roll);
        kalmanY.setAngle(pitch);  
        compAngle.x = roll;
        compAngle.y = pitch;
        gyroAngle.x = roll;
        gyroAngle.y = pitch;
      } //else _Serial.println("Failed to find MPU6050 chip");
    }
    
    void refreshData(unsigned long dt = 0) {
      mpu6050.getEvent(&a, &g, &temp);

      accel.x = a.acceleration.x - accelOffset.x;
      accel.y = a.acceleration.y - accelOffset.y;
      accel.z = a.acceleration.z - accelOffset.z;
      omega.x = (g.gyro.x - gyroOffset.x) * RAD_TO_DEG;
      omega.y = (g.gyro.y - gyroOffset.y) * RAD_TO_DEG;
      omega.z = (g.gyro.z - gyroOffset.z) * RAD_TO_DEG;

      calculateRawAngles();
      calculateKalmanAngles(dt);
    }

    void calculateRawAngles() {
      roll  = atan(accel.y / sqrt(accel.x * accel.x + accel.z * accel.z)) * RAD_TO_DEG;
      pitch = atan2(-accel.x, accel.z) * RAD_TO_DEG;
    }

    void calculateKalmanAngles(unsigned long dt) {
      // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
      if ((pitch < -90 && kalAngle.y > 90) || (pitch > 90 && kalAngle.y < -90)) {
        kalmanY.setAngle(pitch);
        compAngle.y = pitch;
        kalAngle.y = pitch;
        gyroAngle.y = pitch;
      } else
        kalAngle.y = kalmanY.getAngle(pitch, omega.y, dt); // Calculate the angle using a Kalman filter

      if (abs(kalAngle.y) > 90)
        omega.x = -omega.x; // Invert rate, so it fits the restriced accelerometer reading
      kalAngle.x = kalmanX.getAngle(roll, omega.x, dt); // Calculate the angle using a Kalman filter

      // Calculate angles
      gyroAngle.x = omega.x * dt;
      gyroAngle.y = omega.y * dt;

      compAngle.x = 0.93 * (compAngle.x + omega.x * dt) + 0.07 * roll;
      compAngle.y = 0.93 * (compAngle.y + omega.y * dt) + 0.07 * pitch;

      // Reset the gyro angle when it has drifted too much
      if (omega.x < -180 || omega.x > 180)
        omega.x = kalAngle.x;
      if (omega.y < -180 || omega.y > 180)
        omega.y = kalAngle.y;

      angle.x = kalAngle.x;
      angle.y = kalAngle.y;
    }
};