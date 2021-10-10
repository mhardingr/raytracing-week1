#ifndef AABB_H
#define AABB_H

#include "rtweekend.h"

class aabb {
    public:
        aabb() {}
        aabb(const point3& a, const point3& b) { minimum = a; maximum = b; }

        point3 min() const { return minimum; }
        point3 max() const { return maximum; }

        bool hit(const ray& r, double t_min, double t_max) const {
            for (int a = 0; a < 3; a++) {
                // Find t0 <= t1 such that t0,t1 are a-axis coordinates at which
                // ray intersects with aabb along a-axis
                // Note: t0, t1 equal and abs(t0) == abs(t1) == inf -> no intersect
                auto invD = 1.0f / r.direction()[a];
                auto t0 = (min()[a] - r.origin()[a]) * invD;
                auto t1 = (max()[a] - r.origin()[a]) * invD;
                if (invD < 0.0f) {
                    // When ray is pointing backwards for this axis,
                    // need to enforce that t0 < t1
                    std::swap(t0, t1);
                }

                // Keep track of the overlap interval of intersections from
                // previously processed axes
                t_min = t0 > t_min ? t0 : t_min;
                t_max = t1 < t_max ? t1 : t_max;
                if (t_max <= t_min) {
                    // If resulting overlap interval is not valid, no hit
                    return false;
                }
            }
            // Resulting overlap interval is valid
            return true;
        }

        point3 minimum; // One corner of AABB
        point3 maximum; // Other corner of AABB
};

aabb surrounding_box(aabb box0, aabb box1) {
    point3 small(fmin(box0.min().x(), box1.min().x()),
                 fmin(box0.min().y(), box1.min().y()),
                 fmin(box0.min().z(), box1.min().z()));

    point3 big(fmax(box0.max().x(), box1.max().x()),
               fmax(box0.max().y(), box1.max().y()),
               fmax(box0.max().z(), box1.max().z()));

    return aabb(small, big);
}

#endif  // AABB_H
