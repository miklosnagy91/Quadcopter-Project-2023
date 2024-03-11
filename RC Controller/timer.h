#include <Arduino.h>

#ifndef timer_h
#define timer_h

class Timer{
  private:
    int m_timer;
    int m_currentTime;
    int m_nextTime;
    int m_stopWacthStartTime;
    int m_stopWacthStopTime;
    int m_startTime;
    int m_stopTime;

  public:
    Timer() : m_timer(100),m_currentTime(0),m_nextTime(0){}
    Timer(int time) : m_timer(time),m_currentTime(0),m_nextTime(0){}

    bool check(){
      m_currentTime = millis();
      if(m_currentTime > m_nextTime){
        m_nextTime = m_currentTime + m_timer;
        return 1;
      }
      else{
        return 0;
      }
    }
    void start(){
      m_startTime = micros();
    }
    void stop(bool x){
      m_stopTime = micros();
      if(x){
        Serial.print("TIME = ");
        Serial.println(m_stopTime - m_startTime);
      }

    }
};

#endif
