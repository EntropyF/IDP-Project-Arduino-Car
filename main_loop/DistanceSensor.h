/*
Will contain both Ultra Sonic Distance Sensor and Colour Sensor functions.
*/
#include "util.h" // contain global constants

class DistanceSensor {
  public:
    long duration, distance;
    void DistanceSensorSetup();
    void DetectDistance();
    void ColourRecognition();
    int Colour = 0; // This integer contain the state of colour recognition
  private:
    unsigned long previousDetect = 0;  // variable to store the previous time
    unsigned long currentDetect = 0;
};