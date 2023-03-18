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
int s = 0;  // To record how many seconds has elapsed
// Set some variables for the Switch Button; One press for the system to stop, one press for the system to start.
int ledState = HIGH;                 // the current state of the output pin
int buttonState = HIGH;              // the current reading from the input pin
int lastButtonState = HIGH;          // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
int LostCounter = 0;                 // Set a counter to record how much time the car has been lost.
unsigned long ButtonCount = 0;       // Used in NewButton()

// Set more parameters to decribe the state of car position
int JDT = 300;  // (JunctionDetectTime) unit in 10 ms; reserve the time when junction is detect, 300 just for setting
int JDT2 = 300;
int JDT3 = 300;
int JDT4 = 300;
int JDT5 = 300;
unsigned long PinTimeCount1 = 0;
unsigned long PinTimeCount2 = 0;
unsigned long PinTimeCount3 = 0;
unsigned long PinTimeCount4 = 0;
unsigned long PinTimeCount5 = 0;
int BlockCOLLECT = 0;
int LOOP = 0;  // State information

void setup() {
  Serial.begin(9600);  //Enable Serial Communications
  Line.LineSensorSetup();
  Mcon.MotorSetup();
  Dis.DistanceSensorSetup();
  previousMillis = millis();
  pinMode(SwitchButton, INPUT);
  pinMode(BlinkLED, OUTPUT);
  pinMode(ResetPin, OUTPUT);
  digitalWrite(BlinkLED, ledState);  // set initial LED state
  digitalWrite(ResetPin, HIGH);
}


// Several sets of SetMotors that can make the car goes in straight line
// HighSpeed: Mcon.SetMotors(200,185);
// MiddleSpeed: Mcon.SetMotors(180,165);
// SlowSpeed: Mcon.SetMotors(150,130);
void loop() {
  // ### TIMER CODE ###
  unsigned long currentMillis = millis();            // get the current time
  if (currentMillis - previousMillis >= interval) {  // check if the interval has elapsed
    // ### Reset Button ###
    // NewButton();

    // ### Blink LED ###
    LEDblink();
    // Dis.ColourRecognition();
    // LineFollowerSlow();
    // Line.Junction1();
    PinTime();

    // Testing Area (10ms execution base)
    SystemUpdate();
    // TestCode();
    // TwoLoop();

    // ### TEST: stability of rotation (360 degree) ###
    // Want it to stay in the starting box
    // Mcon.RightRotate(500);
    // Mcon.LeftRotate(500);
    // Mcon.SetMotors(150,130);
    // Mcon.servoup();

    // ### TIMER RECORD ELAPSE TIME ###
    previousMillis = currentMillis;  // save the current time for the next comparison
    m++;                             // m integrates every 10 ms
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
      Serial.println(Line.RightDetect);
      Serial.print("JDT = ");
      Serial.println(JDT);
      Serial.print("JDT2 = ");
      Serial.println(JDT2);
      Serial.print("JDT3 = ");
      Serial.println(JDT3);
      Serial.print("JDT4 = ");
      Serial.println(JDT4);
      Serial.print("JDT5 = ");
      Serial.println(JDT5);
      Serial.print("Colour = ");
      Serial.println(Dis.Colour);
    }
  }
}

// This function record some significant time (from m, in unit 10ms)
// First three for RightDetect, 4 for box detect, 5 for leftdetect
void PinTime() {
  if (Line.RightDetect == 1) {
    PinTimeCount1++;  // In order to get m for JDT
    if (PinTimeCount1 == 3) {
      JDT = m;
    }
  }
  if (Line.RightDetect == 2) {
    PinTimeCount2++;  // In order to get m for JDT
    if (PinTimeCount2 == 3) {
      JDT2 = m;
    }
  }
  if (Line.RightDetect == 3) {
    PinTimeCount3++;  // In order to get m for JDT
    if (PinTimeCount3 == 3) {
      JDT3 = m;
    }
  }
  if (Line.BoxDetect == 1) {
    PinTimeCount4++;  // In order to get m for JDT
    if (PinTimeCount4 == 3) {
      JDT4 = m;
    }
  }
  if (Line.LeftDetect == 1) {
    PinTimeCount5++;  // In order to get m for JDT
    if (PinTimeCount5 == 3) {
      JDT5 = m;
    }
  }
}

// ### Line Follower ###
void LineFollower() {
  if ((digitalRead(LeftSensor) == 0) && (digitalRead(RightSensor) == 0)) {  // Set Motor speed to move forward when front sensor is active
    Mcon.SetMotors(180, 165);
  } else if (digitalRead(RightSensor) == 1) {  // Set Motor speed to turn right when right sensor is active
    Mcon.SetMotors(200, -100);
  } else if (digitalRead(LeftSensor) == 1) {  // Set Motor speed to turn left when left sensor is active
    Mcon.SetMotors(-180, 220);
  }
}

// Also Line Follower function, but in a much more slow speed, for testing and more precise junction counting.
void LineFollowerSlow() {
  if ((digitalRead(LeftSensor) == 0) && (digitalRead(RightSensor) == 0)) {  // Set Motor speed to move forward when front sensor is active
    Mcon.SetMotors(150, 130);
  } else if (digitalRead(RightSensor) == 1) {  // Set Motor speed to turn right when right sensor is active
    Mcon.SetMotors(220, 0);
  } else if (digitalRead(LeftSensor) == 1) {  // Set Motor speed to turn left when left sensor is active
    Mcon.SetMotors(-150, 220);
  }
}

void Position() {
  if ((digitalRead(LeftSensor) == 0) && (digitalRead(RightSensor) == 0)) {  // Set Motor speed to move forward when front sensor is active
    Mcon.SetMotors(150, 130);
  } else if (digitalRead(RightSensor) == 1) {  // Set Motor speed to turn right when right sensor is active
    Mcon.SetMotors(180, 100);
  } else if (digitalRead(LeftSensor) == 1) {  // Set Motor speed to turn left when left sensor is active
    Mcon.SetMotors(100, 180);
  }
}

// TestCode for picking the block and colour detect
void TestCode() {
  // An example to test GoHome1 function (Basically, to set the environment for this function: RightDetect No.)
  if (m == 10) {
    Line.RightDetect = 3;
  } else if ((m > 100) && (Line.LeftDetect = 0)) {
    LineFollowerSlow();
  } else if (Line.LeftDetect = 1) {
    GoHome2();
  } else {
    StopMotor();
  }
  Line.Junction1();
  Line.Junction();
  // if ((m > 100) && (Line.RightDetect == 0)) {
  //   LineFollowerSlow();
  // } else if (Line.RightDetect == 1) {
  //   PickBlock();
  // } else if (Line.RightDetect == 2) {
  //   StopMotor();
  // }
}

// Mcon.Reset() goes directly below a rotation
// Test go back through tunnel without picking block
void SystemUpdate() {
  if ((m >= 300) && (m < 600)) {
    Mcon.SetMotors(150, 135);
  } else if ((m >= 600) && (m <= 760)) {
    Mcon.LeftRotate(120);
  } else if (m == 760) {
    Mcon.Reset();
  } else if ((m >= 800) && (m <= 900)) {
    Mcon.SetMotors(150, 135);
  } else if ((m > 900) && (Line.RightDetect == 0)) {
    LineFollower();
  } else if (Line.RightDetect == 1) {
    PickBlock();
  }
  // The car should have gone back to the box side
  else if (Line.RightDetect == 2) {
    if (Dis.Colour == 2) {
      EnterGreen();
    } else if (Dis.Colour == 1) {
      LineFollowerSlow();
    }
  } else if (Line.RightDetect == 3) {
    if (Dis.Colour == 2) {
      GoHome1();
    } else if (Dis.Colour == 1) {
      EnterRed();
    }
  }
  // This is when Junction detect start
  if (m > 1000) {
    Line.Junction();
  }
  // if ((Line.RightDetect == 3) && (m > JDT3 + 1050)) {
  //   Line.Junction1();
  // }
}

void TwoLoop() {
  if (LOOP == 0) {
    if (Line.RightDetect == 0) {
      Initiation();
    } else if (Line.RightDetect == 1) {
      PickBlock();
    } else if (Line.RightDetect == 2) {
      if (Dis.Colour == 2) {
        EnterGreen2();
      } else if (Dis.Colour == 1) {
        LineFollower();
      }
    } else if (Line.RightDetect == 3) {
      EnterRed2();
    }
  } else if (LOOP == 1) {
    if (Line.RightDetect == 0) {
      LineFollower();
    } else if (Line.RightDetect == 1) {
      PickBlock();
    } else if (Line.RightDetect == 2) {
      if (Dis.Colour == 2) {
        EnterGreen();
      } else if (Dis.Colour == 1) {
        LineFollower();
      }
    } else if (Line.RightDetect == 3) {
      if (Dis.Colour == 2) {
        GoHome1();
      } else if (Dis.Colour == 1) {
        EnterRed();
      }
    }
  }
  // This is when Junction detect start
  if (m > 900) {
    Line.Junction();
  }
  if ((Line.RightDetect == 3) && (m > JDT4 + 1050)) {
    Line.Junction1();
  }
}

void ThreeLoop() {
  if (LOOP == 0) {
    if (Line.RightDetect == 0) {
      Initiation();
    } else if (Line.RightDetect == 1) {
      PickBlock();
    } else if (Line.RightDetect == 2) {
      if (Dis.Colour == 2) {
        EnterGreen2();
      } else if (Dis.Colour == 1) {
        LineFollower();
      }
    } else if (Line.RightDetect == 3) {
      EnterRed2();
    }
  } else if (LOOP == 1) {
    if (Line.RightDetect == 0) {
      LineFollower();
    } else if (Line.RightDetect == 1) {
      PickBlock();
    } else if (Line.RightDetect == 2) {
      if (Dis.Colour == 2) {
        EnterGreen2();
      } else if (Dis.Colour == 1) {
        LineFollower();
      }
    } else if (Line.RightDetect == 3) {
      EnterRed2();
    }
  } else if (LOOP == 2) {
    if (Line.RightDetect == 0) {
      LineFollower();
    } else if (Line.RightDetect == 1) {
      PickBlock();
    } else if (Line.RightDetect == 2) {
      if (Dis.Colour == 2) {
        EnterGreen();
      } else if (Dis.Colour == 1) {
        LineFollower();
      }
    } else if (Line.RightDetect == 3) {
      if (Dis.Colour == 2) {
        GoHome1();
      } else if (Dis.Colour == 1) {
        EnterRed();
      }
    }
  }
  // This is when Junction detect start
  if (m > 900) {
    Line.Junction();
  }
  if ((Line.RightDetect == 3) && (m > JDT4 + 1050)) {
    Line.Junction1();
  }
}


// Reset for another loop
void ResetState() {
  m = 0;
  s = 0;
  JDT = 300;
  JDT2 = 300;
  JDT3 = 300;
  JDT4 = 300;
  JDT5 = 300;
  PinTimeCount1 = 0;
  PinTimeCount2 = 0;
  PinTimeCount3 = 0;
  PinTimeCount4 = 0;
  PinTimeCount5 = 0;
  Line.RightDetect = 0;
  Dis.Colour = 0;
  LOOP++;
}

// ### FUNCTIONALITY ###
void Initiation() {
  if ((m >= 300) && (m < 600)) {
    Mcon.SetMotors(150, 135);
  } else if ((m >= 600) && (m <= 760)) {
    Mcon.LeftRotate(120);
  } else if (m == 760) {
    Mcon.Reset();
  } else if ((m >= 800) && (m <= 900)) {
    Mcon.SetMotors(150, 135);
  } else if (m > 900) {
    LineFollower();
  }
}

void PickBlock() {
  if ((m >= JDT) && (m < JDT + 50)) {
    StopMotor();
  } else if ((m > JDT + 50) && (m < JDT + 100)) {
    Mcon.SetMotors(150, 135);  // Flexible. To reach correct position
  } else if (m == JDT + 100) {
    Mcon.Reset();
  } else if ((m > JDT + 100) && (m < JDT + 280)) {  // ### Rotation MARK ###
    Mcon.RightRotate(134);
  } else if ((m > JDT + 280) && (m < JDT + 300)) {  // ### Rotation MARK ###
    Mcon.SetMotors(150, 135);
  } else if (m == JDT + 300) {  // Reset rotation information
    Mcon.Reset();
  } else if ((m > JDT + 300) && (m < JDT + 850)) {  // Colour recognition
    StopMotor();
    Dis.ColourRecognition();
  } else if ((m > JDT + 880) && (m < JDT + 900)) {  // ### Rotation MARK ###
    Mcon.SetMotors(-150, -135);
  } else if ((m > JDT + 900) && (m < JDT + 1100)) {  // Pick block
    StopMotor();
    if ((m > JDT + 1000) && (m < JDT + 1100)) {
      Mcon.servodown();
    }
  } else if (m == JDT + 1100) {  // set block collect to 1 state
    BlockCOLLECT = 1;
  } else if ((m > JDT + 1100) && (m < JDT + 1350)) {  // Go back to line // ### Rotation MARK ###
    Mcon.RightRotate(140);
  } else if (m == 1350) {  // Go back to line
    Mcon.Reset();
  } else if ((m > JDT + 1350) && (m < JDT + 1400)) {  // Move a bit forward
    Mcon.SetMotors(150, 135);
  } else if (m >= JDT + 1400) {
    LineFollower();
  }
}

void EnterRed() {
  if ((m >= JDT3) && (m < JDT3 + 50)) {
    StopMotor();
  } else if ((m > JDT3 + 50) && (m < JDT3 + 100)) {
    Mcon.SetMotors(150, 135);  // Flexible. To reach correct position
  } else if ((m == JDT3 + 100)) {
    Mcon.Reset();
  } else if ((m > JDT3 + 100) && (m < JDT3 + 400)) {  // ### Rotation MARK ###
    Mcon.RightRotate(150);
  } else if (m == JDT3 + 400) {  // Reset rotation information
    Mcon.Reset();
  } else if ((m > JDT3 + 400) && (m < JDT3 + 550)) {  // Go near block
    Mcon.SetMotors(150, 135);
  } else if ((m > JDT3 + 550) && (m < JDT3 + 650)) {  // drop block
    Mcon.servoup();
    StopMotor();
  } else if ((m == JDT3 + 650)) {  // block is put down, set state to 0
    BlockCOLLECT = 0;
  } else if ((m > JDT3 + 650) && (m < JDT3 + 750)) {  // go out of box
    Mcon.SetMotors(-150, -135);
  } else if ((m > JDT3 + 750) && (m < JDT3 + 1000)) {  // ### Rotation MARK ###
    Mcon.RightRotate(150);
  } else if (m == JDT3 + 1000) {  // Reset rotation information
    Mcon.Reset();
  } else if ((m > JDT3 + 1000) && (m < JDT3 + 1200)) {
    Mcon.SetMotors(150, 135);  // Flexible. To reach correct position
  } else if (m > JDT3 + 1200) {
    Line.Junction1();
    if (Line.LeftDetect == 0) {
      LineFollower();  // go back to line follower logic
    } else if (Line.LeftDetect >= 1) {
      GoHome2();
      // Mcon.RightRotate(150);
    }
  }
}

void EnterRed2() {
  if ((m >= JDT3) && (m < JDT3 + 50)) {
    StopMotor();
  } else if ((m > JDT3 + 50) && (m < JDT3 + 100)) {
    Mcon.SetMotors(150, 135);  // Flexible. To reach correct position
  } else if ((m == JDT3 + 100)) {
    Mcon.Reset();
  } else if ((m > JDT3 + 100) && (m < JDT3 + 400)) {  // ### Rotation MARK ###
    Mcon.RightRotate(150);
  } else if (m == JDT3 + 400) {  // Reset rotation information
    Mcon.Reset();
  } else if ((m > JDT3 + 400) && (m < JDT3 + 550)) {  // Go near block
    Mcon.SetMotors(150, 135);
  } else if ((m > JDT3 + 550) && (m < JDT3 + 650)) {  // drop block
    Mcon.servoup();
    StopMotor();
  } else if ((m == JDT3 + 650)) {  // block is put down, set state to 0
    BlockCOLLECT = 0;
  } else if ((m > JDT3 + 650) && (m < JDT3 + 800)) {  // go out of box
    Mcon.SetMotors(-150, -135);
  } else if ((m > JDT3 + 800) && (m < JDT3 + 1000)) {  // ### Rotation MARK ###
    Mcon.RightRotate(150);
  } else if (m == JDT3 + 1000) {  // Reset rotation information
    Mcon.Reset();
  } else if ((m > JDT3 + 1000) && (m < JDT3 + 1100)) {
    Mcon.SetMotors(150, 135);  // Flexible. To reach correct position
  } else if (m == JDT3 + 1100) {
    ResetState();
  }
}

void EnterGreen() {
  if ((m >= JDT2) && (m < JDT2 + 50)) {
    StopMotor();
  } else if ((m > JDT2 + 50) && (m < JDT2 + 100)) {
    Mcon.SetMotors(150, 135);  // Flexible. To reach correct position
  } else if ((m == JDT2 + 100)) {
    Mcon.Reset();
  } else if ((m > JDT2 + 100) && (m < JDT2 + 400)) {  // ### Rotation MARK ###
    Mcon.RightRotate(155);
  } else if (m == JDT2 + 400) {  // Reset rotation information
    Mcon.Reset();
  } else if ((m > JDT2 + 400) && (m < JDT2 + 550)) {  // Go near block
    Mcon.SetMotors(150, 135);
  } else if ((m > JDT2 + 550) && (m < JDT2 + 650)) {  // drop block
    Mcon.servoup();
    StopMotor();
  } else if ((m == JDT2 + 650)) {  // block is put down, set state to 0
    BlockCOLLECT = 0;
  } else if ((m > JDT2 + 650) && (m < JDT2 + 800)) {  // go out of box
    Mcon.SetMotors(-150, -135);
  } else if ((m > JDT2 + 800) && (m < JDT2 + 1000)) {  // ### Rotation MARK ###
    Mcon.LeftRotate(105);
  } else if (m == JDT2 + 1000) {  // Reset rotation information
    Mcon.Reset();
  } else if ((m > JDT2 + 1000) && (m < JDT2 + 1050)) {
    Mcon.SetMotors(150, 135);  // Flexible. To reach correct position
  } else if (m > JDT2 + 1050) {
    LineFollower();  // go back to line follower logic
  }
}

// Used for another loop
void EnterGreen2() {
  if ((m >= JDT2) && (m < JDT2 + 50)) {
    StopMotor();
  } else if ((m > JDT2 + 50) && (m < JDT2 + 100)) {
    Mcon.SetMotors(150, 135);  // Flexible. To reach correct position
  } else if ((m == JDT2 + 100)) {
    Mcon.Reset();
  } else if ((m > JDT2 + 100) && (m < JDT2 + 400)) {  // ### Rotation MARK ###
    Mcon.RightRotate(150);
  } else if (m == JDT2 + 400) {  // Reset rotation information
    Mcon.Reset();
  } else if ((m > JDT2 + 400) && (m < JDT2 + 550)) {  // Go near block
    Mcon.SetMotors(150, 135);
  } else if ((m > JDT2 + 550) && (m < JDT2 + 650)) {  // drop block
    Mcon.servoup();
    StopMotor();
  } else if ((m == JDT2 + 650)) {  // block is put down, set state to 0
    BlockCOLLECT = 0;
  } else if ((m > JDT2 + 650) && (m < JDT2 + 800)) {  // go out of box
    Mcon.SetMotors(-150, -135);
  } else if ((m > JDT2 + 800) && (m < JDT2 + 1000)) {  // ### Rotation MARK ###
    Mcon.RightRotate(150);
  } else if (m == JDT2 + 1000) {  // Reset rotation information
    Mcon.Reset();
  } else if ((m > JDT2 + 1000) && (m < JDT2 + 1050)) {
    Mcon.SetMotors(150, 135);  // Flexible. To reach correct position
  } else if (m == JDT2 + 1050) {
    ResetState();
  }
}

// For the case if block is blue, it can go home at junction 3
void GoHome1() {
  if ((m >= JDT3) && (m < JDT3 + 50)) {
    StopMotor();
  } else if ((m > JDT3 + 50) && (m < JDT3 + 100)) {
    Mcon.SetMotors(150, 135);  // Flexible. To reach correct position
  } else if (m == JDT3 + 100) {
    Mcon.Reset();
  } else if ((m > JDT3 + 100) && (m < JDT3 + 400)) {  // ### Rotation MARK ###
    Mcon.RightRotate(140);
  } else if (m == JDT3 + 400) {  // Reset rotation information
    Mcon.Reset();
  } else if (m > JDT3 + 400) {  // Go near block
    Line.Box();
    if (Line.BoxDetect == 0) {
      Position();
    } else {
      if (m < JDT4 + 50) {
        StopMotor();
      } else if ((m > JDT4 + 50) && (m < JDT4 + 170)) {
        Mcon.SetMotors(150, 135);
      } else if ((m > JDT4 + 170) || (m > JDT3 + 800)) {
        StopMotor();
      }
    }
  }
}

// For the case if block is red, it will go home at junction 5
void GoHome2() {
  if ((m >= JDT5) && (m < JDT5 + 50)) {
    StopMotor();
  } else if ((m > JDT5 + 50) && (m < JDT5 + 100)) {
    Mcon.SetMotors(150, 135);  // Flexible. To reach correct position
  } else if (m == JDT5 + 100) {
    Mcon.Reset();
  } else if ((m > JDT5 + 100) && (m < JDT5 + 400)) {  // ### Rotation MARK ###
    Mcon.LeftRotate(110);
  } else if (m == JDT5 + 400) {  // Reset rotation information
    Mcon.Reset();
  } else if (m > JDT5 + 400) {  // Go near block
    Line.Box();
    if (Line.BoxDetect == 0) {
      Position();
    } else {
      if (m < JDT4 + 50) {
        StopMotor();
      } else if ((m > JDT4 + 50) && (m < JDT4 + 170)) {
        Mcon.SetMotors(150, 135);
      } else if ((m > JDT4 + 170) || (m > JDT3 + 800)) {
        StopMotor();
      }
    }
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
// The debouncing system is quite unstable
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

// Only when press for 1 second can the system been reset
void NewButton() {
  if (digitalRead(SwitchButton) == 0) {
    ButtonCount++;
  } else {
    ButtonCount = 0;
  }
  if (ButtonCount == 100) {
    digitalWrite(ResetPin, LOW);
  } else {
    digitalWrite(ResetPin, HIGH);
  }
}

// The function is simply stopping the motors
void StopMotor() {
  Mcon.SetMotors(0, 0);
}
