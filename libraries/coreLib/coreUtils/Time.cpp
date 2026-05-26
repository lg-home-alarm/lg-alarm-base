/* @file TIme.cpp
*  @brief
*  @author Lorenzo Giuliani
*  @date 25/05/26
*  @note
*/

#include "Time.h"

namespace CoreLib {
namespace CoreUtils {

timespec Time::toTimespec(std::chrono::steady_clock::time_point timepoint) {
    timespec _timespec{};

    auto d = timepoint.time_since_epoch();

    auto secs = duration_cast<std::chrono::seconds>(d);
    auto nsecs = duration_cast<std::chrono::nanoseconds>(d - secs);

    _timespec.tv_sec = secs.count();
    _timespec.tv_nsec = nsecs.count();

    return _timespec;
}

}
}