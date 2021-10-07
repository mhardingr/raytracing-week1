#ifndef CAMERA_H
#define CAMERA_H
#include "rtweekend.h"

class camera {
    public:
        camera(
                point3 lookfrom,
                point3 lookat,
                vec3 vup,           // arb. vertical ray along camera's lookfrom-lookat axis
                double vfov,        // vertical field-of-view in degrees
                double aspect_ratio,
                double aperture,    // Size of hole, controls amount of light
                double focus_dist,  // Distance from _lens_ from which all rays are in perfect focus
                double _time0 = 0,
                double _time1 = 0
        ) {
            double theta = degrees_to_radians(vfov);
            double h = tan(theta/2);
            double viewport_height = 2.0 * h;
            double viewport_width = aspect_ratio * viewport_height;

            w = unit_vector(lookfrom - lookat);  // Point behind camera
            u = unit_vector(cross(vup, w));  // Perp. vector to vup and w, also perp. to v
            v = unit_vector(cross(w, u));    // Vertical vector to w

            origin = lookfrom;
            horizontal = focus_dist * viewport_width * u;   // TODO: why scale by focus_dist
            vertical = focus_dist * viewport_height * v;    // TODO: why scale by focus_dist
            lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist * w;

            lens_radius = aperture / 2;
            time0 = _time0;
            time1 = _time1;
        }

        ray get_ray(double s, double t) const {
            // Return a ray with random offset (offset lies on disk around w-axis)
            // this creates defocus blur according to thin-lens approximation
            vec3 rd = lens_radius * random_in_unit_disk();
            vec3 offset = u * rd.x() + v * rd.y();

            return ray(
                    origin + offset,
                    lower_left_corner + s*horizontal + t*vertical - origin - offset,
                    random_double(time0, time1) // enables motion blur
            );
        }

    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        double lens_radius;
        double time0, time1;    // shutter open/close times
};
#endif  // CAMERA_H
