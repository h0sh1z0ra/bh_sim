#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"
#include "geostate.h"
#include "color.h"

#include <thread>
#include <chrono>
#include <fstream>

class camera {
    public:
        // Image
        double  aspect_ratio      = 1.0;
        int     image_width       = 100;
        int     samples_per_pixel = 10;
        int     max_depth         = 10;

        double vfov = 90;
        point3 lookfrom = point3(0,0,0);    // Where camera is
        point3 lookat   = point3(0,0,-1);   // What we're looking at
        vec3   vup      = vec3(0,1,0);      // Altitude

        double spin = 0.0;
        double M = 1.0;

        // void render(const hittable& world) {
        void render() {
            auto start_time = std::chrono::high_resolution_clock::now();

            initialize();

            // Redone render loop, spread over thread count
            // image.resize(image_width * image_height);

            // const int num_threads = 1;
            const int num_threads = std::thread::hardware_concurrency();
            int rows_per_thread = image_height / num_threads;

            thread_buffers.resize(num_threads);

            // Spawn threads
            std::vector<std::thread> threads;
            for (int t = 0; t < num_threads; t++) {
                // Chunking per thread
                int start = t * rows_per_thread;
                int end   = (t == num_threads - 1) ? image_height : start + rows_per_thread;   // if last thread, take image_height since that's the end of the image.
                
                // Create new thread, run camera.render_rows, on this camera, from start to end, on the scene "world"
                threads.push_back(
                    // std::thread(&camera::render_rows, this, t, start, end, std::ref(world))
                    std::thread(&camera::render_rows, this, t, start, end)
                );
            }
            
            // Wait for all threads to finish before constructing image
            auto t1 = std::chrono::high_resolution_clock::now();
            for (auto& th : threads) {
                th.join();
            }

            // Write the steps taken
            std::clog << "total steps: " << total_steps
                      << " per ray: " << total_steps / (long long)(image_width*image_height*samples_per_pixel)
                      << "\n" << " rays that hit step limit: " << maxxed_rays << "\n";
            
            // Write in P6 binary format (way faster than P3)
            auto t2 = std::chrono::high_resolution_clock::now();

            std::ofstream out("image.ppm", std::ios::binary);   // write out in binary format
            out << "P6\n" << image_width << ' ' << image_height << "\n255\n";

            // Write
            for (const auto& buf : thread_buffers)
                out.write(buf.data(), buf.size());  

            auto end_time = std::chrono::high_resolution_clock::now();

            std::clog << "\nRender Time: " << duration(start_time, end_time) << "ms; " << "Join time: " << 
                                              duration(t1, t2) << "ms; " << "Write time: " << duration(t2, end_time) << "ms\n";
        }


    private:
        int     image_height;           // Rendered image height
        double  pixel_samples_scale;    // Color scale factor for a sum of pixel samples
        point3  centre;                 // Camera centre
        point3  pixel00_loc;            // Location of pixel 0,0
        vec3    pixel_delta_u;          // Offset to pixel to the right
        vec3    pixel_delta_v;          // Offset to pixel to the left
        vec3    u, v, w;                // Camera frame basis vectors
        
        // std::vector<color> image;
        std::vector<std::string> thread_buffers;

        

        ray get_ray(int i, int j) const{
            // Construct a camera ray originating from the origin and directed at a randomly
            // sampled point around the pixle location

            auto offset = sample_square();   // returns small offset
            auto pixel_sample = pixel00_loc
                                + ((i + offset.x()) * pixel_delta_u)
                                + ((j + offset.y()) * pixel_delta_v);

            auto ray_origin = centre;
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        vec3 sample_square() const {
            // Returns vector to a random point in [-.5,-.5]-[+.5,+.5] unit square.
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        color ray_color(const ray& r, int depth, const hittable& world) {
            // If ray bounce limit is exceeded, no more light is gathered
            if (depth <= 0)
                return color(0,0,0);

            hit_record rec;
            
            // small number to prevent shadow acne
            // rays attempt to accurately calculate intersection point; but it's 
            // susceptible to floating point rounding errors
            if (world.hit(r, interval(0.001, infinity), rec)) {
                ray scattered;
                color attenuation;
                if (rec.mat->scatter(r, rec, attenuation, scattered))
                    return attenuation * ray_color(scattered, depth-1, world);
                
                return color(0,0,0);
            }

            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5*(unit_direction.y() + 1.0);
            return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0); 
            // (1-a)*startValue + a*endValue
            // a scales with height, so the higher up the bluer
        }
        
        

        // color trace_geodesic(const RayInit& ray, double a, double M) {
        //     GeoState s = ray.state;
        //     double E = ray.E, Lz = ray.Lz;

        //     double r_horizon = M + std::sqrt(M*M - a*a);  // Outer horizon
        //     double r_escape  = 1000.0;                    // "Far away"
        //     double h = 0.01;
        //     int max_steps = 100000;

        //     for (int i = 0; i < max_steps; i++) {
        //         s = rk4(s, h, E, Lz, a, M);
                
        //         // 1.01 avoids BL coordinates blowing up
        //         if (s.r < r_horizon * 1.01) return color(0,0,0);    // Fell into BH
        //         if (s.r > r_escape)         return color(1,1,1);   // Escaped
        //     }
        //     return color(0,0,0);
        // }

        double degrees_to_radians(const double& deg) {
            return deg * (pi/180);
        }
        
        // Render row by row and store in the buffer
        void render_rows(int thread_id, int start, int end) {
            auto startTime = std::chrono::high_resolution_clock::now();

            for (int y = start; y < end; y++) {
                for (int x = 0; x < image_width; x++) {
                    color pixel_color(0,0,0);

                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        // ray r = get_ray(x, y);
                        // pixel_color += ray_color(r, max_depth, world);
                        pixel_color += trace(x, y);
                    }
                    write_color_fast(thread_buffers[thread_id], pixel_samples_scale * pixel_color);
                    // image[y*image_width + x] = pixel_samples_scale * pixel_color;
                }
            }
            auto endTime = std::chrono::high_resolution_clock::now();
            auto total = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

            std::clog << "Thread Finished Rows " << start << " - " << end <<
                         "; finished at " << total << "ms\n";
        } 

                color trace(int px, int py) {
            /* Pixel to cartesian */
            auto offset = sample_square();
            auto pixel_sample = pixel00_loc 
                              + ((px + offset.x()) * pixel_delta_u) 
                              + ((py + offset.y()) * pixel_delta_v);
            vec3 dir = unit_vector(pixel_sample - centre);

            /* Cartesian cam position to BL coords */
            double r_cam, th_cam, ph_cam;
            cartToBL(centre, r_cam, th_cam, ph_cam);

            /* Cartesian to spherical */
            double n_r, n_th, n_ph;
            cartToSph(dir, th_cam, ph_cam, n_r, n_th, n_ph);

            // Add photon and walk
            RayInit ray = init_ray(r_cam, th_cam, ph_cam, n_r, n_th, n_ph, spin, M);
            return march(ray, spin, M);

        }

        void initialize() {
            // Calculate image height; ensure it's at least 1
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;

            // Camera; viewport dimensions
            centre = lookfrom;

            auto focal_length = (lookfrom - lookat).length();
            auto theta = degrees_to_radians(vfov);
            auto h = std::tan(theta/2);
            auto viewport_height = 2 * h * focal_length;
            auto viewport_width = viewport_height * (double(image_width)/image_height); // actual ratio

            // Calculate u,v,w unit basis vectors for the camera coordinate frame
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            // Calculate the vectors across horizontal and down the vertical viewport edges
            vec3 viewport_u = viewport_width * u;
            vec3 viewport_v = viewport_height * -v;

            // Calculate horizontal and vertical delta vectors from pixel to pixel
            // I think delta vectors are just the distance between pixels
            pixel_delta_u = viewport_u / image_width;    // length across / width
            pixel_delta_v = viewport_v / image_height;   // length upwawrds / height

            // Calculate location of upper left pixel (since we want to start from top to bottom)
            auto viewport_upper_left = centre - (focal_length *w) - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        }
        
        long long duration(std::chrono::high_resolution_clock::time_point a, 
                        std::chrono::high_resolution_clock::time_point b) {
            return std::chrono::duration_cast<std::chrono::milliseconds>(b - a).count();
    }

};


#endif