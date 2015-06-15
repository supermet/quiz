#include "Timer.h"
Timer::Timer(TIMERPROC pr)
{
    interval=1000;
    proc=pr;
    timerid=0;
    enabled=false;
}

void Timer::start()
{
    if (!enabled)
    {
        timerid=SetTimer(0, 0, interval, proc);
        enabled=true;
    }
}

void Timer::stop()
{
    if (enabled)
    {
        KillTimer(0, timerid);
        enabled=false;
    }
}

Timer::~Timer()
{
    enabled=false;
    //dtor
}
