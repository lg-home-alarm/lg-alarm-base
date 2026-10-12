//
// Created by lgiuliani on 01/05/26.
//

#include "Task.h"
#include <iostream>

CoroutineStateHandler::CoroutineStateHandler(EvtData data, EventLoop& evtLoop) : StateHandler(data, evtLoop) {
}

CoroutineStateHandler::CoroutineStateHandler(EventLoop &evtLoop) : StateHandler(evtLoop) {
}

Task CoroutineStateHandler::suspend(EvtData evtData) {
    this->data = evtData;
    co_await Awaiter{this->_evtLoop, this};
}

void CoroutineStateHandler::start() {
    this->_task = this->mainFunction();
    //this->_task.h.resume();
    this->scheduleTask();
}

void CoroutineStateHandler::resume() {
    this->_task.h.resume();
}

CoroutineStateHandler::~CoroutineStateHandler() = default;

TaskStateHandler::TaskStateHandler(EvtData evtData, EventLoop &evtLoop) : StateHandler(evtData, evtLoop) {
}

TaskStateHandler::TaskStateHandler(EventLoop &evtLoop) : StateHandler(evtLoop) {
}

void TaskStateHandler::suspend() {
    _evtLoop.addStateHandler(this);
}

void TaskStateHandler::suspend(EvtData evtData) {
    this->data = evtData;
    this->suspend();
}

void TaskStateHandler::start() {
    this->scheduleTask();
}

void TaskStateHandler::resume() {
    this->mainFunction();
}

TaskStateHandler::~TaskStateHandler() = default;
