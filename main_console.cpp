#include "raytracerconsole.h"
#include <iostream>
#include <chrono>
#include <thread>

int main(int argc, char** argv)
{
    std::cout << "Hello, World!" << std::endl;
    RayTracerConsole tracer;
    tracer.run(8);

    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    return 0;
}
