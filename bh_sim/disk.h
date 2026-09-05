#ifndef DISK_H
#define DISK_H

#include "color.h"

inline double disk_r_inner = 6.0;   // innermost stable circular orbit for schwarzschild; i.e., matter spirals in beyond this
inline double disk_r_outer = 20.0;

inline color disk_color(double r) {
    double t = (r - disk_r_inner) / (disk_r_outer - disk_r_inner);
    // normalise position along the disk
    return (1-t)*color(1.0, 0.8, 0.4) + t*color(0.6, 0.2, 0.05);
}

#endif