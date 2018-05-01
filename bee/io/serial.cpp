#include "serial.h"
#include "usart.h"
#include <stdlib.h>
#include <stdio.h>

const char* Serial::endl = "\r\n";

Serial::Serial(int baud)
{
	USART::begin(baud);
}

Serial& Serial::operator<<(char c)
{
	USART::write(c);
	return *this;
}

Serial& Serial::operator<<(const char* str)
{
	char *it = const_cast<char *>(str);
	while (*it)
	{
		*this << *it;
		++it;
	}
	return *this;
}

Serial& Serial::operator<<(const int i)
{
	char str[30];
	*this << sprintf(str, "%d", i);
	return *this;
}

Serial& Serial::operator<<(const float f)
{
	char str[30];
	*this << sprintf(str, "%f", f);
	return *this;
}

Serial& Serial::operator<<(const double d)
{
	char str[30];
	*this << sprintf(str, "%lf", d);
	return *this;
}
