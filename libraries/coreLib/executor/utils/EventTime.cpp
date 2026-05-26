/* @file EventTime.cpp
*  @brief
*  @author Lorenzo Giuliani
*  @date 25/05/26
*  @note
*/
#include "EventTime.h"

#include <sys/timerfd.h>
#include <time.h>
#include <sys/epoll.h>

namespace CoreLib {
namespace Executor {
namespace Util {

EventTime::EventTime() {
    this->tfd = timerfd_create(CLOCK_MONOTONIC, 0);
}

EvtData EventTime::sleepUntil(std::chrono::steady_clock::time_point timepoint) const {
    timespec _time = CoreUtils::Time::toTimespec(timepoint);

    itimerspec spec {};
    spec.it_value = _time;
    spec.it_interval = {0};

    timerfd_settime(this->tfd, TFD_TIMER_ABSTIME, &spec, nullptr);

    EvtData data(this->tfd, EPOLLIN);

    return data;
}

EventTime::~EventTime() {
    close(tfd);
}
}
}
}
