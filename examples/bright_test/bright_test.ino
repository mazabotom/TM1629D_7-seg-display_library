#define STB 10
#define CLK 11
#define DIO 12
#define brightness 3

#define CommonCathode

#ifdef CommonCathode
#include "TM1629D_CC.h"
TM1629D_CC module(DIO, CLK, STB); // DIO, CLK, STB, intensity default (0=off, 1=min, 8=max)
#else
#include "TM1629D.h"
TM1629D module(DIO, CLK, STB); // DIO, CLK, STB, intensity default (0=off, 1=min, 8=max)
#endif
#include "TM16xxDisplay.h"
TM16xxDisplay LED_display(&module, 8);    // TM16xx object, number of digits on display module

uint32_t timePeriod = 1000, ms1;


void setup() {
  Serial.begin(9600);
  LED_display.println("12.3456783"); // (left shrink)
  delay(2000);
  LED_display.clear();
  LED_display.setBrightness(brightness);
  delay(2000);
  ms1 = millis();
}


void loop() {
  uint32_t ms = millis();
  if ((ms - ms1) > timePeriod || ms < ms1) {
    ms1 = ms;
    
    Serial.print("bright = ");
    Serial.println(brightness);
    
    LED_display.setBrightness(brightness);
    LED_display.println(87654321);
    brightness = brightness + 1;
    if (brightness > 8) brightness = 0;
  }
  delay(10);
}
