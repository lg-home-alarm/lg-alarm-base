
#ifndef LG_ALARM_CENTRAL_EVENTTIME_H
#define LG_ALARM_CENTRAL_EVENTTIME_H

#include "Time.h"

#include <chrono>

#include "EventLoop.h"

namespace CoreLib {
namespace Executor {
namespace Util {

class EventTime {
private:
    int tfd;

    EvtData sleepUntil(timespec time) const;
public:
    EventTime();
    EvtData sleepUntil(std::chrono::steady_clock::time_point timepoint) const;
    EvtData sleepUntil(std::chrono::system_clock::time_point timepoint) const;
    ~EventTime();
};

}
}
}




#endif //LG_ALARM_CENTRAL_EVENTTIME_H
