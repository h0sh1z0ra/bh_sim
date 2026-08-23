#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class camera {
    public:
        // Image
    
        double  aspect_ratio = 1.0;
        int     image_width  = 100;

        void render(const hittable& world) {
            initialize();

            // Render
            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for (int y = 0; y < image_height; y++) {
                std::clog << "\nScanlines remaining: " << (image_height - y) << ' ' << std::flush;
                for (int x = 0; x < image_width; x++) {

                    // Computing position of current pixel
                    auto pixel_centre = pixel00_loc + (x * pixel_delta_u) + (y * pixel_delta_v);
                    // ray goes from camera, to this pixel
                    auto ray_direction = pixel_centre - centre; 
                    // constructing the ray; origin at camera centre, through this pixel
                    ray r(centre, ray_direction); 

                    // Asks "what colour does this ray see?"
                    color pixel_color = ray_color(r, world);
                    write_color(std::cout, pixel_color);
                }
            }
                std::clog << "\nDone.               \n";
        }

    private:
        int     image_height;   // Rendered image height
        point3  centre;         // Camera centre
        point3  pixel00_loc;    // Location of pixel 0,0
        vec3    pixel_delta_u;  // Offset to pixel to the right
        vec3    pixel_delta_v;  // Offset to pixel to the left

        void initialize() {
            // Calculate image height; ensure it's at least 1
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            // Camera; viewport dimensions
            centre = point3(0,0,0);

            auto focal_length = 1.0;
            auto viewport_height = 2.0;
            auto viewport_width = viewport_height * (double(image_width)/image_height); // actual ratio

            // Calculate the vectors across horizontal and down the vertical viewport edges
            auto viewport_u = vec3(viewport_width, 0, 0);
            auto viewport_v = vec3(0, -viewport_height, 0);

            // Calculate horizontal and vertical delta vectors from pixel to pixel
            // I think delta vectors are just the distance between pixels
            pixel_delta_u = viewport_u / image_width;    // length across / width
            pixel_delta_v = viewport_v / image_height;   // length upwawrds / height

            // Calculate location of upper left pixel (since we want to start from top to bottom)
            auto viewport_upper_left = 
                centre - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        }

        color ray_color(const ray& r, const hittable& world) {
            hit_record rec;
            
            if (world.hit(r, interval(0, infinity), rec)) {
                return 0.5 * (rec.normal + color(1,1,1));
            }

            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5*(unit_direction.y() + 1.0);
            return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0); 
            // (1-a)*startValue + a*endValue
            // a scales with height, so the higher up the bluer
        }
};

#endif