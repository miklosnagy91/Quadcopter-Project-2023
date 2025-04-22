#ifndef ESC_h
#define ESC_h

#include <Servo.h>

// ============================================
// ESC Pin Definitions (these are motor outputs)
// ============================================
#define esc1Pin 6 // Motor 1
#define esc2Pin 7 // Motor 2
#define esc3Pin 2 // Motor 3
#define esc4Pin 3 // Motor 4

// ============================================
// Global Servo Instances for Each ESC
// ============================================
Servo esc1;
Servo esc2;
Servo esc3;
Servo esc4;

// ============================================
// ESC Class
// ============================================
class ESC {
public:

  // Constructor (empty in this case)
  ESC() {
    // Local redeclaration here does nothing; ESCs are already declared globally
  }

  // Initialize ESCs and set them to minimum throttle
  void begin() {
    esc1.attach(esc3Pin, 1000, 2000);  // Attach ESC to pin 6
    esc2.attach(esc4Pin, 1000, 2000);  // Attach ESC to pin 7
    esc3.attach(esc1Pin, 1000, 2000);  // Attach ESC to pin 2
    esc4.attach(esc2Pin, 1000, 2000);  // Attach ESC to pin 3

    // Set all ESCs to minimum throttle
    esc1.writeMicroseconds(1000);
    esc2.writeMicroseconds(1000);
    esc3.writeMicroseconds(1000);
    esc4.writeMicroseconds(1000);

    delay(100);  // Small delay to allow ESCs to initialize
  }

  // Write mapped pot value to all ESCs (used for manual throttle testing)
  void escPotWrite(int potValue) {
    int throttle = map(potValue, 0, 1023, 1000, 2000);
    esc1.writeMicroseconds(throttle);
    esc2.writeMicroseconds(throttle);
    esc3.writeMicroseconds(throttle);
    esc4.writeMicroseconds(throttle);
  }

  // Individual ESC control functions
  void write1(int val) { esc1.writeMicroseconds(val); }
  void write2(int val) { esc2.writeMicroseconds(val); }
  void write3(int val) { esc3.writeMicroseconds(val); }
  void write4(int val) { esc4.writeMicroseconds(val); }

  // Placeholder for future shutdown logic
  void off() {
    // You could implement motor kill logic here if needed
  }

} esc; // Create a global instance

#endif
