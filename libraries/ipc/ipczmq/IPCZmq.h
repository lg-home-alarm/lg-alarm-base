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

class IPCZmq : public CoreLib::IPC::IPC {
private:
public:
    void test() override{
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

    void testRecv() override {
        zmq::context_t ctx(1);
        zmq::socket_t sub(ctx, zmq::socket_type::sub);

        sub.connect("tcp://localhost:5556");

        // IMPORTANTISSIMO: senza subscribe non ricevi nulla
        sub.set(zmq::sockopt::subscribe, "");

        while (true) {
            zmq::message_t msg;

            sub.recv(msg);

            std::string data(
                static_cast<char*>(msg.data()),
                msg.size());

            std::cout << "received: " << data << '\n';
        }
    }
};


#endif //LG_ALARM_CENTRAL_IPCZMQ_H
