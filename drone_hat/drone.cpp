#include <Arduino.h>
#include "io/sensors.h"
#include <SPI.h>
#include "hat_pcb.h"

Sensors sensors;

void blink(int count)
{
	while (true)
	{
		for (int i = 0; i < count; i++)
		{
			digitalWrite(LED_STATUS, HIGH);
			delay(100);
			digitalWrite(LED_STATUS, LOW);
			delay(100);
		}
		delay(1000);
	}
}

void setup() 
{
	Serial.begin(9600);

	Serial.println("\n######### STARTING SETUP ############");
	pinMode(LED_STATUS, OUTPUT);

	if (!sensors.setup(BMI_CS, BME_CS))
	{
		Serial.println("Sensor setup failed");
		blink(2);
	}
	
	digitalWrite(LED_STATUS, HIGH);
	Serial.println("\n######### SETUP READY ##############");
}

void loop() 
{
	delay(1000);
	sensors.update();
}
