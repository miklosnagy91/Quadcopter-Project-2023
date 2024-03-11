#ifndef Adafruit_YPR_h
#define Adafruit_YPR_h

#include <Arduino.h>
#include <Adafruit_BNO08x.h>

Adafruit_BNO08x  bno08x(-1);

sh2_SensorValue_t sensorValue;
sh2_SensorId_t reportType = SH2_ARVR_STABILIZED_RV;
long reportIntervalUs = 3000;


class Adafruit_YPR{
  public:
  struct euler_t {
    float yaw;
    float pitch;
    float roll;
  }ypr;


    void begin(){
      bno08x.begin_I2C();
      Wire.setClock(1000000);
      bno08x.enableReport(SH2_ARVR_STABILIZED_RV, reportIntervalUs);
      delay(100);
    }

    void quaternionToEuler(float qr, float qi, float qj, float qk, euler_t* ypr, bool degrees = false) {

        float sqr = sq(qr);
        float sqi = sq(qi);
        float sqj = sq(qj);
        float sqk = sq(qk);

        ypr->yaw = atan2(2.0 * (qi * qj + qk * qr), (sqi - sqj - sqk + sqr));
        ypr->pitch = asin(-2.0 * (qi * qk - qj * qr) / (sqi + sqj + sqk + sqr));
        ypr->roll = atan2(2.0 * (qj * qk + qi * qr), (-sqi - sqj + sqk + sqr));

        if (degrees) {
          ypr->yaw *= RAD_TO_DEG;
          ypr->pitch *= RAD_TO_DEG;
          ypr->roll *= RAD_TO_DEG;
        }
    }

    void quaternionToEulerRV(sh2_RotationVectorWAcc_t* rotational_vector, euler_t* ypr, bool degrees = false) {
        quaternionToEuler(rotational_vector->real, rotational_vector->i, rotational_vector->j, rotational_vector->k, ypr, degrees);
    }

    void get(float * yaw, float * pitch, float * roll, uint8_t * calibration) {
      bno08x.getSensorEvent(&sensorValue);
      quaternionToEulerRV(&sensorValue.un.arvrStabilizedRV, &ypr, true);
      *yaw = ypr.yaw;
      *pitch = ypr.pitch;
      *roll = ypr.roll;
      *calibration = (uint8_t) sensorValue.status;
    }

    void print(){
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
    float m_yaw;
    float m_pitch;
    float m_roll;
    int m_cal;
};

#endif