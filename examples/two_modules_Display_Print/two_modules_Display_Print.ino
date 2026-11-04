/*
  example for Common Anode.
*/
#define STB 10
#define CLK 11
#define DIO 12
#define STB2 13

#include <Arduino.h>
#include "TM1629D.h"
#include "TM16xxDisplay.h"

TM1629D module(DIO, CLK, STB1);
TM16xxDisplay LED_display(&module, 4);    // TM16xx object, 4 digits

TM1629D module2(DIO, CLK, STB2);
TM16xxDisplay display2(&module2, 8);    // TM16xx object, 8 digits

int nCount = 0;


void setup() {
  LED_display.setBrightness(7);
  LED_display.println("12.34.5.6");
  display2.setBrightness(1);
  display2.println("123456.7895");
}


void loop() {
  delay(2000);
  display2.println((String)"Count:" + nCount);
  nCount++;
}