#include "raytracerconsole.h"
#include <iostream>
#include <chrono>
#include <thread>

int main(int argc, char** argv)
{
    int threads = 1;
    if(argc >= 2) {
        threads = atoi(argv[1]);
    }
    std::cout << "Threads " << threads << std::endl;
    RayTracerConsole tracer;
    tracer.run(threads);

    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    return 0;
}
