#pragma once

class Serial
{
public:
	Serial(int baudrate);

	const Serial& operator<<(char c) const;
	const Serial& operator<<(const char *str) const;

	void putc(char c) const;

	inline int getBaudrate() const
	{
		return m_rate;
	}

	static const char endl = '\n';
private:
	 int m_rate;
};
