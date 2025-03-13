#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define USE_START_STOP
#define USE_TIMEIT

#include "../profiler.h"

void Function()
{
    ProfileTimer *timmer = TIMEIT();
    usleep(200);
    timer_stop(timmer);
}

void Function1()
{
    START();
    sleep(1);
    STOP();
}

void Function2()
{
    ProfileTimer *timmer = timer_start(__func__);
    sleep(1);
    timer_stop(timmer);
}

int main()
{
    PROFWRITE(CHROME);

    Function1();
    Function2();
    Function2();

    PROFILE();
    return 0;
}