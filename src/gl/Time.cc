
#include "Time.h"
#include <sys/time.h>
#include <errno.h>
#ifdef __linux
#include <time.h>
#endif
#include <stdexcept>

static struct timeval start_time;
static bool initialized = false;

Time::Time(){}

void
Time::initialize()
{
  initialized=true;
  if(gettimeofday(&start_time, 0) != 0)
    //throw std::runtime_error(std::string("gettimeofday failed: ")+strerror(errno));
    throw std::runtime_error(std::string("gettimeofday failed: "));
}

double Time::currentSeconds()
{
  if(!initialized)
    initialized = true;
  struct timeval now_time;
  if(gettimeofday(&now_time, 0) != 0)
    //throw std::runtime_error(std::string("gettimeofday failed: ")+strerror(errno));
    throw std::runtime_error(std::string("gettimeofday failed: "));
  return (now_time.tv_sec-start_time.tv_sec)+(now_time.tv_usec-start_time.tv_usec)*1.e-6;
}
