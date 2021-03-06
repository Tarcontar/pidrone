#include "motors.h"
#include "../sys/clock.h"
#include "../hat_pcb.h"

Motors::Motors() : m_FLspeed(MIN_THROTTLE)
, m_FRspeed(MIN_THROTTLE)
, m_BRspeed(MIN_THROTTLE)
, m_BLspeed(MIN_THROTTLE)
{
    m_timer = new Timer(MOTORS_RCC_TIMER, MOTORS_TIMER);
    m_timer->enableChannel(MOTOR_1_RCC_PORT, MOTOR_1_CHANNEL, MOTOR_1_PORT, MOTOR_1_PIN);
    m_timer->enableChannel(MOTOR_2_RCC_PORT, MOTOR_2_CHANNEL, MOTOR_2_PORT, MOTOR_2_PIN);
    m_timer->enableChannel(MOTOR_3_RCC_PORT, MOTOR_3_CHANNEL, MOTOR_3_PORT, MOTOR_3_PIN);
    m_timer->enableChannel(MOTOR_4_RCC_PORT, MOTOR_4_CHANNEL, MOTOR_4_PORT, MOTOR_4_PIN);

    setupESCs();
}

void Motors::setupESCs()
{
    m_timer->setChannel(MOTOR_1_CHANNEL, MIN_THROTTLE);
    m_timer->setChannel(MOTOR_2_CHANNEL, MIN_THROTTLE);
    m_timer->setChannel(MOTOR_3_CHANNEL, MIN_THROTTLE);
    m_timer->setChannel(MOTOR_4_CHANNEL, MIN_THROTTLE);
    Clock::sleep(3000); //TODO: decrease this
    m_timer->setChannel(MOTOR_1_CHANNEL, MAX_THROTTLE);
    m_timer->setChannel(MOTOR_2_CHANNEL, MAX_THROTTLE);
    m_timer->setChannel(MOTOR_3_CHANNEL, MAX_THROTTLE);
    m_timer->setChannel(MOTOR_4_CHANNEL, MAX_THROTTLE);
    Clock::sleep(2000); //TODO: decrease this
    m_timer->setChannel(MOTOR_1_CHANNEL, MIN_THROTTLE);
    m_timer->setChannel(MOTOR_2_CHANNEL, MIN_THROTTLE);
    m_timer->setChannel(MOTOR_3_CHANNEL, MIN_THROTTLE);
    m_timer->setChannel(MOTOR_4_CHANNEL, MIN_THROTTLE);
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

    m_timer->setChannel(MOTOR_1_CHANNEL, m_FLspeed);
    m_timer->setChannel(MOTOR_2_CHANNEL, m_FRspeed);
    m_timer->setChannel(MOTOR_3_CHANNEL, m_BRspeed);
    m_timer->setChannel(MOTOR_4_CHANNEL, m_BLspeed);
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
