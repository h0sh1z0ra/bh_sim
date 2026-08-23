#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable {
    public:
        sphere(const point3& centre, double radius) : centre(centre), radius(std::fmax(0, radius)) {}

        bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
            vec3 oc = centre - r.origin();
            auto a = r.direction().length_squared();
            auto h = dot(r.direction(), oc);
            auto c = oc.length_squared() - radius*radius;

            auto discriminant = h*h - a*c;
            if (discriminant < 0)
                return false;
            
                auto sqrtd = std::sqrt(discriminant);

                // Find nearest sqrt that lies in the acceptable range
                // [b +/- rt(b^2 - 4ac)]/2, so that's why + and -
                auto root = (h - sqrtd) / a;
                if (root <= ray_tmin || ray_tmax <= root) {
                    root = (h + sqrtd) / a;

                    if (root <= ray_tmin || ray_tmax <= root)
                        return false;
                }
                
                // Normal determination
                rec.t = root;
                rec.p = r.at(rec.t);
                vec3 outward_normal = (rec.p - centre) / radius;
                rec.set_face_normal(r, outward_normal);

                return true;
            }

    private:
        point3 centre;
        double radius;
};

#endif

// // Math to see if a ray hits a sphere
// double hit_sphere(const point3& centre, double radius, const ray& r) {
//     // Recall that the sphere's (centre C) equation, 
//     // with the ray equation P(t) = Q + td subbed in, is:
//     // (C - P) ⋅ (C - P) = r^2
//     // => (-td + (C-Q) ⋅ (-td + (C-Q))) = r^2
//     vec3 oc = centre - r.origin();

//     // Redefining as b = -2h, since v.v is just length squared
//     auto a = r.direction().length_squared();
//     auto h = dot(r.direction(), oc);
//     auto c = oc.length_squared() - radius*radius;
//     auto discriminant = h*h - a*c;

//     if (discriminant < 0) {
//         return -1.0;
//     } else {
//         return (h - std::sqrt(discriminant)) / a;
//     }
    
// }
// Above does this job as a header file