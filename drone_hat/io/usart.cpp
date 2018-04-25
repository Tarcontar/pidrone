#include "usart.h"
#include "../hat_pcb.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

void USART::begin(int baud)
{
	rcc_periph_clock_enable(_USART_RCC_PORT); 
	rcc_periph_clock_enable(_USART_RCC_USART_PORT);
	
	gpio_set_mode(_USART_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, _USART_TX);
	
	
	//did not work the last time, test first
	usart_set_baudrate(_USART, baud);
	//USART_BRR(_USART) = (uint16_t) ((24000000 << 4) / (baud * 16)); 
	
	
	usart_set_databits(_USART, 8);
	usart_set_stopbits(_USART, USART_STOPBITS_1);
	usart_set_mode(_USART, USART_MODE_TX);
	usart_set_parity(_USART, USART_PARITY_NONE);
	usart_set_flow_control(_USART, USART_FLOWCONTROL_NONE);
	
	usart_enable(_USART);
}

void USART::write(int c)
{
	usart_send_blocking(_USART, c);
}

/*


void USART::print(std::string& str)
{
	for (char& c : str)
		print(c);
}

template <class T>
void USART::print(T t)
{
	//does not compile, MinGW bug
	//print(std::to_string(t));
}

template <class T>
void USART::println(T t)
{
	print(t);
	println();
}
*/
