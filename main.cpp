#include <iostream>

#include "IPC.h"
#include "libraries/coreLib/ipc/IPC.h"
#include "libraries/ipc/ipczmq/IPCZmq.h"
#include <string.h>

int main(int argc, char** argv) {
    CoreLib::IPC::RequestHandler _requestHandler;
    std::shared_ptr<CoreLib::IPC::IPCProtocol> protocol;
    IPCZmq::IPCZmqSubscriber s(std::move(_requestHandler), protocol);
    /* IPCZmq ipc;

    if (strcmp(argv[1], "1") == 0) {
        ipc.test();
    } else if (strcmp(argv[1], "2") == 0) {
        ipc.testRecv();
    }

    ipc.test();*/
    return 0;
}
