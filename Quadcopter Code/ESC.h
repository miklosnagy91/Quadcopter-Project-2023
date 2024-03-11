#ifndef ESC_h
#define ESC_h
#include <Servo.h>


#define esc1Pin 6 //3
#define esc2Pin 7 //4
#define esc3Pin 2 //1
#define esc4Pin 3 //2

Servo esc1;
Servo esc2;
Servo esc3;
Servo esc4;

class ESC{
  public:
    ESC(){
      Servo esc1;
      Servo esc2;
      Servo esc3;
      Servo esc4;
    }

    void begin(){
      esc1.attach(2,1000,2000);
      esc2.attach(3,1000,2000);
      esc3.attach(6,1000,2000);
      esc4.attach(7,1000,2000);

      esc1.writeMicroseconds(1000);
      esc2.writeMicroseconds(1000);
      esc3.writeMicroseconds(1000);
      esc4.writeMicroseconds(1000);
      delay(100);
    }

    void escPotWrite(int potValue){
      esc1.writeMicroseconds(map(potValue,0,1023,1000,2000));
      esc2.writeMicroseconds(map(potValue,0,1023,1000,2000));
      esc3.writeMicroseconds(map(potValue,0,1023,1000,2000));
      esc4.writeMicroseconds(map(potValue,0,1023,1000,2000));
    };

    void write1(int val){
      esc1.writeMicroseconds(val);
    };
    void write2(int val){
      esc2.writeMicroseconds(val);
    };
    void write3(int val){
      esc3.writeMicroseconds(val);
    };
    void write4(int val){
      esc4.writeMicroseconds(val);
    };
    off(){

    };

}esc;
#endif
//extern transceiver trx;