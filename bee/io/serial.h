#pragma once

class Serial
{
public:
	Serial(int baudrate);

	Serial& operator<<(const char c);
	Serial& operator<<(const char* str);
	Serial& operator<<(const int i);
	Serial& operator<<(const float f);

	static const char* endl;

private:
};
