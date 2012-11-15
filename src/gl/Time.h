
#ifndef Time_h
#define Time_h

#include <sys/time.h>
#include <errno.h>
#ifdef __linux
#include <time.h>
#endif
#include <stdexcept>

class Time {
 public:
  static double currentSeconds();
  static void initialize();
  Time();
 private:
  Time(const Time&);
  Time& operator=(const Time&);

};

#endif
