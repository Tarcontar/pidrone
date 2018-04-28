#pragma once 
#include <array>
#include <functional>

class Handler
{
  public:
  static void queryTask(const std::function<void()>& task);

  static void run();
  private:
  static std::array<std::function<void()>,16> m_tasks;
  static unsigned int m_next;
};