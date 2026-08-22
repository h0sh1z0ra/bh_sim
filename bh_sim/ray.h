#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    public:
        ray() {}

        ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

        const point3& origin() const { return orig; }
        const vec3& direction() const { return dir; }
        // ray.origin(), ray.direction()

        point3 at(double t) const {
            return orig + t*dir;
        }  //ray.at(2.5)
    
    private:
        point3 orig;
        vec3 dir;

        // ensures these are read only and CANNOT be modified, ever, from the outside
};

#endif