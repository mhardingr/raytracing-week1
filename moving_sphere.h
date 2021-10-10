#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "aabb.h"
#include "rtweekend.h"
#include "hittable.h"

class moving_sphere : public hittable {
    public:
        moving_sphere() {}
        moving_sphere(
                point3 cen0, point3 cen1, double _time0, double _time1, double r, shared_ptr<material> m
                )
            : center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r), mat_ptr(m)
        {};

        virtual bool hit(
                const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double _time0, double _time1, aabb& output_box) const override;

        point3 center(double time) const;

    public:
        point3 center0, center1;
        double time0, time1;
        double radius;
        shared_ptr<material> mat_ptr;
};

point3 moving_sphere::center(double time) const {
    // Linearly interpolate center (between center1 and center0)
    // for time between time1 and time0
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool moving_sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    // If ray hits/tangents sphere, then return value (>= 0.0) is t of intersect
    // else, t < 0, ray never hits sphere
    // Note: removed factor of 2 in quadratic solution, using half_b
    point3 _center = center(r.time());
    vec3 oc = r.origin() - _center;
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
    vec3 outward_normal = (rec.p - _center) / radius; // Length normalized
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

bool moving_sphere::bounding_box(double _time0, double _time1, aabb& output_box) const {
    aabb box0(
            center(_time0) - vec3(radius, radius, radius),
            center(_time0) + vec3(radius, radius, radius));
    aabb box1(
            center(_time1) - vec3(radius, radius, radius),
            center(_time1) + vec3(radius, radius, radius));

    output_box = surrounding_box(box0, box1);
    return true;
}

#endif  // MOVING_SPHERE_H
