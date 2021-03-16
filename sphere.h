#pragma once
#include "hitable.h"

class sphere : public hitable
{
    public:
        material *mat_ptr;
        double radius;
        vec3 center;

        sphere() {}
        sphere(vec3  cen, double r, material* m):mat_ptr(m), radius(r), center(cen) {}
        virtual ~sphere(){}
        virtual bool hit(const ray& r, double tmin,  double tmax, hit_record& rec) const;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
    vec3 oc = r.origin() -  center;
    double a = dot(r.direction(), r.direction());
    double b = dot(oc, r.direction());
    double c = dot(oc, oc) - radius * radius;
    double discriminant = b * b - a * c;
    if (discriminant > 0)
    {
        double temp = (-b - sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.point_at_parametr(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.point_at_parametr(rec.t);
            rec.normal =  (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}
