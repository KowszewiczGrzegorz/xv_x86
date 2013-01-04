#ifndef __TIME_H
#define __TIME_H

#include <sys/time.h>

typedef unsigned long long timestamp_t;
static timestamp_t get_timestamp () {
    struct timeval now;
    gettimeofday (&now, NULL);
    return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

#endif /* __TIME_H */
