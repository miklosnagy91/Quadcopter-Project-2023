#ifndef batteryState_h
#define batteryState_h

class batteryState{
  public:
    batteryState();
    void begin();
    void readVoltage();
    void readCurrent();
    void calculateCapacity();
    void read();
    void print();
    float getVoltage();
    float getCurrent();
    float getCapacity();
  private:
    float m_voltage;
    float m_current;
    float m_capacity;
};
extern batteryState bs;

#endif