#ifndef distanceSensor_h
#define distanceSensor_h

#include "TCA9548A.h"
#include "Adafruit_VL53L1X.h"

#define VL53L1X_Address 0x29

Adafruit_VL53L1X vl53;
TCA9548A I2CMux;

class distanceSensor{
  private:
    int m_d1;
    int m_d2;
    int m_d3;
    int m_d4;
    int m_d5;
    int m_d6;

  public:
    distanceSensor(){}
    void begin(){
      I2CMux.begin(Wire1);
      Wire1.setClock(400000);
      I2CMux.openAll();
      vl53.begin(VL53L1X_Address, &Wire1);
      vl53.startRanging();
      vl53.setTimingBudget(50);
      I2CMux.closeAll();
      unsigned long timeout = 2;
      Wire1.setTimeout(timeout);
    };

    void read_d1(int * d1){
      I2CMux.openChannel(0);
      if(vl53.dataReady()){
        this->m_d1 = vl53.distance();
        vl53.clearInterrupt();
      }
      I2CMux.closeAll();
      (*d1) = m_d1;
    };

    void read_d2(int * d2){
      I2CMux.openChannel(1);
      if(vl53.dataReady()){
        this->m_d2 = vl53.distance();
        vl53.clearInterrupt();
      }
      I2CMux.closeAll();
      (*d2) = m_d2;
    };

    void read_d3(int * d3){
      vl53.startRanging();
      I2CMux.openChannel(2);
      if(vl53.dataReady()){
        this->m_d3 = vl53.distance();
        vl53.clearInterrupt();
      }
      I2CMux.closeAll();
      (*d3) = m_d3;
    };

    void read_d4(int * d4){
      I2CMux.openChannel(3);
      if(vl53.dataReady()){
        this->m_d4 = vl53.distance();
        vl53.clearInterrupt();
      }else vl53.clearInterrupt();
      I2CMux.closeAll();
      (*d4) = m_d4;
    };

    void read_d5(int * d5){ //HEIGHT
      I2CMux.openChannel(4);
      if(vl53.dataReady()){
        this->m_d5 = vl53.distance();
        vl53.clearInterrupt();
      }
      I2CMux.closeAll();
      (*d5) = m_d5;
    };

    void read_d6(int * d6){
      I2CMux.openChannel(5);
      if(vl53.dataReady()){
        this->m_d6 = vl53.distance();
        vl53.clearInterrupt();
      }
      I2CMux.closeAll();
      (*d6) = m_d6;
    };

    void read(int *d1, int *d2, int *d3, int *d4, int *d5, int *d6){
      read_d1(d1);
      read_d2(d2);
      read_d3(d3);
      read_d4(d4);
      read_d5(d5);
      read_d6(d6);
    }
    
    int16_t get_d5(){
      return m_d5;
    }
    void print(){
      Serial.print("d1 = ");
      Serial.print(m_d1);
      Serial.print(" d2 = ");
      Serial.print(m_d2);
      Serial.print(" d3 = ");
      Serial.print(m_d3);
      Serial.print(" d4 = ");
      Serial.print(m_d4);
      Serial.print(" d5 = ");
      Serial.print(m_d5);
      Serial.print(" d6 = ");
      Serial.println(m_d6);
    }
} ds;

#endif