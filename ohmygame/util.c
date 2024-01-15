#ifndef _UTILS_C
#define _UTILS_C
#include <time.h>



long long nanos() {
    struct timespec current_time;

    if (clock_gettime(CLOCK_REALTIME, &current_time) == 0) {
        long long nanoseconds = current_time.tv_sec * 1000000000LL + current_time.tv_nsec;
        return nanoseconds;
    } else {
        return -1;
    }
}
long long micros() {
    long long val=nanos();

    if(val>=0){
        return val/1000;
    } else {
        return -1;
    }
}

#endif