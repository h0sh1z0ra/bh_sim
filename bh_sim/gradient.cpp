#include "color.h"
#include "vec3.h"

#include <iostream>   // for std::cout (output)

int main() {
    // Image
    const int w = 256;
    const int h = 256;

    // Render
    std::cout << "P3\n" << w << ' ' << h << "\n255\n";

    for (int y = 0; y < h; y++) {
        std::clog << "\nScanlines remaining: " << (h - y) << ' ' << std::flush;
        for (int x = 0; x < w; x++) {
            auto pixel_color = color(double(x)/(w-1),
                                     double(y)/(h-1),
                                     0);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\nDone.               \n";
}




 // for (int y = 0; y < h; y++) {
    //     // Progress bar
    //     std::clog << "\nScanlines remaining: " << (h - y) << ' ' << std::flush;

    //     for (int x = 0; x < w; x++) {
    //         // Writing pixels out in rows; l -> r; t -> b
    //         auto r = double(x) / (w-1);
    //         auto g = double(y) / (h-1);
    //         auto b = 0.25;

    //         auto ir = int(255.999 * r);
    //         auto ig = int(255.999 * g);
    //         auto ib = int(255.999 * b);

    //         std::cout << ir << ' ' << ig << ' ' << ib << "\n";
    //     }