#include <Arduino.h>

void setup() 
{
  Serial.begin(9600);
  pinMode(2,OUTPUT);
  Serial.println("Setup ready...");
}

void loop() 
{
  Serial.println("HIGH");
  digitalWrite(2,HIGH);
  delay(1000);
  Serial.println("LOW");
  digitalWrite(2,LOW);
  delay(1000);
}
