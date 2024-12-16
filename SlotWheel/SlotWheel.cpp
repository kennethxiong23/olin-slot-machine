
/*
  SlotMachine.cpp - Library for slot machine wheels
*/

#include "Arduino.h"
#include <Servo.h>
#include "SlotWheel.h"

SlotWheel::SlotWheel(int servoPin, int encoderPin)
{
  _speed =90;
  _servoPin = servoPin;
  _encoderPin = encoderPin;
  _encoderValue =  0;
  _servo;
}

int SlotWheel::test(){
  return _encoderValue;
}
void SlotWheel::begin()
{
    _servo.attach(_servoPin, 1280, 1720);
    pinMode(_encoderPin, INPUT);

}

int SlotWheel::read()
{
 _encoderValue = map(pulseIn(_encoderPin, LOW), 31, 1067, 0, 255);
  return _encoderValue;
}

void SlotWheel::write(int speed)
{
  _speed = speed;
  _servo.write(_speed);
}
int SlotWheel::getSpeed(){
  return _speed;
}
void SlotWheel::reset(){
read();
  while((_encoderValue > 5) && (_encoderValue < 245)){
    _servo.write(70);
    read();
  }
      _servo.write(90);
}
void SlotWheel::runToPosition(int pos){
  int _remaining = 255;
  while ( _speed > 90 && _remaining > 10){
  if (read() > pos){
      _remaining = 255 - read() + pos;
    }
    else{
      _remaining = pos - read();
    }
    _speed = map(_remaining, 30, 250, 90, 130);
    write(_speed);
  }
    write(92);
  
}
void SlotWheel::runToFace(int face){
  runToPosition(int(face *256/5));
}
int SlotWheel::getFace(){
  read();
  return floor(_encoderValue/(256/5));
}