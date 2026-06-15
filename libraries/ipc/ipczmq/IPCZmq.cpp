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

IPCZmqReader::IPCZmqReader(std::unique_ptr<Transport> transport, int _iothreads, zmq::socket_type _socketType) : transport(std::move(transport)), ctx(_iothreads), _socket(ctx, _socketType) {
}

IPCZmqSubscriber::IPCZmqSubscriber(CoreLib::IPC::RequestHandler&& requestHandler,
                                   std::shared_ptr<CoreLib::IPC::IPCProtocol> protocol) : IPCSubscriber(std::move(requestHandler), protocol), ipcReader(std::move(std::make_unique<IPCTransport>()), 1, zmq::socket_type::sub) {
}

void IPCZmqSubscriber::subscribe(std::string topic) {
    zmq::socket_t& _socket = this->ipcReader.getSocket();
    _socket.set(zmq::sockopt::subscribe, topic);
}

void IPCZmqSubscriber::unsubscribe(std::string topic) {
    zmq::socket_t& _socket = this->ipcReader.getSocket();
    _socket.set(zmq::sockopt::unsubscribe, topic);
}

int IPCZmqSubscriber::recv(std::vector<uint8_t> &data) {
    zmq::message_t topic;
    std::optional<size_t> _topicRes = this->ipcReader.recv(topic);
    if (!_topicRes.has_value()) {
        return 0;
    }
    if (*_topicRes == 0) {
        return 0;
    }

    zmq::message_t msg;
    std::optional<size_t> _messageRes = this->ipcReader.recv(msg);
    if (!_messageRes.has_value()) {
        return 0;
    }
    if (*_messageRes == 0) {
        return 0;
    }

    data.resize(msg.size());
    auto* _start = static_cast<const uint8_t*>(msg.data());
    std::copy(_start, _start + msg.size(), data.begin());
    return static_cast<int>(data.size());
}

void IPCZmqReader::connect() {
    this->_socket.connect(this->transport->getEndpoint());
}

std::optional<size_t>  IPCZmqReader::recv(zmq::message_t& message) {
    return this->_socket.recv(message);
}

std::optional<size_t> IPCZmqReader::send(zmq::message_t& message, zmq::send_flags flags) {
    return this->_socket.send(message, flags);
}

zmq::socket_t & IPCZmqReader::getSocket() {
    return this->_socket;
}

IPCZmqRecvRepl::IPCZmqRecvRepl(CoreLib::IPC::RequestHandler&& requestHandler, std::shared_ptr<CoreLib::IPC::IPCProtocol> protocol) : CoreLib::IPC::IPCRecvRepl(std::move(requestHandler), protocol), ipcReader(std::move(std::make_unique<IPCTransport>()),1, zmq::socket_type::router) {

}

void IPCZmqRecvRepl::connect() {
    this->ipcReader.connect();
}

int IPCZmqRecvRepl::recv(std::vector<uint8_t> &data) {
    std::optional<size_t> _identityRes = this->ipcReader.recv(this->identity);
    if (!_identityRes.has_value()) {
        return 0;
    }
    if (*_identityRes == 0) {
        return 0;
    }

    zmq::message_t msg;
    std::optional<size_t> _messageRes = this->ipcReader.recv(msg);
    if (!_messageRes.has_value()) {
        return 0;
    }
    if (*_messageRes == 0) {
        return 0;
    }

    data.resize(msg.size());
    auto* _start = static_cast<const uint8_t*>(msg.data());
    std::copy(_start, _start + msg.size(), data.begin());
    return static_cast<int>(data.size());
}

int IPCZmqRecvRepl::reply(std::vector<uint8_t> &data) {
    auto _res = this->ipcReader.send(this->identity, zmq::send_flags::sndmore);
    if (!_res.has_value()) {
        return 0;
    }

    zmq::message_t msg(data.data(), data.size());
    _res = this->ipcReader.send(msg, zmq::send_flags::sndmore);
    if (!_res.has_value()) {
        return 0;
    }

    return static_cast<int>(*_res);
}

IPCZmqSubscriber::~IPCZmqSubscriber() = default;

/*void IPCZmqSubscriber::testRecv() {
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
}*/

}

std::shared_ptr<CoreLib::IPC::IPCRecvRepl> CoreLib::IPC::IPCReaderFactory::getIpcRecvResp(RequestHandler&& requestHandler, std::shared_ptr<IPCProtocol> protocol) {
    return std::make_shared<IPCZmq::IPCZmqRecvRepl>(std::move(requestHandler), protocol);
}
