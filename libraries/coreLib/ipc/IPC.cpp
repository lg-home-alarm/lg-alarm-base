/* @file IPC.cpp
*  @brief
*  @author Lorenzo Giuliani
*  @date 26/05/26
*  @note
*/

#include "IPC.h"

namespace CoreLib {
namespace IPC {

IPCReqLambda::IPCReqLambda(std::function<void(std::unique_ptr<IPCMessage>& message)> hnd) {
    this->hnd = std::move(hnd);
}

void IPCReqLambda::hndleRequest(std::unique_ptr<IPCMessage>& message) {
    this->hnd(message);
}

void RequestHandler::on(uint32_t request, std::unique_ptr<IPCReq> reqhnd) {
    this->requests[request] = std::move(reqhnd);
}

void RequestHandler::perform(uint32_t request, std::unique_ptr<IPCMessage>& message) {
    if (this->requests.find(request) != this->requests.end()) {
        this->requests[request]->hndleRequest(message);
    }
}

void IPCMessage::setMessageType(uint32_t messageType) {
    this->messageType = messageType;
}

uint32_t IPCMessage::getMessageType() const {
    return this->messageType;
}

IPCReader::IPCReader(RequestHandler&& requestHandler, std::shared_ptr<IPCProtocol> protocol) : requestHandler(std::move(requestHandler)) {
    this->protocol = protocol;
}

void IPCReader::receiveAndHandle(std::vector<uint8_t> &data) {
    int n = this->recv(data);
    if (n > 0) {
        auto ipcmessage = this->protocol->decodeMessage(data);
        this->requestHandler.perform(ipcmessage->getMessageType(), ipcmessage);
    }
}

IPCRecvRepl::IPCRecvRepl(RequestHandler &&requestHandler, std::shared_ptr<IPCProtocol> protocol) : IPCReader(std::move(requestHandler), protocol) {
}

IPCSender::IPCSender(std::shared_ptr<IPCProtocol> protocol) : protocol(protocol) {

}

bool IPCSender::send(std::unique_ptr<IPCMessage> &message) {
    if (this->protocol == nullptr) {
        return false;
    }
    std::vector<uint8_t> data;
    if (!this->protocol->encodeMessage(message, data)) {
        return false;
    }
    return this->send(data);
}

IPCSubscriber::IPCSubscriber(RequestHandler&& requestHandler, std::shared_ptr<IPCProtocol> protocol) : IPCReader(std::move(requestHandler), protocol) {
}

std::shared_ptr<IPCReader> IPCReaderFactory::getIpcRecvResp(RequestHandler &&requestHandler,
                                    std::shared_ptr<IPCProtocol> protocol) {

}

std::shared_ptr<IPCSubscriber> IPCReaderFactory::getIpcSubscriber(RequestHandler &&requestHandler,
                                        std::shared_ptr<IPCProtocol> protocol) {

}
}
}


