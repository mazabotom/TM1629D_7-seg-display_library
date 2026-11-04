/*
  TM1629D_CC.h - Library for TM1629D_CC.
*/

#ifndef TM1629D_CC_h
#define TM1629D_CC_h

#include "TM16xx.h"

#define TM1629D_CC_MAX_POS 10

class TM1629D_CC : public TM16xx
{
  public:
    /** Instantiate a TM1629D_CC module specifying data, clock and stobe pins, the number of digits
        DEPRECATED: activation and intensity are no longer used by constructor. Use begin() or setupDisplay() instead. */
    TM1629D_CC(byte dataPin, byte clockPin, byte strobePin, byte numDigits = 8, bool activateDisplay = true, byte intensity = 3);

    /** Set the up to 8 segments at a specific position on or off */
    virtual void setSegments(byte segments, byte position);
};

#endif
