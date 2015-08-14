/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef Lti_h
#define Lti_h

#include "Arduino.h"

class Lti
{
  public:
    Lti(int pinLow,int pinMed,int pinHigh);
    void low();
    void med();
    void high();
    void reset();
  private:
    int _pinLow;
    int _pinMed;
    int _pinHigh;
    int _status;
};

#endif

