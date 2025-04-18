#include "../prof.h"
#include <assert.h>

void test_profiler_initialization()
{
    Profiler *prof = GetProfiler();
    assert(prof != NULL);
    assert(prof->m_SessionCount == 0);
    assert(prof->m_Sessions != NULL);
    assert(strcmp(prof->m_Name, "Profiler") == 0);
};

void test_profiler_add_result()
{
    ProfileResult result = create_result("TestSession", 100, 200, 1);
    int res = profiler_add_result(&result);
    assert(res == 0);

    Profiler *prof = GetProfiler();
    assert(prof->m_SessionCount == 1);
    assert(strcmp(prof->m_Sessions[0].m_Name, "TestSession") == 0);
    assert(prof->m_Sessions[0].m_ProfileCount == 1);
    assert(prof->m_Sessions[0].m_Timming.m_Sum == 100);
};

void test_profiler()
{
    profiler();
    assert(s_Profiler == NULL);
};

int main()
{
    test_profiler_initialization();
    test_profiler_add_result();
    test_profiler();
    printf("All tests passed!\n");
    return 0;
};