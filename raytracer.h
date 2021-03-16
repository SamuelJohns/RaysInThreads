#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QObject>
#include <QVector>
#include <QPoint>
#include <QString>
#include <atomic>

#include "camera.h"
#include "hitable.h"

static const int WIDTH = 1200;
static const int HEIGH = 800;
static const int ANTIALIASING = 50;
static const double APERTURE = 0.1;
static const vec3 LOOK_FROM(13.0, 2.0, 6.0);
static const vec3 LOOK_AT(0.0, 1.0, -0.5);

class RayTracer : public QObject
{
    Q_OBJECT
public:
    explicit RayTracer(QObject *parent = nullptr);

    void run(int threads = 1);
signals:
    void done();
    void updateProgress(int index);
private:
    int amountOfPixels;
    int amountOfThreads;
    std::atomic<int> index;
    std::atomic<int> finishedThreads;
    QVector<QString> rgbLines;
    QVector<QPoint> pixels;
    camera* cam;
    hitable *world;

    int nx{WIDTH}, ny{HEIGH}, ns{ANTIALIASING};
    double d_nx{WIDTH}, d_ny{HEIGH}, d_ns{ANTIALIASING};

    void calculate();
    void saveFile();
};

#endif // RAYTRACER_H
