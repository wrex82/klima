/*
  Morse.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#include "Arduino.h"
#include "Lti.h"

Lti::Lti(int pinLow, int pinMed, int pinHigh)
{
  pinMode(pinLow, INPUT_PULLUP);
  pinMode(pinLow, OUTPUT);
  pinMode(pinMed, INPUT_PULLUP);
  pinMode(pinMed, OUTPUT);
  pinMode(pinHigh, INPUT_PULLUP);
  pinMode(pinHigh, OUTPUT);
 
  _pinLow = pinLow;
  _pinMed = pinMed;
  _pinHigh= pinHigh;
}

void Lti::reset(){
  digitalWrite(_pinLow, 1);
  digitalWrite(_pinMed, 1);
  digitalWrite(_pinHigh,1);
  _status = 0;
}

void Lti::low(){
  if(_status != 1){
    reset();  
    digitalWrite(_pinLow,0);
    _status=1;
  }      
}

void Lti::med(){
  if(_status !=2){
    reset();
    digitalWrite(_pinMed, 0);
    _status = 2;
  }      
}

void Lti::high(){
  if(_status !=3){
    reset();
    digitalWrite(_pinHigh,0);
    _status=3; 
  }  
  
}


