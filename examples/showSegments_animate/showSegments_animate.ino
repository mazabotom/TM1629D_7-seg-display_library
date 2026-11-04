/*
*/
#define STB 10
#define CLK 11
#define DIO 12
//#define CommonCathode

#ifdef CommonCathode
#include "TM1629D_CC.h"
TM1629D_CC module(DIO, CLK, STB); // DIO, CLK, STB, intensity default (0=off, 1=min, 8=max)
#else
#include "TM1629D.h"
TM1629D module(DIO, CLK, STB); // DIO, CLK, STB, intensity default (0=off, 1=min, 8=max)
#endif
#include "TM16xxDisplay.h"
TM16xxDisplay LED_display(&module, 4);    // TM16xx object, number of digits on display module

uint16_t volume = 1234;
int num = 1;

void setup() {
  LED_display.setBrightness(2);
  LED_display.setDisplayToDecNumber(volume); // (right shrink)
  delay(500);
}


void loop() {
  if (num == 256) {
    delay(2000);
    module.setSegments(0, 0); delay(2000); // segments=0=0b00000000=OFF => no segments light
    module.setSegments(0, 1); delay(2000);
    module.setSegments(0, 2); delay(2000);
    module.setSegments(0, 3);
    while (1);
  }
  module.setSegments(num, 0); // segments combination in position 0xxx
  delay(25);

  module.setSegments(num, 1); // segments combination in position x1xx
  delay(25);

  module.setSegments(num, 2); // segments combination in position xx2x
  delay(25);

  module.setSegments(num, 3); // segments combination in position xxx3
  delay(25);

  num = num + 1;
}
