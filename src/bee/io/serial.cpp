#include "serial.h"
#include "usart.h"
#include <stdlib.h>
#include <stdio.h>

const char* Serial::endl = "\r\n";

Serial ser;

Serial::Serial()
{
}

Serial& Serial::operator<<(const char* str)
{
    char *it = const_cast<char *>(str);
    while (*it)
    {
        USART::write(*it);
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

Serial& Serial::operator<<(const int32_t i)
{
    *this << (int)i;
    return *this;
}

Serial& Serial::operator<<(const uint32_t i)
{
    *this << (int)i;
    return *this;
}

Serial& Serial::operator<<(const float f)
{
    char str[30];
    int num = f;
    int komma = (f - num) * 100;
    if (komma < 0.0) komma *= -1.0;
    sprintf(str, "%d.%d", num, komma);
    *this << str;
    return *this;
}

Serial& Serial::operator<<(const double d)
{
    *this << (float)d;
    return *this;
}
