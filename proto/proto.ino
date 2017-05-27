#include "Drone.h"

Drone drone;

void setup() 
{
  drone.SetPins(A0, A1, A2, A3, 2, 3, 4, 5);
}


void loop() 
{
  //drone.Update();
  delay(10);
}

