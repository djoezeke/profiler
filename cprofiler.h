/* For by both C and C++ compilers */

/** @file cprofiler.h
 *  This is a Standard C++ Library file.  You should @c \#include this file
 *  in your programs, for all compilers C or C++.
 *
 *  This is the C/C++ Stack Call Profiler Library header @c profiler.h ,
 *
 */

#ifndef PROFILER_H
#define PROFILER_H

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// #define PROFILIER_DEBUG
#define NC "\e[0m" // No Color

#if TABLE_THEME == (1)
#define TH "\e[0;32m"
#define TC "\e[0;30m"
#elif TABLE_THEME == (2)
#define TH "\e[0;33m"
#define TC "\e[0;30m"
#elif TABLE_THEME == (3)
#define TH "\e[0;34m"
#define TC "\e[0;30m"
#elif TABLE_THEME == (4)
#define TH "\e[0;35m"
#define TC "\e[0;30m"
#elif TABLE_THEME == (5)
#define TH "\e[0;32m"
#define TC "\e[0;31m"
#elif TABLE_THEME == (6)
#define TH "\e[0;33m"
#define TC "\e[0;31m"
#elif TABLE_THEME == (7)
#define TH "\e[0;34m"
#define TC "\e[0;31m"
#elif TABLE_THEME == (8)
#define TH "\e[0;35m"
#define TC "\e[0;31m"
#elif TABLE_THEME == (9)
#define TH "\e[0;36m"
#define TC "\e[0;31m"
#elif TABLE_THEME == (10)
#define TH "\e[0;33m"
#define TC "\e[0;34m"
#elif TABLE_THEME == (11)
#define TH "\e[0;35m"
#define TC "\e[0;34m"
#elif TABLE_THEME == (12)
#define TH "\e[0;36m"
#define TC "\e[0;34m"
#elif TABLE_THEME == (13)
#define TH "\e[0;30m"
#define TC "\e[0;35m"
#elif TABLE_THEME == (14)
#define TH "\e[0;32m"
#define TC "\e[0;35m"
#elif TABLE_THEME == (15)
#define TH "\e[0;33m"
#define TC "\e[0;35m"
#elif TABLE_THEME == (16)
#define TH "\e[0;34m"
#define TC "\e[0;35m"
#elif TABLE_THEME == (17)
#define TH "\e[0;36m"
#define TC "\e[0;35m"
#elif TABLE_THEME == (18)
#define TH "\e[0;32m"
#define TC "\e[0;36m"
#elif TABLE_THEME == (19)
#define TH "\e[0;33m"
#define TC "\e[0;36m"
#elif TABLE_THEME == (20)
#define TH "\e[0;34m"
#define TC "\e[0;36m"
#elif TABLE_THEME == (21)
#define TH "\e[0;35m"
#define TC "\e[0;36m"
#else
#define TH "\e[0;32m" // GREEN
#define TC "\e[0;34m" // PURPLE

#endif // TABLE_THEME

typedef struct ProfileTime_t
{
    double m_Max;
    double m_Min;
    double m_Avg;
    double m_Sum;

} ProfileTime_t;

typedef struct ProfileSection_t
{
    char *m_Name;
    struct ProfileTime_t m_Timming;
    size_t m_ProfileCount;

} ProfileSection_t;

typedef struct ProfileTimer_t
{
    char *m_Name;
    clock_t m_Start;
    bool m_Stoped;

} ProfileTimer_t;

typedef struct ProfileResult_t
{
    char *m_Name;
    uint32_t m_ThreadID;
    clock_t m_Start;
    clock_t m_End;

} ProfileResult_t;

typedef struct Profiler_t
{
    char *m_Name;
    ProfileSection_t *m_Sessions;
    size_t m_SessionCount;
    size_t m_SessionCapacity;

} Profiler_t;

#ifdef __cplusplus

#include <string>

#define START(profiler) profiler.Start(__func__)
#define STOP(profiler) profiler.Stop()
#define PROFILE(profiler) profiler.Profile()

class Profiler
{
public:
    Profiler(const char *name);
    Profiler(std::string name);

    void Start(std::string name);
    void Start(const char *name);
    void Stop();
    void Profile();

    ~Profiler();

private:
    Profiler_t *m_profiler;
    ProfileTimer_t *m_timmer;
};

#else // c

#define START(profiler) profiler->Start(profiler, __func__)
#define STOP(profiler) profiler->Stop(profiler)
#define PROFILE(profiler) profiler->Profile(profiler)

typedef struct Profiler Profiler;

struct Profiler
{
    Profiler_t *m_profiler;
    ProfileTimer_t *m_timmer;

    void (*Start)(Profiler *, const char *);
    void (*Stop)(Profiler *);
    void (*Profile)(Profiler *);
};

Profiler *newProfiler(const char *);

void Start(Profiler *, const char *);
void Stop(Profiler *);
void Profile(Profiler *);

void freeProfiler(Profiler *);

#endif // __cplusplus

#endif // PROFILER_H
