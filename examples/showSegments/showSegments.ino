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


void setup() {
  LED_display.setBrightness(2);
  LED_display.setDisplayToDecNumber(1234);
  delay(2000);
}


void loop() {
  module.setSegments(0b00000001, 0); // segment a in position 0xxx
  delay(2000);

  module.setSegments(0b00000100, 1); // segment c in position x1xx
  delay(2000);

  module.setSegments(0b00010000, 2); // segment e in position xx2x
  delay(2000);

  module.setSegments(0b10000000, 3); // segment dp in position xxx3
  delay(2000);

  module.setSegments(0b00000000, 0); // off in position 0xxx
  module.setSegments(0b00000000, 1); // off in position x1xx
  module.setSegments(0b00000000, 2); // off in position xx2x
  module.setSegments(0b11111111, 3); // all in position xxx3
  while (true);
}
