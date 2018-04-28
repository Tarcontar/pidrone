#include "handler.h"
#include "status.h"

void sys_tick_handler(void)
{
  Status::update();
	Handler::run();
}

unsigned int Handler::m_next = 0;
std::array<std::function<void()>,16> Handler::m_tasks;

void Handler::queryTask(const std::function<void()>& task )
{
  m_tasks[m_next] = task;

  m_next++;

  if(m_next==m_tasks.size())
    m_next = 0;
}

void Handler::run()
{
  for(auto& task : m_tasks)
  {
    if(task!=nullptr)
    {
      task();
      task = nullptr;
    }
  }
}