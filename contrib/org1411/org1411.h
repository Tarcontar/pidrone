#ifndef _ORG1411_H_
#define _ORG1411_H_

class Org1411 
{
public:
    bool begin(int pin);
	void read();
private:
	int m_pin;
};

#endif