#ifndef batteryState_h
#define batteryState_h

// Class responsible for reading and tracking battery voltage, current, and estimated capacity
class batteryState {
  public:
    // Constructor
    batteryState();

    // Initializes pins and any necessary setup
    void begin();

    // Reads analog voltage input and converts to actual voltage
    void readVoltage();

    // Reads analog current input and converts to actual current
    void readCurrent();

    // Calculates estimated battery capacity (based on simple formula)
    void calculateCapacity();

    // Calls all reading and calculation functions in sequence
    void read();

    // Prints voltage, current, and capacity to Serial
    void print();

    // Getter for battery voltage
    float getVoltage();

    // Getter for battery current
    float getCurrent();

    // Getter for estimated battery capacity
    float getCapacity();

  private:
    float m_voltage;   // Stores the latest measured battery voltage
    float m_current;   // Stores the latest measured battery current
    float m_capacity;  // Stores the estimated battery capacity
};
