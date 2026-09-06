#ifndef HITTABLE_H
#define HITTABLE_H

// #include "ray.h"
// Using rtweekend.h this isn't needed

#include "aabb.h"

class material;

class hit_record {
    public:
        point3 p;
        vec3 normal;
        shared_ptr<material> mat;
        double t;
        bool front_face;

        void set_face_normal(const ray& r, const vec3& outward_normal) {
            // Sets hit record normal vector
            // NOTE: parameter `outward_normal` is assumed to be unit vector

            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }  
};

class hittable {
    public:
    // Everything hittable must have a hit function
    // Sphere can be inherited from this, since it's a virtual function
        virtual ~hittable() = default;
        // If this ~ isn't here, hittable is destroyed, along with the sphere 
        // if done so. But if not, the sphere would delete but not hittable
        // Like it remains after, which is a memory leak

        // Hits only count if t_min < t < t_max
        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;

        // for aabb
        virtual aabb bounding_box() const = 0;

};

#endif