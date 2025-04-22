#include "Arduino.h"
#include "batteryState.h"

// Define analog input pins for voltage and current sensing
#define voltagePin 15
#define currentPin 21

// Constructor (empty as initialization is handled in begin())
batteryState::batteryState() {}

// Read battery voltage from analog pin and convert to actual voltage
void batteryState::readVoltage() {
  this->m_voltage = 0.0161 * analogRead(voltagePin); // Conversion factor calibrated to sensor
}

// Read battery current from analog pin and convert to actual current
void batteryState::readCurrent() {
  this->m_current = 0.0645 * analogRead(currentPin); // Conversion factor calibrated to sensor
}

// Estimate remaining battery capacity (basic linear model)
void batteryState::calculateCapacity() {
  this->m_capacity = (12.0 - m_voltage - 0.05 * m_current); // Capacity influenced by voltage drop and current draw
}

// Perform all battery state readings in one call
void batteryState::read() {
  readVoltage();
  readCurrent();
  calculateCapacity();
}

// Set up pins for battery monitoring
void batteryState::begin() {
  pinMode(voltagePin, INPUT);
  pinMode(currentPin, INPUT);
}

// Getter for voltage value
float batteryState::getVoltage() {
  return this->m_voltage;
}

// Getter for current value
float batteryState::getCurrent() {
  return this->m_current;
}

// Getter for capacity estimate
float batteryState::getCapacity() {
  return this->m_capacity;
}

// Print all battery data to serial
void batteryState::print() {
  Serial.print("Battery Voltage = ");
  Serial.print(m_voltage);
  Serial.print(" V | Battery Current = ");
  Serial.print(m_current);
  Serial.print(" A | Estimated Capacity = ");
  Serial.println(m_capacity);
}

// Global battery state object
batteryState bs = batteryState();
