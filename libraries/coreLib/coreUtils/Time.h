#ifndef LG_ALARM_CENTRAL_TIME_H
#define LG_ALARM_CENTRAL_TIME_H

#include <chrono>
#include <time.h>

namespace CoreLib {
namespace CoreUtils {

class Time {
public:
    virtual void sleepUntil(std::chrono::steady_clock::time_point timepoint) = 0;
    template<typename Clock, typename Duration>
    static timespec toTimespec( const std::chrono::time_point<Clock, Duration>& timepoint) {
        timespec ts{};

        auto d = timepoint.time_since_epoch();
        auto secs = std::chrono::duration_cast<std::chrono::seconds>(d);
        auto nsecs = std::chrono::duration_cast<std::chrono::nanoseconds>(d - secs);

        ts.tv_sec = secs.count();
        ts.tv_nsec = nsecs.count();

        return ts;
    }
    virtual ~Time() = default;
};

}
}


#endif //LG_ALARM_CENTRAL_TIME_H
