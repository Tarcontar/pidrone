#include <Arduino.h>
#include <BMI160Gen.h>
#include <BME280.h>
#include <org1411.h>
#include <Servo.h>

#define BMI160_PIN 8
#define BME280_PIN 7
#define GPS_PIN 5

#define MIN_THROTTLE 1000
#define MAX_THROTTLE 1700

int FLSpeed = 0;
int FRSpeed = 0;
int BRSpeed = 0;
int BLSpeed = 0;

Servo motorFL;
Servo motorFR;
Servo motorBR;
Servo motorBL;

BME280 bme(BME280_PIN);
Org1411 gps;

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

void printGPS()
{
	if (has_gps)
	{
		gps.read();
	}
}

void setupMotors()
{
	FLSpeed = MIN_THROTTLE;
	FRSpeed = MIN_THROTTLE;
	BRSpeed = MIN_THROTTLE;
	BLSpeed = MIN_THROTTLE;
	
	motorFL.attach(14);
	motorFR.attach(13);
	motorBR.attach(10);
	motorBL.attach(1);
	
	int min = 1000;
	int max = 2000;
	motorFL.writeMicroseconds(min);
	motorFR.writeMicroseconds(min);
	motorBR.writeMicroseconds(min);
	motorBL.writeMicroseconds(min);
	delay(3000);
	motorFL.writeMicroseconds(max);
	motorFR.writeMicroseconds(max);
	motorBR.writeMicroseconds(max);
	motorBL.writeMicroseconds(max);
	delay(2);
	motorFL.writeMicroseconds(min);
	motorFR.writeMicroseconds(min);
	motorBR.writeMicroseconds(min);
	motorBL.writeMicroseconds(min);
}

void startMotors()
{
	int val = 1200;
	motorFL.writeMicroseconds(val);
	motorFR.writeMicroseconds(val);
	motorBR.writeMicroseconds(val);
	motorBL.writeMicroseconds(val);
}

void setup() 
{
	Serial.begin(9600);
	pinMode(2,OUTPUT);
	Serial.println("Setup ready...");
	
	setupMotors();

	if (!gps.begin(GPS_PIN))
	{
		Serial.println("GPS SETUP ERROR");
	}
	else
	{
		has_gps = true;
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
	
	startMotors();
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
