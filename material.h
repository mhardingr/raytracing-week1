#ifndef MATERIAL_H
#define MATERIAL_H
#include "rtweekend.h"

struct hit_record;  // alerts compiler that "hit_record" is a struct

class material {
    public:
        virtual bool scatter(
                const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
                ) const = 0;
};

class lambertian : public material {
    public:
        lambertian(const color& a) : albedo(a) {}

        virtual bool scatter(
                const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
                ) const override {
            // Matte reflections sampled according to true lambertian - cos(a) refl. distr.
            // which has more uniform distribution with high prob. closer to normal
            auto scatter_direction = rec.normal + random_unit_vector();

            // Catch degenerate (0-vec) scatter direction
            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }

            // Always scatter input array and attenuate by reflectance (albedo)
            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    public:
        color albedo;   // "whiteness" aka how well a surface reflects light rays
};

class metal : public material {
    public:
        metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(
                const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
                ) const override {
            vec3 refl_dir = reflect(r_in.direction(), rec.normal);
            scattered = ray(rec.p, refl_dir + fuzz*random_in_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);    // Is ray not absorbed?
        }
    public:
        color albedo;
        double fuzz;    // Radius of unit-sphere at tip of reflected array, aka random offset for reflected array
};

class dielectric : public material {
    public:
        dielectric(double index_of_refraction) : ir(index_of_refraction) {}

        virtual bool scatter(
                const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
                ) const override {
            // Always refracts
            attenuation = color(1.0, 1.0, 1.0);     // Glass surfaces never absorb
            double refraction_ratio = rec.front_face ? (1.0/ir) : ir;   // Assumes other other medium is air (eta = 1.0)

            vec3 unit_direction = unit_vector(r_in.direction());
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            vec3 direction;

            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
                // Reflecting (glass reflectance depends on incident angle - see Schlick's approx)
                direction = reflect(unit_direction, rec.normal);
            } else {
                direction = refract(unit_direction, rec.normal, refraction_ratio);
            }

            scattered = ray(rec.p, direction);
            return true;
        }

    public:
        double ir;  // Index of Refraction for this material

    private:
        static double reflectance(double cosine, double ref_idx) {
            // Use Schlick's approximation for reflectance.
            double r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1-cosine),5);
        }
};

#endif  // MATERIAL_H
