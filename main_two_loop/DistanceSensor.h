/*
Will contain both Ultra Sonic Distance Sensor and Colour Sensor functions.
*/
#include "util.h" // contain global constants

class DistanceSensor {
  public:
    long duration, distance; // data calculated in distance sensor
    void DistanceSensorSetup();
    void DetectDistance(); // Ultra Sonic distance sensor, gives distance in cm
    void ColourRecognition(); // The colour information recognition
    void Tunnel();
    void Block(); // this is used as a redundancy if junction counter doesn't work
    int Colour = 0; // This integer contain the state of colour recognition, 1 is red, 2 is blue
    unsigned long tunnelcount = 0; // This is used to count how many 10 ms has the robot been in tunnel
    unsigned long blockcount = 0;
    int TunnelDETECT = 0; // 1 when tunnel is detected
    int BlockDETECT = 0;
  private:
    unsigned long previousDetect = 0;  // variable to store the previous time
    unsigned long currentDetect = 0;
};