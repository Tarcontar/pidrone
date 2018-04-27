#include "motors.h"
#include "../hat_pcb.h"

Motors::Motors() : m_FLspeed(MIN_THROTTLE), m_FRspeed(MIN_THROTTLE), m_BRspeed(MIN_THROTTLE), m_BLspeed(MIN_THROTTLE)
{
	m_timer = new Timer(_MOTORS_RCC_TIMER, _MOTORS_TIMER);
	m_timer->enableCH(_MOTOR_1_RCC_PORT, _MOTOR_1_CHANNEL, _MOTOR_1_PORT, _MOTOR_1_PIN);
	m_timer->enableCH(_MOTOR_2_RCC_PORT, _MOTOR_2_CHANNEL, _MOTOR_2_PORT, _MOTOR_2_PIN);
	m_timer->enableCH(_MOTOR_3_RCC_PORT, _MOTOR_3_CHANNEL, _MOTOR_3_PORT, _MOTOR_3_PIN);
	m_timer->enableCH(_MOTOR_4_RCC_PORT, _MOTOR_4_CHANNEL, _MOTOR_4_PORT, _MOTOR_4_PIN);
}

void Motors::update(int value)
{
	m_timer->setCH(_MOTOR_1_CHANNEL, value);
}

void Motors::setupESCs()
{
	m_timer->setCH(_MOTOR_1_CHANNEL, MIN_THROTTLE);
	m_timer->setCH(_MOTOR_2_CHANNEL, MIN_THROTTLE);
	m_timer->setCH(_MOTOR_3_CHANNEL, MIN_THROTTLE);
	m_timer->setCH(_MOTOR_4_CHANNEL, MIN_THROTTLE);
	for (uint32_t i = 0; i < 3000; i++)
		__asm__("NOP");
	m_timer->setCH(_MOTOR_1_CHANNEL, MAX_THROTTLE);
	m_timer->setCH(_MOTOR_2_CHANNEL, MAX_THROTTLE);
	m_timer->setCH(_MOTOR_3_CHANNEL, MAX_THROTTLE);
	m_timer->setCH(_MOTOR_4_CHANNEL, MAX_THROTTLE);
	for (uint32_t i = 0; i < 2; i++)
		__asm__("NOP");
	m_timer->setCH(_MOTOR_1_CHANNEL, MIN_THROTTLE);
	m_timer->setCH(_MOTOR_2_CHANNEL, MIN_THROTTLE);
	m_timer->setCH(_MOTOR_3_CHANNEL, MIN_THROTTLE);
	m_timer->setCH(_MOTOR_4_CHANNEL, MIN_THROTTLE);
}

void Motors::update(int throttle, int roll, int pitch, int yaw)
{
	Throttle(throttle + MIN_THROTTLE);
	Roll(roll);
	Pitch(pitch);
	Yaw(yaw);
	
	//m_FLspeed = min(max(m_FLspeed, MIN_THROTTLE), MAX_THROTTLE);
	//m_FRspeed = min(max(m_FRspeed, MIN_THROTTLE), MAX_THROTTLE);
	//m_BRspeed = min(max(m_BRspeed, MIN_THROTTLE), MAX_THROTTLE);
	//m_BLspeed = min(max(m_BLspeed, MIN_THROTTLE), MAX_THROTTLE);
	
	m_timer->setCH(_MOTOR_1_CHANNEL, m_FLspeed);
	m_timer->setCH(_MOTOR_2_CHANNEL, m_FRspeed);
	m_timer->setCH(_MOTOR_3_CHANNEL, m_BRspeed);
	m_timer->setCH(_MOTOR_4_CHANNEL, m_BLspeed);
}

void Motors::Roll(int value)
{
	m_FLspeed += value;
	m_BLspeed += value;
	m_FRspeed -= value;
	m_BRspeed -= value;
}

void Motors::Pitch(int value)
{
	m_FLspeed += value;
	m_BLspeed -= value;
	m_FRspeed += value;
	m_BRspeed -= value;
}

void Motors::Yaw(int value)
{
	m_FLspeed -= value;
	m_BLspeed += value;
	m_FRspeed += value;
	m_BRspeed -= value;
}

void Motors::Throttle(int value)
{
	m_FLspeed += value;
	m_BLspeed += value;
	m_FRspeed += value;
	m_BRspeed += value;
}
