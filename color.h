#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

void write_color(std::ostream &out, color accum_pixel_color, int samples_per_pixel) {
    auto r = accum_pixel_color.x();
    auto g = accum_pixel_color.y();
    auto b = accum_pixel_color.z();

    // Divide the accum_pixel_color by the number of samples and gamma-correct (gamma=2.0)
    double scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif  // COLOR_H
