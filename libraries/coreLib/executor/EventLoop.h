//
// Created by lgiuliani on 01/05/26.
//

#ifndef LG_ALARM_CENTRAL_EVENTLOOP_H
#define LG_ALARM_CENTRAL_EVENTLOOP_H

#include <coroutine>
#include <map>
#include <memory>

class EvtData {
private:
    int fd;
    uint32_t epollFlags;
public:
    EvtData(int fd, uint32_t epollFlags);
    EvtData();
    bool operator<(const EvtData & other) const;
    int getFd() const;
    uint32_t getEpollFlags() const;
};

class StateHandler {
protected:
    EvtData data;
public:
    virtual ~StateHandler() = default;

    StateHandler(const EvtData& data);
    StateHandler();
    const EvtData& getData() const;
    virtual void start() = 0;
    virtual void resume() = 0;
};

class EventLoop {
private:
    int epollFd;
    std::map<EvtData, StateHandler*> shMap;
public:
    EventLoop();
    void addStateHandler(StateHandler* stateHandler);
    void run();
};

#endif //LG_ALARM_CENTRAL_EVENTLOOP_H