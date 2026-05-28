//
// Created by osboxes on 27/05/26.
//

#ifndef LG_ALARM_CENTRAL_IPCZMQ_H
#define LG_ALARM_CENTRAL_IPCZMQ_H

#include "zmq.hpp"
#include <iostream>
#include <thread>
#include <chrono>

#include "IPC.h"

class IPCZmqSender : public CoreLib::IPC::IPCPublisher {
private:
public:
    void test() override;
    bool send(const std::vector<uint8_t>& data) const override;
    void sendToTopic(std::string topic, std::unique_ptr<CoreLib::IPC::IPCMessage>& message) override{}
    void sendToTopic(std::string topic, std::vector<uint8_t> data) override{}
};

class IPCZmqReader : public CoreLib::IPC::IPCSubscriber {
private:
public:
    explicit IPCZmqReader(CoreLib::IPC::RequestHandler requestHandler, std::shared_ptr<CoreLib::IPC::IPCProtocol> protocol);
    void testRecv() override;
    void subscribe(std::string topic) override;
    int recv(std::vector<uint8_t>& data) override;
};


#endif //LG_ALARM_CENTRAL_IPCZMQ_H
