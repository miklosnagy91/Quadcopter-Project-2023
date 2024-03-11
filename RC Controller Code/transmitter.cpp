#include <Arduino.h>
#include <SPI.h>
#include "transmitter.h"
#include "timer.h"
#include <LiquidCrystal.h>
#include <nRF24L01.h>
#include <RF24.h>


extern uint16_t JT_X_L_Val;
extern uint16_t JT_Y_L_Val;
extern uint16_t JT_X_L_Val;
extern uint16_t JT_Y_R_Val;
extern uint16_t JT_X_R_Val;
extern uint16_t Pot_L_Val;
extern uint16_t Pot_R_Val;
extern uint16_t SW1_Val;
extern uint16_t SW2_Val;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
RF24 radio(ce, csn, 8000000);

Timer lcdClearTimer(1000);
Timer d5Timer(250);
Timer lcdPrintTimer(10);

extern Timer lcdTimer, d5Timer;



transmitter::transmitter(){
  m_JT_Y_L_Val = 0;
  m_JT_X_L_Val = 0;
  m_JT_Y_R_Val = 0;
  m_JT_X_R_Val = 0;
  m_Pot_L_Val = 0;
  m_Pot_R_Val = 0;
  m_SW1_Val = 0;
  m_SW2_Val = 0;
  m_voltage = -1;
  m_current = -1;
  m_capacity = -1;
  m_d5 = -1;
  m_ack = 0;
  m_sendTime = -1;
  m_yaw;
  m_pitch;
  m_roll;
}

void transmitter::begin(){

  pinMode(SW1_Pin, INPUT);
  pinMode(SW2_Pin, INPUT);

  //LCD SETUP
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  lcd.print("Hello World!");
  delay(1000);
  lcd.clear();

  //TRANSMITTER SETUP
  radio.begin();
  radio.setRetries(5,0);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_2MBPS);//RF24_1MBPS RF24_250KBPS RF24_2MBPS
  radio.setPayloadSize(32);
  radio.enableAckPayload();
  radio.openWritingPipe(addressess[0]);

  //radio.setRetries(0,0);
  //radio.setPayloadSize(32);
  //radio.enableAckPayload();
  //radio.openReadingPipe(0, addressess[1]);
  //radio.openWritingPipe(addressess[0]);
  //radio.setPALevel(RF24_PA_MIN);
  //radio.setDataRate(RF24_2MBPS); //RF24_1MBPS RF24_250KBPS RF24_2MBPS
}

void transmitter::read(){
  this->m_JT_X_L_Val = 1023 - analogRead(JT_X_L_Pin);
  this->m_JT_Y_L_Val = analogRead(JT_Y_L_Pin);
  this->m_JT_X_R_Val = 1023 - analogRead(JT_X_R_Pin);
  this->m_JT_Y_R_Val = analogRead(JT_Y_R_Pin);
  this->m_SW1_Val = digitalRead(SW1_Pin);
  this->m_SW2_Val = digitalRead(SW2_Pin);
  this->m_Pot_L_Val = analogRead(Pot_L_Pin);
  this->m_Pot_R_Val = analogRead(Pot_R_Pin);
}

void transmitter::send(){
  uint32_t t1 = micros();
  float ackData[8];
  uint16_t data[8] = {m_JT_X_L_Val, m_JT_Y_L_Val, m_JT_X_R_Val, m_JT_Y_R_Val, m_SW1_Val, m_SW2_Val, m_Pot_L_Val, m_Pot_R_Val};

  m_ack = radio.write(&data, sizeof(data));
  uint32_t t2 = micros();

  if(m_ack){
    if(radio.isAckPayloadAvailable()) {
      radio.read(&ackData, sizeof(ackData));
      this->m_voltage = ackData[0];
      this->m_current = ackData[1];
      this->m_capacity = ackData[2];
      this->m_d5 = ackData[3];
      this->m_yaw = ackData[4];
      this->m_pitch = ackData[5];
      this->m_roll = ackData[6];
      this->m_cal = ackData[7];

    }
  }
  uint32_t t3 = micros();

  m_sendTime = t2 - t1;
  m_receiveTime = t3 - t2;
}

void transmitter::lcdPrint(){
  if(m_SW2_Val){
    //JOYSTICK VALUES
    lcd.setCursor(0, 0);
    lcd.print("JTX:");
    lcd.setCursor(4, 0);
    lcd.print(map(m_JT_X_L_Val,0,1003,-9,9));

    lcd.setCursor(0, 1);
    lcd.print("JTY:");
    lcd.setCursor(4, 1);
    lcd.print(map(m_JT_Y_L_Val,0,1003,-9,9));

    lcd.setCursor(6, 0);
    lcd.print("JTX:");
    lcd.setCursor(10, 0);
    lcd.print(map(m_JT_X_R_Val,0,1023,-9,9));

    lcd.setCursor(6, 1);
    lcd.print("JTY:");
    lcd.setCursor(10, 1);
    lcd.print(map(m_JT_Y_R_Val,0,1023,-9,9));


    //POTENTIOMETER VALUES
    lcd.setCursor(0, 2);
    lcd.print("LP:");
    lcd.setCursor(3, 2);
    lcd.print(map(m_Pot_L_Val,0,1023,1000,2000));
    lcd.setCursor(7, 2);
    lcd.print("RP:");
    lcd.setCursor(10, 2);
    lcd.print(m_Pot_R_Val);


    //SWITCH VALUES
    lcd.setCursor(0, 3);
    lcd.print("S1:");
    lcd.setCursor(3, 3);
    lcd.print(m_SW1_Val);
    lcd.setCursor(5, 3);
    lcd.print("S2:");
    lcd.setCursor(8, 3);
    lcd.print(m_SW2_Val);

    //PRINT VOLTAGE CURRENT HEIGHT VALUES
    lcd.setCursor(12, 0);
    lcd.print("V:");
    lcd.setCursor(14, 0);
    lcd.print(m_voltage);
    lcd.setCursor(12, 1);
    lcd.print("I:");
    lcd.setCursor(14, 1);
    lcd.print(m_current);
    lcd.setCursor(12, 3);
    lcd.print("H:");
    lcd.setCursor(14, 3);

    if(d5Timer.check()){
      m_d5_temp = (int) m_d5;
    }
    
    lcd.print(m_d5_temp);

    transmitter::clearLCD();
  }
  //YOU FUCKED UP 
  else{
    //if(lcdPrintTimer.check()){
    lcd.setCursor(0, 3);
    lcd.print("ST:");
    lcd.setCursor(3, 3);
    lcd.print(m_sendTime);

    lcd.setCursor(9, 3);
    lcd.print("RT:");
    lcd.setCursor(12, 3);
    lcd.print(m_receiveTime);

    lcd.setCursor(17, 3);
    lcd.print("A:");
    lcd.setCursor(19, 3);
    lcd.print((bool)m_ack);

    lcd.setCursor(0, 0);
    lcd.print("YAW:");
    lcd.setCursor(6, 0);
    lcd.print(m_yaw);

    lcd.setCursor(17, 0);
    lcd.print("S:");
    lcd.setCursor(19, 0);
    lcd.print((int) m_cal);

    lcd.setCursor(0, 1);
    lcd.print("PITCH:");
    lcd.setCursor(6, 1);
    lcd.print(m_pitch);

    lcd.setCursor(0, 2);
    lcd.print("ROLL:");
    lcd.setCursor(6, 2);
    lcd.print(m_roll);




    transmitter::clearLCD();
    //}

  }
}

void transmitter::clearLCD(){
  if(lcdClearTimer.check()) {
    lcd.clear();
  }
}


