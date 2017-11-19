#include <Arduino.h>
#include <BMI160Gen.h>
#include <BME280.h>
#include <org1411.h>
#include <Servo.h>
#include "io/pwmreceiver.h"


#define BMI160_PIN 8
#define BME280_PIN 7
#define GPS_PIN 5

#define STATUS_LED 2

#define MIN_THROTTLE 1000
#define MAX_THROTTLE 1700

PWMReceiver receiver;

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
	
	motorFL.attach(10);
	motorFR.attach(9);
	motorBR.attach(6);
	motorBL.attach(3);
	
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

void setMotorSpeeds()
{
	FLSpeed = min(max(FLSpeed, MIN_THROTTLE), MAX_THROTTLE);
	FRSpeed = min(max(FRSpeed, MIN_THROTTLE), MAX_THROTTLE);
	BRSpeed = min(max(BRSpeed, MIN_THROTTLE), MAX_THROTTLE);
	BLSpeed = min(max(BLSpeed, MIN_THROTTLE), MAX_THROTTLE);
	
	motorFL.writeMicroseconds(FLSpeed);
	motorFR.writeMicroseconds(FRSpeed);
	motorBR.writeMicroseconds(BRSpeed);
	motorBL.writeMicroseconds(BLSpeed);
}

void Roll(int value)
{
	FLSpeed += value;
	BLSpeed += value;
	FRSpeed -= value;
	BRSpeed -= value;
}

void Pitch(int value)
{
	FLSpeed += value;
	BLSpeed -= value;
	FRSpeed += value;
	BRSpeed -= value;
}

void Yaw(int value)
{
	FLSpeed -= value;
	BLSpeed += value;
	FRSpeed += value;
	BRSpeed -= value;
}

void Throttle(int value)
{
	FLSpeed += value;
	BLSpeed += value;
	FRSpeed += value;
	BRSpeed += value;
}

void setMotors(int speed)
{
	motorFL.writeMicroseconds(speed);
	motorFR.writeMicroseconds(speed);
	motorBR.writeMicroseconds(speed);
	motorBL.writeMicroseconds(speed);
}

void Foreward(int value)
{
	Pitch(value);
	Throttle(value);
}

void Back(int value)
{
	Pitch(-value);
	Throttle(value);
}

void Left(int value)
{
	Roll(-value);
	Throttle(value);
}

void Right(int value)
{
	Roll(value);
	Throttle(value);
}

void setup() 
{
	Serial.begin(9600);
	
	Serial.println("#### STARTING SETUP ####");
	pinMode(STATUS_LED, OUTPUT); 

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
	
	setupMotors();
	
	receiver.SetChannels(A0, A1, A2, A3, A4, A5);
	
	Serial.println("##### SETUP READY ######");
}

void loop() 
{
	digitalWrite(STATUS_LED, HIGH);
	delay(500);
	digitalWrite(STATUS_LED, LOW);
	delay(500);
	
	FLSpeed = MIN_THROTTLE;
	FRSpeed = MIN_THROTTLE;
	BRSpeed = MIN_THROTTLE;
	BLSpeed = MIN_THROTTLE;
	
	int thrust = receiver.getChannel(3) - MIN_THROTTLE;
	Throttle(thrust);
	
	/*
	if (thrust > 0)
	{
		Roll(receiver.getChannel(1) - 1500);
		Pitch(receiver.getChannel(2) - 1500);
		Yaw(receiver.getChannel(4) - 1500);
	}
	*/
	setMotorSpeeds();
}
