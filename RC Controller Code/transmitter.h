#ifndef transmitter_h
#define transmitter_h

const int SW1_Pin = 1, SW2_Pin = 0;
const int JT_Y_L_Pin = A0, JT_X_L_Pin = A1, JT_Y_R_Pin = A2, JT_X_R_Pin = A3;
const int Pot_L_Pin = A6, Pot_R_Pin = A7;
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
const int ce = 9, csn = 10;

const byte addressess[][6] = {"00001", "00002"};

class transmitter
{
  public:
    transmitter();
    void begin();
    void read();
    void send();
    //void receive();
    void lcdPrint();
    
  private:
    uint16_t m_JT_Y_L_Val;
    uint16_t m_JT_X_L_Val;
    uint16_t m_JT_Y_R_Val;
    uint16_t m_JT_X_R_Val;
    uint16_t m_Pot_L_Val;
    uint16_t m_Pot_R_Val;
    uint16_t m_SW1_Val;
    uint16_t m_SW2_Val;
    float m_voltage;
    float m_current;
    float m_capacity;
    float m_d5; int m_d5_temp;
    int m_recTime;
    uint32_t m_sendTime;
    uint32_t m_receiveTime;
    bool m_ack;
    float m_yaw;
    float m_pitch;
    float m_roll;
    float m_cal;
  private:
    void clearLCD();
};

#endif