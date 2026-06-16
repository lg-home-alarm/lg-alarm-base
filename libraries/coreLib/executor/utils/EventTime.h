
#ifndef LG_ALARM_CENTRAL_EVENTTIME_H
#define LG_ALARM_CENTRAL_EVENTTIME_H

#include "Time.h"

#include <chrono>

#include "EventLoop.h"

namespace CoreLib {
namespace Executor {
namespace Util {

class EventTime : public LoopManageable {
private:
    int tfd;

    EvtData sleepUntil(timespec time);
public:
    EventTime();
    EvtData sleepUntil(std::chrono::steady_clock::time_point timepoint);
    EvtData sleepUntil(std::chrono::system_clock::time_point timepoint);
    EvtData getEvtData(uint32_t flags) override;
    ~EventTime() override;
};

}
}
}




#endif //LG_ALARM_CENTRAL_EVENTTIME_H
