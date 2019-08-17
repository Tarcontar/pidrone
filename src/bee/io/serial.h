#pragma once
#include <stdint.h>

class Serial
{
public:
	Serial();

	//Serial& operator<<(const char c);
	Serial& operator<<(const char* str);
	Serial& operator<<(const int32_t i);
	Serial& operator<<(const uint32_t i);
	Serial& operator<<(const float f);
	Serial& operator<<(const double d);

	static const char* endl;

private:
};

extern Serial ser;
