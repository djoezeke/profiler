# CProfiler

## Features:

- Lightweight instrumentation
- Multiple threads Support.
- Console output for logging text.
- Profiles itself.

## Basic Usage

```c
#include "../cprofiler.h"

void Function(Profiler *prof)
{
    START(prof);
    usleep(200);
    STOP(prof);
}

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
```
