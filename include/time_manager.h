#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H
#include <time.h>
#include <sys/time.h>
#include <Arduino.h>

class Time_manager {
    public:
        static void setRTC(time_t right_time);
        static time_t getRTC();
        static String getUTCString();
        static int getMinsInDay();
};

#endif // TIME_MANAGER_H
