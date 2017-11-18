#include <Arduino.h>
#include <SoftwareSerial.h>


SoftwareSerial serial(0, 1);

void setup() 
{
  serial.begin(9600);
  pinMode(2,OUTPUT);
  serial.println("Setup ready...");
}

void loop() 
{
  serial.println("HIGH");
  digitalWrite(2,HIGH);
  delay(1000);
  serial.println("LOW");
  digitalWrite(2,LOW);
  delay(1000);
}
