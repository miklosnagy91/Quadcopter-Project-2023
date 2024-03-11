#include "transmitter.h"
#include "timer.h"

transmitter xmtr;
Timer loopTimer();
void setup() {
  xmtr.begin();
}

void loop() {
  xmtr.read();
  xmtr.send();
  xmtr.lcdPrint();
}
