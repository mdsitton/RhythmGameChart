#include "timer.hpp"

typedef std::chrono::microseconds msec;

Timer::Timer()
:m_start(clk::now()), m_current(m_start), m_previous(m_start)
{

}

void Timer::reset()
{
    m_start = clk::now();
    m_current = m_start;
    m_previous = m_start;
}

double Timer::get_timer_time()
{
    return std::chrono::duration_cast<msec>(m_current - m_start).count() / 1000.0;
}

double Timer::tick()
{
    m_previous = std::move(m_current);
    m_current = clk::now();
    return std::chrono::duration_cast<msec>(m_current - m_previous).count() / 1000.0;
}