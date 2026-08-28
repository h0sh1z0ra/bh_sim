#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

using std::make_shared;
using std::shared_ptr;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility functions
inline double deg_to_rad(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    // Returns a random real in [0,1)
    // static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    // static std::mt19937 generator;
    // return distribution(generator);
    static thread_local std::mt19937 gen(std::random_device{}());
    static thread_local std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(gen);
}

inline double random_double(double min, double max) {
    // Return random real in [min, max)
    return min + (max-min)*random_double();
}

// Common headers
#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif