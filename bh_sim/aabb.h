#ifndef AABB_H
#define AABB_H

#include "interval.h"
#include "ray.h"

#include <utility>


class aabb {
    public:
        interval x, y, z;   


        aabb() {}
        // [xmin, xmax], [ymin, ymax], [zmin, zmax] 
        // If types don't match interval
        aabb(const point3& corner1, const point3& corner2) {
            // Making corner input order irrelevant
            // Body
            x = interval(std::fmin(corner1[0], corner2[0]), std::fmax(corner1[0], corner2[0]));
            y = interval(std::fmin(corner1[1], corner2[1]), std::fmax(corner1[1], corner2[1]));
            z = interval(std::fmin(corner1[2], corner2[2]), std::fmax(corner1[2], corner2[2]));
        }
        
        // if members are intervals
        aabb(const interval& ix, const interval& iy, const interval& iz) : x(ix), y(iy), z(iz) {} 

        // merge
        aabb(const aabb& box1, const aabb& box2) {
            // return smallest box that contains both boxes
            x = interval(box1.x, box2.x);
            y = interval(box1.y, box2.y);
            z = interval(box1.z, box2.z);
        }

        // no & on ray_t since it's gonna be modified here
        bool hit(const ray& r, interval ray_t) const {
            const point3& r_origin = r.origin();
            const vec3&   r_direction = r.direction();
            
            for (int axis = 0; axis < 3; axis++) {
                const interval& ax = axis_interval(axis);
                auto denom = 1.0/r_direction[axis];

                // entry and exit of ray through slab on each axis
                // P(t) = Q + tD
                auto t_enter = (ax.min - r_origin[axis]) * denom;
                auto t_exit  = (ax.max - r_origin[axis]) * denom;
                
                // D
                if (denom < 0) 
                    std::swap(t_enter, t_exit);
                
                
                // Tighten t interval
                ray_t.min = std::fmax(ray_t.min, t_enter);
                ray_t.max = std::fmin(ray_t.max, t_exit);

                if (ray_t.max <= ray_t.min) return false;
            }
            return true;
            
        }

        const interval& axis_interval(int n) const {
            if (n == 1) return y;
            if (n == 2) return z;
            return x;
        }
};

#endif