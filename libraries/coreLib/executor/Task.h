//
// Created by lgiuliani on 01/05/26.
//

#ifndef LG_ALARM_CENTRAL_TASK_H
#define LG_ALARM_CENTRAL_TASK_H

#include <coroutine>
#include <iostream>
#include <thread>
#include <memory>
#include "EventLoop.h"

struct Task {
    struct promise_type {
        Task get_return_object() {
            return Task{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };

    std::coroutine_handle<promise_type> h{};
    Task() = default;
    explicit Task(std::coroutine_handle<promise_type> hh) : h(hh) {}
    Task(Task&& o) noexcept : h(o.h) { o.h = {}; }
    Task(const Task&) = delete;
    Task& operator=(const Task& o) = default;
    ~Task() { if (h) h.destroy(); }
};

struct Awaiter {
    EventLoop& evtLoop;
    StateHandler* stateHandler;

    bool await_ready() {
        return false;
    }
    void await_suspend(std::coroutine_handle<> h) {
        evtLoop.addStateHandler(this->stateHandler);
    }
    void await_resume() {

    }
};

class CoroutineStateHandler : public StateHandler {
private:
    Task _task;
    EventLoop& _evtLoop;
protected:
    virtual Task mainFunction() = 0;
public:
    explicit CoroutineStateHandler(EvtData evtData, EventLoop& evtLoop);
    explicit CoroutineStateHandler(EventLoop& evtLoop);
    Task suspend(EvtData evtData);
    void start() override;
    void resume() override;
    ~CoroutineStateHandler() override;
};

class TaskStateHandler : public StateHandler {
private:
    EventLoop& _evtLoop;
protected:
    virtual void mainFunction() = 0;
public:
    explicit TaskStateHandler(EvtData evtData, EventLoop& evtLoop);
    explicit TaskStateHandler(EventLoop& evtLoop);
    void suspend();
    void suspend(EvtData evtData);
    void start() override;
    void resume() override;
    ~TaskStateHandler() override;
};


#endif //LG_ALARM_CENTRAL_TASK_H