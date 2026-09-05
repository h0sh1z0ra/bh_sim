#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"

#include <algorithm>

// #include <iostream>
// Going to be using rtweekend.h common header, so this isn't needed anymore

using color = vec3;

// Correcting gamma
inline double linear_to_gamma(double linear_component) {
    if (linear_component > 0)
        return std::sqrt(linear_component);
    
        return 0;
}

void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Apply a linear to gamma transform for gamma 2
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Translate the [0,1] component values to the byte range [0,255].
    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

void write_color_fast(std::string& out, const color& pixel_color) {
    auto r = linear_to_gamma(pixel_color.x());
    auto g = linear_to_gamma(pixel_color.y());
    auto b = linear_to_gamma(pixel_color.z());

    static const interval intensity(0.000, 0.999);
    out.push_back(static_cast<char>(int(256 * intensity.clamp(r))));
    out.push_back(static_cast<char>(int(256 * intensity.clamp(g))));
    out.push_back(static_cast<char>(int(256 * intensity.clamp(b))));

    // char buf[32];
    // int len = snprintf(buf, sizeof(buf), "%d %d %d\n", rbyte, gbyte, bbyte);
    // out.append(buf, len);
}

// ACES filmic colour normalisation
inline double aces(double x) {
    return std::clamp((x*(2.51*x + 0.03)) / (x*(2.43*x + 0.59) + 0.14), 0.0, 1.0);
}

inline color tone_map(const color& c) {
    return color(aces(c.x()), aces(c.y()), aces(c.z()));
}

#endif