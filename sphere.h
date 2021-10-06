#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
    public:
        sphere() {}
        sphere(point3 cen, double r, shared_ptr<material> m)
            : center(cen), radius(r), mat_ptr(m) {};

        virtual bool hit(
                const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 center;
        double radius;
        shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    // If ray hits/tangents sphere, then return value (>= 0.0) is t of intersect
    // else, t < 0, ray never hits sphere
    // Note: removed factor of 2 in quadratic solution, using half_b
    vec3 oc = r.origin() - center;
    double a = r.direction().length_squared();  // ==dot(r.dir, r.dir)
    double half_b = dot(oc, r.direction());
    double c = oc.length_squared() - radius*radius;
    double discriminant = half_b*half_b - a*c;
    if (discriminant < 0) {
        return false;
    }
    double sqrtd = sqrt(discriminant);

    // Find nearest root that lies in the acceptable range.
    double root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root) {
            // Both roots are out of range
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius; // Length normalized
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

#endif  // SPHERE_H
