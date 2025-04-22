#ifndef transceiver_h
#define transceiver_h

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "timer.h"

const byte addressess[][6] = {"00001", "00002"};

extern Timer trxTimerSendData;

RF24 radio(8, 9);  // CE, CSN pins for the radio module

class transceiver {
  public:
    uint16_t JT_X_L_Val = 0;  
    uint16_t JT_Y_L_Val = 0;
    uint16_t JT_Y_R_Val = 0;
    uint16_t JT_X_R_Val = 0;
    uint16_t Pot_L_Val = 0;
    uint16_t Pot_R_Val = 0;
    uint16_t SW1_Val = 0;
    uint16_t SW2_Val = 0;
    const byte address[6] = "00001";

  public:
    transceiver() {
      // Initialize all the values to 0 in the constructor (already initialized above)
    }

    void begin() {
      radio.begin();
      radio.setDataRate(RF24_2MBPS); // Set RF24 data rate (1MBPS, 2MBPS, or 250KBPS)
      radio.openReadingPipe(0, addressess[0]);
      radio.enableAckPayload(); // Enable ACK payload
      radio.startListening(); // Start listening for incoming data

      sendInitialAckData();  // Send initial data on start
    }

    void sendInitialAckData() {
      // Send initial acknowledgment data (voltage, current, capacity, etc.)
      float ackData[4] = {bs.getVoltage(), bs.getCurrent(), 0.0, 0.0};
      radio.writeAckPayload(0, &ackData, sizeof(ackData));
    }

    void receive() {
      uint16_t text[8];  // Array to store received data

      if (radio.available()) {
        radio.read(&text, sizeof(text));
        // Update the received control values
        JT_X_L_Val = text[0];
        JT_Y_L_Val = text[1];
        JT_X_R_Val = text[2];
        JT_Y_R_Val = text[3];
        SW1_Val = text[4];
        SW2_Val = text[5];
        Pot_L_Val = text[6];
        Pot_R_Val = text[7];
        
        sendAcknowledgment(); // Send acknowledgment data after receiving control data
      }
    }

    void sendAcknowledgment() {
      // Gather and send telemetry data to the transmitter
      float voltage = bs.getVoltage();
      float current = bs.getCurrent();
      float capacity = bs.getCapacity();
      float altitude = static_cast<float>(ds.get_d5());
      float yaw = states.yaw;
      float pitch = states.pitch;
      float roll = states.roll;
      float calibration = states.calibration;

      float ackData[8] = {voltage, current, capacity, altitude, yaw, pitch, roll, calibration};
      radio.writeAckPayload(0, &ackData, sizeof(ackData));
    }

    void print() {
      // Print the received control values for debugging purposes
      Serial.print("JT_X_L_Val: "); Serial.print(JT_X_L_Val); Serial.print("  ");
      Serial.print("JT_Y_L_Val: "); Serial.print(JT_Y_L_Val); Serial.print("  ");
      Serial.print("JT_X_R_Val: "); Serial.print(JT_X_R_Val); Serial.print("  ");
      Serial.print("JT_Y_R_Val: "); Serial.print(JT_Y_R_Val); Serial.print("  ");
      Serial.print("SW1_Val: "); Serial.print(SW1_Val); Serial.print("  ");
      Serial.print("SW2_Val: "); Serial.print(SW2_Val); Serial.print("  ");
      Serial.print("Pot_L_Val: "); Serial.print(Pot_L_Val); Serial.print("  ");
      Serial.println("Pot_R_Val: " + String(Pot_R_Val));
    }

    void get(uint16_t *a, uint16_t *b, uint16_t *c, uint16_t *d, uint16_t *e, uint16_t *f, uint16_t *g, uint16_t *h) {
      // Get all control values by passing in pointers to variables
      *a = JT_X_L_Val;
      *b = JT_Y_L_Val;
      *c = JT_Y_R_Val;
      *d = JT_X_R_Val;
      *e = Pot_L_Val;
      *f = Pot_R_Val;
      *g = SW1_Val;
      *h = SW2_Val;
    }
};

extern transceiver trx;

#endif
