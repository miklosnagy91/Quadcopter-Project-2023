#ifndef Adafruit_YPR_h
#define Adafruit_YPR_h

#include <Arduino.h>
#include <Adafruit_BNO08x.h>

// ========================
// Global Sensor Definitions
// ========================
Adafruit_BNO08x bno08x(-1);  // BNO08x IMU sensor over I2C
sh2_SensorValue_t sensorValue;  // Struct for latest sensor data
sh2_SensorId_t reportType = SH2_ARVR_STABILIZED_RV;  // Sensor report type
long reportIntervalUs = 3000;  // Report interval in microseconds

// ========================
// Adafruit_YPR Class
// ========================
class Adafruit_YPR {
public:

  // Euler angle container
  struct euler_t {
    float yaw;
    float pitch;
    float roll;
  } ypr;

  // Initialize sensor
  void begin() {
    bno08x.begin_I2C();            // Begin I2C communication with IMU
    Wire.setClock(1000000);        // Set I2C clock speed to 1 MHz
    bno08x.enableReport(SH2_ARVR_STABILIZED_RV, reportIntervalUs);  // Enable rotation vector report
    delay(100);                    // Allow sensor to stabilize
  }

  // Convert quaternion values to Euler angles (yaw, pitch, roll)
  void quaternionToEuler(float qr, float qi, float qj, float qk, euler_t* ypr, bool degrees = false) {
    float sqr = sq(qr);
    float sqi = sq(qi);
    float sqj = sq(qj);
    float sqk = sq(qk);

    ypr->yaw   = atan2(2.0f * (qi * qj + qk * qr), (sqi - sqj - sqk + sqr));
    ypr->pitch = asin(-2.0f * (qi * qk - qj * qr) / (sqi + sqj + sqk + sqr));
    ypr->roll  = atan2(2.0f * (qj * qk + qi * qr), (-sqi - sqj + sqk + sqr));

    if (degrees) {
      ypr->yaw   *= RAD_TO_DEG;
      ypr->pitch *= RAD_TO_DEG;
      ypr->roll  *= RAD_TO_DEG;
    }
  }

  // Convert BNO08x rotation vector to Euler angles
  void quaternionToEulerRV(sh2_RotationVectorWAcc_t* rotational_vector, euler_t* ypr, bool degrees = false) {
    quaternionToEuler(rotational_vector->real, rotational_vector->i, rotational_vector->j, rotational_vector->k, ypr, degrees);
  }

  // Get current yaw, pitch, roll and calibration status from sensor
  void get(float* yaw, float* pitch, float* roll, uint8_t* calibration) {
    bno08x.getSensorEvent(&sensorValue);  // Get the latest sensor event
    quaternionToEulerRV(&sensorValue.un.arvrStabilizedRV, &ypr, true);  // Convert to Euler
    *yaw = ypr.yaw;
    *pitch = ypr.pitch;
    *roll = ypr.roll;
    *calibration = static_cast<uint8_t>(sensorValue.status);  // Sensor calibration level
  }

  // Print current orientation and calibration to Serial
  void print() {
    Serial.print("Yaw = ");
    Serial.print(ypr.yaw);
    Serial.print(" Pitch = ");
    Serial.print(ypr.pitch);
    Serial.print(" Roll = ");
    Serial.print(ypr.roll);
    Serial.print(" Calibration = ");
    Serial.println(sensorValue.status);
  }

private:
  // 
  float m_pitch;
  float m_roll;
  int m_cal;
};

#endif
