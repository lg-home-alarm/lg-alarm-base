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

class IPCZmqReader : public CoreLib::IPC::IPCReader {
protected:
    std::unique_ptr<Transport> transport;
    zmq::context_t ctx;
    zmq::socket_t _socket;
public:
    explicit IPCZmqReader(int _iothreads, zmq::socket_type _socketType, CoreLib::IPC::RequestHandler&& requestHandler, std::shared_ptr<CoreLib::IPC::IPCProtocol> protocol);
    void testRecv(){}
    void connect() override;
    int recv(std::vector<uint8_t>& data) override;
};

class IPCZmqReaderBase : public CoreLib::IPC::IPCReaderBase {
private:
    zmq::socket_t& _socket;
public:
    IPCZmqReaderBase(zmq::socket_t& _socket);
    int reply(std::vector<uint8_t>& data) override;
};

class IPCZmqReaderImpl : public IPCZmqReader {
private:
    std::unique_ptr<CoreLib::IPC::IPCReaderBase> _readerbase;
public:
    explicit IPCZmqReaderImpl(CoreLib::IPC::RequestHandler&& requestHandler, std::shared_ptr<CoreLib::IPC::IPCProtocol> protocol);
};

class IPCZmqSubscriberBase : public CoreLib::IPC::IPCSubscriberBase {
private:
    zmq::socket_t& _socket;
public:
    IPCZmqSubscriberBase(zmq::socket_t& _socket);
    void subscribe(std::string topic) override;
    void unsubscribe(std::string topic) override;
};

class IPCZmqSubscriber : public IPCZmqReader {
private:
    std::unique_ptr<CoreLib::IPC::IPCSubscriberBase> _subscriberBase;
public:
    explicit IPCZmqSubscriber(CoreLib::IPC::RequestHandler&& requestHandler, std::shared_ptr<CoreLib::IPC::IPCProtocol> protocol);
    IPCZmqSubscriber(IPCZmqSubscriber& other) = delete;
    IPCZmqSubscriber(IPCZmqSubscriber&& other) = delete;
    IPCZmqSubscriber operator=(IPCZmqSubscriber& other) = delete;
    void testRecv() override;
    virtual ~IPCZmqSubscriber();
};

}


#endif //LG_ALARM_CENTRAL_IPCZMQ_H
