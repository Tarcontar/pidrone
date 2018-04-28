#include "status.h"
#include "../hat_pcb.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

bool Status::m_ready = false;

void Status::initialize()
{
  if(m_ready)
  {
  	rcc_periph_clock_enable(_LED_STATUS_RCC_PORT);
    gpio_clear(_LED_STATUS_PORT, _LED_STATUS_PIN);
    gpio_set_mode(_LED_STATUS_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, _LED_STATUS_PIN);
    m_ready = true;
  }
}

void Status::update()
{
  initialize();
}