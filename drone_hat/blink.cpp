#include <Arduino.h>
#include <BMI160Gen.h>

#define BMI160_PIN 8

void setup() 
{
	Serial.begin(9600);
	pinMode(2,OUTPUT);
	Serial.println("Setup ready...");

	if (!BMI160.begin()BMI160GenClass::SPI_MODE, BMI160_PIN))
	{
		Serial.println("BMI SETUP ERROR!!!!");
	}
	
	
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
