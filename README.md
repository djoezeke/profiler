# CProfiler
## Features:
* Lightweight instrumentation
* Multiple threads Support.
* Console output for logging text.
* Profiles itself.

## Basic Usage

```c
#define USE_START_STOP
#define USE_TIMEIT
#include "../profiler.h"

void Function()
{
    START();
    usleep(200);
    STOP();
}

void Function1()
{
    START();
    Function1();
    Function1();
    Function1();
    STOP();
}

void Function2()
{
    START();
    Function1();
    Function1();
    STOP();
}

int main()
{
    Function1();
    Function2();
    Function2();

    PROFILE();
    return 0;
}
```
