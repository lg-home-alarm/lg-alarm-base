//
// Created by lgiuliani on 01/05/26.
//

#include "EventLoop.h"

#include <ranges>
#include <sys/epoll.h>

EvtData::EvtData(int fd, uint32_t epollFlags) : fd(fd), epollFlags(epollFlags) {
}

EvtData::EvtData() : fd(-1), epollFlags(0) {

}

bool EvtData::operator<(const EvtData &other) const {
    return (this->fd < other.fd);
}

int EvtData::getFd() const {
    return this->fd;
}

uint32_t EvtData::getEpollFlags() const {
    return this->epollFlags;
}

StateHandler::StateHandler(const EvtData& data, EventLoop& _evtLoop) : data(data), _evtLoop(_evtLoop) {
}

StateHandler::StateHandler(EventLoop& _evtLoop) : data({}), _evtLoop(_evtLoop) {
}

const EvtData& StateHandler::getData() const {
    return this->data;
}

void StateHandler::scheduleTask() {
    this->_evtLoop.scheduleTask(this);
}

EventLoop::EventLoop() {
    this->epollFd = epoll_create1(0);
}

void EventLoop::scheduleTask(StateHandler *stateHandler) {
    this->readyTasks.push_back(stateHandler);
}

void EventLoop::addStateHandler(StateHandler* stateHandler) {
    epoll_event ev{};
    ev.events = stateHandler->getData().getEpollFlags();
    ev.data.fd = stateHandler->getData().getFd();

    epoll_ctl(this->epollFd, EPOLL_CTL_ADD, ev.data.fd, &ev);

    this->shMap[stateHandler->getData()] = stateHandler;
}

void EventLoop::run() {
    for(auto* _readyTask : this->readyTasks) {
        _readyTask->resume();
    }

    while (true) {
        epoll_event events[10];
        int nevt = epoll_wait(this->epollFd, events, 10, -1);
        for (int i : std::views::iota(0, nevt)) {
            int evfd = events[i].data.fd;
            EvtData evtData(evfd, events[i].events);
            auto it = this->shMap.find(evtData);
            if (it == this->shMap.end()) {
                continue;
            }
            auto stateHandler = it->second;
            this->shMap.erase(it);
            epoll_ctl(this->epollFd, EPOLL_CTL_DEL, evfd, nullptr);

            stateHandler->resume();
        }
    }
}