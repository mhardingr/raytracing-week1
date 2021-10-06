#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"

class material;     // Alerts compiler that "material" is a class

struct hit_record {
    point3 p;
    vec3 normal;        // Always against the incident array
    shared_ptr<material> mat_ptr;
    double t;
    bool front_face;    // Remember whether ray is hitting outer side of surface

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        // outward_normal is always "outward" from surface
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif  // HITTABLE_H
