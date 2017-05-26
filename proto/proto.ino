#include "Drone.h"

Drone drone(2, 3, 4, 5, A0, A1, A2, A3);

void setup() 
{
 Serial.begin(9600);
 while(!Serial);
}


void loop() 
{
  drone.Update();
  delay(10);
}

