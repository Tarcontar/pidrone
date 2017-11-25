#include <Arduino.h>
#include "io/sensors.h"
#include <SPI.h>
#include "hat_pcb.h"


Sensors sensors;

void setup() {
	Serial.begin(9600);

	Serial.println("\n######### STARTING SETUP ############");
	pinMode(LED_STATUS, OUTPUT);

	sensors.setup(8, 7);
}

void loop() {
	delay(1000);
	sensors.setup();
	sensors.update();
}
