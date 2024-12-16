/*
  SlotWheel.h - Library for OLIN slot machine wheels.
  Created by Kenneth Xiong Fall 2024
*/
#ifndef SlotWheel_h
#define SlotWheel_h

#include "Arduino.h"
#include <Servo.h>

class SlotWheel
{
  public:
    SlotWheel(int servoPin, int encoderPin);
    void begin();
    int read();
    void write(int speed);
    void reset();
    void runToPosition(int pos);
    void runToFace(int face);
    int getFace();
    int test(); 
    int getSpeed();
  private:
    int _speed;
    int _servoPin;
    int _encoderPin;
    volatile int _encoderValue;
    Servo _servo;

};
#endif