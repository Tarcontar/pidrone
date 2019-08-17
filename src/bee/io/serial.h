#pragma once

class Serial
{
public:
	Serial();

	Serial& operator<<(const char c);
	Serial& operator<<(const char* str);
	Serial& operator<<(const int i);
	Serial& operator<<(const unsigned i);
	Serial& operator<<(const float f);
	Serial& operator<<(const double d);

	static const char* endl;

private:
};

extern Serial ser;
