#include <stdio.h>
#include <unistd.h>

#include "../cprofiler.h"

void Function(Profiler &prof)
{
    START(prof);
    usleep(200);
    STOP(prof);
}

void Function1(Profiler &prof)
{
    START(prof);
    sleep(1);
    STOP(prof);
}

void Function2(Profiler &prof)
{
    START(prof);
    sleep(1);
    STOP(prof);
}

int main()
{
    Profiler prof = Profiler("Profiler");

    Function1(prof);
    Function2(prof);
    Function2(prof);

    Profiler proff = Profiler("Profiler");

    Function1(proff);
    Function2(proff);
    Function2(proff);

    prof.Profile();

    printf("\n");

    proff.Profile();

    return 0;
}