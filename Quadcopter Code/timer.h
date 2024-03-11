#include <Arduino.h>
#ifndef timer_h
#define timer_h

class Timer{
  private:
    long int m_timer;
    long int m_currentTime;
    long int m_nextTime;
    long int m_stopWacthStartTime;
    long int m_stopWacthStopTime;
  public:
    Timer() : m_timer(100000), m_currentTime(0), m_nextTime(0){}
    Timer(int time) : m_timer(time*1000),m_currentTime(0),m_nextTime(0){}

    bool check(){
      m_currentTime = micros();
      if(m_currentTime > m_nextTime){
        m_nextTime = m_currentTime + m_timer;
        return 1;
      }
      else{
        return 0;
      }
    }

    void start(){
      m_stopWacthStartTime = micros();
    }
    long int stop(bool print){
      m_stopWacthStopTime = micros();
      if(print){
        Serial.print("TIME = ");
        Serial.print(m_stopWacthStopTime - m_stopWacthStartTime);
        Serial.println(" us");
      }

      return (m_stopWacthStopTime - m_stopWacthStartTime);
    }
}timer;

#endif