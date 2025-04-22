#ifndef timer_h
#define timer_h

#include <Arduino.h>

class Timer {
  private:
    long int m_timer;                    // Timer interval (in microseconds)
    long int m_currentTime;              // Current time (in microseconds)
    long int m_nextTime;                 // Time when the next check should happen
    long int m_stopwatchStartTime;       // Stopwatch start time (in microseconds)
    long int m_stopwatchStopTime;        // Stopwatch stop time (in microseconds)

  public:
    // Default constructor (100ms)
    Timer() : m_timer(100000), m_currentTime(0), m_nextTime(0) {}

    // Constructor with a custom time (in milliseconds)
    Timer(int time) : m_timer(time * 1000), m_currentTime(0), m_nextTime(0) {}

    // Check if the timer has elapsed (returns true if elapsed)
    bool check() {
      m_currentTime = micros();
      if (m_currentTime >= m_nextTime) {
        m_nextTime = m_currentTime + m_timer;
        return true;  // Timer elapsed
      } else {
        return false; // Timer has not elapsed yet
      }
    }

    // Start the stopwatch
    void start() {
      m_stopwatchStartTime = micros();
    }

    // Stop the stopwatch and optionally print the elapsed time
    long int stop(bool print = false) {
      m_stopwatchStopTime = micros();
      long int elapsedTime = m_stopwatchStopTime - m_stopwatchStartTime;

      if (print) {
        Serial.print("TIME = ");
        Serial.print(elapsedTime);
        Serial.println(" us");
      }

      return elapsedTime;
    }
};

extern Timer timer;

#endif
