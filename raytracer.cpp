#include "raytracer.h"
#include <iostream>
#include <fstream>
#include <QDebug>
#include <QString>
#include <thread>

#include "float.h"
#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "sphere.h"
#include "material.h"
#include "hitable_list.h"
#include "camera.h"



RayTracer::RayTracer(QObject *parent) : QObject(parent)
{

}

vec3& rotateOnAngle(vec3& v, vec3& center, double angle)
{
    double rotate[2][2];
    rotate[0][0] = cos((angle * M_PI) / 180);
    rotate[0][1] = -sin((angle * M_PI) / 180);
    rotate[1][0] = sin((angle * M_PI) / 180);
    rotate[1][1] = cos((angle * M_PI) / 180);

    vec3 rotated = v - center;

    double y = rotated[1] * rotate[0][0] + rotated[2] * rotate[0][1];
    double z = rotated[1] * rotate[1][0] + rotated[2] * rotate[1][1];

    rotated[1] = y;
    rotated[2] = z;
    v = rotated + center;

    return v;
}

hitable* random_dna()
{
    unsigned int n = 500;
    hitable **list = new hitable*[n + 1];
    list[0] = new sphere(vec3(0.0, -1001.0, 0.0), 1000.0, new lambertian(vec3(0.5, 0.5, 0.5)));

    int i = 1;

    vec3 left {-5.0, 1.0, -1.4};
    vec3 right  {-5.0, 1.0, 1.4};
    vec3 moveForward {0.4, 0.0, 0.0};
    vec3 center {0.0, 1.0, 0.0};

    for (int j = 0; j < 25; ++j)
    {
        double choose_mat = drand();
                if (choose_mat < 0.8) {  // diffuse
                        list[i++] = new sphere(left, 0.2,  new lambertian(vec3(drand()*drand(), drand()*drand(), drand()*drand())));
            list[i++] = new sphere(right, 0.2, new lambertian(vec3(drand()*drand(), drand()*drand(), drand()*drand())));
        }
                else if (choose_mat < 0.95) { // metal
                        list[i++] = new sphere(left, 0.2,new metal(vec3(0.5*(1.0 + drand()), 0.5*(1.0 + drand()), 0.5*(1 + drand())), 0.5*drand()));
            list[i++] = new sphere(right, 0.2, new metal(vec3(0.5*(1 + drand()), 0.5*(1 + drand()), 0.5*(1 + drand())), 0.5*drand()));
        }
        else {  // glass
                        list[i++] = new sphere(left, 0.2, new dielectric(1.5));
            list[i++] = new sphere(right, 0.2, new dielectric(1.5));
        }

        rotateOnAngle(left, center, 15.0);
        left += moveForward;
        rotateOnAngle(right, center, 15.0);
        right += moveForward;
    }

    list[i++] = new sphere(vec3(0.0, 1.0, 0.0), 0.8, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4.0, 1.0, 0.0), 0.8, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4.0, 1.0, 0.0), 0.8, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hitable_list(list,  i);
}

hitable* dna()
{
    unsigned int n = 500;
    hitable **list = new hitable*[n + 1];
    list[0] = new sphere(vec3(0.0, -1001.0, 0.0), 1000.0, new lambertian(vec3(0.5, 0.5, 0.5)));

    int i = 1;

    vec3 left {-5.0, 1.0, -1.4};
    vec3 right  {-5.0, 1.0, 1.4};
    vec3 moveForward {0.4, 0.0, 0.0};
    vec3 center {0.0, 1.0, 0.0};

    for (int j = 0; j < 25; ++j)
    {
        if ( j > 16) {  // diffuse
            list[i++] = new sphere(left, 0.2, new dielectric(1.5));
            list[i++] = new sphere(right, 0.2, new dielectric(1.5));
        } else if (j > 8) { // metal
            list[i++] = new sphere(left, 0.2,new metal(vec3(0.7, 0.6, 0.5), 0.0));
            list[i++] = new sphere(right, 0.2, new metal(vec3(0.7, 0.6, 0.5), 0.0));
        } else {  // glass
            list[i++] = new sphere(left, 0.2, new lambertian(vec3(0.4, 0.2, 0.1)));
            list[i++] = new sphere(right, 0.2, new lambertian(vec3(0.4, 0.2, 0.1)));
        }

        rotateOnAngle(left, center, 15.0);
        left += moveForward;
        rotateOnAngle(right, center, 15.0);
        right += moveForward;
    }

    list[i++] = new sphere(vec3(0.0, 1.0, 0.0), 0.8, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4.0, 1.0, 0.0), 0.8, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4.0, 1.0, 0.0), 0.8, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hitable_list(list,  i);
}

vec3 color(const ray &r, hitable  *world,  int depth)
{
    hit_record rec;
    if (world->hit(r, 0.001, DBL_MAX, rec))
    {
        ray scattered;
        vec3 attenuation;
        if(depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth + 1);
        } else {
            return vec3(0.0, 0.0, 0.0);
        }
    }
    else
    {
        vec3 unit_direction = unit_vector(r.direction());
        double t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

void RayTracer::run(int threads)
{
    finishedThreads.store(1, std::memory_order_release);
    amountOfThreads = threads;
    world = dna();
    index.store(0);

    double dist_to_focus = ( LOOK_FROM - LOOK_AT).length();
    cam =  new camera(LOOK_FROM, LOOK_AT, vec3(0.0, 1.0, 0.0), 20.0, double(nx) / double(ny), APERTURE, dist_to_focus);

    rgbLines.resize(nx * ny);
    unsigned int pixelI = 0;

    pixels.resize(nx * ny);
    for (int j = ny - 1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            pixels[pixelI] = QPoint(i, j);
            ++pixelI;
        }
    }
    amountOfPixels = pixels.size();
    for( int k = 0; k < amountOfThreads; ++k) {
        std::thread t(&RayTracer::calculate, this);
        t.detach();
    }
}

void RayTracer::calculate()
{
    int i = index.fetch_add(1);
    while( i < amountOfPixels) {
        if(i % 10000 == 0) {
            emit updateProgress((100 * i)/amountOfPixels);
        }

        ray r;
        vec3 col(0.0, 0.0, 0.0);

        for (int s = 0; s < ns; ++s)
        {
            double u = (pixels[i].x() + drand()) / d_nx;
            double v = (pixels[i].y() + drand()) / d_ny;
            r = cam->get_ray(u, v);
            col += color(r, world, 1);
        }
        col /= d_ns;
        col.sqrt_of_vec3();
        int ir = int(255.99 * col[0]);
        int ig = int(255.99 * col[1]);
        int ib = int(255.99 * col[2]);
        rgbLines[i] = QString::number(ir) + " " + QString::number(ig) +" "+ QString::number(ib) + "\n";
        i = index.fetch_add(1);
    }

    int finished = finishedThreads.fetch_add(1, std::memory_order_relaxed);
    if(finished == amountOfThreads) {
        saveFile();
        qDebug() << "DONE";
        emit done();
    }
}

void RayTracer::saveFile()
{
    qDebug() << "SAVE FILE";
    std::ofstream myfile;
    myfile.open("example.ppm");
    myfile << "P3\n" << nx << " " << ny << "\n255\n";

    for(QString &item : rgbLines) {
        myfile << item.toStdString();
    }
    myfile.close();
}
