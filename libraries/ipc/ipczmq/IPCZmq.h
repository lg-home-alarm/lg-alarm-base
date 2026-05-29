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

namespace IPCZmq {
enum TRANSPORT {
    TCP,
    IPC
};

class IPCZmqSender : public CoreLib::IPC::IPCPublisher {
private:
public:
    void test() override;
    bool send(const std::vector<uint8_t>& data) const override;
    void sendToTopic(std::string topic, std::unique_ptr<CoreLib::IPC::IPCMessage>& message) override{}
    void sendToTopic(std::string topic, std::vector<uint8_t> data) override{}
};

class Endpoint {
public:
    virtual std::string getEndpoint() = 0;
};

class TCPEndpoint : public Endpoint {
private:
    uint32_t port = 5599;
public:
    std::string getEndpoint();
};

class IPCEndpoint : public Endpoint {
private:
    std::string path = "/tmp/sockt";
public:
    std::string getEndpoint();
};

class IPCZmqReader : public CoreLib::IPC::IPCSubscriber {
private:
    TRANSPORT transport = TRANSPORT::IPC;
    uint32_t port = 5599;

    zmq::context_t ctx;
    zmq::socket_t subscriber;
public:
    explicit IPCZmqReader(CoreLib::IPC::RequestHandler requestHandler, std::shared_ptr<CoreLib::IPC::IPCProtocol> protocol);
    void testRecv() override;
    void subscribe(std::string topic) override;
    int recv(std::vector<uint8_t>& data) override;
};
}


#endif //LG_ALARM_CENTRAL_IPCZMQ_H
