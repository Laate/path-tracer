#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libs/stb_image_write.h"

#include "camera.hh"
#include "scene.hh"
#include "vec3.hh"

auto main() -> int
{
    constexpr int color_channels = 3;
    constexpr int width = 1280;
    constexpr int height = 720;
    constexpr float ratio = static_cast<float>(width) / height;

    std::vector<uint8_t> pixels(height * width * color_channels);
    Camera test_camera = Camera(Vec3(0, 0, 15), Vec3(0, 1, 0), Vec3(0, 0, -1), 75);
    Scene test_scene = Scenes::RandomTriangles(1500);

    auto start_time = std::chrono::system_clock::now();
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            // Transform the pixel coordinates to image plane [-1, 1]^2 coordinates
            float img_plane_x = (2 * (static_cast<float>(x) / width) - 1) * ratio;
            float img_plane_y = -2 * (static_cast<float>(y) / height) + 1;
            Ray ray = test_camera.GetRay(img_plane_x, img_plane_y);
            Hit hit = Hit();
            if (test_scene.Intersect(ray, hit))
            {
                pixels[y * width * color_channels + x * color_channels] = 255;
            }
        }
        int percent = ceil(static_cast<float>(100 * y) / height);
        std::cout << "\033[2K\r"
                  << "[" << std::string(percent / 2, '=')
                  << std::string(50 - (percent) / 2, ' ') << "] " << percent
                  << "%" << std::flush;
    }
    auto end_time = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    std::cout
        << "\nRendering complete\n"
        << "Width " << width << ", "
        << "Height " << height << "\n"
        << "Time: " << elapsed << " ms\n"
        << static_cast<float>(elapsed) / (height * width)
        << " ms per iteration"
        << std::endl;

    stbi_write_png("output.png", width, height, color_channels, pixels.data(), width * color_channels);

    return 1;
}
