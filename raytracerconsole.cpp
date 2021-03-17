#include "raytracerconsole.h"
#include <iostream>
#include <fstream>
#include <thread>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "float.h"
#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "sphere.h"
#include "material.h"
#include "hitable_list.h"
#include "camera.h"

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
    unsigned int n = 53;
    sphere* spheres = new sphere[n + 1];
    hitable **list = new hitable*[n + 1];
    for( unsigned int i = 0; i < n +1; ++i) {
        list[i] = &spheres[i];
    }

    spheres[0] = sphere(vec3(0.0, -1001.0, 0.0), 1000.0, new lambertian(vec3(0.5, 0.5, 0.5)));

    int i = 0;

    vec3 left {-5.0, 1.0, -1.4};
    vec3 right  {-5.0, 1.0, 1.4};
    vec3 moveForward {0.4, 0.0, 0.0};
    vec3 center {0.0, 1.0, 0.0};

    for (int j = 0; j < 25; ++j)
    {
        if ( j > 16) {  // diffuse
            spheres[i++] = sphere(left, 0.2, new dielectric(1.5));
            spheres[i++] = sphere(right, 0.2, new dielectric(1.5));
        } else if (j > 8) { // metal
            spheres[i++] = sphere(left, 0.2,new metal(vec3(0.7, 0.6, 0.5), 0.0));
            spheres[i++] = sphere(right, 0.2, new metal(vec3(0.7, 0.6, 0.5), 0.0));
        } else {  // glass
            spheres[i++] = sphere(left, 0.2, new lambertian(vec3(0.4, 0.2, 0.1)));
            spheres[i++] = sphere(right, 0.2, new lambertian(vec3(0.4, 0.2, 0.1)));
        }

        rotateOnAngle(left, center, 15.0);
        left += moveForward;
        rotateOnAngle(right, center, 15.0);
        right += moveForward;
    }

    spheres[i++] = sphere(vec3(0.0, 1.0, 0.0), 0.8, new dielectric(1.5));
    spheres[i++] = sphere(vec3(-4.0, 1.0, 0.0), 0.8, new lambertian(vec3(0.4, 0.2, 0.1)));
    spheres[i++] = sphere(vec3(4.0, 1.0, 0.0), 0.8, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hitable_list(list,  i);
}

vec3 color(const ray &r, hitable  *world,  int depth)
{
    hit_record rec;
    if (world->hit(r, 0.001, DBL_MAX, rec))
    {
        ray scattered;
        vec3 attenuation;
        if(depth < 25 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
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
RayTracerConsole::RayTracerConsole()
{

}

void RayTracerConsole::run(int threads)
{
    startTime = std::chrono::system_clock::now();
    lastTime = startTime;

    finishedThreads.store(1, std::memory_order_release);
    amountOfThreads = threads;
    world = dna();
    index.store(0);

    double dist_to_focus = ( LOOK_FROM - LOOK_AT).length();
    cam =  new camera(LOOK_FROM, LOOK_AT, vec3(0.0, 1.0, 0.0), 20.0, double(nx) / double(ny), APERTURE, dist_to_focus);

    rLines.resize(nx * ny);
    gLines.resize(nx * ny);
    bLines.resize(nx * ny);
    unsigned int pixelIndex = 0;

    pixelX.resize(nx * ny);
    pixelY.resize(nx * ny);
    for (int j = ny - 1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            pixelX[pixelIndex] = i;
            pixelY[pixelIndex] = j;
            ++pixelIndex;
        }
    }
    amountOfPixels = pixelX.size();
    for( int k = 0; k < amountOfThreads; ++k) {
        std::thread t(&RayTracerConsole::calculate, this);
        t.detach();
    }
}

void RayTracerConsole::calculate()
{
    srand(time(NULL));

    int i = index.fetch_add(1);
    ray r;
    vec3 col(0.0, 0.0, 0.0);
    double u,v;

    while( i < amountOfPixels) {
        if(i % 10000 == 0) {
            endTime = std::chrono::system_clock::now();
            long time = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - lastTime).count();
            lastTime = endTime;
            std::cout << (100 * i)/amountOfPixels << "\t" << time << " \n";
        }
        col.reset();
        for (int s = 0; s < ns; ++s)
        {
            //u = (pixelX[i] + drand()) / d_nx;
            //v = (pixelY[i] + drand()) / d_ny;

            u = pixelX[i] / d_nx;
            v = pixelY[i] / d_ny;

            r = cam->get_ray(u, v);
            col += color(r, world, 1);
        }
        col /= d_ns;
        col.sqrt_of_vec3();
        rLines[i] = int(255.99 * col[0]);
        gLines[i] = int(255.99 * col[1]);
        bLines[i] = int(255.99 * col[2]);
        i = index.fetch_add(1);
    }

    int finished = finishedThreads.fetch_add(1, std::memory_order_relaxed);
    if(finished == amountOfThreads) {

        endTime = std::chrono::system_clock::now();
        long time = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        std::cout << "DONE " << time << "\n";
        saveFile();
        std::cout << "SAVE FILE \n";
    }
}

void RayTracerConsole::saveFile()
{
    std::ofstream myfile;
    myfile.open("example.ppm");
    myfile << "P3\n" << nx << " " << ny << "\n255\n";

    for(int i = 0; i < amountOfPixels; ++i) {
        myfile << rLines[i] << " " << gLines[i] << " " << bLines[i] << "\n";
    }
    myfile.close();
}

