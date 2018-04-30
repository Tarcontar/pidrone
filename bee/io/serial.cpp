#include "serial.h"
#include "usart.h"
#include <stdlib.h>
#include <stdio.h>

const char* Serial::endl = "\r\n";

Serial::Serial(int baud)
{
	m_rate = baud;
	USART::begin(baud);
}

Serial& Serial::operator<<(char c)
{
	putchar(c);
	return *this;
}

Serial& Serial::operator<<(const char* str)
{
	printf("%s",str);
	return *this;
}

Serial& Serial::operator<<(int num)
{
	printf("%i",num);
	return *this;
}

Serial& Serial::operator<<(float num)
{
	printf("%f",num);
	return *this;
}