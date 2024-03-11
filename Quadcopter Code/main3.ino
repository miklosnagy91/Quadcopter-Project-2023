#include "States.h"
#include "batteryState.h"
#include "ESC.h"
#include "distanceSensor.h"
#include "timer.h"
#include "transceiver.h"
#include "Adafruit_YPR.h"
#include "PID.h"
/*
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
*/

struct radioController {
  radioController(){
    JT_X_L_Val = 0;  
    JT_Y_L_Val = 0;
    JT_Y_R_Val = 0;
    JT_X_R_Val = 0;
    Pot_L_Val = 0;
    Pot_R_Val = 0;
    SW1_Val = 0;
    SW2_Val = 0;
  }
  uint16_t JT_X_L_Val;  
  uint16_t JT_Y_L_Val;
  uint16_t JT_Y_R_Val;
  uint16_t JT_X_R_Val;
  uint16_t Pot_L_Val;
  uint16_t Pot_R_Val;
  uint16_t SW1_Val;
  uint16_t SW2_Val;
}rc;

//timer h_t500(500);
Timer dsTimer(200);
Timer trxTimerReceiveData(0);
Timer bsTimer(500);
Timer PIDPrintTimer(250);
Timer loopTimer(1);
Timer bno085Timer(0);


float PIDFrequency = 250;
float PIDPeriod = 1/PIDFrequency;
Timer PIDTimer(PIDPeriod*1000);




PID pidYaw(1,1,1,PIDPeriod);
PID pidPitch(.8,2,0,PIDPeriod);
PID pidRoll(.8,2,0,PIDPeriod);






Adafruit_YPR bno;

void setup() {
  analogWriteResolution(12);
  Serial.begin(115200);
  delay(10);
  Serial.println("HELLO1");
  bno.begin();
  ds.begin();
  bs.begin();
  trx.begin(); 
  esc.begin();
  //bno.get();
  delay(100);
}


void loop() {



//__________LOOP TIMER__________
loopTimer.stop(false);
loopTimer.start();
//________________________________________



//__________GET DISTANCE SENSOR MEASUREMENTS__________
/*dsTimer.start();
if(dsTimer.check()){
  int d1,d2,d3,d4,d5,d6;
  ds.read_d1(&d1);
  ds.read_d2(&d2);
  ds.read_d3(&d3);
  ds.read_d4(&d4);
  ds.read_d5(&d5);
ds.print();
}
dsTimer.stop(true);*/
//________________________________________




//__________BATTERY STATES__________
if(bsTimer.check()) {
  bsTimer.start();
  bs.read();
  bsTimer.stop(false);
}
//________________________________________




//__________TRANSMITTING AND RECEIVING DATA__________
if(trxTimerReceiveData.check()){
  trxTimerReceiveData.start();
  trx.receive();
  trx.get(&rc.JT_X_L_Val,&rc.JT_Y_L_Val,&rc.JT_Y_R_Val,&rc.JT_X_R_Val,&rc.Pot_L_Val,&rc.Pot_R_Val,&rc.SW1_Val,&rc.SW2_Val);
  //trx.print();
  long int x = trxTimerReceiveData.stop(false);
}
//________________________________________




//__________MAPPING RC RECEIVED DATA FOR PID INPUT__________
int x = 2000;
float esc1Input, esc2Input, esc3Input, esc4Input;
float yawChange = ((float)map(rc.JT_X_L_Val,7,1007,x,-x))/100;
float rollWanted= ((float)map(rc.JT_X_R_Val,26,1026,-x,x))/300;
float pitchWanted = ((float)map(rc.JT_Y_R_Val,13,1013,-x,x))/300;
float throttle = map(trx.Pot_L_Val,0,1023,0,1000);
//________________________________________



//__________GETTING YAW PITCH ROLL MEASUREMENTS__________
bno085Timer.start();
states.yawOld = states.yaw;
bno.get(&states.yaw, &states.pitch, &states.roll, &states.calibration);
//bno.print();
bno085Timer.stop(false);
//________________________________________




states.yawWanted = states.yawOld + yawChange;
if(states.yawWanted - states.yaw > 180) states.yawWanted = states.yawWanted - 360;
if(states.yawWanted - states.yaw < -180) states.yawWanted = states.yawWanted + 360;
//Serial.println(states.yawWanted);




//__________PID CONTROLLER FOR YAW PITCH AND ROLL__________
pidRoll.calculate(rollWanted, states.roll, &states.pidRollInput);
pidPitch.calculate(pitchWanted, states.pitch, &states.pidPitchInput);
pidYaw.calculate(states.yawWanted, states.yaw, &states.pidYawInput);
//pidYaw.print();
//________________________________________




esc1Input = 1000 + throttle; //- states.pidRollInput + states.pidPitchInput - states.pidYawInput;
esc2Input = 1000 + throttle; //+ states.pidRollInput + states.pidPitchInput + states.pidYawInput;
esc3Input = 1000 + throttle; //+ states.pidRollInput - states.pidPitchInput - states.pidYawInput;
esc4Input = 1000 + throttle; //- states.pidRollInput - states.pidPitchInput + states.pidYawInput;

if(esc1Input < 1050 || throttle == 0){
  esc1Input = 1000; esc2Input = 1000; esc3Input = 1000; esc4Input = 1000;
  //bno.get(&states.yaw, &states.pitch, &states.roll, &states.calibration);
  states.yawWanted = states.yaw;
  pidRoll.reset(); pidPitch.reset(); pidYaw.reset();
}
if(esc2Input < 1050 || throttle == 0) {
  esc1Input = 1000; esc2Input = 1000; esc3Input = 1000; esc4Input = 1000;
  //bno.get(&states.yaw, &states.pitch, &states.roll, &states.calibration);
  states.yawWanted = states.yaw;
  pidRoll.reset(); pidPitch.reset(); pidYaw.reset();
}

if(esc3Input < 1050 || throttle == 0){
  esc1Input = 1000; esc2Input = 1000; esc3Input = 1000; esc4Input = 1000;
  //bno.get(&states.yaw, &states.pitch, &states.roll, &states.calibration);
  states.yawWanted = states.yaw;
  pidRoll.reset(); pidPitch.reset(); pidYaw.reset();
}
if(esc4Input < 1050 || throttle == 0){
  esc1Input = 1000; esc2Input = 1000; esc3Input = 1000; esc4Input = 1000;
  //bno.get(&states.yaw, &states.pitch, &states.roll, &states.calibration);
  states.yawWanted = states.yaw;
  pidRoll.reset(); pidPitch.reset(); pidYaw.reset();
}

while(!PIDTimer.check()){} //PID timed to 4ms
PIDTimer.stop(false);
esc.write1(esc1Input);
esc.write2(esc2Input);
esc.write3(esc3Input);
esc.write4(esc4Input);
PIDTimer.start();
states.yawOld = states.yaw;



}
