#ifndef SKY_H
#define SKY_H

#include <iostream>
#include <algorithm>

#include "stb_image.h"

unsigned char* sky_data = nullptr;
int sky_h = 0, sky_w = 0;

inline void load_sky(const char* path) {
    int channels;
    sky_data = stbi_load(path, &sky_w, &sky_h, &channels, 3);
    if (!sky_data) std::cerr << "Failed to load sky: " << path << "\n";
}

color sky_color(double th, double ph) {
    // /* Random generated "stars" */
    // int u = int(ph*500.0/pi);
    // int v = int(th*500.0/pi);

    // unsigned int hsh = u * 73856093u ^ v * 19349663u; // i have no clue what this is
    // hsh = (hsh ^ (hsh >> 13)) * 1274126177u;
    // double val = (hsh & 0xFFFFFF) / double(0xFFFFFF);

    // if (val > 0.97) {                    // ~0.3% of cells are stars
    //     double bright = (val - 0.997) / 0.003;
    //     return color(bright, bright, bright);
    // }
    // return color(0.02, 0.02, 0.05);       // near-black sky

    /* Milky way */
    // Clamp to prevent wrapping around at [-pi, pi]
    double u = std::fmod((ph + pi) / (2*pi), 1.0);
    if (u < 0) u += 1.0;
    int px = std::min(int(u * sky_w), sky_w - 1);

    double v = std::clamp(th / pi, 0.0, 1.0);
    int py = std::min(int(v * sky_h), sky_h - 1);

    int idx = (py * sky_w + px) * 3;
    return color(sky_data[idx]/255.0, sky_data[idx+1]/255.0, sky_data[idx+2]/255.0);
};

#endif