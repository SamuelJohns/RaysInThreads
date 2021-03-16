#pragma once
#include <math.h>
#include  "ray.h"
#include <QObject>
#include <QDebug>

static int AMOUNT = 0;

inline vec3 random_inunit_disk()
{
    /*
    vec3 p;
    do
    {
        p = 2.0 * vec3(drand(), drand(), 0.0) - vec3(1.0, 1.0, 0.0);
    } while (dot(p, p) >= 1.0);
    */
    vec3 p(2.0 *  drand() - 1.0, 2.0 *  drand() - 1.0, 0.0);
    if ((p.squared_length() >= 1.0))
    {
        p *= (q_rsqrt(p.squared_length()) * drand());
    }
    return p;
}

class camera : public QObject
{
    Q_OBJECT
    public:
        explicit camera(QObject *parent = nullptr);

        double lens_radius;
        vec3 origin;
        vec3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;

        camera(vec3 lookfrom, vec3 lookat, vec3 vup, double vfov, double aspect, double aperture, double focus_dist)
        {
            lens_radius  =  aperture / 2.0;
            double theta = vfov * M_PI / 180.0;
            double half_height = tan(theta / 2);
            double half_width = aspect  * half_height;
            origin = lookfrom;
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);
            lower_left_corner = origin - half_width *focus_dist * u - half_height * focus_dist * v - focus_dist * w;
            horizontal = 2.0 * half_width * focus_dist * u;
            vertical = 2.0 * half_height * focus_dist * v;
        }

        ray get_ray(double s, double t)
        {
            vec3 rd = lens_radius * random_inunit_disk();
            vec3 offset = u * rd.x() + v * rd.y();
            return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
        }
};

