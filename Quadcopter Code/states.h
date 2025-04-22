#ifndef States_h
#define States_h

#include <stdint.h>

struct States {
  // Constructor
  States()
    : yaw(-1), yawWanted(-1), yawOld(-1),
      pitch(-1), roll(-1), calibration(0),
      altitude(-1), pidYawInput(-1),
      pidRollInput(-1), pidPitchInput(-1) {}

  // Orientation
  float yaw;
  float yawWanted;
  float yawOld;
  float pitch;
  float roll;

  // Sensor status
  uint8_t calibration;

  // Environmental
  float altitude;

  // PID inputs
  float pidYawInput;
  float pidRollInput;
  float pidPitchInput;
};

// Declare a global instance
extern States states;

#endif
