/* @file IPCZmq.cpp
*  @brief
*  @author Lorenzo Giuliani
*  @date 27/05/26
*  @note
*/
//
// Created by osboxes on 27/05/26.
//

#include "IPCZmq.h"

namespace IPCZmq {

void IPCZmqSender::test() {
    zmq::context_t ctx(1);
    zmq::socket_t pub(ctx, zmq::socket_type::pub);

    pub.bind("tcp://*:5556");

    int counter = 0;

    while (true) {
        std::string msg = "tick " + std::to_string(counter++);

        pub.send(zmq::buffer(msg), zmq::send_flags::none);

        std::cout << "sent: " << msg << '\n';

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

bool IPCZmqSender::send(const std::vector<uint8_t> &data) const {
    return true;
}

std::string TCPTransport::getEndpoint() {
    std::string endpoint = "tcp://:5556";
    endpoint.append(this->local?"localhost":"*");
    endpoint.append(":");
    endpoint.append(std::to_string(this->port));

    return endpoint;
}

std::string IPCTransport::getEndpoint() {
    return this->path;
}

IPCZmqReader::IPCZmqReader(int _iothreads, zmq::socket_type _socketType, CoreLib::IPC::RequestHandler &&requestHandler,
    std::shared_ptr<CoreLib::IPC::IPCProtocol> protocol) : IPCReader(std::move(requestHandler), protocol), ctx(_iothreads), _socket(ctx, _socketType) {
}

IPCZmqSubscriber::IPCZmqSubscriber(CoreLib::IPC::RequestHandler&& requestHandler,
                                   std::shared_ptr<CoreLib::IPC::IPCProtocol> protocol) : IPCZmqReader(1, zmq::socket_type::pub, std::move(requestHandler), protocol) {
    std::unique_ptr<Transport> _transport = std::make_unique<IPCTransport>();
    this->transport = std::move(_transport);
}

void IPCZmqSubscriber::subscribe(std::string topic) {
    this->_socket.set(zmq::sockopt::subscribe, topic);
}

void IPCZmqSubscriber::unsubscribe(std::string topic) {
    this->_socket.set(zmq::sockopt::unsubscribe, topic);
}

void IPCZmqReader::connect() {
    this->_socket.connect(this->transport->getEndpoint());
}

int IPCZmqReader::recv(std::vector<uint8_t> &data) {
    zmq::message_t msg;

    std::optional<size_t> _res = this->_socket.recv(msg);
    if (!_res.has_value()) {
        return 0;
    }
    if (*_res == 0) {
        return 0;
    }
    data.resize(msg.size());
    auto* _start = static_cast<const uint8_t*>(msg.data());
    std::copy(_start, _start + msg.size(), data.begin());
    return static_cast<int>(data.size());
}

IPCZmqReaderImpl::IPCZmqReaderImpl(CoreLib::IPC::RequestHandler &&requestHandler,
                            std::shared_ptr<CoreLib::IPC::IPCProtocol> protocol) : IPCZmqReader(1, zmq::socket_type::rep, std::move(requestHandler), protocol) {
}

int IPCZmqReaderImpl::reply(std::vector<uint8_t> &data) {
    zmq::message_t msg(data.data(), data.size());
    auto _res = this->_socket.send(msg, zmq::send_flags::none);
    if (!_res.has_value()) {
        return 0;
    }
    return static_cast<int>(*_res);
}

IPCZmqSubscriber::~IPCZmqSubscriber() = default;

void IPCZmqSubscriber::testRecv() {
    zmq::context_t ctx(1);
    zmq::socket_t sub(ctx, zmq::socket_type::sub);

    sub.connect("tcp://localhost:5556");

    // IMPORTANTISSIMO: senza subscribe non ricevi nulla
    sub.set(zmq::sockopt::subscribe, "");

    while (true) {
        zmq::message_t msg;

        auto _r = sub.recv(msg);
        if (!_r.has_value()) {
            continue;
        }

        std::string data(
            static_cast<char*>(msg.data()),
            msg.size());

        std::cout << "received: " << data << '\n';
    }
}

}