#include <Arduino.h>
#include "io/sensors.h"
#include <SPI.h>
#include "hat_pcb.h"

Sensors sensors;

void blink(int ms)
{
	while (true)
	{
		digitalWrite(LED_STATUS, HIGH);
		delay(ms);
		digitalWrite(LED_STATUS, LOW);
		delay(ms);
	}
}

void setup() 
{
	Serial.begin(9600);

	Serial.println("\n######### STARTING SETUP ############");
	pinMode(LED_STATUS, OUTPUT);

	if (!sensors.setup(8, 7))
	{
		Serial.println("Sensor setup failed");
		blink(500);
	}
	
	digitalWrite(LED_STATUS, HIGH);
	Serial.println("\n######### SETUP READY ##############");
}

void loop() 
{
	delay(1000);
	sensors.update();
}
