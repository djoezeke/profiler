#include <stdio.h>
#include <unistd.h>

#include "../cprofiler.h"

void Function(Profiler &profiler)
{
    START(profiler);
    usleep(200);
    STOP(profiler);
}

void Function1(Profiler &profiler)
{
    START(profiler);
    sleep(1);
    STOP(profiler);
}

void Function2(Profiler &profiler)
{
    START(profiler);
    sleep(1);
    STOP(profiler);
}

int main()
{
    Profiler profiler = Profiler("Profiler");

    Function1(profiler);
    Function2(profiler);
    Function2(profiler);

    PROFILE(profiler);

    return 0;
}