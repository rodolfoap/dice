#ifndef REALTIME_H
#define REALTIME_H

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class RealTimeClock
{
private:
    LARGE_INTEGER start, frequency;

public:
    RealTimeClock()
    {
        QueryPerformanceFrequency(&frequency);
	    QueryPerformanceCounter(&start);
    }
    void operator +=(int64_t usecs)
    {
        start.QuadPart += (frequency.QuadPart * usecs) / 1000000;
    }
    uint64_t get_usecs()
    {
        LARGE_INTEGER time;
	    QueryPerformanceCounter(&time);

        return 1000000 * (time.QuadPart - start.QuadPart) / frequency.QuadPart;
    }
};

#elif defined(__linux__)

#include <time.h>

class RealTimeClock
{
private:
    struct timespec start;

public:
    RealTimeClock()
    {
	    clock_gettime(CLOCK_REALTIME, &start);
    }
    void operator +=(int64_t usecs)
    {
        uint64_t time = start.tv_sec * 1000000000 + start.tv_nsec + usecs * 1000;
        start.tv_sec = time / 1000000000;
        start.tv_nsec = time % 1000000000;
    }
    uint64_t get_usecs()
    {
	    struct timespec time;
	    clock_gettime(CLOCK_REALTIME, &time);

        return (uint64_t(time.tv_sec - start.tv_sec) * 1000000 + 
               (time.tv_nsec - start.tv_nsec) / 1000);
    }
};

#elif defined(__APPLE__)

#include <mach/mach.h>
#include <mach/mach_time.h>

class RealTimeClock
{
private:
    mach_timebase_info_data_t timebase_info;
    uint64_t start;

public:
    RealTimeClock()
    {
        mach_timebase_info(&timebase_info);
	    start = mach_absolute_time();
    }
    void operator +=(int64_t usecs)
    {
        start += usecs * 1000 * timebase_info.denom / timebase_info.numer;
    }
    uint64_t get_usecs()
    {
        uint64_t time = mach_absolute_time();
	    
        return (time - start) * timebase_info.numer / (timebase_info.denom * 1000);
    }
};

#else
	#error "realtime.h not implemented for this platform"
#endif

#endif
