#pragma once

#include <chrono>
#include <utility>

typedef std::chrono::high_resolution_clock clk;

class Timer
{
public:
    Timer();
    void reset();
    double get_timer_time();
    double tick();

private:
    std::chrono::time_point<clk> m_start;
    std::chrono::time_point<clk> m_current;
    std::chrono::time_point<clk> m_previous;
};