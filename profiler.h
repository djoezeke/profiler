//
// Basic instrumentation profiler by Sackey Ezekiel Etrue

// Usage:
//      include this header file somewhere in your code (eg. precompiled header), and then use like:
//
// You will probably want to macro-fy this, to switch on/off easily and use things like __FUNCSIG__ for the profile name.
//

#ifndef DJOEZEKE_PROFILER_H
#define DJOEZEKE_PROFILER_H

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define PROFILER_VERSION_MAJOR 0
#define PROFILER_VERSION_MINOR 1
#define PROFILER_VERSION_PATCH 0

// #define PROFILIER_DEBUG
// #define USE_TIMEIT
// #define USE_START_STOP

#define TABLE_THEME 0
#define PROFILE() profiler()
#define PROFWRITE(x) profiler_enable_write(x)

#define CHROME (1)
#define LOGGER (2)

#ifdef USE_TIMEIT
#define TNAME(x) timer_start(x);
#define TIMEIT() timer_start(__func__);
#else
#define TNAME(x)
#define TIMEIT()
#endif // USE_TIMEIT

#ifdef USE_START_STOP
#ifdef USE_TIMEIT
#define SNAME(x) ProfileTimer *timmer = TNAME(x)
#define START() ProfileTimer *timmer = TIMEIT()
#define STOP() timer_stop(timmer);
#else
#define START() ProfileTimer *timmer = timer_start(__func__);
#define SNAME(x) ProfileTimer *timmer = timer_start(x);
#define STOP() timer_stop(timmer);
#endif // USE_TIMEIT
#else
#define SNAME(x)
#define START()
#define STOP()
#endif // USE_START_STOP

#define MAX_LINE_LENGTH 1024
#define MAX_FIELD_LENGTH 256

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

static struct Profiler *s_Profiler = NULL;
static size_t s_SessionCapacity = 10;
static FILE *s_ProfilerFile = NULL;
static size_t s_ProfilerWrite = 0;

#pragma region STRUCTURES

typedef struct Timedata
{
    double m_Max;
    double m_Min;
    double m_Avg;
    double m_Sum;
} Timedata;

typedef struct Section
{
    char *m_Name;
    struct Timedata m_Timming;
    size_t m_ProfileCount;
} Section;

typedef struct ProfileTimer
{
    char *m_Name;
    clock_t m_Start;
    bool m_Stoped;
} ProfileTimer;

typedef struct ProfileResult
{
    char *m_Name;
    uint32_t m_ThreadID;
    clock_t m_Start;
    clock_t m_End;
} ProfileResult;

typedef struct Profiler
{
    char *m_Name;
    Section *m_Sessions;
    size_t m_SessionCount;
} Profiler;

#pragma endregion // STRUCTURES

#pragma region DECLARATIONS

void profiler_enable_write(int platform);
void profiler_write(char *name, long start, long end);
void profiler_write_header();
void profiler_write_footer();

int profiler_add_result(ProfileResult *result);
int profiler();

ProfileTimer *timer_start(const char *name);
int timer_stop(ProfileTimer *timmer);

Timedata new_timing_data(double max, double min, double avg, double sum);
int del_timing_data(Timedata *timmer);

ProfileResult create_result(char *name, clock_t start, clock_t end, uint32_t threadid);
int delete_result(ProfileResult *res);

Section new_session(char *name);
int end_session(Section *section);

void print_table_row(char *section, int count, double min, double max, double avg, double sum);
void print_table_header();
void print_table();

#pragma endregion // DECLARATIONS

#pragma region DEFINATIONS

void profiler_enable_write(int platform)
{
    if (s_ProfilerWrite == 0)
    {
        s_ProfilerWrite = platform;
        profiler_write_header();
    }
};

void profiler_write(char *name, long start, long end)
{
    if (s_ProfilerWrite == 0)
        return;

    if (s_ProfilerWrite == CHROME)
    {
        char json[100];
        s_ProfilerFile = fopen("Profiler.json", "a+");
        sprintf(json, "{\"cat\":\"function\",\"dur\" :%ld,\"name\": \"%s\",\"ph\":\"X\",\"pid\": 0,\"tid\":%f,\"ts\":%ld},", end - start, name, 1.0, start);
        fwrite(json, strlen(json), 1, s_ProfilerFile);
        fflush(s_ProfilerFile);
        fclose(s_ProfilerFile);
        return;
    }

    if (s_ProfilerWrite == LOGGER)
    {
        s_ProfilerFile = fopen("Logger.log", "a+");
        fclose(s_ProfilerFile);
        return;
    }
};

void profiler_write_header()
{
    if (s_ProfilerWrite == 0)
        return;

    if (s_ProfilerWrite == 1)
    {
        char *json = "{\"otherData\": {},\"traceEvents\":[";
        s_ProfilerFile = fopen("Profiler.json", "a+");
        fwrite(json, strlen(json), 1, s_ProfilerFile);
        fflush(s_ProfilerFile);

        fclose(s_ProfilerFile);
        return;
    }
};

void profiler_write_footer()
{
    if (s_ProfilerWrite == 0)
        return;

    if (s_ProfilerWrite == 1)
    {
        char *json = "]}";
        s_ProfilerFile = fopen("Profiler.json", "a+");
        fwrite(json, strlen(json), 1, s_ProfilerFile);
        fflush(s_ProfilerFile);
        fclose(s_ProfilerFile);
    }
};

Timedata new_timing_data(double max, double min, double avg, double sum)
{
    Timedata td;
    td.m_Max = max;
    td.m_Min = min;
    td.m_Avg = avg;
    td.m_Sum = sum;
    return td;
}

int del_timing_data(Timedata *timmer)
{
    if (timmer != NULL)
    {
        return 0;
    }
    else
    {
#ifdef PROFILIER_DEBUG
        printf("Error: null timmer data\n");
#endif // PROFILIER_DEBUG
        return 1;
    }
}

Section new_session(char *name)
{
    Section ses;
    ses.m_Name = strdup(name);
    ses.m_Timming = new_timing_data(0, 0, 0, 0);
    ses.m_ProfileCount = 0;
    return ses;
}

int end_session(Section *section)
{
    if (section != NULL)
    {
        free(section->m_Name);
        return del_timing_data(&section->m_Timming);
    }
    else
    {
#ifdef PROFILIER_DEBUG
        printf("Error: null section\n");
#endif // PROFILIER_DEBUG
        return 1;
    }
}

ProfileResult create_result(char *name, clock_t start, clock_t end, uint32_t threadid)
{
    ProfileResult res;
    res.m_Name = strdup(name);
    res.m_Start = start;
    res.m_End = end;
    res.m_ThreadID = threadid;
    return res;
}

int delete_result(ProfileResult *res)
{
    if (res != NULL)
    {
        free(res->m_Name);
        return 0;
    }
    else
    {
#ifdef PROFILIER_DEBUG
        printf("Error: null result\n");
#endif // PROFILIER_DEBUG
        return 1;
    }
}

ProfileTimer *timer_start(const char *name)
{
    ProfileTimer *timmer = (ProfileTimer *)malloc(sizeof(ProfileTimer));
    if (timmer == NULL)
    {
#ifdef PROFILIER_DEBUG
        printf("Error: failed to allocate memory for timer\n");
#endif // PROFILIER_DEBUG
        return NULL;
    }
    timmer->m_Stoped = false;
    timmer->m_Name = strdup(name);
    timmer->m_Start = clock();
    return timmer;
}

int timer_stop(ProfileTimer *timmer)
{
    if (timmer == NULL)
    {
#ifdef PROFILIER_DEBUG
        printf("Error: null timer\n");
#endif // PROFILIER_DEBUG
        return 1;
    }

    if (!timmer->m_Stoped)
    {
        clock_t end = clock();
        timmer->m_Stoped = true;

        ProfileResult res = create_result(timmer->m_Name, timmer->m_Start, end, 1);

        profiler_write(timmer->m_Name, timmer->m_Start, end);
        profiler_add_result(&res);

        delete_result(&res);
        free(timmer->m_Name);
        free(timmer);
    }
    return 0;
}

Profiler *GetProfiler()
{
    if (s_Profiler == NULL)
    {
        s_Profiler = (Profiler *)malloc(sizeof(Profiler));
        if (s_Profiler == NULL)
        {
#ifdef PROFILIER_DEBUG
            printf("Error: failed to allocate memory for profiler\n");
#endif // PROFILIER_DEBUG
            return NULL;
        }
        s_Profiler->m_Sessions = (Section *)malloc(sizeof(Section) * s_SessionCapacity);
        if (s_Profiler->m_Sessions == NULL)
        {
#ifdef PROFILIER_DEBUG
            printf("Error: failed to allocate memory for sessions\n");
#endif // PROFILIER_DEBUG
            free(s_Profiler);
            s_Profiler = NULL;
            return NULL;
        }
        s_Profiler->m_SessionCount = 0;
        s_Profiler->m_Name = strdup("Profiler");
    }

    return s_Profiler;
}

int profiler_add_result(ProfileResult *result)
{
    Profiler *prof = GetProfiler();

    if (result == NULL || prof == NULL)
        return 1;

    if (prof->m_SessionCount >= s_SessionCapacity)
    {
        s_SessionCapacity += 2;
        Section *new_sessions = (Section *)realloc(prof->m_Sessions, sizeof(Section) * s_SessionCapacity);
        if (new_sessions == NULL)
        {
#ifdef PROFILIER_DEBUG
            printf("Error: failed to reallocate memory for sessions\n");
#endif // PROFILIER_DEBUG
            return 1;
        }
        prof->m_Sessions = new_sessions;
    }

    for (size_t i = 0; i < prof->m_SessionCount; i++)
    {
        if (strcmp(result->m_Name, prof->m_Sessions[i].m_Name) == 0)
        {
            prof->m_Sessions[i].m_ProfileCount++;
            prof->m_Sessions[i].m_Timming.m_Sum += (double)(result->m_End - result->m_Start);

            if (prof->m_Sessions[i].m_Timming.m_Max < (result->m_End - result->m_Start))
                prof->m_Sessions[i].m_Timming.m_Max = (double)(result->m_End - result->m_Start);

            if (prof->m_Sessions[i].m_Timming.m_Min > (result->m_End - result->m_Start))
                prof->m_Sessions[i].m_Timming.m_Min = (double)(result->m_End - result->m_Start);

            prof->m_Sessions[i].m_Timming.m_Avg = prof->m_Sessions[i].m_Timming.m_Sum / prof->m_Sessions[i].m_ProfileCount;

            return 0;
        }
    }

    Section section = new_session(result->m_Name);
    section.m_ProfileCount++;
    section.m_Timming.m_Sum = (double)(result->m_End - result->m_Start);
    section.m_Timming.m_Max = (double)(result->m_End - result->m_Start);
    section.m_Timming.m_Min = (double)(result->m_End - result->m_Start);
    section.m_Timming.m_Avg = (double)(result->m_End - result->m_Start);

    prof->m_Sessions[prof->m_SessionCount] = section;
    prof->m_SessionCount++;
    return 0;
}

void print_table_row(char *section, int count, double min, double max, double avg, double sum)
{
    char row[1024];
    snprintf(row, 1024, TC "| " NC "%20s" TC "| " NC "%12.4f" TC " | " NC "%12.4f" TC " | " NC "%12.4f" TC " | " NC "%12.4f" TC " | " NC "  %6d" TC "  |" NC, section, min, max, avg, sum, count);
    printf("%s\n", row);
    printf(TC "-----------------------------------------------------------------------------------------------\n" NC);
}

void print_table_header()
{
    char header[1024];
    snprintf(header, 1024, TC "| " NC TH "%20s" NC TC "| " NC TH "%12s" NC TC " | " NC TH "%12s" NC TC " | " NC TH "%12s" NC TC " | " NC TH "%12s" NC TC " | " NC TH "%6s" NC TC " |" NC, "Section (Func Name) ", "Min Time(us)", "Max Time(us)", "Avg Time(us)", "Sum Time(us)", "No. Calls");

    printf(TC "-----------------------------------------------------------------------------------------------\n" NC);
    printf("%s\n", header);
    printf(TC "-----------------------------------------------------------------------------------------------\n" NC);
};

void print_table()
{
    print_table_header();

    for (size_t i = 0; i < s_Profiler->m_SessionCount; i++)
    {
        print_table_row(s_Profiler->m_Sessions[i].m_Name, s_Profiler->m_Sessions[i].m_ProfileCount, s_Profiler->m_Sessions[i].m_Timming.m_Min, s_Profiler->m_Sessions[i].m_Timming.m_Max, s_Profiler->m_Sessions[i].m_Timming.m_Avg, s_Profiler->m_Sessions[i].m_Timming.m_Sum);
    }
};

int profiler()
{
    if (s_Profiler == NULL)
    {
#ifdef PROFILIER_DEBUG
        printf("Error: profiler not initialized\n");
#endif // PROFILIER_DEBUG
        return 1;
    }

    print_table();
    profiler_write_footer();

    for (size_t i = 0; i < s_Profiler->m_SessionCount; i++)
    {
        end_session(&s_Profiler->m_Sessions[i]);
    }

    free(s_Profiler->m_Sessions);
    free(s_Profiler->m_Name);
    free(s_Profiler);
    s_Profiler = NULL;
    return 0;
}

#pragma endregion // DEFINATIONS

#endif // DJOEZEKE_PROFILER_H