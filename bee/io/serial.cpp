#include "serial.h"
#include "usart.h"

Serial::Serial(int baud)
{
	m_rate = baud;
	USART::begin(baud);
}

const Serial& Serial::operator<<(char c) const
{
	putc(c);
	return *this;
}

const Serial& Serial::operator<<(const char *str) const
{
	char *it = const_cast<char *>(str);
	while (it)
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