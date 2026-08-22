#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>   // for std::cout (output)

// Math to see if a ray hits a sphere
double hit_sphere(const point3& centre, double radius, const ray& r) {
    // Recall that the sphere's (centre C) equation, 
    // with the ray equation P(t) = Q + td subbed in, is:
    // (C - P) ⋅ (C - P) = r^2
    // => (-td + (C-Q) ⋅ (-td + (C-Q))) = r^2
    vec3 oc = centre - r.origin();
    auto a = dot(r.direction(), r.direction()); // for what t does the ray land on the sphere
    auto b = -2.0 * dot(r.direction(), oc);
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - std::sqrt(discriminant)) / (2.0*a);
    }
    
}

color ray_color(const ray& r) {
    // placing sphere at z = -1, which is 1 unit in front of the camera (inwards is -z)
    auto t = hit_sphere(point3(0,0,-1), 0.5, r);
    if (t > 0.0) {
        vec3 N = unit_vector(r.at(t) - vec3(0,0,-1)); // normalise
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0); 
    // (1-a)*startValue + a*endValue
    // a scales with height, so the higher up the bluer
}

int main() {

    // Image
    
    auto aspect_ratio = 16.0/9.0;
    int image_width = 400;

    // Calculate image height; ensure it's at least 1
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera

    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width)/image_height); // actual ratio
    auto camera_centre = point3(0,0,0);

    // Calculate the vectors across horizontal and down the vertical viewport edges
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate horizontal and vertical delta vectors from pixel to pixel
    // I think delta vectors are just the distance between pixels
    auto pixel_delta_u = viewport_u / image_width;    // length across / width
    auto pixel_delta_v = viewport_v / image_height;   // length upwawrds / height

    // Calculate location of upper left pixel (since we want to start from top to bottom)
    auto viewport_upper_left = camera_centre
                               - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int y = 0; y < image_height; y++) {
        std::clog << "\nScanlines remaining: " << (image_height - y) << ' ' << std::flush;
        for (int x = 0; x < image_width; x++) {

            // Computing position of current pixel
            auto pixel_centre = pixel00_loc + (x * pixel_delta_u) + (y * pixel_delta_v);
            // ray goes from camera, to this pixel
            auto ray_direction = pixel_centre - camera_centre; 
            // constructing the ray; origin at camera centre, through this pixel
            ray r(camera_centre, ray_direction); 
            // Asks "what colour does this ray see?"
            color pixel_color = ray_color(r);

            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\nDone.               \n";
}