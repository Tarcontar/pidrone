#pragma once
#include "usart.h"

class Serial
{
public:
	Serial(int baudrate);

	void operator<<(char c) const;
	void operator<<(const char *str) const;

	void putc(char c) const;

private:
	USART *m_usart;
	static bool m_ready;
};
