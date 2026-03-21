#include "time_manager.h"

void Time_manager::setRTC(time_t right_time) {
    struct timeval tv;
    tv.tv_sec = right_time;
    tv.tv_usec = 0;

    settimeofday(&tv, NULL);
}

time_t Time_manager::getRTC() {
    return time(NULL);
}

String Time_manager::getUTCString() {
    time_t now = time(NULL);
    struct tm *utcTime = gmtime(&now);

    char buf[25];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02dT%02d:%02d:%02dZ",
                utcTime->tm_year + 1900,
                utcTime->tm_mon + 1,
                utcTime->tm_mday,
                utcTime->tm_hour,
                utcTime->tm_min,
                utcTime->tm_sec);

    return String(buf);
}

int Time_manager::getMinsInDay() {
    time_t now = time(NULL);
    struct tm *utcTime = gmtime(&now);
    return utcTime->tm_hour * 60 + utcTime->tm_min;
}