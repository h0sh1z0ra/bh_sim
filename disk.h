#ifndef DISK_H
#define DISK_H

#include "color.h"

inline double disk_r_inner = 6.0;   // innermost stable circular orbit for schwarzschild; i.e., matter spirals in beyond this
inline double disk_r_outer = 10.0;

inline color disk_color(double r) {
    double t = (r - disk_r_inner) / (disk_r_outer - disk_r_inner);
    // normalise position along the disk
    return (1-t)*color(1.0, 0.8, 0.4) + t*color(0.6, 0.2, 0.05);
}

inline double isco_radius(double spin, double M) {
    double a_star = spin/M, a2 = a_star*a_star;
    double Z1 = 1 + std::cbrt(1 - a2) * (std::cbrt(1 + a_star) + std::cbrt(1-a_star));
    double Z2 = std::sqrt(3*a2 + Z1*Z1);
    return M * (3 + Z2 - std::sqrt((3-Z1)*(3+Z1+2*Z2)));
}

#endif