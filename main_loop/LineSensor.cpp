/*
LineSensor.cpp
Details of Junction() and recognition of corners to rotate: bool LineSensor::Straight() etc.
*/
// #pragma once
// #include "Arduino.h"
#include "LineSensor.h"
#include "util.h" // contain some key constants

void LineSensor::LineSensorSetup() {
  pinMode(FrontSensor, INPUT);
  pinMode(RightSensor, INPUT);
  pinMode(LeftSensor, INPUT);
  pinMode(BackSensor, INPUT);
}

void LineSensor::Junction() {
  if((DetectDONE == 0)&&(digitalRead(FrontSensor) == 1)&&(digitalRead(RightSensor) == 1)&&(digitalRead(LeftSensor) == 0)&&(digitalRead(BackSensor) == 1)) {
    Count++;
    if (Count == 5) {
      RightDetect++;
    }
  }
  else {
    Count = 0;
  }
  if(RightDetect == !RightDetect) {
    DetectDONE = 1;
  }
  if(DetectDONE == 1) {
    DetectCounter++;
    if(DetectCounter >= 1000) {
      DetectDONE = 0;
    }
  }
}