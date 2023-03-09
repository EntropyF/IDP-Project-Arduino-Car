/*
LineSensor.h
Set some functions related with Line Sensors, mainly Junction Detect and Line Follower.
Line Follower function is in main_loop.ino right now, but can be moved here.
*/
// #pragma once
#include "util.h" // contain global constants

class LineSensor {
  public:
    void Junction(); // funtion to detect junctions and count it
    void LineSensorSetup(); // Setup function for LineSensor
    // The following variables are numbers of different types of Junctions the car meet
    int RightDetect = 0;

  private:
    int Count = 0; // Count every 10ms elapsed for junction detection
    bool DetectDONE = 0; // The state whether the Junction detect funtion is active
    int DetectCounter = 0; // Counter for cold time in Junction Counter
};