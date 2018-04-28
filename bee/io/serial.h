#pragma once

class Serial
{
public:
	Serial(int baudrate);

	const Serial& operator<<(char c) const;
	const Serial& operator<<(const char *str) const;

	void putc(char c) const;

	static const char endl = '\n';
private:
	static bool m_ready;
};
