#include "Drone.h"
#include <HardwareSerial.h>

Drone drone;

void setup() 
{
  Serial.begin(9600);
  drone.SetSerial(Serial);
  drone.SetPins(2, 3, 4, 5, A0, A1, A2, A3);
  drone.Setup();
}


void loop() 
{
  drone.Update();
  delay(10);
}

