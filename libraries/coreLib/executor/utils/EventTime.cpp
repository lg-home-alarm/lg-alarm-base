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

EvtData EventTime::sleepUntil(timespec time) {
    itimerspec spec {};
    spec.it_value = time;
    spec.it_interval = {0};

    timerfd_settime(this->tfd, TFD_TIMER_ABSTIME, &spec, nullptr);

    return getEvtData(EPOLLIN);
}

EvtData EventTime::sleepUntil(std::chrono::steady_clock::time_point timepoint) {
    timespec _time = CoreUtils::Time::toTimespec(timepoint);
    return this->sleepUntil(_time);
}

EvtData EventTime::sleepUntil(std::chrono::system_clock::time_point timepoint) {
    timespec _time = CoreUtils::Time::toTimespec(timepoint);
    return this->sleepUntil(_time);
}

EvtData EventTime::getEvtData(uint32_t flags) {
    EvtData data(this->tfd, flags);
    return data;
}

EventTime::~EventTime() {
    close(tfd);
}
}
}
}
