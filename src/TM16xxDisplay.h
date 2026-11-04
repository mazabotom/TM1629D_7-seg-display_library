/*
  TM16xxDisplay.h - LED Display class for TM16xx.
*/

#ifndef _TM16XX_DISPLAY_H
#define _TM16XX_DISPLAY_H

#include "TM16xx.h"
#include <Print.h>
//#include "TM16xxFonts.h"


class TM16xxDisplay : public Print
{
  public:
    TM16xxDisplay(TM16xx *pTM16xx, byte nNumDigits);

    void setBrightness(byte intensity);		// intensity 0-7, 0=off, 7=bright

    virtual void clear();

    // Set the display to the String (defaults to built in font)
    virtual void setDisplayToString(const char* string, const word dots = 0, const byte pos = 0, const byte font[] = TM16XX_FONT_DEFAULT);
    virtual void setDisplayToString(String string, const word dots = 0, const byte pos = 0, const byte font[] = TM16XX_FONT_DEFAULT);
    virtual void setDisplayToError();

    // Set the display to a unsigned decimal number, dot position 8-4-2-1 or 0=off, with or without leading zeros
    void setDisplayToDecNumber(unsigned long number, byte dots = 0, bool leadingZeros = false, const byte numberFont[] = TM16XX_NUMBER_FONT);
    // Set the display to a signed decimal number, dot position 8-4-2-1 or 0=off, with or without leading zeros
    void setDisplayToSignedDecNumber(signed long number, byte dots = 0, bool leadingZeros = false, const byte numberFont[] = TM16XX_NUMBER_FONT);

    // support for the Print class
    void setCursor(int8_t nPos); 	// allows setting negative to support scrolled printing
    using Print::write; // pull in write(str) and write(buf, size) from Print
    virtual size_t write(uint8_t character);
    /* virtual size_t write(const char *str); */

  protected:
    TM16xx *_pTM16xx;
    byte _nNumDigits;
    int8_t _nPrintPos = 0;

  private:
    void setDisplayToDecNumberAt(unsigned long number, byte dots, byte startingPos, bool leadingZeros, const byte numberFont[]);
    //TM16xx *TM16xxDisplay::findModuleByPos(const byte nPosFind);
    void sendCharAt(const byte nPos, byte btData, bool fDot);
    void sendAsciiCharAt(const byte nPosCombi, char c, bool fDot, const byte font[] = TM16XX_FONT_DEFAULT);
};
#endif
