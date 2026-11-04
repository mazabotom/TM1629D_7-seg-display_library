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

uint16_t volume = 12;


void setup() {
  LED_display.setBrightness(2);
}


void loop() {
  LED_display.setCursor(1); // cursor position 0-1-2-3
  LED_display.print("A");
  LED_display.println(volume); // expect _A12

  while(1);
}
