/*
This is for both ultra sonic distance sensor and colour sensor
*/
#include "util.h"
#include "DistanceSensor.h"

void DistanceSensor::DistanceSensorSetup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ColourSensor, INPUT);
  pinMode(RedLED, OUTPUT);
  pinMode(GreenLED,OUTPUT);
  digitalWrite(RedLED,LOW);
  digitalWrite(GreenLED,LOW);
}

// Do a detection every 1 ms (1000 microseconds)
void DistanceSensor::DetectDistance() {
  // Set a clock for echo and trig (can drop it as microseconds delay doesn't affect a lot)
  currentDetect = millis();            // get the current time
  if (currentDetect - previousDetect >= 50) {
    previousDetect = currentDetect;
    digitalWrite(trigPin, LOW);  // Added this line
    delayMicroseconds(2);        // Added this line
    digitalWrite(trigPin, HIGH);
    
    delayMicroseconds(10);  // Added this line
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration / 2) / 29.1;
  }
}

// This is the function for colour sensor
// Colour has states 0, 1, 2. 1 for Red; 2 for Green
// We only need to activate this colour sensor in strict condition.
void DistanceSensor::ColourRecognition() {
  if (digitalRead(ColourSensor) == 0) {
    Colour = 1;
    digitalWrite(GreenLED,LOW);
    digitalWrite(RedLED,HIGH);
  }
  else {
    Colour = 2;
    digitalWrite(GreenLED,HIGH);
    digitalWrite(RedLED,LOW);
  }
}

// ### Tunnel Detection ###
// distance range still need to be measured!!
// Tunnel: 7 - 4.4 cm
// Back: 5.8cm to the wall
void DistanceSensor::Tunnel() {
  if(distance<7) {
    tunnelcount++;
  } else {
    tunnelcount = 0;
  }
  if(tunnelcount == 300) {
    TunnelDETECT++;
  }
}

// ### Block Detection ###
// distance range still need to be measured!!
// distance measured 7.2cm to the block
void DistanceSensor::Block() {
  if ((distance >= 7)&&(distance < 10)) {
    blockcount++;
  } else {
    blockcount = 0;
  }
  if (blockcount == 5) {
    BlockDETECT = 1;
  }
}