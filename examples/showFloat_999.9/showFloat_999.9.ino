/*
  - индикация десятых долей единиц от 0 до 999.9 с гашением незначащего нуля.
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

uint16_t volume;
float floatVal;

#define measQuant 100

int measuring(int input_PIN) {
  uint32_t value = 0; // 4 294 967 295 max
  for (uint16_t i = 0; i < measQuant; i++) {
    value += analogRead(input_PIN);
    delayMicroseconds(10);  // delay in between reads for stability
  }
  value = (value / measQuant);
  return value;
}

void showFloat(float floatVal) {
  floatVal = floatVal * 10.0;
  uint32_t value = long(round(floatVal)); // работает правило округления
  if (value >= 9999) {
    LED_display.setDisplayToError();
  }
  else {
    int tysiachi = value / 1000;
    int sotni = (value / 100) % 10;
    int desiatki = (value / 10) % 10;
    int edinicy = value % 10;
    
    //setDisplayDigit(digit, pos, dot, numberFont);
    if (tysiachi == 0) module.setDisplayDigit(10, 0); // most left,  digit0 show space(10) if zero
    else module.setDisplayDigit(tysiachi, 0);

    if (sotni == 0 && tysiachi == 0) module.setDisplayDigit(10, 1); // digit1 show space(10) if zero
    else module.setDisplayDigit(sotni, 1);

    module.setDisplayDigit(desiatki, 2, true); // dot true
    module.setDisplayDigit(edinicy, 3); // most right digit, получаем единицы.
  }
}


void setup() {
  //LED_display.setBrightness(2);
}


void loop() {
  volume = measuring(15);
  floatVal = volume * 1.33; // from 0 to 1361

  showFloat(floatVal);
}
