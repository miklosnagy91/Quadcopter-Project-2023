#ifndef bmp280_h
#define bmp280_h

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

// Create a global BMP280 sensor object (using I2C interface)
Adafruit_BMP280 bmp;

// Create pointers to sensor-specific objects for temperature and pressure
Adafruit_Sensor* bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor* bmp_pressure = bmp.getPressureSensor();

// Class wrapper for BMP280 sensor
class bmp280 {
  private:
    float m_pressure;     // Stores latest pressure reading
    float m_temperature;  // Stores latest temperature reading
    float m_altitude;     // Stores calculated altitude

  public:
    // Initializes sensor and sets preferred sampling settings
    void begin() {
      if (!bmp.begin()) {
        Serial.println("Could not find a valid BMP280 sensor, check wiring!");
        while (1); // Stop program if sensor isn't found
      }

      bmp.setSampling(
        Adafruit_BMP280::MODE_NORMAL,      // Operating Mode
        Adafruit_BMP280::SAMPLING_X2,      // Temp. oversampling
        Adafruit_BMP280::SAMPLING_X16,     // Pressure oversampling
        Adafruit_BMP280::FILTER_X16,       // Filtering
        Adafruit_BMP280::STANDBY_MS_500    // Standby time
      );
    }

    // Reads data from the BMP280 sensor
    void read() {
      m_temperature = bmp.readTemperature();
      m_pressure = bmp.readPressure() / 100.0F; // Convert to hPa
      m_altitude = bmp.readAltitude(1013.25);   // Assumes sea level pressure is 1013.25 hPa
    }

    // Getters
    float getTemperature() { return m_temperature; }
    float getPressure() { return m_pressure; }
    float getAltitude() { return m_altitude; }

    // Debug print to Serial
    void print() {
      Serial.print("Temperature = ");
      Serial.print(m_temperature);
      Serial.print(" °C, Pressure = ");
      Serial.print(m_pressure);
      Serial.print(" hPa, Altitude = ");
      Serial.print(m_altitude);
      Serial.println(" m");
    }
};

#endif
