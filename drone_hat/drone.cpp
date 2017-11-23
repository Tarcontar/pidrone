#include <Arduino.h>
#include <stdint.h>
#include "io/pwmreceiver.h"
#include "io/sensors.h"
#include "math/pid.h"
#include "actuators/motors.h"
#include "hat_pcb.h"

#define PRESSURE_SEALEVEL 1013.25F

PWMReceiver receiver;

Sensors sensors;
Motors motors(FL_MOTOR, FR_MOTOR, BR_MOTOR, BL_MOTOR);

PID rollPID(1.0);
PID pitchPID(1.0);
PID yawPID(1.0);

//use these in debug only!!
bool has_bmi = false;
bool has_bme = false;
bool has_gps = false;


void setup() 
{
	Serial.begin(9600);

	Serial.println("#### STARTING SETUP ####");
	pinMode(LED_STATUS, OUTPUT); 

	sensors.setup();
	
	motors.setupESCs();
	
	int8_t channels[6] = {CH1, CH2, CH3, CH4, CH5, CH6};
	receiver.SetChannels(channels);
	
	float roll = 0.0;
	float pitch = 0.0;
	float yaw = 0.0;
	
	//TODO: save calibrated accel values to eeprom
	
	rollPID.setTarget(roll);
	pitchPID.setTarget(pitch);
	yawPID.setTarget(yaw);
	
	Serial.println("##### SETUP READY ######");
}

void loop() 
{
	digitalWrite(LED_STATUS, HIGH);
	delay(500);
	digitalWrite(LED_STATUS, LOW);
	delay(500);

	sensors.update();
	//get accel values
	float roll = 0.0;
	float pitch = 0.0;
	float yaw = 0.0;
	//hacky since we cant use floats in the motors class (PWM 1000 - 2000)
	int r = rollPID.update(roll) * 10;
	int p = pitchPID.update(pitch) * 10;
	int y = yawPID.update(yaw) * 10;
	
	int thrust = 0; //get thrust from receiver channel
	motors.update(thrust, r, p, 0);
}