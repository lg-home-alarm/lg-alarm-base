#ifndef LG_ALARM_CENTRAL_TIME_H
#define LG_ALARM_CENTRAL_TIME_H

#include <chrono>
#include <time.h>

namespace CoreLib {
namespace CoreUtils {

class Time {
public:
    virtual void sleepUntil(std::chrono::steady_clock::time_point timepoint) = 0;
    static timespec toTimespec(std::chrono::steady_clock::time_point timepoint);
    virtual ~Time() = default;
};

}
}


#endif //LG_ALARM_CENTRAL_TIME_H
