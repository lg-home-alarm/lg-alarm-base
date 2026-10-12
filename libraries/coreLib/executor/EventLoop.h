//
// Created by lgiuliani on 01/05/26.
//

#ifndef LG_ALARM_CENTRAL_EVENTLOOP_H
#define LG_ALARM_CENTRAL_EVENTLOOP_H

#include <coroutine>
#include <map>
#include <vector>
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

class LoopManageable {
public:
    virtual EvtData getEvtData(uint32_t flags) = 0;
    virtual ~LoopManageable() = default;
};

class EventLoop;

class StateHandler {
protected:
    EvtData data;
    EventLoop& _evtLoop;
public:
    StateHandler(const EvtData& data, EventLoop& evtLoop);
    StateHandler(EventLoop& evtLoop);
    const EvtData& getData() const;
    void scheduleTask();
    virtual void start() = 0;
    virtual void resume() = 0;
    virtual ~StateHandler() = default;
};

class EventLoop {
private:
    int epollFd;
    std::map<EvtData, StateHandler*> shMap;
    std::vector<StateHandler*> readyTasks;
public:
    EventLoop();
    void scheduleTask(StateHandler* stateHandler);
    void addStateHandler(StateHandler* stateHandler);
    void run();
};

#endif //LG_ALARM_CENTRAL_EVENTLOOP_H