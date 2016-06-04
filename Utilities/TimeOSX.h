//
// Created by Kacper Harasim on 04.06.2016.
//

#ifndef UXP1A_TIMEOSX_H
#define UXP1A_TIMEOSX_H


#ifdef __MACH__

#define CLOCK_REALTIME 0

#include <sys/time.h>
//clock_gettime is not implemented on OSX
int clock_gettime(int clockId, struct timespec* t) {
    struct timeval now;
    int rv = gettimeofday(&now, NULL);
    if (rv) return rv;
    t->tv_sec  = now.tv_sec;
    t->tv_nsec = now.tv_usec * 1000;
    return 0;
}
#endif

#endif //UXP1A_TIMEOSX_H
