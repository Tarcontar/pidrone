#include "serial.h"

#include "usart.h"

bool Serial::m_ready = false;

Serial::Serial(int baud)
{
	if (!m_ready)
	{
		m_ready = true;
	}
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