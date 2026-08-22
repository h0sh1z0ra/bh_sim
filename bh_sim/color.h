#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

using color = vec3;

void write_color(std::ostream& out, const color& pixel_color) {
                // Write to the real console; a pointer requires derefencing
                // don't modify (const) nor copy (&) the color, just read
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Normalise to 255
    int rbyte = int(255.999 * r);   // prevents overflow
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    // Write out pixel color components
    out << rbyte << ' ' << gbyte
}

#endif