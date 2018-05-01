#include "serial.h"
#include "usart.h"
#include <stdlib.h>
#include <stdio.h>

const char* Serial::endl = "\r\n";

//define the newlib syscall
int _write(int file, char *ptr, int len)
{
	int i;

	if (file == STDOUT_FILENO || file == STDERR_FILENO)
	{
		for (i = 0; i < len; i++)
		{
			if (ptr[i] == '\n')
			{
				USART::write('\r');
			}
			USART::write(ptr[i]);
		}
		return i;
	}
	errno = EIO;
	return -1;
}

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

