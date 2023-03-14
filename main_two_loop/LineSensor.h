/*
LineSensor.h
Set some functions related with Line Sensors, mainly Junction Detect and Line Follower.
Line Follower function is in main_loop.ino right now, but can be moved here.
*/
// #pragma once
#include "util.h" // contain global constants

class LineSensor {
  public:
    void Junction(); // funtion to detect right junction and count it
    void Junction1(); // funtion to detect left junction and count it
    void Box();
    void LineSensorSetup(); // Setup function for LineSensor
    // The following variables are numbers of different types of Junctions the car meet
    int RightDetect = 0;
    int LeftDetect = 0;
    int BoxDetect = 0;
  private:
    unsigned long Count = 0; // Count every 10ms elapsed for junction detection
    bool DetectDONE = 0; // The state whether the Junction detect funtion is active
    bool DetectDONE2 = 0;
    bool DetectDONE3 = 0;
    bool DetectDONE4 = 0;
    unsigned long DetectCounter = 0; // Counter for cold time in Junction Counter
    unsigned long DetectCounter2 = 0; // Counter for cold time in Junction Counter
    unsigned long DetectCounter3 = 0; // Counter for cold time in Junction Counter
    unsigned long DetectCounter4 = 0; // Counter for cold time in Junction Counter

    unsigned long Count1 = 0; // Count every 10ms elapsed for junction detection
    bool DetectDONE1 = 0; // The state whether the Junction detect funtion is active
    unsigned long DetectCounter1 = 0; // Counter for cold time in Junction Counter

    unsigned long BoxCount = 0;
    bool BoxDetectDONE = 0; // The state whether the Junction detect funtion is active
    unsigned long BoxDetectCounter = 0; // Counter for cold time in Junction Counter
};