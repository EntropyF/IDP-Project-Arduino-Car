// #pragma once
#include "MotorControl.h"
#include "util.h"  // contain some key constants
#include <Adafruit_MotorShield.h>
#include <Servo.h>
// Set myServo as the module that can be called
Servo servo1;

// ### Connection Instruction ###
// R Motor connect to M2; L Motor connect to M1

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select M1, M2 for Motor ports (Left and Right)
Adafruit_DCMotor *LeftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *RightMotor = AFMS.getMotor(2);

void MotorControl::MotorSetup() {
  AFMS.begin();  //To enable two motors to run
  RightMotor->run(RELEASE);
  LeftMotor->run(RELEASE);
  servo1.attach(9); // Attach the servo to pin 9
  servo1.write(90); // Set the initial position to the center
}

void MotorControl::SetMotors(int LeftSpeed, int RightSpeed) {
  if (LeftSpeed >= 0) {
    LeftMotor->run(BACKWARD);    
  } else {  
    LeftMotor->run(FORWARD);    
  }
  if (RightSpeed >= 0) {
    RightMotor->run(BACKWARD);
  } else {
    RightMotor->run(FORWARD);
  }
  LeftMotor->setSpeed(LeftSpeed);
  RightMotor->setSpeed(RightSpeed);
}

// ### Programmer Once functions ###
// The two rotation functions can operate only once in certain assigned time period.
// But for another call of these functions we need to do a Reset() before the next time period. 
// Reset() can be allocated to a certain Counter Number.

// 90 degree rotation functions: leftR = 250
void MotorControl::LeftRotate(int leftR) { // Time is in unit 10ms
  if (LeftRotateDONE == false) {
    SetMotors(-150, 150);
    counter++;
  } else {
    SetMotors(0, 0);
  }
  if (counter >= leftR) { // Try 3 seconds with speed 120 
    LeftRotateDONE = true;
  }
}

void MotorControl::RightRotate(int rightR) { // Time is in unit 10ms
  if (RightRotateDONE == false) {
    SetMotors(150, -150);
    counter++;
  } else {
    SetMotors(0, 0);
  }
  if (counter >= rightR) { // Try 3 seconds with speed 120 
    RightRotateDONE = true;
  }
}

void MotorControl::Reset() {
  counter = 0;
  RightRotateDONE = false;
  LeftRotateDONE = false;
}

void MotorControl::pick() {
  while (true) {
    if (ServoPosition < ServoTarget) {
      ServoPosition += speed;
    }
    if (abs(ServoPosition - ServoTarget) < speed) {
      break;
    }
    servo1.write(ServoPosition);
  }
}

void MotorControl::drop() {
  while (true) {
    if (ServoPosition < ServoTarget) {
      ServoPosition -= speed;
    }
    if (abs(ServoPosition - 0) < speed) {
      break;
    }
    servo1.write(ServoPosition);
  }
}