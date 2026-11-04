/*
  TM16xx.h - Library for TM1637, TM1638 and similar chips.
*/

#ifndef TM16XX_h
#define TM16XX_h

#include "Arduino.h"

#if !defined(__max)
// MMOLE 180325: min, max are no macro in ESP core 2.3.9 libraries, see https://github.com/esp8266/Arduino/issues/398
// MMOLE 211229: Redefining min/max has issues in newer ESP cores with certain wifi libraries.
//               See definition as function template below.
#define __min(a,b) ((a)<(b)?(a):(b))
#define __max(a,b) ((a)>(b)?(a):(b))
#endif

#if !defined(_BV)
// MMOLE 220909: _BV is not defined in Raspberry Pi Pico RP2040 core v2.4.0 or v2.5.2 by Earle F. Philhower
// TM16xx uses it in some classes to determine button presses (TM1628/30/37/38/38Anode
// Note that _BV works on int values (16-bit in Arduino). For that reason using bit() would be better.
// This is how _BV is defined in the AVR libraries used within Arduino:
#define _BV( x )( 1<<(x))
#endif

#define TM16XX_CMD_DATA_READ 0x42     // command to read data used on two wire interfaces of TM1637
#define TM16XX_CMD_DATA_FIXED 0x44
#define TM16XX_CMD_DISPLAY 0x80
#define TM16XX_CMD_ADDRESS 0xC0

// Common display modes for selecting different GRID x SEGMENT configuration
// Used by e.g. TM1618, TM1620, TM1628, TM1630, TM1668
// (note: TM1652 combined display control and uses a single bit to designate 5x8 or 6x8)
#define TM16XX_CMD_MODE_4GRID 0x00
#define TM16XX_CMD_MODE_5GRID 0x01    // TM1630: 4x8 - 5x7
#define TM16XX_CMD_MODE_6GRID 0x02    // TM1620: 4x10 - 6x8
#define TM16XX_CMD_MODE_7GRID 0x03    // TM1628, TM1668: 4x13 - 7x10, TM1624: 4x14 - 7x11, TM1618: 4x8 - 7x5
// TM1617 has 14 addresses (00-0D) but only uses GRID1, GRID2 and GRID7. For mode 7x3 instead of 8x2 it uses command MODE_7GRID.

#include "TM16xxFonts.h"

class TM16xx
{
  public:
    /**
       Instantiate a TM16xx module specifying data, clock and strobe pins (no strobe on some modules),
       maxDisplays - the maximum number of displays supported by the chip (as provided by derived chip specific class),
       nDigitsUsed - the number of digits used to display numbers or text,
    */
    TM16xx(byte dataPin, byte clockPin, byte strobePin, byte maxDisplays, byte nDigitsUsed, bool activateDisplay = true, byte intensity = 3);
    /** DEPRECATED: activation, intensity (0-7) and display mode are no longer used by constructor. */

    /** Set the display (segments and LEDs) active or off and intensity (range from 0-7). */
    virtual void setupDisplay(bool active, byte intensity);

    /** Clear the display */
    virtual void clearDisplay();

    /** Use explicit call in setup() or rely on implicit call by sendData(); calls setupDisplay() and clearDisplay() */
    virtual void begin(bool activateDisplay = true, byte intensity = 3);

    /** Set segments of the display */
    virtual void setSegments(byte segments, byte position);

    //
    // Basic display functions. For additional display features use the TM16xxDisplay class
    //

    /** Set a single display at pos (starting at 0) to a digit (left to right 0-1-2-...) */
    virtual void setDisplayDigit(byte digit, byte pos = 0, bool dot = false, const byte numberFont[] = TM16XX_NUMBER_FONT);

    /** Set the display to a decimal number */
    virtual void setDisplayToDecNumber(int nNumber, byte bDots = 0, bool fLeadingZeros = true);

    /** Clear a single display at pos (starting at 0, left to right) */
    virtual void clearDisplayDigit(byte pos, bool dot = false);
    /** Set the display to the values (left to right) */
    virtual void setDisplay(const byte values[], byte size = 8);

    /** Set the display to the string (defaults to built in 7-segment alphanumeric font) */
    virtual void setDisplayToString(const char* string, const word dots = 0, const byte pos = 0, const byte font[] = TM16XX_FONT_DEFAULT);

    virtual void sendChar(byte pos, byte data, bool dot); // made public to allow calling from TM16xxDisplay
    virtual void setNumDigits(byte numDigitsUsed);   // set number of digits used for alignment
    virtual void sendAsciiChar(byte pos, char c, bool dot, const byte font[] = TM16XX_FONT_DEFAULT); // made public to allow calling from TM16xxDisplay

  protected:
    virtual void bitDelay();
    virtual void start();
    virtual void stop();
    virtual void send(byte data);
    virtual void sendCommand(byte led);
    virtual void sendData(byte add, byte data);

#if !defined(max)
    // MMOLE 211229: use c++ function templates to implement our own min/max, as redefining them wont work in newer ESP cores when using certain wifi libraries
    // NOTE: min, max are no macro in ESP core 2.3.9 libraries, see https://github.com/esp8266/Arduino/issues/398
    // See also
    //   https://www.cplusplus.com/doc/oldtutorial/templates/
    //   https://www.alltestanswers.com/c-templates-for-the-two-functions-minimum-and-maximum/
    //   https://www.learncpp.com/cpp-tutorial/function-templates-with-multiple-template-types/
    // MMOLE 220814: Arduino IDE 1.8.12 for LGT328P required "#if !defined(max)" to prevent compilation errors
    template <typename T, typename U>
    auto max(T x, U y) -> decltype(x > y ? x : y)
    {
      return x > y ? x : y;
    }
    template <typename T, typename U>
    auto min(T x, U y) -> decltype(x > y ? x : y)
    {
      return x < y ? x : y;
    }
#endif  // !defined(max)

    byte _maxDisplays = 2; // maximum number of digits (grids), chip-dependent
    byte _maxSegments = 8; // maximum number of segments per display, chip-dependent
    bool fBeginDone = false; // for implicit begin checking;
    //byte intensitySetup; // TODO: prevent changing of intensity when calling setupDisplay followed by clearDisplay
    byte digits;           // number of digits in the display, module dependent
    byte dataPin;
    byte clockPin;
    byte strobePin;
};
#endif
