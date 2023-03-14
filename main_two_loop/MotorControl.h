/*
MotorControl.h
Containing Two low torque motors: Left and Right Motors; and one Servo for block-sweeper
*/
// #pragma once
#include "util.h" // contain global constants

class MotorControl {
  public:
    // set the number of steps per revolution of the motor (depends on the motor)
    const int STEPS_PER_REV = 120;
    int LeftStepCount = 0;
    int RightStepCount = 0;
    void MotorSetup(); // Setup code for Motor states
    void SetMotors(int LeftSpeed, int RightSpeed); // Simpler code for Motor behaviour
    bool LeftRotateDONE = false;
    bool RightRotateDONE = false;
    void LeftRotate(int leftR);
    void RightRotate(int rightR);
    void Reset();
    // Servo functions here
    void servoup();
    void servodown();
    int pos = 90; // Set initial position for servo
  private:
    unsigned long previousMillis = 0;
    unsigned long currentMillis = 0;
    int counter;
};