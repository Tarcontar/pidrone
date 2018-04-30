#pragma once

class Serial
{
public:
	Serial(int baudrate);

	Serial& operator<<(char c);
	Serial& operator<<(const char* str);
	Serial& operator<<(int num);
	Serial& operator<<(float num);

	static const char* endl;

	inline int getBaudrate() const
	{
		return m_rate;
	}
private:
	 int m_rate;
};
