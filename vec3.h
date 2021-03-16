#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

inline double drand() {
    return rand() / (RAND_MAX + 1.0);
}


//The following code is the fast inverse square root implementation from Quake III Arena
inline double q_rsqrt(double number)
{
    return sqrt(number);
    /*
    uint32_t i;
    double x2, y;
    const double threehalfs = 1.5;
    x2 = number * 0.5;
    y = number;
    i = *(long *)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(double *)&i;
    y = y * (threehalfs - (x2 * y * y));
    return y;
    */
}

class vec3
{
public:
    double e[3];

    vec3() {}
    vec3(double e0, double e1, double e2) { e[0] = e0; e[1] = e1; e[2] = e2 ; }
    inline double x() const { return e[0]; }
    inline double y() const { return e[1]; }
    inline double z() const { return e[2]; }
    inline double r() const { return e[0]; }
    inline double g() const { return e[1]; }
    inline double b() const { return e[2]; }

    inline const vec3& operator+() const { return *this; }
    inline vec3 operator-() const {  return vec3(-e[0], -e[1], -e[2]); }
    inline double operator[](int i) const { return e[i]; }
    inline double& operator[](int i) { return e[i]; }

    inline double length() const
    {
        return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    }

    inline double squared_length() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    inline void make_unit_vector()
    {
        double k = 1 / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
        e[0] *= k; e[1] *= k; e[2] *= k;
    }

    inline vec3& operator+=(const vec3 &v)
    {
        e[0] += v[0];
        e[1] += v[1];
        e[2] += v[2];
        return *this;
    }

    inline vec3& operator-=(const vec3 &v)
    {
        e[0] -= v[0];
        e[1] -= v[1];
        e[2] -= v[2];
        return *this;
    }

    inline vec3& operator*=(const vec3 &v)
    {
        e[0] *= v[0];
        e[1] *= v[1];
        e[2] *= v[2];
        return *this;
    }

    inline vec3& operator/=(const vec3 &v)
    {
        e[0] /= v[0];
        e[1] /= v[1];
        e[2] /= v[2];
        return *this;
    }

    inline vec3& operator*=(const double t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    inline vec3& operator/=(const double t)
    {
        double k = 1.0 / t;
        e[0] *= k;
        e[1] *= k;
        e[2] *= k;
        return *this;
    }

    inline void reset()
    {
        e[0] = 0.0;
        e[1] = 0.0;
        e[2] = 0.0;
    }

    inline void sqrt_of_vec3()
    {
        e[0] = sqrt(e[0]);
        e[1] = sqrt(e[1]);
        e[2] = sqrt(e[2]);
    }
};

inline std::istream& operator>>(std::istream &is, vec3 &t)
{
    is >> t.e[0] >> t.e[1] >> t.e[2];
    return is;
}

inline std::ostream& operator<<(std::ostream &os, const vec3 &t)
{
    os << t.e[0] << t.e[1] << t.e[2];
    return os;
}

inline vec3 operator+(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator/(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline vec3 operator*(double t, const vec3 &v)
{
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator/(const vec3 &v, double t)
{
    return vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

inline vec3 operator*(const vec3 &v,double t)
{
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline double dot(const vec3 &v1, const vec3 &v2)
{
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vec3 cross(const vec3  &v1, const vec3 &v2)
{
    return vec3((v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]), -(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0]), (v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
}

inline vec3 unit_vector(vec3 v)
{
    return v / v.length();
}
