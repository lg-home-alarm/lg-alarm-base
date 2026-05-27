
#ifndef LG_ALARM_CENTRAL_IPC_H
#define LG_ALARM_CENTRAL_IPC_H

#include <chrono>
#include <map>
#include <string>
#include <functional>

namespace CoreLib {
namespace IPC {

class IPCReq {
private:
public:
    virtual void hndleRequest() = 0;
    virtual ~IPCReq() = default;
};

class IPCReqLambda : public IPCReq {
private:
    std::function<void()> hnd;
public:
    explicit IPCReqLambda(std::function<void()> hnd);
    void hndleRequest() override;
};

class IPC {
private:
    std::map<uint32_t, IPCReq*> requests;;
public:
    void on(uint32_t request, IPCReq* reqhnd);
};

}
}


#endif //LG_ALARM_CENTRAL_IPC_H
