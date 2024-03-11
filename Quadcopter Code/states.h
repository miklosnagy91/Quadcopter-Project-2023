#ifndef States_h
#define States_h

struct States{
  States(){
  yaw = -1;
  yawWanted = -1;
  yawOld = -1;
  pitch = -1;
  roll = -1; 
  calibration = -1;
  altitude = -1;
  pidYawInput = -1;
  pidRollInput = -1;
  pidPitchInput = -1;
  }
  
  float yaw;
  float yawWanted;
  float yawOld;
  float pitch;
  float roll; 
  uint8_t calibration;
  float altitude;
  float pidYawInput;
  float pidRollInput;
  float pidPitchInput;
}states;

#endif