#include <iostream>   // for std::cout (output)
#include <vector>

int main() {
    // Image
    const int w = 256;
    const int h = 256;

    // Render
    std::cout << "P3\n" << w << ' ' << h << "\n255\n";

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            // Writing pixels out in rows; l -> r; t -> b
            auto r = double(x) / (w-1);
            auto g = double(y) / (h-1);
            auto b = 0.25;

            auto ir = int(255 * r);
            auto ig = int(255 * g);
            auto ib = int(255 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << "\n";
        }
    }
}