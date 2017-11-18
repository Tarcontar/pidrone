#pragma once

class Status
{
public:
	Status(int ledPin);
	~Status();
	
	void Update();
	void SetError(int error);
private:
	int m_ledPin;
	int m_update_count;
}