#include <Arduino.h>
#include <stdint.h>
//#include "io/pwmreceiver.h"
//#include "io/sensors.h"
//#include "math/pid.h"
//#include "actuators/motors.h"
#include "hat_pcb.h"

#include "BMI160Gen.h"

#define PRESSURE_SEALEVEL 1013.25F

//PWMReceiver receiver;

/*
Sensors sensors;
Motors motors(FL_MOTOR, FR_MOTOR, BR_MOTOR, BL_MOTOR);

PID rollPID(1.0);
PID pitchPID(1.0);
PID yawPID(1.0);
*/

//use these in debug only!!
bool has_bmi = false;
bool has_bme = false;
bool has_gps = false;


void setup() 
{
	Serial.begin(9600);

	Serial.println("#### STARTING SETUP ####");
	pinMode(LED_STATUS, OUTPUT); 

	//sensors.setup();
	
	
	if (!BMI160.begin(BMI160GenClass::SPI_MODE, 8))
	{
		Serial.println("BMI160 setup failed");
	}
	else
	{
		BMI160.setGyroRate(25);
		BMI160.setAccelerometerRate(25);
		BMI160.setGyroRange(250);
		BMI160.setAccelerometerRange(2);
		delay(200);
	}
	
	
	//motors.setupESCs();
	
	//int8_t channels[6] = {CH1, CH2, CH3, CH4, CH5, CH6};
	//receiver.setChannels(channels);

	Serial.println("##### SETUP READY ######");
}

void loop() 
{
	digitalWrite(LED_STATUS, HIGH);
	delay(500);
	digitalWrite(LED_STATUS, LOW);
	delay(500);
	
	
	int x, y, z;
	BMI160.readAccelerometer(x, y, z);
	Serial.println(x);
	
	//sensors.setup();

	/*
	//filter these receiver values?
	int thrust = receiver.getChannel(PWMReceiver::CHANNEL::THRUST);
	int rc_roll = receiver.getChannel(PWMReceiver::CHANNEL::ROLL);
	int rc_pitch = receiver.getChannel(PWMReceiver::CHANNEL::PITCH);
	int rc_yaw = receiver.getChannel(PWMReceiver::CHANNEL::YAW);
	
	rollPID.setTarget(rc_roll);
	pitchPID.setTarget(rc_pitch);
	yawPID.setTarget(rc_yaw);
	
	sensors.update();
	//get accel values and map to 1000 - 2000
	
	int roll = 0;
	int pitch = 0;
	int yaw = 0;

	int r = rollPID.update(roll);
	int p = pitchPID.update(pitch);
	int y = yawPID.update(yaw);
	
	motors.update(thrust, r, p, y);
	*/
}