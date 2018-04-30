#include "serial.h"
#include "usart.h"

const char* Serial::endl = "\r\n";

Serial::Serial(int baud)
{
	m_rate = baud;
	USART::begin(baud);
}

Serial& Serial::operator<<(char c)
{
	this->putc(c);
	return *this;
}


Serial& Serial::operator<<(const char* str)
{
	char *it = const_cast<char *>(str);
	while (*it)
	{
		putc(*it);
		++it;
	}
	return *this;
}

void Serial::putc(char c) const
{
	USART::write(c);
}
