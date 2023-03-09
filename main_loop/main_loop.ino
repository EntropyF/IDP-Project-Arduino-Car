/*
main_loop.ino
Main code for Team L212's IDP robot
Mcon.setMotors(200,187) can get robot goes on straight line
rotation set leftR is roughly 250-270 for 90 degree
*/

#include "LineSensor.h"
#include "util.h"
#include "MotorControl.h"
#include "DistanceSensor.h"

// Create objects to access modules
LineSensor Line;
MotorControl Mcon;
DistanceSensor Dis;

unsigned long previousMillis = 0;  // variable to store the previous time
const long interval = 10;          // interval at which to do something (in milliseconds)

int m = 0;  // To Record the time (100 for 1 second) unit: 10ms
int s = 0;        // To record how many seconds has elapsed
// Set some variables for the Switch Button; One press for the system to stop, one press for the system to start.
int ledState = HIGH;                 // the current state of the output pin
int buttonState = HIGH;              // the current reading from the input pin
int lastButtonState = HIGH;          // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
int LostCounter = 0;                 // Set a counter to record how much time the car has been lost.

// Set more parameters to decribe the state of car position
int JDT = 300; // (JunctionDetectTime) unit in 10 ms; reserve the time when junction is detect, 300 just

void setup() {
  Serial.begin(9600);  //Enable Serial Communications
  Line.LineSensorSetup();
  Mcon.MotorSetup();
  Dis.DistanceSensorSetup();
  previousMillis = millis();
  pinMode(SwitchButton, INPUT);
  pinMode(BlinkLED, OUTPUT);
  digitalWrite(BlinkLED, ledState);  // set initial LED state
}

void loop() {
  // ### TIMER CODE ###
  unsigned long currentMillis = millis();            // get the current time
  if (currentMillis - previousMillis >= interval) {  // check if the interval has elapsed
    // Button();

    // ### Blink LED ###
    LEDblink();

    // LineFollower();
    Line.Junction();
    PinTime();
    
    // Testing Area (10ms execution base)
    SystemUpdate();
    // if(Line.RightDetect == 0) {
    //   LineFollower();
    // } else {
    //   StopMotor();
    // }
    // Dis.ColourRecognition();
    // Mcon.LeftRotate(250);
    

    // ### TIMER RECORD ELAPSE TIME ###
    previousMillis = currentMillis;  // save the current time for the next comparison
    m++;                       // m integrates every 10 ms
    if (m % 100 == 0) {
      s++;  // s records the second elpse of the whole process
      // Print System state (0 for off; 1 for on) and Time elapse for s seconds
      // Serial.print("LED state: ");
      // Serial.print(ledState);
      // Serial.print("  Time elapse: ");
      // Serial.print(s);
      // Serial.println(" seconds");
      // // Print anything useful
      Serial.print("RightDetect: ");
      Serial.println(digitalRead(FrontSensor));
      Serial.println(JDT);
    }
  }
}

// This function record some significant time (from m, in unit 10ms)
void PinTime() {
  if(Line.RightDetect == !Line.RightDetect) {
    JDT = m;
  }
}

// TestCode for simple movement
void TestCode() {
  if ((s >= 3) && (s <= 5)) {
    Mcon.SetMotors(120, 110);
  } else if (s <= 6) {
    StopMotor();
  } else if (s <= 8) { // a bit over 90 degree, but too small if set m<=780
    Mcon.SetMotors(-100, 100);
  } else if (s <= 9) {
    StopMotor();
  } else if (s <= 12) {
    Mcon.SetMotors(150, 140);
  } else {
    StopMotor();
  }
}

// Test first whether it can stop at the first junction tmr
void SystemUpdate() {
  if((s>=3)&&(s<=5)) {
    Mcon.SetMotors(150, 140);
  }
  else if((s>=6)&&(s<=9)) {
    Mcon.LeftRotate(250);
  }
  else if((s>=9)&&(Line.RightDetect == 0)) {
    LineFollower();
  }
  else if(Line.RightDetect == 1) {
    if((m>=JDT)&&(m<=JDT+50)) {
      StopMotor();
    }
    else if((m>=JDT+50)&&(m<=JDT+600)) {
      Mcon.RightRotate(500);
    }
    else if(m>=JDT+600) {
      LineFollower();
    }
  }
  else if(Line.RightDetect == 2) {
    StopMotor();
  }
}

// ### Line Follower ###
void LineFollower() {
  if ((digitalRead(FrontSensor) == 1) && (digitalRead(LeftSensor) == 0) && (digitalRead(RightSensor) == 0)) {  // Set Motor speed to move forward when front sensor is active
    Mcon.SetMotors(180, 170);
  } else if ((digitalRead(FrontSensor) == 0) && (digitalRead(RightSensor) == 1)) {  // Set Motor speed to turn right when right sensor is active
    Mcon.SetMotors(200, 0);
  } else if ((digitalRead(FrontSensor) == 0) && (digitalRead(LeftSensor) == 1)) {  // Set Motor speed to turn left when left sensor is active
    Mcon.SetMotors(0, 200);
  }
}

// Also Line Follower function, but in a much more slow speed, for testing and more precise junction counting.
void LineFollowerSlow() {
  if ((digitalRead(FrontSensor) == 1) && (digitalRead(LeftSensor) == 0) && (digitalRead(RightSensor) == 0)) {  // Set Motor speed to move forward when front sensor is active
    Mcon.SetMotors(150, 140);
  } else if ((digitalRead(FrontSensor) == 0) && (digitalRead(RightSensor) == 1)) {  // Set Motor speed to turn right when right sensor is active
    Mcon.SetMotors(180, 0);
  } else if ((digitalRead(FrontSensor) == 0) && (digitalRead(LeftSensor) == 1)) {  // Set Motor speed to turn left when left sensor is active
    Mcon.SetMotors(0, 180);
  }
}

// ### Blink LED ###
// Blink LED if ledState is HIGH, Stop LED and Motor when ledState is LOW.
void LEDblink() {
  if (ledState == LOW) {
    StopMotor();
    digitalWrite(BlinkLED, LOW);
  } else {
    if (m % 50 <= 25) {
      digitalWrite(BlinkLED, HIGH);
    } else {
      digitalWrite(BlinkLED, LOW);
    }
  }
}


// ### Switch Button ###
// Quite unstable right now
void Button() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(SwitchButton);

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == LOW) {
        ledState = !ledState;
      }
    }
  }
  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}

void StopMotor() {
  Mcon.SetMotors(0, 0);
}

