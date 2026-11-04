/*
  TM1629D_CC.cpp - Library implementation for TM1629D_CC.
*/

#include "TM1629D_CC.h"

TM1629D_CC::TM1629D_CC(byte dataPin, byte clockPin, byte strobePin, byte numDigits, bool activateDisplay, byte intensity)
  : TM16xx(dataPin, clockPin, strobePin, TM1629D_CC_MAX_POS, numDigits, activateDisplay, intensity)
{
  // DEPRECATED: activation, intensity (0-7) and display mode are no longer used by constructor.
  _maxSegments = 10;  // on the LED & KEY modules the extra segments are used to drive individual red or red/green LEDs
  /*
    clearDisplay();
    setupDisplay(activateDisplay, intensity);
  */
  // NOTE: CONSTRUCTORS SHOULD NOT CALL DELAY() <= gives hanging on certain ESP8266 cores as well as on LGT8F328P
  // Using micros() or millis() in constructor also gave issues on LST8F328P and CH32.
  // Root cause is that the timer interrupt to update these counters may not be set until setup() is called.
  // To initialize the display an implicit begin() is called by TM16xx::sendData().
}

void TM1629D_CC::setSegments(byte segments, byte position)
{ // TODO: support 10-14 segments on chips like TM1629D_CC/TM1668
  // for the digit displays only the first byte (containing seg1-seg8) is sent
  // Only the LSB (SEG1-8) is sent to the display
  if (position < _maxDisplays)
    sendData(position << 1, segments);
}
