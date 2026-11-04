/*
  TM16xx.cpp - Library for TM1637, TM1638 and similar chips.
*/

#include "TM16xx.h"

TM16xx::TM16xx(byte dataPin, byte clockPin, byte strobePin, byte maxDisplays, byte nDigitsUsed, bool activateDisplay, byte intensity)
{
  // DEPRECATED: activation, intensity (0-7) and display mode are no longer used by constructor.
  this->dataPin = dataPin;
  this->clockPin = clockPin;
  this->strobePin = strobePin;
  this->_maxDisplays = maxDisplays;
  this->digits = nDigitsUsed;

  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(strobePin, OUTPUT);

  digitalWrite(strobePin, HIGH);
  digitalWrite(clockPin, HIGH);

  // NOTE: CONSTRUCTORS SHOULD NOT CALL DELAY() <= gives hanging on certain ESP8266/ESP32 cores as well as on LGT8F328P
  // Using micros() or millis() in constructor also gave issues on LST8F328P and CH32V003.
  // Some TM16xx chips uses bit-timing to communicate, so clearDisplay() and setupDisplay() cannot be called in constructor.
  // Call begin() in setup() to clear the display and set initial activation and intensity.
  // To make begin() optional, sendData() implicitely calls begin(). Although it will execute only once, it may cause a
  // second call to clearDisplay() by an explicit call to clearDisplay(). Therefor sketches should preferably call begin().
}

void TM16xx::setupDisplay(bool active, byte intensity)
{ // Set the display intensity and switch it on/off
  // Some TM16xx classes (e.g. TM1650) use _maxSegments in setupDisplay() to also set the display mode.
  sendCommand(TM16XX_CMD_DISPLAY | (active ? 8 : 0) | min(7, intensity));
}
//
void TM16xx::clearDisplay()
{
  for (byte j = 0; j < _maxDisplays; j++) {
    setSegments(0b00000000, j);
  }
}

void TM16xx::begin(bool activateDisplay, byte intensity)
{ // Call begin() in setup() to clear the display and set initial activation and intensity.
  // begin() is implicitly called upon first sending of display data, but only executes once.
  // Some chips may require begin() to initialize communication or other things.
  //static bool fBeginDone=false; // NOTE: Statics are shared like globals when using multiple object instances. That's unwanted behavior here!
  if (fBeginDone)
    return;
  fBeginDone = true;
  clearDisplay();
  setupDisplay(activateDisplay, intensity);
}

void TM16xx::setSegments(byte segments, byte position)
{ // set 8 leds on common grd as specified
  // TODO: support 10-14 segments on chips like TM1638/TM1668
  if (position < _maxDisplays)
    sendData(position, segments);
  //sendData(TM16XX_CMD_ADDRESS | position, segments);
}

void TM16xx::sendChar(byte pos, byte data, bool dot)
{
  /*
  	if(pos<_maxDisplays)
  	  sendData(pos, data | (dot ? 0b10000000 : 0));
  */
  setSegments(data | (dot ? 0b10000000 : 0), 0 ? this->digits - 1 - pos : pos);
}

void TM16xx::sendAsciiChar(byte pos, char c, bool fDot, const byte font[])
{ // Method to send an Ascii character to the display
  // This method is also called by TM16xxDisplay.print to display characters
  // The base class uses the default 7-segment font to find the LED pattern.
  // Derived classes for multi-segment displays or alternate layout displays can override this method
  sendChar(pos, pgm_read_byte_near(font + (c - 32)), fDot);
}

void TM16xx::setDisplayDigit(byte digit, byte pos, bool dot, const byte numberFont[])
{
  sendChar(pos, pgm_read_byte_near(numberFont + (digit & 0xF)), dot);
}

void TM16xx::setDisplayToDecNumber(int nNumber, byte bDots, bool fLeadingZeros)		// byte bDots=0, bool fLeadingZeros=true
{ // Function to display a decimal number on a n-digit clock display.
  // Kept simple to fit in ATtiny44A
  // For extended display features use the TM16xxDisplay class

  // TODO: support large displays such as 8segx16 on TM1640
  // TODO: how about negative numbers?
  for (byte nPos = 0; nPos < digits; nPos++)
  {
    if (nPos == 0 || nNumber > 0 || fLeadingZeros)
      setDisplayDigit(nNumber % 10, digits - 1 - nPos, bDots & _BV(nPos));
    else
      sendChar(digits - 1 - nPos, 0, bDots & _BV(nPos));
    nNumber /= 10;
  }
}

void TM16xx::clearDisplayDigit(byte pos, bool dot)
{
  sendChar(pos, 0, dot);
}

void TM16xx::setDisplay(const byte values[], byte size)
{ // send an array of values to the display
  for (byte i = 0; i < size; i++) {
    sendChar(i, pgm_read_byte_near(values + i), 0);
  }
}

void TM16xx::setDisplayToString(const char* string, const word dots, const byte pos, const byte font[])
{
  for (int i = 0; i < digits - pos; i++) {
    if (string[i] != '\0') {
      sendAsciiChar(i + pos, string[i], (dots & (1 << (digits - i - 1))) != 0, font);   // use sendAsciiChar to support 14-segments (via derived classes)
    } else {
      break;
    }
  }
}

void TM16xx::setNumDigits(byte numDigitsUsed)
{ // Set the number of digits used.
  // Used by setDisplayToDecNumber() and setDisplayToString() to right align
  // Also used by TM16xxDisplay to combine modules.
  this->digits = numDigitsUsed;

  // The maximum number of digits _maxDisplays is now set by a parameter in the constructor.
  // Some TM16xx chips support multiple display modes. Derived classes use _maxDisplays
  // or _maxSegments to set the display mode when setupDisplay() is called.
  // Those classes could override setNumDigits() to change these maxima.
  // (if numDigitsUsed != _maxDisplays change display mode and adjust maxima)
  // Unfortunately this doesn't work for chips like TM1650, which has TM1650_DISPMODE_4x8
  // and TM1650_DISPMODE_4x7. Mode 4x7 is to allow the DP/KP pin to signal key presses.
  // At the moment that class still uses a display mode parameter in the constructor.
}

//
//  Protected methods
//

void TM16xx::bitDelay()
{ // if needed derived classes can add a delay (eg. for TM1637)
  //delayMicroseconds(50);

  // On fast MCUs like ESP32 a delay is required, especially when reading buttons
  // The TM1638 datasheet specifies a max clock speed of 1MHz.
  // Testing shows that without delay the CLK line exceeds 1.6 MHz on the ESP32. While displaying data still worked, reading buttons failed.
  // Adding a 1us delay gives clockpulses of about 1.75us-2.0us (~ 250kHz) on an ESP32 @240MHz and a similar delay on an ESP8266 @160Mhz.
  // An ESP32 running without delay at 240MHz gave a CLK of  ~0.3us (~ 1.6MHz)
  // An ESP8266 running without delay at 160MHz gave a CLK of  ~0.9us (~ 470kHz)
  // An ESP8266 running without delay  at 80MHz gave a CLK of  ~1.8us (~ 240kHz)
#if F_CPU>100000000
  //#if F_CPU>40000000    // semi-fast processeors like CH32V003 @48Mhz may also need some delay
  delayMicroseconds(1);
#endif
}

void TM16xx::start()
{ // if needed derived classes can use different patterns to start a command (eg. for TM1637)
  digitalWrite(strobePin, LOW);
  bitDelay();
}

void TM16xx::stop()
{ // if needed derived classes can use different patterns to stop a command (eg. for TM1637)
  digitalWrite(strobePin, HIGH);
  bitDelay();
}

void TM16xx::send(byte data)
{
  // MMOLE 180203: shiftout does something, but is not okay (tested on TM1668)
  //shiftOut(dataPin, clockPin, LSBFIRST, data);
  for (int i = 0; i < 8; i++) {
    digitalWrite(clockPin, LOW);
    bitDelay();
    digitalWrite(dataPin, data & 1 ? HIGH : LOW);
    bitDelay();
    data >>= 1;
    digitalWrite(clockPin, HIGH);
    bitDelay();
  }
  bitDelay();		// NOTE: TM1638 specifies a Twait between bytes of minimal 1us.
}

void TM16xx::sendCommand(byte cmd)
{
  start();
  send(cmd);
  stop();
}

void TM16xx::sendData(byte address, byte data)
{
  begin();    // begin() is implicitly called upon first sending of display data, but executes only once.
  sendCommand(TM16XX_CMD_DATA_FIXED);							// use fixed addressing for data
  start();
  send(TM16XX_CMD_ADDRESS | address);						// address command + address
  send(data);
  stop();
}
