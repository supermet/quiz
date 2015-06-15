#ifndef TIMER_H
#define TIMER_H
#include "windows.h"

class Timer
{
public:
    Timer(TIMERPROC pr);
    void start();
    void stop();
    bool enabled;
    UINT interval;
    virtual ~Timer();
protected:
private:
    UINT timerid;
    TIMERPROC proc;
};

#endif // TIMER_H
