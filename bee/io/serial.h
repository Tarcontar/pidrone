#pragma once

class Serial
{
public:
	Serial(int baudrate);

	Serial& operator<<(char c);
	Serial& operator<<(const char* str);

	void putc(char c) const;

	static const char* endl;

	inline int getBaudrate() const
	{
		return m_rate;
	}
private:
	 int m_rate;
};
