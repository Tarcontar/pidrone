#include "serial.h"
#include "usart.h"
#include <stdlib.h>
#include <stdio.h>

const char* Serial::endl = "\r\n";

Serial ser;

Serial::Serial()
{
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
	sprintf(str, "%d", i);
	*this << str;
	return *this;
}

Serial& Serial::operator<<(const float f)
{
	char str[30];
	int num = f;
	int komma = (f-num) * 100;
	sprintf(str, "%d.%d", num, komma);
	*this << str;
	return *this;
}

Serial& Serial::operator<<(const double d)
{
	*this << (float)d;
	return *this;
}
