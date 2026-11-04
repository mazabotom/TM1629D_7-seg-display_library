/*
  TM16xxDisplay.h - LED Display library for TM16xx.
*/

#include "TM16xxDisplay.h"
#include "string.h"

TM16xxDisplay::TM16xxDisplay(TM16xx *pTM16xx, byte nNumDigits)
{ // constructor for a display having only one module
  _pTM16xx = pTM16xx;
  _nNumDigits = nNumDigits;
}

void TM16xxDisplay::sendCharAt(const byte nPosCombi, byte btData, bool fDot)
{ // set the specified data at specified position of the module that has that position)
  // Note that each individual module can have a different size (just to complicate things).
  byte nPos = nPosCombi;
  _pTM16xx->sendChar(nPos, btData, fDot);

}

void TM16xxDisplay::sendAsciiCharAt(const byte nPosCombi, char c, bool fDot, const byte font[])
{ // Set the specified Ascii character at specified position of the module that has that position
  // Note that each individual module can have a different size (just to complicate things).
  byte nPos = nPosCombi;
  _pTM16xx->sendAsciiChar(nPos, c, fDot, font);
}

void TM16xxDisplay::setBrightness(byte intensity)
{ // set the intensity of the module; range 0-8, 0=off, 8=brightest
  if (intensity > 8) intensity = 8;
  _pTM16xx->setupDisplay(intensity != 0, intensity > 0 ? intensity - 1 : 0);
}

void TM16xxDisplay::setDisplayToString(const char* string, const word dots, const byte pos, const byte font[])
{ // call base implementation
  //_pTM16xx->setDisplayToString(string, dots, pos, font);
  int stringLength = strlen(string);

  for (int i = 0; i < _nNumDigits - pos; i++) {
    if (i < stringLength) {
      sendAsciiCharAt(i + pos, string[i], (dots & (1 << (_nNumDigits - i - 1))) != 0, font);   // use sendAsciiChar to support 14-segments (via derived classes)
    }
    else {
      break;
    }
  }
}

void TM16xxDisplay::setDisplayToString(const String string, const word dots, const byte pos, const byte font[])
{ // additional implementation using String class (uses more memory than char * version)
  setDisplayToString(string.c_str(), dots, pos, font);
}

void TM16xxDisplay::setDisplayToError()
{ // set the display to Error text
  //this->clear(); // not usefull, only blinking!
  _pTM16xx->setDisplay(TM16XX_ERROR_DATA, sizeof(TM16XX_ERROR_DATA));  // TODO?: support COMBIDISPLAY
}

void TM16xxDisplay::setDisplayToDecNumberAt(unsigned long number, byte dots, byte startingPos, bool leadingZeros, const byte numberFont[])
{
  for (int nPos = 0; nPos < _nNumDigits - startingPos; nPos++) {
    if (number != 0 || nPos == 0 || leadingZeros) {
      sendCharAt(_nNumDigits - nPos - 1, pgm_read_byte_near(numberFont + (number  % 10)), (dots & (1 << nPos)) != 0);
    }
    else {
      sendCharAt(_nNumDigits - nPos - 1, 0, (dots & (1 << nPos)) != 0);     // clearDisplayDigit
    }
    number /= 10;
  }
}

void TM16xxDisplay::setDisplayToDecNumber(unsigned long number, byte dots, bool leadingZeros,	const byte numberFont[])
{
  setDisplayToDecNumberAt(number, dots, 0, leadingZeros, numberFont);
}

void TM16xxDisplay::setDisplayToSignedDecNumber(signed long number, byte dots, bool leadingZeros, const byte numberFont[])
{
  if (number >= 0) {
    setDisplayToDecNumberAt(number, dots, 0, leadingZeros, numberFont);
  }
  else {
    setDisplayToDecNumberAt(-number, dots, 1, leadingZeros, numberFont);   // start at pos 1
    sendCharAt(0, MINUS, (dots & (0x80)) != 0); // minus at pos 0, use dot at highest bit = bit(7)
  }
}

void TM16xxDisplay::clear()
{
  _pTM16xx->clearDisplay();
}

void TM16xxDisplay::setCursor(int8_t nPos)
{ // Set the print position. Allow negative numbers to support scrolling
  _nPrintPos = nPos;
}


/*
   Support for the Print class

   See https://playground.arduino.cc/Code/Printclass

*/
size_t TM16xxDisplay::write(uint8_t c)
{ //Code to display letter when given the ASCII code for it
  static uint8_t cPrevious = ' ';		// remember last character prnted, to add a dot when needed
  static bool fPrevDot = false;   // remember last dot, for showing ...

  // first check for dot to possibly combine with previous character
  // note that dot at end might still fit, eg. "HELP."
  bool fDot = false;
  if ((c == '.' || c == ',' || c == ':' || c == ';') && _nPrintPos <= _nNumDigits && cPrevious != '\0') {
    if (fPrevDot)
      c = ' ';
    else {
      if (_nPrintPos > 0)
        _nPrintPos--; // use same position to display the dot
      c = cPrevious;
    }
    fDot = true;
    fPrevDot = true;
  }
  else
    fPrevDot = false;

  // handle end-of-line
  if (c == '\0' || c == '\n' || c == '\r' || _nPrintPos >= _nNumDigits) {
    while (_nPrintPos > 0 && _nPrintPos < _nNumDigits) { // clear the remainder of the line
      sendCharAt(_nPrintPos, 0, false);    // sending 0 is same as clearDisplayDigit()
      _nPrintPos++;
    }

    // MMOLE 211103: Returning zero from write should stop printing rest of the string.
    // However, on ESP32 print() won't stop when returning 0, so for compatibility we return 1.
    // and only reset the print position when we're at the end. This should work on all platforms.
    if (c == '\0' || c == '\n' || c == '\r')
      _nPrintPos = 0;
    cPrevious = '\0';
    fPrevDot = false;
    return (1);
  }

  // print character
  if (_nPrintPos >= 0 && _nPrintPos < _nNumDigits)
    sendAsciiCharAt(_nPrintPos, c, fDot);
  cPrevious = c;
  _nPrintPos++;
  return (1);
}

/* MMOLE: not called by print()
  size_t TM16xxDisplay::write(const char *str)
  {
	Serial.println(F("write str"));
	setDisplayToString(str);
	_nPrintPos=0;
  }
*/
