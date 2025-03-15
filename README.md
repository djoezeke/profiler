# CProfiler
## Features:
* Lightweight instrumentation of multiple threads running on the CPU and GPU.
* Web viewer that runs in Chrome, Firefox and Safari; on Desktops, Mobiles or Tablets.
* GPU UI rendering, bypassing the DOM completely, for real-time 60hz viewer updates at 10,000x the performance.
* Automatic thread sampler that tells you what processor cores your threads are running on without requiring Administrator privileges.
* Drop saved traces onto the Remotery window to load historical runs for inspection.
* Console output for logging text.
* Console input for sending commands to your game.
* A Property API for recording named/typed values over time, alongside samples.
* Profiles itself and shows how it's performing in the viewer.

## Basic Usage

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
}```
