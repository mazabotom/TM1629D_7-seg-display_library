/*
 - letters printing control
*/
#define STB 10
#define CLK 11
#define DIO 12
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


void setup() {
  LED_display.setBrightness(4);
}


void loop() {
  LED_display.print("A");
  delay(500);
  LED_display.print("b");
  delay(500);
  LED_display.print("c");
  delay(500);
  LED_display.print("C");
  delay(500);
  LED_display.print("d");
  delay(500);
  LED_display.print("e");
  delay(500);
  LED_display.print("E");
  delay(500);
  LED_display.print("F");
  delay(500);
  LED_display.clear();
  delay(5000);

  LED_display.println("G");
  delay(500);
  LED_display.print("g");
  delay(500);
  LED_display.print("H");
  delay(500);
  LED_display.print("h");
  delay(500);
  LED_display.print("J");
  delay(500);
  LED_display.print("L");
  delay(500);
  LED_display.print("O");
  delay(500);
  LED_display.print("o");
  delay(500);
  LED_display.clear();
  delay(5000);

  LED_display.println("P");
  delay(500);
  LED_display.print("p");
  delay(500);
  LED_display.print("r");
  delay(500);
  LED_display.print("S");
  delay(500);
  LED_display.print("T");
  delay(500);
  LED_display.print("U");
  delay(500);
  LED_display.print("u");
  delay(500);
  LED_display.print("V");
  delay(500);
  LED_display.clear();
  delay(5000);

}
