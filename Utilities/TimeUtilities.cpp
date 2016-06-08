//
// Created by Kacper Harasim on 04.06.2016.
//

#include <ctime>
#include <sys/time.h>
#include "TimeUtilities.h"


struct timespec TimeUtilities::addTimespecs(struct timespec time1, struct timespec time2, struct timespec& result)
{
/* Add the two times together. */

  result.tv_sec = time1.tv_sec + time2.tv_sec ;
  result.tv_nsec = time1.tv_nsec + time2.tv_nsec ;
  if (result.tv_nsec >= 1000000000L) {		/* Carry? */
    result.tv_sec++ ;  result.tv_nsec = result.tv_nsec - 1000000000L ;
  }
}

void TimeUtilities::millisecondsToTimespec(struct timespec *ts, unsigned long ms) {
  ts->tv_sec = ms / 1000;
  ts->tv_nsec = (ms % 1000) * 1000000;
}

struct timeval TimeUtilities::getCurrentTime()
{
  struct timeval currentTime;
  gettimeofday(&currentTime, NULL);
  return currentTime;
}

long TimeUtilities::calculateDifferenceBetweenTimesInMilliseconds(struct timeval a, struct timeval b)
{
    struct timeval difference;
    difference.tv_sec = a.tv_sec - b.tv_sec;
    difference.tv_usec = a.tv_usec - b.tv_usec;

    while(difference.tv_usec < 0)
    {
        difference.tv_usec += 1000000;
        difference.tv_sec -= 1;
    }

    return 1000LL*difference.tv_sec+(difference.tv_usec/1000LL);
}
