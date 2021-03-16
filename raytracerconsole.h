#ifndef RAYTRACERCONSOLE_H
#define RAYTRACERCONSOLE_H
#include <atomic>
#include <mutex>
#include <vector>
#include <chrono>

#include "camera.h"
#include "hitable.h"

static const int WIDTH = 1200;
static const int HEIGH = 800;
static const int ANTIALIASING = 50;
static const double APERTURE = 0.1;
static const vec3 LOOK_FROM(13.0, 2.0, 6.0);
static const vec3 LOOK_AT(0.0, 1.0, -0.5);


class RayTracerConsole
{
public:
    RayTracerConsole();
    void run(int threads = 1);
private:
    int amountOfPixels;
    int amountOfThreads;
    std::atomic<int> index;
    std::atomic<int> finishedThreads;
    std::vector<int> rLines;
    std::vector<int> gLines;
    std::vector<int> bLines;
    std::vector<int> pixelX;
    std::vector<int> pixelY;
    std::chrono::time_point<std::chrono::system_clock> startTime, endTime, lastTime;
    camera* cam;
    hitable *world;

    int nx{WIDTH}, ny{HEIGH}, ns{ANTIALIASING};
    double d_nx{WIDTH}, d_ny{HEIGH}, d_ns{ANTIALIASING};

    void calculate();
    void saveFile();
};

#endif // RAYTRACERCONSOLE_H
