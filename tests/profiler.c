#include "../cprofiler.h"
#include <assert.h>
#include <unistd.h>

void Function1(Profiler *prof)
{
    START(prof);
    sleep(1);
    STOP(prof);
}

void Function2(Profiler *prof)
{
    START(prof);
    sleep(1);
    STOP(prof);
}

int main()
{
    Profiler *prof = newProfiler("Profiler");

    Function1(prof);
    Function2(prof);
    Function2(prof);

    prof->Profile(prof);

    freeProfiler(prof);
    return 0;
}
