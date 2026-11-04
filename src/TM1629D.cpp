/*
  TM1629D.cpp - fork of TM1638Anode.cpp (Library implementation for TM1638 with Common Anode up to 10 digits)
  Schemathics of Anode connection available here https://github.com/SkullKill/ESP32-Wall-Clock-PCB and https://github.com/SkullKill/ESP32-Wall-Clock-PCB/wiki

*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "TM1629D.h"

TM1629D::TM1629D(byte dataPin, byte clockPin, byte strobePin, bool activateDisplay, byte intensity)
  : TM16xx(dataPin, clockPin, strobePin, TM1629D_MAX_POS, 10, activateDisplay, intensity)
{
  _maxSegments = 8;		// on the QYF-TM1638 modules the two extra segment lines are not used. The display uses common anode LEDs
  //_numDigits = numDigits;
  memset(this->bitmap, 0, TM1629D_MAX_POS);
  clearDisplay();
  setupDisplay(activateDisplay, intensity);
}

void TM1629D::setSegments(byte segments, byte position)
{ // set 10 leds on common grd as specified
  // TM1638 uses 10 segments in two bytes, similar to TM1668
  // for the digit displays both byte (containing seg1-seg10) are sent
  if (position < _maxDisplays) {
    //update our memory bitmap
    this->bitmap[position] = segments;

    // transpose the bitmap to counter Common Anode connections
    for (byte nPos = 0; nPos < _maxSegments; nPos++) {
      uint16_t btVal = 0;
      for (byte j = 0; j < TM1629D_MAX_POS; j++) {
        // for schematics of inverted connection, 1st digit connected to SEG10, 2nd digit connected to SEG9 etc
        //btVal |= ((pVal[j] >> nPos) & 1) << (TM1629D_MAX_POS - j - 1);
        // for schematics of 1st digit connected to SEG1, 2nd digit connected to SEG2 etc
        btVal |= ((this->bitmap[j] >> nPos) & 1) << (j);
      }
      // send each byte of the transposed bitmap
      //send 1st byte of data
      sendData(nPos << 1, (byte)btVal & 0xFF);
      // send 2nd byte of data
      sendData((nPos << 1) | 1, (byte)(btVal >> 8) & 0x03);
    }

  }
}
