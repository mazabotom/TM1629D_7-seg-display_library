/*
  TM1629D.h - fork of TM1638Anode.h (Library implementation for TM1638 with Common Anode up to 10 digits)
  Schemathics of Anode connection available here https://github.com/SkullKill/ESP32-Wall-Clock-PCB and https://github.com/SkullKill/ESP32-Wall-Clock-PCB/wiki
*/

#ifndef TM1629D_h
#define TM1629D_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "TM16xx.h"

#define TM1629D_MAX_POS 10

class TM1629D : public TM16xx
{
  public:
    /** Instantiate a tm1638 module specifying data, clock and stobe pins, the display state, the starting intensity (0-7). */
    TM1629D(byte dataPin, byte clockPin, byte strobePin, bool activateDisplay = true, byte intensity = 3);

    /** Set the segments at a specific position on or off */
    virtual void setSegments(byte segments, byte position);

  private:
    byte bitmap[TM1629D_MAX_POS];		// store a bitmap for all TM1629D_MAX_POS digit to allow common anode manipulation

};

#endif
