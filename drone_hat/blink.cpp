#include <Arduino.h>
#include <BMI160Gen.h>
#include <BME280.h>
#include <org1411.h>

#define BMI160_PIN 8
#define BME280_PIN 7
#define GPS_PIN 5

BME280 bme(BME280_PIN);

bool has_bmi = false;
bool has_bme = false;
bool has_gps = false;

#define PRESSURE_SEALEVEL 1013.25F

void printAccel()
{
	if (has_bmi)
	{
		int x, y, z;
		BMI160.readAccelerometer(x, y, z);
		
		Serial.print("Accel: ");
		Serial.print(" X: ");
		Serial.print(x);
		Serial.print(" Y: ");
		Serial.print(y);
		Serial.print(" Z: ");
		Serial.print(z);
		
		Serial.println();
	}
}

void printBME()
{
	if (has_bme)
	{
		Serial.print("Temperature = ");
		Serial.print(bme.readTemperature());
		Serial.println(" C");
		
		Serial.print("Pressure = ");
		Serial.print(bme.readPressure() / 100.0F);
		Serial.println(" hPa");
		
		Serial.print("Altitude = ");
		Serial.print(bme.readAltitude(PRESSURE_SEALEVEL));
		Serial.println(" m");
		
		Serial.print("Humidity = ");
		Serial.print(bme.readHumidity());
		Serial.println(" %");
	}
}

Org1411 gps;

void setup() 
{
	Serial.begin(9600);
	pinMode(2,OUTPUT);
	Serial.println("Setup ready...");

	if (!gps.begin(GPS_PIN))
	{
		Serial.println("GPS SETUP ERROR");
	}
	else
	{
		Serial.println("GPS SUCCESS");
	}
	

	if (!BMI160.begin(BMI160GenClass::SPI_MODE, BMI160_PIN))
	{
		Serial.println("BMI SETUP ERROR!!!!");
	}
	else 
	{
		has_bmi = true;
		Serial.println("BMI SUCCESSFULL");
		BMI160.setGyroRate(25);
		BMI160.setAccelerometerRate(25);
		
		BMI160.setGyroRange(250);
		BMI160.setAccelerometerRange(2);
		
		delay(200);
		printAccel();
	}
	
	if (!bme.begin())
	{
		Serial.println("BME SETUP ERROR");
	}
	else
	{
		has_bme = true;
		Serial.println("BME SUCCESSFULL");	
		printBME();
	}
}

void loop() 
{
	Serial.println("HIGH");
	digitalWrite(2,HIGH);
	printAccel();
	delay(1000);
	Serial.println("LOW");
	digitalWrite(2,LOW);
	delay(1000);
	printBME();
}
