#include <iostream>

#include "LibHw.h"
#include "EventLoop.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Spi spi;
    spi.init();
    EventLoop event_loop;
    return 0;
}