#include <emscripten/emscripten.h>
#include <vector>
#include "rtweekend.h"
#include "camera.h"

static camera cam;
static std::vector<unsigned char> pixels;
static int W = 0, H = 0;

extern "C" {

void EMSCRIPTEN_KEEPALIVE init(int w, int h) {
    W = w; H = h;
    pixels.resize(W * H * 4);

    cam.image_width       = W;
    cam.aspect_ratio      = double(W) / H;
    cam.samples_per_pixel = 1;

    cam.vfov              = 45;
    cam.lookfrom          = point3(-50, 0, 3);
    cam.lookat            = point3(0, 0, 0);
    cam.vup               = vec3(0, 0, 1);
    cam.spin              = 0.0;
    cam.M                 = 1.0;

    disk_r_outer = 10.0;

    load_sky("milkyway.jpg");
}

void EMSCRIPTEN_KEEPALIVE set_spin(float a) { cam.spin = a; }

void EMSCRIPTEN_KEEPALIVE render_frame() { cam.render_to(pixels); }

void EMSCRIPTEN_KEEPALIVE set_camera(float dist, float theta_deg, float phi_deg) {
    float th  = theta_deg * 3.14159265f / 180.0f;
    float phi = phi_deg *  3.14159265f / 180.0f;
    // printf("cam at (%f,%f,%f) dist=%f\n", 
    //        cam.lookfrom.x(), cam.lookfrom.y(), cam.lookfrom.z(), dist);
    cam.lookfrom = point3(dist * sin(th) * cos(phi),
                          dist * sin(th) * sin(phi),
                          dist * cos(th));
    cam.lookat = point3(0,0,0);
    cam.initialize();
}

unsigned char* EMSCRIPTEN_KEEPALIVE get_pixels() { return pixels.data(); }

int EMSCRIPTEN_KEEPALIVE get_width()    { return W; }
int EMSCRIPTEN_KEEPALIVE get_height()   { return H; }

}