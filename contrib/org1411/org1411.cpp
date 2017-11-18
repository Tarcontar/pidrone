#include "org1411.h"
#include "SPI.h"
#include <Arduino.h>

bool Org1411::begin(int pin)
{
	delay(1000);
	SPI.begin();
	pinMode(m_pin, OUTPUT);
	digitalWrite(m_pin, LOW);
	int b;
	while (b = SPI.transfer(0xff))
	{
		Serial.print(b);
	}
	Serial.println();
	digitalWrite(m_pin, HIGH);
	return true;
}

void Org1411::read()
{
	digitalWrite(m_pin, LOW);
	
	int c;
	while (c = SPI.transfer('a'))
	{
		Serial.print(c);
	}
	Serial.println();
	
	digitalWrite(m_pin, HIGH);
}

