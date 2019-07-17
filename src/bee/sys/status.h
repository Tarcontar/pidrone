#pragma once

class Status
{
public:
	static void update();

private:
  static void initialize();

	static bool m_ready;
};
