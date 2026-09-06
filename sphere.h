#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable {
    public:
        sphere(const point3& centre, double radius, shared_ptr<material> mat) : centre(centre), radius(std::fmax(0, radius)), mat(mat) {
            auto rvec = vec3(this->radius, this->radius, this->radius);
            bbox = aabb(centre - rvec, centre + rvec); // min corner, max corner
        } 
        aabb bounding_box() const override { return bbox; }  // needs to be here cuz the virtual for this bounding box is present in hittable

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            vec3 oc = centre - r.origin();
            auto a = r.direction().length_squared();
            auto h = dot(r.direction(), oc);
            auto c = oc.length_squared() - radius*radius;

            auto discriminant = h*h - a*c;
            if (discriminant < 0)
                return false;
            
                auto sqrtd = std::sqrt(discriminant);

                // Find nearest sqrt that lies in the acceptable range
                auto root = (h - sqrtd) / a;
                if (!ray_t.surrounds(root)) {
                    root = (h + sqrtd) / a;

                    if (!ray_t.surrounds(root))
                        return false;
                }
                
                // Normal determination
                rec.t = root;
                rec.p = r.at(rec.t);
                vec3 outward_normal = (rec.p - centre) / radius;
                rec.set_face_normal(r, outward_normal);
                rec.mat = mat;

                return true;
            }

    private:
        point3 centre;
        double radius;
        shared_ptr<material> mat;
        aabb bbox;
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