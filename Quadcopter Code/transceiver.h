#ifndef transceiver_h
#define transceiver_h
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "timer.h"

const byte addressess[][6] = {"00001", "00002"};

extern Timer trxTimerSendData;

RF24 radio(8, 9); //ce csn speed(012)

class transceiver{
  public:
    uint16_t JT_X_L_Val = 0;  
    uint16_t JT_Y_L_Val = 0;
    uint16_t JT_Y_R_Val = 0;
    uint16_t JT_X_R_Val = 0;
    uint16_t Pot_L_Val = 0;
    uint16_t Pot_R_Val = 0;
    uint16_t SW1_Val = 0;
    uint16_t SW2_Val = 0;
    const byte address[6] = "00001";

  public:
    transceiver(){
      JT_Y_L_Val = 0;
      JT_X_L_Val = 0;
      JT_Y_R_Val = 0;
      JT_X_R_Val = 0;
      Pot_L_Val = 0;
      Pot_R_Val = 0;
      SW1_Val = 0;
      SW2_Val = 0;      
    }

    void begin(){
      radio.begin();
      radio.begin();
      radio.setDataRate(RF24_2MBPS); //RF24_1MBPS RF24_250KBPS RF24_2MBPS
      radio.openReadingPipe(0, addressess[0]);
      radio.enableAckPayload();
      radio.startListening();
      float ackData[4] = {bs.getVoltage(), bs.getCurrent(), 0,0};
      radio.writeAckPayload(0, &ackData, sizeof(ackData));
    }

    void receive(){
      uint16_t text[8];
      if(radio.available()){
        radio.read(&text, sizeof(text));
        this->JT_X_L_Val = text[0];
        this->JT_Y_L_Val = text[1];
        this->JT_X_R_Val = text[2];
        this->JT_Y_R_Val = text[3];
        this->SW1_Val    = text[4];
        this->SW2_Val    = text[5];
        this->Pot_L_Val  = text[6];
        this->Pot_R_Val  = text[7];
      }
      float voltage = bs.getVoltage();
      float current = bs.getCurrent();
      float capacity = bs.getCapacity();
      float altitude = (float) ds.get_d5();
      float yaw = states.yaw;
      float pitch = states.pitch;
      float roll = states.roll;
      float calibration = states.calibration;
      
      float ackData[8] = {voltage, current, capacity, altitude, yaw, pitch, roll, calibration};
      radio.writeAckPayload(0, &ackData, sizeof(ackData));
    }

    void print(){
      Serial.print(JT_X_L_Val);
      Serial.print("  ");
      Serial.print(JT_Y_L_Val);
      Serial.print("  ");
      Serial.print(JT_X_R_Val);
      Serial.print("  ");
      Serial.print(JT_Y_R_Val);
      Serial.print("  ");
      Serial.print(SW1_Val);
      Serial.print("  ");
      Serial.print(SW2_Val);
      Serial.print("  ");
      Serial.print(Pot_L_Val); 
      Serial.print("  ");
      Serial.println(Pot_R_Val);
    }
    void get(uint16_t * a, uint16_t * b, uint16_t * c, uint16_t * d, uint16_t * e, uint16_t * f, uint16_t * g, uint16_t * h){
      *a = JT_X_L_Val;
      *b = JT_Y_L_Val;
      *c = JT_Y_R_Val;
      *d = JT_X_R_Val;
      *e = Pot_L_Val;
      *f = Pot_R_Val;
      *g = SW1_Val;
      *h = SW2_Val;
    }
} trx;

#endif