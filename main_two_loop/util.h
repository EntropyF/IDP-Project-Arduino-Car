/*
util.h
contains some global constants and all the pins that used on Arduino
*/

#pragma once
#include <Arduino.h>

// Pins we use for four line sensors
const int FrontSensor = A0; // Actually right right sensor data read from pin A0
const int RightSensor = A1;  // right sensor data read from pin A1
const int LeftSensor = A2; // left sensor data read from pin A2
const int BackSensor = A3;  // Actually left left data read from pin A3

// Pin used for colour sensor
const int ColourSensor = 8;
const int RedLED = 4;
const int GreenLED = 3;

// Pin used for blink LED
const int BlinkLED = 12;

// Pin used for switch button
const int SwitchButton = 13;
const int ResetPin = 5;

// Pin used for Distance Sensor
const int trigPin = 7;
const int echoPin = 6;