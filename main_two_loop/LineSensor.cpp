/*
LineSensor.cpp
Details of Junction() and recognition of corners to rotate: bool LineSensor::Straight() etc.
*/
// #pragma once
// #include "Arduino.h"
#include "LineSensor.h"
#include "util.h"  // contain some key constants

void LineSensor::LineSensorSetup() {
  pinMode(FrontSensor, INPUT);
  pinMode(RightSensor, INPUT);
  pinMode(LeftSensor, INPUT);
  pinMode(BackSensor, INPUT);
}

void LineSensor::Junction() {
  if ((digitalRead(FrontSensor) == 1) && (digitalRead(RightSensor) == 1) && (DetectDONE == 0) && (DetectDONE2 == 0) && (DetectDONE3 == 0) && (DetectDONE4 == 0)) {
    Count++;
    if (Count == 3) {
      RightDetect++;
    }
  } else {
    Count = 0;
  }
  if (RightDetect == 1) {
    DetectDONE = 1;
  }
  if (RightDetect == 2) {
    DetectDONE2 = 1;
  }
  if (RightDetect == 3) {
    DetectDONE3 = 1;
  }
  if (RightDetect == 4) {
    DetectDONE4 = 1;
  }
  if (DetectDONE == 1) {
    DetectCounter++;
    if (DetectCounter >= 1400) {
      DetectDONE = 0;
    }
  }
  if (DetectDONE2 == 1) {
    DetectCounter2++;
    if (DetectCounter2 >= 700) {
      DetectDONE2 = 0;
    }
  }
  if (DetectDONE3 == 1) {
    DetectCounter3++;
    if (DetectCounter3 >= 1000) {
      DetectDONE3 = 0;
    }
  }
  if (DetectDONE4 == 1) {
    DetectCounter4++;
    if (DetectCounter4 >= 1000) {
      DetectDONE4 = 0;
    }
  }
}

void LineSensor::Junction1() {
  if ((digitalRead(BackSensor) == 1) && (digitalRead(LeftSensor) == 1)) {
    Count1++;
    if (Count1 == 3) {
      LeftDetect++;
    }
  } else {
    Count1 = 0;
  }
  if (LeftDetect == 1) {
    DetectDONE1 = 1;
  }
  if (DetectDONE1 == 1) {
    DetectCounter1++;
    if (DetectCounter1 >= 1000) {
      DetectDONE1 = 0;
    }
  }
}

void LineSensor::Box() {
  if ((digitalRead(RightSensor) == 1) && (digitalRead(LeftSensor) == 1)) {
    BoxCount++;
    if (BoxCount == 3) {
      BoxDetect++;
    } else {
      BoxDetect = 0;
    }
  } else {
    BoxCount = 0;
  }
  if (BoxDetect == 1) {
    BoxDetectDONE = 1;
  }
  if (BoxDetectDONE == 1) {
    BoxDetectCounter++;
    if (BoxDetectCounter >= 1000) {
      BoxDetectDONE = 0;
    }
  }
}