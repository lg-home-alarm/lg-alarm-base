/* @file IPC.cpp
*  @brief
*  @author Lorenzo Giuliani
*  @date 26/05/26
*  @note
*/

#include "IPC.h"

namespace CoreLib {
namespace IPC {

IPCReqLambda::IPCReqLambda(std::function<void()> hnd) {
    this->hnd = hnd;
}

void IPCReqLambda::hndleRequest() {
    this->hnd();
}

void IPC::on(uint32_t request, IPCReq *reqhnd) {
    this->requests[request] = reqhnd;
}
}
}


