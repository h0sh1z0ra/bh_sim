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

    load_sky("milkyway.jpg");
}

void EMSCRIPTEN_KEEPALIVE set_spin(float a) { cam.spin = a; }

void EMSCRIPTEN_KEEPALIVE render_frame() { cam.render_to(pixels); }

unsigned char* EMSCRIPTEN_KEEPALIVE get_pixels() { return pixels.data(); }

int EMSCRIPTEN_KEEPALIVE get_width()    { return W; }
int EMSCRIPTEN_KEEPALIVE get_height()   { return H; }

}