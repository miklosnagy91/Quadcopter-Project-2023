#include "Arduino.h"
#include "batteryState.h"

#define voltagePin 15
#define currentPin 21

batteryState::batteryState(){}


void batteryState::readVoltage() {
  this->m_voltage = 0.0161*analogRead(voltagePin);
}

void batteryState::readCurrent() {
  this->m_current = 0.0645*analogRead(currentPin);
}

void batteryState::calculateCapacity() {
  this->m_capacity = (12.0 - m_voltage - 0.05*m_current);
}

void batteryState::read() {
  readVoltage();
  readCurrent();
  calculateCapacity();
}

void batteryState::begin(){
  pinMode(voltagePin, INPUT);
  pinMode(currentPin, INPUT);
}

float batteryState::getVoltage(){
  return this->m_voltage;
}

float batteryState::getCurrent(){
  return this->m_current;
}

float batteryState::getCapacity(){
  return this->m_capacity;
}

void batteryState::print(){
  Serial.print("Battery Voltage = ");
  Serial.print(m_voltage);
  Serial.print(" Battery Current = ");
  Serial.print(m_current);
  Serial.print(" Battery Capacity = ");
  Serial.println(m_capacity);
}

batteryState bs = batteryState();