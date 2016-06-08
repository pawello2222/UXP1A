//
// Created by Kacper Harasim on 04.06.2016.
//

#ifndef UXP1A_TIMEUTILITIES_H
#define UXP1A_TIMEUTILITIES_H


class TimeUtilities {

 public:
  static struct timespec addTimespecs (struct timespec a, struct timespec b, struct timespec& result);
  static void millisecondsToTimespec(struct timespec *ts, unsigned long ms);
  static struct timeval getCurrentTime();
  static long calculateDifferenceBetweenTimesInMilliseconds(struct timeval a, struct timeval b);
};


#endif //UXP1A_TIMEUTILITIES_H
