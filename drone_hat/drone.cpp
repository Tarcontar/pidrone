#include <Arduino.h>
#include <stdint.h>
#include "io/pwmreceiver.h"
#include "io/sensors.h"
#include "actuators/motors.h"
#include "math/pid.h"
#include "hat_pcb.h"

#define PRESSURE_SEALEVEL 1013.25F

Sensors sensors;

PWMReceiver receiver;

Motors motors(FL_MOTOR, FR_MOTOR, BR_MOTOR, BL_MOTOR);

PID rollPID(1.0);
PID pitchPID(1.0);
PID yawPID(1.0);

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
	
	motors.setupESCs();
	
	uint8_t channels[6] = {CH1, CH2, CH3, CH4, CH5, CH6};
	receiver.setChannels(channels);
	
	digitalWrite(LED_STATUS, HIGH);
	Serial.println("\n######### SETUP READY ##############");
}

void loop() 
{
	//filter the receiver values?
	int rc_thrust = receiver.getChannel(PWMReceiver::CHANNEL::THRUST);
	int rc_roll = receiver.getChannel(PWMReceiver::CHANNEL::ROLL);
	int rc_pitch = receiver.getChannel(PWMReceiver::CHANNEL::PITCH);
	int rc_yaw = receiver.getChannel(PWMReceiver::CHANNEL::YAW);
	
	rollPID.setTarget(rc_roll);
	pitchPID.setTarget(rc_pitch);
	yawPID.setTarget(rc_yaw);
	
	double roll = 0;
	double pitch = 0;
	double yaw = 0;
	
	sensors.readBMI(&roll, &pitch, &yaw);
	
	Serial.println();
	Serial.print("roll: ");
	Serial.print(roll);
	Serial.print(" pitch: ");
	Serial.print(pitch);
	Serial.println();
	
	int r = rollPID.update(roll);
	int p = pitchPID.update(pitch);
	int y = yawPID.update(yaw);
	
	motors. update(rc_thrust, r, p, 0); //dont use yaw yet
}
