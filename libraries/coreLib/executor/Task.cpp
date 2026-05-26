//
// Created by lgiuliani on 01/05/26.
//

#include "Task.h"
#include <iostream>

CoroutineStateHandler::CoroutineStateHandler(EvtData data, EventLoop& evtLoop) : StateHandler(data), _evtLoop(evtLoop) {
}

CoroutineStateHandler::CoroutineStateHandler(EventLoop &evtLoop) : _evtLoop(evtLoop) {
}

Task CoroutineStateHandler::suspend(EvtData evtData) {
    this->data = evtData;
    co_await Awaiter{this->_evtLoop, this};
}

void CoroutineStateHandler::start() {
    this->_task = this->mainFunction();
    this->_task.h.resume();
}

void CoroutineStateHandler::resume() {
    this->_task.h.resume();
}

CoroutineStateHandler::~CoroutineStateHandler() = default;

TaskStateHandler::TaskStateHandler(EvtData evtData, EventLoop &evtLoop) : StateHandler(evtData), _evtLoop(evtLoop) {
}

TaskStateHandler::TaskStateHandler(EventLoop &evtLoop) : _evtLoop(evtLoop) {
}

void TaskStateHandler::suspend() {
    _evtLoop.addStateHandler(this);
}

void TaskStateHandler::suspend(EvtData evtData) {
    this->data = evtData;
    this->suspend();
}

void TaskStateHandler::start() {
    this->mainFunction();
}

void TaskStateHandler::resume() {
    this->mainFunction();
}

TaskStateHandler::~TaskStateHandler() = default;
