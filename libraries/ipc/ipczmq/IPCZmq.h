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
#include "EventLoop.h"

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

class Transport {
public:
    virtual std::string getEndpoint() = 0;
    virtual ~Transport() = default;
};

class TCPTransport : public Transport {
private:
    bool local = true;
    uint32_t port = 5599;
public:
    std::string getEndpoint() override;
};

class IPCTransport : public Transport {
private:
    std::string path = "/tmp/sockt";
public:
    std::string getEndpoint() override;
};

class IPCZmqReader {
protected:
    std::unique_ptr<Transport> transport;
    zmq::context_t ctx;
    zmq::socket_t _socket;
public:
    explicit IPCZmqReader(std::unique_ptr<Transport> transport, int _iothreads, zmq::socket_type _socketType);
    void connect();
    std::optional<size_t> recv(zmq::message_t& message);
    std::optional<size_t> send(zmq::message_t& message, zmq::send_flags flags);
    [[nodiscard]] zmq::socket_t& getSocket();
};

class IPCZmqRecvRepl : public CoreLib::IPC::IPCRecvRepl, public LoopManageable {
private:
    IPCZmqReader ipcReader;
    zmq::message_t identity;
public:
    explicit IPCZmqRecvRepl(CoreLib::IPC::RequestHandler&& requestHandler, std::shared_ptr<CoreLib::IPC::IPCProtocol> protocol);
    void connect() override;
    int recv(std::vector<uint8_t>& data) override;
    int reply(std::vector<uint8_t>& data) override;
    EvtData getEvtData(uint32_t flags) override;
};

class IPCZmqSubscriber : public CoreLib::IPC::IPCSubscriber, public LoopManageable {
private:
    IPCZmqReader ipcReader;
public:
    explicit IPCZmqSubscriber(CoreLib::IPC::RequestHandler&& requestHandler, std::shared_ptr<CoreLib::IPC::IPCProtocol> protocol);
    void connect() override;
    void subscribe(std::string topic) override;
    void unsubscribe(std::string topic) override;
    int recv(std::vector<uint8_t>& data) override;
    EvtData getEvtData(uint32_t flags) override;
    ~IPCZmqSubscriber() override;
};

}


#endif //LG_ALARM_CENTRAL_IPCZMQ_H
