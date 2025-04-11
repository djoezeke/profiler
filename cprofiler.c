//
// Basic instrumentation profiler by Sackey Ezekiel Etrue

// Usage:
//      include this header file somewhere in your code (eg. precompiled header), and then use like:
//
// You will probably want to macro-fy this, to switch on/off easily and use things like __FUNCSIG__ for the profile name.
//

#include "cprofiler.h"

#pragma region FUNCTIONS

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    // void profiler_write(char *name, long start, long end)
    // {
    //     if (s_ProfilerWrite == 0)
    //         return;

    //     if (s_ProfilerWrite == CHROME)
    //     {
    //         char json[100];
    //         s_ProfilerFile = fopen("Profiler.json", "a+");
    //         sprintf(json, "{\"cat\":\"function\",\"dur\" :%ld,\"name\": \"%s\",\"ph\":\"X\",\"pid\": 0,\"tid\":%f,\"ts\":%ld},", end - start, name, 1.0, start);
    //         fwrite(json, strlen(json), 1, s_ProfilerFile);
    //         fflush(s_ProfilerFile);
    //         fclose(s_ProfilerFile);
    //         return;
    //     }

    //     if (s_ProfilerWrite == LOGGER)
    //     {
    //         s_ProfilerFile = fopen("Logger.log", "a+");
    //         fclose(s_ProfilerFile);
    //         return;
    //     }
    // };

    // void profiler_write_header()
    // {
    //     if (s_ProfilerWrite == 0)
    //         return;

    //     if (s_ProfilerWrite == 1)
    //     {
    //         char *json = "{\"otherData\": {},\"traceEvents\":[";
    //         s_ProfilerFile = fopen("Profiler.json", "a+");
    //         fwrite(json, strlen(json), 1, s_ProfilerFile);
    //         fflush(s_ProfilerFile);

    //         fclose(s_ProfilerFile);
    //         return;
    //     }
    // };

    // void profiler_write_footer()
    // {
    //     if (s_ProfilerWrite == 0)
    //         return;

    //     if (s_ProfilerWrite == 1)
    //     {
    //         char *json = "]}";
    //         s_ProfilerFile = fopen("Profiler.json", "a+");
    //         fwrite(json, strlen(json), 1, s_ProfilerFile);
    //         fflush(s_ProfilerFile);
    //         fclose(s_ProfilerFile);
    //     }
    // };

    Profiler_t *init_Profiler_t(const char *name);
    void free_profiler_t(Profiler_t *profiler);

    ProfileTime_t init_ProfileTime_t(double max, double min, double avg, double sum);
    void free_ProfileTime_t(ProfileTime_t *timmer);

    ProfileSection_t init_ProfileSection_t(char *name);
    void free_ProfileSection_t(ProfileSection_t *section);

    ProfileResult_t init_ProfileResult_t(char *name, clock_t start, clock_t end, uint32_t threadid);
    void free_ProfileResult_t(ProfileResult_t *res);

    void Profiler_add_result(Profiler_t *profiler, ProfileResult_t *result);

    void print_table_row(char *section, size_t count, double min, double max, double avg, double sum);
    void print_table_header();
    void print_table(Profiler_t *profiler);

    Profiler_t *init_Profiler_t(const char *name)
    {
        Profiler_t *profiler = (Profiler_t *)malloc(sizeof(Profiler_t));
        profiler->m_SessionCapacity = 5;

        if (profiler == NULL)
        {
#ifdef PROFILIER_DEBUG
            printf("Error: failed to allocate memory for profiler\n");
#endif // PROFILIER_DEBUG
            return NULL;
        }

        profiler->m_Sessions = (ProfileSection_t *)malloc(sizeof(ProfileSection_t) * profiler->m_SessionCapacity);

        if (profiler->m_Sessions == NULL)
        {
#ifdef PROFILIER_DEBUG
            printf("Error: failed to allocate memory for sessions\n");
#endif // PROFILIER_DEBUG

            free(profiler);
            return NULL;
        }

        profiler->m_SessionCount = 0;
        profiler->m_Name = strdup(name);

        return profiler;
    }

    void free_profiler_t(Profiler_t *profiler)
    {
        for (size_t i = 0; i < profiler->m_SessionCount; i++)
        {
            free_ProfileSection_t(&profiler->m_Sessions[i]);
        }

        free(profiler->m_Sessions);
        free(profiler->m_Name);
        free(profiler);
    }

    ProfileTime_t init_ProfileTime_t(double max, double min, double avg, double sum)
    {
        ProfileTime_t td;
        td.m_Max = max;
        td.m_Min = min;
        td.m_Avg = avg;
        td.m_Sum = sum;
        return td;
    }

    void free_ProfileTime_t(ProfileTime_t *timmer)
    {
        if (timmer != NULL)
        {
            return;
        }
        else
        {
#ifdef PROFILIER_DEBUG
            printf("Error: null timmer data\n");
#endif // PROFILIER_DEBUG
            return;
        }
    }

    ProfileSection_t init_ProfileSection_t(char *name)
    {
        ProfileSection_t ses;
        ses.m_Name = strdup(name);
        ses.m_Timming = init_ProfileTime_t(0, 0, 0, 0);
        ses.m_ProfileCount = 0;
        return ses;
    }

    void free_ProfileSection_t(ProfileSection_t *section)
    {
        if (section != NULL)
        {
            free(section->m_Name);
            free_ProfileTime_t(&section->m_Timming);
        }
        else
        {
#ifdef PROFILIER_DEBUG
            printf("Error: null section\n");
#endif // PROFILIER_DEBUG
        }
    }

    ProfileResult_t init_ProfileResult_t(char *name, clock_t start, clock_t end, uint32_t threadid)
    {
        ProfileResult_t res;
        res.m_Name = strdup(name);
        res.m_Start = start;
        res.m_End = end;
        res.m_ThreadID = threadid;
        return res;
    }

    void free_ProfileResult_t(ProfileResult_t *res)
    {
        if (res != NULL)
        {
            free(res->m_Name);
        }
        else
        {
#ifdef PROFILIER_DEBUG
            printf("Error: null result\n");
#endif // PROFILIER_DEBUG
        }
    }

    void Profiler_add_result(Profiler_t *profiler, ProfileResult_t *result)
    {
        if (result == NULL || profiler == NULL)
            return;

        if (profiler->m_SessionCount >= profiler->m_SessionCapacity)
        {
            profiler->m_SessionCapacity += 2;

            ProfileSection_t *new_sessions = (ProfileSection_t *)realloc(profiler->m_Sessions, sizeof(ProfileSection_t) * profiler->m_SessionCapacity);

            if (new_sessions == NULL)
            {
#ifdef PROFILIER_DEBUG
                printf("Error: failed to reallocate memory for sessions\n");
#endif // PROFILIER_DEBUG
                return;
            }

            profiler->m_Sessions = new_sessions;
        }

        for (size_t i = 0; i < profiler->m_SessionCount; i++)
        {
            if (strcmp(result->m_Name, profiler->m_Sessions[i].m_Name) == 0)
            {
                profiler->m_Sessions[i].m_ProfileCount++;
                profiler->m_Sessions[i].m_Timming.m_Sum += (double)(result->m_End - result->m_Start);

                if (profiler->m_Sessions[i].m_Timming.m_Max < (result->m_End - result->m_Start))
                    profiler->m_Sessions[i].m_Timming.m_Max = (double)(result->m_End - result->m_Start);

                if (profiler->m_Sessions[i].m_Timming.m_Min > (result->m_End - result->m_Start))
                    profiler->m_Sessions[i].m_Timming.m_Min = (double)(result->m_End - result->m_Start);

                profiler->m_Sessions[i].m_Timming.m_Avg = profiler->m_Sessions[i].m_Timming.m_Sum / profiler->m_Sessions[i].m_ProfileCount;

                return;
            }
        }

        ProfileSection_t section = init_ProfileSection_t(result->m_Name);
        section.m_ProfileCount++;
        section.m_Timming.m_Sum = (double)(result->m_End - result->m_Start);
        section.m_Timming.m_Max = (double)(result->m_End - result->m_Start);
        section.m_Timming.m_Min = (double)(result->m_End - result->m_Start);
        section.m_Timming.m_Avg = (double)(result->m_End - result->m_Start);

        profiler->m_Sessions[profiler->m_SessionCount] = section;
        profiler->m_SessionCount++;
    };

    void print_table_row(char *section, size_t count, double min, double max, double avg, double sum)
    {
        char row[2048];
        snprintf(row, 2048, TC "| " NC "%20s" TC "| " NC "%12.4f" TC " | " NC "%12.4f" TC " | " NC "%12.4f" TC " | " NC "%12.4f" TC " | " NC "  %6d" TC "  |" NC, section, min, max, avg, sum, count);
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

    void print_table(Profiler_t *profiler)
    {
        print_table_header();

        for (size_t i = 0; i < profiler->m_SessionCount; i++)
        {
            print_table_row(profiler->m_Sessions[i].m_Name, profiler->m_Sessions[i].m_ProfileCount, profiler->m_Sessions[i].m_Timming.m_Min, profiler->m_Sessions[i].m_Timming.m_Max, profiler->m_Sessions[i].m_Timming.m_Avg, profiler->m_Sessions[i].m_Timming.m_Sum);
        }
    };

#ifdef __cplusplus
}
#endif // __cplusplus

#pragma endregion // FUNCTIONS

#ifdef __cplusplus

#pragma region CPLUSPLUS

Profiler::Profiler(const char *name)
{
    m_profiler = init_Profiler_t(name);
    m_timmer = NULL;
};

Profiler::Profiler(std::string name)
{
    Profiler(name.c_str());
};

void Profiler::Start(std::string name)
{
    Start(name.c_str());
}

void Profiler::Start(const char *name)
{
    m_timmer = (ProfileTimer_t *)malloc(sizeof(ProfileTimer_t));

    if (m_timmer == NULL)
    {
#ifdef PROFILIER_DEBUG
        printf("Error: failed to allocate memory for timer\n");
#endif // PROFILIER_DEBUG
        return;
    }

    m_timmer->m_Stoped = false;
    m_timmer->m_Name = strdup(name);
    m_timmer->m_Start = clock();
};

void Profiler::Stop()
{
    if (m_timmer == NULL)
    {
#ifdef PROFILIER_DEBUG
        printf("Error: null timer\n");
#endif // PROFILIER_DEBUG
        return;
    }

    if (!m_timmer->m_Stoped)
    {
        clock_t end = clock();
        m_timmer->m_Stoped = true;

        ProfileResult_t res = init_ProfileResult_t(m_timmer->m_Name, m_timmer->m_Start, end, 1);
        Profiler_add_result(m_profiler, &res);

        free_ProfileResult_t(&res);
    }
};

void Profiler::Profile()
{
    if (m_profiler == NULL)
    {
#ifdef PROFILIER_DEBUG
        printf("Error: profiler not initialized\n");
#endif // PROFILIER_DEBUG
        return;
    }

    print_table(m_profiler);
};

Profiler::~Profiler()
{
    free_profiler_t(m_profiler);
    free(m_timmer->m_Name);
};

#pragma endregion // CPLUSPLUS

#else

#pragma region C

void Start(Profiler *profiler, const char *name)
{
    profiler->m_timmer = (ProfileTimer_t *)malloc(sizeof(ProfileTimer_t));

    if (profiler->m_timmer == NULL)
    {
#ifdef PROFILIER_DEBUG
        printf("Error: failed to allocate memory for timer\n");
#endif // PROFILIER_DEBUG
        return;
    }

    profiler->m_timmer->m_Stoped = false;
    profiler->m_timmer->m_Name = strdup(name);
    profiler->m_timmer->m_Start = clock();
}

void Stop(Profiler *profiler)
{
    if (profiler->m_timmer == NULL)
    {
#ifdef PROFILIER_DEBUG
        printf("Error: null timer\n");
#endif // PROFILIER_DEBUG
        return;
    }

    if (!profiler->m_timmer->m_Stoped)
    {
        clock_t end = clock();
        profiler->m_timmer->m_Stoped = true;

        ProfileResult_t res = init_ProfileResult_t(profiler->m_timmer->m_Name, profiler->m_timmer->m_Start, end, 1);
        Profiler_add_result(profiler->m_profiler, &res);

        free_ProfileResult_t(&res);
    }
}

void Profile(Profiler *profiler)
{

    if (profiler == NULL)
    {
#ifdef PROFILIER_DEBUG
        printf("Error: profiler not initialized\n");
#endif // PROFILIER_DEBUG
        return;
    }

    print_table(profiler->m_profiler);
};

Profiler *newProfiler(const char *name)
{
    Profiler *profiler = (Profiler *)malloc(sizeof(Profiler));

    profiler->m_profiler = init_Profiler_t(name);
    profiler->m_timmer = NULL;

    profiler->Start = Start;
    profiler->Stop = Stop;
    profiler->Profile = Profile;

    return profiler;
};

void freeProfiler(Profiler *profiler)
{
    free_profiler_t(profiler->m_profiler);
    free(profiler->m_timmer->m_Name);
    free(profiler);
};

#pragma endregion // C

#endif // __cplusplus
