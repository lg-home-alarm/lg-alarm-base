
#ifndef LG_ALARM_CENTRAL_IPC_H
#define LG_ALARM_CENTRAL_IPC_H

#include <chrono>
#include <map>
#include <string>
#include <functional>
#include <vector>
#include <memory>
#include <string>

namespace CoreLib {
namespace IPC {

class IPCMessage {
private:
    uint32_t messageType;
public:
    void setMessageType(uint32_t messageType);
    uint32_t getMessageType() const;
};

class IPCReq {
private:
public:
    virtual void hndleRequest(std::unique_ptr<IPCMessage>& message) = 0;
    virtual ~IPCReq() = default;
};

class IPCReqLambda : public IPCReq {
private:
    std::function<void(std::unique_ptr<IPCMessage>& message)> hnd;
public:
    explicit IPCReqLambda(std::function<void(std::unique_ptr<IPCMessage>& message)> hnd);
    void hndleRequest(std::unique_ptr<IPCMessage>& message) override;
};

class RequestHandler {
private:
    std::map<uint32_t, std::unique_ptr<IPCReq>> requests;
public:
    void on(uint32_t request, std::unique_ptr<IPCReq> reqhnd);
    void perform(uint32_t request, std::unique_ptr<IPCMessage>& message);
    RequestHandler(RequestHandler&&) = default;
    RequestHandler& operator=(RequestHandler&&) = default;
    RequestHandler(const RequestHandler&) = delete;
};

class IPCProtocol {
private:
public:
    virtual bool encodeMessage(std::unique_ptr<IPCMessage>& ipcmessage, std::vector<uint8_t>& data) = 0;
    virtual std::unique_ptr<IPCMessage> decodeMessage(std::vector<uint8_t>& data) = 0;
};

class IPCReader {
private:
    RequestHandler requestHandler;
    std::shared_ptr<IPCProtocol> protocol;
public:
    IPCReader(RequestHandler requestHandler, std::shared_ptr<IPCProtocol> protocol);
    virtual void testRecv();
    void receiveAndHandle(std::vector<uint8_t>& data);
    virtual int recv(std::vector<uint8_t>& data) = 0;
};

class IPCSender {
private:
    std::shared_ptr<IPCProtocol> protocol = nullptr;
public:
    IPCSender(std::shared_ptr<IPCProtocol> protocol);
    virtual void test();
    virtual bool send(std::unique_ptr<IPCMessage>& message);
    virtual bool send(const std::vector<uint8_t>& data) const = 0;
};

class IPCSubscriber : public IPCReader {
private:
public:
    explicit IPCSubscriber(RequestHandler requestHandler, std::shared_ptr<IPCProtocol> protocol);
    virtual void subscribe(std::string topic) = 0;
};

class IPCPublisher : public IPCSender {
private:
public:
    virtual void sendToTopic(std::string topic, std::unique_ptr<IPCMessage>& message) = 0;
    virtual void sendToTopic(std::string topic, std::vector<uint8_t> data) = 0;
};

}
}


#endif //LG_ALARM_CENTRAL_IPC_H
