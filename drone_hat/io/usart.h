#pragma once
#include <string>

class USART
{
public:
	void begin(int baud);
	
	void write(int c);
	
/*
	void print(std::string& str);
	
	template <class T>
	void print(T t);
	
	template <class T>
	void println(T t);
	
	inline void println() { print('\r'); print('\n'); }
*/
private:

};
